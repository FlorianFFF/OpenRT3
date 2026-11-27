// The game's thin wrappers around Windows (files, clipboard, registry, OS
// version), original against rewrite. The clipboard, registry and version
// imports are replaced by fakes that log every call (through the exe's import
// slots, which both sides call through), so nothing outside the test process
// is touched: no real clipboard or registry access.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

namespace rt3 {
int32_t __fastcall File_Exists(const char* path);
int32_t __fastcall File_Size(const char* path);
int32_t __fastcall Clipboard_SetText(const char* text);
int32_t __fastcall Clipboard_GetText(char* buf, int32_t size);
uint8_t __fastcall SysInfo_IsWin9x();
void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root);
uint32_t __fastcall HandleObj_Destruct(void* o);
void* __fastcall HandleObj_Delete(void* o, int, uint32_t flags);
int32_t __fastcall RegKey_SetString(void* o, int, const char* subKey, const char* name, const char* data);
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def);
}

namespace {

// Import slot swaps, undone at the end of the test.
struct Slots {
    struct S { void** at; void* old; } s[24];
    int n = 0;
    void Set(uint32_t slot, void* fake) {
        DWORD o;
        VirtualProtect((void*)slot, 4, PAGE_READWRITE, &o);
        s[n++] = { (void**)slot, *(void**)slot };
        *(void**)slot = fake;
    }
    ~Slots() {
        while (n) {
            --n;
            *s[n].at = s[n].old;
        }
    }
};

// The call log: one line per import call, pointers to the test's own buffers
// named, strings by content.
char g_log[8192];
int g_logLen;
void Log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (g_logLen < (int)sizeof g_log - 256) g_logLen += vsprintf(g_log + g_logLen, fmt, ap);
    va_end(ap);
}

// Scripted import results for the current case.
struct Script {
    int32_t open, empty, setData, getData, lock, version, platform, create, set, query, queryType;
} g_s;
char g_clip[64];      // the "clipboard memory" GlobalAlloc / GlobalLock hand out
char g_clipText[64];  // what GetClipboardData's memory holds

int32_t __stdcall FakeOpenClipboard(void* w) { Log("Open %p;", w); return g_s.open; }
int32_t __stdcall FakeEmptyClipboard() { Log("Empty;"); return g_s.empty; }
int32_t __stdcall FakeCloseClipboard() { Log("Close;"); return 1; }
void* __stdcall FakeGlobalAlloc(uint32_t flags, size_t size) { Log("Alloc %x %u;", flags, (unsigned)size); return (void*)0x1234; }
void* __stdcall FakeGlobalLock(void* h) {
    Log("Lock %p;", h);
    if (!g_s.lock) return nullptr;
    return h == (void*)0x1234 ? (void*)g_clip : h == (void*)0x5678 ? (void*)g_clipText : nullptr;
}
int32_t __stdcall FakeGlobalUnlock(void* h) { Log("Unlock %p;", h); return 1; }
void* __stdcall FakeSetClipboardData(uint32_t fmt, void* h) { Log("Set %u %p [%s];", fmt, h, g_clip); return g_s.setData ? h : nullptr; }
void* __stdcall FakeGetClipboardData(uint32_t fmt) { Log("Get %u;", fmt); return g_s.getData ? (void*)0x5678 : nullptr; }

int32_t __stdcall FakeGetVersionEx(uint32_t* v) {
    Log("Version size %u;", v[0]);
    if (!g_s.version) return 0;
    v[1] = 5;
    v[2] = 1;
    v[3] = 2600;
    v[4] = (uint32_t)g_s.platform;
    return 1;
}

// Registry: handles are small numbers; values in a one-slot "registry".
char g_regValue[64];
int32_t __stdcall FakeRegCreateKeyEx(void* root, const char* sub, uint32_t res, void* cls, uint32_t opt, uint32_t sam, void* sec, void** out,
                                     uint32_t* disp) {
    Log("Create %p [%s] %u %p %u %x %p;", root, sub ? sub : "(null)", res, cls, opt, sam, sec);
    if (g_s.create) return g_s.create;  // failed: *out untouched
    *out = (void*)0x77;
    if (disp) *disp = 1;
    return 0;
}
int32_t __stdcall FakeRegCloseKey(void* k) { Log("CloseKey %p;", k); return 0; }
int32_t __stdcall FakeRegSetValueEx(void* k, const char* name, uint32_t res, uint32_t type, const void* data, uint32_t size) {
    Log("SetValue %p [%s] %u %u [%s] %u;", k, name ? name : "(null)", res, type, (const char*)data, size);
    if (!g_s.set) strncpy(g_regValue, (const char*)data, sizeof g_regValue - 1);
    return g_s.set;
}
int32_t __stdcall FakeRegQueryValueEx(void* k, const char* name, uint32_t* res, uint32_t* type, uint8_t* data, uint32_t* size) {
    Log("Query %p [%s] %p %u;", k, name ? name : "(null)", res, size ? *size : 0);
    if (g_s.query) return g_s.query;
    if (type) *type = (uint32_t)g_s.queryType;
    if (data) strcpy((char*)data, g_regValue);
    if (size) *size = (uint32_t)strlen(g_regValue) + 1;
    return 0;
}

}  // namespace

DIFFTEST(os_FileQueries) {
    // Files of the game folder (the test runs in it), read only; and missing ones.
    char exe[MAX_PATH];
    GetModuleFileNameA(nullptr, exe, sizeof exe);
    const char* paths[] = { exe, "Data\\Configuration\\game.cfg", "Data", "no such file.xyz", "", "Data\\no\\such\\dir\\x" };
    auto oExists = (int32_t(__fastcall*)(const char*))0x51e430;
    auto oSize = (int32_t(__fastcall*)(const char*))0x51e480;
    for (const char* p : paths) {
        EXPECT_EQ(oExists(p), rt3::File_Exists(p), "exists %s", p);
        EXPECT_EQ(oSize(p), rt3::File_Size(p), "size %s", p);
    }
}

DIFFTEST(os_Clipboard) {
    Slots slots;
    slots.Set(0x5c825c, (void*)&FakeOpenClipboard);
    slots.Set(0x5c8258, (void*)&FakeEmptyClipboard);
    slots.Set(0x5c82d8, (void*)&FakeCloseClipboard);
    slots.Set(0x5c8208, (void*)&FakeGlobalAlloc);
    slots.Set(0x5c820c, (void*)&FakeGlobalLock);
    slots.Set(0x5c8214, (void*)&FakeGlobalUnlock);
    slots.Set(0x5c8224, (void*)&FakeSetClipboardData);
    slots.Set(0x5c8260, (void*)&FakeGetClipboardData);
    auto oSet = (int32_t(__fastcall*)(const char*))0x51e4e0;
    auto oGet = (int32_t(__fastcall*)(char*, int32_t))0x51e560;
    const char* texts[] = { "", "x", "Hello, clipboard", "0123456789012345678901234567890" };
    for (int c = 0; c < 32; ++c) {
        g_s = {};
        g_s.open = c & 1;
        g_s.empty = (c >> 1) & 1;
        g_s.setData = (c >> 2) & 1;
        g_s.getData = (c >> 3) & 1;
        g_s.lock = (c >> 4) & 1;
        for (const char* t : texts) {
            strcpy(g_clipText, t);
            char logA[8192], bufA[32], bufB[32];
            static const int kSizes[] = { 1, 2, 5, 32 };
            for (int size : kSizes) {
                memset(g_clip, 0, sizeof g_clip);
                g_logLen = 0;
                g_log[0] = 0;
                int32_t ra = oSet(t);
                memset(bufA, 0x5a, sizeof bufA);
                int32_t ga = oGet(bufA, size);
                strcpy(logA, g_log);
                memset(g_clip, 0, sizeof g_clip);
                g_logLen = 0;
                g_log[0] = 0;
                int32_t rb = rt3::Clipboard_SetText(t);
                memset(bufB, 0x5a, sizeof bufB);
                int32_t gb = rt3::Clipboard_GetText(bufB, size);
                EXPECT_EQ(ra, rb, "set case %d text [%s]", c, t);
                EXPECT_EQ(ga, gb, "get case %d size %d", c, size);
                EXPECT_EQ(memcmp(bufA, bufB, sizeof bufA), 0, "buffer case %d size %d", c, size);
                EXPECT_EQ(strcmp(logA, g_log), 0, "calls case %d: [%s] vs [%s]", c, logA, g_log);
            }
        }
    }
}

DIFFTEST(os_IsWin9x) {
    Slots slots;
    slots.Set(0x5c81d4, (void*)&FakeGetVersionEx);
    auto o = (uint8_t(__fastcall*)())0x55d460;
    uint8_t* buf = (uint8_t*)0xd97768;
    uint8_t keep[0x100];
    memcpy(keep, buf, sizeof keep);
    for (int c = 0; c < 10; ++c) {
        g_s = {};
        g_s.version = c != 0;
        g_s.platform = c - 1;
        uint8_t a[0x100];
        memset(buf, 0x33, 0x100);
        g_logLen = 0;
        uint32_t ra = 0xabcd0000u | o();
        memcpy(a, buf, sizeof a);
        char logA[256];
        strcpy(logA, g_log);
        memset(buf, 0x33, 0x100);
        g_logLen = 0;
        uint32_t rb = 0xabcd0000u | rt3::SysInfo_IsWin9x();
        EXPECT_EQ(ra, rb, "case %d", c);
        EXPECT_EQ(memcmp(a, buf, sizeof a), 0, "buffer case %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "calls case %d", c);
    }
    memcpy(buf, keep, sizeof keep);
}

DIFFTEST(os_RegistryKey) {
    Slots slots;
    slots.Set(0x5c8008, (void*)&FakeRegCreateKeyEx);
    slots.Set(0x5c800c, (void*)&FakeRegCloseKey);
    slots.Set(0x5c8014, (void*)&FakeRegSetValueEx);
    slots.Set(0x5c8000, (void*)&FakeRegQueryValueEx);
    char* shared = (char*)0x6adbb8;  // the value buffer both sides read into
    char keep[0x400];
    memcpy(keep, shared, sizeof keep);
    auto oCons = (void*(__fastcall*)(uint32_t*, int, const char*, void*))0x4686f0;
    auto oDestruct = (uint32_t(__fastcall*)(void*))0x468740;
    auto oDelete = (void*(__fastcall*)(void*, int, uint32_t))0x468970;
    auto oSetS = (int32_t(__fastcall*)(void*, int, const char*, const char*, const char*))0x4689a0;
    auto oGetS = (const char*(__fastcall*)(void*, int, const char*, const char*, const char*))0x468a10;
    auto newObj = (void*(__cdecl*)(size_t))0x53b070;  // the game's operator new (the deleting form frees with its delete)
    const char* defs[] = { nullptr, "", "default value" };
    for (int c = 0; c < 64; ++c) {
        for (const char* def : defs) {
            g_s = {};
            g_s.create = (c & 1) ? 5 : 0;
            g_s.set = (c & 2) ? 6 : 0;
            g_s.query = (c & 4) ? 2 : (c & 8) ? 7 : 0;
            g_s.queryType = (c & 16) ? 3 : 1;
            const int deleting = (c >> 5) & 1;
            char logA[8192], valueA[64], sharedA[0x40];
            uint32_t a[4] = { 0, 0x11, 0x22, 0x33 }, b[4] = { 0, 0x11, 0x22, 0x33 };
            const char* ra[3];
            const char* rb[3];
            // Original.
            g_logLen = 0;
            g_log[0] = 0;
            strcpy(g_regValue, "stored");
            memset(shared, 0x44, 0x40);
            oCons(a, 0, "Software\\Test", (void*)0x80000001);
            int32_t sa = oSetS(a, 0, "Sub", "Name", "Data text");
            ra[0] = oGetS(a, 0, "Sub", "Name", def);
            ra[1] = (const char*)(uintptr_t)oDestruct(a);
            void* da = nullptr;
            if (deleting) {
                uint32_t* h = (uint32_t*)newObj(12);
                h[0] = 0x5ce15c; h[1] = 0x55; h[2] = 0;
                da = oDelete(h, 0, 1);
                ra[2] = da ? "obj" : nullptr;
            } else {
                da = oDelete(a, 0, 0);
                ra[2] = da == a ? "self" : da ? "other" : nullptr;
            }
            strcpy(logA, g_log);
            strcpy(valueA, g_regValue);
            memcpy(sharedA, shared, sizeof sharedA);
            // Rewrite.
            g_logLen = 0;
            g_log[0] = 0;
            strcpy(g_regValue, "stored");
            memset(shared, 0x44, 0x40);
            rt3::RegKey_Construct(b, 0, "Software\\Test", (void*)0x80000001);
            int32_t sb = rt3::RegKey_SetString(b, 0, "Sub", "Name", "Data text");
            rb[0] = rt3::RegKey_GetString(b, 0, "Sub", "Name", def);
            rb[1] = (const char*)(uintptr_t)rt3::HandleObj_Destruct(b);
            void* db;
            if (deleting) {
                uint32_t* h = (uint32_t*)newObj(12);
                h[0] = 0x5ce15c; h[1] = 0x55; h[2] = 0;
                db = rt3::HandleObj_Delete(h, 0, 1);
                rb[2] = db ? "obj" : nullptr;
            } else {
                db = rt3::HandleObj_Delete(b, 0, 0);
                rb[2] = db == b ? "self" : db ? "other" : nullptr;
            }
            EXPECT_EQ(memcmp(a, b, sizeof a), 0, "object case %d", c);
            EXPECT_EQ(sa, sb, "set case %d", c);
            EXPECT_EQ(ra[0] == def, rb[0] == def, "get result case %d", c);
            EXPECT_EQ(ra[0] == shared, rb[0] == shared, "get result (buffer) case %d", c);
            EXPECT_EQ(ra[1], rb[1], "close result case %d", c);
            EXPECT_EQ(ra[2] == rb[2] || (ra[2] && rb[2] && !strcmp(ra[2], rb[2])) ? 1 : 0, 1, "delete case %d", c);
            EXPECT_EQ(strcmp(logA, g_log), 0, "calls case %d: [%s] vs [%s]", c, logA, g_log);
            EXPECT_EQ(strcmp(valueA, g_regValue), 0, "stored value case %d", c);
            EXPECT_EQ(memcmp(sharedA, shared, sizeof sharedA), 0, "value buffer case %d", c);
        }
    }
    memcpy(shared, keep, sizeof keep);
}
