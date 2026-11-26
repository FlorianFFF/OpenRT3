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
// Only in rt3re.dll / rt3re_test.dll, which always run with the game's image
// loaded at 0x400000.

static const void* const kExeFtol = (const void*)0x5a10d0;

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
