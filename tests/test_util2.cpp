// Difftests: config tokenizer, string helpers, name localizer, colour blend.
#include "difftest.h"
#include "hook/hook.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <initializer_list>

namespace rt3 {
int32_t __fastcall HasString(int32_t id);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall TokenizeLine(char* base, int32_t* pos, int32_t* count, char** tokens);
void __fastcall SortRecordsByName(uint8_t* records, int32_t count, uint32_t size, int32_t keyOffset);
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
char* __fastcall CopyBaseName(const char* src, char* dst);
int32_t __fastcall PowerOfTwo(int32_t v, int32_t roundUp);
void __fastcall StripExtension(char* s);
uint32_t __fastcall BlendColor(uint32_t a, uint32_t b, float t);
void BuildNameIndex();
uint32_t IsEnglish();
}
using namespace rt3;

DIFFTEST(util2_Tokenize) {
    auto orig = (int32_t(__fastcall*)(char*, int32_t*, int32_t*, char**))0x51d9d0;
    static const char* inputs[] = {
        "a b c\nd e\n", "  key = value ; comment\nnext\n", "abc;comment\nx y\n", "a,b,,c\r\nz",
        "\n\n\nq", "", ";only comment\nafter\n", "tab\tsep\tline\n", "last line no newline", "x ;\n",
        "1, 2, 3, 4, 5, 6, 7, 8, 9, 10\n", "  \n", "a;b;c\n", ",\n",
    };
    for (const char* in : inputs) {
        char a[256] = {}, b[256] = {};
        strcpy(a, in);
        strcpy(b, in);
        int32_t pa = 0, pb = 0;
        for (int line = 0; line < 8; ++line) {
            char* ta[64] = {};
            char* tb[64] = {};
            int32_t ca = -1, cb = -1;
            int32_t ra = orig(a, &pa, &ca, ta);
            int32_t rb = TokenizeLine(b, &pb, &cb, tb);
            EXPECT_EQ(ra, rb, "ret '%s' line %d", in, line);
            EXPECT_EQ(pa, pb, "pos '%s' line %d", in, line);
            EXPECT_EQ(ca, cb, "count '%s' line %d", in, line);
            for (int k = 0; k < ca && k < 64; ++k) EXPECT_EQ(ta[k] - a, tb[k] - b, "token %d of '%s'", k, in);
            EXPECT_EQ(memcmp(a, b, sizeof a), 0, "buffer '%s'", in);
            if (!ra) break;
        }
    }
}

DIFFTEST(util2_Strings) {
    auto oTrim = (void(__fastcall*)(char*, int32_t))0x51dd60;
    auto oBase = (void(__fastcall*)(const char*, char*))0x51dde0;
    auto oStrip = (void(__fastcall*)(char*))0x51df90;
    auto oPow = (int32_t(__fastcall*)(int32_t, int32_t))0x51df50;
    static const char* strs[] = { "", " ", "  a  ", "file.txt", "a.b.c", ".hidden", "noext", "x\r\n", " \n x \r", "...", "a." };
    for (const char* s : strs) {
        for (int nl = 0; nl < 2; ++nl) {
            char a[64], b[64];
            memset(a, 0x41, 64); memset(b, 0x41, 64);
            strcpy(a, s); strcpy(b, s);
            oTrim(a, nl); TrimSpaces(b, nl);
            EXPECT_EQ(memcmp(a, b, 64), 0, "trim '%s' %d", s, nl);
        }
        char a[64], b[64];
        memset(a, 0x41, 64); memset(b, 0x41, 64);
        oBase(s, a); CopyBaseName(s, b);
        EXPECT_EQ(memcmp(a, b, 64), 0, "base '%s'", s);
        if (s[0]) {
            strcpy(a, s); strcpy(b, s);
            oStrip(a); StripExtension(b);
            EXPECT_EQ(strcmp(a, b), 0, "strip '%s'", s);
        }
    }
    for (int v = -5; v < 70000; v += (v < 600 ? 1 : 97))
        for (int up = 0; up < 2; ++up) EXPECT_EQ(oPow(v, up), PowerOfTwo(v, up), "pow2 %d %d", v, up);
    EXPECT_EQ(oPow(0x50000000, 1), PowerOfTwo(0x50000000, 1), "pow2 big");

    struct Rec { int32_t id; char name[20]; int32_t pad; };
    Rec ra[40], rb[40];
    static const char* names[] = { "Coal", "coal", "Iron", "apple", "Zinc", "", "Beer", "bEEr", "Mail" };
    srand(41);
    for (int i = 0; i < 40; ++i) {
        ra[i].id = i;
        strcpy(ra[i].name, names[rand() % 9]);
        ra[i].pad = rand();
    }
    memcpy(rb, ra, sizeof ra);
    ((void(__fastcall*)(uint8_t*, int32_t, uint32_t, int32_t))0x51dc60)((uint8_t*)ra, 40, sizeof(Rec), 4);
    SortRecordsByName((uint8_t*)rb, 40, sizeof(Rec), 4);
    EXPECT_EQ(memcmp(ra, rb, sizeof ra), 0, "sort");
}

DIFFTEST(util2_Localize) {
    // Index built by the original, then compared with ours.
    uint8_t saved[26 * 9];
    ((void(*)())0x51b660)();
    memcpy(saved, (void*)0x6d3b98, sizeof saved);
    memset((void*)0x6d3b98, 0x77, sizeof saved);
    BuildNameIndex();
    EXPECT_EQ(memcmp(saved, (void*)0x6d3b98, sizeof saved), 0, "name index");
    auto orig = (const char*(__fastcall*)(const char*))0x51c920;
    auto oHas = (int32_t(__fastcall*)(int32_t))0x51c900;
    const char* const* table = (const char* const*)0x6243c8;
    for (int i = 0; i < 0xeb; ++i) {
        const char* n = table[i * 2];
        EXPECT_EQ(strcmp(orig(n), LocalizeName(n)), 0, "localize %s", n);
        char low[64];
        strcpy(low, n);
        for (char* p = low; *p; ++p) *p = (char)(*p | 0x20);
        EXPECT_EQ(strcmp(orig(low), LocalizeName(low)), 0, "localize %s", low);
    }
    static const char* odd[] = { "", "x", "Nothing", "2", "zz" };
    for (const char* n : odd) EXPECT_EQ(strcmp(orig(n), LocalizeName(n)), 0, "localize odd '%s'", n);
    for (int id = 0; id < 0x1100; ++id) EXPECT_EQ(oHas(id), HasString(id), "has %d", id);
    int32_t& lang = hook::Global<int32_t>(0x6d3c84);
    int32_t keep = lang;
    for (int32_t v : { 0, 1, 6, 0x100, 0x1234500 }) {
        lang = v;
        EXPECT_EQ(((uint32_t(*)())0x51b6f0)(), IsEnglish(), "isEnglish %d", v);
    }
    lang = keep;
}

DIFFTEST(util2_BlendColor) {
    auto orig = (uint32_t(__fastcall*)(uint32_t, uint32_t, float))0x51de10;
    srand(42);
    for (int i = 0; i < 200000; ++i) {
        uint32_t a = (uint32_t)rand() << 17 ^ rand() << 3 ^ rand(), b = (uint32_t)rand() << 16 ^ rand();
        float t = (float)(rand() % 2001 - 500) / 1000.0f;
        EXPECT_EQ(orig(a, b, t), BlendColor(a, b, t), "blend %08x %08x %f", a, b, t);
    }
}
