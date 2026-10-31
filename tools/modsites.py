"""Flags rewritten functions that contain a call site DesyncWatch (the game folder's
DINPUT8.dll) patches, unless the rewrite calls through hook::CallTarget(site).
Calling the callee directly would bypass the mod's hook (e.g. the MP preview-ID fix).

    python tools/modsites.py
"""
import bisect
import glob
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DSW = ROOT.parent / "reversing_notes" / "tools" / "desyncwatch" / "desyncwatch.c"


def main():
    sites = [int(m.group(1), 16) for m in
             re.finditer(r"\{ 0x([0-9A-Fa-f]+), 0x[0-9A-Fa-f]+, \(void \*\)Hook_", DSW.read_text(errors="replace"))]
    idx = sorted((int(l.split("\t")[0], 16), int(l.split("\t")[2]))
                 for l in (ROOT / "decomp" / "index.tsv").read_text().splitlines())
    starts = [a for a, _ in idx]
    rep = {}
    for f in glob.glob(str(ROOT / "src" / "**" / "*.cpp"), recursive=True):
        text = open(f, encoding="utf-8", errors="replace").read()
        for m in re.finditer(r"RT3_REPLACE\(0x([0-9a-fA-F]+),\s*(\w+)", text):
            rep[int(m.group(1), 16)] = (m.group(2), f, text)
    bad = 0
    for s in sorted(set(sites)):
        i = bisect.bisect_right(starts, s) - 1
        a, size = idx[i]
        if not (a <= s < a + size) or a not in rep:
            continue
        name, f, text = rep[a]
        if not re.search(r"CallTarget<[^;]*>\(0x0*%x\)" % s, text, re.I):
            print(f"{os.path.basename(f)}: {name} (0x{a:x}) bypasses the DesyncWatch hook at call site 0x{s:x}")
            bad += 1
    print(f"modsites: {bad} rewrites bypass a DesyncWatch call-site hook")
    # Code DesyncWatch patches inside exe functions (desyncwatch.c WriteCode / the
    # option blocks). A rewrite containing one must follow the live bytes and say
    # so by naming the address in its source (beyond its RT3_REPLACE line).
    code_sites = [0x51F71B, 0x476A60, 0x4B25FB, 0x4B261F, 0x4C6E4A, 0x434DB0,
                  0x4392D6, 0x4392F7, 0x439309, 0x549949, 0x549950, 0x5A4066]
    known = {0x476A60}  # World_AnyCondition: always_new_company lost while rt3re is loaded (open)
    unack = 0
    for s in code_sites:
        i = bisect.bisect_right(starts, s) - 1
        a, size = idx[i]
        if not (a <= s < a + size) or a not in rep:
            continue
        name, f, text = rep[a]
        # Header comments name the function's own address, so the mention must sit
        # near the word DesyncWatch.
        if any("DesyncWatch" in text[max(0, m.start() - 600):m.end() + 600]
               for m in re.finditer(r"0x0*%x\b" % s, text, re.I)):
            continue
        if s in known:
            print(f"modsites: known conflict: {name} (0x{a:x}) replaces DesyncWatch-patched code at 0x{s:x}")
            continue
        print(f"{os.path.basename(f)}: {name} (0x{a:x}) contains DesyncWatch-patched code at 0x{s:x}; follow the live bytes")
        unack += 1
    print(f"modsites: {unack} rewrites ignore DesyncWatch code patches")
    return 1 if bad or unack else 0


if __name__ == "__main__":
    sys.exit(main())
