// Bridges to the C runtime statically linked into RT3_mp.exe.
//
// While rewritten and original code run side by side they share heap blocks:
// memory we allocate may be freed by original code and vice versa. So all heap
// use goes through the exe's own malloc/free, never our DLL's CRT. In the
// standalone build these become the normal CRT functions.
#pragma once
#include <stddef.h>
#include <stdint.h>

namespace rt3 {

inline void* Malloc(size_t n) { return ((void*(__cdecl*)(size_t))0x5a125d)(n); }
inline void  Free(void* p)    { ((void(__cdecl*)(void*))0x5a1145)(p); }

// Locale-dependent CRT calls must use the exe's CRT (the game calls setlocale).
inline int Stricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
inline int Strnicmp(const char* a, const char* b, size_t n) {
    return ((int(__cdecl*)(const char*, const char*, size_t))0x5a5f26)(a, b, n);
}
inline int Tolower(int c) { return ((int(__cdecl*)(int))0x5a5900)(c); }

// Low-level IO: file descriptors live in the exe CRT's table.
inline int Open(const char* p, int flags, int pmode) { return ((int(__cdecl*)(const char*, int, int))0x5a4c57)(p, flags, pmode); }
inline int Close(int fd) { return ((int(__cdecl*)(int))0x5a440d)(fd); }
inline int Write(int fd, const void* b, unsigned n) { return ((int(__cdecl*)(int, const void*, unsigned))0x5a464d)(fd, b, n); }
inline int Read(int fd, void* b, unsigned n) { return ((int(__cdecl*)(int, void*, unsigned))0x5a48c5)(fd, b, n); }
inline long Tell(int fd) { return ((long(__cdecl*)(int))0x5c7820)(fd); }
inline long FileLength(int fd) { return ((long(__cdecl*)(int))0x5c7831)(fd); }

// 0x53afd0: frees *p if set and clears it.
void __fastcall FreeAndNull(void** p);

} // namespace rt3
