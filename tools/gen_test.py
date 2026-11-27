"""Generic harness test generator: writes <tag>_test.cpp (in the current
folder), a differential test of the spec's functions with every function they
call faked (the Gen* harness in tests/test_ui.cpp, template tools/gen_template.cpp);
append it to tests/test_ui.cpp.
    python tools/gen_test.py <tag> <spec.py>
The spec module defines:
  cases      [(exe, name, spec)]   spec as in GenCase (prefix V: return not compared)
  ptrs       [global addresses that get a buffer (sometimes null)]
  ints       [global addresses that get small ints]
  nonnull    [globals among ptrs that are never null]
  setup      C++ run after the globals are set (optional)
  overrides  {callee exe: (retn, nargs, flags, kind)} for register-argument or
             odd callees (optional)
  skip       [callee exe addresses left real] (optional)
  extra_fakes {name: (retn, nargs, flags, kind)}: GenFake<tag>_x<name> for setup to plant (optional)
"""
import re, sys, pathlib, importlib.util
R = pathlib.Path(__file__).resolve().parents[1]
tag, specfile = sys.argv[1], sys.argv[2]
sp = importlib.util.spec_from_file_location("spec", specfile)
spec = importlib.util.module_from_spec(sp)
sp.loader.exec_module(spec)
texts = [p.read_text(encoding="utf-8") for p in (R / "src").rglob("*.cpp")]
texts += [p.read_text(encoding="utf-8") for p in pathlib.Path(".").glob("b[0-9][0-9][0-9]_src.txt")]
alltext = "\n".join(texts)
names = {}
for t in texts:
    for m in re.finditer(r"RT3_REPLACE\((0x[0-9a-fA-F]+), (\w+)\)", t):
        names.setdefault(int(m.group(1), 16), m.group(2))  # the tree (read first) wins over old staged names
EXE_ONLY = {0x5a276f: (0, 4, 0, 3), 0x53b080: (0, 1, 0, 3), 0x5a1145: (0, 1, 0, 3), 0x5a1740: (0, 1, 0, 3), 0x5a125d: (0, 1, 0, 0), 0x5a1887: (0, 2, 0, 0),
            0x58e2e0: (0, 0, 1, 3), 0x58e650: (0, 0, 3, 3), 0x58d7a0: (0, 0, 3, 3), 0x53b070: (0, 1, 0, 0), 0x58da60: (8, 2, 1, 3)}
REAL = {0x5a19c4, 0x5a10d0, 0x5193f0, 0x5a1634, 0x5a1390}
overrides = getattr(spec, "overrides", {})
skip = set(getattr(spec, "skip", []))


def signature(n):
    extra = getattr(spec, "sigs", {})
    if n in extra:
        rt, conv, ps = extra[n]
        return rt, conv, [x.strip() for x in ps.split(",") if x.strip()]
    m = re.search(r"^(?:RT3_NOINLINE |static |__declspec\(naked\) )*((?:const )?[\w:<>]+\*?) (__fastcall |__stdcall |__cdecl )?" + n + r"\(((?:[^;{)]|\n)*)\) *\{",
                  alltext, re.M)
    if not m:
        # Macro-generated: RT3_X(addr, n, ...) where "#define RT3_X(...) \" is
        # followed by a line declaring the function (name or name##suffix).
        inv = re.search(r"^(RT3_\w+)\(0x[0-9a-f]+, " + n + r"\b", alltext, re.M)
        if not inv:
            return None
        d = re.search(r"#define " + inv.group(1) + r"\([^)]*\) *\\\n *((?:const )?[\w:<>]+\*?) (__fastcall |__stdcall |__cdecl )?[\w#]+\(([^)]*)\)", alltext)
        if not d:
            return None
        return d.group(1), (d.group(2) or "").strip(), [x.strip() for x in d.group(3).split(",") if x.strip()]
    return m.group(1), (m.group(2) or "").strip(), [x.strip() for x in m.group(3).split(",") if x.strip()]


DEF_RE = r"^(?:RT3_NOINLINE |static |__declspec\(naked\) )*((?:const )?[\w:<>]+\*?) (__fastcall |__stdcall |__cdecl )?%s\(((?:[^;{)]|\n)*)\) *\{"


def signature_at(a, n):
    """The definition of n that precedes RT3_REPLACE(a, n) (overloads)."""
    key = "RT3_REPLACE(0x%x, %s)" % (a, n)
    for t in texts:
        i = t.find(key)
        if i < 0: continue
        ms = list(re.finditer(DEF_RE % n, t[:i], re.M))
        if ms:
            m = ms[-1]
            return m.group(1), (m.group(2) or "").strip(), [x.strip() for x in m.group(3).split(",") if x.strip()]
    return signature(n)


def overloaded(n):
    return len(re.findall(DEF_RE % n, alltext, re.M)) > 1


def ptype(sig):
    def strip(p):
        m = re.match(r"^(.*[\s\*&])(\w+)$", p)
        return m.group(1).strip() if m and m.group(1).strip() not in ("", "const", "unsigned") else p
    return "%s(%s*)(%s)" % (sig[0], sig[1] + " " if sig[1] else "", ", ".join(strip(p) for p in sig[2]))


def derive(a):
    if a in overrides:
        return overrides[a]
    if a in EXE_ONLY:
        return EXE_ONLY[a]
    n = names.get(a)
    sig = signature_at(a, n) if n else None
    if not sig:
        raise SystemExit("no signature for %x %s" % (a, n))
    rt, conv, ps = sig
    if rt == "void" and not conv and not ps and "naked" in alltext[alltext.find(n + "()") - 30:alltext.find(n + "()")]:
        raise SystemExit("naked callee %x %s needs an override" % (a, n))
    flags, stack = 0, ps
    if conv == "__fastcall":
        # An unnamed register parameter is not read: not logged.
        TYPEWORDS = {"int", "int32_t", "uint32_t", "int16_t", "uint16_t", "int8_t", "uint8_t", "char", "bool", "float", "void", "const",
                     "unsigned", "Collection", "Stream", "HMENU"}
        named = lambda q: bool(q) and q.replace("*", " * ").replace("&", " & ").split()[-1] not in TYPEWORDS | {"*", "&"}
        if len(ps) > 0 and ps[0] != "int" and named(ps[0]): flags |= 1
        if len(ps) > 1 and ps[1] != "int" and named(ps[1]): flags |= 2
        byte = lambda q: q.split()[0] in ("uint8_t", "int8_t", "char", "bool", "uint8_t") and "*" not in q
        if len(ps) > 0 and byte(ps[0]): flags |= 32
        if len(ps) > 1 and byte(ps[1]): flags |= 64
        stack = ps[2:]
    nargs = sum(2 if p.startswith("double") or p.startswith("Stream") and False else 1 for p in stack)
    retn = 0 if conv in ("", "__cdecl") and conv != "__stdcall" and conv != "__fastcall" else 4 * nargs
    if conv == "__stdcall": retn = 4 * nargs
    if conv == "__fastcall": retn = 4 * nargs
    kind = 3 if rt == "void" else 0 if rt.endswith("*") else 4 if rt in ("double", "float") else 2
    if a in (0x51b700, 0x51bfe0, 0x51c000, 0x518de0): kind = 10  # formatters return their buffer (ECX)
    return retn, min(nargs, 15), flags, kind


callee_addrs = set()
# A case that another case calls runs for real there (a fake would replace the case itself).
skip |= {a for a, n, s in spec.cases}
for a, n, s in spec.cases:
    asm = (R / "decomp" / ("%08x.asm" % a)).read_text()
    sys.path.insert(0, str(R / "tools"))
    from fragments import FRAGMENTS
    for frag, parent in FRAGMENTS.items():  # bodies Ghidra listed separately
        if parent == a: asm += (R / "decomp" / ("%08x.asm" % frag)).read_text()
    # The .asm export stops at a jump table; the decompiled C lists every callee.
    cfile = R / "decomp" / ("%08x.c" % a)
    if cfile.exists():
        asm += "".join(" CALL 0x00%s" % f[-6:] for f in re.findall(r"FUN_00([0-9a-f]{6})", cfile.read_text(errors="replace")))
    for m in re.finditer(r"(CALL|JMP) 0x00([0-9a-f]{6})", asm):
        t = int(m.group(2), 16)
        if t in REAL or t in skip: continue
        if t in names or t in EXE_ONLY or t in overrides: callee_addrs.add(t)
callees = []
for t in sorted(callee_addrs):
    retn, nargs, flags, kind = derive(t)
    callees.append((t, names.get(t, "") if t not in EXE_ONLY else "", retn, nargs, flags, kind))
fakes, adds, decls = [], [], set()
shared_fake = {}
fakes += ["GENFAKEF(GenFake%s_v26, GC(200, 0, 1, 3), 0)" % tag, "GENFAKE(GenFake%s_v22, GC(201, 3, 1, 3), 12)" % tag,
          "GENFAKE(GenFake%s_v0, GC(202, 0, 1, 3), 0)" % tag, "GENFAKE(GenFake%s_hook, GC(203, 1, 1, 3), 4)" % tag,
          "GENFAKE(GenFake%s_vp, GC(204, 0, 1, 0), 0)" % tag, "GENFAKE(GenFake%s_v2, GC(205, 1, 1, 3), 4)" % tag]
for name, (retn, nargs, flags, kind) in sorted(getattr(spec, "extra_fakes", {}).items()):
    fakes.append("GENFAKE(GenFake%s_x%s, GC(%d, %d, %d, %d), %d)" % (tag, name, 400 + len(fakes), nargs, flags, kind, retn))
for k, (slot, (retn, nargs, flags, kind)) in enumerate(sorted(getattr(spec, "iat", {}).items())):
    fakes.append("GENFAKE(GenFake%s_iat%d, GC(%d, %d, %d, %d), %d)" % (tag, k, 300 + k, nargs, flags, kind, retn))
    adds.append("    IatSlot iat%d(0x%x, (void*)&GenFake%s_iat%d);" % (k, slot, tag, k))
for i, (t, n, retn, nargs, flags, kind) in enumerate(callees):
    macro = "GENFAKEF" if kind == 4 else "GENFAKE"
    fakes.append("%s(GenFake%s_%d, GC(%d, %d, %d, %d), %d)" % (macro, tag, i, i, nargs, flags, 3 if kind == 4 else kind, retn))
    ref = "nullptr"
    if n and not signature_at(t, n):
        ref = "(void*)&rt3::" + n
    elif n:
        sig = signature_at(t, n)
        decls.add("%s %s%s(%s);" % (sig[0], sig[1] + " " if sig[1] else "", n, ", ".join(sig[2])))
        ref = "(void*)static_cast<%s>(&rt3::%s)" % (ptype(sig), n) if overloaded(n) else "(void*)&rt3::" + n
    fake = getattr(spec, "custom", {}).get(t, "GenFake%s_%d" % (tag, i))
    # Thunks registered under the same rewrite share one fake (the rewrite
    # entry can only be patched once).
    if n and n in shared_fake and t not in getattr(spec, "custom", {}): fake = shared_fake[n]
    elif n: shared_fake.setdefault(n, fake)
    adds.append("    t.Add(0x%x, %s, (void*)&%s);" % (t, ref, fake))
case_decls = []
for a, n, s in spec.cases:
    sig = signature(n)
    if not sig: raise SystemExit("no signature for case %s" % n)
    case_decls.append("%s %s%s(%s);" % (sig[0], sig[1] + " " if sig[1] else "", n, ", ".join(sig[2])))
tmpl = (R / "tools" / "gen_template.cpp").read_text()
def autov(a, n, s):  # a void rewrite's EAX is never compared
    sig = signature_at(a, n)
    return "V" + s if sig and sig[0] == "void" and not s.startswith("V") else s
rows = ",\n".join('        { 0x%x, (const void*)&rt3::%s, "%s" }' % (a, n, autov(a, n, s)) for a, n, s in spec.cases)
out = tmpl.replace("@TAG@", tag).replace("@DECLS@", "\n".join(sorted(decls))).replace("@FAKES@", "\n".join(fakes) + "\n" + getattr(spec, "prelude", ""))
out = out.replace("@CASEDECLS@", "\n".join(case_decls)).replace("@ADDS@", "\n".join(adds)).replace("@CASES@", rows)
out = out.replace("@PTRS@", ", ".join("0x%x" % g for g in spec.ptrs)).replace("@INTS@", ", ".join("0x%x" % g for g in spec.ints))
out = out.replace("@NONNULL@", "\n".join("            *(uint8_t**)0x%x = g_genBuf[4 + %d];" % (g, i % 20) for i, g in enumerate(spec.ptrs) if g in spec.nonnull))
vals = []
for ch, lst in sorted(getattr(spec, "values", {}).items()):
    vals.append("                if (k == '%s') { static const uint32_t v[] = { %s }; return v[GenDraw() %% %d]; }" % (ch, ", ".join(x if isinstance(x, str) else "0x%x" % (x & 0xffffffff) for x in lst), len(lst)))
out = out.replace("@VALUES@", "\n".join(vals))
out = out.replace("@SETUP@", getattr(spec, "setup", "")).replace("@BEFORE@", getattr(spec, "before", "")).replace("@AFTER@", getattr(spec, "after", ""))
out = re.sub(r"(static_cast<[^>]*?)\bStream\b", r"\1rt3::Stream", out)  # rt3 types in casts
if getattr(spec, "wide", False):  # cases with up to 12 stack args (the prelude defines GenCall12)
    out = out.replace("uint32_t a[8] = {};", "uint32_t a[12] = {};").replace("k < 8 && sp[2 + k]", "k < 12 && sp[2 + k]")
    out = out.replace("const uint32_t r = GenCall(", "const uint32_t r = GenCall12(")
open("%s_test.cpp" % tag, "w", newline="\n").write(out)
open("%s_extra.py" % tag, "w", newline="\n").write("import subprocess, sys\nsubprocess.run([sys.executable, \"tools/noinline.py\", %s], check=True)\n"
                                                    % ", ".join('"%s"' % c[1] for c in callees if c[1]))
print(len(callees), "callees,", len(spec.cases), "cases")
