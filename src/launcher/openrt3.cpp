// OpenRT3.exe: runs the game in its own process instead of the game's exe.
//
// The original image is not relocatable (base 0x400000, no relocations), so
// this exe is linked at 0x10000 and carries an uninitialised array that spans
// 0x400000-0xdca000. At startup the game's image (RT3.exe 1.06, see
// game_exe.h) is copied there and its imports are connected; then rt3re.dll
// installs the rewritten functions and the image's C runtime start-up runs,
// which calls our WinMain.
//
// The image is used as found, so an exe whose GameSpy server addresses were
// patched in its data (to play on a replacement master server) works
// unchanged: the GameSpy library code runs from the image.
//
// Link with /BASE:0x10000 /FIXED /SAFESEH:NO: the original's exception
// handlers lie inside this image and are not in a SafeSEH table.
#include <windows.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include "launcher/game_exe.h"

namespace {

const uint32_t kBase = 0x400000;
const uint32_t kEnd = 0xdca000;  // end of the game's image (0xdc9260 rounded up)

// Room for the original image. Uninitialised: no space in this file.
unsigned char g_room[kEnd];

char g_gameExe[MAX_PATH];

void Fail(const char* fmt, ...) {
    char msg[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof msg, fmt, ap);
    va_end(ap);
    MessageBoxA(nullptr, msg, "OpenRT3", MB_ICONERROR);
    ExitProcess(1);
}

// The image asks for its own module: answer with the original's base and file,
// so resources (icons, cursors) and the version info come from it.
HMODULE WINAPI GameGetModuleHandleA(LPCSTR name) {
    return name ? GetModuleHandleA(name) : (HMODULE)kBase;
}

DWORD WINAPI GameGetModuleFileNameA(HMODULE mod, LPSTR out, DWORD size) {
    if (mod && mod != (HMODULE)kBase) return GetModuleFileNameA(mod, out, size);
    DWORD n = (DWORD)strlen(g_gameExe);
    if (size == 0) return 0;
    if (n >= size) n = size - 1;
    memcpy(out, g_gameExe, n);
    out[n] = 0;
    return n;
}

// Diagnostics (OPENRT3_TRACE=1): how the image ends, and any message box.
FILE* g_trace;

void Trace(const char* fmt, ...) {
    if (!g_trace) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_trace, fmt, ap);
    va_end(ap);
    fflush(g_trace);
}

void WINAPI GameExitProcess(UINT code) {
    Trace("ExitProcess(%u) from %p\n", code, _ReturnAddress());
    ExitProcess(code);
}

int WINAPI GameMessageBoxA(HWND w, LPCSTR text, LPCSTR caption, UINT type) {
    // Traced runs are unattended: log the box instead of waiting on it.
    Trace("MessageBoxA \"%s\": %s (from %p)\n", caption ? caption : "", text ? text : "", _ReturnAddress());
    return IDOK;
}

// DirectInput checks that the instance is a loaded module; 0x400000 is not
// one, so it gets this exe's handle.
typedef HRESULT(WINAPI* DirectInput8CreateFn)(HINSTANCE, DWORD, const IID&, void**, IUnknown*);
DirectInput8CreateFn g_directInput8Create;

HRESULT WINAPI GameDirectInput8Create(HINSTANCE inst, DWORD version, const IID& iid, void** out, IUnknown* outer) {
    if (inst == (HINSTANCE)kBase) inst = GetModuleHandleA(nullptr);
    return g_directInput8Create(inst, version, iid, out, outer);
}

void* Rva(uint32_t rva) { return (void*)(kBase + rva); }

}  // namespace

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // The game folder is ours.
    char dir[MAX_PATH];
    GetModuleFileNameA(nullptr, dir, MAX_PATH);
    if (char* slash = strrchr(dir, '\\')) slash[1] = 0;
    if (!rt3game::FindGameExe(dir, g_gameExe, sizeof g_gameExe))
        Fail("No RT3.exe version 1.06 (without copy protection) found in %s", dir);

    char trace[8];
    if (GetEnvironmentVariableA("OPENRT3_TRACE", trace, sizeof trace)) {
        char path[MAX_PATH];
        snprintf(path, sizeof path, "%sOpenRT3.log", dir);
        g_trace = fopen(path, "w");
    }

    uintptr_t lo = (uintptr_t)g_room, hi = lo + sizeof g_room;
    if (lo > kBase || hi < kEnd)
        Fail("Reserved range %08x-%08x does not cover %08x-%08x (rebuild with /BASE:0x10000).",
             (unsigned)lo, (unsigned)hi, kBase, kEnd);

    HANDLE f = CreateFileA(g_gameExe, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE) Fail("Cannot open %s", g_gameExe);
    DWORD size = GetFileSize(f, nullptr), got = 0;
    unsigned char* file = (unsigned char*)VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    if (!file || !ReadFile(f, file, size, &got, nullptr) || got != size) Fail("Cannot read %s", g_gameExe);
    CloseHandle(f);

    auto dos = (IMAGE_DOS_HEADER*)file;
    auto nt = (IMAGE_NT_HEADERS32*)(file + dos->e_lfanew);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.ImageBase != kBase || kBase + nt->OptionalHeader.SizeOfImage > kEnd)
        Fail("%s is not RT3.exe version 1.06", g_gameExe);

    // Copy headers and sections; the rest of the range is already zero.
    DWORD old;
    if (!VirtualProtect((void*)kBase, kEnd - kBase, PAGE_EXECUTE_READWRITE, &old))
        Fail("VirtualProtect failed (%lu)", GetLastError());
    memcpy((void*)kBase, file, nt->OptionalHeader.SizeOfHeaders);
    auto sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++sec) {
        DWORD n = sec->SizeOfRawData < sec->Misc.VirtualSize ? sec->SizeOfRawData : sec->Misc.VirtualSize;
        if (sec->Misc.VirtualSize == 0) n = sec->SizeOfRawData;
        memcpy(Rva(sec->VirtualAddress), file + sec->PointerToRawData, n);
    }

    // Connect the imports, searched for from the game folder as for the original.
    auto& dirImp = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    for (auto d = (IMAGE_IMPORT_DESCRIPTOR*)Rva(dirImp.VirtualAddress); d->Name; ++d) {
        const char* dll = (const char*)Rva(d->Name);
        HMODULE m = LoadLibraryA(dll);
        if (!m) Fail("Cannot load %s (%lu)", dll, GetLastError());
        auto names = (IMAGE_THUNK_DATA32*)Rva(d->OriginalFirstThunk ? d->OriginalFirstThunk : d->FirstThunk);
        auto iat = (IMAGE_THUNK_DATA32*)Rva(d->FirstThunk);
        for (; names->u1.AddressOfData; ++names, ++iat) {
            FARPROC fn;
            const char* fnName = nullptr;
            if (IMAGE_SNAP_BY_ORDINAL32(names->u1.Ordinal)) {
                fn = GetProcAddress(m, (LPCSTR)(uintptr_t)IMAGE_ORDINAL32(names->u1.Ordinal));
            } else {
                fnName = (const char*)((IMAGE_IMPORT_BY_NAME*)Rva(names->u1.AddressOfData))->Name;
                fn = GetProcAddress(m, fnName);
            }
            if (!fn) Fail("%s does not export %s", dll, fnName ? fnName : "an ordinal");
            if (g_trace && fnName && !strcmp(fnName, "ExitProcess")) fn = (FARPROC)&GameExitProcess;
            if (g_trace && fnName && !strcmp(fnName, "MessageBoxA")) fn = (FARPROC)&GameMessageBoxA;
            if (fnName && !strcmp(fnName, "DirectInput8Create")) {
                g_directInput8Create = (DirectInput8CreateFn)fn;
                fn = (FARPROC)&GameDirectInput8Create;
            }
            if (fnName && !_stricmp(dll, "KERNEL32.dll")) {
                if (!strcmp(fnName, "GetModuleHandleA")) fn = (FARPROC)&GameGetModuleHandleA;
                if (!strcmp(fnName, "GetModuleFileNameA")) fn = (FARPROC)&GameGetModuleFileNameA;
            }
            iat->u1.Function = (DWORD)(uintptr_t)fn;
        }
    }
    uint32_t entry = nt->OptionalHeader.AddressOfEntryPoint;
    VirtualFree(file, 0, MEM_RELEASE);

    // The rewritten game.
    char dll[MAX_PATH];
    snprintf(dll, sizeof dll, "%srt3re.dll", dir);
    if (!LoadLibraryA(dll)) Fail("Cannot load %s (%lu)", dll, GetLastError());

    // The image's C runtime start-up; it calls WinMain and exits the process.
    ((void(__cdecl*)())Rva(entry))();
    return 0;
}
