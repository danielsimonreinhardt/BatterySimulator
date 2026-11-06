"""Liest eine Simulink-Datei (.slx) ohne MATLAB und schreibt sie als lesbaren Text.

Eine .slx ist ein ZIP mit XML-Dateien: simulink/systems/system_<SID>.xml je Subsystem,
simulink/stateflow/chart_<id>.xml je Stateflow-Diagramm. Dieses Werkzeug baut daraus

- system_tree.txt  : Hierarchie der Subsysteme (Pfad, SID, Blockanzahl, Abtastzeit)
- systems.txt      : je Subsystem alle Bloecke mit wichtigen Parametern und alle Verbindungen
                     (Quelle -> Ziel mit Signalnamen)
- stateflow.txt    : je Diagramm Zustaende (mit Aktionen), Uebergaenge (Bedingungen) und Daten

Aufruf:  python tools/slx_dump.py <modell.slx> <ausgabeordner>
Die Quelle wird nur gelesen.
"""
from __future__ import annotations

import sys
import zipfile
import xml.etree.ElementTree as ET
from pathlib import Path

# Parameter, die nur Layout beschreiben und beim Lesen stoeren.
SKIP_PARAMS = {
    "Position", "ZOrder", "Location", "Open", "ZoomFactor", "ReportName", "SIDHighWatermark",
    "LibraryVersion", "Ports", "BackgroundColor", "ForegroundColor", "FontSize", "FontName",
    "ShowName", "HideAutomaticName", "NamePlacement", "BlockMirror", "BlockRotation", "DropShadow",
    "IconShape", "Points", "Labels", "MaskIconFrame", "MaskIconOpaque", "MaskIconUnits",
    "ContentPreviewEnabled", "TreatAsGroupedWhenPropagatingVariantConditions", "MaskDisplay",
    "MaskIconRotate", "SystemRect", "ScreenColor", "PaperOrientation", "PaperPositionMode",
    "PaperType", "PaperUnits", "TiledPaperMargins", "TiledPageScale", "ShowPageBoundaries",
    "ZoomFactor", "SimulinkSubDomain", "MultithreadedSim",
}


def text(el: ET.Element | None) -> str:
    return "" if el is None or el.text is None else el.text


def one_line(value: str) -> str:
    return " ".join(value.split())


def params(el: ET.Element) -> dict[str, str]:
    out: dict[str, str] = {}
    for p in el.findall("P"):
        name = p.get("Name", "")
        if name and name not in SKIP_PARAMS:
            out[name] = text(p)
    inst = el.find("InstanceData")
    if inst is not None:
        for p in inst.findall("P"):
            name = p.get("Name", "")
            if name and name not in SKIP_PARAMS:
                out["." + name] = text(p)
    mask = el.find("Mask")
    if mask is not None:
        for mp in mask.iter("MaskParameter"):
            name = mp.get("Name", "")
            val = mp.find("Value")
            if name and val is not None:
                out["~" + name] = text(val)
    return out


class Model:
    def __init__(self, path: Path) -> None:
        self.zip = zipfile.ZipFile(path)
        self.systems: dict[str, ET.Element] = {}
        for name in self.zip.namelist():
            if name.startswith("simulink/systems/system_") and name.endswith(".xml"):
                sid = name.rsplit("/", 1)[1][len("system_"):-4]
                self.systems[sid] = ET.fromstring(self.zip.read(name))
        self.charts: list[ET.Element] = [
            ET.fromstring(self.zip.read(n)) for n in sorted(self.zip.namelist())
            if n.startswith("simulink/stateflow/chart_") and n.endswith(".xml")
        ]
        # Pfad je Subsystem ueber die Block-Referenzen <System Ref="system_X"/>.
        self.path: dict[str, str] = {"root": ""}
        self.sample: dict[str, str] = {}
        self._walk("root", "")

    def _walk(self, sid: str, prefix: str) -> None:
        sys_el = self.systems.get(sid)
        if sys_el is None:
            return
        for blk in sys_el.findall("Block"):
            ref = blk.find("System")
            if ref is None:
                continue
            child = ref.get("Ref", "").replace("system_", "")
            name = one_line(blk.get("Name", ""))
            child_path = f"{prefix}/{name}" if prefix else name
            self.path[child] = child_path
            for p in blk.findall("P"):
                if p.get("Name") == "SystemSampleTime":
                    self.sample[child] = text(p)
            self._walk(child, child_path)

    def block_names(self, sid: str) -> dict[str, str]:
        sys_el = self.systems[sid]
        return {b.get("SID", ""): one_line(b.get("Name", "")) for b in sys_el.findall("Block")}

    def dump_tree(self) -> str:
        lines = []
        for sid, path in sorted(self.path.items(), key=lambda kv: kv[1]):
            depth = path.count("/") + (1 if path else 0)
            n = len(self.systems.get(sid, ET.Element("x")).findall("Block"))
            st = f"  Ts={self.sample[sid]}" if sid in self.sample else ""
            label = path.rsplit("/", 1)[-1] if path else "<root>"
            lines.append(f"{'  ' * depth}{label}  [system_{sid}, {n} Bloecke]{st}")
        return "\n".join(lines) + "\n"

    def dump_systems(self) -> str:
        out = []
        for sid, path in sorted(self.path.items(), key=lambda kv: kv[1]):
            sys_el = self.systems.get(sid)
            if sys_el is None:
                continue
            out.append("=" * 100)
            out.append(f"SYSTEM {path or '<root>'}  (system_{sid})")
            names = self.block_names(sid)
            for blk in sys_el.findall("Block"):
                btype = blk.get("BlockType", "")
                name = one_line(blk.get("Name", ""))
                p = params(blk)
                src = one_line(p.pop("SourceBlock", "")) if btype == "Reference" else ""
                head = f"  [{blk.get('SID')}] {btype}{' <' + src + '>' if src else ''} \"{name}\""
                out.append(head)
                for k, v in p.items():
                    v1 = one_line(v)
                    if len(v1) > 300:
                        v1 = v1[:300] + " ..."
                    out.append(f"        {k} = {v1}")
            out.append("  -- Verbindungen --")
            for line in sys_el.findall("Line"):
                out.extend(self._line(line, names, "  "))
        return "\n".join(out) + "\n"

    def _line(self, line: ET.Element, names: dict[str, str], indent: str, src: str = "") -> list[str]:
        p = {x.get("Name"): text(x) for x in line.findall("P")}
        sig = p.get("Name", "")
        src = p.get("Src", src)
        res = []
        if "Dst" in p:
            res.append(f"{indent}{self._port(src, names)} -> {self._port(p['Dst'], names)}"
                       f"{'  {' + one_line(sig) + '}' if sig else ''}")
        for br in line.findall("Branch"):
            bp = {x.get("Name"): text(x) for x in br.findall("P")}
            bsig = bp.get("Name", sig)
            if "Dst" in bp:
                res.append(f"{indent}{self._port(src, names)} -> {self._port(bp['Dst'], names)}"
                           f"{'  {' + one_line(bsig) + '}' if bsig else ''}")
            for sub in br.findall("Branch"):
                res.extend(self._line(sub, names, indent, src))
        return res

    @staticmethod
    def _port(ref: str, names: dict[str, str]) -> str:
        if "#" not in ref:
            return ref
        sid, port = ref.split("#", 1)
        return f"{names.get(sid, sid)}[{sid}].{port}"

    def dump_stateflow(self) -> str:
        out = []
        for chart in self.charts:
            cname = ""
            for p in chart.findall("P"):
                if p.get("Name") == "name":
                    cname = text(p)
            out.append("=" * 100)
            out.append(f"CHART {cname} (chart_{chart.get('id')})")
            labels: dict[str, str] = {}
            for st in chart.iter("state"):
                labels[st.get("SSID", "")] = text(st.find("P[@Name='labelString']")).split("\n", 1)[0]
            for junc in chart.iter("junction"):
                labels[junc.get("SSID", "")] = f"(J{junc.get('SSID')})"
            self._states(chart.find("Children"), out, 1, labels)
            for data in chart.iter("data"):
                scope = text(data.find("P[@Name='scope']"))
                dtype = text(data.find("props/type/P[@Name='primitive']")) or \
                    text(data.find("props/type/P[@Name='method']"))
                out.append(f"  data {data.get('name')}  scope={scope} {dtype}")
            for ev in chart.iter("event"):
                out.append(f"  event {ev.get('name')}")
        return "\n".join(out) + "\n"

    def _states(self, children: ET.Element | None, out: list[str], depth: int, labels: dict[str, str]) -> None:
        if children is None:
            return
        ind = "  " * depth
        for el in children:
            if el.tag == "state":
                label = text(el.find("P[@Name='labelString']"))
                stype = text(el.find("P[@Name='type']"))
                first, _, rest = label.partition("\n")
                out.append(f"{ind}STATE [{el.get('SSID')}] {first}  ({stype})")
                for ln in rest.splitlines():
                    if ln.strip():
                        out.append(f"{ind}    | {ln.rstrip()}")
                self._states(el.find("Children"), out, depth + 1, labels)
            elif el.tag == "transition":
                label = one_line(text(el.find("P[@Name='labelString']")))
                src = text(el.find("src/P[@Name='SSID']"))
                dst = text(el.find("dst/P[@Name='SSID']"))
                order = text(el.find("P[@Name='executionOrder']"))
                out.append(f"{ind}TRANS {labels.get(src, 'default') if src else 'default'} -> "
                           f"{labels.get(dst, dst)}  [{label}]{' #' + order if order else ''}")
            elif el.tag == "junction":
                out.append(f"{ind}JUNCTION [{el.get('SSID')}]")


def main() -> int:
    if len(sys.argv) != 3:
        print(__doc__)
        return 2
    model = Model(Path(sys.argv[1]))
    out_dir = Path(sys.argv[2])
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "system_tree.txt").write_text(model.dump_tree(), encoding="utf-8")
    (out_dir / "systems.txt").write_text(model.dump_systems(), encoding="utf-8")
    (out_dir / "stateflow.txt").write_text(model.dump_stateflow(), encoding="utf-8")
    print(f"{len(model.systems)} Subsysteme, {len(model.charts)} Diagramme -> {out_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
