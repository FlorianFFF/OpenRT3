// Sound attachment for industry buildings, engines and trains in blocks 0x45..0x4a (batch 192). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Obj_StaticEntry(void* o, int, char* name, int32_t* a, int32_t* b);
char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*);
uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6,
                                  int32_t a7, float vol);
void __fastcall Channel_SetRange(uint8_t* c, int, uint32_t lo, uint32_t hi);
void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v);
void __fastcall Obj_AttachChannel(uint8_t* o, int, uint8_t* ch);
uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
void __fastcall Obj_MoveChannels(uint8_t* o);
void __fastcall Vehicle_UpdateHorn(uint8_t* v, int, int32_t);
float __fastcall Vehicle_GradeSpeed(uint8_t* o, int, uint8_t* so);
void __fastcall Train_UpdateEngineEffects(uint8_t* t, int, float throttle, float speed);

static inline uint8_t* NewChannel() { return (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x141); }
static inline uint32_t U(float f) { return *(uint32_t*)&f; }

// 0x45b210. Gives an industry building its looping ambient sound (from its type's
// static entry: name, volume, range) on terrain `grid` at (x, y, z), stored in
// +0x24a. Returns 0 when the building has no industry or the type no sound.
int32_t __fastcall Building_AttachSound(uint8_t* b, int, int32_t, uint8_t* grid, float x, float y, float z) {
    typedef void*(__thiscall * IndFn)(void*);
    if (!((IndFn)(*(void***)b)[0x80 / 4])(b)) return 0;
    char base[0x1a8 - 0x18];
    int32_t vol, range;
    int32_t found = Obj_StaticEntry(((IndFn)(*(void***)b)[0x80 / 4])(b), 0, base, &vol, &range);
    if (!found) return 0;
    char name[0x190];
    FormatText(name, (const char*)0x5cb550, base, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    uint8_t* p = NewChannel();
    uint8_t* ch = p ? Channel_Setup(p, 0, name, grid, x, y, z, -1, 1, -1.0f) : nullptr;
    F<uint8_t*>(b, 0x24a) = ch;
    float lo = (float)((double)*(float*)&range * *(const double*)0x5cb548);
    Channel_SetRange(ch, 0, U(lo), U((float)((double)lo * *(const float*)0x5ca210)));
    float v = (float)((double)*(float*)&vol * *(const double*)0x5c8750);
    float cv = (float)(v < *(const double*)0x5c8770 ? (double)v : 1.0);
    Channel_SetVolume(F<uint8_t*>(b, 0x24a), 0, U(cv));
    Obj_AttachChannel(b, 0, F<uint8_t*>(b, 0x24a));
    return found;
}
RT3_REPLACE(0x45b210, Building_AttachSound);

// 0x45d130. Gives a vehicle the engine sound of engine type `typeName` (its sample
// table entry 0x5f3a60[+0x83]) on terrain `grid` at (x, y, z), into +0x336.
int32_t __fastcall Vehicle_AttachEngineSound(uint8_t* o, int, const char* typeName, uint8_t* grid, float x, float y, float z) {
    uint32_t id = Table_FindByAltName(hook::Global<Collection*>(0x6ada84), 0, typeName);
    if (!id) return 0;
    uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, id);
    uint8_t* p = NewChannel();
    uint8_t* ch = p ? Channel_Setup(p, 0, ((const char**)0x5f3a60)[F<int32_t>(e, 0x83)], grid, x, y, z, -1, 1, -1.0f)
                    : nullptr;
    F<uint8_t*>(o, 0x336) = ch;
    Channel_SetRange(ch, 0, 0x42b80000, 0x45b80000);
    Channel_SetVolume(F<uint8_t*>(o, 0x336), 0, 0x3f800000);
    Obj_AttachChannel(o, 0, F<uint8_t*>(o, 0x336));
    return 1;
}
RT3_REPLACE(0x45d130, Vehicle_AttachEngineSound);

// 0x4acf30. Moves the train engine's sounds along, updates its horn and sets the
// engine sound and animation from its grade-adjusted speed and the train speed.
void __fastcall Train_UpdateSound(uint8_t* t) {
    if (F<int32_t>(t, 0x41) == -1) return;
    uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, F<uint32_t>(t, 0x41));
    if (!e) return;
    Obj_MoveChannels(e);
    Vehicle_UpdateHorn(e, 0, 0x3f800000);
    float v = Vehicle_GradeSpeed(t, 0, e);
    Train_UpdateEngineEffects(t, 0, v, F<float>(t, 0xcc));
}
RT3_REPLACE(0x4acf30, Train_UpdateSound);

} // namespace rt3

namespace rt3 {

struct PtrListNode;
void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);

static inline int32_t Cell16(float v) {
    double d = (double)v * *(const float*)0x5dcfe8;
    int32_t r;
    __asm {
        fld d
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r >> 4;
}

// 0x52fdc0. Moves the object (+0x1e2..+0x1ea) by (dx, dy, dz), marking it moved
// (+0x20 bit 1); on a terrain (+4) it is re-filed in the 16-cell grid (node +8) when
// it changes cell, and the move is undone (returning false) when it would leave the
// map. Its sound channels follow.
bool __fastcall Obj_MoveBy(uint8_t* o, int, float dx, float dy, float dz) {
    o[0x20] |= 2;
    uint8_t* grid = F<uint8_t*>(o, 4);
    if (!grid) {
        F<float>(o, 0x1e2) = (float)((double)dx + F<float>(o, 0x1e2));
        F<float>(o, 0x1e6) = (float)((double)dy + F<float>(o, 0x1e6));
        F<float>(o, 0x1ea) = (float)((double)dz + F<float>(o, 0x1ea));
        Obj_MoveChannels(o);
        return true;
    }
    int32_t ox = Cell16(F<float>(o, 0x1e2)), oz = Cell16(F<float>(o, 0x1ea));
    float nx = (float)((double)dx + F<float>(o, 0x1e2));
    F<float>(o, 0x1e2) = nx;
    float ny = (float)((double)dy + F<float>(o, 0x1e6));
    F<float>(o, 0x1e6) = ny;
    float nz = (float)((double)dz + F<float>(o, 0x1ea));
    F<float>(o, 0x1ea) = nz;
    int32_t cx = Cell16(nx), cz = Cell16(nz);
    if (ox == cx && oz == cz) {
        Obj_MoveChannels(o);
        return true;
    }
    if (cx < 0 || cz < 0) {
        F<float>(o, 0x1e2) = (float)((double)nx - dx);
        F<float>(o, 0x1e6) = (float)((double)ny - dy);
        F<float>(o, 0x1ea) = (float)((double)nz - dz);
        return false;
    }
    int32_t w = (Terrain_Rows(F<uint8_t*>(o, 4)) - 1) >> 4;
    int32_t h = (Terrain_Cols(F<uint8_t*>(o, 4)) - 1) >> 4;
    if (cx >= w || cz >= h) {
        F<float>(o, 0x1e2) = (float)((double)F<float>(o, 0x1e2) - dx);
        F<float>(o, 0x1e6) = (float)((double)F<float>(o, 0x1e6) - dy);
        F<float>(o, 0x1ea) = (float)((double)F<float>(o, 0x1ea) - dz);
        return false;
    }
    if (F<PtrListNode*>(o, 8)) Terrain_Remove1671(F<uint8_t*>(o, 4), 0, F<PtrListNode*>(o, 8), ox, oz);
    F<PtrListNode*>(o, 8) = Terrain_Add1671(F<uint8_t*>(o, 4), 0, o, cx, cz);
    Obj_MoveChannels(o);
    return true;
}
RT3_REPLACE(0x52fdc0, Obj_MoveBy);

} // namespace rt3
