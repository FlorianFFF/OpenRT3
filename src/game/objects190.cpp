// Object groups, coloured triangles and particle motion in blocks 0x53..0x56 (batch 196).
// See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double Fl(uint32_t a) { return *(const float*)a; }

void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);

// 0x539580. Moves every object of the group (list +8) to (x, y, z).
void __fastcall Group_MoveAllTo(uint8_t* g, int, float x, float y, float z) {
    PtrList* l = F<PtrList*>(g, 8);
    if (!l) return;
    PtrList_Rewind(l);
    for (uint8_t* o = (uint8_t*)PtrList_Next(l); o; o = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8))) Obj_MoveTo(o, 0, x, y, z);
}
RT3_REPLACE(0x539580, Group_MoveAllTo);

// 0x5395d0. Adds `o` to the group (creating its list +8) and places it at the
// group's point (+0xc, +0x10) at `lift` above the mean of the corner heights
// (+0x18 / +0x24 / +0x30 / +0x3c) less one.
RT3_NOINLINE void __fastcall Group_Add(uint8_t* g, int, uint8_t* o, float lift) {
    if (!F<PtrList*>(g, 8)) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(g, 8) = l ? PtrList_Init(l) : nullptr;
    }
    PtrList_Append(F<PtrList*>(g, 8), 0, o);
    double sum = (double)F<float>(g, 0x3c) + F<float>(g, 0x30);
    sum = sum + F<float>(g, 0x24);
    sum = sum + F<float>(g, 0x18);
    float y = (float)(sum * *(const double*)0x5c8758 - *(const double*)0x5c8770 + lift);
    Obj_MoveTo(o, 0, F<float>(g, 0xc), y, F<float>(g, 0x10));
}
RT3_REPLACE(0x5395d0, Group_Add);

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

// Colour `col` with its RGB scaled by `fade` / 2 (clamped to 0..1); alpha kept.
static uint32_t Shade(uint32_t col, float fade) {
    float rf = (float)(int32_t)((col >> 16) & 0xff), gf = (float)(int32_t)((col >> 8) & 0xff);
    double bf = (double)(int32_t)(col & 0xff);
    double k = (double)fade * *(const double*)0x5c8628;
    if (k > *(const double*)0x5c8770) k = Fl(0x5c85b4);
    else if (k < Fl(0x5c8598)) k = Fl(0x5c8598);
    uint32_t c = (uint32_t)Ftol((double)rf * k);
    c = (c << 8) | (uint32_t)Ftol((double)gf * k);
    c = (c << 8) | (uint32_t)Ftol(bf * k);
    return c | (col & 0xff000000);
}

// 0x5648c0. Expands a screen triangle (corners +1/+5, +9/+0xd, +0x11/+0x15, depth
// +0x19, colours +0x1d/+0x21/+0x25 faded by +0x29/+0x2d/+0x31 / 2, texture
// coordinates +0x45..+0x59) into 3 vertices of x, y, z, colour, u, v.
void __stdcall Tri_ToVertices(const uint8_t* s, float* out, int32_t* count) {
    uint8_t* p = (uint8_t*)s;
    uint32_t* v = (uint32_t*)out;
    uint32_t z = F<uint32_t>(p, 0x19);
    for (int32_t i = 0; i < 3; ++i) {
        v[i * 6] = F<uint32_t>(p, 1 + i * 8);
        v[i * 6 + 1] = F<uint32_t>(p, 5 + i * 8);
        v[i * 6 + 2] = z;
        v[i * 6 + 3] = Shade(F<uint32_t>(p, 0x1d + i * 4), F<float>(p, 0x29 + i * 4));
        v[i * 6 + 4] = F<uint32_t>(p, 0x45 + i * 8);
        v[i * 6 + 5] = F<uint32_t>(p, 0x49 + i * 8);
    }
    *count = 3;
}
RT3_REPLACE(0x5648c0, Tri_ToVertices);

// 0x565f40. Advances a particle by `dt`: counts its life (+0x48, of +0x4c) down,
// returning false once spent; moves it by its velocity (+0xc..+0x14) scaled by the
// remaining / elapsed life fraction (flags +0x5c 0x100 / 0x10000000), blending
// towards the end velocity (+0x18..+0x20) over the late part of its life (0x40000000)
// or drifting with the wind (0x625ba8.., growing +0x24 toward its limits) unless
// paused; spins +0x40 (0x20080000), applies gravity (0x800, +0x6c) and grows its
// size +0x44 (by +0x64, at least 0.01).
int32_t __fastcall Particle_Advance(uint8_t* p, int, float dt) {
    if (!(Fl(0x5c8598) < dt) && !(dt != dt)) return 1;
    int32_t n = Ftol((double)F<int32_t>(p, 0x48) - dt);
    F<int32_t>(p, 0x48) = n;
    if (n <= 0) {
        F<int32_t>(p, 0x48) = 0;
        return 0;
    }
    uint32_t flags = F<uint32_t>(p, 0x5c);
    double s = dt;
    if (flags & 0x100) s = (double)n / F<int32_t>(p, 0x4c) * dt;
    else if (flags & 0x10000000) s = (Fl(0x5c85b4) - (double)n / F<int32_t>(p, 0x4c)) * dt;
    if (flags & 0x20080000) F<float>(p, 0x40) = (float)((double)F<float>(p, 0x3c) + F<float>(p, 0x40));
    bool done = false;
    if (flags & 0x40000000) {
        double a = (double)n / F<int32_t>(p, 0x4c);
        double b = Fl(0x5c85b4) - F<float>(p, 0x28);
        if (a < b || a == b) {
            double t = a / b, u = Fl(0x5c85b4) - t;
            F<float>(p, 0) = (float)((u * F<float>(p, 0xc) + t * F<float>(p, 0x18)) * s + F<float>(p, 0));
            F<float>(p, 4) = (float)((t * F<float>(p, 0x20) + u * F<float>(p, 0x10)) * s + F<float>(p, 4));
            F<float>(p, 8) = (float)((u * F<float>(p, 0x14) + t * F<float>(p, 0x1c)) * s + F<float>(p, 8));
            done = true;
        }
    }
    if (!done) {
        float a = (float)((double)n / F<int32_t>(p, 0x4c));
        float b = (float)(Fl(0x5c85b4) - F<float>(p, 0x28));
        if ((a < b || a == b) && !hook::Global<int32_t>(0xd93998) && !hook::Global<int32_t>(0xd97af4)) {
            double w = Fl(0x5c85b4) - (double)a / b;
            const float wind = *(const float*)0x625ba8;
            F<float>(p, 0x18) = (float)((double)*(const float*)0x625bb8 * F<float>(p, 0x24));
            F<float>(p, 0x1c) = (float)((double)*(const float*)0x625bbc * F<float>(p, 0x24));
            F<float>(p, 0x20) = (float)((double)*(const float*)0x625bc0 * F<float>(p, 0x24));
            F<float>(p, 0) = (float)(((double)wind * F<float>(p, 0x18) * w + F<float>(p, 0xc)) * s + F<float>(p, 0));
            F<float>(p, 4) = (float)(((double)wind * w * F<float>(p, 0x1c) + F<float>(p, 0x10)) * s + F<float>(p, 4));
            F<float>(p, 8) = (float)(((double)wind * F<float>(p, 0x20) * w + F<float>(p, 0x14)) * s + F<float>(p, 8));
            const float gust = *(const float*)0x625bb0;
            if ((gust > Fl(0x5c8598) && F<float>(p, 0x24) < *(const float*)0x625bb4) ||
                (gust < Fl(0x5c8598) && F<float>(p, 0x24) > *(const float*)0xd97af0))
                F<float>(p, 0x24) = (float)((double)gust * Fl(0x5cadc0) + F<float>(p, 0x24));
        } else {
            F<float>(p, 0) = (float)(s * F<float>(p, 0xc) + F<float>(p, 0));
            F<float>(p, 4) = (float)(s * F<float>(p, 0x10) + F<float>(p, 4));
            F<float>(p, 8) = (float)(s * F<float>(p, 0x14) + F<float>(p, 8));
        }
    }
    if (flags & 0x800) F<float>(p, 0x10) = (float)((double)F<float>(p, 0x10) - (double)dt * F<float>(p, 0x6c));
    double size = (double)dt * F<float>(p, 0x64) + F<float>(p, 0x44);
    F<float>(p, 0x44) = (float)size;
    if (size < Fl(0x5cb5c0)) F<uint32_t>(p, 0x44) = 0x3c23d70a;
    return 1;
}
RT3_REPLACE(0x565f40, Particle_Advance);

int32_t ClockMs();
void** __fastcall SList_Unlink(void** l, int, void** prev, void** node);

// pow(base, 1.0) through the CRT's x87 helper (as the original computes it).
static inline double PowOne(double base) {
    double r;
    __asm {
        fld base
        fld1
        mov eax, 0x5a1ba0
        call eax
        fstp r
    }
    return r;
}

// 0x555120. Advances the emitter's particles by `dt` once per frame (frame counter
// 0x6d4030 +0x2c, kept in +0x1f9): an emitter attached to an object (+0xc; trails
// +0xac & 0x19) follows it (unless frozen, 0xd939a0) and drags its particles by the
// move (spread over life with 0x10 / 1; vertical damped without 1); flip-book
// sprites (+0x118 frames, +0x124 per row, cell +0x110 x +0x114 from +0x11c/+0x120,
// rate +0x128) step on; spent particles (unless immortal, 0x2000000) move from the
// live list (+4) to the free list (+0). 0 when skipped.
int32_t __fastcall Emitter_Update(uint8_t* e, int, int32_t dt) {
    if (dt < 0) return 0;
    int32_t frame = F<int32_t>(hook::Global<uint8_t*>(0x6d4030), 0x2c);
    if (F<int32_t>(e, 0x1f9) == frame) return 0;
    uint32_t trail = F<uint32_t>(e, 0xac) & 0x19;
    float mx = 0.0f, my = 0.0f, mz = 0.0f;
    F<int32_t>(e, 0x1f9) = frame;
    if (trail && F<uint8_t*>(e, 0xc)) {
        if (!hook::Global<int32_t>(0xd939a0)) {
            for (int32_t k = 0; k < 3; ++k) F<uint32_t>(e, 0x20 + k * 4) = F<uint32_t>(e, 0x14 + k * 4);
            for (int32_t k = 0; k < 3; ++k) F<uint32_t>(e, 0x14 + k * 4) = F<uint32_t>(F<uint8_t*>(e, 0xc), 0x1e2 + k * 4);
            uint8_t* g = hook::Global<uint8_t*>(0xd939a4);
            if ((F<uint32_t>(e, 0xac) & 0x800000) && g) {
                for (int32_t k = 0; k < 3; ++k) F<uint32_t>(g, 0x94 + k * 4) = F<uint32_t>(e, 0x14 + k * 4);
                for (int32_t k = 0; k < 3; ++k) F<uint32_t>(g, 0x88 + k * 4) = F<uint32_t>(e, 0x20 + k * 4);
            }
        }
        mx = (float)((double)F<float>(e, 0x14) - F<float>(e, 0x20));
        float dy = (float)((double)F<float>(e, 0x18) - F<float>(e, 0x24));
        mz = (float)((double)F<float>(e, 0x1c) - F<float>(e, 0x28));
        my = dy;
        if (!(e[0xac] & 1)) my = (float)((double)dy * Fl(0x5d006c));
    }
    int32_t now = ClockMs();
    void** prev = nullptr;
    void** node = *F<void***>(e, 4);
    if (!node) return 1;
    float fdt = (float)dt;
    while (node) {
        uint8_t* p = (uint8_t*)node[1];
        if (trail) {
            float px = F<float>(p, 0), py = F<float>(p, 4), pz = F<float>(p, 8);
            uint32_t fl = F<uint32_t>(e, 0xac);
            if (((fl & 0x10) && dt > 0) || (fl & 1)) {
                F<int32_t>(e, 0x2c) = 1;
                double m = PowOne((double)F<int32_t>(p, 0x48) / F<int32_t>(p, 0x4c));
                float ox = (float)(mx * m), oy = (float)(my * m);
                double oz = m * mz;
                double nx = (double)ox + px, ny = (double)oy + py;
                F<float>(p, 8) = (float)(oz + pz);
                F<float>(p, 0) = (float)nx;
                F<float>(p, 4) = (float)ny;
            } else {
                double nx = (double)px + mx, ny = (double)py + my;
                F<float>(p, 8) = (float)((double)pz + mz);
                F<float>(p, 0) = (float)nx;
                F<float>(p, 4) = (float)ny;
            }
        }
        int32_t frames = F<int32_t>(e, 0x118);
        if (frames > 1) {
            uint8_t* q = (uint8_t*)node[1];
            if (1000 / F<int32_t>(e, 0x128) < now - F<int32_t>(q, 0x58)) {
                int32_t fr = F<int32_t>(q, 0x54) % frames;
                int32_t col = fr % F<int32_t>(e, 0x124), row = fr / F<int32_t>(e, 0x124);
                const double k = Fl(0x5ddcb4);
                double u0 = (double)(col * F<int32_t>(e, 0x110) + F<int32_t>(e, 0x11c)) * k;
                double v0 = (double)(row * F<int32_t>(e, 0x114) + F<int32_t>(e, 0x120)) * k;
                double u1 = (double)F<int32_t>(e, 0x110) * k + u0;
                double v1 = (double)F<int32_t>(e, 0x114) * k + v0;
                F<float>(q, 0x2c) = (float)u0;
                F<float>(q, 0x30) = (float)v1;
                F<float>(q, 0x34) = (float)u1;
                F<float>(q, 0x38) = (float)v0;
                F<int32_t>((uint8_t*)node[1], 0x58) = now;
                F<int32_t>((uint8_t*)node[1], 0x54) = fr + 1;
            }
        }
        if (!Particle_Advance((uint8_t*)node[1], 0, fdt) && !(F<uint32_t>(e, 0xac) & 0x2000000)) {
            void** next = (void**)node[0];
            if (prev) SList_Unlink(F<void**>(e, 4), 0, prev, node);
            else {
                void** l = F<void**>(e, 4);
                if (void** h = (void**)l[0]) {
                    void** n2 = (void**)h[0];
                    bool last = h == (void**)l[1];
                    l[0] = n2;
                    if (last) l[1] = n2;
                    --*(int32_t*)&l[2];
                }
            }
            void** fl = F<void**>(e, 0);
            node[0] = fl[0];
            fl[0] = node;
            ++*(int32_t*)&fl[2];
            if (!fl[1]) fl[1] = node;
            node = next;
        } else {
            prev = node;
            node = (void**)node[0];
        }
    }
    return 1;
}
RT3_REPLACE(0x555120, Emitter_Update);

void __fastcall Vec_Cross(float* x, float* y, float ax, float ay, float az, float bx, float by, float bz, float* z);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
static inline double Sqrt87(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

// 0x553ba0. Emits a particle (+8) of the batch as a camera-facing quad (6 vertices;
// corner offsets 0xd97abc.. x 10 x size +0x14 x scale +0x25) at its position (less
// the batch origin +0x29) in the object's frame (+0 -> +0xce), and unless it is a
// point (+4 == 1) a streak along its velocity (+0x14.., width +0xc, length factor
// +0x10, faded tail). Vertices are x, y, z, colour (+8), u, v; *count += 6 each.
void __fastcall Particle_EmitQuads(uint8_t* pd, int, float* out, int32_t* count) {
    uint8_t* part = F<uint8_t*>(pd, 8);
    double size = (double)F<float>(part, 0x14) * F<float>(pd, 0x25);
    if (!(size > Fl(0x5c8598)) && size == size) return;
    const float* pos = F<float*>(part, 0);
    double x = (double)pos[0] - F<float>(pd, 0x29), y = (double)pos[1] - F<float>(pd, 0x2d),
           z = (double)pos[2] - F<float>(pd, 0x31);
    float hs = (float)(size * Fl(0x5c85f8));
    const float* m = (const float*)(F<uint8_t*>(pd, 0) + 0xce);
    float wx = (float)((x * m[0] + z * m[8]) + y * m[4] + m[12]);
    float wy = (float)((x * m[1] + z * m[9]) + y * m[5] + m[13]);
    float wz = (float)((x * m[2] + z * m[10]) + y * m[6] + m[14]);
    *count += 6;
    uint32_t col = F<uint32_t>(part, 8);
    auto g = [](uint32_t a) { return (double)*(const float*)a; };
    auto put = [&](float* v, double cx, double cy, double cz, uint32_t c, uint32_t u, uint32_t w) {
        v[0] = (float)cx;
        v[1] = (float)cy;
        v[2] = (float)cz;
        ((uint32_t*)v)[3] = c;
        ((uint32_t*)v)[4] = u;
        ((uint32_t*)v)[5] = w;
    };
    auto corner = [&](float* v, uint32_t ax, uint32_t ay, uint32_t az, uint32_t u, uint32_t w) {
        put(v, g(ax) * hs + wx, g(ay) * hs + wy, g(az) * hs + wz, col, u, w);
    };
    corner(out, 0xd97ae0, 0xd97ac4, 0xd97ae8, 0, 0x3f000000);
    corner(out + 6, 0xd97ad4, 0xd97acc, 0xd97ad8, 0x3f800000, 0x3f000000);
    corner(out + 12, 0xd97ac0, 0xd97ae4, 0xd97abc, 0, 0x3f800000);
    corner(out + 18, 0xd97ad4, 0xd97acc, 0xd97ad8, 0x3f800000, 0x3f000000);
    corner(out + 24, 0xd97ad0, 0xd97adc, 0xd97ac8, 0x3f800000, 0x3f800000);
    corner(out + 30, 0xd97ac0, 0xd97ae4, 0xd97abc, 0, 0x3f800000);
    if (F<int32_t>(F<uint8_t*>(pd, 8), 4) == 1) return;
    double vx = F<float>(pd, 0x14), vy = F<float>(pd, 0x18), vz = F<float>(pd, 0x1c);
    float rx = (float)((vx * m[0] + vz * m[8]) + vy * m[4]);
    float ry = (float)((vx * m[1] + vz * m[9]) + vy * m[5]);
    float rz = (float)((vx * m[2] + vz * m[10]) + vy * m[6]);
    float cam[3];
    Obj_GetXYZ1c(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, (uint32_t*)&cam[0], (uint32_t*)&cam[1],
                 (uint32_t*)&cam[2]);
    float dx = (float)((double)cam[0] - wx), dy = (float)((double)cam[1] - wy), dz = (float)((double)cam[2] - wz);
    float cx, cy, cz;
    Vec_Cross(&cx, &cy, rx, ry, rz, dx, dy, dz, &cz);
    double inv = *(const double*)0x5c8770 / Sqrt87((double)cx * cx + (double)cy * cy + (double)cz * cz);
    double k = (double)F<float>(pd, 0xc) * F<float>(pd, 0x25) * inv;
    *count += 6;
    double px = cx * k;
    float py = (float)(k * cy);
    float pz = (float)(cz * k);
    float hx = (float)((double)rx * F<float>(pd, 0x25) + wx);
    float hy = (float)((double)ry * F<float>(pd, 0x25) + wy);
    float hz = (float)((double)rz * F<float>(pd, 0x25) + wz);
    uint32_t half = ((col >> 1) & 0x7f000000) | (col & 0xffffff);
    float* v = out + 36;
    put(v, wx, wy, wz, col, 0, 0x3e800000);
    put(v + 6, hx + px, (double)hy + py, (double)hz + pz, col, 0x3f7d70a4, 0);
    put(v + 12, hx - px, (double)hy - py, (double)hz - pz, col, 0x3f7d70a4, 0x3f000000);
    double qx = px * F<float>(pd, 0x10);
    double qy = (double)py * F<float>(pd, 0x10);
    float qz = (float)((double)pz * F<float>(pd, 0x10));
    put(v + 18, wx, wy, wz, half, 0, 0x3e800000);
    put(v + 24, hx + qx, hy + qy, (double)hz + qz, half, 0x3f7d70a4, 0);
    put(v + 30, hx - qx, hy - qy, (double)hz - qz, half, 0x3f7d70a4, 0x3f000000);
}
RT3_REPLACE(0x553ba0, Particle_EmitQuads);

void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall PtrList_Clear(PtrList* l);

// 0x53a5d0. Puts `o` into the group: the first one is added normally; later
// ones replace the group's object, taking over its angles and position (the
// old one is deleted).
void __fastcall Group_SetObject(uint8_t* g, int, uint8_t* o) {
    if (!F<PtrList*>(g, 8)) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(g, 8) = l ? PtrList_Init(l) : nullptr;
        Group_Add(g, 0, o, 0.0f);
        return;
    }
    PtrList_Rewind(F<PtrList*>(g, 8));
    uint32_t a = 0, b = 0, c = 0;
    uint8_t* old = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8));
    PtrList_Clear(F<PtrList*>(g, 8));
    PtrList_Rewind(F<PtrList*>(g, 8));
    SceneObj_GetAngles(old, 0, &a, &b, &c);
    SceneObj_SetAngles(o, 0, a, b, c);
    SceneObj_GetPos(old, 0, &a, &b, &c);
    if (old) {
        SceneObj_Destructor(old);
        ((void(__cdecl*)(void*))0x53b080)(old);
    }
    // The position goes on as raw bits (the original pushes the dwords).
    ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t))&Obj_MoveTo)(o, 0, a, b, c);
    PtrList_Append(F<PtrList*>(g, 8), 0, o);
}
RT3_REPLACE(0x53a5d0, Group_SetObject);

} // namespace rt3
