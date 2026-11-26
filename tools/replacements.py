"""Source registrations and game functions deliberately left in the original exe."""
import re


# Registrations are statements, not mentions in comments describing disabled hooks.
REPLACEMENT_RE = re.compile(
    r"^[ \t]*RT3_REPLACE\(\s*0x([0-9a-fA-F]+)\s*,\s*(\w+)\s*\)",
    re.MULTILINE,
)

# These implementations remain callable at their exe addresses. They are valid
# dependencies for new rewrites, but must never count as installed replacements.
ORIGINAL_ONLY = frozenset({
    # Car_SetPiece: the rewrite matches the asm and its difftest, but simcheck
    # diverges (160 writes) with it on; suspected stack residue read by the
    # unrewritten 0x41ce80 that every caller runs next. Revisit with 0x41ce80.
    0x41c700,
    0x41b080, 0x41ce80,
    # With the leading car (+0x34e) off the track the outputs of 0x48edf0 are
    # never written but still stored (+0x362 / +0x366) and passed on.
    0x41c790,
    0x458140, 0x48f2b0, 0x48f7f0, 0x493cf0, 0x493ec0,
    # Callers consume unwritten stack locals; see docs/devin_handoff.md.
    0x4a9b70, 0x4b0310, 0x4b23d0,
    # Passes an unwritten stack local (the first company's logo) to
    # Company_Init 0x428420; -1 there draws a random logo.
    0x47d440,
    # Stores an unwritten local (the gear step) as the wake's vertical rate.
    0x486240,
    # Network handler: copies four never-written stack bytes into each
    # appended 9-byte table entry (Table9_Append).
    0x46c8c0,
    # Network handler: copies four never-written stack bytes into each
    # appended 9-byte table entry (Table9_Append).
    0x46c8c0,
    # Obj_SetNamedAmount: builds the new {name, amount} entry in a stack
    # buffer it never clears, so the saved name padding is stack residue
    # (the map editor's city building mixes showed it).
    0x4206b0,
})
