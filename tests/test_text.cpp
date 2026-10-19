#include "difftest.h"

namespace rt3 { int __fastcall ToUpper(char c); }

DIFFTEST(text_ToUpper) {
    auto orig = (int(__fastcall*)(char))0x51d660;
    for (int c = 0; c < 256; ++c)
        EXPECT_EQ(orig((char)c), rt3::ToUpper((char)c), "c=%d", c);
}
