"""PNGs of the harness's screenshots (the "shot" action, src/harness.cpp).

    python tools/shot.py [<pattern>] [<out dir>]

Converts the 24-bit BMPs matching <pattern> (default "*.bmp") in the test
mirror into PNGs in <out dir> (default the mirror's "shots" folder) and
prints their paths. Pure Python (zlib), no imaging library needed.
"""
import struct
import sys
import zlib
from pathlib import Path

import stockgame


def bmp_to_png(src, dst):
    b = src.read_bytes()
    off = struct.unpack_from("<I", b, 10)[0]
    w, h = struct.unpack_from("<ii", b, 18)
    bpp = struct.unpack_from("<H", b, 28)[0]
    if bpp != 24:
        raise ValueError(f"{src}: {bpp} bits per pixel")
    row = (w * 3 + 3) & ~3
    raw = bytearray()
    for y in range(abs(h)):
        r = off + (abs(h) - 1 - y if h > 0 else y) * row
        line = b[r:r + w * 3]
        rgb = bytearray(w * 3)
        rgb[0::3], rgb[1::3], rgb[2::3] = line[2::3], line[1::3], line[0::3]
        raw += b"\0" + rgb

    def chunk(t, d):
        return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xffffffff)

    dst.write_bytes(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, abs(h), 8, 2, 0, 0, 0))
                    + chunk(b"IDAT", zlib.compress(bytes(raw), 6)) + chunk(b"IEND", b""))


def main():
    pattern = sys.argv[1] if len(sys.argv) > 1 else "*.bmp"
    out = Path(sys.argv[2]) if len(sys.argv) > 2 else stockgame.MIRROR / "shots"
    out.mkdir(exist_ok=True)
    for p in sorted(stockgame.MIRROR.glob(pattern)):
        d = out / (p.stem + ".png")
        bmp_to_png(p, d)
        print(d)


if __name__ == "__main__":
    main()
