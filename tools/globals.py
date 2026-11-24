"""Names for the game's globals (phase 3, step 1).

The rewrite still keeps the game's global data where RT3.exe keeps it. This
tool gives every such global one name, listed in tools/globals.tsv (address,
name, type, note), and generates src/core/globals.h, where each name is a
macro for exactly the old access:

    #define g_world (::hook::Global<uint8_t*>(0x6cec78))

so the compiled code does not change. A use with another type than the
listed one keeps its exact meaning through hook::As<T>(g_name). Moving a
global into our own code later (step 2) replaces its macro by a variable of
the same name; the call sites stay.

    python tools/globals.py init     # build globals.tsv from the sources (once)
    python tools/globals.py apply    # write globals.h, rewrite the sources

Renaming: edit the name in globals.tsv and run apply again; uses of the old
name are renamed too. Placeholder names (g_6cec78 style) mark globals whose
purpose is not yet established.
"""
import collections
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TSV = ROOT / "tools" / "globals.tsv"
HEADER = ROOT / "src" / "core" / "globals.h"

ACCESS = re.compile(r"hook::Global<([^()]*?)>\(\s*0x([0-9a-fA-F]+)\s*\)")
# Types the header can name: built-ins (and pointers to them) and the game's structs.
BUILTIN = {"void", "char", "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t",
           "int64_t", "uint64_t", "float", "double", "bool", "const char", "const uint32_t", "const uint8_t"}
STRUCTS = {"Collection", "HashMap", "StringList", "PtrList", "MaxHeap", "MaxList"}

# Names established from the code and its comments (constructors, what the
# comments say the address holds). Everything else gets a placeholder.
NAMES = {
    0x6cec78: ("g_world", "the running game's world (date, rules, players' state)"),
    0x6cec74: ("g_app", "application object from WinMain: current screen +8, editor +0x68"),
    0x6cec7c: ("g_newGame", "new-game / lobby setup"),
    0x6d4024: ("g_engine", "the engine (display, screen modes, settings)"),
    0x6d4020: ("g_pack", "the open data pack (name table)"),
    0x6d4018: ("g_input", "input"),
    0x6d401c: ("g_windowManager", "window manager (handle table)"),
    0x6d402c: ("g_sound", "sound system"),
    0x6d4030: ("g_models", "model / particle pool"),
    0x62c120: ("g_map", "the map (terrain, cells, view distance)"),
    0x6cfca8: ("g_tracks", "track pieces"),
    0x62be10: ("g_companies", "companies"),
    0x6ceb9c: ("g_players", "players"),
    0x6cfcbc: ("g_trains", "trains"),
    0x62b26c: ("g_buildings", "placed buildings"),
    0x62bae0: ("g_cities", "cities"),
    0x6cec20: ("g_stations", "stations"),
    0x6cfc9c: ("g_territories", "territories"),
    0x62ba8c: ("g_cargoTypes", "cargo types"),
    0x62ba88: ("g_carTypes", "car types (freight / passenger class +0x388)"),
    0x62b2fc: ("g_buildingTypes", "building types (.bty)"),
    0x62be18: ("g_events", "event records"),
    0x6cfcb4: ("g_routes", "routes"),
    0x6acd34: ("g_messageLog", "message / news log"),
    0x6cd8d8: ("g_session", "network session"),
    0xccbb20: ("g_cursorManager", "cursor manager"),
    0x6d0818: ("g_gameWindow", "the game window (selected company +0x90)"),
    0x6d16d8: ("g_stationWindow", "station detail window"),
    0x6cff84: ("g_cityDialog", "city dialog"),
    0x6cff74: ("g_selectedCity", "selected / edited city"),
    0x6d1350: ("g_optionsWindow", "options window"),
    0x6d3c84: ("g_language", "language"),
    0x624324: ("g_lcg", "shared LCG random state"),
    0x62431c: ("g_seed", "SimRand seed"),
    0x624320: ("g_seedSaved", "saved SimRand seed"),
    0x6d3b94: ("g_seedDepth", "seed stack depth"),
    0xccb9e8: ("g_skipCount", "skipped stream reads"),
}


def norm_type(t):
    t = re.sub(r"\s+", " ", t.strip())
    return re.sub(r"\s*\*\s*", "*", t).replace("*", "*")


def nameable(t):
    base = t.rstrip("*")
    return base in BUILTIN or base in STRUCTS


def header_type(t):
    base = t.rstrip("*")
    stars = t[len(base):]
    return ("::rt3::" + base if base in STRUCTS else base) + stars


def sources():
    return sorted(p for p in (ROOT / "src").rglob("*.cpp") if "launcher" not in p.parts)


def init():
    uses = collections.defaultdict(collections.Counter)
    for p in sources():
        for m in ACCESS.finditer(p.read_text(encoding="utf-8-sig")):
            uses[int(m.group(2), 16)][norm_type(m.group(1))] += 1
    rows = []
    for a in sorted(uses):
        types = [t for t, _ in uses[a].most_common() if nameable(t)]
        t = types[0] if types else "void*"
        name, note = NAMES.get(a, ("g_%06x" % a, ""))
        rows.append("%06x\t%s\t%s\t%s" % (a, name, t, note))
    TSV.write_text("address\tname\ttype\tnote\n" + "\n".join(rows) + "\n", encoding="utf-8")
    print("wrote %d globals to %s" % (len(rows), TSV.relative_to(ROOT)))


def load():
    table = {}
    for line in TSV.read_text(encoding="utf-8").splitlines()[1:]:
        if line.strip():
            a, name, t, note = (line.split("\t") + [""])[:4]
            table[int(a, 16)] = (name, t, note)
    names = [n for n, _, _ in table.values()]
    dup = [n for n, c in collections.Counter(names).items() if c > 1]
    if dup:
        sys.exit("duplicate names in globals.tsv: %s" % dup)
    return table


def old_names():
    """Name -> address in the current header (to rename after a table edit)."""
    if not HEADER.exists():
        return {}
    return {m.group(1): int(m.group(2), 16)
            for m in re.finditer(r"#define (\w+) \(::hook::Global<.*?>\(0x([0-9a-f]+)\)\)",
                                 HEADER.read_text(encoding="utf-8"))}


def write_header(table):
    lines = ["// Generated by tools/globals.py from tools/globals.tsv: do not edit.",
             "//",
             "// The game's globals, still at their addresses in RT3.exe's image (phase 3,",
             "// step 1). Each name is exactly the old hook::Global access; step 2 replaces",
             "// a name by a variable of our own. Placeholder names (g_6cec78) mark globals",
             "// whose purpose is not established yet.",
             "#pragma once",
             "#include <stdint.h>",
             '#include "hook/hook.h"',
             "",
             "namespace rt3 { struct " + "; struct ".join(sorted(STRUCTS)) + "; }",
             ""]
    for a in sorted(table):
        name, t, note = table[a]
        line = "#define %s (::hook::Global<%s>(0x%06x))" % (name, header_type(t), a)
        lines.append(line + ("  // " + note if note else ""))
    HEADER.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def apply():
    table = load()
    by_addr = {a: (n, t) for a, (n, t, _) in table.items()}
    renamed = {old: by_addr[a][0] for old, a in old_names().items() if a in by_addr and by_addr[a][0] != old}
    write_header(table)
    alias = re.compile(r"^([ \t]*)(?:static\s+)?([\w:]+(?:\s*\*)*)\s*&\s*(\w+)\s*=\s*hook::Global<([^()]*?)>\(\s*0x([0-9a-fA-F]+)\s*\)\s*;[ \t]*\r?\n",
                       re.M)
    changed = 0
    for p in sources():
        raw = p.read_bytes().decode("utf-8")
        bom = raw.startswith("﻿")
        text = raw[1:] if bom else raw
        new = text
        # Globals' aliases (g_*): one name per global, so drop the alias and use the table's name.
        renames = {}
        def drop(m):
            t, name, a = m.group(2), m.group(3), int(m.group(5), 16)
            if a not in by_addr or not name.startswith("g_"):
                return m.group(0)
            gname, gt = by_addr[a]
            # The alias goes; its uses become the table's name (as the alias's type).
            renames[name] = gname if norm_type(t) == gt else "hook::As<%s>(%s)" % (t.strip(), gname)
            return ""
        new = alias.sub(drop, new)
        mapping = dict(renamed)
        mapping.update(renames)
        if mapping:  # all at once, so one rename never feeds another
            new = re.sub(r"(?<![\w.>])(%s)\b" % "|".join(map(re.escape, mapping)), lambda m: mapping[m.group(1)], new)

        def access(m):
            t, a = norm_type(m.group(1)), int(m.group(2), 16)
            if a not in by_addr:
                return m.group(0)
            gname, gt = by_addr[a]
            return gname if t == gt else "hook::As<%s>(%s)" % (m.group(1).strip(), gname)
        new = ACCESS.sub(access, new)
        if new != text:
            if '#include "core/globals.h"' not in new:
                new = re.sub(r'(#include "hook/hook.h"[ \t]*\r?\n)', r'\1#include "core/globals.h"\n', new, count=1)
                if '#include "core/globals.h"' not in new:
                    new = '#include "core/globals.h"\n' + new
            p.write_bytes((("﻿" if bom else "") + new).encode("utf-8"))
            changed += 1
    left = sum(len(ACCESS.findall(p.read_text(encoding="utf-8-sig"))) for p in sources())
    print("header: %d globals; rewrote %d files; literal hook::Global accesses left: %d" % (len(table), changed, left))
    return 0


if __name__ == "__main__":
    {"init": init, "apply": apply}[sys.argv[1] if len(sys.argv) > 1 else "apply"]()
