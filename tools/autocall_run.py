"""Runs the scripted calls of tools/autocall.py through simcheck.

    python tools/autocall_run.py [--save=mature.gmc] [--frames=200] [--timeout=90] [--out=out/autocall] [chunk_NNN.txt ...]

Each chunk is first probed with the original alone (simcheck --probe): when the
original does not get through (a call ended or stalled the game), the chunk is
split until that call is found, which then goes to <out>/skip.txt (the next
autocall.py leaves it out). The rest of the chunk runs as one simcheck (the
original twice, then the rewrite) with its coverage in
out/coverage_runs/auto_<save>_<chunk>.txt for tools/coverage.py. When only the
rewrite fails, or a call's result, the saved game or the drawing differs, the
simcheck output goes to <out>/findings.txt.
"""
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def simcheck(save, frames, timeout, lines, tag, extra):
    tmp = ROOT / "out" / "autocall" / f"_run_{tag}.txt"
    tmp.write_text("\n".join(lines) + "\n")
    cmd = [sys.executable, str(ROOT / "tools" / "simcheck.py"), save, str(frames), f"--do=@{tmp}", f"--timeout={timeout}"] + extra
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, errors="replace")
    return p.stdout + p.stderr


GAME = Path(os.environ.get("TEMP", "")) / "rt3_stock"  # the test mirror (tools/stockgame.py)


def probe_ok(save, frames, timeout, lines, tag):
    return "PROBE OK" in simcheck(save, frames, timeout, lines, tag, ["--probe", "--nocoverage"])


def faulted():
    """Addresses whose call faulted in the last probe (the original run's "R <addr> fault")."""
    log = GAME / "simcheck_orig1.log"
    if not log.exists():
        return set()
    return {l.split()[1].lstrip("0") for l in log.read_text(errors="replace").splitlines()
            if l.startswith("R ") and l.rstrip().endswith("fault")}


def calls_of(path):
    return [l for l in path.read_text().splitlines() if l and not l.startswith("#")]


def main():
    save = next((a[7:] for a in sys.argv if a.startswith("--save=")), "mature.gmc")
    frames = int(next((a[9:] for a in sys.argv if a.startswith("--frames=")), 200))
    timeout = int(next((a[10:] for a in sys.argv if a.startswith("--timeout=")), 90))
    out_dir = Path(next((a[6:] for a in sys.argv if a.startswith("--out=")), ROOT / "out" / "autocall"))
    chunks = [Path(a) for a in sys.argv[1:] if not a.startswith("--")] or sorted(out_dir.glob("chunk_*.txt"))
    findings = out_dir / "findings.txt"
    skip = out_dir / "skip.txt"
    for ch in chunks:
        lines = calls_of(ch)
        # Drop the calls that stop the original (each found by halving).
        while lines and not probe_ok(save, frames, timeout, lines, ch.stem):
            part = list(lines)
            while len(part) > 1:
                half = part[: len(part) // 2]
                part = half if not probe_ok(save, frames, timeout, half, ch.stem) else part[len(part) // 2:]
            bad = part[0]
            with skip.open("a") as f:
                f.write(f"{bad.split()[2]} {bad.split('#')[-1].strip()}: stops the original game ({save})\n")
            print(f"{ch.stem}: dropped {bad.split('#')[-1].strip()} (stops the original)", flush=True)
            lines = [l for l in lines if l != bad]
        # A call that faults with its generated arguments stops part-way, at a
        # point that differs between the original's code and ours: no valid
        # comparison, so it is left out (the probe that passed ran them all).
        bad = faulted()
        for l in [l for l in lines if l.split()[2].lstrip("0") in bad]:
            with skip.open("a") as f:
                f.write(f"{l.split()[2]} {l.split('#')[-1].strip()}: faults with the generated arguments\n")
            lines.remove(l)
        if not lines:
            continue
        # A run without a save: split until the calls that end a run are found.
        # When the original runs failed too (calls whose generated arguments make
        # even the original nondeterministic) they are skipped, else they are a
        # finding.
        parts = [(lines, ch.stem)]
        while parts:
            part, tag = parts.pop(0)
            cov = ROOT / "out" / "coverage_runs" / f"auto_{Path(save).stem}_{tag}.txt"
            out = simcheck(save, frames, timeout, part, tag, [f"--coverage={cov}"])
            verdict = [l for l in out.splitlines() if "MATCH" in l or "DIFFER" in l or "FAILED" in l]
            print(f"{tag}: {len(part)} calls: " + " | ".join(verdict), flush=True)
            if any("FAILED" in l for l in verdict):
                if len(part) > 1:
                    half = len(part) // 2
                    parts += [(part[:half], tag + "a"), (part[half:], tag + "b")]
                    continue
                orig_too = "orig1: exception" in out or "orig2: exception" in out
                with (skip if orig_too else findings).open("a") as f:
                    f.write(f"{part[0].split()[2]} {part[0].split('#')[-1].strip()}: "
                            + ("unstable in the original with the generated arguments\n" if orig_too else f"fails only in the rewrite\n{out}\n"))
                continue
            if any("DIFFER" in l for l in verdict):
                with findings.open("a") as f:
                    f.write(f"== {tag} ({save}):\n" + "\n".join(part) + "\n--\n" + out + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
