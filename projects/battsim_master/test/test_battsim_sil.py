"""Host test of the components of battsim_master (software in the loop, no hardware).

Generates the RTE of the project with tools/rtegen of the pinned neXtBSW tree into a temporary folder, compiles
the C test (battsim_sil_test.c) together with the generated model code of SupplyMonitor, BattSimMode, CellCal and CellChannels, the RTE and the
CAN database code for the PC with zig cc, and runs it. Started by the test list of the workspace (ci/tests.yaml)
from the workspace folder.
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

WORKSPACE = Path(__file__).resolve().parents[3]
TREE = WORKSPACE / "nextbsw"
PROJECT = WORKSPACE / "projects" / "battsim_master"
COMPONENTS = ["SupplyMonitor", "BattSimMode", "CellCal", "CellChannels"]
E2E = TREE / "libs" / "bsw" / "e2e"
ALIVE_CONFIG = re.compile(
    r"E2E_AliveConfigType const (\w+)_ALIVE_CONFIG = \{/\* CounterBits \*/ (\d+)U, /\* MaxDeltaCounter \*/ (\d+)U, "
    r"/\* CounterMin \*/ (\d+)U, /\* CounterMax \*/ (\d+)U\};")


def run(argv: list[str], **kwargs) -> subprocess.CompletedProcess:
    print("$", " ".join(str(a) for a in argv), flush=True)
    return subprocess.run([str(a) for a in argv], text=True, capture_output=True, **kwargs)


def alive_defines(rte: Path) -> list[str] | None:
    """-D options with the alive configuration of the control messages from the generated RteConfig.cpp; None if the
    nine control messages do not share one (the test checks one configuration for all)."""
    configs = ALIVE_CONFIG.findall((rte / "RteConfig.cpp").read_text(encoding="utf-8"))
    if len(configs) != 9 or len({c[1:] for c in configs}) != 1:
        print(f"alive configurations of the control messages: {configs}")
        return None
    _, bits, delta, low, high = configs[0]
    return [f"-DALIVE_COUNTER_BITS={bits}U", f"-DALIVE_MAX_DELTA={delta}U", f"-DALIVE_COUNTER_MIN={low}U",
            f"-DALIVE_COUNTER_MAX={high}U"]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("test_source", nargs="?", default=str(PROJECT / "test" / "battsim_sil_test.c"),
                        help="C file of the test (default: battsim_sil_test.c next to this script)")
    test_source = Path(parser.parse_args().test_source).resolve()
    with tempfile.TemporaryDirectory(prefix="battsim_sil_") as tmp:
        tmp_dir = Path(tmp)
        rte = tmp_dir / "rte"
        result = run([sys.executable, TREE / "tools" / "rtegen" / "rtegen.py", PROJECT / "project.yaml", rte,
                      "--repo", TREE])
        if result.returncode != 0:
            print(result.stdout, result.stderr)
            print("rtegen failed")
            return 1
        exe = tmp_dir / "battsim_sil.exe"
        sources = []
        for name in COMPONENTS:      # <Swc>.c and, where the model has constant block outputs, <Swc>_data.c
            sources += sorted((PROJECT / "swc" / name / "generated" / "src").glob("*.c"))
        sources += [rte / "Rte.c", rte / "CanDb.c", E2E / "src" / "e2e" / "E2E.c", E2E / "src" / "e2e" / "E2E_Alive.c",
                    test_source]
        defines = alive_defines(rte)
        if defines is None:
            print("alive configuration differs between the control messages")
            return 1
        includes = [TREE / "libs" / "bsw" / "stdBase" / "include", E2E / "include", rte]
        includes += [PROJECT / "swc" / name / "generated" / "src" for name in COMPONENTS]
        argv = [sys.executable, "-m", "ziglang", "cc", "-std=c99", "-O1", "-Wall", "-Wextra", "-Werror",
                "-Wno-unused-function"]
        argv += defines + [f"-I{path}" for path in includes]
        argv += [str(path) for path in sources] + ["-lm", "-o", exe]
        result = run(argv)
        if result.returncode != 0:
            print(result.stdout, result.stderr)
            print("compile failed")
            return 1
        result = run([exe])
        print(result.stdout, result.stderr)
        return result.returncode


if __name__ == "__main__":
    sys.exit(main())
