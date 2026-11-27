"""Generates docs/assets/logo.svg: OPENRT3 on a rusted, riveted iron plate with a winged wheel.

Needs fontTools (pip install fonttools) and Rockwell Extra Bold (C:/Windows/Fonts/ROCKEB.TTF);
the letters are stored as outlines, so the SVG looks the same without the font.
Usage: python tools/make_logo.py [out.svg]
"""
import math, os, sys
from fontTools.ttLib import TTFont
from fontTools.pens.svgPathPen import SVGPathPen
from fontTools.pens.transformPen import TransformPen

OUT_FILE = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), '..', 'docs', 'assets', 'logo.svg')
OUT = os.path.dirname(OUT_FILE)
FONTS = {}


def tp(font, text, size, cx, base, track=0, maxw=None):
    if font not in FONTS:
        FONTS[font] = TTFont('C:/Windows/Fonts/' + font)
    f = FONTS[font]; gs = f.getGlyphSet(); cm = f.getBestCmap(); upm = f['head'].unitsPerEm

    def width(sz):
        return sum(gs[cm[ord(c)]].width for c in text) * sz / upm + track * (len(text) - 1)
    if maxw and width(size) > maxw:
        size = size * maxw / width(size)
    s = size / upm; x = cx - width(size) / 2; d = []
    for c in text:
        n = cm[ord(c)]; pen = SVGPathPen(gs)
        gs[n].draw(TransformPen(pen, (s, 0, 0, -s, x, base)))
        d.append(pen.getCommands())
        x += gs[n].width * s + track
    return ' '.join(d)


def noise(id, freq, octv, seed, rgb, k, th):
    """Layer whose alpha = k*(noise - th): blobs of colour rgb."""
    r, g, b = rgb
    return (f'<filter id="{id}" x="0" y="0" width="520" height="260" filterUnits="userSpaceOnUse">'
            f'<feTurbulence type="fractalNoise" baseFrequency="{freq}" numOctaves="{octv}" seed="{seed}"/>'
            f'<feColorMatrix type="matrix" values="0 0 0 0 {r} 0 0 0 0 {g} 0 0 0 0 {b} {k} 0 0 0 {-k*th:.3f}"/></filter>')


def rough(id, freq, scale, seed=1):
    return (f'<filter id="{id}" x="-5%" y="-5%" width="110%" height="110%">'
            f'<feTurbulence type="fractalNoise" baseFrequency="{freq}" numOctaves="3" seed="{seed}" result="t"/>'
            f'<feDisplacementMap in="SourceGraphic" in2="t" scale="{scale}" xChannelSelector="R" yChannelSelector="G"/></filter>')


def hole_mask(id, freq, seed, th, k=8):
    """Mask: white except noise blobs, to chip paint away."""
    return (f'<filter id="{id}f" x="0" y="0" width="520" height="260" filterUnits="userSpaceOnUse">'
            f'<feTurbulence type="fractalNoise" baseFrequency="{freq}" numOctaves="4" seed="{seed}"/>'
            f'<feColorMatrix type="matrix" values="0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 {-k} 0 0 0 {k*th:.3f}"/></filter>'
            f'<mask id="{id}" maskUnits="userSpaceOnUse" x="0" y="0" width="520" height="260">'
            f'<rect x="0" y="0" width="520" height="260" fill="#fff" filter="url(#{id}f)"/></mask>')


FULL = '<rect x="0" y="0" width="520" height="260"'


def svg(label, body, defs):
    return (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 520 260" width="520" height="260" '
            f'role="img" aria-label="OpenRT3 logo: {label}"><defs>{defs}</defs>{body}</svg>\n')


def bolt(x, y, r=7):
    return (f'<circle cx="{x}" cy="{y}" r="{r}" fill="url(#bolt)" stroke="#140f0a" stroke-width="1.5"/>'
            f'<path d="M {x-r/2},{y-r/2} L {x+r/2},{y+r/2}" stroke="#2a2216" stroke-width="1.4"/>')


BOLTG = ('<radialGradient id="bolt" cx="0.35" cy="0.35" r="0.7"><stop offset="0" stop-color="#cfc29a"/>'
         '<stop offset="0.5" stop-color="#7d6f4c"/><stop offset="1" stop-color="#3a2e1e"/></radialGradient>')


def write(name, label, body, defs):
    with open(os.path.join(OUT, name), 'w', encoding='utf-8') as fh:
        fh.write(svg(label, body, defs))



PLATE = "M 40,78 Q 260,28 480,78 Q 492,80 492,92 L 492,180 Q 492,192 480,194 Q 260,244 40,194 Q 28,192 28,180 L 28,92 Q 28,80 40,78 Z"
FIELD = "M 50,88 Q 260,42 470,88 Q 478,90 478,98 L 478,174 Q 478,182 470,184 Q 260,230 50,184 Q 42,182 42,174 L 42,98 Q 42,90 50,88 Z"
word = tp('ROCKEB.TTF', 'OPENRT3', 66, 260, 156, 1)
streaks = ''.join(f'<path d="M {x-5},{y} L {x+5},{y} L {x+3},{y+h} L {x-2},{y+h} Z" fill="url(#streak)"/>'
                  for x, y, h in [(56, 104, 70), (464, 104, 64), (56, 170, 24), (464, 170, 22),
                                  (150, 90, 40), (330, 80, 55), (250, 170, 30)])
wings = ''.join(f'<path d="M {cx},{y-h/2} L {cx+sg*(L-10)},{y-h/2} L {cx+sg*L},{y} L {cx+sg*(L-10)},{y+h/2} L {cx},{y+h/2} Z"/>'
                for sg, cx in [(-1, 236), (1, 284)] for y, L, h in [(184, 118, 7), (194, 100, 6), (203, 80, 5)])
spokes = ' '.join(f'M 260,194 L {260+15*math.cos(a*math.pi/4):.1f},{194+15*math.sin(a*math.pi/4):.1f}' for a in range(8))
defs = (BOLTG + rough('edge', 0.035, 9, 4) + rough('edgeS', 0.08, 1.6, 9)
        + '<linearGradient id="rim" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#9e936a"/><stop offset="1" stop-color="#3e3824"/></linearGradient>'
        + '<linearGradient id="field" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#7a2616"/><stop offset="1" stop-color="#2e0c06"/></linearGradient>'
        + '<linearGradient id="stone" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#f2ead0"/><stop offset="0.6" stop-color="#cfc59a"/><stop offset="1" stop-color="#9a8f66"/></linearGradient>'
        + '<linearGradient id="streak" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#b0561c" stop-opacity="0.9"/><stop offset="1" stop-color="#6a2c0c" stop-opacity="0"/></linearGradient>'
        + '<radialGradient id="burn" cx="0.5" cy="0.45" r="0.6"><stop offset="0.55" stop-color="#000" stop-opacity="0"/><stop offset="1" stop-color="#120804" stop-opacity="0.75"/></radialGradient>'
        + noise('rustA', 0.03, 5, 11, (0.55, 0.24, 0.07), 7, 0.52)
        + noise('rustB', 0.09, 4, 5, (0.36, 0.14, 0.04), 6, 0.55)
        + noise('grit', 0.75, 3, 2, (0.08, 0.04, 0.02), -3, -0.45)
        + noise('specks', 0.5, 2, 8, (0.85, 0.5, 0.2), 9, 0.76)
        + hole_mask('chip', 0.06, 21, 0.72)
        + f'<clipPath id="pc"><path d="{PLATE}"/></clipPath><clipPath id="fc"><path d="{FIELD}"/></clipPath>'
        + f'<clipPath id="wc"><path d="{word}"/></clipPath>')
body = (f'<g filter="url(#edge)">'
        f'<path d="{PLATE}" fill="#140f0a" transform="translate(2,5)" opacity="0.5"/>'
        f'<path d="{PLATE}" fill="url(#rim)" stroke="#140f0a" stroke-width="3"/>'
        f'<g clip-path="url(#pc)">{FULL} filter="url(#rustA)"/>{FULL} filter="url(#rustB)"/></g>'
        f'<path d="{FIELD}" fill="url(#field)" stroke="#1c0c06" stroke-width="2"/>'
        f'<g clip-path="url(#fc)">{FULL} filter="url(#rustA)" opacity="0.8"/>{FULL} filter="url(#rustB)"/>{streaks}{FULL} filter="url(#grit)"/></g>'
        f'</g>'
        + ''.join(bolt(x, y) for x, y in [(56, 104), (464, 104), (56, 170), (464, 170)])
        + f'<g filter="url(#edgeS)"><path d="{word}" fill="none" stroke="#140f0a" stroke-width="9" stroke-linejoin="round"/>'
        f'<path d="{word}" fill="#3a2414"/>'
        f'<path d="{word}" fill="url(#stone)" mask="url(#chip)"/>'
        f'<g clip-path="url(#wc)">{FULL} filter="url(#specks)"/>{FULL} filter="url(#grit)" opacity="0.3"/></g></g>'
        f'<g fill="none" stroke="#e8d9a8" stroke-opacity="0.25" stroke-width="0.8"><path d="M 90,120 l 60,-14"/><path d="M 380,196 l 50,-6"/><path d="M 300,92 l 30,10"/></g>'
        f'<path d="{PLATE}" fill="url(#burn)"/>'
        f'<g filter="url(#edgeS)" fill="#a39468" stroke="#140f0a" stroke-width="1.5">{wings}<circle cx="260" cy="194" r="19"/></g>'
        '<circle cx="260" cy="194" r="12" fill="none" stroke="#140f0a" stroke-width="2"/>'
        f'<path d="{spokes}" stroke="#140f0a" stroke-width="2"/><circle cx="260" cy="194" r="3.5" fill="#140f0a"/>')
write(os.path.basename(OUT_FILE), 'a rusted riveted iron plate with chipped lettering', body, defs)
print('wrote', OUT_FILE)
