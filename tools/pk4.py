"""PK4 archive reader for Railroad Tycoon 3.

Layout (verified against every .PK4 shipped with the game):
    u32 magic   = 0x3EB
    u32 count
    count * 74-byte directory entries:
        u32 hash      (name hash; not needed for extraction)
        u32 size
        u32 offset    (relative to the end of the directory)
        u8  flag      (0 = stored)
        char name[61] (NUL-terminated)
    file data
"""
import struct
import sys
from pathlib import Path

MAGIC = 0x3EB
ENTRY = 74


def read_dir(data):
    magic, count = struct.unpack_from("<II", data, 0)
    if magic != MAGIC:
        raise ValueError(f"bad PK4 magic {magic:#x}")
    base = 8 + count * ENTRY
    entries = []
    for i in range(count):
        off = 8 + i * ENTRY
        h, size, rel, flag = struct.unpack_from("<IIIB", data, off)
        name = data[off + 13: off + ENTRY].split(b"\0", 1)[0].decode("latin-1")
        entries.append({"name": name, "hash": h, "size": size,
                        "offset": base + rel, "flag": flag})
    return entries


def open_pk4(path):
    data = Path(path).read_bytes()
    return data, read_dir(data)


def extract(path, out_dir):
    data, entries = open_pk4(path)
    out = Path(out_dir)
    out.mkdir(parents=True, exist_ok=True)
    for e in entries:
        (out / e["name"]).write_bytes(data[e["offset"]: e["offset"] + e["size"]])
    return entries


if __name__ == "__main__":
    if len(sys.argv) == 2:
        _, ents = open_pk4(sys.argv[1])
        for e in ents:
            print(f'{e["size"]:>10} {e["flag"]} {e["name"]}')
    else:
        extract(sys.argv[1], sys.argv[2])
