// Larger game functions, part 7 (batch 205). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Node_GetLink(void* n, int, int32_t first);
int32_t __fastcall Node_TurnsLeft(Collection* nodes, int, uint32_t id);
int32_t __fastcall Train_IsRunning(void*, int, uint32_t id);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);

// Notes that the train took the switch at `node` (+0x1cf) on side `side` (+0x1ce bit 2).
static void TookSwitch(uint8_t* t, uint32_t node, bool side, int32_t* switched) {
    F<uint32_t>(t, 0x1cf) = node;
    t[0x1ce] = (uint8_t)((t[0x1ce] & ~4) | (side ? 4 : 0));
    *switched = 1;
}

// 0x4ab2d0. Next track node for a train (+0x25 = node to avoid) arriving at `cur`
// from `prev` (-1 = none): the link on the far side, else the alternative route at
// +0x253 (either side), else via a switch (+0x218 == 4, branch +0x20e) at the
// previous or current node - only where the switch does not turn (left) the wrong way
// and the far ends agree. Taking a switch records it. The result must be a node the
// train can enter, else -1 (switch record cleared).
int32_t __fastcall Train_NextNode(uint8_t* t, int, int32_t forward, uint32_t prev, uint32_t curId, int32_t* switched) {
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    uint8_t* prevN = prev == 0xffffffff ? nullptr : (uint8_t*)Collection_Get(nodes, 0, prev);
    uint8_t* cur = (uint8_t*)Collection_Get(nodes, 0, curId);
    *switched = 0;
    int32_t side;
    if (F<uint32_t>(cur, 0x206) == prev) side = 1;
    else if (F<uint32_t>(cur, 0x20a) == prev) side = 0;
    else if (F<uint32_t>(cur, 0x20e) == prev) side = (cur[0x201] >> 1) & 1;
    else return -1;
    int32_t n = Node_GetLink(cur, 0, side);
    if (n == F<int32_t>(t, 0x25)) n = -1;
    if (n != -1) goto finish;
    if (Collection_IsValid(nodes, 0, F<uint32_t>(cur, 0x253))) {
        int32_t s = 1;
        n = Node_GetLink(Collection_Get(nodes, 0, F<uint32_t>(cur, 0x253)), 0, 1);
        if (n == -1) {
            s = 0;
            n = Node_GetLink(Collection_Get(nodes, 0, F<uint32_t>(cur, 0x253)), 0, 0);
        }
        if (n != -1) {
            if (n != F<int32_t>(t, 0x25)) TookSwitch(t, F<uint32_t>(cur, 0x253), s != 0, switched);
            goto finish;
        }
    }
    if (!prevN) return n;
    if (prevN[0x218] == 4 && F<uint32_t>(prevN, 0x20e) == curId) {
        int32_t s = forward == 0;
        if (Node_GetLink(prevN, 0, s) != -1 && !Node_TurnsLeft(nodes, 0, F<uint32_t>(prevN, 0x202))) {
            n = Node_GetLink(prevN, 0, s);
            TookSwitch(t, F<uint32_t>(prevN, 0x202), forward == 0, switched);
        }
    }
    if (n != -1) goto finish;
    if (cur[0x218] != 4) return n;
    if (F<uint32_t>(cur, 0x20e) == prev) {
        int32_t s = side == 0;
        if (Node_GetLink(cur, 0, s) != n && Node_TurnsLeft(nodes, 0, F<uint32_t>(cur, 0x202))) {
            n = Node_GetLink(cur, 0, s);
            TookSwitch(t, F<uint32_t>(cur, 0x202), side == 0, switched);
            if (n != -1) goto finish;
        }
    }
    if (cur[0x218] != 4 || F<int32_t>(cur, 0x20e) == -1) return n;
    {
        int32_t s = side == 0;
        if (Node_GetLink(cur, 0, s) == -1) return n;
        uint8_t* b = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(cur, 0x20e));
        int32_t c = F<uint32_t>(b, 0x206) == curId;
        int32_t r1 = Node_GetLink(cur, 0, s);
        if (r1 == Node_GetLink(b, 0, c)) {
            if (Node_GetLink(b, 0, c == 0) != F<int32_t>(t, 0x25) && !Node_TurnsLeft(nodes, 0, F<uint32_t>(cur, 0x202))) {
                n = Node_GetLink(cur, 0, s);
                if (n != -1) {
                    TookSwitch(t, F<uint32_t>(cur, 0x202), side == 0, switched);
                    goto finish;
                }
            }
        }
        int32_t r4 = Node_GetLink(b, 0, c == 0);
        if (Node_GetLink(cur, 0, s) != r4) return n;
        if (!Node_TurnsLeft(nodes, 0, F<uint32_t>(cur, 0x202))) return n;
        n = Node_GetLink(cur, 0, s);
        if (n == -1) return n;
        TookSwitch(t, F<uint32_t>(cur, 0x202), side == 0, switched);
    }
finish:
    if (!Train_IsRunning(t, 0, (uint32_t)n)) {
        *switched = 0;
        F<int32_t>(t, 0x1cf) = -1;
        return -1;
    }
    return n;
}
RT3_REPLACE(0x4ab2d0, Train_NextNode);

int32_t __stdcall Ray_HitsTriangle(float, float, float, float, float, float, float, float, float, float, float, float,
                                   float, float, float);

// 0x52f500. Emits the object's current triangles (mesh +0x18: 0x4c-byte records of
// 3 vertex pointers, 3 normals, 3 uvs; walked through the globals 0xcc3b64 /
// 0xcc3b5c) as 0x20-byte vertices (position, normal, uv) into `out`, adding them to
// *count. When pickable (+0x20 & 1) and given a ray (`dir` from `origin`), each
// triangle is first tested against the ray in model space (inverse transform +0x8e,
// refreshed when +0x20 & 4); after the first hit the rest are copied untested.
// Returns whether the ray hit.
int32_t __fastcall Mesh_EmitPickTriangles(uint8_t* o, int, uint8_t* out, int32_t* count, const float* dir, const float* origin) {
    uint8_t*& tri = hook::Global<uint8_t*>(0xcc3b64);
    int32_t& left = hook::Global<int32_t>(0xcc3b5c);
    tri = F<uint8_t*>(F<uint8_t*>(o, 0x18), 4);
    left = F<int32_t>(F<uint8_t*>(o, 0x18), 0xc);
    if (left <= 0) return 0;
    if (!(o[0x20] & 1)) dir = origin = nullptr;
    *count += left * 3;
    auto emit = [&]() {
        for (int32_t k = 0; k < 3; ++k) {
            memcpy(out, F<uint8_t*>(tri, k * 4), 12);
            memcpy(out + 0xc, tri + 0xc + k * 0xc, 12);
            F<uint32_t>(out, 0x18) = F<uint32_t>(tri, 0x30 + k * 8);
            F<uint32_t>(out, 0x1c) = F<uint32_t>(tri, 0x34 + k * 8);
            out += 0x20;
        }
        tri += 0x4c;
    };
    if (dir) {
        float dx = dir[0], dy = dir[1], dz = dir[2];
        if (o[0x20] & 4) {
            o[0x20] &= ~4;
            float det = 0.0f;
            ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(o + 0x8e, &det, o + 0xce);
            (void)det;
        }
        auto m = [&](int r, int c) { return (double)F<float>(o, 0x8e + r * 16 + c * 4); };
        float ld[3], lo[3];
        ld[0] = (float)((dy * m(1, 0) + dz * m(2, 0)) + dx * m(0, 0));
        ld[1] = (float)((dz * m(2, 1) + dx * m(0, 1)) + dy * m(1, 1));
        ld[2] = (float)((dy * m(1, 2) + dz * m(2, 2)) + dx * m(0, 2));
        double px = (double)origin[0] - F<float>(o, 0x1e2), py = (double)origin[1] - F<float>(o, 0x1e6),
               pz = (double)origin[2] - F<float>(o, 0x1ea);
        lo[0] = (float)((py * m(1, 0) + pz * m(2, 0)) + px * m(0, 0));
        lo[1] = (float)((pz * m(2, 1) + px * m(0, 1)) + py * m(1, 1));
        lo[2] = (float)((py * m(1, 2) + pz * m(2, 2)) + px * m(0, 2));
        do {
            const float* a = F<float*>(tri, 0);
            const float* b = F<float*>(tri, 4);
            const float* c = F<float*>(tri, 8);
            --left;
            if (Ray_HitsTriangle(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], lo[0], lo[1], lo[2], ld[0], ld[1], ld[2])) {
                emit();
                while (left) {
                    --left;
                    emit();
                }
                return 1;
            }
            emit();
        } while (left);
        return 0;
    }
    do {
        --left;
        emit();
    } while (left);
    return 0;
}
RT3_REPLACE(0x52f500, Mesh_EmitPickTriangles);

} // namespace rt3
