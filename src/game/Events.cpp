// Events.cpp - RT3.exe 0x430000-0x432000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall List1a_Free(uint8_t** head);
void __fastcall Obj_FreeFourLists(void* o);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Obj_SetSlot35b(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Obj_SetSlot3b5(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Obj_SetSlot2ab(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Obj_SetSlot8a(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);

uint32_t __fastcall Table_AddNamedRecord(Collection* t, int, const char* name, uint32_t key);
void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint8_t __fastcall Obj_Flag3d4(void* o);
uint32_t __fastcall Obj_GridByte(void* o);
uint32_t __fastcall Obj_CityGridByte(void* o);
uint32_t __fastcall Train_GridByteAtCar45(uint8_t* t);
uint32_t __fastcall Obj_CenterGridByte(void* o);
void __fastcall Track_SetOwner(uint8_t* t, int, uint16_t owner);
void* __fastcall Company_Owner(void* c);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t __fastcall Collection_Count(Collection* c);
int __fastcall ToUpper(char c);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void* __fastcall Obj_Ref25(void* o);
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
int32_t IsMultiplayer();
int32_t __fastcall Obj_Get25(void* o);
void __fastcall RuleText_Expand(uint8_t* self, int, const char* src, char* dst, int32_t size, uint8_t* pl, uint8_t* co, uint8_t* st);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
int32_t CountHumanPlayers();
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
int32_t __fastcall Company_Get7680(void* c);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint32_t __fastcall Obj_FindRecord62b2fc(void* o);
void __stdcall Event_BuildIndustries(const uint8_t* rec);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
void __fastcall World_SetDifficulty(uint8_t* w, int, int32_t level);
void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
void __stdcall Options_SetWeatherOverride(int32_t on, int32_t level);
void __fastcall Camera_StartShake(uint8_t* c, int, float strength);
void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
void __stdcall Territory_GiveToOwner(int32_t* owner, int32_t* filter, int32_t kind);
void __fastcall Company_Dissolve(uint8_t* c);
void __fastcall Obj_SetPlayer(void* o, int, uint32_t player);
void __fastcall Effects_Apply(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company, uint8_t* player, int32_t slot);
uint8_t __fastcall Load_CargoFlags(void* o);
uint32_t __fastcall Event_ShowText(uint8_t* e, int, uint8_t* pl, uint8_t* co, uint8_t* st, char* out, int32_t status);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
uint8_t __fastcall Obj_ListFlags(void* o, int, int32_t i, int32_t classify);
int32_t __fastcall Scenario_CheckSetup(uint32_t* s, int, char* out);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall Company_BuildBuilding(uint8_t* co, int, uint32_t id, float x, float y, uint32_t angle, int32_t checkOnly, uint32_t* outId);
RT3_NOINLINE void __fastcall Company_Dissolve(uint8_t* c);
RT3_NOINLINE int32_t __stdcall RuleVar_Get(int32_t var, uint8_t* pl, uint8_t* co, uint8_t* st, int32_t p5, int32_t p6, int32_t p7, int32_t p8);
RT3_NOINLINE void __stdcall Editor_DeleteKind(int32_t* owner, uint32_t* region, int32_t kind);
RT3_NOINLINE void __stdcall Territory_GiveToOwner(int32_t* owner, int32_t* filter, int32_t kind);
RT3_NOINLINE void __stdcall Event_BuildIndustries(const uint8_t* rec);
void __fastcall RuleText_Expand(uint8_t* self, int, const char* src, char* dst, int32_t size, uint8_t* pl, uint8_t* co, uint8_t* st);
RT3_NOINLINE void __fastcall Events_AddTimed(uint8_t* self, int, uint8_t* cond, int32_t* a, int32_t* b, int32_t* c);
void __fastcall EventRules_Save(uint8_t* o, int, Stream st);
RT3_NOINLINE int32_t __fastcall Scenario_CheckSetup(uint32_t* s, int, char* out);
void __fastcall Obj_FreeAllLists2(void* o);
RT3_NOINLINE void __fastcall Effects_Apply(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company, uint8_t* player, int32_t slot);
RT3_NOINLINE uint32_t __fastcall Event_ShowText(uint8_t* e, int, uint8_t* pl, uint8_t* co, uint8_t* st, char* out, int32_t status);
RT3_NOINLINE void __fastcall Effects_Dispatch(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company, uint8_t* player, int32_t slot);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);

static inline uint8_t* World() { return g_world; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline char* Fmt(char* dst, const char* fmt, const char* a = 0, const char* b = 0) {
    return FormatText(dst, fmt, a, b, 0, 0, 0, 0, 0, 0);
}

// Writes the 100-byte name at `off` of record `idx` in table `table`.
static void WriteName(Stream st, uint32_t table, int32_t idx, uint32_t off) {
    char buf[0x65]; // StrCopyN also writes the terminator at [0x64]
    uint8_t* rec = (uint8_t*)Collection_Get(hook::Global<Collection*>(table), 0, idx);
    StrCopyN(buf, (const char*)rec + off, 0x64);
    StreamWrite(st, buf, 0x64, 0);
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

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

// 0x430040. Scenario effect: gives everything of `kind` in territory *filter (null:
// everywhere) to owner *owner. Kinds 11 / 12 (and 9 = all) industries owned by
// flag +0x3d4 == 1: kind 11 the ones whose vtable +0x70 says yes, 12 the others
// (sic: inverted), owner into +0x276; 23 (and 9) trains (0x6cfcbc, by either end's
// territory) into +0x51; 10 (and 9) track pieces of type 2 (0x6cfca8) get the owner
// and so do their stations (0x6cfcb4, +0x1e), then two UI refresh events.
RT3_NOINLINE void __stdcall Territory_GiveToOwner(int32_t* owner, int32_t* filter, int32_t kind) {
    if (!owner) return;
    if (kind == 0xb || kind == 0xc || kind == 9) {
        for (int32_t i = 0; i < Collection_Count(g_buildings); ++i) {
            Collection* c = g_buildings;
            uint8_t* b = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
            if (Obj_Flag3d4(b) != 1) continue;
            if (filter && *filter != (int32_t)Obj_GridByte(b)) continue;
            int32_t t = ((int32_t(__fastcall*)(uint8_t*))(*(void***)b)[0x70 / 4])(b);
            if (t ? kind == 0xc : kind == 0xb) continue;
            F<int32_t>(b, 0x276) = *owner;
        }
    }
    if (kind == 0x17 || kind == 9) {
        for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
            Collection* c = g_trains;
            uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
            if (filter && *filter != (int32_t)Obj_CityGridByte(t) && *filter != (int32_t)Train_GridByteAtCar45(t)) continue;
            F<int32_t>(t, 0x51) = *owner;
        }
    }
    if (kind != 0xa && kind != 9) return;
    for (int32_t i = 0; i < Collection_Count(g_tracks); ++i) {
        Collection* c = g_tracks;
        uint8_t* n = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (n[0x216] != 2) continue;
        if (filter && *filter != (int32_t)Obj_CenterGridByte(n)) continue;
        Track_SetOwner(n, 0, (uint16_t)*owner);
        if (!Collection_IsValid(g_routes, 0, F<uint32_t>(n, 0x212))) continue;
        uint8_t* st = (uint8_t*)Collection_Get(g_routes, 0, F<uint32_t>(n, 0x212));
        if (F<int32_t>(st, 0x1e) != *owner)
            F<int32_t>(Collection_Get(g_routes, 0, F<uint32_t>(n, 0x212)), 0x1e) = *owner;
    }
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x659d, 0, 0, 0);
}
RT3_REPLACE(0x430040, Territory_GiveToOwner);

// 0x430270. Scenario event rec: builds rec +0x11 industries of the type named
// by event table entry rec[0] (0x61039c, 0x6e apart, -0x1f7) around cell
// (+0x12, +0x14) within radius +0x16 (at least 1): each takes up to 200
// tries at random spots (sum of two SimRand(0, 20r) / 0x5c9be0 from the
// corner) inside the radius and the map, a random angle (quarter turns for
// the first 25 tries) and the cell's city as builder. The city collection's
// +0x88 is held up meanwhile.
RT3_NOINLINE void __stdcall Event_BuildIndustries(const uint8_t* rec) {
    int32_t count = rec[0x11];
    uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, *(int32_t*)(0x61039c + F<int32_t>((void*)rec, 0) * 0x6e) - 0x1f7);
    if (!type) return;
    uint32_t bt = Obj_FindRecord62b2fc(type);
    if (!bt) return;
    ++F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88);
    int32_t radius = F<uint16_t>((void*)rec, 0x16) > 1 ? F<uint16_t>((void*)rec, 0x16) : 1;
    int32_t tries = 0;
    while (count > 0 && tries <= 0xc8) {
        ++tries;
        int32_t span = radius * 20;
        int32_t a = SimRand(0, span);
        int32_t b = SimRand(0, span);
        double cx = (double)(F<uint16_t>((void*)rec, 0x12) - radius);
        float fx = (float)((double)(b + a) / D(0x5c9be0) + cx);
        int32_t c = SimRand(0, span);
        int32_t d = SimRand(0, span);
        float fy = (float)((double)(d + c) / D(0x5c9be0) + (double)(F<uint16_t>((void*)rec, 0x14) - radius));
        int32_t yi = Ftol(fy);
        int32_t xi = Ftol(fx);
        float dist = (float)(uint16_t)DistanceInt(xi, yi, F<uint16_t>((void*)rec, 0x12), F<uint16_t>((void*)rec, 0x14));
        if (!(dist <= (float)radius)) continue;
        uint8_t* m = g_map;
        if (fx < g_5c8598 || !(fx <= (float)(F<int32_t>(m, 0x2155) - 1))) continue;
        if (fy < g_5c8598 || !(fy <= (float)(F<int32_t>(m, 0x2159) - 1))) continue;
        float angle;
        if (tries <= 0x19) angle = (float)((double)(float)SimRand(0, 3) * Fl(0x5c8760) * D(0x5c8628));
        else angle = (float)((double)SimRand(0, 0x3e7) / D(0x5c87f8) * D(0x5c8c08));
        uint16_t cityId = (uint16_t)Map_CellCity(g_map, 0, xi, yi);
        uint8_t* city = (uint8_t*)Collection_Get(g_cities, 0, cityId);
        if (!city) continue;
        uint32_t out;
        if (Company_BuildBuilding(city, 0, bt, fx, fy, *(uint32_t*)&angle, 0, &out)) {
            --count;
            tries = 0;
        }
    }
    --F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88);
}
RT3_REPLACE(0x430270, Event_BuildIndustries);

// 0x430560. Expands scenario text: "[name]" becomes the value of rule variable name
// (table 0x5f3d9c, 901 entries of 0x81 bytes) for the given player / company / city,
// formatted by the variable's type, or by a "[name~~format]" override (number,
// yes/no word pairs). Copies multibyte characters whole. An unknown name formats a
// warning when the debug option +0x68 is on, but the text is never used.
RT3_NOINLINE void __fastcall RuleText_Expand(uint8_t* self, int, const char* src, char* dst, int32_t size, uint8_t* pl, uint8_t* co, uint8_t* st) {
    typedef LPSTR(WINAPI * CharNextExFn)(WORD, LPCSTR, DWORD);
    if (!st) {
        if (!co) goto start;
        st = (uint8_t*)Company_Owner(co);
    }
    if (!co && st && Collection_IsValid(g_companies, 0, F<uint32_t>(st, 0x1dd)))
        co = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(st, 0x1dd));
start:
    memset(dst, 0, (size_t)size);
    const char* p = src;
    char* out = dst;
    if (!*p) return;
    CharNextExFn charNext = hook::As<CharNextExFn>(g_5c8244);
    char buf[100], name[200], fmt[700];
    for (;;) {
        if (*p == '[') {
            const char* tag = ++p;
            int32_t mode = 0;
            name[0] = 0;
            bool scanned = false;
            for (;;) {
                char c = *p;
                if (!c) break;
                if (c == ']') {
                    StrCopyN(name, tag, (size_t)(p - tag));
                    ++p;
                    scanned = true;
                    break;
                }
                if (c == '~' && p[1] == '~') {
                    StrCopyN(name, tag, (size_t)(p - tag));
                    p += 2;
                    const char* f = p;
                    while (*p && *p != ']') ++p;
                    if (*p == ']') {
                        StrCopyN(fmt, f, (size_t)(p - f));
                        if (strlen(fmt) >= 1)
                            for (int32_t i = 0; i < 6; ++i)
                                if (!Stricmp(fmt, GetString(*(int32_t*)(0x620b6c + i * 4)))) mode = i;
                        ++p;
                    }
                    scanned = true;
                    break;
                }
                p = g_6d4034 ? charNext(hook::As<WORD>(g_624328), p, 0) : p + 1;
            }
            if (scanned) {
                char up = (char)ToUpper(name[0]);
                int32_t var = 0;
                const char* e = (const char*)0x5f3d9c;
                for (; (uint32_t)e < 0x6103a1; e += 0x81, ++var)
                    if ((char)ToUpper(*e) == up && !Stricmp(name, e)) break;
                if ((uint32_t)e < 0x6103a1) {
                    int32_t v = RuleVar_Get(var, pl, co, st, 0, 0, 0, 0);
                    buf[0] = *(char*)0x5c87a8;
                    if (mode == 0) {
                        uint8_t* info = (uint8_t*)0x5f3d98 + var * 0x81;
                        int32_t type = F<int32_t>(info, 0x4c);
                        if (type == 0 || type == 2 || type == 3) strcpy(buf, GetString(v > 0 ? 0x6ce : 0x6cf));
                        else {
                            switch (info[0x4b]) {
                            case 0: {
                                int32_t kind = F<int32_t>(info, 0);
                                Num_Format(buf, kind == 7 || kind == 6 || kind == 0x3c ? 8 : 1, (double)v, 10, 1, 0, 0);
                                break;
                            }
                            case 1: Num_Format(buf, 5, (double)v / D(0x5c85d8), 10, 1, 0, 0); break;
                            case 2: Num_FormatSimple(buf, 9, (double)v, 0xb); break;
                            case 4:
                                if (v) strcpy(buf, (const char*)(uintptr_t)v);
                                break;
                            }
                        }
                    } else if (mode == 1) Num_FormatSimple(buf, 9, (double)v, 0xb);
                    else strcpy(buf, GetString(0xb16 + (mode - 2) * 2 + (v != 0 ? 0 : 1)));
                    int32_t len = (int32_t)strlen(buf);
                    if (len - (int32_t)(uintptr_t)dst + (int32_t)(uintptr_t)out + 2 < size) {
                        memcpy(out + strlen(out), buf, (size_t)len);
                        out += strlen(buf);
                    }
                }
                if (var >= 0x385 && F<int32_t>(g_app, 0x68)) {
                    char warn[0x1f4];
                    FormatText(warn, GetString(0xd4), name, (const char*)self + 0x18, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                }
            }
        } else if (g_6d4034) {
            const char* from = p;
            p = charNext(hook::As<WORD>(g_624328), p, 0);
            while (from != p) *out++ = *from++;
        } else *out++ = *p++;
        if ((int32_t)(out - dst) + 2 >= size) {
            *out = 0;
            return;
        }
        if (!*p) return;
    }
}
RT3_REPLACE(0x430560, RuleText_Expand);

// 0x430b50. Adds a scenario event record (table 0x62be18, named by string 0xd5 with
// this +0x18) that fires delay->years/months (+9/+0xd) from now (events 7 = year,
// 8 = month) with the condition (+0 id, +4 value, +8 kind: 1 = flag, 2 = negated).
// Optional values a/b/c fill +0x7f0 (else 99), +0x82c and +0x830.
RT3_NOINLINE void __fastcall Events_AddTimed(uint8_t* self, int, uint8_t* cond, int32_t* a, int32_t* b, int32_t* c) {
    Collection* t = g_events;
    uint32_t key = Collection_Count(t) + 1;
    char name[0x190];
    Fmt(name, GetString(0xd5), (const char*)self + 0x18);
    uint8_t* r = (uint8_t*)Collection_Get(g_events, 0,
                                          Table_AddNamedRecord(g_events, 0, name, key));
    r[0x7ee] = 0;
    r[0x7ef] = World()[0x11] < 0xe ? 2 : 3;
    F<int32_t>(r, 0x7f5) = 1;
    r[0x7f4] = self[0x7f4];
    if (b) {
        r[0x7f9] = 0x63;
        F<int32_t>(r, 0x82c) = *b;
    } else r[0x7f9] = 0;
    if (c) {
        r[0x7fa] = 0x63;
        F<int32_t>(r, 0x830) = *c;
    } else r[0x7fa] = 0;
    F<int32_t>(r, 0x7f0) = a ? *a : 0x63;
    r[0x7fb] = 0;
    F<int32_t>(r, 0x7ff) = 1;
    r[0x82b] = 1;
    uint8_t* w = World();
    int32_t year = F<uint16_t>(w, 0xd) + F<int32_t>(cond, 9);
    int32_t month = w[0xf] + F<int32_t>(cond, 0xd);
    if (month > 12) {
        year += (month - 1) / 12;
        month = (month - 1) % 12 + 1;
    }
    uint8_t* e1 = (uint8_t*)ExeNew(0x1e);
    memset(e1, 0, 0x1e);
    F<int32_t>(e1, 0) = 7;
    e1[4] = 4;
    F<int32_t>(e1, 5) = 0;
    F<int32_t>(e1, 9) = year;
    e1[0xd] = 2;
    F<uint8_t*>(r, 0) = e1;
    uint8_t* e2 = (uint8_t*)ExeNew(0x1e);
    memset(e2, 0, 0x1e);
    F<int32_t>(e2, 0) = 8;
    e2[4] = 4;
    F<int32_t>(e2, 5) = 0;
    F<int32_t>(e2, 9) = month;
    e2[0xd] = 0;
    F<int32_t>(e2, 0x1a) = 0;
    F<uint8_t*>(e1, 0x1a) = e2;
    uint8_t* n = (uint8_t*)ExeNew(0x28);
    F<int32_t>(n, 0) = F<int32_t>(cond, 0);
    uint8_t kind = cond[8];
    if (kind == 1) F<int32_t>(n, 4) = F<int32_t>(cond, 4) != 0;
    else if (kind == 2) F<int32_t>(n, 4) = -F<int32_t>(cond, 4);
    else F<int32_t>(n, 4) = 0;
    n[8] = cond[8];
    F<int32_t>(n, 9) = -1;
    F<int32_t>(n, 0xd) = -1;
    F<int32_t>(n, 0x18) = 0;
    F<int32_t>(n, 0x1c) = 0;
    F<int32_t>(n, 0x20) = 0;
    F<int32_t>(n, 0x24) = 0;
    F<uint8_t*>(r, 4) = n;
}
RT3_REPLACE(0x430b50, Events_AddTimed);

// 0x430d70. Saves the scenario's event rules: the six texts (tags converted first),
// the triggers (list +0, next +0x1a) and the four effect lists (+4.., next +0x24)
// field by field, each followed by the names of the cargo / building / industry it
// refers to (so they survive re-numbering); tagged 0x526f .. 0x4eb8 .. 0x4eb9.
void __fastcall EventRules_Save(uint8_t* o, int, Stream st) {
    Text_ConvertTags((char*)o + 0xe0, 0x320, 0);
    static const uint32_t texts[] = { 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
    for (uint32_t t : texts) Text_ConvertTags((char*)o + t, 0xc8, 0);
    int32_t triggers = 0;
    int32_t effects[4] = {};
    for (uint8_t* e = F<uint8_t*>(o, 0); e; e = F<uint8_t*>(e, 0x1a)) ++triggers;
    for (int32_t k = 0; k < 4; ++k)
        for (uint8_t* e = F<uint8_t*>(o, 4 + k * 4); e; e = F<uint8_t*>(e, 0x24)) ++effects[k];
    uint32_t tag = 0x526f;
    StreamWrite(st, &tag, 4, 0);
    StreamWrite(st, &triggers, 4, 0);
    for (uint8_t* e = F<uint8_t*>(o, 0); e; e = F<uint8_t*>(e, 0x1a)) {
        static const uint8_t fields[][2] = { { 0, 4 }, { 4, 1 }, { 5, 4 }, { 9, 4 }, { 0xd, 1 }, { 0xe, 4 }, { 0x12, 4 } };
        for (auto& f : fields) StreamWrite(st, e + f[0], f[1], 0);
        int32_t kind = F<int32_t>(e, 0);
        if (kind >= 0x83 && kind <= 0x35e) WriteName(st, 0x62ba8c, (kind - 0x83) % 0x3d, 4);
    }
    StreamWriteU32(st, 0x4eb8);
    for (int32_t k = 0; k < 4; ++k) {
        StreamWrite(st, &effects[k], 4, 0);
        for (uint8_t* e = F<uint8_t*>(o, 4 + k * 4); e; e = F<uint8_t*>(e, 0x24)) {
            static const uint8_t fields[][2] = { { 0, 4 }, { 4, 4 }, { 8, 1 }, { 9, 4 }, { 0xd, 4 },
                                                 { 0x11, 4 }, { 0x12, 4 }, { 0x14, 4 }, { 0x16, 4 }, { 0x18, 0xc } };
            for (auto& f : fields) StreamWrite(st, e + f[0], f[1], 0);
            int32_t kind = F<int32_t>(e, 0), idx;
            if (kind >= 0xfb && kind <= 0x15f) idx = kind - 0xfb;
            else if (kind >= 0x160 && kind <= 0x1c4) idx = kind - 0x160;
            else idx = -1;
            if (idx != -1) WriteName(st, 0x6ada84, idx, 0x34);
            kind = F<int32_t>(e, 0);
            if (kind >= 0x6a && kind <= 0xb0) idx = kind - 0x6a;
            else if (kind >= 0xb4 && kind <= 0xfa) idx = kind - 0xb4;
            else idx = -1;
            if (idx != -1) WriteName(st, 0x62ba8c, idx, 4);
            kind = F<int32_t>(e, 0);
            if (kind >= 0x1f7 && kind <= 0x265) WriteName(st, 0x62b268, kind - 0x1f7, 4);
        }
    }
    StreamWriteU32(st, 0x4eb9);
}
RT3_REPLACE(0x430d70, EventRules_Save);

// 0x431120 (with its switch cases at 0x431400). Checks scenario setup s and
// copies the problems into out (when given): no goals (string 0xd6), no
// limits and no description (+0xe0; 0xd7), and per condition (list s[0],
// 0x1a-byte links) a share goal in a non-competitive game (0x3c, 0xfd8), an
// unknown territory (0x4b / 0x4d, 0xb63) or city (0x4c / 0x4e / 0x35f /
// 0x360, 0xb64; the second city 0xb65). For each player slot (only the
// first unless +0x7ee) the wanted kinds (Obj_ListFlags: 1 / 2 / 4) need a
// matching start kind (+0x7fb; 0 by its cargo, 2..4, 6..8, 5), else strings
// 0xb66 / 0xb67 / 0xb68. Returns 1 when there is nothing to report.
RT3_NOINLINE int32_t __fastcall Scenario_CheckSetup(uint32_t* s, int, char* out) {
    char msg[0x3e8], line[0x1f4];
    msg[0] = *(const char*)0x5c87a8;
    if (!s[0]) strcat(msg, GetString(0xd6));
    if (!s[1] && !s[2] && !s[3] && !s[4] && strlen((const char*)s + 0xe0) <= 1) {
        if (strlen(msg)) memcpy(msg + strlen(msg), (const void*)0x5c9ce4, 3);
        strcat(msg, GetString(0xd7));
    }
    const uint8_t* bytes = (const uint8_t*)s;
    for (uint8_t* c = (uint8_t*)(uintptr_t)s[0]; c; c = F<uint8_t*>(c, 0x1a)) {
        const uint32_t kind = F<uint32_t>(c, 0);
        if (kind == 0x3c && (bytes[0x7ef] == 0 || bytes[0x7ef] == 1)) strcat(msg, GetString(0xfd8));
        if ((F<uint32_t>(c, 0) == 0x4b || F<uint32_t>(c, 0) == 0x4d) && !Collection_IsValid(g_territories, 0, F<uint32_t>(c, 0xe)))
            strcat(msg, GetString(0xb63));
        const uint32_t k2 = F<uint32_t>(c, 0);
        if ((k2 == 0x4c || k2 == 0x4e || k2 == 0x35f || k2 == 0x360) && !Collection_IsValid(g_cities, 0, F<uint32_t>(c, 0xe)))
            strcat(msg, GetString(0xb64));
        const uint32_t k3 = F<uint32_t>(c, 0);
        if ((k3 == 0x35f || k3 == 0x360) && !Collection_IsValid(g_cities, 0, F<uint32_t>(c, 0x12)))
            strcat(msg, GetString(0xb65));
    }
    for (int32_t i = 0; i < 4; ++i) {
        if (i > 0 && bytes[0x7ee] != 1) continue;
        const uint8_t want = Obj_ListFlags(s, 0, i, 1);
        bool a = false, b = false, d = false;
        switch (bytes[0x7fb + i]) {
        case 0: {
            const uint8_t f = Load_CargoFlags(s);
            if (f & 1) a = true;
            if (f & 2) b = true;
            if (f & 4) d = true;
            break;
        }
        case 2:
        case 3:
        case 4:
            a = true;
            break;
        case 5:
            d = true;
            break;
        case 6:
        case 7:
        case 8:
            b = true;
            break;
        }
        if ((want & 1) && !a) {
            FormatText(line, GetString(0xb66), 0, 0, 0, 0, 0, 0, 0, 0);
            strcat(msg, line);
        }
        if ((want & 2) && !b && !a) {
            FormatText(line, GetString(0xb67), 0, 0, 0, 0, 0, 0, 0, 0);
            strcat(msg, line);
        }
        if ((want & 4) && !d) {
            FormatText(line, GetString(0xb68), 0, 0, 0, 0, 0, 0, 0, 0);
            strcat(msg, line);
        }
    }
    if (out) strcpy(out, msg);
    return strlen(msg) == 0 ? 1 : 0;
}
RT3_REPLACE(0x431120, Scenario_CheckSetup);

void __fastcall Obj_FreeAllLists2(void* o) {
    List1a_Free((uint8_t**)o);
    Obj_FreeFourLists(o);
}
RT3_REPLACE(0x4315c0, Obj_FreeAllLists2);

// 0x4315d0. Applies effect record rec ({id, value, kind byte}) through scope `slot`
// (+0x7fb kinds, +0x7ff keyed): to the company (a2, slots 35b), player (a3, 2ab),
// region (a1, 3b5) or the world (8a), per the effect's flags (table 0x6103fd, 0x6e bytes).
RT3_NOINLINE void __fastcall Effects_Apply(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company,
                              uint8_t* player, int32_t slot) {
    int32_t id = rec[0];
    int32_t value = rec[1];
    int32_t add = ((const uint8_t*)rec)[8] == 2 ? 1 : 0;
    if ((id == 0x1c6 || id == 0x1c7 || id == 0x1c8 || (id >= 0xfb && id <= 0x15f)) && value == 0) value = -1;
    int32_t key = 0;
    if (F<int32_t>(fx, 0x7ff + slot * 4))
        key = F<int32_t>(World(), 0x15) * 1000 + (int32_t)(F<uint32_t>(fx, 0x14) % 1000) + (id << 16);
    uint32_t kind = fx[0x7fb + slot];
    if (id >= 0x27 && id <= 0x2a) kind = 1;
    if (kind > 8) return;
    uint8_t flags = ((const uint8_t*)0x6103fd)[id * 0x6e];
    switch (kind) {
    case 0:
        if (company && (flags & 1)) return Obj_SetSlot35b(company, 0, id, value, key, add);
        if (player && (((const uint8_t*)0x6103fd)[id * 0x6e] & 2)) return Obj_SetSlot2ab(player, 0, id, value, key, add);
        if (region && (((const uint8_t*)0x6103fd)[id * 0x6e] & 4)) return Obj_SetSlot3b5(region, 0, id, value, key, add);
        if (((const uint8_t*)0x6103fd)[id * 0x6e] & 8) Obj_SetSlot8a(World(), 0, id, value, key, add);
        return;
    case 1:
        if (flags & 8) Obj_SetSlot8a(World(), 0, id, value, key, add);
        return;
    case 2: case 3: case 4:
        if (company) Obj_SetSlot35b(company, 0, id, value, key, add);
        return;
    case 5:
        if (region) Obj_SetSlot3b5(region, 0, id, value, key, add);
        return;
    default:
        if (player) Obj_SetSlot2ab(player, 0, id, value, key, add);
        return;
    }
}
RT3_REPLACE(0x4315d0, Effects_Apply);

// 0x4317c0. Shows event e's text (+0xe0, with the player / company / station
// placeholders of pl, co, st filled) the way its kind (+0x7ee) asks: 0 the
// scenario status line (date record +0x4b47 when `status`, else a world
// entry); 1 a message box with up to four radio choices (+0x4ca, 0xc9 each;
// returns the pick, single player only, then flags 0x62be18 +0x88); 2 a
// newspaper with headline +0x401; 3 a log entry (also copied to out); 4 the
// text into out. Nothing for a blank text.
RT3_NOINLINE uint32_t __fastcall Event_ShowText(uint8_t* e, int, uint8_t* pl, uint8_t* co, uint8_t* st, char* out, int32_t status) {
    TrimSpaces((char*)e + 0xe0, 1);
    if (!strlen((const char*)e + 0xe0)) return 0;
    int32_t picked = 0;
    char text[0x320];
    switch (e[0x7ee]) {
    case 0:
        RuleText_Expand(e, 0, (const char*)e + 0xe0, text, 0x320, pl, co, st);
        if (status) StrCopyN((char*)g_world + 0x4b47, text, 0x12c);
        else World_AddNamedEntry(g_world, 0, text, 0, 1, -1, -1, -1, -1);
        return picked;
    case 1: {
        const char* choices[4];
        int32_t n = 0;
        for (int32_t k = 0; k < 4; ++k) {
            TrimSpaces((char*)e + 0x4ca + k * 0xc9, 1);
            choices[k] = (const char*)e + 0x4ca + k * 0xc9;
        }
        for (int32_t k = 0; k < 4; ++k) {
            if (!e[0x4ca + k * 0xc9]) break;
            ++n;
        }
        if (CountHumanPlayers() > 1) {
            F<int32_t>(hook::As<uint8_t*>(g_events), 0x88) = 1;
            return 0;
        }
        RuleText_Expand(e, 0, (const char*)e + 0xe0, text, 0x320, pl, co, st);
        Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0, n, choices, &picked, 1, 0, nullptr, 0);
        F<int32_t>(hook::As<uint8_t*>(g_events), 0x88) = 1;
        return (uint32_t)picked;
    }
    case 2: {
        char headline[0xc8];
        RuleText_Expand(e, 0, (const char*)e + 0xe0, text, 0x320, pl, co, st);
        RuleText_Expand(e, 0, (const char*)e + 0x401, headline, 0xc8, pl, co, st);
        News_Add(g_6cea4c, 0, text, headline, 0, 0);
        return picked;
    }
    case 3:
        RuleText_Expand(e, 0, (const char*)e + 0xe0, text, 0x320, pl, co, st);
        strcpy(out, text);
        Log_Add(g_messageLog, 0, text, 0xb, 0, 0);
        return picked;
    case 4:
        RuleText_Expand(e, 0, (const char*)e + 0xe0, text, 0x320, pl, co, st);
        strcpy(out, text);
        return picked;
    }
    return 0;
}
RT3_REPLACE(0x4317c0, Event_ShowText);

// 0x431b20. Event effect rec (+0 effect id, +4 value, +8 op: 2 add, 4 set)
// for the effect table's kind (0x61039c + id * 0x6e): 0x1f7..0x265 build
// industries; 1 / 2 a player's cash (+0x154) / company's (+0x11f7, stat 0xd);
// 3 a company's territory entry flag (refresh 0x948c); 4..7 the game ends
// (won 2 / 3 / 4, lost 1 - in multiplayer each player wins (4) unless the
// effect is a loss or it is someone else's company or player; the game then
// stops); 8 difficulty; 9..12, 23 territory to the owner; 13 company
// dissolves; 14 player +4 = 0; 15 trains in the region / company / player's
// company go; 16 company +0x7680 (at least 0); 17..21 editor deletions; 22
// weather override; 24 the player becomes the local human; 25..28 camera
// shakes; 29 the scenario texts (+0xe0) become the world's. Others go to
// Effects_Apply.
RT3_NOINLINE void __fastcall Effects_Dispatch(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company, uint8_t* player, int32_t slot) {
    int32_t kind = *(int32_t*)(0x61039c + rec[0] * 0x6e);
    if (kind >= 0x1f7 && kind <= 0x265) {
        Event_BuildIndustries((const uint8_t*)rec);
        return;
    }
    uint8_t* w = g_world;
    uint8_t op = ((const uint8_t*)rec)[8];
    switch (kind) {
    case 4: case 5: case 6: case 7: {
        int32_t mp = IsMultiplayer();
        w = g_world;
        if (F<int32_t>(w, 0x4a73)) return;
        if (mp) {
            F<int32_t>(w, 0x4a73) = 4;
            if (*(int32_t*)(0x61039c + rec[0] * 0x6e) == 7) {
                F<int32_t>(g_world, 0x4a73) = 1;
            } else if (player) {
                if (F<int32_t>(player, 0) != Obj_Get25(hook::As<void*>(g_world))) F<int32_t>(g_world, 0x4a73) = 1;
            } else if (company) {
                if (F<int32_t>(company, 0) != Obj_Get21(hook::As<void*>(g_world))) F<int32_t>(g_world, 0x4a73) = 1;
            }
            g_world[0x4b47] = *(const char*)0x5c87a8;
            F<int32_t>(g_world, 0x4d) = 0;
            w = g_world;
            F<int32_t>(w, 0x4c88) = F<int32_t>(w, 0x15);
            if (hook::As<uint32_t>(g_6d40dc)) World_SetSpeed(g_world, 0, 0, 1, 0);
            return;
        }
        static const int32_t kEnd[] = { 2, 3, 4, 1 };
        F<int32_t>(w, 0x4a73) = kEnd[*(int32_t*)(0x61039c + rec[0] * 0x6e) - 4];
        w = g_world;
        if (F<int32_t>(w, 0x4a73)) {
            F<int32_t>(w, 0x4d) = 0;
            w = g_world;
            F<int32_t>(w, 0x4c88) = F<int32_t>(w, 0x15);
            w = g_world;
        }
        w[0x4b47] = *(const char*)0x5c87a8;
        return;
    }
    case 1:
        if (!player) return;
        if (op == 4) F<double>(player, 0x154) = (double)rec[1];
        else Obj_AddDouble154(player, 0, rec[1]);
        return;
    case 2:
        if (!company) return;
        if (op == 4) F<double>(company, 0x11f7) = (double)rec[1];
        else Company_AddStat(company, 0, 0xd, (float)rec[1]);
        return;
    case 8:
        if (op == 2) World_SetDifficulty(g_world, 0, F<int32_t>(g_world, 0x2d) + rec[1]);
        else if (rec[1] >= 0) World_SetDifficulty(g_world, 0, rec[1]);
        return;
    case 22:
        if (g_world) Weather_ScheduleNext(g_world, 0, 1);
        Options_SetWeatherOverride(0, 0);
        return;
    case 25: case 26: case 27: case 28: {
        static const uint32_t kShake[] = { 0x3e800000, 0x3f0ccccd, 0x3f800000, 0x3fcccccd };
        if (uint8_t* cam = g_62be68) Camera_StartShake(cam, 0, *(const float*)&kShake[kind - 25]);
        return;
    }
    case 16: {
        if (!company) return;
        int32_t v = op == 2 ? Company_Get7680(company) : 0;
        v += rec[1];
        F<int32_t>(company, 0x7680) = v < 0 ? 0 : v;
        return;
    }
    case 3:
        if (!company || !region) return;
        Company_SetEntryFlag(company, 0, *(int32_t*)region, (uint8_t)rec[1]);
        if (hook::As<uint8_t**>(g_windowManager)) Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x948c, 0, 0, 0);
        return;
    case 9: case 10: case 11: case 12: case 23:
        if (!company) return;
        Territory_GiveToOwner((int32_t*)company, (int32_t*)region, kind);
        return;
    case 17: case 18: case 19: case 20: case 21:
        Editor_DeleteKind((int32_t*)company, (uint32_t*)region, kind);
        return;
    case 13:
        if (company) Company_Dissolve(company);
        return;
    case 15:
        for (int32_t i = Collection_Count(g_trains) - 1; i >= 0; --i) {
            Collection* ts = g_trains;
            uint8_t* t = (uint8_t*)Collection_Get(ts, 0, Collection_IdAt(ts, 0, (uint32_t)i, 0));
            if (region && *(uint32_t*)region != Obj_CityGridByte(t)) continue;
            if (company && F<int32_t>(company, 0) != F<int32_t>(t, 0x51)) continue;
            if (player && F<int32_t>(player, 0x1dd) != F<int32_t>(t, 0x51)) continue;
            ts = g_trains;
            CollectionVtbl* vt = ts->vtbl;
            vt->release(g_trains, Collection_IdAt(ts, 0, (uint32_t)i, 0));
        }
        return;
    case 24:
        if (!player) return;
        F<int32_t>(Obj_Ref25(hook::As<void*>(g_world)), 0x1e1) = 0;
        Obj_SetPlayer(hook::As<void*>(g_world), 0, F<uint32_t>(player, 0));
        F<int32_t>(player, 0x1e1) = 1;
        return;
    case 29:
        StrCopyN((char*)g_world + 0x4f30, (const char*)fx + 0xe0, 0xbb8);
        StrCopyN((char*)g_world + 0x5ae9, (const char*)fx + 0xe0, 0xbb8);
        return;
    case 14:
        if (player) F<int32_t>(player, 4) = 0;
        return;
    default:
        Effects_Apply(fx, 0, rec, region, company, player, slot);
        return;
    }
}
RT3_REPLACE(0x431b20, Effects_Dispatch);

}  // namespace rt3
