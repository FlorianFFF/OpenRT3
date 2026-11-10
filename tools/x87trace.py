"""Symbolic x87 trace of a decompiled function, to help rewrite float-heavy code.

    python tools/x87trace.py 0x498320 [--pops 0x51db80=16,...] [--ret 0x51db80,...]

Walks decomp/<addr>.asm in order, tracking ESP (so [ESP+x] becomes a fixed
frame slot S<off>, measured after the prologue pushes), the x87 stack as
expressions, registers loaded from frame slots, and pushed call arguments.
Prints each FSTP/FST/FCOM*/CALL/MOV-to-slot with the expression involved.
The FPU stack is reset at jump targets (state across joins is not merged),
so check those spots by hand. Callee stack pops come from --pops; float
results from --ret (CRT math 0x5a1494 sin, 0x5a13e4 cos, 0x5a1270 atan,
0x5a152e fabs and 0x5a2f84 are built in).
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import pe as pemod

P = pemod.PE()
addr = int(sys.argv[1], 16)
pops = {}
base_at = None
rets = {0x5a1494, 0x5a13e4, 0x5a1270, 0x5a152e, 0x5a2f84}
args = sys.argv[2:]
i = 0
while i < len(args):
    if args[i] == "--pops":
        for kv in args[i + 1].split(","):
            k, v = kv.split("=")
            pops[int(k, 16)] = int(v, 0)
        i += 2
    elif args[i] == "--base":
        base_at = int(args[i + 1], 16)
        i += 2
    elif args[i] == "--ret":
        rets |= {int(x, 16) for x in args[i + 1].split(",")}
        i += 2
    else:
        i += 1
names = {0x5a1494: "sin", 0x5a13e4: "cos", 0x5a1270: "atan", 0x5a152e: "fabs", 0x5a2f84: "crt5a2f84"}

lines = (ROOT / "decomp" / ("%08x.asm" % addr)).read_text().splitlines()
targets = set()
for ln in lines:
    m = re.search(r"\b(J\w+|CALL)\s+0x([0-9a-f]+)", ln)
    if m and m.group(1) != "CALL":
        targets.add(int(m.group(2), 16))


def const(sz, a):
    try:
        if sz == "dword":
            return "%sF(%x)" % ("", a) + "{%g}" % P.f32(a) if hasattr(P, "f32") else "F(%x)" % a
        return "D(%x)" % a
    except Exception:
        return "[%x]" % a


def fconst(sz, a):
    import struct
    try:
        raw = P.read(a, 4 if sz == "dword" else 8)
        v = struct.unpack("<f" if sz == "dword" else "<d", raw)[0]
        return "%s(%x)=%.9g" % ("F" if sz == "dword" else "D", a, v)
    except Exception:
        return "[%x]" % a


delta = None
fpu = []
regs = {}
slots = {}
argstack = []  # symbolic pushed values, top last
at_label = {}  # jump target -> (delta, fpu) at the first jump to it
dead = False  # the previous instruction never falls through


slotver = {}


def slotname(off, write=False):
    o = off - delta
    if o < 0:
        return "arg%d" % (-o // 4)
    if write:
        slotver[o] = slotver.get(o, 0) + 1
    v = slotver.get(o, 0)
    return ("S%x" % o) + ("" if v == 0 else "'" * v if v < 4 else ".%d" % v)


def mem(sz, expr):
    # returns symbolic operand text for a memory reference
    m = re.match(r"ESP \+ (0x[0-9a-f]+)$", expr) or re.match(r"ESP$", expr)
    if m:
        off = int(m.group(1), 16) if m.groups() else 0
        return slotname(off)
    m = re.match(r"(0x[0-9a-f]+)$", expr)
    if m:
        return fconst(sz, int(m.group(1), 16))
    return "[" + expr + "]"


def pr(a, text):
    print("%s  %s" % (a, text))


for ln in lines:
    m = re.match(r"([0-9a-f]{8})\s+(\S+)\s*(.*)", ln)
    if not m:
        continue
    a, op, ops = m.groups()
    ia = int(a, 16)
    if ia in targets:
        st = at_label.get(ia)
        if st is not None and (dead or delta is None or st[0] == delta):
            if delta is not None:
                if not dead and fpu != st[1]:
                    n = max(len(fpu), len(st[1]))
                    f2 = []
                    for k in range(n):
                        x = st[1][k] if k < len(st[1]) else "?"
                        y = fpu[k] if k < len(fpu) else "?"
                        f2.append(x if x == y else "phi(%s | %s)" % (x, y))
                    st = (st[0], f2)
                delta, fpu = st[0], list(st[1])
                while len(argstack) > delta // 4: argstack.pop()
                while len(argstack) < delta // 4: argstack.append("<?>")
        elif st is not None:
            pr(a, "-- label: delta %s here vs %s from a jump" % (delta, st[0]))
        print("%s: label" % a)
    dead = False
    if delta is None:
        if base_at is None or ia < base_at:
            continue
        delta = 0
    if op == "PUSH":
        delta += 4
        v = regs.get(ops, ops)
        if ops.startswith("dword ptr ["):
            v = mem("dword", ops[11:-1])
        argstack.append(v)
        continue
    if op == "POP":
        delta -= 4
        if argstack:
            argstack.pop()
        continue
    if op in ("SUB", "ADD") and ops.startswith("ESP,"):
        n = int(ops[4:], 16)
        delta += n if op == "SUB" else -n
        if op == "SUB":
            argstack += ["<slot>"] * (n // 4)
        else:
            for _ in range(n // 4):
                if argstack:
                    argstack.pop()
        continue
    if op == "CALL":
        t = int(ops, 16) if ops.startswith("0x") else None
        n = pops.get(t, 0)
        k = n // 4
        a_ = list(reversed(argstack[-k:])) if k else []
        pr(a, "CALL %s(%s)" % (names.get(t, ops), ", ".join(a_)))
        for _ in range(k):
            if argstack:
                argstack.pop()
        delta -= n
        if t in rets:
            fpu.insert(0, "%s(...)@%s" % (names.get(t, ops), a))
        regs.pop("EAX", None)
        continue
    mm = re.match(r"(dword|qword) ptr \[(.*)\]$", ops)
    if op in ("FLD", "FILD"):
        if mm:
            v = mem(mm.group(1), mm.group(2))
            fpu.insert(0, ("int " if op == "FILD" else "") + v)
        elif ops.startswith("ST"):
            k = int(ops[2:] or 0)
            fpu.insert(0, fpu[k] if k < len(fpu) else "?")
        continue
    if op in ("FST", "FSTP"):
        if mm:
            om0 = re.match(r"ESP \+ (0x[0-9a-f]+)$", mm.group(2)) or re.match(r"ESP$", mm.group(2))
            if om0:
                dst = slotname(int(om0.group(1), 16) if om0.groups() else 0, True)
            else:
                dst = mem(mm.group(1), mm.group(2))
            v = fpu[0] if fpu else "?"
            pr(a, "%s = (%s) %s" % (dst, "float" if mm.group(1) == "dword" else "double", v))
            if dst.startswith("S"):
                # within an outgoing-arg area this is an argument
                pass
            slots[dst] = dst
            om = re.match(r"ESP \+ (0x[0-9a-f]+)$", mm.group(2)) or re.match(r"ESP$", mm.group(2))
            if om:
                off = int(om.group(1), 16) if om.groups() else 0
                if off < delta and off // 4 < len(argstack):
                    argstack[-(1 + off // 4)] = v
            if op == "FSTP" and fpu:
                fpu.pop(0)
            # if the slot is an arg slot (below base), update argstack
            pass
        elif ops.startswith("ST"):
            k = int(ops[2:])
            if fpu:
                v = fpu[0]
                if k < len(fpu):
                    fpu[k] = v
                if op == "FSTP":
                    fpu.pop(0)
        continue
    if op in ("FADD", "FSUB", "FMUL", "FDIV", "FSUBR", "FDIVR"):
        sym = {"FADD": "+", "FSUB": "-", "FMUL": "*", "FDIV": "/", "FSUBR": "-", "FDIVR": "/"}[op]
        if mm:
            v = mem(mm.group(1), mm.group(2))
            x = fpu[0] if fpu else "?"
            fpu[0:1] = ["(%s %s %s)" % (v, sym, x) if op in ("FSUBR", "FDIVR") else "(%s %s %s)" % (x, sym, v)]
        else:
            p = ops.split(",")
            d = int(p[0][2:] or 0)
            s = int(p[1][2:] or 0) if len(p) > 1 else 1
            x, y = fpu[d] if d < len(fpu) else "?", fpu[s] if s < len(fpu) else "?"
            fpu[d] = "(%s %s %s)" % ((y, sym, x) if op in ("FSUBR", "FDIVR") else (x, sym, y))
        continue
    if op in ("FADDP", "FSUBP", "FMULP", "FDIVP", "FSUBRP", "FDIVRP"):
        sym = {"FADDP": "+", "FSUBP": "-", "FMULP": "*", "FDIVP": "/", "FSUBRP": "-", "FDIVRP": "/"}[op]
        x, y = (fpu[0] if fpu else "?"), (fpu[1] if len(fpu) > 1 else "?")
        # ST1 = ST1 op ST0 (Intel: FSUBP ST1,ST0 -> ST1 - ST0), pop
        r = "(%s %s %s)" % ((y, sym, x) if op in ("FADDP", "FSUBP", "FMULP", "FDIVP") else (x, sym, y))
        fpu = [r] + fpu[2:]
        continue
    if op == "FCHS":
        fpu[0] = "-" + fpu[0]
        continue
    if op == "FABS":
        fpu[0] = "fabs(%s)" % fpu[0]
        continue
    if op == "FXCH":
        k = int(ops[2:]) if ops else 1
        fpu[0], fpu[k] = fpu[k], fpu[0]
        continue
    if op in ("FCOMP", "FCOM", "FCOMPP", "FUCOMPP", "FUCOMP"):
        if mm:
            v = mem(mm.group(1), mm.group(2))
            pr(a, "compare %s ? %s" % (fpu[0] if fpu else "?", v))
            if op == "FCOMP" and fpu:
                fpu.pop(0)
        else:
            pr(a, "compare %s ? %s" % (fpu[0] if fpu else "?", fpu[1] if len(fpu) > 1 else "?"))
            if op in ("FCOMPP", "FUCOMPP"):
                fpu = fpu[2:]
        continue
    if op == "FNSTSW":
        continue
    if op == "MOV":
        mr = re.match(r"(E[A-Z][A-Z]|[A-Z]L),(dword|byte|word) ptr \[(.*)\]$", ops)
        if mr:
            regs[mr.group(1)] = mem("dword", mr.group(3))
            continue
        ms = re.match(r"(dword|byte|word) ptr \[ESP \+ (0x[0-9a-f]+)\],(.*)$", ops) or re.match(r"(dword|byte|word) ptr \[ESP\],(.*)$", ops)
        if ms:
            if len(ms.groups()) == 3:
                dst = slotname(int(ms.group(2), 16), True)
                src = ms.group(3)
            else:
                dst = slotname(0, True)
                src = ms.group(2)
            pr(a, "%s = %s" % (dst, regs.get(src, src)))
            continue
        mr2 = re.match(r"(E[A-Z][A-Z]),(.*)$", ops)
        if mr2:
            regs[mr2.group(1)] = regs.get(mr2.group(2), mr2.group(2))
            continue
    if op.startswith("J") and delta is not None:
        mj = re.match(r"0x([0-9a-f]+)$", ops)
        if mj:
            at_label.setdefault(int(mj.group(1), 16), (delta, list(fpu)))
        if op == "JMP":
            dead = True
    if op == "RET":
        dead = True
    if op.startswith("J") or op in ("TEST", "CMP", "LEA", "XOR", "INC", "DEC", "IMUL", "SHL", "SAR", "AND", "OR", "NEG", "SBB", "SETZ", "SETNZ", "CDQ", "RET", "NOP", "MOVSX", "MOVZX"):
        if op.startswith("J") or op in ("CMP", "TEST"):
            pr(a, "%s %s" % (op, ops))
        if op in ("LEA", "XOR", "INC", "DEC", "IMUL", "SHL", "SAR", "AND", "OR", "MOVSX", "MOVZX"):
            r = ops.split(",")[0]
            if op == "LEA":
                ml = re.match(r"(E[A-Z][A-Z]),\[ESP \+ (0x[0-9a-f]+)\]$", ops)
                regs[r] = ("&" + slotname(int(ml.group(2), 16))) if ml else ops
            else:
                regs.pop(r, None)
        continue
    pr(a, "?? %s %s" % (op, ops))
