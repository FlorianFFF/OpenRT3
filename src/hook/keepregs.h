// Register-preserving replacement wrappers.
//
// The original compiler knew which registers each of its functions leaves
// untouched, and some callers rely on that: e.g. 0x44d410 keeps using ECX
// after calling 0x449e90 without reloading it. A C++ replacement may clobber
// ECX/EDX, so such functions are replaced by a naked wrapper that saves both,
// copies the N stack arguments, calls the C++ implementation (which receives
// ECX/EDX unchanged, so __fastcall `this` still works) and restores them:
//
//     __declspec(naked) int32_t __stdcall Foo(int32_t a, int32_t b) { RT3_KEEP_ECX_EDX(Foo_Impl, 2) }
//     RT3_REPLACE(0x123456, Foo);
//
// The implementation must pop exactly N dwords (callee-cleanup, as the
// original). tools/regpreserve.py finds the callers that need this.
#pragma once

#define RT3_KEEP_PUSH0
#define RT3_KEEP_PUSH1(N) __asm push dword ptr [esp + 8 + 4 * N]
#define RT3_KEEP_PUSH2(N) RT3_KEEP_PUSH1(N) RT3_KEEP_PUSH1(N)
#define RT3_KEEP_PUSH3(N) RT3_KEEP_PUSH2(N) RT3_KEEP_PUSH1(N)
#define RT3_KEEP_PUSH4(N) RT3_KEEP_PUSH3(N) RT3_KEEP_PUSH1(N)
#define RT3_KEEP_PUSH5(N) RT3_KEEP_PUSH4(N) RT3_KEEP_PUSH1(N)
#define RT3_KEEP_PUSH6(N) RT3_KEEP_PUSH5(N) RT3_KEEP_PUSH1(N)

#define RT3_KEEP_ARGS_0
#define RT3_KEEP_ARGS_1 RT3_KEEP_PUSH1(1)
#define RT3_KEEP_ARGS_2 RT3_KEEP_PUSH2(2)
#define RT3_KEEP_ARGS_3 RT3_KEEP_PUSH3(3)
#define RT3_KEEP_ARGS_4 RT3_KEEP_PUSH4(4)
#define RT3_KEEP_ARGS_5 RT3_KEEP_PUSH5(5)
#define RT3_KEEP_ARGS_6 RT3_KEEP_PUSH6(6)

#define RT3_KEEP_ECX_EDX(IMPL, N)                                                          \
    __asm push ecx                                                                         \
    __asm push edx                                                                         \
    RT3_KEEP_ARGS_##N                                                                      \
    __asm call IMPL                                                                        \
    __asm pop edx                                                                          \
    __asm pop ecx                                                                          \
    __asm ret 4 * N
