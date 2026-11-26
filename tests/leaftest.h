#pragma once
#include <stdint.h>

// One rewritten leaf function (tools/gen_leaftests.py): its exe address, the
// arguments and result (tests/test_leaves.cpp), its name.
struct LeafCase {
    uint32_t exe;
    const char* spec;  // result V none / F x87 / B byte / W word / R dword, then ECX, EDX, stack words:
                       // p pointer, i integer, f float, d double (low word; '-' its high word), - unused
    const char* name;
};
extern const LeafCase kLeafCases[];
extern const int kNumLeafCases;
