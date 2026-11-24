// In-game differential tests for view routines reversed by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall View_SetToolMode(uint8_t*, int, uint32_t, int32_t);
void __stdcall World_SetEditorButtons(uint32_t, uint32_t);
void __fastcall View_ResetEditorTool(uint8_t*);
void __fastcall App_SetViewMode(uint8_t*, int, int32_t);
void __fastcall View_DestroyToolForMode(uint8_t*, int, uint32_t);
int32_t __stdcall View_OnModeInput(const uint32_t*);
}
namespace {
template <typename T> T& F(void* p, uint32_t o) { return *(T*)((uint8_t*)p + o); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } patches[48];
    int count = 0;
    void Add(uintptr_t address, void* fn) {
        Patch& p = patches[count++]; p.at = (uint8_t*)address;
        DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.old, p.at, 5); p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)fn - address - 5);
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    void Pair(uintptr_t original, void* cpp, void* fn) { Add(original, fn); Add((uintptr_t)cpp, fn); }
    ~Redirects() {
        while (count) {
            Patch& p = patches[--count]; DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.old, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
const uint32_t kGlobals[] = {
    0x6d0818, 0x6d0820, 0x62be68, 0x6cec78, 0x6cfe4c, 0x6d12f0,
    0x6d3b4c, 0x6d12d8, 0x6d401c, 0x6cfca8, 0x62c120
};
struct Fixture {
    uint8_t view[0xa0], app[0x80], tools[3][0x200];
    uint32_t globals[sizeof kGlobals / sizeof kGlobals[0]], log[2048];
    int used, sent, result;
};
Fixture g_f;
uint32_t g_bits, g_case;
int g_mode;
uint32_t Id(void* p) {
    if (p == g_f.view) return 1;
    if (p == g_f.app) return 2;
    for (int i = 0; i < 3; ++i) if (p == g_f.tools[i]) return 10 + i;
    return (uint32_t)(uintptr_t)p;
}
void Log(uint32_t a, uint32_t b = 0, uint32_t c = 0) {
    g_f.log[g_f.used++] = a; g_f.log[g_f.used++] = b; g_f.log[g_f.used++] = c;
}
void __fastcall Destroy(uint8_t* view, int, uint32_t mode) {
    Log(0x4dd950, Id(view), mode);
    Log(F<uint32_t>(view, 0x8c), Id(F<void*>(view, 0x88)));
    F<void*>(view, 0x88) = nullptr;
    if (g_bits & 0x200000) {
        const uint32_t modes[] = {9, 12, 0, 27};
        F<uint32_t>(view, 0x8c) = modes[(g_bits >> 22) & 3];
    }
}
void __stdcall Buttons(uint32_t mode, uint32_t disabled) {
    Log(0x433da0, mode, disabled);
    if (g_bits & 0x100000) F<uint32_t>(g_f.view, 0x8c) = 9;
}
void* __cdecl Allocate(uint32_t bytes) {
    Log(0x53b070, bytes, Id(F<void*>(g_f.view, 0x88)));
    return g_bits & 0x1000000 ? nullptr : g_f.tools[1];
}
template <uint32_t Address> uint8_t* __fastcall Construct(uint8_t* memory) {
    Log(Address, Id(memory), F<uint32_t>(g_f.view, 0x8c));
    F<uint32_t>(memory, 0) = Address;
    if (g_bits & 0x80000) F<uint32_t>(g_f.view, 0x8c) = 12;
    if (g_bits & 0x40000) return nullptr;
    return g_bits & 0x20000 ? g_f.tools[2] : memory;
}
template <uint32_t Address> void __fastcall Method(void* self) {
    Log(Address, Id(self), F<uint32_t>(g_f.view, 0x8c));
    if (Address == 0x4c2ca0 && (g_bits & 0x10000)) F<uint32_t>(g_f.view, 0x8c) = 9;
    if (Address == 0x4dd410 && (g_bits & 0x8000)) F<uint32_t>(g_f.view, 0x8c) = g_bits & 0x4000 ? 2 : 9;
    if (Address == 0x4dd630 && (g_bits & 0x2000)) F<uint32_t>(g_f.view, 0x8c) = 12;
}
template <uint32_t Address> void Call() {
    Log(Address, Id(F<void*>(g_f.view, 0x88)), F<uint32_t>(g_f.app, 0x68));
    if (Address == 0x4d44a0 && (g_bits & 0x1000)) *(void**)0x6d0818 = g_f.tools[0];
}
void __fastcall Picker(void* picker, int, int32_t tab, int32_t value) {
    Log(0x4f4970, Id(picker), (uint32_t)tab); Log((uint32_t)value);
    if (g_bits & 0x800) *(uint32_t*)0x6d3b4c ^= 1;
}
void __fastcall Insert(void* manager, int, uint8_t* tool, int32_t select) {
    Log(0x538e50, Id(manager), Id(tool)); Log((uint32_t)select, Id(F<void*>(g_f.view, 0x88)));
}
void __fastcall Event(void* self, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Log(0x540120, Id(self), type); Log(id, a, b); Log(c);
    ++g_f.sent;
    if (g_mode == 1 && g_f.sent == 1 && (g_bits & 0x400)) *(uint32_t*)0x6d0820 = 0x821;
    if (g_mode == 1 && id == 0x7ef && (g_bits & 0x200)) *(void**)0x6d0818 = g_f.tools[0];
}
void __fastcall SetMode(uint8_t* view, int, uint32_t mode, int32_t selected) {
    Log(0x4ddbd0, Id(view), mode); Log((uint32_t)selected);
    F<uint32_t>(view, 0x8c) = mode; F<int32_t>(view, 0x90) = selected;
}
void __fastcall Reset(uint8_t* view) { Log(0x4dfda0, Id(view)); F<uint32_t>(view, 0x8c) = 0; }
void __fastcall Editor(void* table, int, uint32_t mode) { Log(0x43a0a0, Id(table), mode); }
void __fastcall Graphics(void* app, int, int32_t which, int32_t rebuild) {
    Log(0x484d70, Id(app), (uint32_t)which); Log((uint32_t)rebuild, F<uint32_t>(app, 0x68));
}
void Setup(uint32_t it, int mode, bool nested) {
    memset(&g_f, 0, sizeof g_f); g_case = it; g_mode = mode;
    uint32_t r = it * 2654435761u + 65537;
    auto rnd = [&]() { r = r * 1103515245u + 12345u; return r; };
    g_bits = rnd();
    for (uint8_t& b : g_f.view) b = (uint8_t)(rnd() >> 16);
    for (uint8_t& b : g_f.app) b = (uint8_t)(rnd() >> 16);
    for (auto& tool : g_f.tools) for (uint8_t& b : tool) b = (uint8_t)(rnd() >> 16);
    uint32_t oldMode = mode == 2 ? it % 40 : (it / 32) % 32;
    if (nested) oldMode = 25;
    if (oldMode == 39) oldMode = 0xffffffff;
    F<uint32_t>(g_f.view, 0x8c) = oldMode;
    F<int32_t>(g_f.view, 0x90) = (int32_t)((g_bits >> 8) % 21) - 1;
    F<void*>(g_f.view, 0x88) = g_bits & 0x80 ? g_f.tools[0] : nullptr;
    const int32_t appModes[] = {0, 1, 2, -1};
    F<int32_t>(g_f.app, 0x68) = appModes[(it / 4) % 4];
    *(void**)0x6d0818 = mode == 3 && (g_bits & 0x40) ? nullptr : g_f.view;
    *(uint32_t*)0x6d0820 = 0x820;
    *(uint32_t*)0x62be68 = 0xbe68;
    *(uint32_t*)0x6cec78 = 0xcec78;
    *(uint32_t*)0x6cfe4c = g_bits & 0x20 ? 0xfe4c : 0;
    *(uint32_t*)0x6d12f0 = 0x12f0;
    *(uint32_t*)0x6d3b4c = (g_bits >> 4) & 1;
    *(uint32_t*)0x6d12d8 = rnd();
    *(uint32_t*)0x6d401c = 0x401c;
    *(uint32_t*)0x6cfca8 = 0xfca8;
    *(uint32_t*)0x62c120 = g_bits & 8 ? 0xc120 : 0;
}
void RunCases(int mode, uintptr_t original, void* cpp, uint32_t count, bool nested = false) {
    uint32_t saved[sizeof kGlobals / sizeof kGlobals[0]];
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) saved[i] = *(uint32_t*)kGlobals[i];
    for (uint32_t it = 0; it < count; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it, mode, nested); void* fn = side ? cpp : (void*)original;
            if (!mode) {
                int32_t selections[] = {-1, 0, 17, F<int32_t>(g_f.view, 0x90)};
                uint32_t newMode = it % 32;
                if (newMode == 31) newMode = 0xffffffff;
                ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))fn)(
                    g_f.view, 0, newMode, selections[(it / (nested ? 32 : 1024)) % 4]);
            } else if (mode == 1) ((void(__stdcall*)(uint32_t, uint32_t))fn)(g_bits, g_bits >> 8);
            else if (mode == 2) ((void(__fastcall*)(uint8_t*))fn)(g_f.view);
            else if (mode == 3) {
                const int32_t modes[] = {0, 1, 2, -1};
                ((void(__fastcall*)(uint8_t*, int, int32_t))fn)(g_f.app, 0, modes[it % 4]);
            } else {
                const uint32_t types[] = {3, 1000, 0, 999, 1001, 0xffffffff, 0x800003e8, 0x80000003};
                uint32_t event[] = {types[it % 8], ~g_bits, g_bits};
                g_f.result = ((int32_t(__stdcall*)(const uint32_t*))fn)(event);
            }
            for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) g_f.globals[i] = *(uint32_t*)kGlobals[i];
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                EXPECT_EQ(1, 0, "view mode %d case %u byte %u: %02x / %02x (events %d / %d)",
                          mode, it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off],
                          expected.used, g_f.used);
            }
        }
    }
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) *(uint32_t*)kGlobals[i] = saved[i];
}
}

namespace {
void PatchController(Redirects& p, bool fakeButtons) {
    if (fakeButtons) p.Pair(0x433da0, (void*)&rt3::World_SetEditorButtons, (void*)&Buttons);
    p.Pair(0x4dd950, (void*)&rt3::View_DestroyToolForMode, (void*)&Destroy);
    p.Add(0x53b070, (void*)&Allocate);
#define CTOR(a) p.Add(a, (void*)&Construct<a>)
    CTOR(0x515f50); CTOR(0x514420); CTOR(0x5074c0); CTOR(0x5068c0); CTOR(0x509d80);
    CTOR(0x4c7200); CTOR(0x4c5540); CTOR(0x4f5020); CTOR(0x4b81d0); CTOR(0x50c290);
    CTOR(0x50e400); CTOR(0x4b8720); CTOR(0x4bcb50); CTOR(0x4bcdd0); CTOR(0x4f7ce0);
    CTOR(0x4fcf30); CTOR(0x4f54d0); CTOR(0x4f5670);
#undef CTOR
    p.Add(0x4c2ca0, (void*)&Method<0x4c2ca0>);
    p.Add(0x4dd410, (void*)&Method<0x4dd410>);
    p.Add(0x4d44f0, (void*)&Call<0x4d44f0>);
    p.Add(0x514410, (void*)&Call<0x514410>);
    p.Add(0x4f4970, (void*)&Picker);
    p.Add(0x540120, (void*)&Event);
    p.Add(0x4557b0, (void*)&Call<0x4557b0>);
    p.Add(0x538e50, (void*)&Insert);
    p.Add(0x4dd630, (void*)&Method<0x4dd630>);
    p.Add(0x493150, (void*)&Method<0x493150>);
    p.Add(0x43a0a0, (void*)&Editor);
}
}
DIFFTEST(game_ViewSetToolMode) {
    Redirects p;
    PatchController(p, true);
    RunCases(0, 0x4ddbd0, (void*)&rt3::View_SetToolMode, 4096);
}
DIFFTEST(game_ViewEditorModeCycle) {
    Redirects p;
    PatchController(p, false);
    RunCases(0, 0x4ddbd0, (void*)&rt3::View_SetToolMode, 256, true);
}
DIFFTEST(game_WorldSetEditorButtons) {
    Redirects p;
    p.Pair(0x4ddbd0, (void*)&rt3::View_SetToolMode, (void*)&SetMode);
    p.Add(0x540120, (void*)&Event);
    p.Add(0x43a0a0, (void*)&Editor);
    RunCases(1, 0x433da0, (void*)&rt3::World_SetEditorButtons, 1024);
}
DIFFTEST(game_ViewResetEditorTool) {
    Redirects p;
    p.Pair(0x4ddbd0, (void*)&rt3::View_SetToolMode, (void*)&SetMode);
    RunCases(2, 0x4dfda0, (void*)&rt3::View_ResetEditorTool, 1024);
}
DIFFTEST(game_AppSetViewMode) {
    Redirects p;
    p.Pair(0x4dfda0, (void*)&rt3::View_ResetEditorTool, (void*)&Reset);
    p.Add(0x4d44a0, (void*)&Call<0x4d44a0>);
    p.Add(0x4d4500, (void*)&Call<0x4d4500>);
    p.Add(0x44fb70, (void*)&Method<0x44fb70>);
    p.Add(0x484d70, (void*)&Graphics);
    RunCases(3, 0x482e50, (void*)&rt3::App_SetViewMode, 1024);
}
DIFFTEST(game_ViewOnModeInput) {
    Redirects p;
    p.Pair(0x4ddbd0, (void*)&rt3::View_SetToolMode, (void*)&SetMode);
    RunCases(4, 0x4338d0, (void*)&rt3::View_OnModeInput, 1024);
}
