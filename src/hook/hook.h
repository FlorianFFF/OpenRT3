// Function-replacement registry.
//
// The rebuild runs inside the original RT3_mp.exe (imagebase 0x400000, fixed).
// Every reimplemented function registers itself with the address of the
// original it replaces; at startup the DLL patches a JMP at that address.
// This keeps the game playable while functions are rewritten one by one, and
// lets every replacement be switched off again from rt3re.ini to compare
// against the original.
#pragma once
#include <stdint.h>

namespace hook {

struct Entry {
    uint32_t    addr;   // original function VA in RT3_mp.exe
    void*       impl;   // our replacement (same calling convention)
    const char* name;   // section/key in rt3re.ini
    Entry*      next;
};

// Registers at static-init time; see RT3_REPLACE.
struct Registrar {
    Registrar(uint32_t addr, void* impl, const char* name);
};

// Writes all enabled JMPs. Called once from DllMain.
int InstallAll(const char* iniPath);

// Writes "addr name calls" per replacement (only counted with RT3RE_COVERAGE set).
void DumpCoverage(const char* path);

// Pointer to an original function, for calling code not yet rewritten.
template <typename T> inline T Orig(uint32_t addr) { return reinterpret_cast<T>(addr); }

// Raw memory accessors for globals that still live in the exe.
template <typename T> inline T& Global(uint32_t addr) { return *reinterpret_cast<T*>(addr); }

} // namespace hook

// thiscall methods are reimplemented as __fastcall(this, edx_unused, args...).
#define RT3_REPLACE(addr, fn) \
    static hook::Registrar rt3_reg_##fn(addr, (void*)&fn, #fn)
