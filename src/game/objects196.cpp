// Larger game functions, part 4 (batch 202). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
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

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);

// A city's position in 4x4 blocks.
static inline float BlockOf(double cell) { return (float)((cell - Fl(0x5c878c)) / D(0x5c87b8)); }

// 0x421730. Assigns the map's 4x4 blocks (+0x212d, u16 city at +1) to cities: each
// city (not flagged +0x23e) claims the blocks within its radius (+0x256, in
// blocks) that are unclaimed or closer to it than to their owner. Then every land
// block counts towards its city's block count (+0x242) and extents (+0x246..+0x252;
// unclaimed blocks go to their region byte's city). Leaves the block height in EAX
// (a caller passes it on).
int32_t __fastcall Cities_AssignBlocks(Collection* cities) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    F<int32_t>(m, 0x2201) = 0;
    int32_t W = F<int32_t>(m, 0x214d), H = F<int32_t>(m, 0x2151);
    auto block = [&](int32_t x, int32_t y) {
        uint8_t* mm = hook::Global<uint8_t*>(0x62c120);
        return F<uint8_t*>(mm, 0x212d) + (F<int32_t>(mm, 0x214d) * y + x) * 4;
    };
    for (int32_t y = 0; y < H; ++y)
        for (int32_t x = 0; x < W; ++x) F<uint16_t>(block(x, y), 1) = 0;
    for (int32_t i = 0; i < Collection_Count(cities); ++i) {
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
        F<int32_t>(c, 0x246) = 9999;
        F<int32_t>(c, 0x24a) = 9999;
        F<int32_t>(c, 0x24e) = 0;
        F<int32_t>(c, 0x252) = 0;
        F<int32_t>(c, 0x242) = 0;
    }
    for (int32_t i = 0; i < Collection_Count(cities); ++i) {
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
        if (F<int32_t>(c, 0x23e)) continue;
        float r = (float)(((double)F<int32_t>(c, 0x256) + D(0x5c87d8)) / D(0x5c87b8));
        float r2 = (float)((double)r * r);
        float bx = BlockOf(Obj_CellX(c)), by = BlockOf(Obj_CellY(c));
        int32_t x0 = Ftol((double)bx - r), y0 = Ftol((double)by - r);
        int32_t x1 = Ftol((double)bx + r + Fl(0x5c85b4)), y1 = Ftol((double)by + r + Fl(0x5c85b4));
        if (x0 < 0) x0 = 0;
        if (y0 < 0) y0 = 0;
        if (x1 >= W) x1 = W - 1;
        if (y1 >= H) y1 = H - 1;
        for (int32_t y = y0; y <= y1; ++y) {
            float fy = (float)y;
            for (int32_t x = x0; x <= x1; ++x) {
                float fx = (float)x;
                float d = (float)DistanceSq2D(bx, by, fx, fy);
                if (!(d < r2)) continue;
                uint16_t& owner = F<uint16_t>(block(x, y), 1);
                if (!owner) {
                    owner = F<uint16_t>(c, 0x23a);
                    continue;
                }
                uint8_t* o = (uint8_t*)Collection_Get(cities, 0, owner);
                float ox = BlockOf(Obj_CellX(o)), oy = BlockOf(Obj_CellY(o));
                float d2 = (float)DistanceSq2D(ox, oy, fx, fy);
                if (d < d2) F<uint16_t>(block(x, y), 1) = F<uint16_t>(c, 0x23a);
            }
        }
    }
    for (int32_t y = 0; y < H; ++y) {
        for (int32_t x = 0; x < W; ++x) {
            if (Terrain_KindIs1Or4(hook::Global<uint8_t*>(0x62c120), 0, 2 + x * 4, 2 + y * 4)) continue;
            uint8_t* b = block(x, y);
            uint32_t id = F<uint16_t>(b, 1);
            if (!id) id = b[0];
            uint8_t* c = (uint8_t*)Collection_Get(cities, 0, id);
            ++F<int32_t>(c, 0x242);
            if (x < F<int32_t>(c, 0x246)) F<int32_t>(c, 0x246) = x;
            if (x > F<int32_t>(c, 0x24e)) F<int32_t>(c, 0x24e) = x;
            if (y < F<int32_t>(c, 0x24a)) F<int32_t>(c, 0x24a) = y;
            if (y > F<int32_t>(c, 0x252)) F<int32_t>(c, 0x252) = y;
        }
    }
    return H;
}
RT3_REPLACE(0x421730, Cities_AssignBlocks);

void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode);
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t);

static inline void Rd(Stream st, void* p, uint32_t n) { StreamRead(st, p, n, 0, 1); }

// Reads a saved 100-byte name and returns the index the lookup gives for it.
template <typename Fn> static int32_t ReadIndex(Stream st, char* buf, Fn lookup) {
    Rd(st, buf, 0x64);
    return (int32_t)lookup(buf);
}

// 0x42db20. Loads the scenario's event rules saved by EventRules_Save (version
// 0x526d+ effects / 0x526e+ triggers carry all fields; 0x526f+ texts tag-converted):
// re-links the trigger and four effect lists, re-numbering cargo / building /
// industry references by their saved names.
void __fastcall EventRules_Load(uint8_t* o, int, Stream st) {
    int32_t version;
    Rd(st, &version, 4);
    if (version >= 0x526f) {
        Text_ConvertTags((char*)o + 0xe0, 0x320, 0);
        static const uint32_t texts[] = { 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
        for (uint32_t t : texts) Text_ConvertTags((char*)o + t, 0xc8, 0);
    }
    char name[0x68] = {};
    int32_t triggers;
    Rd(st, &triggers, 4);
    auto alloc = [](size_t n) { return (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(n); };
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    uint8_t* prev = nullptr;
    for (int32_t i = 0; i < triggers; ++i) {
        uint8_t* e = alloc(0x1e);
        Rd(st, e, 4);
        Rd(st, e + 4, 1);
        Rd(st, e + 5, 4);
        Rd(st, e + 9, 4);
        Rd(st, e + 0xd, 1);
        if (version >= 0x526e) {
            Rd(st, e + 0xe, 4);
            Rd(st, e + 0x12, 4);
        }
        int32_t kind = F<int32_t>(e, 0);
        if (kind >= 0x83 && kind <= 0x35e) {
            int32_t base = kind - (kind - 0x83) % 0x3d;
            int32_t idx = ReadIndex(st, name, [&](const char* n) { return Table_FindByName(cargos, 0, n); });
            if (idx > 0 && idx <= 0x46) F<int32_t>(e, 0) = idx + base;
        }
        F<uint8_t*>(e, 0x1a) = nullptr;
        if (i == 0) F<uint8_t*>(o, 0) = e;
        else F<uint8_t*>(prev, 0x1a) = e;
        prev = e;
    }
    StreamSkip4(st, 0x4eb8, 0, 0);
    int32_t counts[4];
    for (int32_t k = 0; k < 4; ++k) {
        Rd(st, &counts[k], 4);
        for (int32_t i = 0; i < counts[k]; ++i) {
            uint8_t* e = alloc(0x28);
            Rd(st, e, 4);
            Rd(st, e + 4, 4);
            Rd(st, e + 8, 1);
            Rd(st, e + 9, 4);
            Rd(st, e + 0xd, 4);
            if (version >= 0x526d) {
                Rd(st, e + 0x11, 4);
                Rd(st, e + 0x12, 4);
                Rd(st, e + 0x14, 4);
                Rd(st, e + 0x16, 4);
                Rd(st, e + 0x18, 0xc);
            }
            int32_t kind = F<int32_t>(e, 0), base = -1;
            if (kind >= 0xfb && kind <= 0x15f) base = 0xfb;
            else if (kind >= 0x160 && kind <= 0x1c4) base = 0x160;
            if (base != -1) {
                int32_t idx = ReadIndex(st, name, [&](const char* n) {
                    return Table_FindByAltName(hook::Global<Collection*>(0x6ada84), 0, n);
                });
                if (idx > 0 && idx <= 0x64) F<int32_t>(e, 0) = idx + base;
            }
            kind = F<int32_t>(e, 0);
            base = -1;
            if (kind >= 0x6a && kind <= 0xb0) base = 0x6a;
            else if (kind >= 0xb4 && kind <= 0xfa) base = 0xb4;
            if (base != -1) {
                int32_t idx = ReadIndex(st, name, [&](const char* n) { return Table_FindByName(cargos, 0, n); });
                if (idx > 0 && idx <= 0x46) F<int32_t>(e, 0) = idx + base;
            }
            kind = F<int32_t>(e, 0);
            if (kind >= 0x1f7 && kind <= 0x265) {
                int32_t idx = ReadIndex(st, name, [&](const char* n) {
                    return Table_FindByName2(hook::Global<Collection*>(0x62b268), 0, n, 0);
                });
                if (idx > 0 && idx <= 0x6e) F<int32_t>(e, 0) = idx + 0x1f7;
            }
            F<uint8_t*>(e, 0x24) = nullptr;
            if (i == 0) F<uint8_t*>(o, 4 + k * 4) = e;
            else F<uint8_t*>(prev, 0x24) = e;
            prev = e;
        }
    }
    StreamSkip4(st, 0x4eb9, 0, 0);
}
RT3_REPLACE(0x42db20, EventRules_Load);

uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id);
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
int32_t __fastcall Obj_YearReached(void* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);

static uint8_t* StationRegion(uint8_t* st) {
    return Obj_Region(F<uint32_t>(st, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(st, 4)) : nullptr);
}

// 0x47eb90. A train from station `from` unloaded at this station: for each car's
// cargo class (consist +0x28 & 0x40: per-car list +1, else one class +0x24) the
// cargo this station could best have supplied (stock beyond what is already
// counted, demand at `from`'s region minus here, weighted by availability) is
// counted, and the cargos more wanted there than here (by more than that) are
// noted with their demand. The station's per-cargo trip counts (+0x3e), dates
// (+0x1e6) and demand averages (+0x112; a higher new demand gets weight x 0x5c8900)
// are then updated.
void __fastcall Station_LearnDemand(uint8_t* st, int, uint32_t from, uint8_t* consist) {
    if (from == F<uint32_t>(st, 0)) return;
    uint8_t* here = StationRegion(st);
    uint8_t* there = StationRegion((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, from));
    float demand[0x35] = {}, wanted[0x35] = {}, counted[0x35] = {};
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    int32_t n = consist[0x28] & 0x40 ? consist[0] : consist[0x23];
    float best = 0.0f;
    for (int32_t car = 0; car < n; ++car) {
        int32_t tag = consist[0x28] & 0x40 ? F<int32_t>(consist, 1 + car * 4) : F<int32_t>(consist, 0x24);
        best = 0.0f;
        int32_t pick = -1;
        for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
            int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
            if (F<int32_t>(cargos, 0x9a + id * 4) != -1) continue;
            if (!Cargo_MatchesClass(Collection_Get(cargos, 0, id), 0, tag)) continue;
            float stock, quota;
            if (!Obj_CargoStockAndQuota(st, 0, id, &stock, &quota)) continue;
            stock = (float)((double)stock - counted[id]);
            if (stock < D(0x5c8628)) continue;
            float v = (float)City_CargoQuota(there, 0, id);
            v = (float)((double)v - quota);
            v = (float)(v * (stock < D(0x5c8770) ? (double)stock : 1.0));
            if (v > best) {
                best = v;
                pick = id;
            }
        }
        if (pick != -1) counted[pick] = (float)((double)counted[pick] + D(0x5c8770));
        for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
            int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
            if (F<int32_t>(cargos, 0x9a + id * 4) != -1) continue;
            uint8_t* rec = (uint8_t*)Collection_Get(cargos, 0, id);
            if (!Cargo_MatchesClass(rec, 0, tag) || !Obj_YearReached(rec)) continue;
            int32_t qa = City_CargoQuota(there, 0, id);
            float d = (float)(qa - City_CargoQuota(here, 0, id));
            if (!(d > best)) continue;
            wanted[id] = (float)((double)wanted[id] + Fl(0x5c85b4));
            demand[id] = (float)City_CargoQuota(there, 0, id);
        }
    }
    for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
        int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
        if (!(counted[j] > Fl(0x5c8598)) && counted[j] == counted[j] && !(wanted[j] > Fl(0x5c8598)) && wanted[j] == wanted[j])
            continue;
        float trips = (float)((double)wanted[id] + F<float>(st, 0x3e + id * 4));
        F<int32_t>(st, 0x1e6 + id * 4) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
        float w = wanted[id];
        F<float>(st, 0x3e + id * 4) = trips;
        if (demand[id] > F<float>(st, 0x112 + id * 4)) w = (float)(w * Fl(0x5c8900));
        float sum = (float)((double)trips + w);
        double den = sum < D(0x5c86b8) ? 0.1 : (double)sum;
        float& avg = F<float>(st, 0x112 + id * 4);
        avg = (float)(((double)trips * avg + (double)w * demand[id]) / den);
    }
}
RT3_REPLACE(0x47eb90, Station_LearnDemand);

void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);

// 0x47f910. Picks the cargo of class `tag` this station should load for station
// `dest`: city cargos (with a quota slot) by their simulated pickup value, others by
// the destination's demand less what is already promised, x availability (scenario
// mode +0x4af7: a flat 10000, 20000 for Diesel / Weapons / Ammunition). In
// randomised modes (+0x130 / +0x82 / sandbox) a value may be swapped for a 1..100
// x k roll. Returns the best above the floor (1500, 0 in those modes) or -1; *lost
// tells whether any candidate lost, *fallback gets a city cargo with nothing waiting
// (preferring 0x62ba8c +0x88/+0x8a).
int32_t __fastcall Station_PickCargo(uint8_t* st, int, int32_t tag, uint32_t destId, int32_t* lost, int32_t* fallback) {
    uint8_t* dest = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, destId);
    uint8_t* region = Obj_Region(F<uint32_t>(dest, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(dest, 4))
                                                      : nullptr);
    auto game = []() { return hook::Global<uint8_t*>(0x6cec74); };
    auto opts = []() { return hook::Global<uint8_t*>(0x6cec7c); };
    auto world = []() { return hook::Global<uint8_t*>(0x6cec78); };
    auto sandbox = [&]() { return F<int32_t>(world(), 0x4aef) && !F<int32_t>(world(), 0x4af7); };
    float floor = 1500.0f;
    if (F<int32_t>(game(), 0x130) || opts()[0x82] || sandbox()) floor = 0.0f;
    if (fallback) *fallback = 0;
    int32_t best = -1, rolled = 0, anyLost = 0;
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
        int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
        if (!Cargo_MatchesClass(Collection_Get(cargos, 0, id), 0, tag)) continue;
        float value = 0.0f;
        bool randomMode = F<int32_t>(game(), 0x130) || opts()[0x82] || sandbox();
        if (F<int32_t>(cargos, 0x9a + id * 4) != -1) {
            if (st[0x5c9] || F<int32_t>(world(), 0x4afb)) continue;
            float count, flagged;
            Train_GatherLoads(st, 0, 1, nullptr, dest, id, 1.0f, &count, &value, &flagged);
            if (count < D(0x5c8628) && !randomMode) {
                if (flagged < D(0x5c8628) || flagged != flagged || !fallback) continue;
                uint32_t c88 = F<uint16_t>(cargos, 0x88), c8a = F<uint16_t>(cargos, 0x8a);
                if ((uint32_t)*fallback == c88) continue;
                if ((uint32_t)*fallback == c8a && (uint32_t)id != c88) continue;
                *fallback = id;
                continue;
            }
        } else {
            float stock, quota;
            if (!Obj_CargoStockAndQuota(st, 0, id, &stock, &quota)) continue;
            if (stock < D(0x5c8628) && !randomMode) continue;
            value = (float)City_CargoQuota(region, 0, id);
            value = (float)((double)value - quota);
            if (F<int32_t>(world(), 0x4af7)) {
                if (st[0x5c9]) value = 0.0f;
                else {
                    value = 10000.0f;
                    if (F<int32_t>(world(), 0x4afb)) {
                        const char* name = (const char*)Collection_Get(cargos, 0, id) + 4;
                        if (!Stricmp(name, (const char*)0x5c99b8) || !Stricmp(name, (const char*)0x5c99b0) ||
                            !Stricmp(name, (const char*)0x5c99a4))
                            value = 20000.0f;
                    }
                }
            }
            value = (float)(value * (stock < D(0x5c8770) ? (double)stock : 1.0));
        }
        int32_t roll = 0;
        if (opts()[0x82] || sandbox() || F<int32_t>(game(), 0x130)) {
            if (best == -1 || rolled) {
                double v = value;
                double r = (double)SimRand(1, 100) * D(0x5c8678);
                if (!(r < v)) r = (double)SimRand(1, 100) * D(0x5c8678);
                else r = value;
                value = (float)r;
                roll = 1;
            }
        }
        if (value > floor) {
            rolled = roll;
            floor = value;
            best = id;
        } else anyLost = 1;
    }
    if (lost) *lost = anyLost;
    return best;
}
RT3_REPLACE(0x47f910, Station_PickCargo);

} // namespace rt3
