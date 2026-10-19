// GetString (0x5193f0): the game's text lookup by string ID.
//
// Order of lookup depends on the language (0x6d3c84, set from RT3.lng by
// 0x51cf80):
//   - ID 1 is always "\n".
//   - Languages 0-4, 8, 10: built-in strings for that language first, then the
//     strings loaded from the language file (0x6d3c94).
//   - Languages 6, 11, 12: loaded strings first, then the English built-ins.
//   - Others: loaded strings only.
//   - Missing everywhere: ID 7 gives "File error opening '%1'", anything else
//     "" (both pointers come from globals 0x6243c4 / 0x6243c0).
// The built-in table was a ~5 KB switch in the original; it is generated
// data here (tools/gen_builtin_strings.py).
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

struct BuiltinString { int32_t id; const char* text; };
struct BuiltinTable { const BuiltinString* entries; int32_t count; };
#include "builtin_strings.inc"

struct StringList { int32_t count; char** items; };

static const char* Builtin(int32_t lang, int32_t id) {
    const BuiltinTable& t = kBuiltinTables[lang];
    int32_t lo = 0, hi = t.count - 1;
    while (lo <= hi) {
        int32_t mid = (lo + hi) / 2;
        if (t.entries[mid].id == id) return t.entries[mid].text;
        if (t.entries[mid].id < id) lo = mid + 1;
        else hi = mid - 1;
    }
    return nullptr;
}

// QUIRK: like the original, a negative id indexes before the loaded array.
static const char* Loaded(int32_t id) {
    StringList* l = hook::Global<StringList*>(0x6d3c94);
    if (!l || !l->items || l->count <= id) return nullptr;
    return l->items[id];
}

const char* __fastcall GetString(int32_t id) {
    if (id == 1) return (const char*)0x5c9ce8; // "\n"
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    const char* s;
    switch (lang) {
    case 6: case 11: case 12:
        if ((s = Loaded(id)) != nullptr) return s;
        if ((s = Builtin(0, id)) != nullptr) return s;
        break;
    case 0: case 1: case 2: case 3: case 4: case 8: case 10:
        if ((s = Builtin(lang, id)) != nullptr) return s;
        break;
    }
    if ((s = Loaded(id)) != nullptr) return s;
    return id == 7 ? hook::Global<const char*>(0x6243c4) : hook::Global<const char*>(0x6243c0);
}
RT3_REPLACE(0x5193f0, GetString);

} // namespace rt3
