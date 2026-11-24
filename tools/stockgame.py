"""The stock game for tests: a mirror of the game folder in %TEMP% without mods.

The game folder may hold mods that patch RT3_mp.exe as it loads - DesyncWatch
comes in as a dinput8.dll proxy. The rewrite follows the unmodded exe, so the
difftests and simcheck run here instead, where the game gets the system's
DirectInput. Subfolders are junctions (Saved Games included: saves still land
in the real folder) and top-level files are hard links (copies if linking
fails), so building the mirror is instant and needs no disk space.

Never delete the mirror with shutil.rmtree: it would follow the junctions into
the real game folder.

    python tools/stockgame.py     (prints the mirror's path)
"""
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

GAME = Path(__file__).resolve().parents[2] / "Railroad Tycoon 3 Full Game"
MIRROR = Path(tempfile.gettempdir()) / "rt3_stock"
# Mods, and the files a run writes (created fresh in the mirror).
SKIP = {"dinput8.dll", "dinput8.dll.bak", "desyncwatch", "desyncwatch.ini",
        "rt3re.ini", "rt3re.log", "difftest.log"}


def skipped(name):
    n = name.lower()
    return n in SKIP or n.endswith(".idb") or n.startswith("simcheck_")


def build():
    MIRROR.mkdir(exist_ok=True)
    for p in GAME.iterdir():
        if skipped(p.name):
            continue
        m = MIRROR / p.name
        if p.is_dir():
            if not m.exists():
                subprocess.run(["cmd", "/c", "mklink", "/J", str(m), str(p)], check=True, capture_output=True)
            continue
        if m.exists():
            if os.path.samefile(m, p):
                continue
            m.unlink()
        try:
            os.link(p, m)
        except OSError:
            shutil.copy2(p, m)
    for m in MIRROR.iterdir():  # a mod that got into the mirror some other way
        if m.name.lower() in SKIP - {"rt3re.ini", "rt3re.log", "difftest.log"} and m.is_file():
            m.unlink()
    return MIRROR


if __name__ == "__main__":
    print(build())
