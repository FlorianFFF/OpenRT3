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
#include <string.h>

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

RT3_NOINLINE const char* __fastcall GetString(int32_t id) {
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

int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void BuildNameIndex();
void __fastcall String_Assign(char** s, const char* src);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

static bool LeadByte(char c) {
    return hook::Global<int32_t>(0x6d3c8c) == 3 && ((int(__cdecl*)(int))0x5a330d)((uint8_t)c) != 0;
}

// 0x51c020. Loads a language file: lines `<id> "text"` with ids 1..14999,
// optionally followed by a `;` comment. Lines starting with `;` or `/` are
// comments. `\n` in a text becomes a newline. Braces in texts are replaced
// by parentheses (with a warning) unless entry 4040 holds a non-empty text;
// the first pass only looks for that. Problems are logged with the line
// number and parsing goes on. Returns the list (count = highest id + 1).
// QUIRK: the first pass counts no lines (it reports line 0, or -1 after a
// CRLF), and the second pass starts counting from where the first left off.
RT3_NOINLINE StringList* __fastcall Strings_LoadFile(const char* path) {
    auto malloc_ = (void*(__cdecl*)(size_t))0x5a125d;
    auto atol_ = (int32_t(__cdecl*)(const char*))0x5a1ea5;
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    auto memmove_ = (void*(__cdecl*)(void*, const void*, size_t))0x5a20b0;
    int32_t f = StreamOpen(path, 5, 200000, 1);
    StringList* l = (StringList*)malloc_(8);
    l->count = 0;
    l->items = nullptr;
    l->items = (char**)malloc_(60000);
    memset(l->items, 0, 60000);
    static char line[2000];
    static char num[0x40], msg[0x800];
    memset(line, 0, sizeof line);
    char prev = 0;
    int32_t maxId = 0, raw = 0, lineNo = 0, more;
    auto report = [&](const char* fmt) {
        FormatText(msg, fmt, Num_Format(num, 1, (double)lineNo, 10, 1, 0, 0), line, 0, 0, 0, 0, 0, 0);
        DebugLog(msg, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234);
    };
    auto digits = [&](int32_t j) {
        int32_t k = 1;
        if ((signed char)line[j + 1] >= '0') {
            char c2;
            do {
                if ((signed char)line[j + k] > '9') break;
                c2 = line[j + k + 1];
                ++k;
            } while ((signed char)c2 >= '0');
        }
        return j + k;
    };
    do {
        lineNo = 0;
        int32_t n = 0;
        for (;;) {
            more = StreamRead(f, &line[n], 1, 0, 0);
            if (line[n] == '\r' || line[n] == '\n' || !more) break;
            ++n;
        }
        if (line[n] == '\n' && prev == '\r') lineNo = -1;
        prev = line[n];
        line[n] = 0;
        for (int32_t j = 0; j < n;) {
            signed char c = (signed char)line[j];
            if (c < '0' || c > '9') {
                if (c > ' ') break;
                ++j;
                continue;
            }
            int32_t e = digits(j);
            line[e] = 0;
            int32_t id = atol_(&line[j]);
            if (id <= 0 || id >= 15000) {
                report((const char*)0x5dcca4);
                break;
            }
            if (id == 0xfc8) {
                char* p = &line[6];
                bool quote = false;
                if ((signed char)*p >= ' ') {
                    for (;;) {
                        if (*p == '"') {
                            quote = true;
                            break;
                        }
                        ++p;
                        if ((signed char)*p < ' ') break;
                    }
                }
                if (!quote && *p == '"') quote = true;
                if (quote && p[1] != '"') raw = 1;
            }
            j = e;
        }
    } while (more);
    StreamClose(f);
    f = StreamOpen(path, 5, 200000, 1);
    do {
        ++lineNo;
        int32_t n = 0;
        for (;;) {
            more = StreamRead(f, &line[n], 1, 0, 0);
            if (line[n] == '\r' || line[n] == '\n' || !more) break;
            ++n;
        }
        if (line[n] == '\n' && prev == '\r') --lineNo;
        prev = line[n];
        line[n] = 0;
        int32_t j = 0;
        while (j < n) {
            signed char c = (signed char)line[j];
            if (c < '0' || c > '9') {
                if (c > ' ') {
                    if (c != ';' && c != '/') report((const char*)0x5dcbbc);
                    break;
                }
                if (LeadByte(c)) ++j;
                ++j;
                continue;
            }
            int32_t e = digits(j);
            char save = line[e];
            line[e] = 0;
            int32_t id = atol_(&line[j]);
            line[e] = save;
            if (id <= 0 || id >= 15000) {
                report((const char*)0x5dcca4);
                break;
            }
            if (line[e]) {
                char c2;
                do {
                    if ((signed char)line[e] > ' ') break;
                    if (LeadByte(line[e])) ++e;
                    c2 = line[e + 1];
                    ++e;
                } while (c2);
            }
            if (!line[e]) {
                report((const char*)0x5dcc0c);
                break;
            }
            if (line[e] != '"') {
                report((const char*)0x5dcbdc);
                break;
            }
            int32_t start = ++e, p = e;
            if (line[p]) {
                for (;;) {
                    if (line[p] == '"') break;
                    if (!raw) {
                        for (int pass = 0; pass < 2; ++pass) {
                            char ch = line[p];
                            if (pass == 0 ? (ch == '{' || ch == '[') : (ch == '}' || ch == ']')) {
                                Num_Format(num, 1, (double)lineNo, 10, 1, 0, 0);
                                sprintf_(msg, (const char*)0x5dcc7c, (int)(signed char)ch, num, line);
                                DebugLog(msg, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234);
                                line[p] = pass == 0 ? '(' : ')';
                            }
                        }
                    }
                    if (LeadByte(line[p])) ++p;
                    char c2 = line[p + 1];
                    ++p;
                    if (!c2) break;
                }
            }
            if (line[p] != '"') report((const char*)0x5dcc58);
            line[p] = 0;
            String_Assign(&l->items[id], &line[start]);
            line[p] = '"';
            char* s = l->items[id];
            ++p;
            size_t len = strlen(s);
            if (*s) {
                for (;;) {
                    if (s[0] == '\\' && s[1] == 'n') {
                        *s = '\n';
                        memmove_(s + 1, s + 2, (size_t)(l->items[id] - s) + len - 1);
                    }
                    if (LeadByte(*s)) ++s;
                    char c2 = s[1];
                    ++s;
                    if (!c2) break;
                }
            }
            if (!(maxId > id)) maxId = id;
            if (line[p]) {
                char c2;
                do {
                    if ((signed char)line[p] > ' ') break;
                    if (LeadByte(line[p])) ++p;
                    c2 = line[p + 1];
                    ++p;
                } while (c2);
                if (line[p]) {
                    if (line[p] != ';') report((const char*)0x5dcc2c);
                    break;
                }
            }
            j = p;
        }
    } while (more);
    StreamClose(f);
    size_t size = (size_t)maxId * 4 + 4;
    char** items = (char**)malloc_(size);
    memcpy(items, l->items, size);
    ((void(__cdecl*)(void*))0x5a1145)(l->items);
    l->items = items;
    l->count = maxId + 1;
    BuildNameIndex();
    return l;
}
RT3_REPLACE(0x51c020, Strings_LoadFile);

void Language_Select();
void __fastcall DebugLog_SetFile(const char* name);
void DebugLog_WriteHeader();
void __fastcall StringList_Free(StringList* l);
void __fastcall FreeAndNull(void** p);
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);

// 0x51cf80. Loads the language once (0x6d3c9c). It picks the language folder
// (0x6d3c90), loads "data\language\[<folder>\]RT3.lng" into 0x6d3c94, then
// tells the language 0x6d3c84 from how text 0x5be ("Issue Bonds") reads.
// Text 4040 marks a double-byte language (0x6d3c8c = 3, option 0x6d4034).
// Text 4041 gives the character set 0x6d3c88, which sets the code page
// 0x624328 and the C locale. Texts 0x283 / 0x284 give the decimal and
// thousands separators (0x62432c / 0x62432d).
RT3_NOINLINE void Language_Load() {
    if (hook::Global<int32_t>(0x6d3c9c)) return;
    hook::Global<int32_t>(0x6d3c9c) = 1;
    Language_Select();
    char path[0x12c];
    const char* folder = hook::Global<const char*>(0x6d3c90);
    FormatText(path, folder ? (const char*)0x5dce20 : (const char*)0x5dce00, folder, 0, 0, 0, 0, 0, 0, 0);
    char* p = path;
    if (*p)
        while (*p != ' ' && *++p) {
        }
    *p = 0;
    DebugLog_SetFile((const char*)0x5dcdec);
    DebugLog_WriteHeader();
    if (hook::Global<StringList*>(0x6d3c94)) {
        StringList_Free(hook::Global<StringList*>(0x6d3c94));
        FreeAndNull((void**)0x6d3c94);
    }
    hook::Global<StringList*>(0x6d3c94) = Strings_LoadFile(path);
    DebugLog_SetFile((const char*)0x5dcddc);
    hook::Global<int32_t>(0x6d3c84) = 0;
    static const struct { uint32_t name; int32_t lang; } kLangs[] = {
        { 0x5dcdd0, 0 }, { 0x5dcdbc, 1 }, { 0x5dcda8, 2 }, { 0x5dcd94, 3 }, { 0x5dcd84, 4 },
        { 0x5dcd70, 8 }, { 0x5dcd5c, 10 }, { 0x5dcd50, 11 },
    };
    bool found = false;
    for (auto& l : kLangs)
        if (StrFind(GetString(0x5be), (const char*)l.name, 0)) {
            hook::Global<int32_t>(0x6d3c84) = l.lang;
            found = true;
            break;
        }
    if (!found) hook::Global<int32_t>(0x6d3c84) = StrFind(GetString(0x5be), (const char*)0x5cec80, 0) ? 12 : 6;
    if (strlen(GetString(0xfc8)) >= 2) {
        hook::Global<int32_t>(0x6d3c8c) = 3;
        hook::Global<int32_t>(0x6d4034) = 1;
    }
    if (strlen(GetString(0xfc9))) hook::Global<int32_t>(0x6d3c88) = ((int32_t(__cdecl*)(const char*))0x5a1ea5)(GetString(0xfc9));
    auto setlocale_ = (char*(__cdecl*)(int, const char*))0x5a3c18;
    switch (hook::Global<int32_t>(0x6d3c88)) {
    case 0x80: hook::Global<int32_t>(0x624328) = 0x3a4; setlocale_(0, (const char*)0x5dcd44); break;
    case 0x81: hook::Global<int32_t>(0x624328) = 0x3b5; setlocale_(0, (const char*)0x5dcd14); break;
    case 0x86: hook::Global<int32_t>(0x624328) = 0x3b6; setlocale_(0, (const char*)0x5dcd1c); break;
    case 0x88: hook::Global<int32_t>(0x624328) = 0x3b6; setlocale_(0, (const char*)0x5dcd30); break;
    case 0xde: hook::Global<int32_t>(0x624328) = 0x36a; setlocale_(0, (const char*)0x5dcd0c); break;
    }
    char sep[0x3e8];
    strcpy(sep, GetString(0x283));
    if (strlen(sep)) hook::Global<char>(0x62432c) = sep[0];
    strcpy(sep, GetString(0x284));
    if (strlen(sep)) hook::Global<char>(0x62432d) = sep[0];
}
RT3_REPLACE(0x51cf80, Language_Load);

} // namespace rt3
