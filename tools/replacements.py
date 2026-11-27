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
    # Empty: every game function is installed. The ones whose original reads a
    # local it never wrote (a stack leftover) start that local at 0 and count
    # the path (g_unwrittenReads, src/game/Train.cpp).
})
