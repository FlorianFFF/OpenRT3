// Mixed helpers in blocks 0x41..0x53 (batch 79). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/containers.h"
#include "core/stream.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Model_SetPartFrame(uint8_t* o, int, int32_t part, int32_t frame);
void __fastcall Model_ShowOnlyPart(uint8_t* o, int, int32_t sel);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Obj_StateB9(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
void __fastcall SceneObj_RotateAxis(uint8_t* o, int, float ax, float ay, float az, float angle);

static inline Collection* Coll(uint32_t a) { return hook::Global<Collection*>(a); }
static inline double ExeFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

// 0x41c5f0. Shows the car's cargo load: the cargo type's sprite frame (+0x3a)
// on part 0 when the car type (+0x388) wants it (+0x12e), and model part +0x20.
void __fastcall Car_ShowCargoModel(uint8_t* c) {
    uint32_t id = F<uint32_t>(c, 0x38c);
    if (id) {
        int32_t frame = F<int32_t>(Collection_Get(Coll(0x62ba8c), 0, id), 0x3a);
        if (frame) {
            uint8_t* t = (uint8_t*)Collection_Get(Coll(0x62ba88), 0, F<uint32_t>(c, 0x388));
            if (t[0x12e]) Model_SetPartFrame(c, 0, 0, frame);
        }
    }
    if (!F<uint32_t>(c, 0x38c) || !Collection_Get(Coll(0x62ba8c), 0, F<uint32_t>(c, 0x38c))) return;
    uint8_t* cargo = (uint8_t*)Collection_Get(Coll(0x62ba8c), 0, F<uint32_t>(c, 0x38c));
    if ((int8_t)cargo[0x20] <= 0) return;
    cargo = (uint8_t*)Collection_Get(Coll(0x62ba8c), 0, F<uint32_t>(c, 0x38c));
    Model_ShowOnlyPart(c, 0, (int8_t)cargo[0x20]);
}
RT3_REPLACE(0x41c5f0, Car_ShowCargoModel);

// 0x439be0. Accumulates how far the camera moved since the last snapshot
// (+0x352..: positions +0x4.. and +0x1c..) into +0x34e, capped at 1.
void __fastcall View_AccumulateMotion(uint8_t* o) {
    double t = ExeFabs((double)F<float>(o, 0x366) - F<float>(o, 0x24));
    t = ExeFabs((double)F<float>(o, 0x35e) - F<float>(o, 0x1c)) + t;
    t = ExeFabs((double)F<float>(o, 0x352) - F<float>(o, 0x4)) + t;
    float a = (float)((ExeFabs((double)F<float>(o, 0x35a) - F<float>(o, 0xc)) + t) / *(const double*)0x5c8c48);
    t = ExeFabs((double)F<float>(o, 0x362) - F<float>(o, 0x20));
    float b = (float)(ExeFabs((double)F<float>(o, 0x356) - F<float>(o, 0x8)) + t);
    const float zero = *(const float*)0x5c8598;
    if (b == zero && a == zero) return;
    float v = (float)((double)a / *(const double*)0x5c8798);
    F<uint32_t>(o, 0x352) = F<uint32_t>(o, 0x4);
    F<uint32_t>(o, 0x35e) = F<uint32_t>(o, 0x1c);
    F<uint32_t>(o, 0x356) = F<uint32_t>(o, 0x8);
    F<uint32_t>(o, 0x35a) = F<uint32_t>(o, 0xc);
    F<uint32_t>(o, 0x362) = F<uint32_t>(o, 0x20);
    F<uint32_t>(o, 0x366) = F<uint32_t>(o, 0x24);
    float s = (float)((double)v + F<float>(o, 0x34e));
    F<float>(o, 0x34e) = s;
    if ((double)s > *(const double*)0x5c8770) F<uint32_t>(o, 0x34e) = 0x3f800000;
}
RT3_REPLACE(0x439be0, View_AccumulateMotion);

// 0x468580. Picks the next playlist track (+0x10c of +0x100 entries at +0x108,
// 0x104 bytes, name hash +0x100). The first pick prefers the hashed default
// track; shuffle (+0x104) avoids the last few picks (history 0x6ada9c).
void __fastcall Playlist_Next(uint8_t* p) {
    uint8_t* entries = F<uint8_t*>(p, 0x108);
    uint32_t h = Crc32String((const char*)0x5ce144, 1);
    if (!F<uint8_t*>(p, 0x108)) return;
    if (!F<int32_t>(p, 0x114)) {
        for (int32_t i = 0; i < F<int32_t>(p, 0x100); ++i)
            if (h == F<uint32_t>(entries, 0x100 + i * 0x104)) {
                F<int32_t>(p, 0x10c) = i;
                return;
            }
    }
    int32_t* hist = (int32_t*)0x6ada9c;
    for (;;) {
        if (!F<int32_t>(p, 0x104) || F<int32_t>(p, 0x100) <= 2) {
            F<int32_t>(p, 0x10c) = (F<int32_t>(p, 0x10c) + 1) % F<int32_t>(p, 0x100);
            return;
        }
        int32_t r = Rand(0, F<int32_t>(p, 0x100) - 1);
        int32_t n = F<int32_t>(p, 0x100);
        int32_t k = n > 6 ? 4 : n - 2;
        int32_t i = 0;
        if (k > 0) {
            uint32_t e = F<uint32_t>(F<uint8_t*>(p, 0x108), r * 0x104 + 0x100);
            for (; i < k; ++i)
                if ((uint32_t)hist[i] == e || e == h) break;
        }
        if (i != k) continue;
        int32_t c = k - 2;
        if (c >= 0) do {
            --c;
            hist[c + 2] = hist[c + 1];
        } while (c >= 0);
        hist[0] = F<int32_t>(F<uint8_t*>(p, 0x108), r * 0x104 + 0x100);
        F<int32_t>(p, 0x10c) = r;
        if (F<int32_t>(p, 0x114)) F<int32_t>(p, 0x114)++;
        return;
    }
}
RT3_REPLACE(0x468580, Playlist_Next);

// 0x47fdb0. Records a nearby object (within 0x5c8738 cells of the building +4)
// in the per-kind (Obj_StateB9) list +0x5a4 / count +0x590 as {+0x2a4, distance}.
void __fastcall Station_RecordNearby(uint8_t* s, int, uint8_t* obj) {
    if (!Obj_BuildingStateActive(s)) return;
    uint32_t id = F<uint32_t>(s, 4);
    void* b1 = id ? Collection_Get(Coll(0x62b26c), 0, id) : nullptr;
    void* b2 = F<uint32_t>(s, 4) ? Collection_Get(Coll(0x62b26c), 0, F<uint32_t>(s, 4)) : nullptr;
    float bx = (float)Obj_CellX(b2);
    float by = (float)Obj_CellY(b1);
    float oy = (float)Obj_CellY(obj);
    float ox = (float)Obj_CellX(obj);
    float d = (float)Distance2D(ox, oy, bx, by);
    if (!(d <= *(const float*)0x5c8738)) return;
    int32_t k = Obj_StateB9(obj);
    int32_t cnt = ++F<int32_t>(s, 0x590 + k * 4);
    uint32_t* rec = (uint32_t*)Malloc(cnt * 8);
    if (F<int32_t>(s, 0x590 + k * 4) > 1) {
        uint8_t* old = F<uint8_t*>(s, 0x5a4 + k * 4);
        uint32_t n = F<int32_t>(s, 0x590 + k * 4) * 8 - 8;
        for (uint32_t j = 0; j < n; ++j) ((uint8_t*)rec)[j] = old[j];
        Free(F<void*>(s, 0x5a4 + k * 4));
    }
    rec[F<int32_t>(s, 0x590 + k * 4) * 2 - 2] = F<uint32_t>(obj, 0x2a4);
    rec[F<int32_t>(s, 0x590 + k * 4) * 2 - 1] = *(uint32_t*)&d;
    F<uint32_t*>(s, 0x5a4 + k * 4) = rec;
    F<int32_t>(s, 0x5b8)++;
}
RT3_REPLACE(0x47fdb0, Station_RecordNearby);

// 0x48e3f0. Computes once (flag 1 of +0x24c) whether the tile is blocked (flag 2):
// by a connected track node (+0x202, +0x206) or a nearby region centre.
void __fastcall Tile_UpdateBlocked(uint8_t* o) {
    uint8_t fl = o[0x24c];
    if (fl & 1) return;
    float cx = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / *(const float*)0x5cfcfc);
    o[0x24c] = (uint8_t)((fl & 0xfd) | 1);
    float cy = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / *(const float*)0x5cfcfc);
    for (int32_t i = 0; i < 3; ++i) {
        uint32_t id = i == 0 ? F<uint32_t>(o, 0x202) : F<uint32_t>(o, 0x206);
        if (id == 0xffffffff) continue;
        uint8_t* n = (uint8_t*)Collection_Get(Coll(0x6cfca8), 0, id);
        if (!n) continue;
        if (F<int32_t>(n, 0x226) || F<int32_t>(n, 0x20e) != -1 || F<int32_t>(n, 0x222) != -1) o[0x24c] |= 2;
    }
    if (o[0x24c] & 2) return;
    for (int32_t j = 0; j < Collection_Count(Coll(0x62bae0)); ++j) {
        Collection* c = Coll(0x62bae0);
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        float ry = (float)Obj_CellY(r);
        float rx = (float)Obj_CellX(r);
        float d = (float)DistanceSq2D(cx, cy, rx, ry);
        if (d < *(const float*)0x5cfdd4) {
            o[0x24c] |= 2;
            return;
        }
    }
}
RT3_REPLACE(0x48e3f0, Tile_UpdateBlocked);

// 0x4f5cf0. Sets the water preset idx (+0x98) from the table for the map style
// (+0x8c / +0x90) and passes the level (+0.5) to the renderer.
void __fastcall Map_SetWaterPreset(uint8_t* o, int, int32_t idx) {
    F<int32_t>(o, 0x98) = idx;
    const double half = *(const double*)0x5c85b8;
    float v;
    if (F<int32_t>(o, 0x8c) == 1) {
        v = (float)((double)*(const float*)0x6227b4 + half);
    } else {
        int32_t style = F<int32_t>(o, 0x90);
        uint32_t table;
        if (style == 4) table = 0x6227e4;
        else if (style == 6) table = 0x622800;
        else if (style == 7) table = 0x62281c;
        else if (style == 3 || style == 2) table = 0x6227c8;
        else table = 0x6227ac;
        v = (float)((double)((const float*)table)[idx] + half);
    }
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, v);
}
RT3_REPLACE(0x4f5cf0, Map_SetWaterPreset);

// 0x51ea80. Resets the engine state fields.
int32_t __fastcall Engine_ResetState_Impl(uint8_t* e) {
    e[0x114253] = 0xff;
    e[0x11422e] = 0xff;
    e[0x114250] = 0xff;
    e[0x114251] = 0xff;
    F<int32_t>(e, 0x114212) = 0;
    e[0x114255] = 1;
    F<int32_t>(e, 0xc) = 0;
    F<int32_t>(e, 0x114243) = 0;
    F<int32_t>(e, 0x114247) = 0;
    F<int32_t>(e, 0x11427e) = 0;
    F<int32_t>(e, 0x114282) = 0;
    F<int32_t>(e, 0x1141f5) = 0;
    F<int32_t>(e, 0x1141f9) = 0;
    F<int32_t>(e, 0x1141fd) = 0;
    F<int32_t>(e, 0x4d) = 0;
    F<int32_t>(e, 0x51) = 0;
    F<int32_t>(e, 0x28) = 0;
    e[0x56] = 0;
    F<int32_t>(e, 0x58) = 0;
    F<int32_t>(e, 0x4) = 0;
    F<int32_t>(e, 0x0) = 0;
    F<int32_t>(e, 0x18) = 0;
    F<int32_t>(e, 0x1c) = 0;
    F<int32_t>(e, 0x8) = 0;
    F<int32_t>(e, 0x11420a) = 0;
    F<int32_t>(e, 0x11421a) = 1;
    F<int32_t>(e, 0x11420e) = 0x5c;
    F<int32_t>(e, 0x114216) = 0;
    F<int32_t>(e, 0x114233) = 0;
    e[0x11422c] = 0;
    e[0x114252] = 0;
    e[0x114231] = 0;
    e[0x11422d] = 0;
    e[0x114230] = 0;
    e[0x11422f] = 0;
    e[0x114254] = 1;
    e[0x114232] = 1;
    F<int32_t>(e, 0x114237) = 0;
    F<int32_t>(e, 0x114228) = 2;
    e[0x57] = 0;
    F<int32_t>(e, 0x1136ad) = 0;
    F<int32_t>(e, 0x1136a9) = 0;
    F<int32_t>(e, 0x2c) = 0;
    F<int32_t>(e, 0x114276) = 0;
    F<int32_t>(e, 0x11425a) = 0;
    F<int32_t>(e, 0x11425e) = 0;
    F<int32_t>(e, 0x114262) = 0;
    F<int32_t>(e, 0x114266) = 0;
    F<int32_t>(e, 0x11426a) = 0;
    F<int32_t>(e, 0x11426e) = 0;
    F<int32_t>(e, 0x114272) = 0;
    F<int32_t>(e, 0x11427a) = 0;
    F<int32_t>(e, 0x114256) = 0;
    e[0x114227] = 0x11;
    return 0;
}
// 0x51ffa5 keeps using ECX (the engine) after the call.
__declspec(naked) int32_t Engine_ResetState() { RT3_KEEP_ECX_EDX(Engine_ResetState_Impl, 0) }
RT3_REPLACE(0x51ea80, Engine_ResetState);

// 0x52ec50. Saves flag bits 5 and 6 of +0x20 as two bytes.
void __fastcall SceneObj_SaveFlags(uint8_t* o, int, Stream s) {
    uint8_t b = (o[0x20] >> 5) & 1;
    StreamWrite(s, &b, 1, 0);
    b = (o[0x20] >> 6) & 1;
    StreamWrite(s, &b, 1, 0);
}
RT3_REPLACE(0x52ec50, SceneObj_SaveFlags);

// 0x52ffb0. Scales the object's quaternion rotation (+0x1ce): rotates again
// about its own axis (scaled by 0x5c9b08) by its angle times f (D3DX).
void __fastcall SceneObj_ScaleRotation(uint8_t* o, int, float f) {
    float angle, axis[3];
    ((void*(__stdcall*)(const void*, float*, float*))0x56e556)(o + 0x1ce, axis, &angle);
    const float k = *(const float*)0x5c9b08;
    float ax = (float)((double)axis[0] * k);
    axis[0] = ax;
    axis[1] = (float)((double)axis[1] * k);
    axis[2] = (float)((double)axis[2] * k);
    float a = (float)((double)angle * f);
    SceneObj_RotateAxis(o, 0, ax, axis[1], axis[2], a);
}
RT3_REPLACE(0x52ffb0, SceneObj_ScaleRotation);

} // namespace rt3
