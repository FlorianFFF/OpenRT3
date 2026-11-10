// Byte-stream helpers of the save-file packer (block 0x56b..0x56c) and the CPU
// feature probes. The stream helpers take their refill / flush callback in a
// register (ESI, EBX or EDI) and the coder state in EDI, so each has a naked entry
// that passes those on to the C++ body. Callbacks are __fastcall(buffer, count).
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>
#include <intrin.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
typedef int32_t(__fastcall* StreamFn)(uint8_t* buf, int32_t n);

// Output stream A: 0x8000-byte buffer *0xda7dd8, fill 0xda7dd4, flushed as 0xd97dc8.
static int32_t PutA(uint8_t b, StreamFn flush) {
    int32_t pos = hook::Global<int32_t>(0xda7dd4);
    hook::Global<uint8_t*>(0xda7dd8)[pos] = b;
    ++pos;
    hook::Global<int32_t>(0xda7dd4) = pos;
    int32_t r = 1;
    if (pos == 0x8000 && flush) {
        r = flush((uint8_t*)0xd97dc8, pos);
        hook::Global<int32_t>(0xda7dd4) = 0;
    }
    return r;
}

// 0x56b940 body.
static int32_t __cdecl StreamA_PutImpl(uint32_t b, StreamFn flush) { return PutA((uint8_t)b, flush); }
// 0x56b940. Appends one byte to stream A (flush callback in ESI).
__declspec(naked) void StreamA_Put() {
    __asm {
        mov eax, [esp + 4]
        push esi
        push eax
        call StreamA_PutImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56b940, StreamA_Put);

// 0x56c320 body: the same for stream B (buffer *0xdb7dec, fill 0xdafde4, flushed as 0xda7de0).
static int32_t __cdecl StreamB_PutImpl(uint32_t b, StreamFn flush) {
    int32_t pos = hook::Global<int32_t>(0xdafde4);
    hook::Global<uint8_t*>(0xdb7dec)[pos] = (uint8_t)b;
    ++pos;
    hook::Global<int32_t>(0xdafde4) = pos;
    int32_t r = 1;
    if (pos == 0x8000 && flush) {
        r = flush((uint8_t*)0xda7de0, pos);
        hook::Global<int32_t>(0xdafde4) = 0;
    }
    return r;
}
// 0x56c320. Appends one byte to stream B (flush callback in ESI).
__declspec(naked) void StreamB_Put() {
    __asm {
        mov eax, [esp + 4]
        push esi
        push eax
        call StreamB_PutImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56c320, StreamB_Put);

// 0x56bbb0 body. Writes the coder's pending bytes (+0x3c, count +0x18) to stream A;
// returns what the original leaves in EAX.
static int32_t __cdecl StreamA_PutPendingImpl(StreamFn flush, uint8_t* c, int32_t eaxIn) {
    if (!flush) return eaxIn;
    int32_t n = F<int32_t>(c, 0x18);
    if (n <= 0) return n;
    int32_t pos = 0;
    for (int32_t i = 0; i < F<int32_t>(c, 0x18); ++i) {
        pos = hook::Global<int32_t>(0xda7dd4);
        hook::Global<uint8_t*>(0xda7dd8)[pos] = c[0x3c + i];
        ++pos;
        hook::Global<int32_t>(0xda7dd4) = pos;
        if (pos == 0x8000) {
            flush((uint8_t*)0xd97dc8, pos);
            pos = 0;
            hook::Global<int32_t>(0xda7dd4) = 0;
        }
    }
    return pos;
}
// 0x56bbb0. Flush callback in EBX, coder in EDI.
__declspec(naked) void StreamA_PutPending() {
    __asm {
        push eax
        push edi
        push ebx
        call StreamA_PutPendingImpl
        add esp, 12
        ret
    }
}
RT3_REPLACE(0x56bbb0, StreamA_PutPending);

// 0x56b8f0 body. Reads one byte from input stream A (pointer 0xda7dd0, left
// 0xd9fdc8), refilling 0x8000 bytes at 0xd9fdd0 through the callback; -1 at the end.
static int32_t __cdecl StreamA_GetImpl(StreamFn refill) {
    int32_t left = hook::Global<int32_t>(0xd9fdc8);
    if (left <= 0) {
        if (!refill) return -1;
        hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fdd0;
        left = refill((uint8_t*)0xd9fdd0, 0x8000);
        if (left == -1) {
            hook::Global<int32_t>(0xd9fdc8) = 0;
            return -1;
        }
    }
    uint8_t* p = hook::Global<uint8_t*>(0xda7dd0);
    int32_t b = *p;
    hook::Global<int32_t>(0xd9fdc8) = left - 1;
    hook::Global<uint8_t*>(0xda7dd0) = p + 1;
    return b;
}
// 0x56b8f0. Refill callback in ESI.
__declspec(naked) void StreamA_Get() {
    __asm {
        push esi
        call StreamA_GetImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x56b8f0, StreamA_Get);

// 0x56c370 body. Reads up to n bytes from input stream B (pointer 0xdafde0, left
// 0xdb7de8, refills at 0xdafde8) into dst; the count read.
static int32_t __cdecl StreamB_ReadImpl(int32_t n, StreamFn refill, uint8_t* dst) {
    int32_t i = 0;
    while (i < n) {
        int32_t left = hook::Global<int32_t>(0xdb7de8);
        if (left <= 0) {
            if (!refill) return i;
            hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafde8;
            left = refill((uint8_t*)0xdafde8, 0x8000);
            if (left == -1) {
                hook::Global<int32_t>(0xdb7de8) = 0;
                return i;
            }
        }
        uint8_t* p = hook::Global<uint8_t*>(0xdafde0);
        uint32_t b = *p;
        hook::Global<uint8_t*>(0xdafde0) = p + 1;
        hook::Global<int32_t>(0xdb7de8) = left - 1;
        if (b == 0xffffffff) return i;
        dst[i++] = (uint8_t)b;
    }
    return i;
}
// 0x56c370. Count in EBX, refill callback in EDI, destination on the stack.
__declspec(naked) void StreamB_Read() {
    __asm {
        push [esp + 4]
        push edi
        push ebx
        call StreamB_ReadImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x56c370, StreamB_Read);

// 0x55ccc0. Whether the CPUID instruction runs.
int32_t Cpu_HasCpuid() {
    __try {
        __asm {
            xor eax, eax
            cpuid
        }
    } __except (1) {
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x55ccc0, Cpu_HasCpuid);

// 0x55cd40. Whether an instruction of feature kind runs without faulting: 1 MMX,
// 2 SSE, 4 SSE2, 8 3DNow!; other kinds are taken as present.
int32_t __stdcall Cpu_ProbeFeature(int32_t kind) {
    __try {
        switch (kind) {
        case 1:
            __asm {
                pxor mm0, mm0
                emms
            }
            break;
        case 2: __asm { xorps xmm0, xmm0 } break;
        case 4: __asm { xorpd xmm0, xmm0 } break;
        case 8:
            __asm {
                _emit 0x0f
                _emit 0x0f
                _emit 0xc0
                _emit 0x96
                emms
            }
            break;
        }
    } __except (1) {
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x55cd40, Cpu_ProbeFeature);

// 0x56bb10 body. Deletes node p from the packer's binary search tree (state +0x24
// parent, +0x28 left, +0x2c right, +8 the nil index), as in LZSS DeleteNode.
static void __cdecl Tree_DeleteNodeImpl(int32_t p, uint8_t* st) {
    int32_t* parent = F<int32_t*>(st, 0x24);
    int32_t* left = F<int32_t*>(st, 0x28);
    int32_t* right = F<int32_t*>(st, 0x2c);
    int32_t nil = F<int32_t>(st, 8);
    if (parent[p] == nil) return;
    int32_t q;
    if (right[p] == nil) q = left[p];
    else if (left[p] == nil) q = right[p];
    else {
        q = left[p];
        if (right[q] != nil) {
            do q = right[q];
            while (right[q] != nil);
            right[parent[q]] = left[q];
            parent[left[q]] = parent[q];
            left[q] = left[p];
            parent[left[p]] = q;
        }
        right[q] = right[p];
        parent[right[p]] = q;
    }
    parent[q] = parent[p];
    if (right[parent[p]] == p) right[parent[p]] = q;
    else left[parent[p]] = q;
    parent[p] = F<int32_t>(st, 8);
}
// 0x56bb10. Node in EDX, state on the stack.
__declspec(naked) void Tree_DeleteNode() {
    __asm {
        push [esp + 4]
        push edx
        call Tree_DeleteNodeImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56bb10, Tree_DeleteNode);

// 0x56bc00 body. Emits the coder's current token: a literal byte (match length
// +0x20 <= 2; sets the token's flag bit +0x4d in the flag byte +0x3c) or a match
// (position +0x1c, length - 3, split by the bit widths +0xc / +0x10). Every 8
// tokens the group (+0x3c, length +0x18) is written to stream A and restarted.
static int32_t __cdecl Coder_EmitImpl(uint8_t* c, StreamFn flush) {
    if (F<int32_t>(c, 0x20) <= 2) {
        c[0x3c] |= c[0x4d];
        F<int32_t>(c, 0x20) = 1;
        c[0x3c + F<int32_t>(c, 0x18)] = F<uint8_t*>(c, 0x38)[F<int32_t>(c, 0x14)];
    } else {
        uint8_t lo = (uint8_t)F<int32_t>(c, 0x1c);
        c[0x3c + F<int32_t>(c, 0x18)] = lo;
        uint8_t hi = (uint8_t)(F<int32_t>(c, 0x1c) >> ((F<int32_t>(c, 0xc) - 8) & 31));
        uint8_t mask = (uint8_t)(0xff << (c[0x10] & 31));
        int32_t at = ++F<int32_t>(c, 0x18);
        c[0x3c + at] = (uint8_t)((hi & mask) | (uint8_t)(c[0x20] - 3));
    }
    uint8_t bit = (uint8_t)(c[0x4d] << 1);
    int32_t n = ++F<int32_t>(c, 0x18);
    c[0x4d] = bit;
    if (bit) return n;
    F<int32_t>(c, 0) += n;
    StreamA_PutPendingImpl(flush, c, n);
    F<int32_t>(c, 0x18) = 1;
    c[0x4d] = 1;
    c[0x3c] = 0;
    return 1;
}
// 0x56bc00. Coder in EAX, flush callback on the stack.
__declspec(naked) void Coder_Emit() {
    __asm {
        push [esp + 4]
        push eax
        call Coder_EmitImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56bc00, Coder_Emit);

// 0x56be10. Unpacks an LZSS stream (header word 0x10, 0x91; 512-byte window
// starting at 0x1be; a flag byte per 8 tokens: literal, or 9-bit offset + 7-bit
// length + 3 copy) from read into write. 1 when done, -2 for an empty input, -3
// without callbacks, -4 for a bad header.
RT3_NOINLINE int32_t __fastcall Lzss_Unpack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xd9fdd0, 0x8000);
    hook::Global<int32_t>(0xd9fdc8) = n;
    if (n <= 0) return -2;
    if (hook::Global<uint16_t>(0xd9fddc) != 0x10 || hook::Global<uint16_t>(0xd9fdde) != 0x91) return -4;
    hook::Global<int32_t>(0xd9fdc8) = n - 0x10;
    hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fde0;
    hook::Global<int32_t>(0xda7dd4) = 0;
    hook::Global<uint8_t*>(0xda7dd8) = (uint8_t*)0xd97dc8;
    uint8_t win[0x200];
    memset(win, 0, sizeof win);
    uint32_t r = 0x1be, flags = 0;
    for (;;) {
        flags >>= 1;
        if (!(flags & 0x100)) {
            int32_t c = StreamA_GetImpl(read);
            if (c == -1) break;
            flags = (uint32_t)c | 0xff00;
        }
        if (flags & 1) {
            int32_t c = StreamA_GetImpl(read);
            if (c == -1) break;
            PutA((uint8_t)c, write);
            win[r] = (uint8_t)c;
            r = (r + 1) & 0x1ff;
            continue;
        }
        int32_t lo = StreamA_GetImpl(read);
        if (lo == -1) break;
        int32_t hi = StreamA_GetImpl(read);
        if (hi == -1) break;
        int32_t pos = lo | (hi & (int32_t)0xffffff80) << 1;
        int32_t len = (hi & 0x7f) + 2;
        for (int32_t k = 0; k <= len; ++k) {
            uint8_t c = win[(k + pos) & 0x1ff];
            PutA(c, write);
            win[r] = c;
            r = (r + 1) & 0x1ff;
        }
    }
    if (hook::Global<int32_t>(0xda7dd4) > 0) {
        write((uint8_t*)0xd97dc8, hook::Global<int32_t>(0xda7dd4));
        hook::Global<int32_t>(0xda7dd4) = 0;
    }
    return 1;
}
RT3_REPLACE(0x56be10, Lzss_Unpack);

// 0x56b990 body. LZSS InsertNode: inserts text position r (text +0x38) into the
// coder's binary trees (parent +0x24, left +0x28, right +0x2c, roots 0x201 + first
// byte, nil +8), recording the longest match (+0x1c position, +0x20 length, up to
// 0x42); a full-length match replaces the old node.
static void __cdecl Tree_InsertNodeImpl(uint8_t* c, int32_t r) {
    int32_t* parent = F<int32_t*>(c, 0x24);
    int32_t* left = F<int32_t*>(c, 0x28);
    int32_t* right = F<int32_t*>(c, 0x2c);
    uint8_t* text = F<uint8_t*>(c, 0x38);
    int32_t nil = F<int32_t>(c, 8);
    right[r] = nil;
    left[r] = nil;
    const uint8_t* key = text + r;
    int32_t p = 0x201 + key[0];
    F<int32_t>(c, 0x20) = 0;
    int32_t cmp = 1;
    for (;;) {
        if (cmp >= 0) {
            if (right[p] == F<int32_t>(c, 8)) {
                right[p] = r;
                parent[r] = p;
                return;
            }
            p = right[p];
        } else {
            if (left[p] == F<int32_t>(c, 8)) {
                left[p] = r;
                parent[r] = p;
                return;
            }
            p = left[p];
        }
        int32_t i = 1;
        cmp = 0;
        for (; i < 0x42; ++i)
            if ((cmp = (int32_t)key[i] - (int32_t)text[p + i]) != 0) break;
        if (i <= F<int32_t>(c, 0x20)) continue;
        F<int32_t>(c, 0x1c) = p;
        F<int32_t>(c, 0x20) = i;
        if (i >= 0x42) break;
    }
    parent[r] = parent[p];
    left[r] = left[p];
    right[r] = right[p];
    parent[left[p]] = r;
    parent[right[p]] = r;
    if (right[parent[p]] == p) right[parent[p]] = r;
    else left[parent[p]] = r;
    parent[p] = F<int32_t>(c, 8);
    (void)nil;
}
// 0x56b990. Coder in EBX, position on the stack.
__declspec(naked) void Tree_InsertNode() {
    __asm {
        push [esp + 4]
        push ebx
        call Tree_InsertNodeImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56b990, Tree_InsertNode);

// 0x56bca0 body. Allocates and clears the coder's trees: 9/7-bit position/length
// split, nil 0x200 in every node and root (roots 0x201..0x300).
static void __cdecl Tree_InitImpl(uint8_t* c) {
    typedef void*(__cdecl * MallocFn)(size_t);
    MallocFn alloc = (MallocFn)0x5a125d;
    F<int32_t>(c, 0x10) = 7;
    F<int32_t>(c, 0xc) = 9;
    F<void*>(c, 0x24) = alloc(0x804);
    F<void*>(c, 0x28) = alloc(0x804);
    F<void*>(c, 0x2c) = alloc(0xc04);
    F<void*>(c, 0x38) = alloc(0x241);
    F<int32_t>(c, 8) = 0x200;
    for (int32_t i = 0; i < 0x200; ++i) {
        F<int32_t*>(c, 0x24)[i] = F<int32_t>(c, 8);
        F<int32_t*>(c, 0x28)[i] = F<int32_t>(c, 8);
        F<int32_t*>(c, 0x2c)[i] = F<int32_t>(c, 8);
    }
    for (int32_t i = 0x201; i <= 0x300; ++i) F<int32_t*>(c, 0x2c)[i] = F<int32_t>(c, 8);
}
// 0x56bca0. Coder in ESI.
__declspec(naked) void Tree_Init() {
    __asm {
        push esi
        call Tree_InitImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x56bca0, Tree_Init);

// 0x56c3e0. Packs read into write as RLE (header 0xbabe, word 0x10, 0x92; a byte
// 0x80 | n-1 before a run of n <= 0x80 copies, or n-1 before n literal bytes),
// working through a 256-byte look-ahead. 1 when done, -2 for an empty input, -3
// without callbacks.
RT3_NOINLINE int32_t __fastcall Rle_Pack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xdafde8, 0x8000);
    hook::Global<int32_t>(0xdb7de8) = n;
    if (n <= 0) return -2;
    hook::Global<int32_t>(0xda7de8) = 0;
    hook::Global<int32_t>(0xda7de4) = 0;
    hook::Global<int32_t>(0xdafde4) = 0x10;
    hook::Global<int32_t>(0xda7de0) = 0xbabe;
    hook::Global<int32_t>(0xda7dec) = 0x920010;
    hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafde8;
    hook::Global<uint8_t*>(0xdb7dec) = (uint8_t*)0xda7de0;
    uint8_t buf[0x100];
    int32_t len = StreamB_ReadImpl(0x100, read, buf);
    while (len > 0) {
        int32_t i = 2;
        if (len > 2) {
            while (i < 0x80 && buf[i] == buf[i - 1] && buf[i - 1] == buf[i - 2]) {
                ++i;
                if (i >= len) break;
            }
            if (i > 2) {
                StreamB_PutImpl((uint8_t)(i - 1) | 0x80u, write);
                StreamB_PutImpl(buf[0], write);
                memmove(buf, buf + i, 0x100 - i);
                len += StreamB_ReadImpl(i, read, buf + 0x100 - i) - i;
            }
        }
        if (len <= 0) break;
        int32_t j = 0;
        while (j < 0x80) {
            if (j + 2 < len && buf[j] == buf[j + 1] && buf[j + 1] == buf[j + 2]) break;
            ++j;
            if (j >= len) break;
        }
        if (j) {
            StreamB_PutImpl((uint8_t)(j - 1), write);
            for (int32_t k = 0; k < j; ++k) StreamB_PutImpl(buf[k], write);
            memmove(buf, buf + j, 0x100 - j);
            len += StreamB_ReadImpl(j, read, buf + 0x100 - j) - j;
        }
    }
    if (hook::Global<int32_t>(0xdafde4) > 0) {
        write((uint8_t*)0xda7de0, hook::Global<int32_t>(0xdafde4));
        hook::Global<int32_t>(0xdafde4) = 0;
    }
    return 1;
}
RT3_REPLACE(0x56c3e0, Rle_Pack);

// 0x56c630. Unpacks Rle_Pack's format from read into write. 1 when done, -2 for an
// empty input, -3 without callbacks, -4 for a bad header.
RT3_NOINLINE int32_t __fastcall Rle_Unpack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xdafde8, 0x8000);
    hook::Global<int32_t>(0xdb7de8) = n;
    if (n <= 0) return -2;
    if (hook::Global<uint16_t>(0xdafdf4) != 0x10 || hook::Global<uint16_t>(0xdafdf6) != 0x92) return -4;
    hook::Global<int32_t>(0xdb7de8) = n - 0x10;
    hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafdf8;
    hook::Global<int32_t>(0xdafde4) = 0;
    hook::Global<uint8_t*>(0xdb7dec) = (uint8_t*)0xda7de0;
    uint8_t buf[0x100];
    int32_t len = StreamB_ReadImpl(0x100, read, buf);
    while (len > 0) {
        uint8_t ctrl = buf[0];
        memmove(buf, buf + 1, 0xff);
        len += StreamB_ReadImpl(1, read, buf + 0xff) - 1;
        int32_t count = (ctrl & 0x7f) + 1;
        if (ctrl & 0x80) {
            uint8_t v = buf[0];
            memmove(buf, buf + 1, 0xff);
            len += StreamB_ReadImpl(1, read, buf + 0xff) - 1;
            for (int32_t k = 0; k < count; ++k) StreamB_PutImpl(v, write);
        } else {
            int32_t m = count < len ? count : len;
            for (int32_t k = 0; k < m; ++k) StreamB_PutImpl(buf[k], write);
            memmove(buf, buf + m, 0x100 - m);
            len += StreamB_ReadImpl(m, read, buf + 0x100 - m) - m;
        }
    }
    if (hook::Global<int32_t>(0xdafde4) > 0) {
        write((uint8_t*)0xda7de0, hook::Global<int32_t>(0xdafde4));
        hook::Global<int32_t>(0xdafde4) = 0;
    }
    return 1;
}
RT3_REPLACE(0x56c630, Rle_Unpack);

// 0x56bfe0. Packs read into write as LZSS (header 0xbabe, word 0x10, 0x91; the
// format Lzss_Unpack reads), with the coder state and its trees on the stack and
// in the heap. 1 when done, -2 for an empty input, -3 without callbacks.
RT3_NOINLINE int32_t __fastcall Lzss_Pack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xd9fdd0, 0x8000);
    hook::Global<int32_t>(0xd9fdc8) = n;
    if (n <= 0) return -2;
    hook::Global<int32_t>(0xd97dcc) = 0;
    hook::Global<int32_t>(0xd97dd0) = 0;
    hook::Global<int32_t>(0xd97dd4) = 0x910010;
    uint8_t c[0x50];
    memset(c, 0, 0x4e);
    hook::Global<int32_t>(0xda7dd4) = 0x10;
    hook::Global<int32_t>(0xd97dc8) = 0xbabe;
    hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fdd0;
    hook::Global<uint8_t*>(0xda7dd8) = (uint8_t*)0xd97dc8;
    Tree_InitImpl(c);
    uint8_t* text = F<uint8_t*>(c, 0x38);
    memset(text, 0, 0x200);
    int32_t r = 0x1be, len = 0;
    F<int32_t>(c, 0x14) = r;
    for (; len < 0x42; ++len) {
        int32_t ch = StreamA_GetImpl(read);
        if (ch == -1) break;
        text[0x1be + len] = (uint8_t)ch;
    }
    for (int32_t i = 1; i <= 0x42; ++i) Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14) - i);
    Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14));
    int32_t s0 = 0;
    F<int32_t>(c, 0x18) = 1;
    c[0x4d] = 1;
    c[0x3c] = 0;
    F<int32_t>(c, 0) = 0;
    do {
        if (F<int32_t>(c, 0x20) >= len) F<int32_t>(c, 0x20) = len;
        Coder_EmitImpl(c, write);
        int32_t last = F<int32_t>(c, 0x20), i = 0;
        for (; i < last; ++i) {
            int32_t ch = StreamA_GetImpl(read);
            if (ch == -1) break;
            Tree_DeleteNodeImpl(s0, c);
            text[s0] = (uint8_t)ch;
            if (s0 < 0x41) text[s0 + 0x200] = (uint8_t)ch;
            s0 = (s0 + 1) & 0x1ff;
            F<int32_t>(c, 0x14) = (F<int32_t>(c, 0x14) + 1) & 0x1ff;
            Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14));
        }
        for (; i < last; ++i) {
            Tree_DeleteNodeImpl(s0, c);
            s0 = (s0 + 1) & 0x1ff;
            F<int32_t>(c, 0x14) = (F<int32_t>(c, 0x14) + 1) & 0x1ff;
            if (--len) Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14));
        }
    } while (len > 0);
    int32_t pending = F<int32_t>(c, 0x18);
    if (pending > 1) {
        F<int32_t>(c, 0) += pending;
        for (int32_t k = 0; k < pending; ++k) PutA(c[0x3c + k], write);
    }
    if (hook::Global<int32_t>(0xda7dd4) > 0) {
        write((uint8_t*)0xd97dc8, hook::Global<int32_t>(0xda7dd4));
        hook::Global<int32_t>(0xda7dd4) = 0;
    }
    static const uint32_t bufs[] = { 0x24, 0x28, 0x2c, 0x38 };
    for (uint32_t off : bufs)
        if (void* b = F<void*>(c, off)) ((void(__cdecl*)(void*))0x5a1145)(b);
    return 1;
}
RT3_REPLACE(0x56bfe0, Lzss_Pack);

// 0x553000. Packs n bytes at src with the packer's method (+0: 0x91 LZSS, 0x92
// RLE) into a new buffer of n + 0x10 bytes (the stream context is 0xd93984,
// read/write callbacks 0x552f30 / 0x552fa0). On success *out / *outSize get the
// buffer and packed size (also stored at +4, with the original size at +8) and
// the packer's 1 comes back; otherwise both are cleared and the error returned
// (-5: output overflow, -6: no memory, -7: unknown method).
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize) {
    uint8_t* buf = ((uint8_t*(__cdecl*)(size_t))0x5a125d)((size_t)(n + 0x10));
    if (!buf) return -6;
    int32_t ctx[6] = { (int32_t)(uintptr_t)src, 0, n, (int32_t)(uintptr_t)buf, 0, n };
    hook::Global<int32_t*>(0xd93984) = ctx;
    int32_t r;
    bool failed = false;
    if (p[0] == 0x91) r = Lzss_Pack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else if (p[0] == 0x92) r = Rle_Pack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else {
        r = -7;
        failed = true;
    }
    if (r != 1) failed = true;
    int32_t* c = hook::Global<int32_t*>(0xd93984);
    if (c[4] > c[5]) {
        r = -5;
        failed = true;
    }
    if (failed) {
        *out = nullptr;
        *outSize = 0;
        ((void(__cdecl*)(void*))0x5a1145)(buf);
        return r;
    }
    F<int32_t>((void*)(uintptr_t)ctx[3], 8) = c[2];
    F<int32_t>((void*)(uintptr_t)ctx[3], 4) = c[4];
    *out = buf;
    *outSize = c[4];
    return r;
}
RT3_REPLACE(0x553000, Pack_Compress);

// 0x552f30. Pack_Compress's read callback: the next up to n bytes of the source
// (context 0xd93984: data +0, position +4, size +8). When the progress callback
// 0xd93988 is set it gets position / size * [0x5c8c28] (x87, then _ftol). -1 at
// the end.
int32_t __fastcall PackSrc_Read(uint8_t* buf, int32_t n) {
    int32_t* c = hook::Global<int32_t*>(0xd93984);
    int32_t left = c[2] - c[1];
    if (n >= left) n = left;
    if (!n) return -1;
    memcpy(buf, (const uint8_t*)(uintptr_t)c[0] + c[1], (size_t)n);
    int32_t pos = c[1] + n;
    c[1] = pos;
    if (void* progress = hook::Global<void*>(0xd93988)) {
        int32_t* total = &c[2];
        int32_t pct;
        __asm {
            fild pos
            mov eax, total
            fidiv dword ptr [eax]
            mov eax, 0x5c8c28
            fmul dword ptr [eax]
            mov eax, 0x5a10d0
            call eax
            mov pct, eax
        }
        ((void(__fastcall*)(int32_t))progress)(pct);
    }
    return n;
}
RT3_REPLACE(0x552f30, PackSrc_Read);

// 0x552fa0. Pack_Compress's write callback: appends n bytes at the output
// (context: buffer +0xc, used +0x10, capacity +0x14) when they fit; the used
// count grows either way. n when copied, else 0.
int32_t __fastcall PackDst_Write(uint8_t* buf, int32_t n) {
    if (!n) return 0;
    int32_t* c = hook::Global<int32_t*>(0xd93984);
    int32_t r = 0;
    if (n + c[4] <= c[5]) {
        memcpy((uint8_t*)(uintptr_t)c[3] + c[4], buf, (size_t)n);
        r = n;
    }
    c[4] += n;
    return r;
}
RT3_REPLACE(0x552fa0, PackDst_Write);

// 0x553120. Unpacks a Pack_Compress buffer (header 0xbabe, packed size +4,
// original size +8, method word +0xe) of n bytes (n <= 0: the header's size) into
// a new buffer; *out / *outSize get it and its length and the unpacker's 1 comes
// back, else both are cleared and the error returned (-4: no header, -6: no
// memory, -7: unknown method). The packer's method +0 is restored afterwards.
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize) {
    if (src[0] != 0xbabe) return -4;
    uint8_t* buf = ((uint8_t*(__cdecl*)(size_t))0x5a125d)((size_t)src[2]);
    if (!buf) return -6;
    int32_t ctx[6] = { (int32_t)(uintptr_t)src, 0, n > 0 ? n : src[1], (int32_t)(uintptr_t)buf, 0, src[2] };
    int32_t saved = p[0];
    hook::Global<int32_t*>(0xd93984) = ctx;
    int32_t method = ((const uint16_t*)src)[7];
    p[0] = method;
    int32_t r;
    if (method == 0x91) r = Lzss_Unpack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else if (method == 0x92) r = Rle_Unpack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else r = -7;
    p[0] = saved;
    if (r != 1) {
        *out = nullptr;
        *outSize = 0;
        ((void(__cdecl*)(void*))0x5a1145)(buf);
        return r;
    }
    *out = buf;
    *outSize = hook::Global<int32_t*>(0xd93984)[4];
    return r;
}
RT3_REPLACE(0x553120, Pack_Decompress);

// 0x552e20. Loads the JPEG in file path (from byte offset when non-zero) with the
// bundled libjpeg: *width / *height get its size and the 24-bit pixels come back
// in a new buffer (null when the file can't be opened).
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c9678);
    if (!f) return nullptr;
    if (offset) ((int(__cdecl*)(void*, int32_t, int))0x5a261f)(f, offset, 0);
    struct {
        uint8_t* row;
        uint8_t cinfo[0x1a0];
        uint8_t err[0x84];
    } l;
    F<void*>(l.cinfo, 0) = ((void*(__fastcall*)(void*))0x5b3b30)(l.err);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b3230)(l.cinfo, 0x3e, 0x19f);
    ((void(__fastcall*)(void*, void*))0x5b3980)(l.cinfo, f);
    ((int(__fastcall*)(void*, uint8_t))0x5b3610)(l.cinfo, 1);
    ((int(__fastcall*)(void*))0x5b37e0)(l.cinfo);
    uint32_t w = F<uint32_t>(l.cinfo, 0x52), h = F<uint32_t>(l.cinfo, 0x56);
    *width = (int32_t)w;
    *height = (int32_t)h;
    uint8_t* buf = ((uint8_t*(__cdecl*)(size_t))0x5a125d)(h * w * 3);
    while (F<uint32_t>(l.cinfo, 0x6e) < F<uint32_t>(l.cinfo, 0x56)) {
        l.row = buf + F<uint32_t>(l.cinfo, 0x6e) * F<uint32_t>(l.cinfo, 0x52) * 3;
        ((uint32_t(__fastcall*)(void*, uint8_t**, uint32_t))0x5b3740)(l.cinfo, &l.row, 1);
    }
    ((int(__fastcall*)(void*))0x5b3550)(l.cinfo);
    ((void(__fastcall*)(void*))0x5b3300)(l.cinfo);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return buf;
}
RT3_REPLACE(0x552e20, Jpeg_Load);

uint64_t __fastcall SysInfo_CpuMhz();
void __stdcall Cpu_Name_Impl(char* out, const char* vendor, int32_t model, int32_t family);

// 0x55da40. Fills the system-info block: memory status (0xd97548), CPU MHz
// (0xd9743b) and, when CPUID runs, the vendor (0xd973f8), family / model /
// stepping (0xd97423 / 0xd97427 / 0xd9742b, extended family for family 0xf), the
// supported (0xd9742f) and usable (0xd97433) MMX 1 / SSE 2 / SSE2 4 / 3DNow 8
// features (SSE2 sets 0xd9743f, passed to the CRT) and the CPU name (0xd97405).
// 0xd97437 ends up 0xf (the original reuses that register).
void SysInfo_Init() {
    memset((void*)0xd973f8, 0, 0x4b);
    memset((void*)0xd97548, 0, 0x20);
    hook::Global<void(__stdcall*)(void*)>(0x5c81d8)((void*)0xd97548);
    hook::Global<uint32_t>(0xd9743b) = (uint32_t)SysInfo_CpuMhz();
    if (!Cpu_HasCpuid()) return;
    int r[4];
    char vendor[13];
    __cpuid(r, 0);
    memcpy(vendor, &r[1], 4);
    memcpy(vendor + 4, &r[3], 4);
    memcpy(vendor + 8, &r[2], 4);
    vendor[12] = 0;
    __cpuid(r, 1);
    uint32_t sig = (uint32_t)r[0], feat = (uint32_t)r[3], ext = 0;
    __cpuid(r, (int)0x80000000);
    if ((uint32_t)r[0] >= 0x80000000) {
        __cpuid(r, (int)0x80000001);
        ext = (uint32_t)r[3];
    }
    uint32_t have = 0, usable = 0;
    if (feat & 0x800000) {
        have = 1;
        if (Cpu_ProbeFeature(1)) usable = 1;
    }
    if (ext & 0x80000000) {
        have |= 8;
        if (Cpu_ProbeFeature(8)) usable |= 8;
    }
    if (feat & 0x2000000) {
        have |= 2;
        if (Cpu_ProbeFeature(2)) usable |= 2;
    }
    if (feat & 0x4000000) {
        have |= 4;
        if (Cpu_ProbeFeature(4)) {
            usable |= 4;
            hook::Global<int32_t>(0xd9743f) = 1;
        }
    }
    ((void(__cdecl*)(int32_t))0x5a4376)(hook::Global<int32_t>(0xd9743f));
    hook::Global<uint32_t>(0xd97433) = usable;
    hook::Global<uint32_t>(0xd9742f) = have;
    hook::Global<uint32_t>(0xd9742b) = sig & 0xf;
    uint32_t family = sig >> 8 & 0xf;
    hook::Global<uint32_t>(0xd97423) = family;
    if (family == 0xf) hook::Global<uint32_t>(0xd97423) = ((sig & 0xff00000) | 0xf0000) >> 16;
    hook::Global<uint32_t>(0xd97427) = sig >> 4 & 0xf;
    strcpy((char*)0xd973f8, vendor);
    Cpu_Name_Impl((char*)0xd97405, (const char*)0xd973f8, hook::Global<int32_t>(0xd97427), hook::Global<int32_t>(0xd97423));
    hook::Global<int32_t>(0xd97437) = 0xf;
}
RT3_REPLACE(0x55da40, SysInfo_Init);

} // namespace rt3
