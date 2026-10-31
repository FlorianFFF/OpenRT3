// Train cargo pickup / station industry catchment / command line switches / map size view distance in block 0x47..0x48 (batch 169). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Station_LoadShare(uint8_t* o, int, uint8_t* from, uint8_t* to, uint8_t* city, float* out, int32_t apply);
void __fastcall Route_ChargeRandomBuilding(uint8_t* o, int, uint8_t* r, int32_t kind);
void __fastcall Load_AttachToTrain(uint32_t* load, int, uint32_t id);
int32_t __fastcall Obj_BuildingStateActive(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
float __fastcall Grid_Roughness(uint8_t* g);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline double Bits(uint64_t u) { return *(double*)&u; }
static inline double Sqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }
static inline void* Memmove(void* d, const void* s, size_t n) { return ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(d, s, n); }
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

// 0x47e690. Gathers the cargo lots of `cargo` waiting at the station's buildings
// (+0x466 list, lot chains from building +0x272) that the train going to `to` can
// take, best share first, at most min(40, capacity) lots. Reports the number of
// lots / k (outCount), the summed share (outAmount) and the lots flagged 2 / k
// (outFlagged); unless `simulate`, loads them (charging upkeep on fresh lots).
void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged) {
    int32_t room = Ftol(((double)cap + D(0x5c8678)) * D(0x5c8d68));
    *outCount = 0.0f;
    *outAmount = 0.0f;
    if (outFlagged) *outFlagged = 0.0f;
    if (!room) return;
    int32_t maxN = room > 0x28 ? 0x28 : room;
    int32_t cls = F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cargo * 4);
    int32_t n = 0, flagged = 0;
    float vals[0x28];
    uint32_t ids[0x28];
    float v;
    for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint16_t>(F<uint8_t*>(o, 0x466), i * 6));
        for (uint32_t id = F<uint32_t>(b, 0x272); id;) {
            uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada90), 0, id);
            uint32_t next = F<uint32_t>(c, 4);
            if (c[0x10] == cls) {
                if (c[0x12] & 2) ++flagged;
                if (Station_LoadShare(c, 0, o, to, city, &v, 0)) {
                    int32_t pos = 0;
                    while (pos < n && !(v > vals[pos])) ++pos;
                    if (pos < maxN) {
                        int32_t last = maxN - 1 > n ? n : maxN - 1;
                        int32_t cnt = last - pos;
                        if (cnt) {
                            Memmove(&ids[pos + 1], &ids[pos], cnt * 4);
                            Memmove(&vals[pos + 1], &vals[pos], cnt * 4);
                        }
                        vals[pos] = v;
                        ids[pos] = id;
                        if (++n > maxN) n = maxN;
                    }
                }
            }
            id = next;
        }
    }
    for (int32_t j = 0; j < n; ++j) {
        *outAmount = (float)(vals[j] + (double)*outAmount);
        if (simulate) continue;
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada90), 0, ids[j]);
        if (F<uint16_t>(c, 0xa) == F<uint16_t>(c, 0xc)) Route_ChargeRandomBuilding(c, 0, o, simulate);
        Station_LoadShare(c, 0, o, to, city, &v, 1);
        Load_AttachToTrain((uint32_t*)c, 0, F<uint32_t>(city, 0x25));
    }
    *outCount = (float)((double)(float)n / Fl(0x5c8554));
    if (outFlagged) *outFlagged = (float)((double)(float)flagged / Fl(0x5c8554));
}
RT3_REPLACE(0x47e690, Train_GatherLoads);

static inline uint8_t* BuildingOf(uint8_t* o) {
    return F<uint32_t>(o, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr;
}

// 0x47fef0. For an active station collects the industries (kind 4) of the nearby
// region cells within range into the per-type lists (+0x590 counts, +0x5a4 arrays
// of {building id, distance}, +0x5b8 total). Quirk: the cells scanned are offset
// -2..2 minus the station's cell, not plus.
void __fastcall Station_CollectIndustries(uint8_t* o) {
    if (!Obj_BuildingStateActive(o)) return;
    float x = (float)Obj_CellX(BuildingOf(o));
    float y = (float)Obj_CellY(BuildingOf(o));
    for (int32_t dy = -2; dy <= 2; ++dy)
        for (int32_t dx = -2; dx <= 2; ++dx) {
            int32_t cx = dx - Ftol(Obj_CellX(BuildingOf(o)) / Fl(0x5c859c));
            int32_t cy = dy - Ftol(Obj_CellY(BuildingOf(o)) / Fl(0x5c859c));
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (cx < 0 || cx >= F<int32_t>(m, 0x2145) || cy < 0 || cy >= F<int32_t>(m, 0x2149)) continue;
            for (uint32_t id = F<uint16_t>(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * cy + cx) * 0x1d7, 0xd6); id;) {
                uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id);
                uint8_t* ind = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
                if (ind[0x32] == 4) {
                    int32_t kind = (int8_t)ind[0xb9];
                    float py = (float)Obj_CellY(b);
                    float d = (float)Distance2D((float)Obj_CellX(b), py, x, y);
                    if (d <= Fl(0x5c8738)) {
                        ++F<int32_t>(o, 0x5b8);
                        int32_t cnt = ++F<int32_t>(o, 0x590 + kind * 4);
                        uint8_t* arr = (uint8_t*)Malloc(cnt * 8);
                        if (F<int32_t>(o, 0x590 + kind * 4) > 1) {
                            memcpy(arr, F<uint8_t*>(o, 0x5a4 + kind * 4), F<int32_t>(o, 0x590 + kind * 4) * 8 - 8);
                            Free(F<uint8_t*>(o, 0x5a4 + kind * 4));
                        }
                        F<uint32_t>(arr, F<int32_t>(o, 0x590 + kind * 4) * 8 - 8) = id;
                        F<float>(arr, F<int32_t>(o, 0x590 + kind * 4) * 8 - 4) = d;
                        F<uint8_t*>(o, 0x5a4 + kind * 4) = arr;
                    }
                }
                id = F<uint16_t>(b, 0x2a2);
            }
        }
}
RT3_REPLACE(0x47fef0, Station_CollectIndustries);

// Copies a switch argument after the first space or quote (skipping a second) into
// buf, stopping at a '-' or '/' (returns the position there) or the end (nullptr).
static const char* SwitchName(const char* p, char* buf) {
    int32_t n = 0;
    char c = *p;
    if (!c) return nullptr;
    do {
        if (c == '-' || c == '/') return p;
        if ((c == ' ' || c == '"') && !n) {
            char c2 = *++p;
            if (c2 == ' ' || c2 == '"') ++p;
            buf[0] = *p;
            n = 1;
        } else if (n && c != '"') buf[n++] = c;
        c = *++p;
    } while (c);
    return nullptr;
}

// 0x481d00. Parses the command line switches: -S<n> screen mode (1,2,3,4,9 else 1),
// -L logging, -H host, -N "name" player name, -P "name" password, -C name join
// address. Logging (0x6d3ca0) ends up on in any case.
void __fastcall Cmdline_Parse(const char* p) {
    int32_t named = 0, passworded = 0;
    if (*p) {
        for (;;) {
            char c = *p;
            if (c == '-' || c == '/') {
                char sw = p[1];
                ++p;
                switch (sw) {
                case 'S': case 's': {
                    int32_t v = p[1] - '0';
                    hook::Global<int32_t>(0x6cec6c) = v;
                    if (v != 1 && v != 2 && v != 4 && v != 3 && v != 9) {
                        v = 1;
                        hook::Global<int32_t>(0x6cec6c) = 1;
                    }
                    hook::Global<int32_t>(0x6cec70) = v;
                    hook::Global<int32_t>(0x6d4028) = v;
                    break;
                }
                case 'L': case 'l':
                    hook::Global<int32_t>(0x6d3ca0) = 1;
                    break;
                case 'H': case 'h':
                    hook::Global<int32_t>(0x6cd920) = 1;
                    hook::Global<int32_t>(0x6ae3c4) = 0;
                    hook::Global<int32_t>(0x6ae1c0) = 1;
                    if (!named) memset((void*)0x6ae2c4, 0, 0x100);
                    if (!passworded) memset((void*)0x6ae3cc, 0, 0x100);
                    break;
                case 'N': case 'n':
                    memset((void*)0x6ae2c4, 0, 0x100);
                    named = 1;
                    if (!passworded) memset((void*)0x6ae3cc, 0, 0x100);
                    if (!(p = SwitchName(p, (char*)0x6ae2c4))) goto done;
                    break;
                case 'P': case 'p':
                    memset((void*)0x6ae3cc, 0, 0x100);
                    passworded = 1;
                    if (!(p = SwitchName(p, (char*)0x6ae3cc))) goto done;
                    break;
                case 'C': case 'c': {
                    hook::Global<int32_t>(0x6ae3c4) = 0;
                    hook::Global<int32_t>(0x6ae1c0) = 0;
                    hook::Global<int32_t>(0x6cd920) = 1;
                    memset((void*)0x6ae1c4, 0, 0x100);
                    if (!passworded) memset((void*)0x6ae3cc, 0, 0x100);
                    if (!named) memset((void*)0x6ae2c4, 0, 0x100);
                    int32_t n = 0;
                    char ch = *p;
                    if (!ch) goto done;
                    bool stop = false;
                    do {
                        if (ch == ' ') {
                            if (n) {
                                stop = true;
                                break;
                            }
                            ((char*)0x6ae1c4)[0] = *++p;
                            n = 1;
                        } else if (n) ((char*)0x6ae1c4)[n++] = ch;
                        ch = *++p;
                    } while (ch);
                    if (!stop) goto done;
                    break;
                }
                }
            }
            c = *p;
            if (!c) break;
            if (c != '-' && c != '/') ++p;
            if (!*p) break;
        }
    }
done:
    hook::Global<int32_t>(0x6d3ca0) = 1;
}
RT3_REPLACE(0x481d00, Cmdline_Parse);

// One view distance band: when the map is larger than `minArea` and the distance is
// above `top` - roughness, scales it down to area / sqrt(area / div), capped at that.
static inline float Band(float v, float area, float rough, uint32_t minArea, uint32_t top, uint32_t div) {
    if (!(area > Fl(minArea))) return v;
    double lim = D(top) - rough;
    if (!(v > lim)) return v;
    float s = (float)Sqrt((double)area / D(div));
    v = (float)((double)area / s);
    return (float)(v > lim ? lim : (double)v);
}

// 0x484b00. The view distance for detail level +0xb8 (table 0x6219b8), reduced on
// big maps (more so on smooth ones).
float __fastcall Map_ViewDistance(uint8_t* o) {
    float v = ((const float*)0x6219b8)[F<int32_t>(o, 0xb8)];
    float area = 262144.0f, rough = 1.0f;
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    if (m) {
        area = (float)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155));
        rough = Grid_Roughness(m);
    }
    rough = (float)(((double)rough - D(0x5c8770)) * D(0x5c8988));
    if (Fl(0x5c8598) > rough) rough = 0.0f;
    rough = (float)(rough > D(0x5c88c8) ? Bits(0x3fd3333333333333ull) : (double)rough);
    v = Band(v, area, rough, 0x5cef10, 0x5c88f8, 0x5cef08);
    v = Band(v, area, rough, 0x5cef00, 0x5c8818, 0x5ceef8);
    v = Band(v, area, rough, 0x5ceef0, 0x5c87e8, 0x5ceee8);
    return v;
}
RT3_REPLACE(0x484b00, Map_ViewDistance);

} // namespace rt3
