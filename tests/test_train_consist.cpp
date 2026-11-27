// Differential fixtures for train functions reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include "game/collection.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
// Rewrites the fakes below also patch (they are called by name now).
void __fastcall Car_UpdatePose(uint8_t* car, int, float dist);
void __fastcall Car_SetPiece(uint8_t* car, int, int32_t piece, int32_t pos);
int32_t __fastcall Car_FollowLeader(uint8_t* c, int, float* pos);
void __fastcall Train_ReanchorConsist(uint8_t* t);
void __fastcall Train_DetachCar(uint8_t* t, int, uint32_t id);
void __fastcall Train_SetState3(uint8_t* t, int, uint8_t* car, uint8_t* target, uint32_t flag);
void __fastcall Train_SetState5(uint8_t* t, int, void* unused, uint8_t* target, uint32_t flag);
void* __fastcall Obj_City352(void* o);
void* __fastcall Obj_Ref388(void* o);
void* __fastcall Obj_Building(void* o);
void __fastcall Train_ScanCarKinds(uint8_t* t);
void __fastcall Train_RefreshRows(uint8_t* t);
int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
void __fastcall Train_EnterBlock(uint8_t* t, int, uint8_t* car, uint8_t* other);
void __fastcall Train_ResetCarAnimations(uint8_t* t);
int32_t IsMultiplayer();
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alpha, int32_t deep);
void __fastcall Model_SetOpacity(uint8_t* m, int, float a);
void __fastcall Station_UnlinkNodes(void* t);
}

namespace {

template <typename T> T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

struct Redirects {
    struct Patch { uint8_t* at; uint8_t saved[5]; } patches[40];
    int count = 0;
    void AddOne(uintptr_t address, void* fn) {
        Patch& p = patches[count++];
        p.at = (uint8_t*)address;
        DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.saved, p.at, 5);
        p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)fn - (address + 5));
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    void Add(uintptr_t original, void* cpp, void* fn) {
        AddOne(original, fn);
        if (cpp) AddOne((uintptr_t)cpp, fn);
    }
    ~Redirects() {
        while (count) {
            Patch& p = patches[--count];
            DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.saved, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};

struct Fixture {
    uint8_t train[0x200], cars[4][0x400], types[2][0x120], cargo[0x60], target[0x580];
    uint32_t log[256];
    int used;
};
Fixture g_f;
rt3::Collection g_cars, g_cargo;
uint32_t g_bits, g_flag;
int g_mode, g_gets, g_refs, g_mp, g_remove;

void Event(uint32_t a, uint32_t b = 0, uint32_t c = 0) {
    g_f.log[g_f.used++] = a;
    g_f.log[g_f.used++] = b;
    g_f.log[g_f.used++] = c;
}
int Car(const void* p) {
    for (int i = 0; i < 4; ++i) if (p == g_f.cars[i]) return i;
    return -1;
}
void* __fastcall Get(rt3::Collection* c, int, uint32_t id) {
    Event(1, c == &g_cars ? 0 : 1, id);
    if (c == &g_cargo) return g_f.cargo;
    if (g_mode == 0 && g_gets++ == 0) {
        if (g_bits & 1) return nullptr;
        if (g_bits & 2) F<uint32_t>(g_f.train, 0x41) = 1;
    }
    return id < 4 ? g_f.cars[id] : nullptr;
}
void* __fastcall Next(void* car) {
    int32_t next = F<int32_t>(car, 0x352);
    Event(2, (uint32_t)Car(car), (uint32_t)next);
    return next == -1 ? nullptr : g_f.cars[next];
}
void __fastcall Move(uint8_t* car, int, uint32_t node, uint32_t position) {
    Event(3, (uint32_t)Car(car), node);
    Event(4, position);
    F<uint32_t>(car, 0x356) = node;
    F<uint32_t>(car, 0x35e) = position;
}
void __fastcall Pose(uint8_t* car, int, uint32_t movement) {
    Event(5, (uint32_t)Car(car), movement);
    F<uint32_t>(car, 0x10d) ^= 0x55aa55aa;
}
int32_t __fastcall Follow(uint8_t* car, int, void* position) {
    Event(6, (uint32_t)Car(car), (uint32_t)(uintptr_t)position);
    F<uint32_t>(car, 0x362) = 0x12340000 | (uint32_t)Car(car);
    return 13;
}
void* __fastcall Type(void* car) {
    Event(7, (uint32_t)Car(car), g_refs);
    return g_f.types[g_refs++ & 1];
}
void __fastcall Release(rt3::Collection* c, int, uint32_t id) {
    Event(8, c == &g_cars, id);
    Event(9, F<uint32_t>(g_f.train, 0x41), F<uint32_t>(g_f.train, 0x45));
    g_f.cars[id][0] = 0xcc;
    if (g_bits & 0x20000) g_f.train[0x1ab] ^= 1;
}
void __fastcall Scan(uint8_t* t) { Event(10, t == g_f.train); t[0x197] = 1; }
void __fastcall Rows(uint8_t* t) { Event(11, t == g_f.train); t[0x198] = 2; }
int32_t __fastcall SetState(uint8_t* t, int, int32_t state, float timeout) {
    Event(12, (uint32_t)state, *(uint32_t*)&timeout);
    Event(13, F<uint32_t>(t, 0xcc), t[0xfc]);
    F<int32_t>(t, 0xe8) = state;
    F<float>(t, 0xec) = timeout;
    return 77;
}
void* __fastcall Building(void* target) { Event(14, target == g_f.target); return g_f.types[0]; }
void __fastcall Enter(uint8_t* t, int, uint8_t* car, uint8_t* block) {
    Event(15, t == g_f.train, (uint32_t)Car(car));
    Event(16, block == g_f.types[0]);
}
void __fastcall Reset(uint8_t* t) {
    Event(17, F<uint32_t>(t, 0xe8), t[0x1a2]);
    ++t[0x1b0];
}
int32_t Mp() {
    Event(18, g_mp);
    if (!g_mp && (g_bits & 0x40000000)) g_f.train[0xf4] = (uint8_t)((g_bits >> 29) & 1);
    return (g_bits >> (19 + g_mp++)) & 1;
}
int32_t __stdcall Version(float version) {
    Event(19, *(uint32_t*)&version);
    return (g_bits >> 25) & 1;
}
void __fastcall Alpha(uint8_t* car, int, uint32_t alpha, int32_t deep) {
    Event(20, (uint32_t)Car(car), alpha);
    Event(21, (uint32_t)deep);
    if (g_bits & 0x4000000) g_f.train[0xf4] = (uint8_t)((g_bits >> 27) & 1);
}
void __fastcall Opacity(uint8_t* car, int, float opacity) {
    Event(22, (uint32_t)Car(car), *(uint32_t*)&opacity);
    F<float>(car, 0x30) = opacity;
}
void __fastcall Unlink(void* train) { Event(23, train == g_f.train, g_f.train[0xf4]); }
void __fastcall Reanchor(uint8_t* train) { Event(24, train == g_f.train); ++train[0x1b1]; }

void PatchCommon(Redirects& p) {
    p.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&Get);
    p.Add(0x41ad90, (void*)&rt3::Obj_City352, (void*)&Next);
    p.Add(0x41adb0, (void*)&rt3::Obj_Ref388, (void*)&Type);
    p.Add(0x41c700, (void*)&rt3::Car_SetPiece, (void*)&Move);
    p.Add(0x41b080, (void*)&rt3::Car_UpdatePose, (void*)&Pose);
    p.Add(0x41c790, (void*)&rt3::Car_FollowLeader, (void*)&Follow);
    p.Add(0x4a9060, (void*)&rt3::Train_ScanCarKinds, (void*)&Scan);
    p.Add(0x4a8f50, (void*)&rt3::Train_RefreshRows, (void*)&Rows);
}
void PatchStates(Redirects& p) {
    p.Add(0x4ab980, (void*)&rt3::Train_SetState, (void*)&SetState);
    p.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&Building);
    p.Add(0x4aaad0, (void*)&rt3::Train_EnterBlock, (void*)&Enter);
    p.Add(0x4a7810, (void*)&rt3::Train_ResetCarAnimations, (void*)&Reset);
    p.Add(0x434050, (void*)&rt3::IsMultiplayer, (void*)&Mp);
    p.Add(0x46d2b0, (void*)&rt3::Net_AllHaveVersion, (void*)&Version);
    p.Add(0x52fb70, (void*)&rt3::SceneObj_SetAlpha, (void*)&Alpha);
    p.Add(0x45c8d0, (void*)&rt3::Model_SetOpacity, (void*)&Opacity);
    p.Add(0x4a81b0, (void*)&rt3::Station_UnlinkNodes, (void*)&Unlink);
    p.Add(0x4a9840, (void*)&rt3::Train_ReanchorConsist, (void*)&Reanchor);
}

void Setup(uint32_t it, int mode) {
    memset(&g_f, 0, sizeof g_f);
    uint32_t r = it * 2654435761u + 497;
    auto rnd = [&]() { r = r * 1103515245u + 12345u; return r; };
    g_bits = rnd();
    g_flag = rnd();
    g_mode = mode;
    g_gets = g_refs = g_mp = 0;
    for (uint8_t& b : g_f.train) b = (uint8_t)(rnd() >> 16);
    for (auto& car : g_f.cars) for (uint8_t& b : car) b = (uint8_t)(rnd() >> 16);
    for (uint8_t& b : g_f.target) b = (uint8_t)(rnd() >> 16);
    int count = 1 + (int)(it % 4);
    g_remove = (int)((it / 4) % (uint32_t)count);
    F<int32_t>(g_f.train, 0x41) = mode != 1 && (g_bits & 0x10) ? -1 : 0;
    F<int32_t>(g_f.train, 0x45) = count - 1;
    F<int32_t>(g_f.train, 0xe8) = (g_bits & 1) ? (mode == 2 ? 3 : 5) : 0;
    g_f.train[0xf4] = (uint8_t)((g_bits >> 2) & 3);
    g_f.train[0x1ab] = (uint8_t)((g_bits >> 3) & 1);
    for (int i = 0; i < 4; ++i) {
        F<int32_t>(g_f.cars[i], 0x34e) = i ? i - 1 : -1;
        F<int32_t>(g_f.cars[i], 0x352) = i + 1 < count ? i + 1 : -1;
        F<uint32_t>(g_f.cars[i], 0x38c) = (g_bits & 0x40) ? 12 : 0;
    }
    // Keep the head-change reanchor case valid even with a one-car list.
    if (mode == 0 && count == 1) g_bits &= ~2u;
    static const uint32_t floats[] = {
        0, 0x80000000, 0x3f800000, 0xbf800000, 0x7f800000, 0xff800000,
        0x7fc12345, 0x7f812345, 0x00800000, 1, 0x4f000000, 0x3e395810
    };
    F<uint32_t>(g_f.train, 0xf5) = floats[it % 12];
    F<uint32_t>(g_f.types[0], 0xfd) = floats[(it / 12) % 12];
    F<int32_t>(g_f.types[1], 0x41) = (g_bits & 0x80) ? 1 : 4;
    g_f.cargo[0x47] = (uint8_t)((g_bits >> 8) & 1);
}

void RunCases(int mode, uintptr_t original, void* cpp) {
    uint32_t* carsGlobal = (uint32_t*)0x62ba84;
    uint32_t* cargoGlobal = (uint32_t*)0x62ba8c;
    uint32_t savedCars = *carsGlobal, savedCargo = *cargoGlobal;
    rt3::CollectionVtbl vt = {};
    vt.release = (void(__thiscall*)(rt3::Collection*, uint32_t))&Release;
    g_cars.vtbl = &vt;
    *carsGlobal = (uint32_t)(uintptr_t)&g_cars;
    *cargoGlobal = (uint32_t)(uintptr_t)&g_cargo;
    for (uint32_t it = 0; it < 4096; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it, mode);
            void* fn = side ? cpp : (void*)original;
            if (mode == 0) ((void(__fastcall*)(uint8_t*))fn)(g_f.train);
            else if (mode == 1) ((void(__fastcall*)(uint8_t*, int, uint32_t))fn)(g_f.train, 0, (uint32_t)g_remove);
            else ((void(__fastcall*)(uint8_t*, int, uint8_t*, uint8_t*, uint32_t))fn)(
                g_f.train, 0, g_f.cars[0], g_f.target, g_flag);
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t offset = 0;
                while (((uint8_t*)&expected)[offset] == ((uint8_t*)&g_f)[offset]) ++offset;
                EXPECT_EQ(1, 0, "train consist mode %d case %u byte %u: %02x / %02x (events %d / %d)",
                          mode, it, (unsigned)offset, ((uint8_t*)&expected)[offset], ((uint8_t*)&g_f)[offset],
                          expected.used, g_f.used);
            }
        }
    }
    *carsGlobal = savedCars;
    *cargoGlobal = savedCargo;
}

} // namespace

DIFFTEST(game_TrainReanchorConsist) {
    Redirects p;
    PatchCommon(p);
    RunCases(0, 0x4a9840, (void*)&rt3::Train_ReanchorConsist);
}
DIFFTEST(game_TrainDetachCar) {
    Redirects p;
    PatchCommon(p);
    RunCases(1, 0x4a9e00, (void*)&rt3::Train_DetachCar);
}
DIFFTEST(game_TrainSetState3) {
    Redirects p;
    PatchStates(p);
    RunCases(2, 0x4acf90, (void*)&rt3::Train_SetState3);
}
DIFFTEST(game_TrainSetState5) {
    Redirects p;
    PatchCommon(p);
    PatchStates(p);
    RunCases(3, 0x4ad020, (void*)&rt3::Train_SetState5);
}
