"""Regroups src/game/objects*.cpp into module files that follow the original
source layout.

MSVC keeps the functions of one object file together in the exe, so an
address band of RT3_mp.exe approximates one original .cpp. The two file names
the exe still contains (Misc.cpp, used at 0x51e9b3, and MouseCursor.cpp, used
at 0x53f071-0x53f473) lie inside the Misc and MouseCursor bands below.

Every registered function (and the unregistered non-static helpers, which go
with the next registered function of their old file) moves to the module of
its address. Support items it saw in its old file (static helpers, types,
macros, prototypes) are copied along; identical copies are merged, and a name
that two old files defined differently is renamed in the later one. A function
that moves away from a static that its old neighbours call loses `static`.
A function called from code of another old file gets RT3_NOINLINE: such calls
were never inlined before (no LTCG), and the difftests fake callees by
patching their entry.

  python tools/regroup.py --dry   # statistics only
  python tools/regroup.py         # write modules, delete objects*.cpp
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(__file__))
from regroup_parse import items, REPLACE  # noqa: E402

ROOT = os.path.join(os.path.dirname(__file__), "..")

GAME = [
    (0x401000, "AI"), (0x40a000, "Vehicles"), (0x40c000, "Buildings"),
    (0x414000, "Sites"), (0x41a000, "Cars"), (0x41e000, "Cargo"),
    (0x420000, "Cities"), (0x424000, "Company"), (0x42c000, "Records"),
    (0x430000, "Events"), (0x432000, "World"), (0x438000, "Camera"),
    (0x440000, "Commands"), (0x444000, "SaveGame"), (0x448000, "Map"),
    (0x454000, "SceneObjects"), (0x45e000, "Hotkeys"), (0x462000, "DevMenu"),
    (0x468000, "Network"), (0x474000, "Effects"), (0x476000, "Players"),
    (0x478000, "Minimap"), (0x47c000, "Stations"), (0x482000, "App"),
    (0x486000, "StationTypes"), (0x488000, "ScenarioText"),
    (0x48a000, "Track"), (0x494000, "TrackNet"), (0x4a6000, "Train"),
    (0x4b2000, "Trains"), (0x4b6000, "IndustryWindow"),
    (0x4be000, "CompanyWindow"), (0x4c6000, "CompanyList"),
    (0x4ca000, "EditorDialogs"), (0x4d2000, "RulesDialog"),
    (0x4dc000, "GameWindows"), (0x4e2000, "Reports"),
    (0x4ea000, "MultiplayerDialog"), (0x4f2000, "Newspaper"),
    (0x4f6000, "MapEditor"), (0x4fc000, "OptionsDialog"),
    (0x502000, "SetupScreen"), (0x504000, "StationWindow"),
    (0x50a000, "StockWindow"), (0x50c000, "TrackTool"),
    (0x510000, "TrainWindow"), (0x516000, "Tutorial"),
]
ENGINE_START = 0x517c00
ENGINE = [
    (0x517c00, "CollectionCore"), (0x51a000, "TextDate"), (0x51e000, "Misc"),
    (0x520000, "NetCore"), (0x524000, "Scene"), (0x52c000, "SceneNode"),
    (0x530000, "Sound"), (0x532000, "Terrain"), (0x538000, "EngineContainers"),
    (0x53e000, "MouseCursor"), (0x540000, "Texture"), (0x544000, "Render"),
    (0x54c000, "Fx"), (0x54e000, "Input"), (0x550000, "Font"),
    (0x554000, "Emitter"), (0x556000, "Music"), (0x558000, "Widgets"),
    (0x55c000, "SysInfo"), (0x55e000, "Controls"), (0x564000, "Utilities"),
    (0x568000, "Sound3D"),
]
BANDS = sorted(GAME + ENGINE)


def module_of(addr):
    name = BANDS[0][1]
    for start, n in BANDS:
        if addr >= start:
            name = n
    return name


def header_names():
    out = {}
    for h in glob.glob(os.path.join(ROOT, "src/*/*.h")):
        rel = os.path.relpath(h, os.path.join(ROOT, "src")).replace(os.sep, "/")
        for m in re.finditer(r"^inline\s+[\w\s\*&]+?\b(\w+)\s*\(", open(h, encoding="utf-8-sig").read(), re.M):
            out.setdefault(m.group(1), '#include "%s"' % rel)
    return out


HEADER_NAMES = header_names()


def norm(s):
    return " ".join(s.split())


WORD = re.compile(r"[A-Za-z_]\w*")


def header_of(body):
    """Text of a definition before its '{', or None for non-functions."""
    depth = 0
    for i, c in enumerate(body):
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        elif depth == 0 and c in "{;=":
            return body[:i] if c == "{" else None
    return None


def func_name(head):
    h = re.sub(r"__declspec\s*\([^)]*\)", " ", head)
    h = re.sub(r"template\s*<[^>]*>", " ", h)
    m = re.search(r"([A-Za-z_]\w*)\s*\(", h)
    return m.group(1) if m else None


def names_of(body):
    """(defined names, is_declaration_only) of a support item."""
    b = body.strip()
    if b.startswith("#include"):  # a data include (.inc): the tables it defines
        m = re.search(r'"([^"]+)"', b)
        path = os.path.join(ROOT, "src", "game", os.path.basename(m.group(1)))
        text = open(path, encoding="utf-8-sig").read() if os.path.exists(path) else ""
        return set(re.findall(r"^static const \w+\*? (\w+)", text, re.M)), False
    if b.startswith("#define"):
        return {re.match(r"#define\s+(\w+)", b).group(1)}, False
    if b.startswith("#"):
        return set(), True
    if re.match(r"namespace\s*\{", b):
        inner = b[b.index("{") + 1:b.rindex("}")]
        out = set()
        for k, a, e, c in items(inner):
            if k == "item":
                out |= names_of(inner[c:e])[0]
        return out, False
    m = re.match(r"(?:template\s*<[^>]*>\s*)?(struct|class|union|enum)\s+(?:class\s+)?(\w+)\s*(;|\{|:)", b)
    if m:
        return {m.group(2)}, m.group(3) == ";"
    if b.startswith("typedef"):
        m = re.search(r"\(\s*(?:\w+\s*)?\*\s*(\w+)\s*\)", b) or re.search(r"(\w+)\s*;$", b)
        return {m.group(1)}, False
    m = re.match(r"using\s+(\w+)\s*=", b)
    if m:
        return {m.group(1)}, False
    head = header_of(b)
    if head is not None:
        return {func_name(head)} - {None}, False
    pre = b.split("=")[0]
    if "(" in pre and "=" not in b.split("(")[0]:
        n = func_name(pre)  # prototype
        return {n} - {None}, True
    m = re.search(r"(\w+)\s*(\[[^\]]*\])*\s*$", pre.rstrip(";").strip())
    return ({m.group(1)} if m else set()), False


class Item:
    pass


# Hand-written rewrite files from before the per-batch files; regrouped too.
EXTRA = [
    "core/containers.cpp", "core/crt.cpp", "core/ptrlist.cpp", "core/stream.cpp",
    "engine/engine.cpp", "game/byte_streams.cpp", "game/calendar.cpp",
    "game/collection.cpp", "game/dialog_flow.cpp", "game/dialog_selection.cpp",
    "game/engine_lists.cpp", "game/gamespy_glue.cpp", "game/map_lifecycle.cpp",
    "game/mathutil.cpp", "game/network_train_sync.cpp", "game/random.cpp",
    "game/regargs.cpp", "game/runtime_wrappers.cpp", "game/strings.cpp",
    "game/text.cpp", "game/thunks.cpp", "game/tool_lifecycle.cpp",
    "game/track_tool_action.cpp", "game/train_consist.cpp",
    "game/train_engine_swap.cpp", "game/train_initialization.cpp",
    "game/train_loading.cpp", "game/train_station.cpp", "game/view_mode.cpp",
]


def inputs():
    objs = sorted(glob.glob(os.path.join(ROOT, "src/game/objects*.cpp")),
                  key=lambda p: int(re.search(r"objects(\d*)", p).group(1) or 0))
    return objs + [os.path.join(ROOT, "src", e) for e in EXTRA if os.path.exists(os.path.join(ROOT, "src", e))]


def is_var_def(it):
    """A namespace-scope variable with external linkage (one definition only)."""
    b = it.body
    if not it.ns or b.startswith(("#", "static", "template", "typedef", "using", "struct", "class", "union",
                                  "enum", "namespace", "const ", "constexpr", "extern", "inline")):
        return False
    if header_of(b) is not None:
        return False
    pre = b.split("=")[0]
    return "(" not in pre and b.endswith(";")


def load():
    files = []
    for p in inputs():
        text = open(p, encoding="utf-8-sig").read()
        its = []
        inside = False
        for k, a, b, c in items(text):
            if k == "ns_open":
                inside = True
                continue
            if k == "ns_close":
                inside = False
                continue
            if k == "tail":
                continue
            it = Item()
            it.file = os.path.basename(p)
            it.full = text[a:b].strip("\n")
            it.body = text[c:b].strip()
            it.ns = inside
            it.words = set(WORD.findall(it.body))
            regs = [int(m.group(1), 16) for m in REPLACE.finditer(it.body)]
            mac = re.match(r"(RT3_[A-Z0-9_]+)\(\s*0x([0-9a-fA-F]+)\s*,", it.body)
            it.addr = None
            if it.body.startswith("#include"):
                sub = os.path.basename(os.path.dirname(p))
                fixed = re.sub(r'#include\s+"([^"/]+)"', lambda m: '#include "%s/%s"' % (sub, m.group(1)), it.body)
                it.full = it.full.replace(it.body, fixed)
                it.body = fixed
            it.kind = "support"
            if it.body.startswith("#include") and not it.body.rstrip().endswith('.inc"'):
                it.kind = "include"
            elif it.body.startswith("#undef"):
                it.kind = "undef"
            elif regs:
                it.kind, it.addr = "primary", regs[0]
            elif mac and not it.body.startswith("#"):
                it.kind, it.addr = "primary", int(mac.group(2), 16)
            elif (header_of(it.body) is not None and not it.body.startswith(("#", "static", "template", "namespace", "inline"))
                  and not re.match(r"(RT3_NOINLINE\s+)?static\b", it.body) and not re.match(r"(struct|class|union|enum)\b", it.body)):
                it.kind = "helper"
            if it.kind == "support":
                it.names, it.decl = names_of(it.body)
            else:
                it.names, it.decl = set(), True
            if it.kind in ("primary", "helper"):
                head = header_of(it.body)
                it.fname = func_name(head) if head else None
            it.isvar = False
            if it.kind == "support" and is_var_def(it):
                it.kind, it.isvar = "helper", True
                it.fname = re.search(r"(\w+)\s*(\[[^\]]*\])*\s*$", it.body.split("=")[0].rstrip(";").strip()).group(1)
            if its and its[-1].body.startswith("#pragma pack(push") and chr(10) not in its[-1].body:
                glue = its.pop()
                it.full = glue.full + "\n" + it.full
                it.body = glue.body + "\n" + it.body
            if it.body.startswith("#pragma pack(pop") and its:
                its[-1].full += "\n" + it.full
                its[-1].body += "\n" + it.body
                continue
            its.append(it)
        # helpers go with the next registered item of their file
        nxt = None
        for it in reversed(its):
            if it.kind == "primary":
                nxt = it.addr
            elif it.kind == "helper":
                it.addr = nxt
        prev = None
        for it in its:
            if it.kind == "primary":
                prev = it.addr
            elif it.kind == "helper" and it.addr is None:
                it.addr = prev
        files.append((it.file if its else os.path.basename(p), its))
    return files


def main():
    dry = "--dry" in sys.argv
    files = load()
    modules = {}
    for fname, its in files:
        for it in its:
            if it.kind in ("primary", "helper"):
                it.module = module_of(it.addr)
                modules.setdefault(it.module, []).append(it)

    # which primary/helper functions are used from other modules of their file
    defined_in = {}  # (file, fname) -> item
    for fname, its in files:
        for it in its:
            if it.kind in ("primary", "helper") and it.fname:
                defined_in.setdefault((it.file, it.fname), it)
    unstatic = set()
    cross_protos = {}  # module -> list of prototype texts
    for fname, its in files:
        for it in its:
            if it.kind not in ("primary", "helper"):
                continue
            for w in it.words:
                d = defined_in.get((it.file, w))
                if d is not None and d is not it and d.module != it.module:
                    cross_protos.setdefault(it.module, []).append(d)
                    if re.match(r"(RT3_NOINLINE\s+)?static\b", d.body):
                        unstatic.add(id(d))

    by_file = {f: its for f, its in files}
    out = {}
    stats = {}
    local_static_warn = set()
    noinl = 0
    for mod, prim in modules.items():
        prim_files = []
        for it in prim:
            if it.file not in prim_files:
                prim_files.append(it.file)
        includes = []
        support = []       # (item, renamed text)
        provided = {}      # name -> (file, frozenset of normalized texts)
        seen_text = set()
        renames = {}       # file -> {old: new}
        for f in prim_files:
            for it in by_file[f]:
                if it.kind == "include" and norm(it.body) not in includes:
                    includes.append(norm(it.body))
        for f in prim_files:
            its = by_file[f]
            mine = [it for it in prim if it.file == f]
            need = set()
            for it in mine:
                need |= it.words
            sup = [it for it in its if it.kind == "support"]
            chosen = set()
            changed = True
            while changed:
                changed = False
                for i, it in enumerate(sup):
                    if i not in chosen and it.names & need:
                        chosen.add(i)
                        need |= it.words
                        changed = True
            for it in its:
                if it.kind == "include" and norm(it.body) not in includes:
                    includes.append(norm(it.body))
            ren = {}
            for n in sorted({n for i in chosen if not sup[i].decl for n in sup[i].names}):
                texts = frozenset(norm(s.body) for s in sup if n in s.names and not s.decl)
                if (n in provided and provided[n][1] != texts) or any(
                        d.fname == n and d.file != f for d in prim):
                    ren[n] = "%s_%s" % (n, re.sub(r"\D", "", f) or f.split(".")[0])
                else:
                    provided.setdefault(n, (f, texts))
            renames[f] = ren
            for i in sorted(chosen):
                it = sup[i]
                if it.names and all(HEADER_NAMES.get(n) in includes for n in it.names):
                    continue
                t = rename(it.full, ren)
                key = norm(rename(it.body, ren))
                if key in seen_text:
                    continue
                seen_text.add(key)
                support.append((it, t))
                if re.search(r"\bstatic\b", it.body[it.body.find("{"):] if "{" in it.body else ""):
                    local_static_warn.add((it.file, tuple(sorted(it.names))))
        protos = []
        users = list(prim) + [it for it, t in support]
        cross = []
        for u in users:
            for w in u.words:
                d = defined_in.get((u.file, w))
                if d is not None and d is not u and d.module != mod:
                    cross.append(d)
        for d in sorted(prim + cross, key=lambda d: d.addr):
            if d.isvar:
                if d in cross:
                    p = "extern " + norm(d.body.split("=")[0].rstrip(";").strip()) + ";"
                    if p not in protos:
                        protos.append(p)
                continue
            head = header_of(d.body)
            if not head or not d.fname or head.lstrip().startswith("template"):
                continue
            h = re.sub(r"__declspec\s*\(\s*naked\s*\)\s*", "", head.strip())
            if id(d) in unstatic:
                h = re.sub(r"^(RT3_NOINLINE\s+)?static\s+", r"\1", h)
            p = norm(h) + ";"
            if p not in protos:
                protos.append(p)
        body_items = []
        for it in sorted(prim, key=lambda it: (it.addr, it.file)):
            t = rename(it.full, renames[it.file])
            if id(it) in unstatic:
                t = re.sub(r"^((?://[^\n]*\n|\s)*)(RT3_NOINLINE\s+)?static\s+", r"\1\2", t, count=1)
            # Calls from code of another old file were never inlined (no LTCG);
            # keep it that way now that they share a file.
            head = header_of(it.body) or ""
            if (it.fname and header_of(it.body) is not None and not re.search(r"noinline|naked|RT3_NOINLINE|\btemplate\b", head)
                    and any(u.file != it.file and it.fname in u.words for u in users if u is not it)):
                t = re.sub(r"^((?://[^\n]*\n|[ \t\n])*)", lambda m: m.group(1) + "RT3_NOINLINE ", t, count=1)
                noinl += 1
            body_items.append((it, t))
        out[mod] = (includes, support, protos, body_items)
        stats[mod] = (len(prim), len(support), sum(len(r) for r in renames.values()))

    for mod in sorted(stats, key=lambda m: [s for s, n in BANDS if n == m][0]):
        print("%-18s %5d items %5d support %3d renames" % ((mod,) + stats[mod]))
    print("unstatic:", len(unstatic), "noinline added:", noinl)
    if local_static_warn:
        print("support items with local statics:", sorted(local_static_warn)[:20])
    if dry:
        return

    for mod, (includes, support, protos, body_items) in out.items():
        start = [s for s, n in BANDS if n == mod][0]
        end = min([s for s, n in BANDS if s > start] + [0x5a0000])
        d = "engine" if start >= ENGINE_START else "game"
        lines = ["// %s.cpp - RT3_mp.exe 0x%06x-0x%06x." % (mod, start, end),
                 "// Regrouped by tools/regroup.py from the per-batch files; one band of",
                 "// the exe per original object file.", ""]
        inc = [i for i in includes if "<" in i] + [i for i in includes if "<" not in i]
        lines += inc + [""]
        if "#include <windows.h>" in inc:
            lines += ["#undef near", "#undef far", "#undef small", ""]
        glob_sup = [t for it, t in support if not it.ns]
        if glob_sup:
            lines += ["\n\n".join(glob_sup), ""]
        lines += ["namespace rt3 {", ""]
        ns_sup = [(it, t) for it, t in support if it.ns]
        first = [t for it, t in ns_sup if not is_code(it)]
        code = [t for it, t in ns_sup if is_code(it)]
        if first:
            lines += [join_items(first), ""]
        if protos:
            lines += ["// Functions of this file, in exe order."] + protos + [""]
        if code:
            lines += ["\n\n".join(code), ""]
        lines += ["\n\n".join(t for it, t in body_items), "", "}  // namespace rt3", ""]
        path = os.path.join(ROOT, "src", d, mod + ".cpp")
        with open(path, "w", encoding="utf-8", newline="\n") as fh:
            fh.write("\n".join(lines))
    for p in inputs():
        os.remove(p)
    print("wrote %d modules" % len(out))


def join_items(texts):
    """Blank lines between items, except between one-line declarations."""
    out = []
    for t in texts:
        one = "\n" not in t and not t.lstrip().startswith(("//", "#define"))
        if out:
            out.append("\n" if one and prev_one else "\n\n")
        out.append(t)
        prev_one = one
    return "".join(out)


def is_code(it):
    b = it.body
    if b.startswith("#") or it.decl:
        return False
    if re.match(r"namespace\s*\{", b):
        return True
    if re.match(r"(template\s*<[^>]*>\s*)?(struct|class|union|enum|typedef|using)\b", b):
        return False
    if re.match(r"template\s*<", b):
        return False
    return header_of(b) is not None


def rename(text, ren):
    for old, new in ren.items():
        text = re.sub(r"(?<![\w.])(?<!->)%s(?!\w)" % re.escape(old), new, text)
    return text


if __name__ == "__main__":
    main()
