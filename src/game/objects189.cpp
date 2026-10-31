// Re-grounding objects and the ambient sound slots in blocks 0x40..0x53 (batch 195).
// See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double Fl(uint32_t a) { return *(const float*)a; }

void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t IsMultiplayer();
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __fastcall Map_NearestLandHeight(uint8_t* m, int, int32_t x, int32_t y, int32_t r);
void __fastcall Obj_SettleOnNode(uint8_t* o);
void __fastcall Obj_DropToGround(uint8_t* o, int, int32_t lift);

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

// 0x40ee10. Puts the object back on the ground at its own cell position: on the
// terrain, or (types whose kind has +0xba) on the nearest land within 6 cells minus a
// margin; whole units in multiplayer. Then settles it on its track node.
void __fastcall Obj_Reground(uint8_t* o) {
    float cx = (float)Obj_CellX(o);
    float cz = (float)Obj_CellY(o);
    float z = (float)((double)cz * Fl(0x5c8700));
    float x = (float)((double)cx * Fl(0x5c8700));
    float y = (float)Terrain_HeightAt((uint8_t*)Engine_Get1C(*(void**)0x6d4024), 0, x, z);
    uint8_t* type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(o, 0x3cc));
    uint8_t* kind = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(type, 0x173));
    if (kind[0xba]) {
        int32_t iz = Ftol(cz);
        float h = (float)Map_NearestLandHeight(hook::Global<uint8_t*>(0x62c120), 0, Ftol(cx), iz, 6);
        y = (float)((double)h - Fl(0x5c8840));
    }
    if (IsMultiplayer()) y = (float)Ftol(y);
    Obj_MoveTo(o, 0, x, y, z);
    Obj_SettleOnNode(o);
}
RT3_REPLACE(0x40ee10, Obj_Reground);

static inline bool InCellRect(uint8_t* o, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    return Obj_CellX(o) >= (float)x0 && Obj_CellX(o) <= (float)x1 && Obj_CellY(o) >= (float)y0 &&
           Obj_CellY(o) <= (float)y1;
}

// 0x421f50. Drops every object of the collection inside the cell rectangle (x0, y0)..
// (x1, y1) to the ground, except those flagged +0x23e.
void __fastcall Objs_DropInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(o, 0x23e)) continue;
        if (InCellRect(o, x0, y0, x1, y1)) Obj_DropToGround(o, 0, 0);
    }
}
RT3_REPLACE(0x421f50, Objs_DropInRect);

// 0x461770. Drops every object of the collection inside the cell rectangle to the
// ground.
void __fastcall Objs_DropAllInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (InCellRect(o, x0, y0, x1, y1)) Obj_DropToGround(o, 0, 0);
    }
}
RT3_REPLACE(0x461770, Objs_DropAllInRect);

int32_t __fastcall Sound_StartInSlot(uint8_t* s, int, const char* name);

// 0x5373b0. Clears the 27 sound slots and loads the weather sounds: rain
// (wea_rain.wav) and six thunder claps (wea_thunder_01..06.wav).
void __fastcall Weather_LoadSounds(uint8_t* s) {
    for (int32_t i = 0; i < 0x1b; ++i) F<uint8_t*>(s, 0x40c + i * 4) = nullptr;
    F<int32_t>(s, 0x408) = 0;
    static const uint32_t names[] = { 0x5dd378, 0x5dd364, 0x5dd350, 0x5dd33c, 0x5dd328, 0x5dd314, 0x5dd300 };
    for (uint32_t n : names) Sound_StartInSlot(s, 0, (const char*)n);
}
RT3_REPLACE(0x5373b0, Weather_LoadSounds);

} // namespace rt3
