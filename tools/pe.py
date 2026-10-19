"""Read data from RT3_mp.exe by virtual address (vtables, constants, tables).

    python pe.py dd 0x5d1aa0 16      # 16 dwords
    python pe.py f 0x5c99fc 4        # 4 floats
    python pe.py d 0x5c8770 2        # 2 doubles
    python pe.py s 0x5c9884          # C string
    python pe.py x 0x5c9884 64       # hex dump
"""
import struct
import sys
from pathlib import Path

EXE = Path(__file__).resolve().parents[2] / "Railroad Tycoon 3 Full Game" / "RT3_mp.exe"


class PE:
    def __init__(self, path=EXE):
        self.data = Path(path).read_bytes()
        pe = struct.unpack_from("<I", self.data, 0x3C)[0]
        nsec = struct.unpack_from("<H", self.data, pe + 6)[0]
        optsz = struct.unpack_from("<H", self.data, pe + 20)[0]
        self.base = struct.unpack_from("<I", self.data, pe + 24 + 28)[0]
        self.secs = []
        for i in range(nsec):
            o = pe + 24 + optsz + i * 40
            name = self.data[o:o + 8].rstrip(b"\0").decode()
            vsize, va, rsize, raw = struct.unpack_from("<IIII", self.data, o + 8)
            self.secs.append((name, self.base + va, vsize, raw, rsize))

    def read(self, va, n):
        for name, sva, vsize, raw, rsize in self.secs:
            if sva <= va < sva + max(vsize, rsize):
                off = va - sva
                chunk = self.data[raw + off: raw + min(off + n, rsize)]
                return chunk + b"\0" * (n - len(chunk))  # .bss part reads as zero
        raise ValueError(f"{va:#x} not in any section")

    def dd(self, va, n=1):
        return list(struct.unpack(f"<{n}I", self.read(va, 4 * n)))

    def f(self, va, n=1):
        return list(struct.unpack(f"<{n}f", self.read(va, 4 * n)))

    def d(self, va, n=1):
        return list(struct.unpack(f"<{n}d", self.read(va, 8 * n)))

    def s(self, va):
        return self.read(va, 512).split(b"\0", 1)[0].decode("latin-1")


if __name__ == "__main__":
    pe = PE()
    kind, va = sys.argv[1], int(sys.argv[2], 16)
    n = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    if kind == "dd":
        for i, v in enumerate(pe.dd(va, n)):
            print(f"{va + 4 * i:08x}: {v:08x}")
    elif kind in ("f", "d"):
        step = 4 if kind == "f" else 8
        for i, v in enumerate(getattr(pe, kind)(va, n)):
            print(f"{va + step * i:08x}: {v!r}")
    elif kind == "s":
        print(repr(pe.s(va)))
    else:
        b = pe.read(va, n)
        for i in range(0, n, 16):
            print(f"{va + i:08x}: {b[i:i + 16].hex(' ')}")
