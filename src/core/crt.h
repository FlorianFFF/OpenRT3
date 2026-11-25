// The C runtime statically linked into RT3.exe (MSVC 7.0), and the game's
// operator new/delete: one name per function, the standard C name.
//
// While rewritten and original code run side by side they share the runtime's
// state: heap blocks (memory we allocate may be freed by original code and vice
// versa), file handles, the locale (the game calls setlocale), errno, the rand
// seed and the x87/SSE2 math dispatch. So the rewrite calls the exe's runtime,
// never our DLL's. Its FILE is not our compiler's FILE, so streams are void*.
//
// Moving off the exe later replaces these definitions; the call sites stay.
#pragma once
#include <stddef.h>
#include <stdint.h>

namespace rt3 {
namespace crt {

#define RT3_CRT(ret, name, params, addr) static ret(__cdecl* const name) params = (ret(__cdecl*) params)addr

// Memory. operator_new/operator_delete are the game's own (0x53b070/0x53b080).
RT3_CRT(void*, operator_new, (size_t), 0x53b070);
RT3_CRT(void, operator_delete, (void*), 0x53b080);
RT3_CRT(void*, malloc, (size_t), 0x5a125d);
RT3_CRT(void, free, (void*), 0x5a1145);
RT3_CRT(void*, memmove, (void*, const void*, size_t), 0x5a20b0);

// Math (each dispatches to an x87 or SSE2 version, see set_sse2_enable).
RT3_CRT(double, fabs, (double), 0x5a152e);
RT3_CRT(double, sin, (double), 0x5a13e4);
RT3_CRT(double, cos, (double), 0x5a1494);
RT3_CRT(double, tan, (double), 0x5a3054);
RT3_CRT(double, atan, (double), 0x5a1270);
RT3_CRT(double, atan2, (double, double), 0x5a29d0);
RT3_CRT(double, exp, (double), 0x5a2ee0);
RT3_CRT(double, sqrt, (double), 0x5a1634);
RT3_CRT(double, fmod, (double, double), 0x5a1390);
RT3_CRT(double, pow, (double, double), 0x5a1b60);
RT3_CRT(void, set_sse2_enable, (int), 0x5a4376);  // _set_SSE2_enable
// div(): the div_t comes back in edx:eax, quotient in the low half.
RT3_CRT(uint64_t, div, (int32_t, int32_t), 0x5a16da);
static uint64_t(__stdcall* const alldiv)(uint64_t, uint64_t) = (uint64_t(__stdcall*)(uint64_t, uint64_t))0x5a4240;  // __alldiv
RT3_CRT(int, rand, (), 0x5a29ab);
RT3_CRT(void, srand, (unsigned), 0x5a299e);

// Strings.
RT3_CRT(int, sprintf, (char*, const char*, ...), 0x5a19c4);
RT3_CRT(int, atoi, (const char*), 0x5a1ea5);
RT3_CRT(long, atol, (const char*), 0x5a1e1d);
RT3_CRT(double, atof, (const char*), 0x5a2076);
RT3_CRT(int, stricmp, (const char*, const char*), 0x5a57cf);
RT3_CRT(int, strnicmp, (const char*, const char*, size_t), 0x5a5f26);
RT3_CRT(char*, strstr, (const char*, const char*), 0x5a1ff0);
RT3_CRT(char*, strchr, (const char*, int), 0x5a2a00);
RT3_CRT(char*, strrchr, (const char*, int), 0x5a1df0);
RT3_CRT(char*, strncpy, (char*, const char*, size_t), 0x5a18a0);
RT3_CRT(char*, strncat, (char*, const char*, size_t), 0x5a1eb0);
RT3_CRT(char*, strlwr, (char*), 0x5a2abe);
RT3_CRT(size_t, wcslen, (const wchar_t*), 0x5a3e74);
RT3_CRT(int, tolower, (int), 0x5a5900);
RT3_CRT(int, isleadbyte, (int), 0x5a330d);

// Streams.
RT3_CRT(void*, fopen, (const char*, const char*), 0x5a1887);
RT3_CRT(void*, fdopen, (int32_t, const char*), 0x5c7905);
RT3_CRT(int, fclose, (void*), 0x5a1740);
RT3_CRT(size_t, fread, (void*, size_t, size_t, void*), 0x5a1b05);
RT3_CRT(size_t, fwrite, (const void*, size_t, size_t, void*), 0x5a276f);
RT3_CRT(int, fseek, (void*, long, int), 0x5a261f);
RT3_CRT(long, ftell, (void*), 0x5a254f);
RT3_CRT(int, fflush, (void*), 0x5a2945);
RT3_CRT(char*, fgets, (char*, int, void*), 0x5a3fe0);
RT3_CRT(int, fscanf, (void*, const char*, ...), 0x5a17e1);
RT3_CRT(int, remove, (const char*), 0x5a27bb);

// Low-level IO: file descriptors live in the runtime's table.
RT3_CRT(int, open, (const char*, int, ...), 0x5a4c57);
RT3_CRT(int, close, (int), 0x5a440d);
RT3_CRT(int, read, (int, void*, unsigned), 0x5a48c5);
RT3_CRT(int, write, (int, const void*, unsigned), 0x5a464d);
RT3_CRT(long, lseek, (int, long, int), 0x5ac18d);
RT3_CRT(long, tell, (int), 0x5c7820);
RT3_CRT(long, filelength, (int), 0x5c7831);

// Process.
RT3_CRT(int*, errno_, (), 0x5a3f5b);  // _errno(): where errno lives
RT3_CRT(int, atexit, (void*), 0x5a2d10);
RT3_CRT(void, exit, (int), 0x5a2e9c);

#undef RT3_CRT

} // namespace crt

// 0x53afd0: frees *p if set and clears it.
void __fastcall FreeAndNull(void** p);

} // namespace rt3
