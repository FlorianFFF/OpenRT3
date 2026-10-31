// Larger game functions, part 9 (batch 208). See objects.cpp for conventions.
#include <windows.h>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o);
uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t);
double __stdcall Effects_CargoMultiplier(int32_t idx);
uint8_t* __fastcall Obj_BuildingRegion(void* o);

// 0x407bd0. Re-evaluates the company's (id c[0]) station records (+0x5bd[id]): for
// each active station, every free (non-city) cargo on offer is priced at its local
// value (quota / price multiplier) and matched with the best-paying linked
// destination (record list +6, 13 bytes: on, station, distance, rate) by gain per
// distance (+2000 margin); the totals go to +0x12 (supply) and +0xe (gain), and the
// best ratios fill up to 6 loads into +0x16 (expected rate). Dated +0xa / +0xd3a.
void __fastcall Company_RateStations(int32_t* co) {
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    F<int32_t>(co, 0xd3a) = F<int32_t>(world, 0x15);
    Collection* stations = hook::Global<Collection*>(0x6cec20);
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
        stations = hook::Global<Collection*>(0x6cec20);
        uint8_t* s = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, i, 0));
        auto rec = [&]() { return F<uint8_t**>(s, 0x5bd)[co[0]]; };
        if (!rec()[0] || !rec()[1]) continue;
        F<int32_t>(rec(), 0xa) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
        F<int32_t>(rec(), 0xe) = 0;
        F<int32_t>(rec(), 0x12) = 0;
        F<int32_t>(rec(), 0x16) = 0;
        int32_t n = Obj_CountAvailableCargos(s);
        float stock[0x35] = {}, value[0x35] = {}, ratio[0x35], gain[0x35];
        uint8_t has[0x35] = {}, used[0x35];
        int32_t lo = 9999, hi = 0;
        for (int32_t k = 0; k < n; ++k) {
            float amount, quota;
            uint32_t id = Obj_NthAvailableCargo(s, 0, k + 1, &amount, &quota, 0);
            if (F<int32_t>(hook::Global<Collection*>(0x62ba8c), 0x9a + id * 4) != -1) continue;
            if (!(amount > Fl(0x5c8598))) continue;
            if (lo >= (int32_t)id) lo = id;
            if (hi <= (int32_t)id) hi = id;
            stock[id] = amount;
            float mul = (float)Effects_CargoMultiplier(id);
            double d = D(0x5c85b8) > (double)mul ? 0.01 : (double)mul;
            value[id] = (float)((double)quota / d);
        }
        for (int32_t j = 0; j < F<int32_t>(rec(), 2); ++j) {
            uint8_t* e = F<uint8_t*>(rec(), 6) + j * 0xd;
            if (!e[0]) continue;
            const uint16_t* want =
                (const uint16_t*)Obj_BuildingRegion(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(e, 1)));
            for (int32_t c = lo; c <= hi; ++c) {
                if (!(stock[c] > Fl(0x5c8598))) continue;
                float q = (float)((int32_t)want[c] << 4);
                if (!((double)value[c] + Fl(0x5c88e0) < q)) continue;
                float g = (float)(((double)q - value[c]) * F<float>(e, 9));
                float dist = (float)(uint32_t)(F<int32_t>(e, 5) + 0x28);
                float r = (float)((double)g / dist);
                if (has[c] && !(r > ratio[c])) continue;
                gain[c] = g;
                has[c] = 1;
                ratio[c] = r;
            }
        }
        for (int32_t c = lo; c <= hi; ++c) {
            if (!has[c]) continue;
            F<float>(rec(), 0x12) = (float)((double)stock[c] + F<float>(rec(), 0x12));
            F<float>(rec(), 0xe) = (float)((double)gain[c] + F<float>(rec(), 0xe));
        }
        memset(used, 0, sizeof used);
        float loads = 0.0f;
        for (;;) {
            int32_t best = -1;
            for (int32_t c = lo; c <= hi; ++c)
                if (has[c] && !used[c] && (best == -1 || ratio[c] > ratio[best])) best = c;
            if (best == -1) break;
            used[best] = 1;
            double room = D(0x5c8610) - loads;
            float take = (float)(room < (double)stock[best] ? room : (double)stock[best]);
            loads = (float)((double)take + loads);
            F<float>(rec(), 0x16) = (float)((double)take * ratio[best] + F<float>(rec(), 0x16));
            if (loads > D(0x5c88d8)) break;
        }
    }
}
RT3_REPLACE(0x407bd0, Company_RateStations);

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);

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

// 0x40d540. Purchase price of the building for `company`: its type's value (+0x22,
// plus +0x43 when +0x246) blended over its first 5 years with its average yearly
// profit (revenue +0x310 less costs +0x338 / +0x360, recent years weighted 6..2, the
// current year scaled by the months elapsed), at least 0.8 x the value, rounded up
// to 10K / 50K / 100K steps and scaled by the price effects for its region.
RT3_NOINLINE float __fastcall Building_Price(uint8_t* b, int, uint32_t company) {
    auto type = [&]() { return (*(uint8_t*(__fastcall**)(void*))(*(uint8_t**)b + 0x80))(b); };
    float base = (float)F<uint32_t>(type(), 0x22);
    if (F<int32_t>(b, 0x246)) base = (float)((double)(float)F<int32_t>(type(), 0x43) + base);
    uint32_t player = 0;
    if (company) player = F<uint32_t>(Collection_Get(hook::Global<Collection*>(0x62be10), 0, company), 0x3b);
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    int32_t years = F<uint16_t>(world, 0xd) - F<int32_t>(b, 0x3d5) + 1;
    float weighted = 0.0f, weights = 0.0f;
    int32_t span = 0, n = 0;
    if (years >= 0) {
        span = years;
        n = years > 5 ? 5 : years;
    }
    for (int32_t k = 0; k < n; ++k) {
        float profit = (float)((double)F<float>(b, 0x310 + k * 4) - F<float>(b, 0x338 + k * 4) - F<float>(b, 0x360 + k * 4));
        float w = (float)(6 - k);
        if (k == 0) {
            int32_t m = world[0xf] - 1;
            if (m < 1) continue;
            profit = (float)(D(0x5c8978) / (double)m * profit);
            w = (float)((double)m / D(0x5c8978) * w);
        }
        weights = (float)((double)w + weights);
        weighted = (float)((double)w * profit + weighted);
    }
    float div = Fl(0x5c85b4) > weights ? 1.0f : weights;
    float avg = (float)((double)weighted / div);
    avg = (float)(avg / D(0x5c8658));
    double r = (double)span * D(0x5c8c20);
    float k = (float)(D(0x5c8770) < r ? 1.0 : r);
    float v = (float)((D(0x5c8770) - k) * (double)base + (double)k * avg);
    double cap = (double)base * D(0x5c8680);
    if (v < cap) v = (float)cap;
    int32_t price;
    if (v < Fl(0x5c8d20)) {
        price = Ftol((double)v / D(0x5c8d18) + D(0x5c8628)) * 10000;
        if (price < 10000) price = 10000;
    } else if (v < Fl(0x5c8d10)) {
        price = Ftol((double)v / D(0x5c8d08) + D(0x5c8628)) * 50000;
        if (price < 50000) price = 50000;
    } else {
        price = Ftol((double)v / Fl(0x5c8d00) + D(0x5c8628)) * 100000;
        if (price < 100000) price = 100000;
    }
    float fp = (float)price;
    float cy = (float)Obj_CellY(b), cx = (float)Obj_CellX(b);
    uint32_t region = Map_GridByte(hook::Global<uint8_t*>(0x62c120), 0, cx, cy);
    return (float)(Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, 0x3c, company, player, region) * (double)fp);
}
RT3_REPLACE(0x40d540, Building_Price);

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x414490. Loads a building footprint file (`path`, "rb", exe CRT stdio) into `t`:
// version, name and 5 more strings, grid size (+0xb8 w, +0xb9 h, +0xba cell
// resolution), header fields, version-dependent extras (1001+ / 1003+ 12 slots /
// 1004+ name / flags), renaming ServiceTower / Maintenance (+0xcd = 0x81 / 0x9d);
// then decodes the nibble-packed occupancy (classes 1-4 per sub-cell) into four
// bitmasks (+0xcf..+0xdb, 32 bytes per cell; freed when unused, +0x177..). Returns
// 1 when the file opened.
int32_t __fastcall BuildingType_LoadFootprint(void*, int, uint8_t* t, const char* path) {
    FILE* f = ((FILE*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c9678);
    int32_t ok = 0;
    if (f) {
        auto rd = [&](void* p, size_t n) { ((size_t(__cdecl*)(void*, size_t, size_t, FILE*))0x5a1b05)(p, 1, n, f); };
        rd(t, 4);
        static const uint16_t texts[] = { 4, 0x22, 0x40, 0x5e, 0x7c, 0x9a };
        for (uint16_t o : texts) rd(t + o, 0x1e);
        rd(t + 0xb8, 1);
        rd(t + 0xb9, 1);
        rd(t + 0xba, 1);
        rd(t + 0xbb, 4);
        rd(t + 0xbf, 4);
        rd(t + 0xc3, 4);
        rd(t + 0xc7, 4);
        rd(t + 0xcb, 2);
        rd(t + 0xcd, 2);
        if (!Stricmp((const char*)t + 4, (const char*)0x5c871c)) F<uint16_t>(t, 0xcd) = 0x81;
        if (!Stricmp((const char*)t + 4, (const char*)0x5c8710)) F<uint16_t>(t, 0xcd) = 0x9d;
        int32_t size = t[0xb8] * t[0xb9] * t[0xba] * t[0xba] / 2;
        uint8_t* buf = (uint8_t*)ExeNew(size);
        rd(buf, size);
        if (F<int32_t>(t, 0) >= 0x3e9) {
            rd(t + 0xdf, 4);
            rd(t + 0xe3, 4);
            if (F<int32_t>(t, 0) >= 0x3eb)
                for (int32_t i = 0; i < 0xc; ++i) {
                    rd(t + 0xe7 + i, 1);
                    rd(t + 0xf3 + i * 2, 2);
                    rd(t + 0x10b + i * 2, 2);
                }
            if (F<int32_t>(t, 0) >= 0x3ec) rd(t + 0x123, 0x1e);
            rd(t + 0x141, 4);
            if (!F<int32_t>(t, 0x141)) F<int32_t>(t, 0x141) = 1;
            rd(t + 0x145, 0x2e);
        }
        ((int(__cdecl*)(FILE*))0x5a1740)(f);
        for (int32_t k = 0; k < 4; ++k) {
            uint32_t n = t[0xb8] * t[0xb9] * 32;
            uint8_t* m = (uint8_t*)ExeNew(n);
            F<uint8_t*>(t, 0xcf + k * 4) = m;
            memset(m, 0, n);
        }
        int32_t q = t[0xba] >> 4;
        int32_t state = 2, src = 0, dst = 0, bit = 7;
        uint8_t bytes[3];
        for (int32_t y = 0; y < t[0xb9]; ++y) {
            for (int32_t x = 0; x < t[0xb8]; ++x) {
                for (int32_t a = 0; a < 4; ++a) {
                    for (int32_t b = 0; b < 4; ++b) {
                        for (int32_t c = 0; c < 4; ++c) {
                            for (int32_t d = 0; d < 4; ++d) {
                                uint8_t cls[4] = {};
                                for (int32_t i = 0; i < q;) {
                                    bool high;
                                    if (state == 2) {
                                        int32_t half = t[0xba] >> 1;
                                        bytes[0] = buf[src];
                                        bytes[1] = buf[src + half];
                                        bytes[2] = buf[src + half * 2];
                                        ++src;
                                        high = true;
                                    } else if (state == 0) high = true;
                                    else if (state == 1) high = false;
                                    else continue;
                                    if (high) {
                                        for (int32_t k = 0; k < 3; ++k) {
                                            uint32_t v = (uint32_t)(bytes[k] >> 4) - 1;
                                            if (v <= 3) cls[v] = 1;
                                        }
                                        state = 1;
                                        ++i;
                                        if (i >= q) break;
                                    }
                                    for (int32_t k = 0; k < 3; ++k) {
                                        uint32_t v = (uint32_t)(bytes[k] & 0xf) - 1;
                                        if (v <= 3) cls[v] = 1;
                                    }
                                    state = 2;
                                    ++i;
                                }
                                for (int32_t k = 0; k < 4; ++k)
                                    if (cls[k]) {
                                        F<uint8_t*>(t, 0xcf + k * 4)[dst] |= (uint8_t)(1 << bit);
                                        F<int32_t>(t, 0x177 + k * 4) = 1;
                                    }
                                if (--bit < 0) {
                                    ++dst;
                                    bit = 7;
                                }
                            }
                            src += (t[0xba] * 3 - q * 4) / 2;
                        }
                        src += -((t[0xba] * 4 - 4) * q / 2);
                    }
                    src += t[0xba] * (q * 4 - 1) / 2;
                }
            }
        }
        ExeDelete(buf);
        F<uint32_t>(t, 0x1a3) = 0x3f800000;
        ok = 1;
    }
    for (int32_t k = 0; k < 4; ++k)
        if (!F<int32_t>(t, 0x177 + k * 4)) {
            ExeDelete(F<void*>(t, 0xcf + k * 4));
            F<void*>(t, 0xcf + k * 4) = nullptr;
        }
    return ok;
}
RT3_REPLACE(0x414490, BuildingType_LoadFootprint);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
const char* __fastcall LocalizeName(const char* name);
const char* __fastcall GetString(int32_t id);
void __fastcall CargoTypes_MarkAvailable(Collection* c);
int __fastcall ToUpper(char c);
int32_t __fastcall Game_EffectiveVersion();

// 0x41ede0. Loads the cargo types: the Data\CargoTypes\*.cty files (up to 499,
// sorted by name) are read into records of 0xd0 bytes (at most 52 cargos): name,
// value, decay, flags, upper-cased abbreviation, and v1001+ extras; each gets its
// speed factor (+0x52), the Mail / Passengers / Troops markers (+0x8a / +0x88 /
// +0x8c, flags +0x46..+0x49), a value class (+0x4a / +0x4e) and, for the first 71,
// localized names in the cargo string tables (0x61312c / 0x6150f8, 12 more lists at
// 0x5f7f9f). Then the special cargo ids (+0x8e..+0x96) and the quota-slot table
// (+0x9a) are rebuilt and availability marked.
void __fastcall CargoTypes_Load(Collection* c) {
    static char names[0x1f3][0xc9];
    F<uint16_t>(c, 0x88) = 0;
    F<uint16_t>(c, 0x8a) = 0;
    F<uint16_t>(c, 0x8c) = 0;
    char pattern[0x100];
    FormatText(pattern, (const char*)0x5c9a08, (const char*)0x5c9a10, 0, 0, 0, 0, 0, 0, 0);
    int32_t n = 0;
    WIN32_FIND_DATAA fd;
    HANDLE h = ((HANDLE(__stdcall*)(const char*, WIN32_FIND_DATAA*))hook::Global<void*>(0x5c8190))(pattern, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        BOOL more = 1;
        while (more) {
            if (n < 0x1f3) {
                StrCopyN(names[n], fd.cFileName, 0xc8);
                more = ((BOOL(__stdcall*)(HANDLE, WIN32_FIND_DATAA*))hook::Global<void*>(0x5c8194))(h, &fd);
                ++n;
            }
        }
        ((BOOL(__stdcall*)(HANDLE))hook::Global<void*>(0x5c819c))(h);
    }
    if (n <= 0) goto specials;
    for (int32_t i = 1; i <= n; ++i)
        for (int32_t j = i; j < n; ++j)
            if (Stricmp(names[i - 1], names[j]) >= 0) {
                char tmp[0xc9];
                strcpy(tmp, names[j]);
                strcpy(names[j], names[i - 1]);
                strcpy(names[i - 1], tmp);
            }
    for (int32_t k = 0; k < n; ++k) {
        char path[0x200];
        FormatText(path, (const char*)0x5c8730, (const char*)0x5c9a10, names[k], 0, 0, 0, 0, 0, 0);
        FILE* f = ((FILE*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c9678);
        if (!f) continue;
        auto rd = [&](void* p, size_t sz) { ((size_t(__cdecl*)(void*, size_t, size_t, FILE*))0x5a1b05)(p, 1, sz, f); };
        uint8_t rec[0xd0] = {};
        rd(rec, 4);
        rd(rec + 4, 0x14);
        rd(rec + 0x18, 4);
        rd(rec + 0x1c, 4);
        rd(rec + 0x20, 1);
        rd(rec + 0x21, 4);
        rd(rec + 0x25, 0x15);
        if (F<int32_t>(rec, 0) >= 0x3e9) rd(rec + 0x3a, 4);
        else F<int32_t>(rec, 0x3a) = 0;
        if (F<int32_t>(rec, 0) <= 0x3e9) rec[0x20] = 0;
        for (int32_t i = (int32_t)strlen((const char*)rec + 0x25) - 1; i >= 0; --i)
            rec[0x25 + i] = (uint8_t)ToUpper((char)rec[0x25 + i]);
        ((int(__cdecl*)(FILE*))0x5a1740)(f);
        double x = Game_EffectiveVersion() >= 0x67 ? (double)F<float>(rec, 0x1c) * D(0x5c85a8) / D(0x5c9a00)
                                                   : (double)F<float>(rec, 0x1c) / Fl(0x5c99fc);
        F<float>(rec, 0x42) = (float)(D(0x5c8770) - x);
        if (Collection_Count(c) >= 0x34) continue;
        uint32_t id = c->vtbl->allocate(c, rec, 0, 0, 0);
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, id);
        F<uint32_t>(r, 0x3e) = id;
        F<float>(r, 0x52) = (float)((double)F<float>(r, 0x1c) / Fl(0x5c99f8) * D(0x5c8570) / D(0x5c99e8) + D(0x5c8770));
        const char* nm = (const char*)rec + 4;
        if (!Stricmp(nm, (const char*)0x5c99f0)) {
            F<uint16_t>(c, 0x8a) = (uint16_t)id;
            r[0x48] = 1;
            r[0x46] = 1;
        }
        if (!Stricmp(nm, (const char*)0x5c99d8)) {
            F<uint16_t>(c, 0x88) = (uint16_t)id;
            r[0x47] = 1;
            r[0x46] = 1;
        }
        if (!Stricmp(nm, (const char*)0x5c99d0)) {
            F<uint16_t>(c, 0x8c) = (uint16_t)id;
            r[0x49] = 1;
            r[0x46] = 1;
        }
        float value = F<float>(rec, 0x18);
        if (value >= Fl(0x5c99cc)) {
            F<uint32_t>(r, 0x4a) = 0x3fcccccd;
            F<uint32_t>(r, 0x4e) = 0x3f000000;
        } else if (value >= Fl(0x5c99c8)) {
            F<uint32_t>(r, 0x4a) = 0x3fe66666;
            F<uint32_t>(r, 0x4e) = 0x3ecccccd;
        } else if (value >= Fl(0x5c8db0)) {
            F<uint32_t>(r, 0x4a) = 0x40000000;
            F<uint32_t>(r, 0x4e) = 0x3e4ccccd;
        } else {
            F<uint32_t>(r, 0x4a) = 0x400ccccd;
            F<uint32_t>(r, 0x4e) = 0x3dcccccd;
        }
        char text[0x200];
        if (id <= 0x46) {
            FormatText(text, GetString(0xc7), LocalizeName(nm), 0, 0, 0, 0, 0, 0, 0);
            StrCopyN((char*)0x61312c + id * 0x6e, text, 0x46);
            F<uint32_t>((void*)0x613173, id * 0x6e) = id;
            FormatText(text, GetString(0xc8), LocalizeName(nm), 0, 0, 0, 0, 0, 0, 0);
            StrCopyN((char*)0x6150f8 + id * 0x6e, text, 0x46);
            F<uint32_t>((void*)0x61513f, id * 0x6e) = id;
        }
        char* e = (char*)0x5f7f9f + id * 0x81;
        for (int32_t k2 = 0; k2 < 0xc; ++k2, e += 0x1ebd) {
            FormatText(text, GetString(F<int32_t>(e, 0x69)), LocalizeName(nm), 0, 0, 0, 0, 0, 0, 0);
            StrCopyN(e, text, 0x46);
            F<uint32_t>(e, -8) = id;
            F<uint32_t>(e, 0x5c) = 0;
        }
    }
specials:
    F<int32_t>(c, 0x8e) = -1;
    F<int32_t>(c, 0x92) = -1;
    F<int32_t>(c, 0x96) = -1;
    for (int32_t k = 0; k < 0x35; ++k) F<int32_t>(c, 0x9a + k * 4) = -1;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        auto name = [&]() { return (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 4; };
        if (!Stricmp(name(), (const char*)0x5c99f0)) {
            F<uint32_t>(c, 0x92) = Collection_IdAt(c, 0, i, 0);
            F<int32_t>(c, 0x9a + Collection_IdAt(c, 0, i, 0) * 4) = 1;
        }
        if (!Stricmp(name(), (const char*)0x5c99d8)) {
            F<uint32_t>(c, 0x8e) = Collection_IdAt(c, 0, i, 0);
            F<int32_t>(c, 0x9a + Collection_IdAt(c, 0, i, 0) * 4) = 0;
        }
        if (!Stricmp(name(), (const char*)0x5c99d0)) {
            F<uint32_t>(c, 0x96) = Collection_IdAt(c, 0, i, 0);
            F<int32_t>(c, 0x9a + Collection_IdAt(c, 0, i, 0) * 4) = 2;
        }
    }
    CargoTypes_MarkAvailable(c);
}
RT3_REPLACE(0x41ede0, CargoTypes_Load);

void __fastcall CargoTypes_Recount(Collection* t);

// 0x41f4e0. Cargo type table constructor: 0xd0-byte records (10, +5), loaded from
// the cargo files, then counted.
Collection* __fastcall CargoTypes_Construct(Collection* c) {
    Collection_SetVtbl(c);
    *(uint32_t*)c = 0x5c997c;
    F<uint16_t>(c, 0x88) = 0;
    F<uint16_t>(c, 0x8a) = 0;
    F<uint16_t>(c, 0x8c) = 0;
    Collection_Init(c, 0, 1, 0xd0, 10, 5, 1, 0, 0);
    CargoTypes_Load(c);
    CargoTypes_Recount(c);
    return c;
}
RT3_REPLACE(0x41f4e0, CargoTypes_Construct);

// 0x41f530. Releases every cargo type and reloads them from the files.
void __fastcall CargoTypes_Reload(Collection* c) {
    while (Collection_Count(c)) c->vtbl->release(c, Collection_IdAt(c, 0, 0, 0));
    F<uint16_t>(c, 0x88) = 0;
    F<uint16_t>(c, 0x8a) = 0;
    F<uint16_t>(c, 0x8c) = 0;
    CargoTypes_Load(c);
    CargoTypes_Recount(c);
}
RT3_REPLACE(0x41f530, CargoTypes_Reload);

// 0x42b4a0. Saves a region's cargo economy compactly: a 3-bit-per-cargo mask of
// which of supply (+0 u16), demand (+0x6a u16) and price (+0x103 float, != 0) are set
// for available cargos (0x62ba8c +0x16e), then those values, then the trailer
// (+0xe6, +0xd4, +0xd6, +0xd8..), the entry count (+0xeb, as u16) and its 6-byte
// entries (+0xef).
void __fastcall Region_SaveCargo(uint8_t* r, int, Stream st) {
    uint8_t mask[0x14] = {};
    uint8_t* cargos = hook::Global<uint8_t*>(0x62ba8c);
    for (int32_t k = 0; k < 0x35; ++k) {
        int32_t avail = F<int32_t>(cargos, 0x16e + k * 4);
        if (F<uint16_t>(r, k * 2) && avail) mask[(k * 3) >> 3] |= (uint8_t)(1 << ((k * 3) & 7));
        if (F<uint16_t>(r, 0x6a + k * 2) && avail) mask[(k * 3 + 1) >> 3] |= (uint8_t)(1 << ((k * 3 + 1) & 7));
        if (!(F<float>(r, 0x103 + k * 4) == 0.0f) && avail) mask[(k * 3 + 2) >> 3] |= (uint8_t)(1 << ((k * 3 + 2) & 7));
    }
    StreamWrite(st, mask, 0x14, 0);
    for (int32_t k = 0; k < 0x35; ++k) {
        if (!F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x16e + k * 4)) continue;
        if (F<uint16_t>(r, k * 2)) StreamWrite(st, r + k * 2, 2, 0);
        if (F<uint16_t>(r, 0x6a + k * 2)) StreamWrite(st, r + 0x6a + k * 2, 2, 0);
        if (!(F<float>(r, 0x103 + k * 4) == 0.0f)) StreamWrite(st, r + 0x103 + k * 4, 4, 0);
    }
    StreamWrite(st, r + 0xe6, 1, 0);
    StreamWrite(st, r + 0xd4, 2, 0);
    StreamWrite(st, r + 0xd6, 2, 0);
    StreamWrite(st, r + 0xd8, 7, 0);
    int32_t n = F<uint16_t>(r, 0xeb);
    StreamWrite(st, &n, 2, 0);
    for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) StreamWrite(st, F<uint8_t*>(r, 0xef) + i * 6, 6, 0);
}
RT3_REPLACE(0x42b4a0, Region_SaveCargo);

void __fastcall Cargo_MoveTo66(void* c, int, float a);
int32_t __fastcall City_HasCargoSurplus(uint8_t* o, int, int32_t i);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);

// 0x42ce00. Monthly cargo economy of a region (free cargos only): the price
// (+0x103) decays with the cargo's decay (+0x1c) faster as the map ages (moved to
// the cargo's +0x66 pool); with no demand (+0x6a) supply (+0) drifts by the price
// level; otherwise supply follows the price-to-demand ratio (a piecewise curve,
// boosted for local producers +0xd8 bits, shaped by how saturated the market is),
// capped between the cargo's min / max (+0x4e / +0x4a) x base price (+0x18 x
// difficulty table 0x5f33a4), and demand slowly decays (randomly when small).
// Changes are gentler when the economy is frozen (+0x46c38).
void __fastcall Region_UpdateCargoEconomy(uint8_t* r) {
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    int32_t frozen = F<int32_t>(world, 0x46c38);
    int32_t n = Collection_Count(hook::Global<Collection*>(0x62ba8c));
    world = hook::Global<uint8_t*>(0x6cec78);
    float base = *(const float*)(0x5f33a4 + F<int32_t>(world, 0x2d) * 4);
    int32_t age = F<uint16_t>(world, 0xd) - F<uint16_t>(world, 0x5);
    float decay;
    if (age > 0x14) age = 0x14;
    if (age >= 5) decay = (float)((double)(age - 5) * D(0x5c8918) + D(0x5c88c8));
    else decay = 0.3f;
    int32_t v103 = Game_EffectiveVersion() >= 0x67;
    for (int32_t k = 1; k <= n; ++k) {
        uint8_t* cargos = hook::Global<uint8_t*>(0x62ba8c);
        if (!F<int32_t>(cargos, 0x16e + k * 4) || F<int32_t>(cargos, 0x9a + k * 4) != -1) continue;
        float& price = F<float>(r, 0x103 + k * 4);
        uint16_t& supply = F<uint16_t>(r, k * 2);
        uint16_t& demand = F<uint16_t>(r, 0x6a + k * 2);
        uint8_t* rec = nullptr;
        if (!(price == 0.0f)) {
            rec = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, k);
            float old = price;
            if (price > D(0x5c9cc8)) {
                auto step = [&](float v) {
                    return (float)((D(0x5c8770) - (double)decay * F<float>(rec, 0x1c) / Fl(0x5c8968)) * v);
                };
                float p1 = step(price);
                price = p1;
                if (p1 > Fl(0x5c8900)) {
                    float p2 = step(p1);
                    price = p2;
                    if (p2 > Fl(0x5c89a4)) price = step(p2);
                }
                Cargo_MoveTo66(rec, 0, (float)((double)old - price));
            } else {
                price = 0.0f;
                Cargo_MoveTo66(rec, 0, old);
            }
        }
        if (demand == 0) {
            double q = price < D(0x5c8d68) ? (double)price : 5.0;
            double f;
            if (F<uint16_t>(r, 0xd4) && City_HasCargoSurplus(r, 0, k)) f = D(0x5c9cc0) - q / D(0x5c8d68) * D(0x5c8668);
            else if (r[0xdf + (k >> 3)] & (1 << (k & 7))) f = D(0x5c9cb8) - q / D(0x5c8d68) * D(0x5c8c20);
            else f = D(0x5c9b60) - q / D(0x5c8d68) * D(0x5c88c8);
            float g = (float)f;
            if (frozen) supply = (uint16_t)Ftol(((double)g * D(0x5c85c8) / D(0x5c9ab8) + D(0x5c8770)) * (double)supply);
            else supply = (uint16_t)Ftol(((double)g / Fl(0x5c8968) + D(0x5c8770)) * (double)supply);
            continue;
        }
        if (!rec) rec = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, k);
        float d = (float)((double)demand / D(0x5c9c40));
        float ratio = (float)((double)price / d);
        float g;
        bool positive;
        if (ratio < D(0x5c85b8)) {
            g = 0.25f;
            positive = true;
        } else {
            if (ratio < D(0x5c8818)) g = (float)((D(0x5c8818) - ratio) / D(0x5c8818) * D(0x5c9cb0) + D(0x5c8658));
            else if (ratio < D(0x5c9ca8)) g = (float)((D(0x5c9ca8) - ratio) / D(0x5c8818) * D(0x5c8658));
            else if (ratio < D(0x5c9ca0)) g = (float)((D(0x5c9ca0) - ratio) / D(0x5c8818) * D(0x5c9c98));
            else if (ratio < D(0x5c9c90)) g = (float)(D(0x5c9c98) - (D(0x5c9c90) - ratio) / D(0x5c9ca8) * D(0x5c8590));
            else if (ratio < D(0x5c9c88)) g = (float)(D(0x5c9c78) - (D(0x5c9c88) - ratio) / D(0x5c9ca0) * D(0x5c9c80));
            else {
                F<uint32_t>(&g, 0) = 0xbecccccd;
                positive = false;
                goto shaped;
            }
            positive = g > Fl(0x5c8598);
        }
        if (positive && (r[0xd8 + (k >> 3)] & (1 << (k & 7)))) {
            g = (float)(g * D(0x5c88f8));
            if (!(d < D(0x5c87d8))) g = (float)(g * D(0x5c8570));
        }
    shaped:
        {
            int32_t s16 = supply << 4;
            float pbase = (float)((double)base * F<float>(rec, 0x18));
            double m = Effects_CargoMultiplier(k);
            float w = (float)Ftol(m * (double)(float)s16);
            w = (float)((double)w / pbase);
            if (w < D(0x5c8770) && g > Fl(0x5c8598)) g = (float)(((D(0x5c8770) - w) * D(0x5c8578) + D(0x5c8770)) * g);
            if (w > D(0x5c8770) && g < Fl(0x5c8598)) g = (float)(((double)w + D(0x5c8628)) / D(0x5c8578) * g);
            if (g > Fl(0x5c8598) && ratio > D(0x5c8678) && v103) g = (float)(g * D(0x5c8640));
            double sd = (double)s16;
            int32_t ns = Ftol(((double)g / Fl(0x5c8968) + D(0x5c8770)) * sd);
            if (frozen) ns = Ftol(((double)g / D(0x5c8970) + D(0x5c8770)) * sd);
            float nf = (float)ns;
            float lo = (float)((double)pbase * F<float>(rec, 0x4e));
            if (nf < lo) ns = Ftol(lo);
            else {
                float hi = (float)((double)pbase * F<float>(rec, 0x4a));
                if (nf > hi) {
                    int32_t c = Ftol(hi);
                    if (v103) {
                        int32_t lim = ratio < D(0x5c8678) ? s16 + 1000 : s16 - 3000;
                        int32_t a = c > lim ? c : lim;
                        c = a < c + 10000 ? a : c + 10000;
                    }
                    if (ns >= c) ns = c;
                }
            }
            int32_t v = ns >> 4;
            if (v >= 0xfde8) v = 0xfde8;
            supply = (uint16_t)v;
        }
        if (demand > 0x30) demand = (uint16_t)Ftol((double)(float)demand * Fl(0x5c9c70));
        else {
            uint16_t dm = demand;
            if (SimRand(0, 0x18) <= (int32_t)(dm >> 1)) demand = dm > 2 ? (uint16_t)(dm - 2) : 0;
        }
    }
}
RT3_REPLACE(0x42ce00, Region_UpdateCargoEconomy);

} // namespace rt3
