// TextDate.cpp - RT3_mp.exe 0x51a000-0x51e000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"
#include "core/crt.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
static DWORD& g_clockStart = hook::Global<DWORD>(0x6d3d14);
struct StringList { int32_t count; char** items; };
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void BuildNameIndex();
void __fastcall String_Assign(char** s, const char* src);
void Language_Select();
void __fastcall DebugLog_SetFile(const char* name);
void DebugLog_WriteHeader();
void __fastcall StringList_Free(StringList* l);
void __fastcall FreeAndNull(void** p);
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);

// 0x51b660. Per-letter index into the name table (0x6243c8, 235 entries):
// 26 records of {char letter; int first; int last} at 0x6d3b98. Names that
// start with '2' count as 'A'.
#pragma pack(push, 1)
struct LetterRange { char letter; int32_t first; int32_t last; };
#pragma pack(pop)

int32_t __fastcall IsLeadByte(uint16_t c);
int32_t __fastcall IsSingleByte(uint16_t c);

// Functions of this file, in exe order.
RT3_NOINLINE const char* __fastcall GetString(int32_t id);
int32_t __fastcall IsLeadByte(uint16_t c);
int32_t __fastcall IsSingleByte(uint16_t c);
RT3_NOINLINE void BuildNameIndex();
RT3_NOINLINE uint32_t IsEnglish();
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE StringList* __fastcall Strings_LoadFile(const char* path);
RT3_NOINLINE int32_t __fastcall HasString(int32_t id);
RT3_NOINLINE const char* __fastcall LocalizeName(const char* name);
RT3_NOINLINE char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
RT3_NOINLINE void Language_Load();
RT3_NOINLINE int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d);
RT3_NOINLINE uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
uint64_t __fastcall DateFromMinutes(int32_t m);
uint64_t __fastcall DateAddMinutes(int32_t minutes, GameDate d);
RT3_NOINLINE uint64_t __fastcall DateAddDays(int32_t days, GameDate d);
int32_t __stdcall DateDiffMinutes(GameDate a, GameDate b);
RT3_NOINLINE int __fastcall ToUpper(char c);
RT3_NOINLINE void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
RT3_NOINLINE void __fastcall String_Free(char** s);
RT3_NOINLINE void __fastcall String_Assign(char** s, const char* src);
void StartClock();
RT3_NOINLINE int32_t ClockMs();
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall StrCatN(char* dst, const char* src, int32_t n);
RT3_NOINLINE const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
int32_t __fastcall StrIsBlank(const unsigned char* s);
int32_t __fastcall TokenizeLine(char* base, int32_t* pos, int32_t* count, char** tokens);
RT3_NOINLINE double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
int32_t __fastcall DistanceSqInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void __fastcall SortRecordsByName(uint8_t* records, int32_t count, uint32_t size, int32_t keyOffset);
RT3_NOINLINE void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
RT3_NOINLINE char* __fastcall CopyBaseName(const char* src, char* dst);
uint32_t __fastcall BlendColor(uint32_t a, uint32_t b, float t);
int32_t __fastcall PowerOfTwo(int32_t v, int32_t roundUp);
RT3_NOINLINE void __fastcall StripExtension(char* s);
RT3_NOINLINE uint32_t __stdcall HeatColor(float t);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// One channel of the ramp: rises from k to 1 below 0.5, falls to 0 after.
static inline double Ramp(double v) {
    if (v < D(0x5dce78)) return D(0x5dce70) * v * D(0x5c88e8) + D(0x5c8880);
    if (v < D(0x5c8628)) return 1.0;
    if (v < D(0x5dce68)) return D(0x5c8770) - (v - D(0x5c8628)) * D(0x5dce60);
    return 0.0;
}

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

static inline void StrCat(char* dst, const char* src) { memcpy(dst + strlen(dst), src, strlen(src) + 1); }

static inline int32_t Abs32(int32_t v) { return (int32_t)(v < 0 ? 0u - (uint32_t)v : (uint32_t)v); }

static inline uint64_t Pack(const GameDate& d) {
    uint64_t v;
    memcpy(&v, &d, 8);
    return v;
}

static inline int32_t DaysOf(const GameDate& d) { // days since year 0, month 1, day 1
    return (d.year * 12 + d.month) * 28 + d.day - 29;
}

// Through the exe's import slot, like the original (the harness fakes it).
static inline DWORD Ticks() { return ((DWORD(WINAPI*)())*(void**)0x5c8068)(); }

static bool LeadByte(char c) {
    return hook::Global<int32_t>(0x6d3c8c) == 3 && ((int(__cdecl*)(int))0x5a330d)((uint8_t)c) != 0;
}

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

RT3_NOINLINE void BuildNameIndex() {
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
RT3_NOINLINE uint32_t IsEnglish() {
    uint32_t lang = hook::Global<uint32_t>(0x6d3c84);
    return (lang & 0xffffff00u) | (lang == 0);
}
RT3_REPLACE(0x51b6f0, IsEnglish);

// 0x51b700. The game's number formatter, into `buf`. Styles: 1 integer with
// thousands separators (or "N,000" / an error string past 1e9 / 1e12), 2 / 13 the
// value in thousands / millions with the K / M suffix, 3 / 4 two / one decimals,
// 5 / 6 / 7 a percentage (x100) with 0 / 1 / 2 decimals, 8 "%.f", 9 money ($,
// falling back to K and M while wider than `width`), 10 money with cents, 11 / 12
// money in K / M. Style 1 also shrinks to K / M (suffix strings 0x281 / 0x282)
// while longer than `width` (+4 before going straight to M); `inK` / `inM` say the
// value is already scaled. With the engine flag +0x114732 set, styles 2 / 13 fall
// back to 1 and 11 / 12 to 9. At the outermost level ',' and '.' become the
// locale's separators (0x62432c / 0x62432d). `unused` is passed along but never
// read. Unknown styles leave buf untouched (but the separator pass still runs over
// whatever it holds, up to the first NUL). Quirks: the K / M money styles round
// (+0.5) but print no sign below 1 unit, and the one/two-decimal styles print the
// truncated integer part and then |fraction|, so values between -1 and 0 lose
// their minus sign (-0.5 shows as "0.4") and negative fractions round the wrong
// way (-1.27 shows as "-1.2").
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM) {
    int32_t& depth = hook::Global<int32_t>(0x6d3c98);
    ++depth;
    int32_t iv = Ftol(v);
    uint8_t* engine = hook::Global<uint8_t*>(0x6d4024);
    if (engine && F<int32_t>(engine, 0x114732)) {
        if (style == 2 || style == 13) style = 1;
        else if (style == 11 || style == 12) style = 9;
    }
    char tmp[0x60];
    SprintfFn sprintf_ = ExeSprintf();
    switch (style) {
    case 1: {
        double a = fabs(v);
        if (a < 1000.0) {
            sprintf_(buf, "%d", iv);
        } else if (a < 1000000.0) {
            sprintf_(buf, "%d,%03d", iv / 1000, Abs32(iv) % 1000);
        } else if (a < 1000000000.0) {
            int32_t m = Abs32(iv);
            sprintf_(buf, "%d,%03d,%03d", iv / 1000000, m % 1000000 / 1000, m % 1000);
        } else if (a < 1000000000000.0) {
            Num_Format(buf, style, v * 0.001, 999, 0, 0, 0);
            StrCat(buf, ",000");
        } else {
            FormatText(buf, GetString(0x40), 0, 0, 0, 0, 0, 0, 0, 0);
        }
        int32_t len = (int32_t)strlen(buf);
        if (len > width + 4 && !inK && !inM) {
            Num_Format(buf, style, v * 1e-06, width, 0, 0, 1);
        } else if ((int32_t)strlen(buf) > width && !inM) {
            if (inK) Num_Format(buf, style, v * 0.001, width, 0, 0, 1);
            else Num_Format(buf, style, v * 0.001, width, 0, 1, 0);
        } else {
            if (inK) StrCat(buf, GetString(0x281));
            if (inM) StrCat(buf, GetString(0x282));
        }
        break;
    }
    case 2:
    case 13: {
        int32_t neg = v < 0.0;
        double scaled = fabs(v * (style == 2 ? 0.001 : 1e-06));
        Num_Format(tmp, 1, scaled, width - neg - 1, unused, style == 2, style == 13);
        sprintf_(buf, "%s%s", v < 0.0 ? "-" : "", tmp);
        break;
    }
    case 3:
    case 4: {
        int32_t digits = Ftol((v - (double)iv + (style == 3 ? 0.005 : 0.05)) * (style == 3 ? 100.0 : 10.0));
        int32_t cap = style == 3 ? 99 : 9;
        if (digits >= cap) digits = cap;
        Num_Format(tmp, 1, v, 10, 0, 0, 0);
        sprintf_(buf, style == 3 ? "%s.%02d" : "%s.%01d", tmp, Abs32(digits));
        break;
    }
    case 5:
    case 6:
    case 7:
        sprintf_(buf, "%s%s", Num_Format(tmp, style == 5 ? 1 : style == 6 ? 4 : 3, v * 1.00001 * 100.0, 10, 0, 0, 0), "%");
        break;
    case 8:
        sprintf_(buf, "%.f", v);
        break;
    case 9: {
        for (int k = 0; k < 3; ++k) {
            int32_t neg = v < 0.0;
            double scaled = k == 0 ? fabs(v) : fabs(v * (k == 1 ? 0.001 : 1e-06));
            Num_Format(tmp, 1, scaled, width - neg - 1, 1, 0, 0);
            sprintf_(buf, "%s$%s", v < 0.0 ? "-" : "", tmp);
            if (k) StrCat(buf, GetString(k == 1 ? 0x281 : 0x282));
            if (k < 2 && (int32_t)strlen(buf) <= width) break;
        }
        break;
    }
    case 10: {
        Num_Format(tmp, 3, fabs(v), 10, 1, 0, 0);
        sprintf_(buf, "%s$%s", Ftol(v * 100.0) < 0 ? "-" : "", tmp);
        break;
    }
    case 11:
    case 12: {
        float f = (float)(fabs(v) * (style == 11 ? 0.001 : 1e-06) + 0.5);
        int32_t neg = v < 0.0;
        Num_Format(tmp, 1, (double)f, width - neg - 1, unused, style == 11, style == 12);
        sprintf_(buf, "%s$%s", !(f >= 1.0f) ? "" : v < 0.0 ? "-" : "", tmp);
        break;
    }
    }
    if (depth == 1 && *buf) {
        char comma = hook::Global<char>(0x62432c), point = hook::Global<char>(0x62432d);
        for (char* p = buf; *p; ++p) {
            if (*p == ',') *p = comma;
            else if (*p == '.') *p = point;
        }
    }
    --depth;
    return buf;
}
RT3_REPLACE(0x51b700, Num_Format);

// 0x51bfe0 / 0x51c000 (two identical copies). Num_Format with the unused flag set.
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width) { return Num_Format(buf, style, v, width, 1, 0, 0); }
RT3_REPLACE(0x51bfe0, Num_FormatSimple);

RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width) { return Num_Format(buf, style, v, width, 1, 0, 0); }
RT3_REPLACE(0x51c000, Num_FormatSimple2);

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

// 0x51c900. True if string `id` is non-empty.
RT3_NOINLINE int32_t __fastcall HasString(int32_t id) { return GetString(id)[0] != '\0' ? 1 : 0; }

 // full 0/1 in EAX
RT3_REPLACE(0x51c900, HasString);

// 0x51c920. Localizes an internal name ("Coal", ...) through the name table at
// 0x6243c8 ({name, string id} x 235, sorted by first letter; per-letter index
// ranges at 0x6d3b99 built by 0x51b660). Unknown names come back unchanged.
RT3_NOINLINE const char* __fastcall LocalizeName(const char* name) {
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

// 0x51ca00. Formats date `d` (year +0 word, month +2, day +4, hour +5, minute
// +6) into `buf` by `mode` for the language 0x6d3c84: 1 "month year", 2 the
// long date (day / month / year in the language's order; month names
// 0x62438c), 3 "month day", 4 the time ("h:mm am/pm" in English, else 24h),
// 5 "month day, year" (month names 0x62435c). Japanese (7) uses its year
// marks. Other modes leave `buf` alone. Returns `buf`.
RT3_NOINLINE char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode) {
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    const int32_t* shortNames = (const int32_t*)0x62435c;
    const int32_t* longNames = (const int32_t*)0x62438c;
    char ya[0x40], yb[0x40];
    auto num = [](char* t, int32_t style, int32_t v) { return Num_Format(t, style, (double)v, 10, 1, 0, 0); };
    switch (mode) {
    case 1: {
        const char* year = num(ya, 8, F<uint16_t>((void*)d, 0));
        FormatText(buf, lang == 7 ? (const char*)0x5dcce8 : (const char*)0x5d0070, GetString(shortNames[d[2]]), year, 0, 0, 0, 0, 0, 0);
        break;
    }
    case 2: {
        if (lang == 7) {
            const char* day = num(ya, 1, d[4]);
            const char* year = num(yb, 8, F<uint16_t>((void*)d, 0));
            FormatText(buf, (const char*)0x5dccd0, GetString(longNames[d[2]]), year, day, 0, 0, 0, 0, 0);
        } else if (lang >= 1 && lang <= 5) {
            const char* year = num(ya, 8, F<uint16_t>((void*)d, 0));
            const char* month = GetString(longNames[d[2]]);
            const char* day = num(yb, 8, d[4]);
            const char* fmt = lang == 1 ? (const char*)0x5dccdc : lang <= 3 ? (const char*)0x5d0d5c : (const char*)0x5dccc4;
            FormatText(buf, fmt, day, month, year, 0, 0, 0, 0, 0);
        } else {
            const char* year = num(ya, 8, F<uint16_t>((void*)d, 0));
            const char* day = num(yb, 8, d[4]);
            FormatText(buf, (const char*)0x5dccc4, GetString(longNames[d[2]]), day, year, 0, 0, 0, 0, 0);
        }
        break;
    }
    case 3: {
        const char* day = num(ya, 8, d[4]);
        FormatText(buf, (const char*)0x5d0070, GetString(shortNames[d[2]]), day, 0, 0, 0, 0, 0, 0);
        break;
    }
    case 4:
        if (lang) {
            ExeSprintf()(buf, (const char*)0x5dcd04, d[5], d[6]);
        } else {
            int32_t h = d[5] % 12;
            if (!h) h = 12;
            ExeSprintf()(buf, (const char*)0x5dccf0, h, d[6], d[5] >= 12 ? (const char*)0x5dcd00 : (const char*)0x5dccfc);
        }
        break;
    case 5: {
        if (lang == 7) {
            const char* year = num(ya, 8, F<uint16_t>((void*)d, 0));
            FormatText(buf, (const char*)0x5dcce8, GetString(shortNames[d[2]]), year, 0, 0, 0, 0, 0, 0);
        } else {
            const char* year = num(ya, 8, F<uint16_t>((void*)d, 0));
            const char* day = num(yb, 8, d[4]);
            FormatText(buf, (const char*)0x5dccc4, GetString(shortNames[d[2]]), day, year, 0, 0, 0, 0, 0);
        }
        break;
    }
    }
    return buf;
}
RT3_REPLACE(0x51ca00, Date_Format);

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

RT3_NOINLINE int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute) {
    return (((year * 12 + month) * 28 - 29 + day) * 24 + hour) * 60 + minute;
}
RT3_REPLACE(0x51d390, MinutesFromParts);

RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d) {
    return (DaysOf(*d) * 24 + d->hour) * 60 + d->minute;
}
RT3_REPLACE(0x51d3c0, DateToMinutes);

RT3_NOINLINE uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute) {
    GameDate d;
    d.year = year;
    d.month = month;
    d.week = (uint8_t)(day / 7 + 1);
    d.day = day;
    d.hour = hour;
    d.minute = minute;
    d.quarter = (uint8_t)(hour / 6 + 1);
    return Pack(d);
}
RT3_REPLACE(0x51d3f0, MakeDate);

uint64_t __fastcall DateFromMinutes(int32_t m) {
    GameDate d;
    d.year = (uint16_t)(m / kMinutesPerYear);
    m -= d.year * kMinutesPerYear; // QUIRK: uses the 16-bit year, as the original
    d.month = (uint8_t)(m / kMinutesPerMonth + 1);
    m -= (d.month - 1) * kMinutesPerMonth;
    d.day = (uint8_t)(m / kMinutesPerDay + 1);
    m -= (d.day - 1) * kMinutesPerDay;
    uint8_t hour = (uint8_t)(m / 60);
    d.week = (uint8_t)(d.day / 7 + 1);
    d.hour = hour;
    d.minute = (uint8_t)(m - hour * 60);
    d.quarter = (uint8_t)(hour / 6 + 1);
    return Pack(d);
}
RT3_REPLACE(0x51d460, DateFromMinutes);

uint64_t __fastcall DateAddMinutes(int32_t minutes, GameDate d) {
    return DateFromMinutes(minutes + (DaysOf(d) * 24 + d.hour) * 60 + d.minute);
}
RT3_REPLACE(0x51d550, DateAddMinutes);

RT3_NOINLINE uint64_t __fastcall DateAddDays(int32_t days, GameDate d) {
    return DateFromMinutes(((days + DaysOf(d)) * 24 + d.hour) * 60 + d.minute);
}
RT3_REPLACE(0x51d5a0, DateAddDays);

int32_t __stdcall DateDiffMinutes(GameDate a, GameDate b) {
    return ((((a.year - b.year) * 12 - b.month + a.month) * 28 - b.day + a.day) * 24
            - b.hour + a.hour) * 60 - b.minute + a.minute;
}
RT3_REPLACE(0x51d5f0, DateDiffMinutes);

// 0x51d660. Uppercases a-z and the Latin-1 range 0xf0..0xff (by -0x20);
// everything else passes through. Char arrives in CL.
RT3_NOINLINE int __fastcall ToUpper(char c) {
    unsigned char u = (unsigned char)c;
    if ((u >= 'a' && u <= 'z') || u >= 0xf0) return u - 0x20;
    return u;
}
RT3_REPLACE(0x51d660, ToUpper);

// 0x51d680. Appends "\n<text><ints...>" to .\data\log\<0x6d3ca8> when debug
// logging (0x6d3ca0) is on. Ints equal to -1234 are skipped.
RT3_NOINLINE void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
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

// 0x51d800
RT3_NOINLINE void __fastcall String_Free(char** s) {
    if (*s) {
        Free(*s);
        *s = nullptr;
    }
}
RT3_REPLACE(0x51d800, String_Free);

// 0x51d820. Replaces *s with a heap copy of src (or NULL).
RT3_NOINLINE void __fastcall String_Assign(char** s, const char* src) {
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

// 0x51d870. Starts the game's millisecond clock (once).
void StartClock() {
    if (g_clockStart == 0) g_clockStart = Ticks();
}
RT3_REPLACE(0x51d870, StartClock);

// 0x51d890. Milliseconds since StartClock.
RT3_NOINLINE int32_t ClockMs() { return (int32_t)(Ticks() - g_clockStart); }
RT3_REPLACE(0x51d890, ClockMs);

// 0x51d8a0. strncpy that always terminates (dst needs n+1 bytes).
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n) {
    strncpy(dst, src, n);
    dst[n] = '\0';
}
RT3_REPLACE(0x51d8a0, StrCopyN);

// 0x51d8c0. Appends src while the total stays within n chars (dst needs n+1).
RT3_NOINLINE void __fastcall StrCatN(char* dst, const char* src, int32_t n) {
    int32_t len = (int32_t)strlen(dst);
    int32_t room = n - len;
    if (room > 0) {
        strncpy(dst + len, src, room);
        dst[len + room] = '\0';
    }
}
RT3_REPLACE(0x51d8c0, StrCatN);

// 0x51d900. strstr, case-insensitive unless caseSensitive. NULL if either is empty.
RT3_NOINLINE const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive) {
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

// 0x51db40
RT3_NOINLINE double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2) {
    double dx = (double)x2 - x1, dy = (double)y2 - y1, dz = (double)z2 - z1;
    return sqrt((dz * dz + dy * dy) + dx * dx); // x87 order of the original
}
RT3_REPLACE(0x51db40, Distance3D);

// 0x51db80
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2) {
    double dx = (double)x2 - x1, dy = (double)y2 - y1;
    return sqrt(dx * dx + dy * dy);
}
RT3_REPLACE(0x51db80, Distance2D);

// 0x51dbb0
RT3_NOINLINE double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2) {
    double dx = (double)x2 - x1, dy = (double)y2 - y1;
    return dx * dx + dy * dy;
}
RT3_REPLACE(0x51dbb0, DistanceSq2D);

// 0x51dbe0
int32_t __fastcall DistanceSqInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    return (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1);
}
RT3_REPLACE(0x51dbe0, DistanceSqInt);

// 0x51dc00. Integer distance; small offsets come from a precomputed
// 1048x1048 sqrt table (0x6d9098, u16), larger ones are computed and truncated.
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    int32_t dx = x1 > x2 ? x1 - x2 : x2 - x1;
    int32_t dy = y1 > y2 ? y1 - y2 : y2 - y1;
    if (dx < 0x418 && dy < 0x418)
        return (int32_t)(((uint32_t)dy & 0xffff0000u) | ((const uint16_t*)0x6d9098)[dx * 0x418 + dy]);
    return (int32_t)sqrt((double)(dy * dy + dx * dx));
}
RT3_REPLACE(0x51dc00, DistanceInt);

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
RT3_NOINLINE void __fastcall TrimSpaces(char* s, int32_t alsoNewlines) {
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
RT3_NOINLINE char* __fastcall CopyBaseName(const char* src, char* dst) {
    for (int32_t i = 0; src[i] != '\0'; ++i) {
        if (src[i] == '.') { dst[i] = '\0'; break; }
        dst[i] = src[i];
    }
    return dst;
}
RT3_REPLACE(0x51dde0, CopyBaseName);

// 0x51de10. Per-channel blend of two 0xAARRGGBB colours: a*t + b*(1-t), truncated.
uint32_t __fastcall BlendColor(uint32_t a, uint32_t b, float t) {
    double u = 1.0 - t;
    uint32_t out;
    static const int kShift[4] = { 16, 24, 8, 0 }; // order of the original
    uint32_t ch[4];
    for (int i = 0; i < 4; ++i) {
        double x = (double)(int32_t)((a >> kShift[i]) & 0xff) * t + (double)(int32_t)((b >> kShift[i]) & 0xff) * u;
        ch[i] = (uint32_t)(int32_t)x;
    }
    out = ((ch[0] | ch[1] << 8) << 8 | ch[2]) << 8 | ch[3]; // = A<<24 | R<<16 | G<<8 | B
    return out;
}
RT3_REPLACE(0x51de10, BlendColor);

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
RT3_NOINLINE void __fastcall StripExtension(char* s) {
    char* p = s + strlen(s) - 1;
    if (p == s) return;
    while (*p != '.') {
        if (--p == s) return;
    }
    *p = '\0';
}
RT3_REPLACE(0x51df90, StripExtension);

// 0x51dfc0. Colour for t in 0..1 (cold to hot) in the renderer's pixel format
// (+0x11474e: 0 = RGB, 4, 3, 2 variants, else grey), alpha 0xcf.
RT3_NOINLINE uint32_t __stdcall HeatColor(float t) {
    float v = t;
    if (v < Fl(0x5c8598)) v = Fl(0x5c8598);
    else if (v > Fl(0x5c85b4)) v = Fl(0x5c85b4);
    float a = (float)Ramp(v);
    float u = (float)((double)Fl(0x5c85b4) - v);
    double bv;
    if (u < D(0x5dce78)) bv = (double)u * D(0x5dce70) * D(0x5c88e8) + D(0x5c8880);
    else if (u < D(0x5c8628)) bv = Fl(0x5c85b4);
    else if (u < D(0x5dce68)) bv = D(0x5c8770) - ((double)u - D(0x5c8628)) * D(0x5dce60);
    else bv = Fl(0x5c8598);
    float c = (float)((double)Fl(0x5c85b4) - u);
    double k = Fl(0x5ca0ec);
    int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11474e);
    uint32_t px;
    if (mode == 0) {
        uint32_t ra = Ftol((double)a * k);
        px = ((ra << 8) | (uint32_t)Ftol(bv * k)) << 8;
    } else if (mode == 4) {
        uint32_t ra = Ftol((double)a * k);
        px = (ra << 16) | (uint32_t)Ftol(bv * k);
    } else if (mode == 3) {
        uint32_t g = Ftol(bv * k);
        px = (((g << 8) | g) << 8) | (uint32_t)Ftol((double)a * k);
    } else if (mode == 2) {
        uint32_t g = Ftol(bv * k);
        px = (g << 8) | (uint32_t)Ftol((double)a * k);
    } else {
        uint32_t g = Ftol((double)c * k);
        px = (((g << 8) | g) << 8) | g;
    }
    return px | 0xcf000000;
}
RT3_REPLACE(0x51dfc0, HeatColor);

}  // namespace rt3
