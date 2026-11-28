// The rewrite's own copy of the game's read-only data (.rdata, 0x5c8000..):
// constants, tables and strings, generated at build time from the user's
// RT3.exe by tools/gen_rdata.py (out/gen/rdata.inc, not in the repository).
// rt3::Rdata (src/hook/hook.h) maps an exe address in that range to the copy.
// This file holds only the bytes: other files see the array through an extern
// declaration, so the compiler loads each constant from memory as the
// original does instead of folding it into the arithmetic.
#include "hook/hook.h"

#include "rdata.inc"

static_assert(RT3_RDATA_START == rt3::kRdataStart && RT3_RDATA_SIZE == rt3::kRdataSize,
              "the game's .rdata section is not where src/hook/hook.h expects it");
