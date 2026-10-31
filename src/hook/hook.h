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

// The current target of the CALL rel32 at `site` in the exe. Use it where a
// rewritten function makes a call that a loaded mod (DesyncWatch's DINPUT8.dll)
// redirects by patching that call site, so the mod keeps working; without the
// mod it is the original callee.
template <typename T> inline T CallTarget(uint32_t site) {
    return reinterpret_cast<T>(site + 5 + *reinterpret_cast<const int32_t*>(site + 1));
}

// Raw memory accessors for globals that still live in the exe.
template <typename T> inline T& Global(uint32_t addr) { return *reinterpret_cast<T*>(addr); }

} // namespace hook

// thiscall methods are reimplemented as __fastcall(this, edx_unused, args...).
// Keeps a function out of line so the UI difftests can redirect every call to
// it (tests/test_ui.cpp patches the entry point).
#define RT3_NOINLINE __declspec(noinline)

#define RT3_REPLACE(addr, fn) \
    static hook::Registrar rt3_reg_##fn(addr, (void*)&fn, #fn)
