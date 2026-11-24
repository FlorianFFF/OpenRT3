// Finds the game's executable: version 1.06 (built 2004-09-08) without copy
// protection, the one every address in this project refers to. It is RT3.exe
// in the DRM-free releases; a protected RT3.exe (extra encrypted sections, a
// different entry point) cannot be used, so any other .exe in the folder with
// the 1.06 image is taken instead.
#pragma once
#include <windows.h>
#include <stdio.h>
#include <string.h>

namespace rt3game {

const DWORD kImageBase = 0x400000, kEntry = 0x1a313b, kImageSize = 0x9c8260;

// Whether `path` holds the 1.06 image (checked by its PE headers).
inline bool IsGameImage(const char* path) {
    HANDLE f = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE) return false;
    unsigned char head[0x1000];
    DWORD got = 0;
    bool ok = ReadFile(f, head, sizeof head, &got, nullptr) && got == sizeof head;
    CloseHandle(f);
    if (!ok) return false;
    LONG pe = ((IMAGE_DOS_HEADER*)head)->e_lfanew;
    if (((IMAGE_DOS_HEADER*)head)->e_magic != IMAGE_DOS_SIGNATURE || pe < 0 || pe + sizeof(IMAGE_NT_HEADERS32) > sizeof head)
        return false;
    auto nt = (IMAGE_NT_HEADERS32*)(head + pe);
    return nt->Signature == IMAGE_NT_SIGNATURE && nt->OptionalHeader.ImageBase == kImageBase &&
           nt->OptionalHeader.AddressOfEntryPoint == kEntry && nt->OptionalHeader.SizeOfImage == kImageSize;
}

// Full path of the game exe in `dir` (ending in a backslash) into `out`.
inline bool FindGameExe(const char* dir, char* out, size_t outSize) {
    snprintf(out, outSize, "%sRT3.exe", dir);
    if (IsGameImage(out)) return true;
    char pattern[MAX_PATH];
    snprintf(pattern, sizeof pattern, "%s*.exe", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return false;
    bool found = false;
    do {
        snprintf(out, outSize, "%s%s", dir, fd.cFileName);
        found = IsGameImage(out);
    } while (!found && FindNextFileA(h, &fd));
    FindClose(h);
    return found;
}

}  // namespace rt3game
