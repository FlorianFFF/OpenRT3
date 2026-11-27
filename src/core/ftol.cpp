// Float-to-integer conversion as in the original game.
//
// The game was compiled with MSVC 7.0, which converts every float to an
// integer with its C runtime's _ftol (0x5a10d0 in the exe): truncation of the
// x87 value through a 64-bit FISTP, the result in EDX:EAX. A value beyond the
// 32-bit range gives the low 32 bits of that integer, and NaN gives 0.
//
// Our compiler (x86, /arch:IA32) converts by calling its runtime's _ftol2 /
// _ftol2_sse / _ftoui2 instead, which differ there (0x80000000 for a value out
// of range or NaN, and _ftol2_sse first rounds the x87 value to a double).
// Defining those helpers here makes every cast in the rewrite call the exe's
// _ftol, as the original did. They all take the value in ST0 (and pop it).
//
// Rt3_Ftol is that _ftol, instruction for instruction (MSVC 7.0's: FISTP with
// the current rounding, round to nearest, then the result corrected towards
// zero by the sign of what the rounding left), so no conversion depends on the
// exe's code any more.
#include "hook/hook.h"

extern "C" __declspec(naked) void Rt3_Ftol() {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x20
        and esp, 0xfffffff0
        fld st(0)
        fst dword ptr [esp + 0x18]
        fistp qword ptr [esp + 0x10]
        fild qword ptr [esp + 0x10]
        mov edx, dword ptr [esp + 0x18]
        mov eax, dword ptr [esp + 0x10]
        test eax, eax
        jz low_zero
    fraction:
        fsubp st(1), st(0)
        test edx, edx
        jns positive
        fstp dword ptr [esp]
        mov ecx, dword ptr [esp]
        xor ecx, 0x80000000
        add ecx, 0x7fffffff
        adc eax, 0
        mov edx, dword ptr [esp + 0x14]
        adc edx, 0
        jmp done
    positive:
        fstp dword ptr [esp]
        mov ecx, dword ptr [esp]
        add ecx, 0x7fffffff
        sbb eax, 0
        mov edx, dword ptr [esp + 0x14]
        sbb edx, 0
        jmp done
    low_zero:
        mov edx, dword ptr [esp + 0x14]
        test edx, 0x7fffffff
        jnz fraction
        fstp dword ptr [esp + 0x18]
        fstp dword ptr [esp + 0x18]
    done:
        leave
        ret
    }
}

static const void* const kExeFtol = (const void*)&Rt3_Ftol;

// Our runtime's helpers keep the registers that are not their result (its
// _ftol2_sse converts with SSE2 and touches nothing else), and compiled code
// relies on that; the exe's _ftol uses ECX and EDX, so they are saved around
// the call. _ftol2's result is EDX:EAX, the 32-bit ones' EAX.
extern "C" __declspec(naked) void _ftol2() {
    __asm {
        push ecx
        call dword ptr [kExeFtol]
        pop ecx
        ret
    }
}

extern "C" __declspec(naked) void _ftol2_sse() {
    __asm {
        push ecx
        push edx
        call dword ptr [kExeFtol]
        pop edx
        pop ecx
        ret
    }
}

// To unsigned: the original took _ftol's low word.
extern "C" __declspec(naked) void _ftoui2() {
    __asm {
        push ecx
        push edx
        call dword ptr [kExeFtol]
        pop edx
        pop ecx
        ret
    }
}
