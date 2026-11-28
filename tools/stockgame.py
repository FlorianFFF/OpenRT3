"""The stock game for tests: a mirror of the game folder in %TEMP% without mods.

The game folder may hold mods that patch the game as it loads (for example a
dinput8.dll proxy). The rewrite follows the unmodded game, so the difftests
and simcheck run here instead, where the game gets the system's DirectInput.

Nothing a run writes may reach the real game folder. Read-only content is
shared: subfolders of Data are junctions and top-level files hard links
(copies if linking fails), so the mirror costs no disk space. What the game
writes is private to the mirror: Data/Configuration and Data/Log (settings
saved on exit, logs), HotKeys (saved at shutdown) and Saved Games (autosaves,
simcheck's saves) are copies, refreshed from the game folder only where the game folder changed;
maps (where the map editor saves) is a real folder of hard links, so files a
run adds there stay in the mirror;
the settings are reset to the game folder's before every run (reset_settings).
Files a run creates at the top level (logs, traces) are never linked.

Never delete the mirror with shutil.rmtree: it would follow the junctions into
the real game folder.

    python tools/stockgame.py     (builds the mirror, resets its settings, prints its path)
"""
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

GAME = Path(__file__).resolve().parents[2] / "Railroad Tycoon 3 Full Game"
MIRROR = Path(tempfile.gettempdir()) / "rt3_stock"
# Mods, and the files a run writes (created fresh in the mirror).
SKIP = {"dinput8.dll", "dinput8.dll.bak", "rt3re.ini", "rt3re.log", "difftest.log", "stall.txt",
        "difftest.coverage", "difftest_leaves.log", "difftest_leaves.coverage", "leaves_current.txt"}
# Folders the game writes to: private copies in the mirror.
PRIVATE = {"Saved Games", "Data/Configuration", "Data/Log", "HotKeys"}  # (the game rewrites HotKeys/hotkeys.txt every run)
# Folders the game reads but can also add to (the map editor saves its maps in
# maps): real folders in the mirror whose files are hard links, so a file a run
# creates there stays in the mirror (and the 500 MB of maps cost no space).
LINKED = {"maps"}


def skipped(name):
    n = name.lower()
    return n in SKIP or n.endswith(".idb") or n.startswith("simcheck_")


def is_junction(p):
    try:
        return bool(os.readlink(p))
    except OSError:
        return False


def unjunction(m):
    """A junction in the mirror becomes an empty real folder. os.rmdir removes
    the junction itself, never what it points to."""
    if is_junction(m):
        os.rmdir(m)
    m.mkdir(exist_ok=True)


def junction(m, p):
    if not m.exists():
        subprocess.run(["cmd", "/c", "mklink", "/J", str(m), str(p)], check=True, capture_output=True)


def copy_newer(src, dst):
    """Copies the files of src into dst where dst lacks them or src's copy
    changed since (a run's own changes in dst stay otherwise)."""
    dst.mkdir(exist_ok=True)
    for p in src.iterdir():
        m = dst / p.name
        if p.is_dir():
            copy_newer(p, m)
            continue
        if skipped(p.name):
            continue
        if m.exists() and not os.path.samefile(m, p) and m.stat().st_mtime >= p.stat().st_mtime:
            continue
        if m.exists():
            m.unlink()  # (a hard link from an older mirror: unlinking drops only the link)
        shutil.copy2(p, m)


def link_file(p, m):
    if m.exists():
        if os.path.samefile(m, p):
            return
        m.unlink()
    try:
        os.link(p, m)
    except OSError:
        shutil.copy2(p, m)


def mirror_dir(src, dst, rel=""):
    for p in src.iterdir():
        r = (rel + "/" + p.name).lstrip("/")
        m = dst / p.name
        if p.is_dir():
            if r in PRIVATE:
                unjunction(m)
                copy_newer(p, m)
            elif r in LINKED:
                unjunction(m)
                for f in p.iterdir():
                    if f.is_dir():
                        junction(m / f.name, f)
                    elif not skipped(f.name):
                        link_file(f, m / f.name)
            elif any(x.startswith(r + "/") for x in PRIVATE):
                unjunction(m)  # holds a private folder: a real folder, linked inside
                mirror_dir(p, m, r)
            else:
                junction(m, p)
            continue
        if skipped(p.name):
            if m.exists() and not is_junction(m) and os.path.samefile(m, p):
                m.unlink()  # an older mirror linked it: drop the link, keep the game's file
            continue
        link_file(p, m)


def reset_settings(mirror=MIRROR):
    """Every run starts from the game folder's settings: the game saves its
    settings on exit, and a run's changes must not carry into the next one."""
    for rel in ("Data/Configuration",):
        src, dst = GAME / rel, mirror / rel
        # Files a run created (the options dialog saves new ones) go too, or they
        # carry into the next run.
        for m in dst.iterdir():
            if m.is_file() and not (src / m.name).exists():
                m.unlink()
        for p in src.iterdir():
            if p.is_file():
                m = dst / p.name
                if m.exists():
                    m.unlink()
                shutil.copy2(p, m)


def build(mirror=MIRROR, maps=True):
    mirror.mkdir(exist_ok=True)
    mirror_dir(GAME, mirror)
    for m in mirror.iterdir():  # a mod that got into the mirror some other way
        if m.name.lower() in {"dinput8.dll", "dinput8.dll.bak"} and m.is_file():
            m.unlink()
    if not maps:
        # No scenarios of its own (a client from here is refused by the host:
        # the game does not send scenarios). Links to the game's maps go (dropping a link keeps the game's
        # file), and so does a map an earlier run received.
        maps_dir = mirror / "maps"
        if is_junction(maps_dir) or maps_dir.resolve() == (GAME / "maps").resolve():
            raise RuntimeError(f"{maps_dir} is the game's own maps folder")
        for m in maps_dir.iterdir():
            if m.is_file():
                m.unlink()
    return mirror


# A second mirror whose maps folder is empty (tools/mpcheck.py --client-nomaps).
MIRROR_NOMAPS = Path(tempfile.gettempdir()) / "rt3_stock_nomaps"


def build_nomaps():
    return build(MIRROR_NOMAPS, maps=False)


if __name__ == "__main__":
    print(build())
    reset_settings()  # (build.bat test: the tests start from the game's settings too)
