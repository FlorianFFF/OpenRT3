// String helpers used all over the game.
#include "hook/hook.h"
#include "core/crt.h"
#include <string.h>
#include <stdint.h>

namespace rt3 {

// 0x51d660. Uppercases a-z and the Latin-1 range 0xf0..0xff (by -0x20);
// everything else passes through. Char arrives in CL.
int __fastcall ToUpper(char c) {
    unsigned char u = (unsigned char)c;
    if ((u >= 'a' && u <= 'z') || u >= 0xf0) return u - 0x20;
    return u;
}
RT3_REPLACE(0x51d660, ToUpper);

// 0x518de0. The game's text formatter (strings from RT3.lng use it):
//   %1..%8 (and %10.. two digits) insert argument N; %S inserts the next
//   argument in sequence; %% is a literal percent. Any other %x is copied as is.
// QUIRKS kept from the original:
//   - "%05" reads as argument 5 (05) but only advances 2 chars, so "5" is
//     then copied literally.
//   - Arguments 8 and above, and %0, all take the 8th argument.
//   - A trailing "%%" copies the terminator and keeps reading past it.
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2,
                            const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8) {
    char* out = dst;
    const char* p = fmt;
    if (*p != '\0') {
        int seq = 1;
        do {
            if (*p == '%') {
                if (p[1] == '%') {
                    *out++ = '%';
                    p += 2;
                }
                if (*p != '%') goto literal;
                bool isS = ToUpper(p[1]) == 'S';
                if (!isS && (p[1] < '0' || p[1] > '9')) goto literal;
                int n;
                if (ToUpper(p[1]) == 'S') {
                    n = seq;
                } else {
                    n = p[1] - '0';
                    if (p[2] >= '0' && p[2] <= '9') n = n * 10 + p[2] - '0';
                }
                const char* arg;
                switch (n) {
                case 1: arg = a1; break;
                case 2: arg = a2; break;
                case 3: arg = a3; break;
                case 4: arg = a4; break;
                case 5: arg = a5; break;
                case 6: arg = a6; break;
                case 7: arg = a7; break;
                default: arg = a8; break;
                }
                if (arg) {
                    size_t len = strlen(arg);
                    memcpy(out, arg, len + 1);
                    out += len;
                }
                p += (n >= 10 && ToUpper(p[1]) != 'S') ? 3 : 2;
                seq++;
                continue;
            }
        literal:
            *out++ = *p++;
        } while (*p != '\0');
    }
    *out = '\0';
    return dst;
}
RT3_REPLACE(0x518de0, FormatText);

// {int count; char** items}
struct StringList {
    int32_t count;
    char**  items;
};

// 0x518f60
void __fastcall StringList_Free(StringList* l) {
    if (!l->items) return;
    for (int32_t i = 0; i < l->count; ++i)
        if (l->items[i]) Free(l->items[i]);
    FreeAndNull((void**)&l->items);
}
RT3_REPLACE(0x518f60, StringList_Free);

// 0x5193d0. Frees the strings loaded from the language file (0x6d3c94).
void FreeLanguageStrings() {
    StringList*& args = hook::Global<StringList*>(0x6d3c94);
    if (!args) return;
    StringList_Free(args);
    FreeAndNull((void**)&args);
}
RT3_REPLACE(0x5193d0, FreeLanguageStrings);

// 0x51d800
void __fastcall String_Free(char** s) {
    if (*s) {
        Free(*s);
        *s = nullptr;
    }
}
RT3_REPLACE(0x51d800, String_Free);

// 0x51d820. Replaces *s with a heap copy of src (or NULL).
void __fastcall String_Assign(char** s, const char* src) {
    if (*s) {
        Free(*s);
        *s = nullptr;
    }
    if (src) {
        size_t n = strlen(src) + 1;
        *s = (char*)Malloc(n);
        memcpy(*s, src, n);
    }
}
RT3_REPLACE(0x51d820, String_Assign);

// 0x51d8a0. strncpy that always terminates (dst needs n+1 bytes).
void __fastcall StrCopyN(char* dst, const char* src, size_t n) {
    strncpy(dst, src, n);
    dst[n] = '\0';
}
RT3_REPLACE(0x51d8a0, StrCopyN);

// 0x51d8c0. Appends src while the total stays within n chars (dst needs n+1).
void __fastcall StrCatN(char* dst, const char* src, int32_t n) {
    int32_t len = (int32_t)strlen(dst);
    int32_t room = n - len;
    if (room > 0) {
        strncpy(dst + len, src, room);
        dst[len + room] = '\0';
    }
}
RT3_REPLACE(0x51d8c0, StrCatN);

// 0x51d900. strstr, case-insensitive unless caseSensitive. NULL if either is empty.
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive) {
    int32_t hayLen = (int32_t)strlen(hay);
    int32_t len = (int32_t)strlen(needle);
    if (hayLen == 0 || len > hayLen) return nullptr;
    int32_t tries = hayLen - len + 1;
    for (int32_t i = 0; i < tries; ++i) {
        int r = caseSensitive ? strncmp(hay + i, needle, len) : Strnicmp(hay + i, needle, len);
        if (r == 0) return hay + i;
    }
    return nullptr;
}
RT3_REPLACE(0x51d900, StrFind);

// 0x51d9a0. 1 if s is NULL or only spaces/control characters.
int32_t __fastcall StrIsBlank(const unsigned char* s) {
    if (s) {
        for (; *s; ++s)
            if (*s > 0x20) return 0;
    }
    return 1;
}
RT3_REPLACE(0x51d9a0, StrIsBlank);

} // namespace rt3

namespace rt3 {

const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);

// 0x51c900. True if string `id` is non-empty.
int32_t __fastcall HasString(int32_t id) { return GetString(id)[0] != '\0' ? 1 : 0; } // full 0/1 in EAX
RT3_REPLACE(0x51c900, HasString);

// 0x51c920. Localizes an internal name ("Coal", ...) through the name table at
// 0x6243c8 ({name, string id} x 235, sorted by first letter; per-letter index
// ranges at 0x6d3b99 built by 0x51b660). Unknown names come back unchanged.
const char* __fastcall LocalizeName(const char* name) {
    struct Entry { const char* name; int32_t stringId; };
    const Entry* table = (const Entry*)0x6243c8;
    char c = (char)ToUpper(name[0]);
    if (c < 'A' || c > 'Z') c = 'A';
    const uint8_t* range = (const uint8_t*)0x6d3950 + c * 9;
    int32_t first = *(const int32_t*)range, last = *(const int32_t*)(range + 4);
    for (int32_t i = first; i <= last; ++i) {
        if ((char)ToUpper(table[i].name[1]) != (char)ToUpper(name[1])) continue;
        if (Stricmp(table[i].name, name) != 0) continue;
        if (GetString(table[i].stringId)[0] != '\0') return GetString(table[i].stringId);
    }
    // (The original also formats a "missing name" message into a local buffer
    // and discards it; that has no effect and is left out.)
    return name;
}
RT3_REPLACE(0x51c920, LocalizeName);

// 0x51d680. Appends "\n<text><ints...>" to .\data\log\<0x6d3ca8> when debug
// logging (0x6d3ca0) is on. Ints equal to -1234 are skipped.
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                         int32_t f, int32_t g, int32_t h, int32_t i) {
    if (!hook::Global<int32_t>(0x6d3ca0)) return;
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    int32_t v[9] = { a, b, c, d, e, f, g, h, i };
    char line[2000] = "\n";
    strcat(line, text);
    for (int k = 0; k < 9; ++k) {
        if (v[k] == -1234) continue;
        char num[200];
        sprintf_(num, "% 12d", v[k]);
        strcat(line, num);
    }
    char path[499];
    sprintf_(path, ".\\data\\log\\%s", (const char*)0x6d3ca8);
    int fd = Open(path, 0x4009, 0x80); // _O_WRONLY | _O_APPEND | _O_TEXT
    if (fd != -1) {
        Write(fd, line, (unsigned)strlen(line));
        Close(fd);
    }
}
RT3_REPLACE(0x51d680, DebugLog);

// 0x51d9d0. Splits one config line in place into tokens (separators: space,
// tab, comma; ';' starts a comment). Advances *pos past the line.
// Returns 0 if the text was already at its end.
// QUIRK: a token ended directly by ';' ("abc;comment") is stored twice.
int32_t __fastcall TokenizeLine(char* base, int32_t* pos, int32_t* count, char** tokens) {
    char* p = base + *pos;
    int32_t ret = 1, notEnd = 1, more = 1, n = 0;
    if (*p == '\0') ret = 0;
    for (;;) {
        while (*p == ' ' || *p == '\t') ++p;
        char* tok = p;
        bool emptyComment = false;
        if (*p) {
            for (;;) {
                char ch = *p;
                if (ch == ',' || ch == ' ' || ch == '\t' || ch == ';' || ch == '\n' || ch == '\r') break;
                if (*++p == '\0') break;
            }
        }
        if (*p == ';') {
            if (p - tok > 0) tokens[n++] = tok;
            else emptyComment = true;
            *p = '\0';
            do { ++p; } while (*p && *p != '\n' && *p != '\r');
        }
        if (*p == '\n' || *p == '\r') {
            bool add = (p - tok > 0) && !emptyComment;
            *p++ = '\0';
            if (add) tokens[n++] = tok;
            while (*p == '\n') ++p;
            more = 0;
        }
        if (*p == '\0') notEnd = 0;
        if (!emptyComment && (*p == ' ' || *p == ',' || *p == '\t')) {
            *p = '\0';
            if (tok[0] != '\0') tokens[n++] = tok;
            ++p;
        }
        if (!notEnd || !more) break;
    }
    *pos = (int32_t)(p - base);
    *count = n;
    return ret;
}
RT3_REPLACE(0x51d9d0, TokenizeLine);

// 0x51dc60. Bubble sort of `count` records of `size` bytes by the string at
// `keyOffset`, case-insensitive (exe stricmp).
void __fastcall SortRecordsByName(uint8_t* records, int32_t count, uint32_t size, int32_t keyOffset) {
    uint8_t* tmp = (uint8_t*)Malloc(size);
    for (int32_t pass = count - 1; pass > 0; --pass) {
        for (int32_t i = 0; i < pass; ++i) {
            uint8_t* a = records + i * size;
            uint8_t* b = a + size;
            if (Stricmp((const char*)b + keyOffset, (const char*)a + keyOffset) < 0) {
                memcpy(tmp, a, size);
                memcpy(a, b, size);
                memcpy(b, tmp, size);
            }
        }
    }
    Free(tmp);
}
RT3_REPLACE(0x51dc60, SortRecordsByName);

// 0x51dd60. Trims trailing spaces/NULs (and CR/LF if asked) and leading spaces.
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines) {
    for (int32_t i = (int32_t)strlen(s); i >= 0; --i) {
        char c = s[i];
        if (c == ' ' || c == '\0' || (alsoNewlines && (c == '\n' || c == '\r'))) s[i] = '\0';
        else break;
    }
    int32_t len = (int32_t)strlen(s), lead = 0;
    if (len > 0) {
        while (lead < len && s[lead] == ' ') ++lead;
        if (lead) memmove(s, s + lead, len - lead + 1);
    }
}
RT3_REPLACE(0x51dd60, TrimSpaces);

// 0x51dde0. Copies src up to its first '.' into dst. QUIRK: if there is no
// '.', dst is not terminated. Returns dst (the save loader formats it).
char* __fastcall CopyBaseName(const char* src, char* dst) {
    for (int32_t i = 0; src[i] != '\0'; ++i) {
        if (src[i] == '.') { dst[i] = '\0'; break; }
        dst[i] = src[i];
    }
    return dst;
}
RT3_REPLACE(0x51dde0, CopyBaseName);

// 0x51df50. Power of two: v itself if it is one, else the next larger (roundUp)
// or smaller one. 0 if v > 2^30.
int32_t __fastcall PowerOfTwo(int32_t v, int32_t roundUp) {
    int32_t bit = 0;
    while ((1 << bit) < v) {
        if (++bit > 30) return 0;
    }
    if (v == (1 << bit)) return v;
    return roundUp ? (1 << bit) : (1 << ((bit - 1) & 31));
}
RT3_REPLACE(0x51df50, PowerOfTwo);

// 0x51df90. Cuts the extension (last '.'; not if it is the first char).
void __fastcall StripExtension(char* s) {
    char* p = s + strlen(s) - 1;
    if (p == s) return;
    while (*p != '.') {
        if (--p == s) return;
    }
    *p = '\0';
}
RT3_REPLACE(0x51df90, StripExtension);

} // namespace rt3

namespace rt3 {

static inline int ExeIsLeadByte(unsigned c) { return ((int(__cdecl*)(unsigned))0x5a330d)(c); }

// 0x51b600. In DBCS mode (0x6d3c8c == 3): the exe CRT's lead-byte test on the
// high byte of a 16-bit char. 0 otherwise.
int32_t __fastcall IsLeadByte(uint16_t c) {
    if (hook::Global<int32_t>(0x6d3c8c) != 3) return 0;
    if (c > 0xff) c >>= 8;
    return ExeIsLeadByte(c);
}
RT3_REPLACE(0x51b600, IsLeadByte);

// 0x51b630. True in DBCS mode when c is not a lead byte.
int32_t __fastcall IsSingleByte(uint16_t c) {
    if (hook::Global<int32_t>(0x6d3c8c) != 3) return false;
    if (c > 0xff) c >>= 8;
    return ExeIsLeadByte(c) == 0;
}
RT3_REPLACE(0x51b630, IsSingleByte);

// 0x51b660. Per-letter index into the name table (0x6243c8, 235 entries):
// 26 records of {char letter; int first; int last} at 0x6d3b98. Names that
// start with '2' count as 'A'.
#pragma pack(push, 1)
struct LetterRange { char letter; int32_t first; int32_t last; };
#pragma pack(pop)
void BuildNameIndex() {
    LetterRange* idx = (LetterRange*)0x6d3b98;
    const char* const* table = (const char* const*)0x6243c8; // {name, id} pairs
    for (int i = 0; i < 26; ++i) {
        idx[i].letter = (char)('A' + i);
        idx[i].first = -1;
        idx[i].last = -2;
    }
    char cur = 'A';
    int32_t runStart = 0, i = 0;
    for (; i < 0xeb; ++i) {
        char c = table[i * 2][0];
        if (c == '2') c = 'A';
        if (c != cur) {
            idx[cur - 'A'].first = runStart;
            idx[cur - 'A'].last = i - 1;
            runStart = i;
            cur = c;
        }
    }
    idx[cur - 'A'].first = runStart;
    idx[cur - 'A'].last = i - 1;
}
RT3_REPLACE(0x51b660, BuildNameIndex);

// 0x51b6f0. AL = (language == English); the upper bytes of EAX keep the
// language value, as in the original.
uint32_t IsEnglish() {
    uint32_t lang = hook::Global<uint32_t>(0x6d3c84);
    return (lang & 0xffffff00u) | (lang == 0);
}
RT3_REPLACE(0x51b6f0, IsEnglish);

} // namespace rt3
