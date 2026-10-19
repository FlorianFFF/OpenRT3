// Data extraction from the original functions (not tests). Run with
//   rt3re.exe --test --filter=dump_
// Each writes a file in the game folder that tools/ turn into C++ tables.
#include "difftest.h"
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>

// 0x5193f0 BuiltinString(id) for every language index: lang \t id \t ptr \t hex(bytes)
DIFFTEST(dump_builtin_strings) {
    auto orig = (const char*(__fastcall*)(int32_t))0x5193f0;
    int32_t& lang = hook::Global<int32_t>(0x6d3c84);
    int32_t saved = lang;
    FILE* f = fopen("builtin_strings.tsv", "w");
    for (int32_t l = 0; l <= 12; ++l) {
        lang = l;
        for (int32_t id = -2; id < 0x3000; ++id) {
            const char* s = orig(id);
            fprintf(f, "%d\t%d\t%08x\t", l, id, (unsigned)(uintptr_t)s);
            for (const unsigned char* p = (const unsigned char*)s; p && *p; ++p) fprintf(f, "%02x", *p);
            fputc('\n', f);
        }
    }
    fclose(f);
    lang = saved;
}
