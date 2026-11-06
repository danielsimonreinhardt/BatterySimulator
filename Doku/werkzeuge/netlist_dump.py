"""KiCad-XML-Netzliste als Text: je Bauteil Pins -> Netz, je Netz die Teilnehmer.
Aufruf: netlist_dump.py <net.xml> <out.txt>"""
import sys
import xml.etree.ElementTree as ET
from collections import defaultdict

root = ET.parse(sys.argv[1]).getroot()
comps = {}
for c in root.iter("comp"):
    ref = c.get("ref")
    comps[ref] = (c.findtext("value") or "", c.findtext("footprint") or "")
pins = defaultdict(dict)
nets = {}
for n in root.iter("net"):
    name = n.get("name")
    members = []
    for node in n.findall("node"):
        ref, pin = node.get("ref"), node.get("pin")
        fn = node.get("pinfunction") or ""
        pins[ref][pin] = (name, fn)
        members.append(f"{ref}.{pin}{'(' + fn + ')' if fn else ''}")
    nets[name] = members


def key(r):
    import re
    m = re.match(r"([A-Za-z#]+)(\d+)", r)
    return (m.group(1), int(m.group(2))) if m else (r, 0)


out = ["== BAUTEILE"]
for ref in sorted(comps, key=key):
    val, fp = comps[ref]
    out.append(f"{ref}  {val}  [{fp.split(':')[-1]}]")
    for pin, (net, fn) in sorted(pins[ref].items(), key=lambda kv: (len(kv[0]), kv[0])):
        out.append(f"    {pin:>3} {fn:<14} {net}")
out.append("")
out.append("== NETZE")
for name in sorted(nets):
    out.append(f"{name}: {' '.join(nets[name])}")
open(sys.argv[2], "w", encoding="utf-8").write("\n".join(out) + "\n")
print(len(comps), "Bauteile,", len(nets), "Netze")
