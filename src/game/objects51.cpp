// Functions across blocks 0x40..0x4b (batch 54). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);

uint8_t* __fastcall Obj_RegionAt(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_SharesRegionKey(void* o, int, uint32_t bid);
void __fastcall Table_Hook47d8d0(Collection* t);
void __fastcall Table_TouchRegionSites(Collection* t);
void __fastcall Table_Hook4a7350(Collection* t);
void __fastcall Track_Validate(Collection* t);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
double __fastcall Obj_Float18(void* o);
void __fastcall Camera_UpdateZoomFlags(uint8_t* o);
uint32_t __fastcall Obj_CenterGridByte(void* o);
void __fastcall Routes_Recount(Collection* routes, int, int32_t rid);
double __fastcall Obj_Float2c0(void* o, int, int32_t i);

static inline char* ExeStrstr(const char* s, const char* k) { return ((char*(__cdecl*)(const char*, const char*))0x5a1ff0)(s, k); }

// 0x401860. The company's home station node (+0xd35): kept while it is an
// own station piece (+0x216 == 2), else re-found among the track pieces.
int32_t __fastcall Company_HomeStation(int32_t* c) {
    bool ok = false;
    if (Collection_IsValid(g_trackNodes, 0, F<uint32_t>(c, 0xd35))) {
        uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(c, 0xd35));
        ok = F<uint16_t>(n, 0x22e) == c[0] && n[0x216] == 2;
    }
    if (!ok) F<int32_t>(c, 0xd35) = -1;
    if (F<int32_t>(c, 0xd35) == -1 && F<int32_t>(c, 0x766c) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664) > 0) {
        for (int32_t i = 0; i < Collection_Count(g_trackNodes); ++i) {
            uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, Collection_IdAt(g_trackNodes, 0, i, 0));
            if (F<uint16_t>(n, 0x22e) == c[0] && n[0x216] == 2) {
                F<int32_t>(c, 0xd35) = F<int32_t>(n, 0x202);
                break;
            }
        }
    }
    return F<int32_t>(c, 0xd35);
}
RT3_REPLACE(0x401860, Company_HomeStation);

// 0x40d270. When this building's region holds key k, drops the demand
// records of its chain (+0x272) whose source (+0xe) shares a region key.
void __fastcall Building_DropSharedDemand(uint8_t* b, int, uint16_t k) {
    if (!Obj_HasKey(Obj_RegionAt(b), 0, k)) return;
    Collection* demand = hook::Global<Collection*>(0x6ada90);
    for (uint32_t id = F<uint32_t>(b, 0x272); id; ) {
        uint8_t* rec = (uint8_t*)Collection_Get(demand, 0, id);
        uint32_t next = F<uint32_t>(rec, 4);
        if (Obj_SharesRegionKey(b, 0, F<uint16_t>(rec, 0xe))) {
            void** vt = *(void***)demand;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(demand, id);
        }
        id = next;
    }
}
RT3_REPLACE(0x40d270, Building_DropSharedDemand);

// 0x433b80. Debug hooks/validation over the main tables (unless 0x62be40).
void RunDebugValidation() {
    if (hook::Global<int32_t>(0x62be40)) return;
    if (Collection* t = hook::Global<Collection*>(0x6cec20)) Table_Hook47d8d0(t);
    if (Collection* t = hook::Global<Collection*>(0x62b26c)) Table_TouchRegionSites(t);
    if (Collection* t = hook::Global<Collection*>(0x6cfcbc)) Table_Hook4a7350(t);
    if (Collection* t = hook::Global<Collection*>(0x6cfca8)) Track_Validate(t);
}
RT3_REPLACE(0x433b80, RunDebugValidation);

// 0x468d00. Adds (mode 1, ';'-separated, if absent) or removes a name in the list +0x905c.
void __fastcall Obj_EditNameList(uint8_t* o, int, const char* name, int32_t mode) {
    char* list = (char*)o + 0x905c;
    auto append = [](char* dst, const char* src) {
        while (*dst) ++dst;
        while ((*dst++ = *src++) != 0) {}
    };
    if (mode == 1) {
        if (ExeStrstr(list, name)) return;
        append(list, (const char*)0x5cbc48);
        append(list, name);
        return;
    }
    char* p = ExeStrstr(list, name);
    if (!p) return;
    if (p == list) *p = 0;
    else p[-1] = 0;
    size_t len = 0;
    while (name[len]) ++len;
    char* rest = p + len + 1;
    if (!*rest) return;
    if (*rest == ';') ++rest;
    append(list, rest);
}
RT3_REPLACE(0x468d00, Obj_EditNameList);

// 0x487540. Initialises a station record (name "-", stats cleared).
void __fastcall Station_Init(uint8_t* s) {
    F<int32_t>(s, 0) = 0;
    F<int32_t>(s, 0x2d) = 0;
    F<int32_t>(s, 0x31) = 1;
    F<int32_t>(s, 0x35) = 1;
    F<int32_t>(s, 0x39) = 100000;
    FormatText((char*)s + 4, (const char*)0x5ca2b4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    static const uint32_t kZero[] = { 0x3d, 0x41, 0x45, 0x49, 0x1fd, 0x201, 0x4d, 0x51, 0x205, 0x209 };
    for (uint32_t off : kZero) F<int32_t>(s, off) = 0;
    for (int32_t i = 0; i < 0x35; ++i) {
        F<int32_t>(s, 0x55 + i * 4) = 0;
        F<int32_t>(s, 0x129 + i * 4) = 0;
        F<int32_t>(s, 0x20d + i * 4) = 0;
        F<int32_t>(s, 0x2e1 + i * 4) = 0;
    }
    for (int k = 0; k < 7; ++k) F<int32_t>(s, 0xd51 + k * 4) = 0;
    F<int32_t>(s, 0xd4d) = 1;
    for (int k = 0; k < 0x266; ++k) F<int32_t>(s, 0x3b5 + k * 4) = 0;
    for (int k = 0; k < 0x19; ++k) F<int32_t>(s, 0xd6d + k * 4) = 0;
}
RT3_REPLACE(0x487540, Station_Init);

// 0x4934a0..0x493560. Zoom flag of a camera action (+0xb0 for +0xa0/+0xa4,
// +0xb4 for +0xa8/+0xac), refreshed when the camera value moved; 0 if inactive.
static int32_t CameraFlag(uint8_t* o, uint32_t active, uint32_t flag) {
    if (!F<int32_t>(o, active)) return 0;
    if (!(Obj_Float18(hook::Global<void*>(0x62be68)) == F<float>(o, 0xb8))) Camera_UpdateZoomFlags(o);
    return F<int32_t>(o, flag);
}
int32_t __fastcall Camera_FlagA0(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xa0, 0xb0); }
RT3_REPLACE(0x4934a0, Camera_FlagA0);
int32_t __fastcall Camera_FlagA4(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xa4, 0xb0); }
RT3_REPLACE(0x4934e0, Camera_FlagA4);
int32_t __fastcall Camera_FlagA8(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xa8, 0xb4); }
RT3_REPLACE(0x493520, Camera_FlagA8);
int32_t __fastcall Camera_FlagAc(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xac, 0xb4); }
RT3_REPLACE(0x493560, Camera_FlagAc);

// 0x495020. Upgrades a route record from older save versions (world
// +0x46c34) and resets its scratch fields; single-piece routes are recounted.
// QUIRK: the owner fix-up writes +0x18, not +0x1e.
void __fastcall Route_Upgrade(uint8_t* r, int, int32_t) {
    auto firstNode = [&]() -> int32_t {
        if (F<int32_t>(r, 4) != -1) return F<int32_t>(r, 4);
        return F<int32_t>(r, 8);
    };
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c34) <= 0x2533) {
        F<int32_t>(r, 0x18) = 1;
        int32_t id = firstNode();
        if (id != -1) F<uint32_t>(r, 0x18) = Obj_CenterGridByte(Collection_Get(g_trackNodes, 0, id));
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c34) <= 0x2535) {
        F<int32_t>(r, 0x1e) = 1;
        int32_t id = firstNode();
        if (id != -1) F<int32_t>(r, 0x18) = F<uint16_t>(Collection_Get(g_trackNodes, 0, id), 0x22e);
    }
    for (int k = 0; k < 0x1e; ++k) r[0x24 + k] = 0;
    F<int32_t>(r, 0x42) = 0;
    F<int32_t>(r, 0x46) = 0;
    if (F<int32_t>(r, 4) == F<int32_t>(r, 8) && F<int32_t>(r, 0x10) > 1)
        Routes_Recount(hook::Global<Collection*>(0x6cfcb4), 0, F<int32_t>(r, 0));
}
RT3_REPLACE(0x495020, Route_Upgrade);

// 0x4b2b80 / 0x4b2bf0. Order list (0x33-byte entries, count +4, cursor +8):
// count of unflagged orders (+0x28 >= 0); current order (0 if none).
int32_t __fastcall OrderList_CountPlain(uint8_t* o) {
    int32_t n = 0;
    const uint8_t* e = F<uint8_t*>(o, 0) + 0x28;
    for (int32_t i = 0; i < F<int32_t>(o, 4); ++i, e += 0x33)
        if ((int8_t)*e >= 0) ++n;
    return n;
}
RT3_REPLACE(0x4b2b80, OrderList_CountPlain);
uint8_t* __fastcall OrderList_Current(uint8_t* o) {
    int32_t i = F<int32_t>(o, 8);
    if (i < 0) return nullptr;
    if (i == 0) i = F<int32_t>(o, 4);
    return F<uint8_t*>(o, 0) + (i - 1) * 0x33;
}
RT3_REPLACE(0x4b2bf0, OrderList_Current);

// 0x4b34e0. Index (last match, 0 if none) of name +4 in the 23-entry table 0x621ac0.
int32_t __fastcall Obj_NameTableIndex(uint8_t* o) {
    int32_t found = 0;
    for (int32_t i = 0; i < 0x17; ++i)
        if (Stricmp(((const char**)0x621ac0)[i], (const char*)o + 4) == 0) found = i;
    return found;
}
RT3_REPLACE(0x4b34e0, Obj_NameTableIndex);

double __fastcall Building_Total2c0(uint8_t* b) { return Obj_Float2c0(b, 0, 0) + F<float>(b, 0x2ac); }
RT3_REPLACE(0x4b35b0, Building_Total2c0);

} // namespace rt3
