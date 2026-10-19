#include "difftest.h"
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>

namespace rt3 { const char* __fastcall GetString(int32_t id); }

DIFFTEST(strings_GetString) {
    auto orig = (const char*(__fastcall*)(int32_t))0x5193f0;
    int32_t& lang = hook::Global<int32_t>(0x6d3c84);
    struct List { int32_t count; char** items; };
    List*& loaded = hook::Global<List*>(0x6d3c94);
    int32_t savedLang = lang;
    List* savedLoaded = loaded;

    // A fake language file: every third ID present, with gaps (NULL items).
    static char* items[0x1200];
    static char text[0x1200][16];
    for (int i = 0; i < 0x1200; ++i) {
        sprintf(text[i], "lng%d", i);
        items[i] = (i % 3 == 0) ? text[i] : nullptr;
    }
    List fake = { 0x1100, items };

    for (int withFile = 0; withFile < 2; ++withFile) {
        loaded = withFile ? &fake : nullptr;
        for (int32_t l = 0; l <= 12; ++l) {
            lang = l;
            for (int32_t id = withFile ? 0 : -3; id < 0x1300; ++id) {
                const char* a = orig(id);
                const char* b = rt3::GetString(id);
                bool exeA = (uintptr_t)a >= 0x400000 && (uintptr_t)a < 0xdc9000;
                if (exeA && (uintptr_t)a >= 0x5c8000 && (uintptr_t)a < 0x5ed000 && a != (const char*)0x5c87a8 &&
                    a != (const char*)0x5d2170 && a != (const char*)0x5c9ce8)
                    EXPECT_EQ(strcmp(a, b), 0, "lang %d id %d file %d: '%s' vs '%s'", l, id, withFile, a, b); // built-in text
                else
                    EXPECT_EQ(a, b, "lang %d id %d file %d ptr", l, id, withFile);
            }
        }
    }
    lang = savedLang;
    loaded = savedLoaded;
}
