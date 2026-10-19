// Functions across blocks 0x44..0x51 (batch 66). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall Obj_Get21(void* o);
void* SelectedTrain();
int32_t ClockMs();
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);

static inline int32_t ExeOpen(const char* p, int32_t mode) { return ((int32_t(__cdecl*)(const char*, int32_t, ...))0x5a4c57)(p, mode); }
static inline int32_t ExeClose(int32_t fd) { return ((int32_t(__cdecl*)(int32_t))0x5a440d)(fd); }
static inline char* ExeStrncpy(char* d, const char* s, size_t n) { return ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)(d, s, n); }

static inline bool OwnTrain(uint8_t* t) { return F<int32_t>(t, 0x51) == Obj_Get21(hook::Global<void*>(0x6cec78)); }

// 0x441790. The player's own train under focus: the camera's followed
// vehicle (+0x60, if virtual slot 27 says so), else the selected train; 0 if not own.
void* FocusedOwnTrain() {
    uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
    if (!cam) return nullptr;
    uint8_t* t = nullptr;
    if (void* v = F<void*>(cam, 0x60)) {
        if (((int32_t(__thiscall*)(void*))(*(void***)v)[0x6c / 4])(v)) {
            t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0,
                                         F<uint32_t>(F<uint8_t*>(hook::Global<uint8_t*>(0x62be68), 0x60), 0x390));
            if (t && OwnTrain(t)) return t;
        }
    }
    t = (uint8_t*)SelectedTrain();
    if (t && !OwnTrain(t)) t = nullptr;
    return t;
}
RT3_REPLACE(0x441790, FocusedOwnTrain);

void* __fastcall Obj_SelectedTrainIfFlag(uint8_t* o) { return (F<uint32_t>(o, 0x33) & 0x10000) ? SelectedTrain() : nullptr; }
RT3_REPLACE(0x477ad0, Obj_SelectedTrainIfFlag);

// 0x516590 / 0x5165e0 / 0x516610. Tutorial step checks (0x6d3b50 into 0x622b48).
static inline int32_t TutorialStep() { return ((const int32_t*)0x622b48)[hook::Global<int32_t>(0x6d3b50) * 0x10]; }
int32_t Tutorial_Blink37a() {
    if (!hook::Global<int32_t>(0x6d3b4c) || TutorialStep() != 0x37a) return 0;
    return (ClockMs() / 350) & 1 ? 1 : 0;
}
RT3_REPLACE(0x516590, Tutorial_Blink37a);
int32_t Tutorial_Is19a() { return hook::Global<int32_t>(0x6d3b4c) && TutorialStep() == 0x19a ? 1 : 0; }
RT3_REPLACE(0x5165e0, Tutorial_Is19a);
int32_t Tutorial_Is1ea() { return hook::Global<int32_t>(0x6d3b4c) && TutorialStep() == 0x1ea ? 1 : 0; }
RT3_REPLACE(0x516610, Tutorial_Is1ea);

// 0x51e430 / 0x51e480. File exists / file size (exe CRT open, read-only binary).
int32_t __fastcall File_Exists(const char* path) {
    char buf[0x190];
    char* d = buf;
    while ((*d++ = *path++) != 0) {}
    int32_t fd = ExeOpen(buf, 0x8000);
    if (fd == -1) return 0;
    ExeClose(fd);
    return 1;
}
RT3_REPLACE(0x51e430, File_Exists);
int32_t __fastcall File_Size(const char* path) {
    char buf[0x190];
    char* d = buf;
    while ((*d++ = *path++) != 0) {}
    int32_t fd = ExeOpen(buf, 0x8000);
    if (fd == -1) return 0;
    int32_t size = ((int32_t(__cdecl*)(int32_t, int32_t, int32_t))0x5ac18d)(fd, 0, 2);
    ExeClose(fd);
    return size;
}
RT3_REPLACE(0x51e480, File_Size);

// 0x51e4e0. Copies text to the clipboard (CF_TEXT); 1 on success.
int32_t __fastcall Clipboard_SetText(const char* text) {
    if (!Import<int32_t(__stdcall*)(void*)>(0x5c825c)(nullptr)) return 0;
    if (!Import<int32_t(__stdcall*)()>(0x5c8258)()) return 0;
    size_t len = 0;
    while (text[len]) ++len;
    void* h = Import<void*(__stdcall*)(uint32_t, size_t)>(0x5c8208)(2, len + 1);
    void* p = Import<void*(__stdcall*)(void*)>(0x5c820c)(h);
    Import<char*(__stdcall*)(void*, const char*)>(0x5c8210)(p, text);
    Import<int32_t(__stdcall*)(void*)>(0x5c8214)(h);
    void* ok = Import<void*(__stdcall*)(uint32_t, void*)>(0x5c8224)(1, h);
    Import<int32_t(__stdcall*)()>(0x5c82d8)();
    return ok ? 1 : 0;
}
RT3_REPLACE(0x51e4e0, Clipboard_SetText);

// 0x51e560. Reads clipboard text into buf (size bytes); 1 on success.
int32_t __fastcall Clipboard_GetText(char* buf, int32_t size) {
    if (!Import<int32_t(__stdcall*)(void*)>(0x5c825c)(nullptr)) return 0;
    void* h = Import<void*(__stdcall*)(uint32_t)>(0x5c8260)(1);
    void* p = h ? Import<void*(__stdcall*)(void*)>(0x5c820c)(h) : nullptr;
    if (!p) {
        Import<int32_t(__stdcall*)()>(0x5c82d8)();
        return 0;
    }
    Import<char*(__stdcall*)(char*, const void*, int32_t)>(0x5c8204)(buf, p, size - 1);
    buf[size - 1] = 0;
    Import<int32_t(__stdcall*)(void*)>(0x5c8214)(h);
    Import<int32_t(__stdcall*)()>(0x5c82d8)();
    return 1;
}
RT3_REPLACE(0x51e560, Clipboard_GetText);

// 0x51e5d0. Replaces the first (or, with `all`, every) occurrence of `find`
// in str (buffer of `size` bytes) by `repl`.
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all) {
    for (;;) {
        char* tmp = (char*)Malloc(size + 0x1f4);
        char* p = (char*)StrFind(str, find, 0);
        int32_t found = 0;
        if (p) {
            *p = 0;
            char* d = tmp;
            const char* s = str;
            while ((*d++ = *s++) != 0) {}
            size_t flen = 0;
            while (find[flen]) ++flen;
            d = tmp;
            while (*d) ++d;
            for (s = repl; (*d++ = *s++) != 0;) {}
            d = tmp;
            while (*d) ++d;
            for (s = p + flen; (*d++ = *s++) != 0;) {}
            ExeStrncpy(str, tmp, size - 1);
            str[size - 1] = 0;
            found = 1;
        }
        Free(tmp);
        if (!found || !all) return;
    }
}
RT3_REPLACE(0x51e5d0, Str_Replace);

// 0x51e6e0. Non-empty and all decimal digits.
int32_t __fastcall Str_IsNumber(const char* s) {
    if (!s) return 0;
    size_t n = 0;
    while (s[n]) ++n;
    if (!n) return 0;
    for (size_t i = 0; i < n; ++i)
        if (s[i] < '0' || s[i] > '9') return 0;
    return 1;
}
RT3_REPLACE(0x51e6e0, Str_IsNumber);

// 0x51e720. Sets the debug log file name (0x6d3ca8, 100 chars) and resets its state.
void __fastcall DebugLog_SetFile(const char* name) {
    ExeStrncpy((char*)0x6d3ca8, name, 100);
    hook::Global<uint8_t>(0x6d3d0c) = 0;
    hook::Global<int32_t>(0x6d3d10) = 0;
}
RT3_REPLACE(0x51e720, DebugLog_SetFile);

} // namespace rt3
