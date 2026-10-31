// Larger game functions, part 1 (batch 199). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

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

int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
double __fastcall Obj_BuildingCellX(void* o);
double __fastcall Obj_BuildingCellY(void* o);
int32_t __fastcall Game_EffectiveVersion();
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

// 0x408f70. Expected rate for company c (id c[0]) from its active stations' records
// (+0x5bd[id], +0x12): the average over a year-dependent divisor x 0.25, stretched by
// the distance between the first two stations when it has exactly two (v1.03+),
// scaled by era and station count, then clamped to [avg / 2, avg] (2 when <= the
// minimum).
float __fastcall Company_StationRate(int32_t* c) {
    Collection* stations = hook::Global<Collection*>(0x6cec20);
    float sum = 0.0f;
    int32_t count = 0;
    uint8_t* first = nullptr;
    uint8_t* second = nullptr;
    for (int32_t i = 0; i < Collection_Count(stations); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, i, 0));
        int32_t id = c[0];
        uint8_t* rec = F<uint8_t**>(s, 0x5bd)[id];
        if (!rec[0] || !rec[1] || Obj_BuildingValue(s) != id || !Obj_BuildingStateActive(s)) continue;
        if (!first) first = s;
        else if (!second) second = s;
        sum = (float)((double)sum + F<float>(F<uint8_t**>(s, 0x5bd)[c[0]], 0x12));
        ++count;
    }
    int32_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    int32_t div = year < 0x749 ? 4 : year < 0x758 ? 5 : year < 0x76c ? 6 : year < 0x780 ? 5 : year < 0x78f ? 6
                : year < 0x79e ? 7 : year < 0x7b2 ? 6 : (year >= 0x7c6) + 7;
    float avg = (float)((double)sum / (float)div);
    float rate = (float)(avg * D(0x5c8680));
    if (Game_EffectiveVersion() >= 0x67 && count == 2) {
        int32_t y2 = Ftol(Obj_BuildingCellY(second));
        int32_t x2 = Ftol(Obj_BuildingCellX(second));
        int32_t y1 = Ftol(Obj_BuildingCellY(first));
        int32_t x1 = Ftol(Obj_BuildingCellX(first));
        float dist = (float)(int32_t)(uint16_t)DistanceInt(x1, y1, x2, y2);
        uint16_t y = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
        float k = 0.85f;
        if (y >= 0x79e) k = 1.55f;
        else if (y >= 0x753) k = 1.15f;
        if (dist > Fl(0x5c8940)) rate = (float)(((double)dist / D(0x5c8938) + k) / ((double)k + D(0x5c8770)) * rate);
    }
    year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    uint32_t era = year < 0x73a ? 0x5c88f0 : year < 0x74e ? 0x5c8660 : year < 0x762 ? 0x5c8878 : year < 0x776 ? 0x5c85a8
                 : year < 0x78a ? 0x5c8930 : year < 0x79e ? 0x5c8928 : year < 0x7b2 ? 0x5c85a0 : 0x5c8920;
    rate = (float)(rate * D(era));
    if (count <= 4) rate = (float)(rate * D(0x5c8888));
    else if (count > 8) rate = (float)(rate * D(count <= 12 ? 0x5c85a8 : count <= 20 ? 0x5c8698 : 0x5c8660));
    if (!(rate < avg)) rate = avg;
    double half = avg * D(0x5c8628);
    float r = (float)(rate > half ? (double)rate : half);
    if (!(r > Fl(0x5c878c))) r = 2.0f;
    return r;
}
RT3_REPLACE(0x408f70, Company_StationRate);

void __fastcall Station_PickUp(uint8_t* o, int, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* outCount,
                               float* outValue, float* outAmount);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void* __fastcall Obj_BuildingKind(void* o);
void* __fastcall Train_Company(void* t);
double __fastcall Company_QualityRaw(void* c);

// 0x41bb40. Loads a not-yet-full car (+0x38c cargo, +0x390 train, fill +0x398) at
// station `at` bound for `to`: picks up cargo for its free room, raising the fill
// (max 1) and averaging the load age (+0x3a0); for cargo without a city quota the
// unmet demand at `to` is booked (+0x3a4). The car's waiting time (+0x39c) is then
// re-rolled (1..20 s, in some game modes) and grows by the unmet amount x both
// stations' kind factors x the company's quality blend for the difficulty.
void __fastcall Car_LoadAt(uint8_t* car, int, uint8_t* at, uint8_t* to) {
    if (!(F<float>(car, 0x398) < D(0x5c9808)) && !(F<float>(car, 0x398) != F<float>(car, 0x398))) return;
    if (F<float>(car, 0x39c) < Fl(0x5c8598)) F<float>(car, 0x39c) = 0.0f;
    int32_t cargo = F<int32_t>(car, 0x38c);
    float fill = F<float>(car, 0x398);
    uint8_t* train = F<int32_t>(car, 0x390) != -1
                         ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(car, 0x390))
                         : nullptr;
    float room = (float)(D(0x5c8770) - fill);
    float got, value, amount;
    Station_PickUp(at, 0, train, to, cargo, room, &got, &value, &amount);
    float unmet = 0.0f;
    if (got != 0.0f || got != got) {
        float t = (float)((double)got + F<float>(car, 0x398));
        float nf = Fl(0x5c85b4) < t ? 1.0f : t;
        auto denom = [&]() { return nf < D(0x5c8678) ? 0.001 : (double)nf; };
        if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + F<int32_t>(car, 0x38c) * 4) != -1) {
            double dv = denom();
            unmet = amount;
            F<float>(car, 0x3a0) = (float)(((double)F<float>(car, 0x3a0) * F<float>(car, 0x398) + got) / dv);
        } else {
            float quota = (float)City_CargoQuota(Obj_BuildingRegion(to), 0, F<int32_t>(car, 0x38c));
            float w = (float)((double)quota * got - value);
            float u = w < Fl(0x5c8598) ? 0.0f : w;
            double dv = denom();
            unmet = u;
            F<float>(car, 0x3a0) = (float)(((double)F<float>(car, 0x3a0) * F<float>(car, 0x398) + got) / dv);
            F<float>(car, 0x3a4) = (float)((double)value + F<float>(car, 0x3a4));
        }
        F<float>(car, 0x398) = nf;
    }
    if (F<float>(car, 0x39c) < Fl(0x5c8598) || F<float>(car, 0x39c) == Fl(0x5c8598)) {
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x130) || hook::Global<uint8_t*>(0x6cec7c)[0x82] ||
            F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef))
            F<float>(car, 0x39c) = (float)(HashRand(F<int32_t>(car, 0x34a), 4, 0x14) * 1000);
    }
    uint8_t* ka = at ? (uint8_t*)Obj_BuildingKind(at) : nullptr;
    uint8_t* kb = to ? (uint8_t*)Obj_BuildingKind(to) : nullptr;
    float m = 1.0f;
    if (ka) m = (float)(((double)F<float>(ka, 0x31b) + D(0x5c8770)) / D(0x5c85d0));
    if (kb) m = (float)(((double)F<float>(kb, 0x31b) + D(0x5c8770)) / D(0x5c85d0) * m);
    float x = (float)((double)m * unmet);
    train = F<int32_t>(car, 0x390) != -1
                ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(car, 0x390))
                : nullptr;
    float q = (float)Company_QualityRaw(Train_Company(train));
    uint32_t idx = hook::Global<uint8_t*>(0x6cec7c)[0x7d] * 4;
    uint32_t ta = 0x5f3cb8, tb = 0x5f3cc8;
    if (Game_EffectiveVersion() >= 0x67) {
        ta = 0x5f3c98;
        tb = 0x5f3ca8;
    }
    float blend = (float)(Fl(ta + idx) * (D(0x5c8770) - q) + (double)q * Fl(tb + idx));
    x = (float)((double)blend * x);
    F<float>(car, 0x39c) = (float)((double)x + F<float>(car, 0x39c));
}
RT3_REPLACE(0x41bb40, Car_LoadAt);

void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);

// Writes the 100-byte name at `off` of record `idx` in table `table`.
static void WriteName(Stream st, uint32_t table, int32_t idx, uint32_t off) {
    char buf[0x65]; // StrCopyN also writes the terminator at [0x64]
    uint8_t* rec = (uint8_t*)Collection_Get(hook::Global<Collection*>(table), 0, idx);
    StrCopyN(buf, (const char*)rec + off, 0x64);
    StreamWrite(st, buf, 0x64, 0);
}

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

} // namespace rt3
