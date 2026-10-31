// Vehicle part animation, positioning and sound slots in blocks 0x45..0x53 (batch 193).
// See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }
static inline double ExeCos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }
static inline double ExeSin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }
static inline double ExeFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
float __fastcall SceneObj_Angle1(uint8_t* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
bool __fastcall Obj_MoveBy(uint8_t* o, int, float dx, float dy, float dz);

// 0x456210. Swings the two pairs of linked parts (+0x32e / +0x33e, phases +0x332..
// / +0x342..) along with the drive phase (+0x336 / +0x346, advanced by step x
// (max(cos, 0) x k + +0x34e)), then rocks the body: pitch from the drive phase, a
// vertical bob, and a damped roll accumulated in +0x37e.
void __fastcall Vehicle_AnimateLinks(uint8_t* v, int, float step) {
    for (int32_t i = 0; i < 2; ++i) {
        float w = (float)(D(0x5c8770) / ((double)(float)(i * 3) + D(0x5c8770)));
        if (uint8_t* part = F<uint8_t*>(v, 0x32e + i * 4)) {
            float a;
            if (i) {
                double c = ExeCos(F<float>(v, 0x336 + i * 4));
                a = (float)((ExeCos(F<float>(v, 0x332 + i * 4)) + c) * w - D(0x5cb2d0));
            } else a = (float)(ExeCos(F<float>(v, 0x336)) * w + D(0x5cb2c8));
            SceneObj_SetAngles(part, 0, 0, 0, Bits(a));
            float t = (float)(ExeCos(F<float>(v, 0x336 + i * 4)) * D(0x5c8798));
            if (t < *(const float*)0x5c8598) t = 0.0f;
            F<float>(v, 0x336 + i * 4) = (float)(((double)t + F<float>(v, 0x34e)) * step + F<float>(v, 0x336 + i * 4));
        }
        if (uint8_t* part = F<uint8_t*>(v, 0x33e + i * 4)) {
            float a;
            if (i) {
                double c = -ExeCos(F<float>(v, 0x346 + i * 4));
                a = (float)((c - ExeCos(F<float>(v, 0x342 + i * 4))) * w + D(0x5cb2d0));
            } else a = (float)(-(ExeCos(F<float>(v, 0x346)) * w) - D(0x5cb2c8));
            SceneObj_SetAngles(part, 0, 0, 0, Bits(a));
            float t = (float)(ExeCos(F<float>(v, 0x346 + i * 4)) * D(0x5c8798));
            if (t < *(const float*)0x5c8598) t = 0.0f;
            F<float>(v, 0x346 + i * 4) = (float)(((double)t + F<float>(v, 0x34e)) * step + F<float>(v, 0x346 + i * 4));
        }
    }
    double pitch = SceneObj_Angle1(v);
    float roll = (float)(pitch - ExeSin((double)F<float>(v, 0x336) * D(0x5c8628)) * D(0x5c8678));
    float yaw = SceneObj_Angle2(v);
    SceneObj_SetAngles(v, 0, Bits(roll), Bits(yaw), 0);
    float bob = (float)(ExeCos((double)F<float>(v, 0x336) - D(0x5c88f0)) * D(0x5c9ee0));
    Obj_MoveBy(v, 0, 0.0f, bob, 0.0f);
    float acc = (float)((double)roll + F<float>(v, 0x37e));
    F<float>(v, 0x37e) = acc;
    if (!(step < *(const float*)0x5c85b4)) step = 1.0f;
    float damp = 0.1f;
    if (ExeFabs(acc) > D(0x5c85d0)) damp = 0.3f;
    F<float>(v, 0x37e) = (float)((D(0x5c8770) - (double)damp * step) * F<float>(v, 0x37e));
}
RT3_REPLACE(0x456210, Vehicle_AnimateLinks);

void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall Building_AttachSound(uint8_t* b, int, int32_t, uint8_t* grid, float x, float y, float z);
void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v);

// 0x45b5f0. Attaches sound `kind` to the object at ground level under its position,
// then turns looping off on its channel (+0x24a).
void __fastcall Obj_AttachGroundSound(uint8_t* o, int, int32_t kind) {
    float x, y, z;
    SceneObj_GetPos(o, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    void* engine = *(void**)0x6d4024;
    float h = (float)Terrain_HeightAt((uint8_t*)Engine_Get1C(engine), 0, x, z);
    Building_AttachSound(o, 0, kind, (uint8_t*)Engine_Get1C(engine), x, h, z);
    if (uint8_t* ch = F<uint8_t*>(o, 0x24a)) Channel_SetLoop(ch, 0, 0);
}
RT3_REPLACE(0x45b5f0, Obj_AttachGroundSound);

struct PtrListNode;
void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y);
void __fastcall Obj_MoveChannels(uint8_t* o);

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

// 0x530720. Moves the object to (x, y, z), marking it moved (+0x20 bit 1) and
// re-filing it in its terrain's 16-cell grid (node +8) when it changes cell (no
// bounds check); its sound channels follow.
RT3_NOINLINE void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z) {
    o[0x20] |= 2;
    if (!F<uint8_t*>(o, 4)) {
        F<float>(o, 0x1e2) = x;
        F<float>(o, 0x1e6) = y;
        F<float>(o, 0x1ea) = z;
        Obj_MoveChannels(o);
        return;
    }
    int32_t ox = Cell16(F<float>(o, 0x1e2)), oz = Cell16(F<float>(o, 0x1ea));
    F<float>(o, 0x1e2) = x;
    F<float>(o, 0x1e6) = y;
    F<float>(o, 0x1ea) = z;
    int32_t cx = Cell16(x), cz = Cell16(z);
    if (ox != cx || oz != cz) {
        if (F<PtrListNode*>(o, 8)) Terrain_Remove1671(F<uint8_t*>(o, 4), 0, F<PtrListNode*>(o, 8), ox, oz);
        F<PtrListNode*>(o, 8) = Terrain_Add1671(F<uint8_t*>(o, 4), 0, o, cx, cz);
    }
    Obj_MoveChannels(o);
}
RT3_REPLACE(0x530720, Obj_MoveTo);

uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6,
                                  int32_t a7, float vol);

// 0x535950. Starts sample `name` in the first free one of the 27 sound slots
// (+0x40c, count +0x408); returns the slot or -1 when all are taken.
int32_t __fastcall Sound_StartInSlot(uint8_t* s, int, const char* name) {
    int32_t i = 0;
    while (F<uint8_t*>(s, 0x40c + i * 4)) {
        if (++i >= 0x1b) return -1;
    }
    uint8_t* ch = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x141);
    if (ch) ch = Channel_Setup(ch, 0, name, s, 0.0f, 0.0f, 0.0f, 0, 1, -1.0f);
    F<uint8_t*>(s, 0x40c + i * 4) = ch;
    ++F<int32_t>(s, 0x408);
    return i;
}
RT3_REPLACE(0x535950, Sound_StartInSlot);

} // namespace rt3
