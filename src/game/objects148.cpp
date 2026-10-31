// Building profit score / industry cargo stats / port and warehouse rebuild / site table load / channel restart in blocks 0x40..0x56 (batch 151). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_Remaining(void* o, int, int32_t i);
int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag);
int32_t __fastcall Obj_CountAvailableVehicles(void* o, int, int32_t flag);
void __fastcall Industry_BuildCargoLists(uint8_t* o);
void __fastcall Obj_UpdateEntryFlags(void* o);
void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

// 0x40fcc0. Profit score of the building (0..1, 0.5 = neutral or not applicable):
// its weighted mean yearly result over up to 10 years (the current year weighted by
// the year's progress) against its capacity (type +0x22, + +0x43 when +0x246).
float __fastcall Building_ProfitScore(uint8_t* b) {
    void** vt = *(void***)b;
    if (((int32_t(__thiscall*)(void*))vt[0x70 / 4])(b)) return Fl(0x5c8d48);
    uint8_t* t = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    if (!F<int32_t>(t, 0x3f)) return Fl(0x5c8d48);
    float cap = (float)(double)F<uint32_t>(t, 0x22);
    if (F<int32_t>(b, 0x246)) cap = (float)((double)(float)F<int32_t>(t, 0x43) + cap);
    int32_t years = F<uint16_t>(World(), 0xd) - F<int32_t>(b, 0x3d5) + 1;
    float sumV = 0.0f, sumW = 0.0f;
    for (int32_t i = 0; i < (years > 10 ? 10 : years); ++i) {
        float v = (float)Obj_Remaining(b, 0, i);
        if (i == 0) {
            float m = (float)((double)World()[0xf] / D(0x5c8978));
            float w = (float)((double)m * D(0x5c85f0));
            double d = (double)m > D(0x5c8c20) ? (double)m : 0.2;
            sumV = (float)((double)w * v * (D(0x5c8770) / d) + sumV);
            sumW = (float)((double)w + sumW);
        } else {
            float w = (float)(10 - i);
            sumW = (float)((double)w + sumW);
            sumV = (float)((double)w * v + sumV);
        }
    }
    float denom = sumW > Fl(0x5c85b4) ? sumW : 1.0f;
    float avg = (float)((double)sumV / denom);
    float r = (float)((double)avg / cap * D(0x5c8d40));
    if (r < Fl(0x5c8598)) r = (float)((double)r * D(0x5c8d38));
    r = (float)((double)r * D(0x5c8628) + D(0x5c8628));
    if (r > Fl(0x5c85b4)) return 1.0f;
    if (r < Fl(0x5c8598)) return 0.0f;
    return r;
}
RT3_REPLACE(0x40fcc0, Building_ProfitScore);

// 0x412650. The industry's yearly cargo figures for (cargo, p) - recomputed once a
// year (+0x788) over its recipes (+0x37, +0x33 x 0xbc) for both p: base output
// (+0xe8), output share of the industry size +0x2a (+0x290), inputs (+0x438) and
// inputs per output share (+0x5e0); each array 0x6a floats by cargo*2 + p.
void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare) {
    int32_t year = F<uint16_t>(World(), 0xd);
    auto A = [&](uint32_t idx) -> float& { return F<float>(ind, idx * 4); };
    if (year != F<int32_t>(ind, 0x788)) {
        F<int32_t>(ind, 0x788) = year;
        memset(ind + 0xe8, 0, 0x6a * 4);
        memset(ind + 0x290, 0, 0x6a * 4);
        memset(ind + 0x438, 0, 0x6a * 4);
        memset(ind + 0x5e0, 0, 0x6a * 4);
        for (int32_t q = 0; q < 2; ++q) {
            int32_t count = 0;
            for (int32_t i = 0; i < F<int32_t>(ind, 0x33); ++i) {
                uint8_t* e = F<uint8_t*>(ind, 0x37) + i * 0xbc;
                if (!Vehicle_Available(ind, 0, e, q)) continue;
                if (F<int32_t>(e, 0)) {
                    for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j) {
                        uint8_t* it = e + 0x44 + j * 0x1c;
                        float& a = A(q + F<int32_t>(it, 0) * 2 + 0x10e);
                        a = (float)((double)F<float>(it, 4) + a);
                    }
                } else if (F<int32_t>(e, 0x2c) > 0) {
                    if (!count) {
                        count = Obj_CountAvailableVehicles(ind, 0, q);
                        if (!count) count = 1;
                    }
                    float s = (float)((double)F<float>(ind, 0x2a) / (float)count);
                    float& a = A(q + F<int32_t>(e, 0x1c) * 2 + 0xa4);
                    a = (float)((double)s + a);
                    double rate = (double)F<float>(e, 4) < D(0x5c86b8) ? 0.1 : (double)F<float>(e, 4);
                    s = (float)((double)s / rate);
                    for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j) {
                        uint8_t* it = e + 0x44 + j * 0x1c;
                        float& b = A(q + F<int32_t>(it, 0) * 2 + 0x178);
                        b = (float)((double)s * F<float>(it, 4) + b);
                    }
                } else {
                    float& a = A(q + F<int32_t>(e, 0x1c) * 2 + 0x3a);
                    a = (float)((double)F<float>(e, 4) + a);
                }
            }
        }
    }
    F<int32_t>(ind, 0x788) = F<uint16_t>(World(), 0xd);
    uint32_t k = p + cargo * 2;
    *(uint32_t*)base = F<uint32_t>(ind, (k + 0x3a) * 4);
    *(uint32_t*)share = F<uint32_t>(ind, (k + 0xa4) * 4);
    *(uint32_t*)in = F<uint32_t>(ind, (k + 0x10e) * 4);
    *(uint32_t*)inShare = F<uint32_t>(ind, (k + 0x178) * 4);
}
RT3_REPLACE(0x412650, Industry_CargoStats);

// 0x412d70. Rebuilds the 12 port (pass 0, flag +0xba) and warehouse (pass 1, +0xbb)
// industries from the world's templates (+0x1024.., 0x4e1 bytes each: recipe count,
// recipes, size +0x13d4): reuses the one for template k (+0x794) or creates one,
// cloning the last industry of its kind, and names it Port%02d / Warehouse%02d.
void __fastcall Industries_RebuildPortsWarehouses(Collection* c) {
    if (!World()) return;
    for (int32_t pass = 0; pass < 2; ++pass) {
        int32_t k = 0;
        for (int32_t off = 0; off < 0x3a8c; off += 0x4e1, ++k) {
            int32_t any = -1, exact = -1;
            for (int32_t i = 0; i < Collection_Count(c); ++i) {
                uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
                if (!(pass == 0 ? e[0xba] : pass == 1 && e[0xbb])) continue;
                any = Collection_IdAt(c, 0, i, 0);
                if (F<int32_t>(e, 0x794) == k) exact = Collection_IdAt(c, 0, i, 0);
            }
            int32_t id = exact;
            uint8_t* e;
            if (id == -1) {
                id = ((int32_t(__thiscall*)(Collection*, int32_t, int32_t, int32_t, int32_t))(*(void***)c)[1])(c, 0, 0, 0, 0);
                e = (uint8_t*)Collection_Get(c, 0, id);
                memset(e, 0, 0x1f2 * 4);
            } else {
                e = (uint8_t*)Collection_Get(c, 0, id);
                FreeAndNull((void**)(e + 0x79c));
                FreeAndNull((void**)(e + 0x7a0));
                FreeAndNull((void**)(e + 0x37));
            }
            if (id != any) memcpy(e, Collection_Get(c, 0, any), 0x1f2 * 4);
            F<void*>(e, 0x79c) = nullptr;
            F<void*>(e, 0x7a0) = nullptr;
            F<int32_t>(e, 0x794) = k;
            F<int32_t>(e, 0xe4) = id;
            int32_t n = F<int32_t>(World(), off + 0x1024);
            F<int32_t>(e, 0x33) = n;
            F<uint8_t*>(e, 0x37) = (uint8_t*)Malloc(n * 0xbc + 1);
            for (int32_t j = 0; j < F<int32_t>(e, 0x33); ++j)
                memcpy(F<uint8_t*>(e, 0x37) + j * 0xbc, World() + off + 0x1028 + j * 0xbc, 0xbc);
            F<uint32_t>(e, 0x2a) = F<uint32_t>(World(), off + 0x13d4);
            ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)e + 4, (const char*)(e[0xba] ? 0x5c93e8 : 0x5c93d8), k);
            Industry_BuildCargoLists(e);
            Obj_UpdateEntryFlags(e);
        }
    }
}
RT3_REPLACE(0x412d70, Industries_RebuildPortsWarehouses);

// 0x413f80. Loads the site table: a 12-byte header (count third), the site ids
// (0x62ba6c/0x62ba70) and the 0x74e-byte site records (0x62b2f8) with their
// variable parts. Returns the bytes of the fixed part read.
int32_t __stdcall Sites_LoadTable(Stream s) {
    int32_t hdr[3] = { 0, 0, 0 };
    StreamRead(s, hdr, 0xc, 0, 1);
    int32_t n = hdr[2];
    hook::Global<int32_t>(0x62ba64) = n;
    if (n <= 0) {
        hook::Global<int32_t>(0x62ba74) = 0;
        hook::Global<int32_t>(0x62ba68) = n;
        return 0xc;
    }
    uint32_t* ids = (uint32_t*)Malloc(n * 4);
    hook::Global<uint32_t*>(0x62ba6c) = ids;
    hook::Global<uint32_t*>(0x62ba70) = ids;
    if (hook::Global<void*>(0x62b2f8)) {
        Free(hook::Global<void*>(0x62b2f8));
        hook::Global<void*>(0x62b2f8) = nullptr;
    }
    hook::Global<uint8_t*>(0x62b2f8) = (uint8_t*)Malloc(hook::Global<int32_t>(0x62ba64) * 0x74e);
    for (int32_t i = 0; i < hook::Global<int32_t>(0x62ba64); ++i) {
        uint8_t* r = hook::Global<uint8_t*>(0x62b2f8) + i * 0x74e;
        memset(r, 0, 0x74e);
        StreamRead(s, &hook::Global<uint32_t*>(0x62ba6c)[i], 4, 0, 1);
        static const uint16_t fixed[] = { 4, 8, 0xc, 0x4b8, 0x4bc, 0x49c };
        for (uint16_t off : fixed) StreamRead(s, r + off, 4, 0, 1);
        for (uint32_t k = 0; k < F<uint32_t>(r, 0x49c) + 1; ++k) StreamRead(s, r + 0x10 + k * 4, 4, 0, 1);
        StreamRead(s, r + 0x4a0, 0x10, 0, 1);
        StreamRead(s, r + 0x4c4, 4, 0, 1);
        if (F<int32_t>(r, 0x4c4) > 0) {
            void* p = Malloc(F<int32_t>(r, 0x4c4));
            F<void*>(r, 0x4c0) = p;
            StreamRead(s, p, F<int32_t>(r, 0x4c4), 0, 1);
        }
        StreamRead(s, r + 0x65c, 4, 0, 1);
        for (uint32_t k = 0; k < F<uint32_t>(r, 0x65c); ++k) StreamRead(s, r + 0x4c8 + k * 4, 4, 0, 1);
        StreamRead(s, r + 0x72e, 4, 0, 1);
        for (uint32_t k = 0; k < F<uint32_t>(r, 0x72e); ++k) {
            StreamRead(s, r + 0x664 + k, 1, 0, 1);
            StreamRead(s, r + 0x6c9 + k, 1, 0, 1);
        }
        StreamRead(s, r + 0x732, 4, 0, 1);
    }
    int32_t cnt = hook::Global<int32_t>(0x62ba64);
    hook::Global<int32_t>(0x62ba74) = 0;
    hook::Global<int32_t>(0x62ba68) = cnt;
    return cnt * 4 + 0xc;
}
RT3_REPLACE(0x413f80, Sites_LoadTable);

// 0x563ed0. Restarts every sound channel of the object (+0x8d list), looping.
void __fastcall Channels_PlayAll(uint8_t* o) {
    PtrList_Rewind(F<PtrList*>(o, 0x8d));
    for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d)))
        Channel_Command(c, 0, 1, -1);
}
RT3_REPLACE(0x563ed0, Channels_PlayAll);

} // namespace rt3
