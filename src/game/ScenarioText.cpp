// ScenarioText.cpp - RT3.exe 0x488000-0x48a000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Effect_Release(uint8_t* e);
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Light_SetMode(uint8_t* o, int, uint8_t mode);
uint8_t* __fastcall Light_Construct(uint8_t* o);
void __fastcall Obj_SetName46b(uint8_t* o, int, const char* s);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
void __fastcall Territories_BuildBorders(Collection* c);
int32_t __fastcall Region_StationsLinked(uint32_t* o, int, int32_t company);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
int32_t __fastcall CountWords(const char* s);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
const char* __fastcall GetString(int32_t id);
void __fastcall Territories_Recount(Collection* c);
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
int32_t __fastcall Table_CountField3f(Collection* t);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
int32_t Global6d07b4Set2();
int32_t __fastcall Text_NextNumberedString(char** cursor, int32_t* num, char** out);
void __fastcall Obj_Rename(int32_t kind, uint32_t id, int32_t field, const char* text);
void __fastcall FreeAndNull(void** p);
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamSize(int32_t s);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
void __fastcall StreamWriteText(int32_t s, const char* str);
void __fastcall Text_ExportLine(int32_t st, int32_t section, int32_t item, int32_t sub, const char* text);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
int32_t __fastcall Obj_AnyKind4to7(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
uint8_t* __fastcall Station_Construct(uint8_t* s);
void __fastcall Nop487630(void*);
void __fastcall Station_InitWithId(uint8_t* s, int, int32_t id);
void __fastcall Territories_AddTestSet(Collection* t);
void __fastcall StreamClose(Stream s);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t);
RT3_NOINLINE void __fastcall Territories_Recount(Collection* c);
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
uint32_t __fastcall ScenarioText_Import(const char* name, int32_t report);
int32_t __fastcall ScenarioText_Export(const char* name, int32_t report);
int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall Maps_BatchScenarioText(int32_t import);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE int32_t __fastcall Table_CountPositive3d(Collection* t);
RT3_NOINLINE int32_t __fastcall Territories_CountLinked(Collection* c, int, int32_t company);
RT3_NOINLINE void __fastcall Territories_Recount(Collection* c);
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t);
int32_t __fastcall CountWords(const char* s);
void __fastcall Text_ExportLine(int32_t st, int32_t section, int32_t item, int32_t sub, const char* text);
int32_t __fastcall Text_NextNumberedString(char** cursor, int32_t* num, char** out);
void __fastcall Obj_SetName46b(uint8_t* o, int, const char* s);
RT3_NOINLINE int32_t __fastcall ScenarioText_Export(const char* name, int32_t report);
void __fastcall Obj_Rename(int32_t kind, uint32_t id, int32_t field, const char* text);
RT3_NOINLINE uint32_t __fastcall ScenarioText_Import(const char* name, int32_t report);
RT3_NOINLINE void __fastcall Maps_BatchScenarioText(int32_t import);
uint8_t* __fastcall TrackLight_Construct(uint8_t* o);
void __fastcall Effect489e80_Release(uint8_t* e);
void __fastcall Vehicle_PushState(uint8_t* o, int, uint32_t state, uint8_t mode);
RT3_NOINLINE void __fastcall Obj_FreeList244(uint8_t* o);
RT3_NOINLINE void __fastcall Node_Set212(void* n, int, int32_t v);
void __fastcall Obj_UpdateHeightLight(uint8_t* o);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline uint8_t* Map() { return g_map; }

static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

RT3_NOINLINE int32_t __fastcall Table_CountPositive3d(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3d) > 0) ++n;
    return n;
}
RT3_REPLACE(0x4880d0, Table_CountPositive3d);

// 0x488110. Number of territory groups (leader +0 / group +0x35) with a station of
// the company linked to another area.
RT3_NOINLINE int32_t __fastcall Territories_CountLinked(Collection* c, int, int32_t company) {
    uint8_t seen[0x19];
    memset(seen, 0, sizeof seen);
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint32_t* e = (uint32_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!Region_StationsLinked(e, 0, company)) continue;
        uint32_t g = F<int32_t>(e, 0x31) ? e[0] : F<uint32_t>(e, 0x35);
        if (seen[g]) continue;
        seen[g] = 1;
        ++n;
    }
    return n;
}
RT3_REPLACE(0x488110, Territories_CountLinked);

// 0x4881b0. Counts the land tiles of each territory (+0x3d, tile flags >> 3), sums
// them into each group leader (+0x41) and rebuilds the borders.
RT3_NOINLINE void __fastcall Territories_Recount(Collection* c) {
    int32_t counts[1000];
    memset(counts, 0, sizeof counts);
    const uint8_t* t = F<uint8_t*>(Map(), 0x2131);
    for (int32_t y = 0; y < F<int32_t>(Map(), 0x2159); ++y)
        for (int32_t x = 0; x < F<int32_t>(Map(), 0x2155); ++x, ++t)
            if (!Map_CellLow3(Map(), 0, x, y)) ++counts[*t >> 3];
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        F<int32_t>(e, 0x3d) = counts[F<int32_t>(e, 0)];
        F<int32_t>(e, 0x41) = 0;
    }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x31)) continue;
        F<int32_t>(Collection_Get(c, 0, F<uint32_t>(e, 0x35)), 0x41) += F<int32_t>(e, 0x3d);
    }
    Territories_BuildBorders(g_territories);
}
RT3_REPLACE(0x4881b0, Territories_Recount);

// 0x4882e0. Territory table t: adds 24 test territories (0x11e with "01"..
// "24" (a constructed 0xdd1-byte record each, set up with
// its id), then recounts them.
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t) {
    for (int32_t i = 1; i <= 0x18; ++i) {
        Loading_Pump(g_app, 0, 0, 0);
        char num[0x20], name[0x120];
        crt::sprintf(num, (const char*)0x5c9aec, i);
        FormatText(name, GetString(0x11e), num, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        alignas(8) uint8_t tmp[0xde0];
        Station_Construct(tmp);
        uint32_t id = Collection_Allocate(t, 0, tmp, 0xdd1, 0, 0);
        uint8_t* s = (uint8_t*)Collection_Get(t, 0, id);
        Station_InitWithId(s, 0, (int32_t)id);
        StrCopyN((char*)s + 4, name, 0x28);
        Nop487630(tmp);
    }
    Territories_Recount(t);
}
RT3_REPLACE(0x4882e0, Territories_AddTestSet);

// 0x4883b0. Number of runs of characters >= 'A' (signed) in s.
RT3_NOINLINE int32_t __fastcall CountWords(const char* s) {
    int32_t len = 0;
    while (s[len]) ++len;
    int32_t n = 0, inWord = 0;
    for (int32_t i = 0; i < len; ++i) {
        if ((signed char)s[i] < 0x41) {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            ++n;
        }
    }
    return n;
}
RT3_REPLACE(0x4883b0, CountWords);

// 0x4883f0. Writes one text for translation: `<id>   "<text>"` with id =
// (section * 100000 + item) * 10 + sub and line breaks escaped as \n (CR dropped);
// its words are added to the running count 0x6cfca0.
RT3_NOINLINE void __fastcall Text_ExportLine(int32_t st, int32_t section, int32_t item, int32_t sub, const char* text) {
    int32_t len = (int32_t)strlen(text);
    char* buf = (char*)crt::malloc(len * 2 + 0x32);
    strcpy(buf, text);
    Str_Replace(buf, len * 2, (const char*)0x5cf130, (const char*)0x5cf134, 1);
    Str_Replace(buf, len * 2, (const char*)0x5c9ce8, (const char*)0x5cf134, 1);
    Str_Replace(buf, len * 2, (const char*)0x5cf12c, (const char*)0x5c87a8, 1);
    g_6cfca0 += CountWords(buf);
    int32_t id = sub + (section * 100000 + item) * 10;
    char num[0x40], line[0x1760];
    Num_Format(num, 8, (double)id, 10, 1, 0, 0);
    FormatText(line, (const char*)0x5cf120, num, buf, 0, 0, 0, 0, 0, 0);
    StreamWrite(st, line, (uint32_t)strlen(line), 0);
    crt::free(buf);
}
RT3_REPLACE(0x4883f0, Text_ExportLine);

// 0x488540. Reads the next line of the form  <number> ... "<text>"  from *cursor:
// *num = the number, *out = a new copy of the text (\n escapes expanded); lines
// without both are skipped. Returns 0 at the end of the text.
RT3_NOINLINE int32_t __fastcall Text_NextNumberedString(char** cursor, int32_t* num, char** out) {
    char* p = *cursor;
    do {
        int32_t n = -1;
        char* start = nullptr;
        char* end = nullptr;
        for (int8_t c = *p; c && c != '\n' && c != '\r'; c = *p) {
            if (c >= '0' && c <= '9' && n == -1 && !start) {
                do {
                    n = n == -1 ? c - '0' : n * 10 + c - '0';
                    c = *++p;
                } while (c >= '0' && c <= '9');
            } else if (c == '"' && !start) {
                c = *++p;
                start = p;
                char* quote = nullptr;
                for (; c && c != '\n'; c = *++p) {
                    if (c == '"') quote = p;
                    else if (c > ' ') quote = nullptr;
                }
                end = quote ? quote - 1 : p - 1;
            } else {
                ++p;
            }
        }
        if (n != -1 && start && end && (uint32_t)(end - 1) >= (uint32_t)start) {
            uint32_t len = (uint32_t)(end - start) + 1;
            char* s = (char*)crt::malloc(len + 1);
            *out = s;
            if (len) memcpy(s, start, len);
            (*out)[len] = 0;
            Str_Replace(*out, (int32_t)len, (const char*)0x5cf134, (const char*)0x5c9ce8, 1);
            *cursor = p;
            *num = n;
            return 1;
        }
        while (*p == '\n' || *p == '\r') ++p;
    } while (*p);
    *cursor = p;
    return 0;
}
RT3_REPLACE(0x488540, Text_NextNumberedString);

RT3_NOINLINE void __fastcall Obj_SetName46b(uint8_t* o, int, const char* s) { StrCopyN((char*)o + 0x46b, s, 0x3c); }
RT3_REPLACE(0x4886c0, Obj_SetName46b);

// 0x4886e0 (name in ECX, `report` in EDX). Writes the scenario text file
// `name` (path format 0x5cfcb8) in the editor (else 0xe6d): the translation
// notes header, then the map description / briefings (sections 2..4), the
// used territories (5), cities (6), geographic labels (10), stations (9),
// companies (8) and every event's texts (7; with a comment line naming the
// kind, and a debug report when a newspaper headline needs more than six
// lines). Text_ExportLine counts the words into 0x6cfca0, which is written
// as a trailer and returned; 0xe6e reports the file when `report`.
RT3_NOINLINE int32_t __fastcall ScenarioText_Export(const char* name, int32_t report) {
    if (!g_world || !Global6d07b4Set2()) {
        Message_Show(GetString(0xe6d), 1, 0);
        return 0;
    }
    char path[0x12c];
    g_6cfca0 = 0;
    FormatText(path, (const char*)0x5cfcb8, name, 0, 0, 0, 0, 0, 0, 0);
    int32_t s = StreamOpen(path, 4, 5000, 1);
    static const uint32_t header[] = { 0x5cfcb0, 0x5cfcb0, 0x5cfc8c, 0x5cfcb0, 0x5cfc40, 0x5cfc0c, 0x5cfcb0, 0x5cfbec, 0x5cfcb0, 0x5cfba0, 0x5cfb50,
                                       0x5cfb00, 0x5cfac0, 0x5cfcb0, 0x5cfcb0, 0x5cfa58, 0x5cf9f8, 0x5cf998, 0x5cf940, 0x5cfcb0, 0x5cf8d8, 0x5cf878,
                                       0x5cfcb0, 0x5cfcb0, 0x5cf868, 0x5cfcb0, 0x5cf808, 0x5cf7dc, 0x5cfcb0, 0x5cf788, 0x5cf728, 0x5cf6c0, 0x5cf668,
                                       0x5cf5f0, 0x5cf580, 0x5cf508, 0x5cf4b0, 0x5cf448, 0x5cf3e0, 0x5cf390, 0x5cf348, 0x5cfcb0, 0x5cf130, 0x5cf130,
                                       0x5cf334 };
    char text[0x2710];
    text[0] = 0;
    for (uint32_t a : header) strcat(text, (const char*)a);
    strcat(text, name);
    strcat(text, (const char*)0x5cf32c);
    strcat(text, (const char*)0x5cf130);
    auto section = [&](uint32_t title) {
        strcpy(text, (const char*)title);
        StreamWrite(s, text, (uint32_t)strlen(text), 0);
    };
    StreamWrite(s, text, (uint32_t)strlen(text), 0);
    section(0x5cf314);
    Text_ExportLine(s, 2, 0, 0, (const char*)g_world + 0x672e);
    section(0x5cf2ec);
    Text_ExportLine(s, 3, 0, 0, (const char*)g_world + 0x4f30);
    section(0x5cf2b8);
    Text_ExportLine(s, 4, 0, 0, (const char*)g_world + 0x5ae9);
    section(0x5cf298);
    for (int32_t i = 0; i < Collection_Count(g_territories); ++i) {
        Collection* c = g_territories;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (F<int32_t>(t, 0x3d) > 0 && F<int32_t>(t, 0x31)) Text_ExportLine(s, 5, F<int32_t>(t, 0), 0, (const char*)t + 4);
    }
    section(0x5cf288);
    for (int32_t i = 0; i < Collection_Count(g_cities); ++i) {
        Collection* c = g_cities;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!F<int32_t>(t, 0x23e)) Text_ExportLine(s, 6, F<int32_t>(t, 0x23a), 0, (const char*)t + 0x356);
    }
    section(0x5cf26c);
    for (int32_t i = 0; i < Collection_Count(g_6ada80); ++i) {
        Collection* c = g_6ada80;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        Text_ExportLine(s, 0xa, (int32_t)Collection_IdAt(g_6ada80, 0, (uint32_t)i, 0), 0, (const char*)t + 0x2a2);
    }
    section(0x5cf250);
    for (int32_t i = 0; i < Collection_Count(g_stations); ++i) {
        Collection* c = g_stations;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        Text_ExportLine(s, 9, F<int32_t>(t, 0), 0, (const char*)t + 0x46b);
    }
    section(0x5cf234);
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i) {
        uint8_t* t = (uint8_t*)Table_NthField3f(g_companies, 0, i);
        Text_ExportLine(s, 8, F<int32_t>(t, 0), 0, (const char*)t + 4);
    }
    section(0x5cf220);
    const char* kinds[6] = { (const char*)0x5cf20c, (const char*)0x5cf1f8, (const char*)0x5cf1d4,
                             (const char*)0x5cf1b8, (const char*)0x5cf1a4, (const char*)0x5cf190 };
    for (int32_t i = 0; i < Collection_Count(g_events); ++i) {
        Collection* c = g_events;
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        int32_t id = (int32_t)Collection_IdAt(g_events, 0, (uint32_t)i, 0);
        char* body = (char*)e + 0xe0;
        TrimSpaces(body, 1);
        TrimSpaces((char*)e + 0x401, 1);
        char* choices[4] = { (char*)e + 0x4ca, (char*)e + 0x593, (char*)e + 0x65c, (char*)e + 0x725 };
        for (char* p : choices) TrimSpaces(p, 1);
        bool news = false;
        for (int32_t k = 0; k < 4; ++k)
            for (uint8_t* n = F<uint8_t*>(e, 4 + k * 4); n; n = F<uint8_t*>(n, 0x24)) {
                int32_t type = F<int32_t>(n, 0);
                if (type == 4 || type == 5 || type == 6 || type == 7) news = true;
            }
        if (news) {
            int32_t lines = Font_CountLines(F<uint8_t*>(g_world, 0x46a9c), 0, body, Prod_Bits(0x3e9eb852), nullptr);
            if (lines > 6) {
                char msg[0x4ac];
                crt::sprintf(msg, (const char*)0x5cf160, name, (const char*)e + 0x18, lines, body);
                DebugLog(msg, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2);
            }
        }
        if (!strlen(body)) continue;
        StreamWriteText(s, Obj_AnyKind4to7(e) ? (const char*)0x5cf190 : kinds[e[0x7ee]]);
        Text_ExportLine(s, 7, id, 1, body);
        if (e[0x7ee] == 2 && strlen((const char*)e + 0x401)) Text_ExportLine(s, 7, id, 2, (const char*)e + 0x401);
        if (e[0x7ee] == 1)
            for (int32_t k = 0; k < 4; ++k)
                if (strlen(choices[k])) Text_ExportLine(s, 7, id, 3 + k, choices[k]);
        StreamWrite(s, (const void*)0x5cf130, 2, 0);
    }
    char trailer[0x1f4];
    crt::sprintf(trailer, (const char*)0x5cf138, g_6cfca0);
    StreamWriteText(s, trailer);
    StreamClose(s);
    FormatText(text, GetString(0xe6e), path, 0, 0, 0, 0, 0, 0, 0);
    if (report) Message_Show(text, 1, 0);
    return g_6cfca0;
}
RT3_REPLACE(0x4886e0, ScenarioText_Export);

// 0x4895c0. Renames object `id` of the given kind: 2..4 scenario texts in the world,
// 5 ..., 6 cities, 7 event texts (field 1..6), 8 players, 9 stations, 10 ...
RT3_NOINLINE void __fastcall Obj_Rename(int32_t kind, uint32_t id, int32_t field, const char* text) {
    uint8_t* w = g_world;
    auto in = [&](uint32_t g) -> uint8_t* {
        Collection* c = hook::Global<Collection*>(g);
        return Collection_IsValid(c, 0, id) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(g), 0, id) : nullptr;
    };
    uint8_t* o;
    switch (kind) {
    case 2: StrCopyN((char*)w + 0x672e, text, 0x258); break;
    case 3: StrCopyN((char*)w + 0x4f30, text, 0xbb8); break;
    case 4: StrCopyN((char*)w + 0x5ae9, text, 0xbb8); break;
    case 5: if ((o = in(0x6cfc9c)) != nullptr) StrCopyN((char*)o + 4, text, 0x28); break;
    case 6: if ((o = in(0x62bae0)) != nullptr) StrCopyN((char*)o + 0x356, text, 0x28); break;
    case 7:
        if ((o = in(0x62be18)) == nullptr) break;
        switch (field) {
        case 1: StrCopyN((char*)o + 0xe0, text, 0x320); break;
        case 2: StrCopyN((char*)o + 0x401, text, 0xc8); break;
        case 3: StrCopyN((char*)o + 0x4ca, text, 0xc8); break;
        case 4: StrCopyN((char*)o + 0x593, text, 0xc8); break;
        case 5: StrCopyN((char*)o + 0x65c, text, 0xc8); break;
        case 6: StrCopyN((char*)o + 0x725, text, 0xc8); break;
        }
        break;
    case 8: if ((o = in(0x62be10)) != nullptr) StrCopyN((char*)o + 4, text, 0x32); break;
    case 9: if ((o = in(0x6cec20)) != nullptr) Obj_SetName46b(o, 0, text); break;
    case 10: if ((o = in(0x6ada80)) != nullptr) StrCopyN((char*)o + 0x2a2, text, 0x28); break;
    }
}
RT3_REPLACE(0x4895c0, Obj_Rename);

// 0x489830 (name in ECX, `report` in EDX). Applies the scenario text file
// `name` (path format 0x5cfcb8) in the editor: each line "<number> ...
// "<text>"" renames object (number / 1000000, number % 1000000 / 10, field
// number % 10). Messages 0xe6d (not editing), 0xac (missing file) and 0xe6f
// (done) when reporting. Returns 1 when the file was read. The file buffer
// is not freed (as in the original).
RT3_NOINLINE uint32_t __fastcall ScenarioText_Import(const char* name, int32_t report) {
    if (!g_world || !Global6d07b4Set2()) {
        if (report) Message_Show(GetString(0xe6d), 1, 0);
        return 0;
    }
    char path[0x130];
    FormatText(path, (const char*)0x5cfcb8, name, 0, 0, 0, 0, 0, 0, 0);
    int32_t s = StreamOpen(path, 5, 200000, 0);
    if (s == -1) {
        char msg[0x200];
        FormatText(msg, GetString(0xac), name, 0, 0, 0, 0, 0, 0, 0);
        if (report) Message_Show(msg, 1, 0);
        return 0;
    }
    int32_t size = StreamSize(s);
    char* buf = (char*)crt::malloc(size + 1);
    StreamRead(s, buf, size, 0, 1);
    buf[size] = 0;
    StreamClose(s);
    char* cursor = buf;
    int32_t num;
    char* text = nullptr;
    while (Text_NextNumberedString(&cursor, &num, &text)) {
        Obj_Rename(num / 1000000, (uint32_t)(num % 1000000 / 10), num % 10, text);
        FreeAndNull((void**)&text);
    }
    if (report) Message_Show(GetString(0xe6f), 1, 0);
    return 1;
}
RT3_REPLACE(0x489830, ScenarioText_Import);

// 0x489a20. Developer tool (menu): every map maps\*.gmp is loaded in turn
// (progress "i/n  text name" in the status line, lobby mode 3, the app's
// +0x6c set meanwhile) and `import` decides what happens to its scenario
// texts (the map's name without its last four letters): 0 exports them
// (0x4886e0, counting what it wrote), else imports them (0x489830) and
// saves the map again, collecting the names that failed (one per line).
// A message box sums it up (0xe73 imported / 0xe75 exported, 0xe74 plus the
// failures). Without a world, or when 0x4338c0 refuses, it only says so
// (0xe70). 0x6cfca4 is set while it runs.
// QUIRK: FindClose also gets the invalid handle when there is no map.
// QUIRK: only 100 names fit; more maps overrun the name table.
RT3_NOINLINE void __fastcall Maps_BatchScenarioText(int32_t import) {
    if (!g_world || !Global6d07b4Set2()) {
        Dialog_RunMessage(GetString(0xe70), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        return;
    }
    auto findFirst = ((void*(__stdcall*)(const char*, void*))imp::FindFirstFileA);
    auto findNext = imp::FindNextFileA;
    auto findClose = imp::FindClose;
    char names[100][200], msg[5000], failed[3000], pattern[504], status[400], stem[304], num1[0x30], num2[0x30];
    char fd[0x140];
    g_6cfca4 = 1;
    int32_t count = 0, exported = 0, imported = 0;
    failed[0] = 0;
    FormatText(pattern, (const char*)0x5cfcd4, 0, 0, 0, 0, 0, 0, 0, 0);
    void* h = findFirst(pattern, fd);
    if (h != (void*)-1) {
        do strcpy(names[count++], fd + 0x2c);
        while (findNext(h, fd));
    }
    findClose(h);
    for (int32_t i = 0; i < count; ++i) {
        const char* name = names[i];
        F<int32_t>(g_newGame, 1) = 3;
        crt::sprintf(status, (const char*)0x5cfcc4, i, count, GetString(0xe71), name);
        Status_SetMessage(g_windowManager, 0, status);
        F<int32_t>(g_app, 0x6c) = 1;
        World_Load(g_world, 0, name, 0, nullptr);
        stem[0] = 0;
        strcat(stem, name);
        const size_t len = strlen(stem);
        if (len > 4) stem[len - 4] = 0;
        if (!import) {
            exported += ScenarioText_Export(stem, 0);
        } else if (ScenarioText_Import(stem, 0)) {
            ++imported;
            crt::sprintf(status, (const char*)0x5cfcc4, i, count, GetString(0xe72), name);
            Status_SetMessage(g_windowManager, 0, status);
            World_Save(g_world, 0, name, 0, nullptr);
        } else {
            strcat(failed, name);
            memcpy(failed + strlen(failed), (const void*)0x5c9ce8, 2);
        }
        F<int32_t>(g_app, 0x6c) = 0;
    }
    if (import) {
        const char* total = Num_Format(num1, 1, (double)count, 10, 1, 0, 0);
        const char* ok = Num_Format(num2, 1, (double)imported, 10, 1, 0, 0);
        FormatText(msg, GetString(0xe73), ok, total, 0, 0, 0, 0, 0, 0);
        if (imported != count) strcat(msg, GetString(0xe74)), strcat(msg, failed);
    } else {
        const char* n = Num_Format(num1, 1, (double)exported, 10, 1, 0, 0);
        const char* total = Num_Format(num2, 1, (double)count, 10, 1, 0, 0);
        FormatText(msg, GetString(0xe75), total, n, 0, 0, 0, 0, 0, 0);
    }
    Dialog_RunMessage(msg, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    g_6cfca4 = 0;
}
RT3_REPLACE(0x489a20, Maps_BatchScenarioText);

// 0x489e60. Constructs a track light (vtable 0x5cfce0, flag +0x201 bit 0 clear).
uint8_t* __fastcall TrackLight_Construct(uint8_t* o) {
    Light_Construct(o);
    o[0x201] &= 0xfe;
    F<uint32_t>(o, 0) = 0x5cfce0;
    return o;
}
RT3_REPLACE(0x489e60, TrackLight_Construct);

void __fastcall Effect489e80_Release(uint8_t* e) {
    F<uint32_t>(e, 0) = 0x5cfce0;
    Effect_Release(e);
}
RT3_REPLACE(0x489e80, Effect489e80_Release);

// 0x489e90. Pushes an undo record (0x42 bytes: state, flags and the corners) onto
// the chain +0x244 and sets the new state (+0x240, +0x218).
void __fastcall Vehicle_PushState(uint8_t* o, int, uint32_t state, uint8_t mode) {
    uint8_t* p = (uint8_t*)crt::malloc(0x42);
    F<uint32_t>(p, 0) = F<uint32_t>(o, 0x240);
    p[4] = o[0x218];
    p[5] = o[0x216];
    p[6] = o[0x44] != 0;
    p[7] = F<int32_t>(o, 0x248) != 0;
    F<uint32_t>(p, 0xe) = F<uint32_t>(o, 0x244);
    F<uint32_t>(p, 0xa) = F<uint32_t>(o, 0x230);
    p[8] = o[0x217];
    p[9] = (o[0x24c] >> 4) & 1;
    Effect_GetVectors(o, 0, (uint32_t*)(p + 0x12), (uint32_t*)(p + 0x1e), (uint32_t*)(p + 0x2a), (uint32_t*)(p + 0x36));
    F<uint8_t*>(o, 0x244) = p;
    F<uint32_t>(o, 0x240) = state;
    o[0x218] = mode;
}
RT3_REPLACE(0x489e90, Vehicle_PushState);

// 0x489f40. Frees the singly linked list +0x244 (next at +0xe).
RT3_NOINLINE void __fastcall Obj_FreeList244(uint8_t* o) {
    while (uint8_t* n = F<uint8_t*>(o, 0x244)) {
        F<uint8_t*>(o, 0x244) = F<uint8_t*>(n, 0xe);
        crt::free(n);
    }
}
RT3_REPLACE(0x489f40, Obj_FreeList244);

RT3_NOINLINE void __fastcall Node_Set212(void* n, int, int32_t v) { F<int32_t>(n, 0x212) = v; }
RT3_REPLACE(0x489f80, Node_Set212);

// 0x489f90. Sets the piece's centre (+0x1ed/+0x1f1 in cells) and its light mode: 1
// (raised) when the mean of its corner heights is over 10 above the ground, else 0.
void __fastcall Obj_UpdateHeightLight(uint8_t* o) {
    float ground = (float)Terrain_HeightAt(F<uint8_t*>(o, 4), 0, F<float>(o, 0xc), F<float>(o, 0x10));
    float s = (float)((double)F<float>(o, 0x3c) + F<float>(o, 0x30));
    float avg = (float)(((double)s + F<float>(o, 0x24) + F<float>(o, 0x18)) / D(0x5c87b8));
    F<float>(o, 0x1ed) = (float)((double)s / Fl(0x5c878c));
    F<float>(o, 0x1f1) = (float)(((double)F<float>(o, 0x24) + F<float>(o, 0x18)) / Fl(0x5c878c));
    Light_SetMode(o, 0, (double)avg - ground > D(0x5c85f0) ? 1 : 0);
}
RT3_REPLACE(0x489f90, Obj_UpdateHeightLight);

}  // namespace rt3
