"""Scripted calls for rewritten functions that no game check has run yet.

    python tools/autocall.py [--coverage=docs/simcheck_coverage.txt] [--out=out/autocall] [--chunk=40]
                             [--skip=out/autocall/skip.txt] [--only=Name,...]

Reads which functions ran in a game check (docs/simcheck_coverage.txt: "addr name
runs"), finds the definition of every function that never ran in src/, and writes
"call" actions for it (tools/simcheck.py parse_actions, src/harness.cpp
CallFunction) with arguments from the loaded save: the object or collection its
name prefix names (Train_ -> the trains, Company_ -> the companies, ...), ids of
real objects for parameters named like one, small numbers, scratch buffers and
strings for the rest. Functions it cannot build arguments for are listed in
<out>/unbuilt.txt with the reason. The calls go into <out>/chunk_NNN.txt files of
--chunk calls each, to be run with tools/autocall_run.py (simcheck on a save: the
original run calls the original, the rewrite run the rewrite, and the results,
the saved game and the drawing are compared). <out>/skip.txt (addresses, one per
line, # comments) lists calls left out on purpose (a call that ends the game in
both runs, found by autocall_run.py).
"""
import re
import sys
from pathlib import Path

from replacements import REPLACEMENT_RE

ROOT = Path(__file__).resolve().parents[1]

# Name prefix -> what its first (object) parameter is: ("coll", global) for an
# object of a collection, ("glob", global) for a singleton held in a global.
PREFIX = {
    "Train": ("coll", 0x6cfcbc), "Trains": ("coll", 0x6cfcbc), "TrainWindow": None,
    "Station": ("coll", 0x6cec20), "Stations": ("coll", 0x6cec20),
    "Company": ("coll", 0x62be10), "Companies": ("coll", 0x62be10), "CompanyCmd": ("coll", 0x62be10),
    "Player": ("coll", 0x6ceb9c), "Players": ("coll", 0x6ceb9c),
    "Car": ("coll", 0x62ba84), "Cars": ("coll", 0x62ba84),
    "Track": ("coll", 0x6cfca8), "Tracks": ("coll", 0x6cfca8), "TrackNet": ("coll", 0x6cfca8),
    "Node": ("coll", 0x6cfca8), "Grid": ("coll", 0x6cfca8),
    "City": ("coll", 0x62bae0), "Cities": ("coll", 0x62bae0),
    "Building": ("coll", 0x62b26c), "Buildings": ("coll", 0x62b26c), "Site": ("coll", 0x62b26c),
    "Industry": ("coll", 0x62b268), "IndustryTypes": ("coll", 0x62b268),
    "Cargo": ("coll", 0x62ba8c), "CargoTypes": ("coll", 0x62ba8c),
    "Event": ("coll", 0x62be18), "Events": ("coll", 0x62be18),
    "World": ("glob", 0x6cec78), "Map": ("glob", 0x62c120), "Terrain": ("glob", 0x62c120),
    "Camera": ("glob", 0x62be68), "View": ("glob", 0x62be68),
    "App": ("glob", 0x6cec74), "Options": ("glob", 0x6cec74),
    "Engine": ("glob", 0x6d4024), "Cursor": ("glob", 0xccbb20),
}
# Generic prefixes (Obj_, Table_ ...): the object of the file's subject.
FILE_SUBJECT = {
    "Train.cpp": ("coll", 0x6cfcbc), "Trains.cpp": ("coll", 0x6cfcbc), "Cars.cpp": ("coll", 0x62ba84),
    "Company.cpp": ("coll", 0x62be10), "Stations.cpp": ("coll", 0x6cec20), "Cities.cpp": ("coll", 0x62bae0),
    "Buildings.cpp": ("coll", 0x62b26c), "Sites.cpp": ("coll", 0x62b26c), "Track.cpp": ("coll", 0x6cfca8),
    "TrackNet.cpp": ("coll", 0x6cfca8), "Map.cpp": ("glob", 0x62c120), "World.cpp": ("glob", 0x6cec78),
    "Players.cpp": ("coll", 0x6ceb9c), "Player.cpp": ("coll", 0x6ceb9c),
}
# Collections by the parameter names that hold an object id of theirs.
ID_NAMES = {
    "company": 0x62be10, "co": 0x62be10, "player": 0x6ceb9c, "station": 0x6cec20, "st": 0x6cec20,
    "train": 0x6cfcbc, "car": 0x62ba84, "city": 0x62bae0, "node": 0x6cfca8, "piece": 0x6cfca8,
    "track": 0x6cfca8, "cargo": 0x62ba8c, "building": 0x62b26c, "site": 0x62b26c, "industry": 0x62b268,
}
# Prefixes of functions that need a running multiplayer session, a window or a
# device the save run does not have; the two-player checks and the window
# scripts cover those instead.
NO_CALL = ("Net", "NetMsg", "Lobby", "Movie", "Window", "Widget", "Dialog", "Tooltip", "Picker",
           "Hotkey", "Return0", "Return1")
# Names of functions that open windows or message boxes, or touch files, sound or
# textures: a blind call stalls the game on a modal box or fails on a missing
# file, so these are left to the window / dialog scripts.
UI_RE = re.compile(r"Open|Show|Load|Save|File|Texture|Model|Message|Prompt|Ask|Confirm|Popup|Menu|Play|Sound|Music|"
                   r"Font|Exit|Quit|Debug|Movie|Shell|Print|Dialog|Window|Screen|Render|Device|Video")

DEF_RE = re.compile(r"^(?P<head>[^\n;{}()#/]*?\b(?P<name>\w+))\s*\((?P<params>[^()]*)\)\s*(?:const\s*)?\{", re.MULTILINE)


def definitions():
    """name -> (file, return type, convention, [(type, name)])"""
    out = {}
    for p in sorted((ROOT / "src").rglob("*.cpp")):
        text = p.read_text(encoding="utf-8", errors="replace")
        for m in DEF_RE.finditer(text):
            head, name = m.group("head"), m.group("name")
            if name in ("if", "for", "while", "switch", "return"):
                continue
            words = head.replace("RT3_NOINLINE", "").replace("static", "").replace("inline", "").split()
            cc = next((w for w in words if w.startswith("__")), "")
            ret = " ".join(w for w in words[:-1] if not w.startswith("__"))
            params = []
            for raw in m.group("params").split(","):
                raw = raw.strip()
                if not raw or raw == "void":
                    continue
                mm = re.match(r"^(.*?)(\w+)?$", raw)
                typ, pname = (raw, "") if not mm.group(2) or mm.group(1).strip() in ("", "const") else (mm.group(1).strip(), mm.group(2))
                params.append((typ, pname))
            out.setdefault(name, (p.relative_to(ROOT).as_posix(), ret, cc, params))
    return out


def prefix_of(name):
    return name.split("_")[0]


# First-parameter names that hold the object the function works on (anything
# else, like "cells" or "out", is a buffer).
OBJECT_NAMES = {"t", "o", "c", "m", "w", "s", "e", "b", "p", "g", "car", "train", "company", "co", "player", "st",
                "station", "map", "world", "game", "app", "obj", "n", "node", "city", "tbl", "table", "coll", "self",
                "v", "cam", "view", "r", "rec", "tracks", "net", "trains", "cars", "players", "companies", "stations"}


def object_param(typ, pname):
    t = typ.replace("const ", "").strip()
    return t in ("uint8_t*", "void*", "Collection*", "int32_t*", "uint32_t*") and pname in OBJECT_NAMES


def arg_for(fn, i, typ, pname, first_obj):
    """One call token, or None when it cannot be built."""
    t = typ.replace("const ", "").strip()
    low = pname.lower()
    if t in ("int", "int32_t", "uint32_t", "uint16_t", "uint8_t", "int16_t", "int8_t", "unsigned", "char", "bool", "long", "DWORD"):
        if i == 1 and t == "int" and not pname:
            return "n0"  # the edx slot of a thiscall
        for key, coll in ID_NAMES.items():
            if low == key or low == key + "id" or low.startswith(key + "_"):
                return f"i{coll:x}.0"
        if low in ("on", "flag", "enable", "show", "force", "quiet", "announce"):
            return "n1"
        return "n0"
    if t in ("float",):
        return "f1"
    if t in ("double",):
        return "d1"
    if t in ("char*", "char *"):
        return "b0"
    if t in ("char*", "const char*") or typ.startswith("const char"):
        return "sx"
    if t.endswith("*"):
        if i == 0 and first_obj and object_param(typ, pname):
            return first_obj
        if "Collection" in t:
            pre = PREFIX.get(prefix_of(fn))
            return f"g{pre[1]:x}" if pre and pre[0] == "coll" else None
        return "b1" if i % 2 else "b2"
    return None


def build(fn, d):
    file, ret, cc, params = d
    pre = PREFIX.get(prefix_of(fn), None) or FILE_SUBJECT.get(Path(file).name)
    first_obj = None
    if pre:
        first_obj = f"o{pre[1]:x}.0" if pre[0] == "coll" else f"g{pre[1]:x}"
    if params and params[0][0].replace("const ", "").strip().startswith("Collection") and pre and pre[0] == "coll":
        first_obj = f"g{pre[1]:x}"
    tokens = []
    for i, (typ, pname) in enumerate(params):
        a = arg_for(fn, i, typ, pname, first_obj)
        if a is None:
            return None, f"parameter {i} '{typ} {pname}'"
        tokens.append(a)
    # Every pointer parameter gets a buffer of its own (shared out-buffers would
    # make the result depend on the order of the stores).
    order = iter(["b4", "b5", "b6", "b7", "b1", "b2", "b3"] * 4)
    tokens = [next(order) if t in ("b1", "b2") else t for t in tokens]
    if params and object_param(*params[0]) and not first_obj:
        return None, f"no object for '{params[0][0]} {params[0][1]}' (prefix {prefix_of(fn)})"
    r = "v" if ret.strip() == "void" else "f" if ret.strip() in ("float", "double") else "i"
    conv = "f" if cc == "__fastcall" else "s"
    return f"call {{addr}} {conv} {r} " + " ".join(tokens), None


def main():
    cov = Path(next((a[11:] for a in sys.argv if a.startswith("--coverage=")), ROOT / "docs" / "simcheck_coverage.txt"))
    out = Path(next((a[6:] for a in sys.argv if a.startswith("--out=")), ROOT / "out" / "autocall"))
    chunk = int(next((a[8:] for a in sys.argv if a.startswith("--chunk=")), 40))
    only = {n for a in sys.argv if a.startswith("--only=") for n in a[7:].split(",")}
    out.mkdir(parents=True, exist_ok=True)
    skip_file = Path(next((a[7:] for a in sys.argv if a.startswith("--skip=")), out / "skip.txt"))
    skip = set()
    if skip_file.exists():
        skip = {int(l.split()[0], 16) for l in skip_file.read_text().splitlines() if l.strip() and not l.startswith("#")}
    todo = []
    for line in cov.read_text().splitlines():
        a, name, runs = line.split()
        if int(runs) == 0 and (not only or name in only):
            todo.append((int(a, 16), name))
    defs = definitions()
    calls, unbuilt = [], []
    for addr, name in todo:
        if addr in skip:
            continue
        if name.startswith(NO_CALL):
            unbuilt.append(f"{addr:06x} {name}: needs a session, window or device (other checks)")
            continue
        if UI_RE.search(name):
            unbuilt.append(f"{addr:06x} {name}: opens UI or touches files / sound / textures (window scripts)")
            continue
        d = defs.get(name)
        if not d:
            unbuilt.append(f"{addr:06x} {name}: definition not found")
            continue
        spec, why = build(name, d)
        if not spec:
            unbuilt.append(f"{addr:06x} {name}: {why}")
            continue
        calls.append(f"{spec.format(addr=f'{addr:x}')}   # {name}")
    for old in out.glob("chunk_*.txt"):
        old.unlink()
    for k in range(0, len(calls), chunk):
        part = calls[k:k + chunk]
        body = [f"{30 + j} {c}" for j, c in enumerate(part)]
        (out / f"chunk_{k // chunk:03d}.txt").write_text(
            "# Generated by tools/autocall.py: calls of functions no check ran yet.\n" + "\n".join(body) + "\n")
    (out / "unbuilt.txt").write_text("\n".join(unbuilt) + "\n")
    print(f"{len(todo)} never ran; {len(calls)} calls in {(len(calls) + chunk - 1) // chunk} chunks; {len(unbuilt)} not built "
          f"({out / 'unbuilt.txt'})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
