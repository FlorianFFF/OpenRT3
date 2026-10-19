"""Prints asm of ready (all callees done/library) functions.
    python tools/dumpready.py <block hex|all> <maxsize> <count> [skip addrs...]"""
import sys, re, importlib
sys.path.insert(0, str(__import__('pathlib').Path(__file__).resolve().parent))
nx = importlib.import_module('next')
funcs, callees, callers, done = nx.load()
game = lambda a: 0x401000 <= a < 0x56c850
isfun = lambda n: re.match(r"(thunk_)?FUN_", n)
conv = {int(l.split('\t')[0], 16): l.split('\t')[2] for l in open(nx.ROOT / 'decomp' / 'conv.tsv')}
block = sys.argv[1]; maxsize = int(sys.argv[2]); count = int(sys.argv[3])
skip = {int(x, 16) for x in sys.argv[4:]} | {0x414c50, 0x45ede0, 0x455720, 0x469800, 0x469890, 0x441ec0, 0x413df0,
    # register args / thunks / unknown-arity tail calls / GameSpy wrappers (later)
    0x415900, 0x455860, 0x455a40, 0x4691d0, 0x4694c0, 0x469620, 0x469660, 0x469680, 0x4696d0, 0x46c2f0, 0x469580, 0x45ec70, 0x46d980, 0x459bc0, 0x469450, 0x469520, 0x472ea0, 0x45ec00, 0x45a5f0, 0x459c30, 0x46ed30, 0x46f870, 0x458140, 0x47e390, 0x48a060, 0x45ecd0, 0x45ee40, 0x4a78d0, 0x45a700, 0x45f140, 0x4d9ec0, 0x459d00, 0x501f20, 0x507a50, 0x521620, 0x521b40, 0x521b60, 0x521d10, 0x521d40, 0x521d60, 0x521f90, 0x522050, 0x5220d0, 0x5221a0, 0x522240, 0x414cd0}
ready = sorted(a for a in funcs if game(a) and (block == 'all' or a >> 16 == int(block, 16)) and a not in done
               and a not in skip and not (0x521590 <= a < 0x525000) and a not in (0x5579d0, 0x557a30, 0x557af0, 0x557b30, 0x557330, 0x557390, 0x55ccc0, 0x55cd40, 0x552e20) and isfun(funcs[a][0]) and funcs[a][1] < maxsize
               and all(c in done or not game(c) or not isfun(funcs.get(c, ("FUN_",))[0]) for c in callees[a]))
for a in ready[:count]:
    print(f"== {a:08x} p={conv.get(a)} size={funcs[a][1]}")
    print(open(nx.ROOT / 'decomp' / f'{a:08x}.asm').read())
