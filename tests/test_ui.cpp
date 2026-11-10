// Difftests for UI builders and draw callbacks: the leaf calls they make (text,
// glyphs, lines, widget constructors, window events, strings, allocations) are
// redirected to recorders, and the original's call log is compared with the
// rewrite's. Allocations come from a bump arena that restarts for each side, so
// pointers to new objects and strings match when the calls do.
#include <limits>
#include <vector>
#include "core/containers.h"
#include "game/collection.h"
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace rt3 {
void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                 int32_t valign, uint32_t flags, float scale);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t,
                            float, float, float, float);
void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
void __fastcall Window_AddChild(uint8_t* w, int, uint8_t* c, int32_t);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall String_Assign(char** s, const char* src);
void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign,
                              uint32_t flags, const char* tip, int32_t id, uint32_t color, uint32_t hover);
}

using rt3::Collection;
namespace {

uint32_t g_seed = 777;
uint32_t Rnd() {
    g_seed = g_seed * 1103515245u + 12345u;
    return g_seed ^ (g_seed >> 16);
}
float RndF(float lo, float hi) { return lo + (hi - lo) * (float)((Rnd() >> 8) & 0xffff) / 65535.0f; }

// --- recording ---------------------------------------------------------------

std::string g_log;
uint8_t g_arena[0x40000];
uint32_t g_arenaUsed;

void* ArenaAlloc(size_t n) {
    n = (n + 7) & ~7u;
    if (g_arenaUsed + n > sizeof g_arena) return nullptr;
    void* p = g_arena + g_arenaUsed;
    memset(p, 0, n);
    g_arenaUsed += (uint32_t)n;
    return p;
}
bool InArena(const void* p) { return (const uint8_t*)p >= g_arena && (const uint8_t*)p < g_arena + sizeof g_arena; }

void Logf(const char* fmt, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    g_log += buf;
    g_log += '\n';
}
uint32_t B(float f) { return *(uint32_t*)&f; }
// Text arguments may live in the caller's stack frame, whose address differs
// between the two implementations: log stack and arena strings by content.
bool OnStack(uint32_t v) {
    uint32_t lo = __readfsdword(8), hi = __readfsdword(4);
    return v >= lo && v < hi;
}
std::string Arg(uint32_t v) {
    if (v && (OnStack(v) || InArena((void*)(uintptr_t)v))) return std::string("\"") + (const char*)(uintptr_t)v + "\"";
    char b[16];
    sprintf(b, "%08x", v);
    return b;
}

void __fastcall RecText(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                        uint32_t flags, float scale) {
    Logf("text %p \"%s\" %08x %08x %08x %08x %08x %d %d %x %08x", f, text, B(x), B(y), B(w), B(h), color, align, valign, flags, B(scale));
}
void __fastcall RecGlyph(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t a, int32_t b, int32_t c,
                         float f1, float f2, float f3, float f4) {
    Logf("glyph %p %08x %08x %08x %08x %08x %08x %d %d %d %08x %08x %08x %08x", g, B(x), B(y), tint, B(sx), B(sy), color, a, b, c, B(f1), B(f2), B(f3),
         B(f4));
}
void __fastcall RecLine(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width) {
    Logf("line %08x %d %08x %08x %08x %08x %08x", color, glow, B(x1), B(y1), B(x2), B(y2), B(width));
}
void __fastcall RecAddChild(uint8_t* w, int, uint8_t* c, int32_t k) { Logf("child %p %p %d", w, c, k); }
int32_t __fastcall RecEvent(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Logf("event %p %x %x %s %s %s", w, type, id, Arg(a).c_str(), Arg(b).c_str(), Arg(c).c_str());
    return 0;
}
void LogDef(const char* kind, uint8_t* w, const uint8_t* def, uint32_t n) {
    std::string s = std::string(kind) + " ";
    char b[8];
    for (uint32_t k = 0; k < n; ++k) {
        sprintf(b, "%02x", def[k]);
        s += b;
    }
    Logf("%s -> %p", s.c_str(), w);
}
uint8_t* __fastcall RecScrollbar(uint8_t* w, int, const uint8_t* def) { LogDef("scrollbar", w, def, 0x89); return w; }
uint8_t* __fastcall RecListBox(uint8_t* w, int, const uint8_t* def) { LogDef("listbox", w, def, 0xe3); return w; }
uint8_t* __fastcall RecEditBox(uint8_t* w, int, const uint8_t* def) { LogDef("editbox", w, def, 0x91); return w; }
uint8_t* __fastcall RecControl(uint8_t* w, int, const uint8_t* def) { LogDef("control", w, def, 0xa5); return w; }
uint8_t* __fastcall RecTimer(uint8_t* w, int, const uint8_t* def) { LogDef("timer", w, def, 0x8b); return w; }
void __fastcall RecAssign(char** s, const char* src) {
    size_t n = src ? strlen(src) : 0;
    char* d = (char*)ArenaAlloc(n + 1);
    if (n) memcpy(d, src, n);
    *s = d;
    // A def being filled sits in the caller's frame; the string's arena address
    // shows up in the def bytes when it is constructed.
    if (OnStack((uint32_t)(uintptr_t)s)) Logf("assign stack \"%s\"", src ? src : "(null)");
    else Logf("assign %p \"%s\"", (void*)s, src ? src : "(null)");
}
void* __cdecl RecNew(size_t n) {
    void* p = ArenaAlloc(n);
    Logf("new %x -> %p", (uint32_t)n, p);
    return p;
}
void __cdecl RecFree(void* p) {
    if (InArena(p) || !p) Logf("free %p", p);
}
void __fastcall RecTextLink(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign,
                            uint32_t flags, const char* tip, int32_t id, uint32_t color, uint32_t hover) {
    Logf("link %p %p \"%s\" %08x %08x %08x %08x %d %d %x \"%s\" %x %08x %08x", win, font, text, B(x), B(y), B(w), B(h), align, valign, flags,
         tip ? tip : "", id, color, hover);
}

// --- hooks -------------------------------------------------------------------

struct Patch {
    uint8_t* at;
    uint8_t saved[5];
};
Patch g_patches[128];
int g_npatch;

void Hook(void* target, void* fn) {
    uint8_t* p = (uint8_t*)target;
    DWORD prot;
    VirtualProtect(p, 5, PAGE_EXECUTE_READWRITE, &prot);
    Patch& pt = g_patches[g_npatch++];
    pt.at = p;
    memcpy(pt.saved, p, 5);
    p[0] = 0xe9;
    *(int32_t*)(p + 1) = (int32_t)((uint8_t*)fn - (p + 5));
    VirtualProtect(p, 5, prot, &prot);
    FlushInstructionCache(GetCurrentProcess(), p, 5);
}

// Redirects the UI leaves for the lifetime of the object. The rewrite calls the
// rt3 functions directly and the original reaches them through the exe's JMPs,
// so patching the rt3 entry points catches both; allocations and frees are
// called through the exe addresses by both.
struct UiTrace {
    UiTrace() {
        g_npatch = 0;
        Add(0x5519f0, (void*)&rt3::Font_DrawTextBox, (void*)&RecText);
        Add(0x552560, (void*)&rt3::Glyph_Queue, (void*)&RecGlyph);
        Add(0x54fb10, (void*)&rt3::Ui_Line, (void*)&RecLine);
        Add(0x53f9c0, (void*)&rt3::Window_AddChild, (void*)&RecAddChild);
        Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&RecEvent);
        Add(0x55a040, (void*)&rt3::Scrollbar_Construct, (void*)&RecScrollbar);
        Add(0x55ab50, (void*)&rt3::ListBox_Construct, (void*)&RecListBox);
        Add(0x55ba70, (void*)&rt3::EditBox_Construct, (void*)&RecEditBox);
        Add(0x540430, (void*)&rt3::Control_Construct, (void*)&RecControl);
        Add(0x563210, (void*)&rt3::TimerWidget_Construct, (void*)&RecTimer);
        Add(0x51d820, (void*)&rt3::String_Assign, (void*)&RecAssign);
        Add(0x563290, (void*)&rt3::TextLink_Draw, (void*)&RecTextLink);
        Hook((void*)0x53b070, (void*)&RecNew);
        Hook((void*)0x5a1145, (void*)&RecFree);
    }
    // Redirects a function for this test: the exe original (which the
    // original code calls; difftests run without the JMP replacements) and its
    // rewrite (called directly by the rewrite), or just one of them.
    void Add(uint32_t exe, void* ours, void* fn) {
        if (exe) Hook((void*)exe, fn);
        if (ours) Hook(ours, fn);
    }
    ~UiTrace() {
        for (int k = g_npatch - 1; k >= 0; --k) {
            Patch& pt = g_patches[k];
            DWORD prot;
            VirtualProtect(pt.at, 5, PAGE_EXECUTE_READWRITE, &prot);
            memcpy(pt.at, pt.saved, 5);
            VirtualProtect(pt.at, 5, prot, &prot);
            FlushInstructionCache(GetCurrentProcess(), pt.at, 5);
        }
        g_npatch = 0;
    }
    // Runs one side from a fresh log and arena; returns its log.
    template <typename Fn> std::string Run(Fn fn) {
        g_log.clear();
        g_arenaUsed = 0;
        fn();
        return g_log;
    }
};

// Debug aid: while alive, a watcher thread dumps the test thread's EIP and the
// first stack words to the log if one step takes longer than `ms`.
struct StallWatch {
    HANDLE thread = nullptr, target = nullptr, stop = nullptr;
    DWORD ms;
    explicit StallWatch(DWORD timeout) : ms(timeout) {
        DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &target, 0, FALSE, DUPLICATE_SAME_ACCESS);
        stop = CreateEventA(nullptr, TRUE, FALSE, nullptr);
        thread = CreateThread(nullptr, 0, &Run, this, 0, nullptr);
    }
    ~StallWatch() {
        SetEvent(stop);
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
        CloseHandle(stop);
        CloseHandle(target);
    }
    static DWORD WINAPI Run(void* p) {
        StallWatch* w = (StallWatch*)p;
        if (WaitForSingleObject(w->stop, w->ms) != WAIT_TIMEOUT) return 0;
        SuspendThread(w->target);
        CONTEXT c;
        memset(&c, 0, sizeof c);
        c.ContextFlags = CONTEXT_FULL;
        GetThreadContext(w->target, &c);
        char line[512];
        int n = wsprintfA(line, "  STALL eip %08x esp %08x:", c.Eip, c.Esp);
        // Words that look like code addresses (exe or this DLL), deepest first.
        HMODULE self = nullptr;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (const char*)&Run, &self);
        uint32_t lo = (uint32_t)(uintptr_t)self, hi = lo + 0x400000;
        for (int k = 0; k < 4096 && n < 480; ++k) {
            MEMORY_BASIC_INFORMATION mi;
            uint32_t a = c.Esp + k * 4;
            if (!VirtualQuery((void*)a, &mi, sizeof mi) || mi.State != MEM_COMMIT) break;
            uint32_t v = *(uint32_t*)a;
            if ((v >= 0x401000 && v < 0x5c8000) || (v >= lo && v < hi)) n += wsprintfA(line + n, " %08x", v >= lo ? v - lo + 0x10000000 : v);
        }
        line[n++] = 13;
        line[n++] = 10;
        DWORD wr;
        HANDLE f = CreateFileA("stall_test.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, 0, nullptr);
        WriteFile(f, line, n, &wr, nullptr);
        CloseHandle(f);
        ResumeThread(w->target);
        return 0;
    }
};

// Reports where two logs first differ.
void CompareLogs(const std::string& a, const std::string& b, const char* what, int it) {
    if (a == b) {
        EXPECT_EQ(0, 0, "");
        return;
    }
    size_t k = 0;
    while (k < a.size() && k < b.size() && a[k] == b[k]) ++k;
    size_t line = a.rfind('\n', k);
    line = line == std::string::npos ? 0 : line + 1;
    std::string la = a.substr(line, a.find('\n', line) - line), lb = b.substr(line, b.find('\n', line) - line);
    EXPECT_EQ(1, 0, "%s iteration %d differs at line:\n    orig: %s\n    ours: %s", what, it, la.c_str(), lb.c_str());
}

// Saves globals on entry and restores them on exit.
struct SavedGlobals {
    uint32_t addr[32], val[32];
    int n = 0;
    SavedGlobals(std::initializer_list<uint32_t> list) {
        for (uint32_t a : list) {
            addr[n] = a;
            val[n++] = hook::Global<uint32_t>(a);
        }
    }
    ~SavedGlobals() {
        for (int k = 0; k < n; ++k) hook::Global<uint32_t>(addr[k]) = val[k];
    }
};

} // namespace

namespace rt3 {
void __fastcall Players_DrawTable(void*, int, float x, float y, float, float h, int32_t);
void __fastcall TrainView_DrawIncomeTable(void*, int, float x, float y, float, float, int32_t);
void __fastcall CompanyView_DrawIncomeTable(void*, int, float x, float y, float, float, int32_t);
void __fastcall StationView_DrawCargoGauge(int32_t which, int, float x, float y, float w, float h, float amount);
void __fastcall StationDetail_AddKindButtons(void*, int, float x, float y, float, float);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t Global6d07b4Set2();
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
}

namespace {

// A collection of `n` records of `size` bytes filled by `fill(rec, i)`.
struct FakeTable {
    Collection c;
    // `lists` 1 also keeps the three order lists (needed for list positions).
    template <typename Fill> FakeTable(int n, uint32_t size, Fill fill, int32_t lists = 0) {
        memset(&c, 0, sizeof c);
        rt3::Collection_Construct(&c, 0, 1, size, 8, 8, lists, lists, lists);
        uint8_t* rec = (uint8_t*)calloc(1, size);
        for (int i = 0; i < n; ++i) {
            memset(rec, 0, size);
            fill(rec, i);
            rt3::Collection_Allocate(&c, 0, rec, size, 0, 0);
        }
        free(rec);
    }
    ~FakeTable() { rt3::Collection_Destruct(&c); }
};

int32_t g_companyStatCalls;
double __fastcall FakeCompanyStat(uint8_t* c, int, int32_t id, int32_t year) {
    Logf("stat %p %x %d", c, id, year);
    return (double)((id * 7919 + year * 31 + (int32_t)(uintptr_t)c) % 20001 - 10000) * 1.25;
}
// Image table entries as fake handles (only passed on to recorded calls).
uint32_t __fastcall FakeTableEntry(uint8_t* t, int, int32_t idx) { return 0x100000 + (uint32_t)(uintptr_t)t * 0 + (uint32_t)idx * 0x10; }
int32_t g_selfFlag;
int32_t FakeGlobal6d07b4Set2() { return g_selfFlag; }

} // namespace

DIFFTEST(ui_TablesAndGauges) {
    SavedGlobals sg({ 0x62be18, 0x6d0818, 0x6cfcbc, 0x62be10, 0x6cec78, 0x6cec7c, 0x6cec20, 0x6d16ec, 0x6d16d8, 0x6d16f0, 0x6cff90, 0x6d3b14,
                      0x6cfe50, 0x6cfe4c, 0x6d40e4 });
    static uint8_t view[0x100], world[0x4b00], newGame[0x100];
    hook::Global<uint8_t*>(0x6d0818) = view;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint32_t>(0x6cff90) = 0x1110;
    hook::Global<uint32_t>(0x6d3b14) = 0x2220;
    hook::Global<uint32_t>(0x6cfe50) = 0x3330;
    hook::Global<uint32_t>(0x6cfe4c) = 0x4440;
    hook::Global<uint32_t>(0x6d16ec) = 0x5550;
    hook::Global<uint32_t>(0x6d16d8) = 0x6660;
    UiTrace t;
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x4338c0, (void*)&rt3::Global6d07b4Set2, (void*)&FakeGlobal6d07b4Set2);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    size_t total[5] = {};
    for (int it = 0; it < 400; ++it) {
        int n = (int)(Rnd() % 6);
        FakeTable players(n, 0x88f, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x18, "Player %d", i);
            *(int32_t*)(r + 0x823) = (int32_t)(Rnd() % 2000) - 500;
            *(int32_t*)(r + 0x827) = (int32_t)(Rnd() % 50);
            r[0x82b] = Rnd() % 4 == 0;
        });
        FakeTable trains(1 + Rnd() % 3, 0x400, [](uint8_t* r, int) {
            *(int32_t*)(r + 0xd0) = 1900 + (int32_t)(Rnd() % 6);
            for (uint32_t o = 0xfd; o < 0x1a0; o += 4) *(float*)(r + o) = RndF(-50000.0f, 90000.0f);
        });
        FakeTable companies(1 + Rnd() % 3, 0x1000, [](uint8_t* r, int) { *(uint16_t*)(r + 0x157) = (uint16_t)(1898 + Rnd() % 8); });
        FakeTable stations(1 + Rnd() % 3, 0x700, [](uint8_t* r, int) {
            for (int k = 0; k < 5; ++k) *(int32_t*)(r + 0x590 + k * 4) = Rnd() % 3 ? (int32_t)(Rnd() % 4) - 1 : 0;
        });
        hook::Global<Collection*>(0x62be18) = &players.c;
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        hook::Global<Collection*>(0x62be10) = &companies.c;
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        *(uint32_t*)(view + 0x90) = Rnd() % 5;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1900 + Rnd() % 6);
        *(int32_t*)(world + 0x4aef) = Rnd() % 8 == 0;
        newGame[0x82] = Rnd() % 8 == 0;
        hook::Global<int32_t>(0x6d16f0) = Rnd() % 4 == 0;
        g_selfFlag = Rnd() % 2;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.5f), amount = RndF(-1000.0f, 1200000.0f);
        int32_t which = (int32_t)(Rnd() % 2);
        int op = it % 5;
        std::string a = t.Run([&] {
            switch (op) {
            case 0: ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d3600)(nullptr, 0, x, y, w, h, 0); break;
            case 1: ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x5137d0)(nullptr, 0, x, y, w, h, 0); break;
            case 2: ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4c1730)(nullptr, 0, x, y, w, h, 0); break;
            case 3: ((void(__fastcall*)(int32_t, int, float, float, float, float, float))0x505150)(which, 0, x, y, w, h, amount); break;
            case 4: ((void(__fastcall*)(void*, int, float, float, float, float))0x505470)(nullptr, 0, x, y, w, h); break;
            }
        });
        std::string b = t.Run([&] {
            switch (op) {
            case 0: rt3::Players_DrawTable(nullptr, 0, x, y, w, h, 0); break;
            case 1: rt3::TrainView_DrawIncomeTable(nullptr, 0, x, y, w, h, 0); break;
            case 2: rt3::CompanyView_DrawIncomeTable(nullptr, 0, x, y, w, h, 0); break;
            case 3: rt3::StationView_DrawCargoGauge(which, 0, x, y, w, h, amount); break;
            case 4: rt3::StationDetail_AddKindButtons(nullptr, 0, x, y, w, h); break;
            }
        });
        static const char* names[] = { "players table", "train income", "company income", "cargo gauge", "station kinds" };
        CompareLogs(a, b, names[op], it);
        total[op] += a.size();
        if (it == 399)
            for (int k = 0; k < 5; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

namespace rt3 {
uint8_t* __fastcall StationPick_Construct(uint8_t* w);
uint8_t* __fastcall ConditionDialog_Construct(uint8_t* w, int, uint8_t* r);
void __fastcall TrainDetail_AddPictures(uint8_t* win, int, float x, float y, float, float);
void __fastcall DetailDialog_AddHeader(uint8_t* win, int, float x, float y, float, float);
void __fastcall List_CreateScrollbar(uint8_t* l);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void __fastcall RulesDialog_FillConditionTypes(uint8_t* w, int, int32_t sel);
void __fastcall ConditionDialog_ShowValue(uint8_t* w);
void __fastcall RulesDialog_UpdateButtons4ff7(uint8_t* w);
int32_t __fastcall ConditionDialog_Store(uint8_t* w);
void __fastcall RulesDialog_ShowLimits(uint8_t* w);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void* __fastcall Obj_CityRef(void* o);
void* __fastcall Obj_City41(void* o);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall Obj_NamedEntry40(uint8_t* o);
int32_t __fastcall HandleTable_Acquire(uint8_t* t, int, uint16_t id);
void __fastcall List_SyncScrollbar(uint8_t* l);
}

namespace {

uint8_t* __fastcall RecSetVtbl(uint8_t* w) {
    Logf("widget base %p", w);
    return w;
}
void __fastcall RecLoad(uint8_t* win, int, float x, float y, const char* name) { Logf("load %p %08x %08x \"%s\"", win, B(x), B(y), name); }
int32_t __fastcall FakeStateActive(void* o) {
    Logf("active? %p", o);
    return ((uint8_t*)o)[0x10] & 1;
}
uint8_t* g_condRecord;
void __fastcall RecFillTypes(uint8_t* w, int, int32_t sel) {
    Logf("fill types %p %d", w, sel);
    // Filling resets the record's fields; the constructor puts them back.
    memset(g_condRecord + 4, 0x5a, 0x14);
}
void __fastcall RecShowValue(uint8_t* w) { Logf("show value %p", w); }
void __fastcall RecUpdateButtons(uint8_t* w) { Logf("update buttons %p", w); }
int32_t __fastcall RecStore(uint8_t* w) {
    Logf("store %p", w);
    return 1;
}
void __fastcall RecShowLimits(uint8_t* w) { Logf("show limits %p", w); }
uint8_t g_child[0x100];
uint8_t* __fastcall RecFindChild(uint8_t* w, int, int32_t id) {
    Logf("find child %p %x", w, id);
    return g_child;
}
uint8_t g_engine[0x100], g_territory[0x400];
void* __fastcall FakeCityRef(void* o) {
    Logf("engine of %p", o);
    return g_engine;
}
void* __fastcall FakeCity41(void* o) {
    Logf("territory of %p", o);
    return g_territory;
}
int32_t __fastcall FakeContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix) {
    Logf("has \"%s\" %d %p", name, fatal, suffix);
    return (int32_t)(strlen(name) & 1);
}
char g_named[0x80] = "Custom Face";
uint8_t* __fastcall FakeNamedEntry40(uint8_t* o) {
    Logf("named %p", o);
    return (uint8_t*)g_named;
}
int32_t __fastcall FakeAcquire(uint8_t* t, int, uint16_t id) {
    Logf("acquire %p %x", t, id);
    return 0x1234;
}
void __fastcall RecSync(uint8_t* l) { Logf("sync %p", l); }

} // namespace

DIFFTEST(ui_WindowBuilders) {
    SavedGlobals sg({ 0x6cec20, 0x6d07c4, 0x6d07c8, 0x6d3c84, 0x6cfcbc, 0x6d07dc, 0x62ba88, 0x6d07e8, 0x6d4020, 0x6d10a8, 0x6ceb9c, 0x62be10, 0x6d401c });
    UiTrace t;
    t.Add(0x53f7e0, (void*)&rt3::Widget_SetVtblRet, (void*)&RecSetVtbl);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLoad);
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeStateActive);
    t.Add(0x4d5d00, (void*)&rt3::RulesDialog_FillConditionTypes, (void*)&RecFillTypes);
    t.Add(0x4d6090, (void*)&rt3::ConditionDialog_ShowValue, (void*)&RecShowValue);
    t.Add(0x4d5f50, (void*)&rt3::RulesDialog_UpdateButtons4ff7, (void*)&RecUpdateButtons);
    t.Add(0x4d67f0, (void*)&rt3::ConditionDialog_Store, (void*)&RecStore);
    t.Add(0x4d5ba0, (void*)&rt3::RulesDialog_ShowLimits, (void*)&RecShowLimits);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&RecFindChild);
    t.Add(0x4a8840, (void*)&rt3::Obj_CityRef, (void*)&FakeCityRef);
    t.Add(0x4a77b0, (void*)&rt3::Obj_City41, (void*)&FakeCity41);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeContainsName);
    t.Add(0x476ab0, (void*)&rt3::Obj_NamedEntry40, (void*)&FakeNamedEntry40);
    t.Add(0x538890, (void*)&rt3::HandleTable_Acquire, (void*)&FakeAcquire);
    t.Add(0x55faf0, (void*)&rt3::List_SyncScrollbar, (void*)&RecSync);
    static uint8_t pack[0x43000], win[0x200], rec[0x40], dlg[0x100], list[0x200];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint32_t>(0x6d401c) = 0x7770;
    size_t total[5] = {};
    for (int it = 0; it < 500; ++it) {
        int op = it % 5;
        FakeTable stations(Rnd() % 7, 0x500, [](uint8_t* r, int i) {
            r[0x10] = (uint8_t)Rnd();
            static const char* names[] = { "Alpha", "beta", "Alpha", "Gamma", "delta", "BETA" };
            strcpy((char*)r + 0x46b, names[Rnd() % 6]);
        });
        FakeTable trains(1 + Rnd() % 2, 0x200, [](uint8_t*, int) {});
        FakeTable engines(2, 0x200, [](uint8_t* r, int i) { sprintf((char*)r + 0x82, "Engine%d", i + Rnd() % 3); });
        FakeTable overrides(Rnd() % 4, 0x100, [](uint8_t* r, int i) {
            static const char* names[] = { "Europe", "America", "europe", "Asia" };
            strcpy((char*)r, names[Rnd() % 4]);
            sprintf((char*)r + 0x80, "Override%d_%%1.imb", i);
        });
        FakeTable players(2, 0x200, [](uint8_t* r, int) { r[0x15c] = Rnd() % 3 ? (uint8_t)(Rnd() % 0x30) : 0x29; });
        FakeTable companies(2, 0x1000, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x37) = Rnd() % 2 ? -1 : (int32_t)(Rnd() % 9);
            sprintf((char*)r + 0xd65, "Logo%d.imb", i);
        });
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        hook::Global<Collection*>(0x62ba88) = &engines.c;
        hook::Global<Collection*>(0x6ceb9c) = &players.c;
        hook::Global<Collection*>(0x62be10) = &companies.c;
        *(Collection**)(pack + 0x429b8) = &overrides.c;
        hook::Global<uint32_t>(0x6d07dc) = 1 + Rnd() % 2;
        *(uint32_t*)(g_engine + 0x72) = 1 + Rnd() % 2; // 0 makes the original format from address 0x82
        *(int32_t*)(g_engine + 0x10) = (int32_t)(Rnd() % 4);
        static const char* terr[] = { "Europe", "Asia", "Mars" };
        strcpy((char*)g_territory + 0x3ac, terr[Rnd() % 3]);
        g_territory[0x3cc] = Rnd() % 2;
        hook::Global<uint8_t*>(0x6d07e8) = Rnd() % 4 ? g_territory : nullptr;
        *(int32_t*)(dlg + 0x78) = Rnd() % 2 ? 8 : 3;
        *(uint32_t*)(dlg + 0x8c) = 1 + Rnd() % 2;
        *(uint32_t*)(dlg + 0x88) = 1 + Rnd() % 2;
        hook::Global<uint8_t*>(0x6d10a8) = dlg;
        hook::Global<int32_t>(0x6d07c8) = Rnd() % 2;
        int32_t lang = (int32_t)(Rnd() % 5);
        uint8_t recStart[0x40];
        for (uint8_t& b : recStart) b = (uint8_t)Rnd();
        *(int32_t*)recStart = Rnd() % 3 ? (int32_t)(Rnd() % 600) : -1;
        recStart[8] = (uint8_t)(Rnd() % 6);
        *(int32_t*)(recStart + 9) = Rnd() % 2 ? (int32_t)(Rnd() % 100) - 20 : 0;
        *(int32_t*)(recStart + 0xd) = Rnd() % 2 ? (int32_t)(Rnd() % 100) - 20 : 0;
        uint8_t listStart[0x200];
        for (uint8_t& b : listStart) b = (uint8_t)Rnd();
        *(int32_t*)(listStart + 0x126) = Rnd() % 4 == 0;
        *(int32_t*)(listStart + 0x47) = 0;
        *(int32_t*)(listStart + 0x43) = 0;
        *(int32_t*)(listStart + 0xee) = Rnd() % 2;
        *(int32_t*)(listStart + 0xf2) = Rnd() % 2;
        *(int32_t*)(listStart + 0xf6) = Rnd() % 2 ? 0 : (int32_t)Rnd();
        *(int32_t*)(listStart + 0xa2) = (int32_t)(Rnd() % 8) - 1;
        for (uint32_t o = 0xb; o <= 0x17; o += 4) *(float*)(listStart + o) = RndF(0.0f, 1.0f);
        for (uint32_t o = 0xc6; o <= 0xda; o += 4) *(float*)(listStart + o) = RndF(0.0f, 0.5f);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        uint8_t outW[2][0x200], outR[2][0x40], outL[2][0x200], outChild[2][0x100];
        uint32_t outG[2][2];
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(win, 0, sizeof win);
            memset(g_child, 0, sizeof g_child);
            memcpy(rec, recStart, sizeof rec);
            memcpy(list, listStart, sizeof list);
            g_condRecord = rec;
            hook::Global<int32_t>(0x6d3c84) = lang;
            hook::Global<uint32_t>(0x6d07c4) = 0;
            logs[side] = t.Run([&] {
                switch (op) {
                case 0: side ? rt3::StationPick_Construct(win) : ((uint8_t*(__fastcall*)(uint8_t*))0x507620)(win); break;
                case 1: side ? rt3::ConditionDialog_Construct(win, 0, rec) : ((uint8_t*(__fastcall*)(uint8_t*, int, uint8_t*))0x4d6d60)(win, 0, rec); break;
                case 2:
                    side ? rt3::TrainDetail_AddPictures(win, 0, x, y, 0.0f, 0.0f)
                         : ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4dc180)(win, 0, x, y, 0.0f, 0.0f);
                    break;
                case 3:
                    side ? rt3::DetailDialog_AddHeader(win, 0, x, y, 0.0f, 0.0f)
                         : ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4e1fa0)(win, 0, x, y, 0.0f, 0.0f);
                    break;
                case 4: side ? rt3::List_CreateScrollbar(list) : ((void(__fastcall*)(uint8_t*))0x560000)(list); break;
                }
            });
            hook::Global<int32_t>(0x6d3c84) = 0;
            memcpy(outW[side], win, sizeof win);
            memcpy(outR[side], rec, sizeof rec);
            memcpy(outL[side], list, sizeof list);
            memcpy(outChild[side], g_child, sizeof g_child);
            outG[side][0] = hook::Global<uint32_t>(0x6d07c4);
            outG[side][1] = hook::Global<uint32_t>(0x6d07c8);
        }
        static const char* names[] = { "station pick", "condition dialog", "train pictures", "detail header", "list scrollbar" };
        CompareLogs(logs[0], logs[1], names[op], it);
        EXPECT_EQ(memcmp(outW[0], outW[1], sizeof outW[0]), 0, "%s window iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outR[0], outR[1], sizeof outR[0]), 0, "%s record iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outL[0], outL[1], sizeof outL[0]), 0, "%s list iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outChild[0], outChild[1], sizeof outChild[0]), 0, "%s child iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outG[0], outG[1], sizeof outG[0]), 0, "%s globals iteration %d", names[op], it);
        total[op] += logs[0].size();
        if (it == 499)
            for (int k = 0; k < 5; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

namespace rt3 {
void __fastcall SceneLabel_Draw(uint8_t* o);
void __fastcall SceneObj_DrawMarker(uint8_t* o);
void __fastcall Ticker_Draw(uint8_t* t, int, float x, float y, float, float);
uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);
void __fastcall Chat_AddLine(const char* text);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength);
int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y);
double __fastcall Scroll_Visible(uint8_t* s);
void __stdcall Camera_DetailFactor(int32_t x, int32_t z, float* out);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
int32_t ClockMs();
void __fastcall Ticker_PickNext(uint8_t* t);
void __fastcall ModelLibrary_Init(uint32_t* o, int, int32_t count);
uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
void __fastcall Emitter_Reset(uint8_t* e, int, int32_t n, int32_t);
void __fastcall Lobby_AddChatText(const char* text);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
}

namespace {

// Scene-side fakes, driven by per-iteration values.
struct SceneFakes {
    float ground, sx, sy, depth, scale, strength, detail, lineH, step[2], vfloat;
    int32_t shown, inBounds, lines, kind, height, clock;
    uint32_t fogFont, color;
    char text1[0x40], text2[0x40];
} g_sf;

double __fastcall FakeHeight(uint8_t* r, int, float x, float z) {
    Logf("height %p %08x %08x", r, B(x), B(z));
    return g_sf.ground;
}
int32_t __fastcall FakeProject(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    Logf("project %p %08x %08x %08x", s, B(x), B(y), B(z));
    *ox = g_sf.sx;
    *oy = g_sf.sy;
    *oz = g_sf.depth;
    return g_sf.shown;
}
void __fastcall FakeFog(uint8_t* t, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength) {
    Logf("fog %p %d %d %08x", t, x, z, B(bias));
    *scale = g_sf.scale;
    *color = g_sf.fogFont;
    *strength = g_sf.strength;
}
int32_t __fastcall FakeInBounds(uint8_t* c, int, float x, float y) {
    Logf("bounds %p %08x %08x", c, B(x), B(y));
    return g_sf.inBounds;
}
double __fastcall FakeLineHeight(uint8_t* f) {
    Logf("line height %p", f);
    return g_sf.lineH;
}
void __stdcall FakeDetail(int32_t x, int32_t z, float* out) {
    Logf("detail %d %d", x, z);
    *out = g_sf.detail;
}
double __fastcall FakeStepA(uint8_t* s) { return g_sf.step[0] + (double)(((uintptr_t)s >> 4) & 0xf) * 0.01; }
double __fastcall FakeStepB(uint8_t* s) { return g_sf.step[1] + (double)(((uintptr_t)s >> 4) & 0xf) * 0.02; }
int32_t FakeClock() { return g_sf.clock; }
void __fastcall RecPickNext(uint8_t* t) { Logf("pick next %p", t); }
uint32_t __fastcall FakeEntry(uint8_t* t, int, int32_t idx) { return (uint32_t)(uintptr_t)t + (uint32_t)idx * 0x10; }
void __fastcall RecModelInit(uint32_t* o, int, int32_t n) {
    Logf("model library %p %x", o, n);
    o[5] = 1;
}
uint8_t g_model[0x40], g_shape[0x20];
float g_points[16 * 3];
uint8_t* __fastcall FakeCollision(uint8_t* pack, int, const char* name) {
    Logf("collision %p \"%s\"", pack, name);
    return g_model;
}
void __fastcall RecEmitterReset(uint8_t* e, int, int32_t n, int32_t f) { Logf("emitter reset %p %d %x", e, n, f); }
void* __cdecl FakeMalloc(size_t n) {
    void* p = ArenaAlloc(n);
    Logf("malloc %x -> %p", (uint32_t)n, p);
    return p;
}
void __fastcall RecLobbyChat(const char* text) { Logf("lobby \"%s\"", text); }
uint32_t __fastcall RecLogAdd(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d) {
    Logf("log %p \"%s\" %d %d %d", t, name, b, c, d);
    return 0;
}

// Scene label / marker object vtable entries.
int32_t __fastcall FakeLabelText(uint8_t* o, int, char* buf, int32_t n, int32_t* color, float* bias, char* buf2, int32_t n2) {
    Logf("label text %p %d %d", o, n, n2);
    strcpy(buf, g_sf.text1);
    strcpy(buf2, g_sf.text2);
    *color = (int32_t)g_sf.color;
    *bias = g_sf.vfloat;
    return g_sf.lines;
}
int32_t __fastcall FakeLabelOnGround(uint8_t* o) {
    Logf("on ground %p", o);
    return g_sf.shown & 2;
}
int32_t __fastcall FakeMarkerText(uint8_t* o, int, char* buf, int32_t n, int32_t* color, float* bias, uint8_t** icon) {
    Logf("marker text %p %d", o, n);
    strcpy(buf, g_sf.text1);
    *color = (int32_t)g_sf.color;
    *bias = g_sf.vfloat;
    *icon = (uint8_t*)(uintptr_t)0x123450;
    return g_sf.kind;
}
int32_t __fastcall FakeMarkerHeight(uint8_t* o) { return g_sf.height; }
int32_t __fastcall FakeMarkerHeight2(uint8_t* o) { return g_sf.height * 2 + 1; }
double __fastcall FakeMarkerMax(uint8_t* o) { return g_sf.strength * 1.5; }

} // namespace

DIFFTEST(ui_SceneOverlays) {
    SavedGlobals sg({ 0x6d4024, 0xd93850, 0x625464, 0x6d4030, 0x6d4020, 0x6cec78, 0x6acd34 });
    UiTrace t;
    t.Add(0x534490, (void*)&rt3::Terrain_HeightAt, (void*)&FakeHeight);
    t.Add(0x527450, (void*)&rt3::Scene_ProjectNormalised, (void*)&FakeProject);
    t.Add(0x5371f0, (void*)&rt3::Fog_ParamsAt, (void*)&FakeFog);
    t.Add(0x52d0f0, (void*)&rt3::Camera_InBounds, (void*)&FakeInBounds);
    t.Add(0x5500b0, (void*)&rt3::Scroll_Visible, (void*)&FakeLineHeight);
    t.Add(0x537170, (void*)&rt3::Camera_DetailFactor, (void*)&FakeDetail);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeStepA);
    t.Add(0x5521a0, (void*)&rt3::Slider_StepB, (void*)&FakeStepB);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeClock);
    t.Add(0x4c7ad0, (void*)&rt3::Ticker_PickNext, (void*)&RecPickNext);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeEntry);
    t.Add(0x556770, (void*)&rt3::ModelLibrary_Init, (void*)&RecModelInit);
    t.Add(0x53d0f0, (void*)&rt3::Pack_AcquireCollision, (void*)&FakeCollision);
    t.Add(0x554ce0, (void*)&rt3::Emitter_Reset, (void*)&RecEmitterReset);
    t.Add(0x4ee810, (void*)&rt3::Lobby_AddChatText, (void*)&RecLobbyChat);
    t.Add(0x4554e0, (void*)&rt3::Log_Add, (void*)&RecLogAdd);
    t.Add(0x5a125d, nullptr, (void*)&FakeMalloc);
    static uint8_t engine[0x114800], obj[0x300], ticker[0x200], emitter[0x300], lib[0x40], scene[0x10], terrain[0x10], world[0x100];
    static void* labelVtbl[0x10] = { 0, 0, 0, 0, (void*)&FakeLabelText, (void*)&FakeLabelOnGround };
    static void* markerVtbl[0x10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (void*)&FakeMarkerText, (void*)&FakeMarkerHeight, (void*)&FakeMarkerHeight2,
                                      (void*)&FakeMarkerMax };
    *(uint8_t**)(engine + 0x18) = scene;
    *(uint8_t**)(engine + 0x1c) = terrain;
    *(uint32_t*)(engine + 0x28) = 0x28280;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint8_t*>(0x6d4030) = lib;
    hook::Global<uint32_t>(0x6d4020) = 0x4020400;
    hook::Global<uint32_t>(0x6acd34) = 0x6acd340;
    *(uint8_t**)g_model = g_shape;
    *(uint8_t**)(g_shape + 0xc) = g_shape + 0x10;
    *(float**)(g_shape + 0x10) = g_points;
    for (float& p : g_points) p = RndF(-5.0f, 5.0f);
    size_t total[5] = {};
    for (int it = 0; it < 2500; ++it) {
        int op = it % 5;
        *(uint32_t*)(engine + 0x1141f5) = Rnd() % 6 ? 0x1141f50 : 0;
        g_sf.ground = RndF(-50.0f, 200.0f);
        g_sf.sx = Rnd() % 6 ? RndF(-0.2f, 1.2f) : (Rnd() % 2 ? 0.0f : 1.0f);
        g_sf.sy = Rnd() % 6 ? RndF(-0.2f, 1.2f) : (Rnd() % 2 ? 0.0f : 1.0f);
        g_sf.depth = RndF(0.0f, 1.0f);
        g_sf.scale = RndF(0.25f, 1.25f);
        g_sf.strength = RndF(0.0f, 1.5f);
        g_sf.detail = Rnd() % 4 ? RndF(0.0f, 2.0f) : 0.1f;
        g_sf.lineH = RndF(0.0f, 0.1f);
        g_sf.step[0] = RndF(0.0f, 0.3f);
        g_sf.step[1] = RndF(0.0f, 0.3f);
        g_sf.vfloat = RndF(-1.0f, 1.0f);
        g_sf.shown = (int32_t)(Rnd() % 4);
        g_sf.inBounds = Rnd() % 4 != 0;
        g_sf.lines = (int32_t)(Rnd() % 3);
        g_sf.kind = (int32_t)(Rnd() % 5);
        g_sf.height = (int32_t)(Rnd() % 200);
        g_sf.fogFont = 0xf0f0f00 + (Rnd() % 4) * 0x100;
        g_sf.color = Rnd();
        static const char* texts[] = { "", "A", "AB", "ABC", "Station" };
        strcpy(g_sf.text1, texts[Rnd() % 5]);
        strcpy(g_sf.text2, texts[Rnd() % 5]);
        float fontSub[8] = {};
        uint8_t fontSubBytes[0x40] = {};
        (void)fontSub;
        uint8_t objStart[0x300];
        for (uint8_t& b : objStart) b = (uint8_t)Rnd();
        for (uint32_t o : { 0xcu, 0x10u, 0x18u, 0x30u, 0x63u, 0x6bu, 0x1e2u, 0x1e6u, 0x1eau }) *(float*)(objStart + o) = RndF(0.0f, 5000.0f);
        *(void***)objStart = op == 0 ? labelVtbl : markerVtbl;
        uint8_t tickStart[0x200];
        memset(tickStart, 0, sizeof tickStart);
        *(int32_t*)(tickStart + 0x170) = (int32_t)(Rnd() % 20000);
        *(int32_t*)(tickStart + 0x178) = (int32_t)(Rnd() % 3);
        *(uint32_t*)(tickStart + 0x17c) = 0x700000;
        *(uint32_t*)(tickStart + 0x180) = Rnd() % 2 ? 0x710000 : 0;
        g_sf.clock = (int32_t)(Rnd() % 25000);
        int32_t shapeCount = (int32_t)(Rnd() % 16);
        *(int32_t*)(g_shape + 0x18) = shapeCount;
        int32_t libReady = Rnd() % 2;
        *(uint32_t*)(lib + 0x28) = Rnd();
        bool named = Rnd() % 3 != 0, withRect = Rnd() % 2, withObj = Rnd() % 2;
        float rect[4] = { RndF(0.0f, 1.0f), RndF(0.0f, 1.0f), RndF(0.0f, 1.0f), RndF(0.0f, 1.0f) };
        uint32_t color = Rnd(), flags = Rnd() & 0x00c00c00;
        int32_t perPoint = (int32_t)(Rnd() % 10);
        static const char* chats[] = { "hello", "<x> hi", "" };
        const char* chat = chats[Rnd() % 3];
        uint32_t worldPtr = Rnd() % 2 ? 0x6cec780 : 0;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        std::string logs[2];
        uint8_t outObj[2][0x300], outEm[2][0x300], outFont[2][0x40], outArena[2][0x400];
        uint32_t outG[2][3];
        for (int side = 0; side < 2; ++side) {
            memcpy(obj, objStart, sizeof obj);
            memcpy(ticker, tickStart, sizeof ticker);
            memset(emitter, 0x77, sizeof emitter);
            *(int32_t*)(lib + 0x14) = libReady;
            memset(fontSubBytes, 0, sizeof fontSubBytes);
            g_sf.fogFont = (uint32_t)(uintptr_t)fontSubBytes;
            *(uint8_t**)(fontSubBytes + 0x18) = fontSubBytes + 0x20;
            hook::Global<uint32_t>(0xd93850) = 0x3f000000;
            hook::Global<uint32_t>(0x625464) = 0;
            hook::Global<uint32_t>(0x6cec78) = worldPtr;
            logs[side] = t.Run([&] {
                switch (op) {
                case 0: side ? rt3::SceneLabel_Draw(obj) : ((void(__fastcall*)(uint8_t*))0x53a960)(obj); break;
                case 1: side ? rt3::SceneObj_DrawMarker(obj) : ((void(__fastcall*)(uint8_t*))0x530280)(obj); break;
                case 2:
                    side ? rt3::Ticker_Draw(ticker, 0, x, y, 0.0f, 0.0f)
                         : ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4c7c80)(ticker, 0, x, y, 0.0f, 0.0f);
                    break;
                case 3:
                    side ? rt3::Emitter_Construct(emitter, 0, named ? "smoke" : nullptr, color, perPoint, withRect ? rect : nullptr, withObj ? obj : nullptr,
                                                  flags)
                         : ((uint8_t*(__fastcall*)(uint8_t*, int, const char*, uint32_t, int32_t, const float*, uint8_t*, uint32_t))0x556920)(
                               emitter, 0, named ? "smoke" : nullptr, color, perPoint, withRect ? rect : nullptr, withObj ? obj : nullptr, flags);
                    break;
                case 4: side ? rt3::Chat_AddLine(chat) : ((void(__fastcall*)(const char*))0x46c360)(chat); break;
                }
            });
            memcpy(outObj[side], obj, sizeof obj);
            memcpy(outEm[side], emitter, sizeof emitter);
            memcpy(outFont[side], fontSubBytes, sizeof fontSubBytes);
            memcpy(outArena[side], g_arena, sizeof outArena[side]);
            outG[side][0] = hook::Global<uint32_t>(0xd93850);
            outG[side][1] = hook::Global<uint32_t>(0x625464);
            outG[side][2] = *(uint32_t*)(lib + 0x14);
        }
        static const char* names[] = { "scene label", "scene marker", "ticker", "emitter", "chat line" };
        CompareLogs(logs[0], logs[1], names[op], it);
        EXPECT_EQ(memcmp(outObj[0], outObj[1], sizeof outObj[0]), 0, "%s object iteration %d", names[op], it);
        {
            int d = 0;
            while (d < (int)sizeof outEm[0] && outEm[0][d] == outEm[1][d]) ++d;
            EXPECT_EQ(d, (int)sizeof outEm[0], "%s emitter iteration %d at +%x: %08x vs %08x (rect %d obj %d named %d flags %x)", names[op], it, d,
                      *(uint32_t*)(outEm[0] + (d & ~3)), *(uint32_t*)(outEm[1] + (d & ~3)), withRect, withObj, named, flags);
        }
        EXPECT_EQ(memcmp(outFont[0], outFont[1], sizeof outFont[0]), 0, "%s font iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outArena[0], outArena[1], sizeof outArena[0]), 0, "%s arena iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outG[0], outG[1], sizeof outG[0]), 0, "%s globals iteration %d", names[op], it);
        total[op] += logs[0].size();
        if (it == 2499)
            for (int k = 0; k < 5; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

#include "core/ptrlist.h"
using rt3::HashMap;
using rt3::NodePool;
using rt3::SList;
using rt3::PtrList;
using rt3::PtrListNode;

namespace rt3 {
void __fastcall NamedObj_Destructor(uint8_t* o);
void __fastcall Terrain_DeleteMarkers(uint8_t* r);
void __fastcall Terrain_FreeBlockObjects(uint8_t* r);
void __fastcall Group_SetObject(uint8_t* g, int, uint8_t* o);
int32_t __fastcall EmitterLibrary_Register(uint8_t* lib, int, uint32_t key, int32_t perPoint, const float* rect, uint32_t flags);
uint8_t* __fastcall EmitterLibrary_Get(uint8_t* lib, int, uint32_t key, uint32_t color, const char* name, uint8_t* obj, int32_t spawn);
void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall Group_Add(uint8_t* g, int, uint8_t* o, float lift);
void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);
void __fastcall Emitter_Reset(uint8_t* e, int, int32_t n, int32_t);
PtrList* __fastcall PtrList_Init(PtrList* l);
PtrListNode* __fastcall PtrList_Append(PtrList* l, int, void* value);
void __fastcall PtrList_Destroy(PtrList* l);
}

namespace {

void __fastcall RecSceneDtor(uint8_t* o) {
    Logf("scene dtor %p vt %08x", o, *(uint32_t*)o);
}
void __cdecl RecDelete(void* p) { Logf("delete %p", p); }
void __fastcall RecGetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    Logf("get angles %p", o);
    *a = 0x11111111 ^ (uint32_t)(uintptr_t)o;
    *b = 0x22222222;
    *c = 0x7fa00001; // a signalling NaN pattern must pass through untouched
}
void __fastcall RecSetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) { Logf("set angles %p %08x %08x %08x", o, a, b, c); }
void __fastcall RecGetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    Logf("get pos %p", o);
    *x = 0x44444444;
    *y = 0xffa00002;
    *z = 0x3f800000 ^ (uint32_t)(uintptr_t)o;
}
void __fastcall RecMoveTo(uint8_t* o, int, float x, float y, float z) { Logf("move %p %08x %08x %08x", o, B(x), B(y), B(z)); }
void __fastcall RecGroupAdd(uint8_t* g, int, uint8_t* o, float lift) { Logf("group add %p %p %08x", g, o, B(lift)); }
// A tiny hash: key -> value pairs per map.
struct FakeMap { uint32_t key[64]; int32_t value[64]; int n; };
FakeMap g_maps[2];
void __fastcall RecHashInsert(HashMap* m, int, uint32_t key, int32_t value) {
    Logf("hash insert %p %x %x", m, key, (uint32_t)value);
    FakeMap& f = g_maps[(uint8_t*)m == (uint8_t*)&g_maps[1] ? 1 : 0];
    if (f.n < 64) {
        f.key[f.n] = key;
        f.value[f.n++] = value;
    }
}
int32_t __fastcall RecHashGet(HashMap* m, int, uint32_t key) {
    Logf("hash get %p %x", m, key);
    FakeMap& f = g_maps[(uint8_t*)m == (uint8_t*)&g_maps[1] ? 1 : 0];
    for (int k = f.n - 1; k >= 0; --k)
        if (f.key[k] == key) return f.value[k];
    return 0;
}
uint8_t* __fastcall RecEmitterCtor(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                   uint32_t flags) {
    Logf("emitter %p \"%s\" %08x %d %p %p %x", e, name ? name : "(null)", color, perPoint, rect, obj, flags);
    *(int32_t*)(e + 0x38) = perPoint * 3;
    return e;
}
void __fastcall RecReset(uint8_t* e, int, int32_t n, int32_t f) { Logf("reset %p %d %x", e, n, f); }

} // namespace

DIFFTEST(ui_SceneLists) {
    UiTrace t;
    t.Add(0x530680, (void*)&rt3::SceneObj_Destructor, (void*)&RecSceneDtor);
    t.Add(0x53b080, nullptr, (void*)&RecDelete);
    t.Add(0x52e880, (void*)&rt3::SceneObj_GetAngles, (void*)&RecGetAngles);
    t.Add(0x52e8b0, (void*)&rt3::SceneObj_SetAngles, (void*)&RecSetAngles);
    t.Add(0x52e720, (void*)&rt3::SceneObj_GetPos, (void*)&RecGetPos);
    t.Add(0x530720, (void*)&rt3::Obj_MoveTo, (void*)&RecMoveTo);
    t.Add(0x5395d0, (void*)&rt3::Group_Add, (void*)&RecGroupAdd);
    t.Add(0x53d960, (void*)&rt3::Hash_Insert, (void*)&RecHashInsert);
    t.Add(0x53dae0, (void*)&rt3::Hash_Get, (void*)&RecHashGet);
    t.Add(0x556920, (void*)&rt3::Emitter_Construct, (void*)&RecEmitterCtor);
    t.Add(0x554ce0, (void*)&rt3::Emitter_Reset, (void*)&RecReset);
    static uint8_t objs[16][0x40], terrain[0x2000], group[0x40], lib[0x40], poolMem[0x100];
    static NodePool pool;
    static PtrList listMem[10];
    static SList freeList;
    size_t total[4] = {};
    for (int it = 0; it < 2000; ++it) {
        int op = it % 4;
        int nMarkers = (int)(Rnd() % 8);
        uint8_t markerUse[8];
        for (uint8_t& u : markerUse) u = Rnd() % 3 != 0;
        int bw = (int)(Rnd() % 3) * 16 + (int)(Rnd() % 16), bh = (int)(Rnd() % 3) * 16 + (int)(Rnd() % 16);
        bool haveGrid = Rnd() % 5 != 0;
        uint32_t cellPick = Rnd();
        bool haveList = Rnd() % 2;
        int capacity = (int)(Rnd() % 2);
        int ops = 1 + (int)(Rnd() % 6);
        uint32_t keys[6], colors[6], spawns[6];
        for (int k = 0; k < 6; ++k) {
            keys[k] = Rnd() % 4;
            colors[k] = Rnd();
            spawns[k] = Rnd() % 3;
        }
        float rect[4] = { 0.1f, 0.2f, 0.3f, 0.4f };
        std::string logs[2];
        uint32_t outT[2][2], outG[2][4];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < 16; ++k) {
                memset(objs[k], 0, sizeof objs[k]);
                *(uint32_t*)objs[k] = 0x5cb000 + k;
            }
            memset(terrain, 0, sizeof terrain);
            *(int32_t*)(terrain + 0x1691) = nMarkers;
            for (int k = 0; k < nMarkers; ++k) *(uint8_t**)(terrain + 0x1695 + k * 9) = markerUse[k] ? objs[k] : nullptr;
            *(int32_t*)(terrain + 0x15d9) = bw;
            *(int32_t*)(terrain + 0x15d5) = bh;
            PtrList* lists[9] = {};
            PtrList** grid = nullptr;
            int gw = bw >> 4, gh = bh >> 4;
            if (op == 1 && haveGrid) {
                grid = (PtrList**)((void*(__cdecl*)(size_t))0x5a125d)(sizeof(PtrList*) * 9 + 4);
                for (int k = 0; k < gw * gh; ++k) {
                    lists[k] = &listMem[k];
                    rt3::PtrList_Init(lists[k]);
                    int n = (int)((cellPick >> (k * 3)) & 3);
                    for (int j = 0; j < n; ++j) rt3::PtrList_Append(lists[k], 0, (j == 1 && (cellPick & 1)) ? nullptr : objs[(k * 3 + j) & 15]);
                    grid[k] = lists[k];
                }
            }
            *(PtrList***)(terrain + 0x1671) = grid;
            memset(group, 0, sizeof group);
            PtrList* gl = nullptr;
            if (op == 2 && haveList) {
                gl = &listMem[9];
                rt3::PtrList_Init(gl);
                rt3::PtrList_Append(gl, 0, objs[3]);
                rt3::PtrList_Append(gl, 0, objs[4]);
                *(PtrList**)(group + 8) = gl;
            }
            memset(lib, 0, sizeof lib);
            memset(g_maps, 0, sizeof g_maps);
            memset(poolMem, 0, sizeof poolMem);
            memset(&pool, 0, sizeof pool);
            memset(&freeList, 0, sizeof freeList);
            for (int k = 0; k < 4; ++k) {
                void** node = (void**)(poolMem + k * 0x10);
                node[0] = k < 3 ? (void*)(poolMem + (k + 1) * 0x10) : nullptr;
            }
            freeList.head = (void**)poolMem;
            freeList.tail = (void**)(poolMem + 0x30);
            freeList.count = 4;
            pool.freeList = &freeList;
            pool.capacity = capacity ? 4 : 0;
            pool.elemSize = 8;
            *(HashMap**)(lib + 8) = (HashMap*)&g_maps[0];
            *(HashMap**)(lib + 4) = (HashMap*)&g_maps[1];
            *(NodePool**)(lib + 0xc) = &pool;
            logs[side] = t.Run([&] {
                switch (op) {
                case 0:
                    if (side) {
                        rt3::Terrain_DeleteMarkers(terrain);
                        rt3::NamedObj_Destructor(objs[15]);
                    } else {
                        ((void(__fastcall*)(uint8_t*))0x534430)(terrain);
                        ((void(__fastcall*)(uint8_t*))0x455650)(objs[15]);
                    }
                    break;
                case 1: side ? rt3::Terrain_FreeBlockObjects(terrain) : ((void(__fastcall*)(uint8_t*))0x535b40)(terrain); break;
                case 2: side ? rt3::Group_SetObject(group, 0, objs[7]) : ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x53a5d0)(group, 0, objs[7]); break;
                case 3:
                    for (int k = 0; k < ops && (capacity == 0 || k < 4); ++k) {
                        if (spawns[k] == 0) {
                            side ? rt3::EmitterLibrary_Register(lib, 0, keys[k], (int32_t)(colors[k] % 9), rect, colors[k] & 0xff)
                                 : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, int32_t, const float*, uint32_t))0x556c50)(
                                       lib, 0, keys[k], (int32_t)(colors[k] % 9), rect, colors[k] & 0xff);
                        } else {
                            uint8_t* e = side ? rt3::EmitterLibrary_Get(lib, 0, keys[k], colors[k], "spark", objs[1], (int32_t)spawns[k])
                                              : ((uint8_t*(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, const char*, uint8_t*, int32_t))0x556ce0)(
                                                    lib, 0, keys[k], colors[k], "spark", objs[1], (int32_t)spawns[k]);
                            Logf("got %p", e);
                        }
                    }
                    break;
                }
            });
            outT[side][0] = *(uint32_t*)(terrain + 0x1691);
            outT[side][1] = *(uint32_t*)(terrain + 0x1671);
            outG[side][0] = gl ? (uint32_t)gl->count : 0xdead;
            outG[side][1] = gl && gl->head ? (uint32_t)(uintptr_t)gl->head->value : 0;
            outG[side][2] = (uint32_t)freeList.count;
            outG[side][3] = (uint32_t)(uintptr_t)freeList.head ^ ((uint32_t)(uintptr_t)freeList.tail << 1);
            if (gl) rt3::PtrList_Destroy(gl);
        }
        static const char* names[] = { "markers", "block objects", "group object", "emitter library" };
        CompareLogs(logs[0], logs[1], names[op], it);
        EXPECT_EQ(memcmp(outT[0], outT[1], sizeof outT[0]), 0, "%s terrain iteration %d", names[op], it);
        EXPECT_EQ(memcmp(outG[0], outG[1], sizeof outG[0]), 0, "%s state iteration %d", names[op], it);
        total[op] += logs[0].size();
        if (it == 1999)
            for (int k = 0; k < 4; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

namespace rt3 {
void __fastcall Scene_Shutdown(uint8_t* s);
void __fastcall Engine_Shutdown(uint8_t* e, int, uint8_t full);
void __fastcall Terrain_Free(uint8_t* r);
void __fastcall Map_DropMarkers(uint8_t* m);
void __fastcall NamedObj41f590_Destructor(uint8_t* o);
void __fastcall NamedObj456900_Destructor(uint8_t* o);
void __fastcall ModelObj_Destructor(uint8_t* o);
void __fastcall PartModel_Destructor(uint8_t* o);
void __fastcall NamedObj4611e0_Destructor(uint8_t* o);
void __fastcall NamedObj474110_Destructor(uint8_t* o);
void __fastcall RulesDialog_Refresh(uint8_t* w);
void __fastcall Effects_RegisterEmitters();
void __fastcall Render_RestoreTarget(uint8_t* r);
void __fastcall Textures_ReleaseTargets();
void __fastcall VisitGrid_Free(uint8_t* g);
void ReleaseResourceD9399c();
void ReleaseResourceD93990();
void ReleaseGlobalResource();
void ReleaseResourceD97ab0();
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Render_SaveAndShutdown(uint8_t* r);
void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
void __fastcall Mouse_Destroy(uint8_t* m);
void __fastcall Display_DestroyRenderer(uint8_t* o);
void __fastcall Terrain_FreeBlockObjects(uint8_t* r);
void __fastcall Terrain_DestroyCell1681(uint8_t* r);
void __fastcall Terrain_DestroyCell1685(uint8_t* r);
void __fastcall Terrain_DestroyCell1679(uint8_t* r);
void __fastcall Terrain_DestroyCell1675(uint8_t* r);
void __fastcall Terrain_DestroyCell167d(uint8_t* r);
void FreeBuffers6acd20();
void __fastcall Terrain_DeleteMarkers(uint8_t* r);
void __fastcall NamedObj_Destructor(uint8_t* o);
void __fastcall Effect_Free(uint32_t* e);
void __fastcall RulesDialog_SwitchPage(uint8_t* w);
void __fastcall RulesDialog_ShowMessage(uint8_t* w);
void __fastcall RulesDialog_UpdateScope(uint8_t* w);
void __fastcall RulesDialog_ShowSlot(uint8_t* w);
void __fastcall RulesDialog_FillTriggers(uint8_t* w);
void __fastcall RulesDialog_FillConditions(uint8_t* w);
}

namespace {

// One recorder per signature shape, naming the call by a tag set before hooking.
#define REC1(name) void __fastcall Rec_##name(uint8_t* p) { Logf(#name " %p", p); }
#define REC0(name) void __fastcall Rec_##name() { Logf(#name); }
REC1(RestoreTarget) REC0(ReleaseTargets) REC1(VisitFree) REC0(RelD9399c) REC0(RelD93990) REC0(RelGlobal) REC0(RelD97ab0) REC1(SaveShutdown)
REC1(FontHandles) REC1(MouseDestroy) REC1(SceneShutdown) REC1(DestroyRenderer) REC1(FreeBlocks) REC1(Cell1681) REC1(Cell1685) REC1(Cell1679)
REC1(Cell1675) REC1(Cell167d) REC0(FreeBuffers) REC1(DeleteMarkers) REC1(NamedDtor) REC1(EffectFree) REC1(SwitchPage) REC1(ShowMessage)
REC1(UpdateScope) REC1(ShowSlot) REC1(FillTriggers) REC1(FillConditions) REC1(ListDestroy)
void __fastcall Rec_PackRelease(uint8_t* pack, int, void* o) { Logf("pack release %p %p", pack, o); }
int32_t __fastcall Rec_Register(uint8_t* lib, int, uint32_t key, int32_t perPoint, const float* rect, uint32_t flags) {
    Logf("register %p %08x %d %p %x", lib, key, perPoint, rect, flags);
    return 1;
}
void __fastcall Rec_SceneDtor2(uint8_t* o) { Logf("scene dtor %p", o); }
void __cdecl Rec_Delete2(void* p) { Logf("delete %p", p); }
ULONG __stdcall FakeRelease(void* self) {
    Logf("com release %p", self);
    return 0;
}
void FakeShutdownHook() { Logf("shutdown hook"); }
BOOL __stdcall FakeDestroyWindow(HWND h) {
    Logf("destroy window %p", h);
    return TRUE;
}
BOOL __stdcall FakeUnregister(const char* cls, HINSTANCE h) {
    Logf("unregister \"%s\" %p", cls, h);
    return TRUE;
}
// Offset of the first non-zero byte (n when all are zero).
size_t FirstNonZero(const uint8_t* p, size_t n) {
    size_t k = 0;
    while (k < n && !p[k]) ++k;
    return k;
}
void SetIat(uint32_t slot, void* fn, void** saved) {
    DWORD prot;
    VirtualProtect((void*)slot, 4, PAGE_READWRITE, &prot);
    if (saved) *saved = *(void**)slot;
    *(void**)slot = fn;
    VirtualProtect((void*)slot, 4, prot, &prot);
}

} // namespace

DIFFTEST(ui_Shutdowns) {
    SavedGlobals sg({ 0x6d4020, 0x6d4010, 0x62be18, 0x6cea60, 0x6d4030 });
    void* savedIat[2];
    SetIat(0x5c82c4, (void*)&FakeDestroyWindow, &savedIat[0]);
    SetIat(0x5c82c8, (void*)&FakeUnregister, &savedIat[1]);
    {
        UiTrace t;
        t.Add(0x547610, (void*)&rt3::Render_RestoreTarget, (void*)&Rec_RestoreTarget);
        t.Add(0x542c10, (void*)&rt3::Textures_ReleaseTargets, (void*)&Rec_ReleaseTargets);
        t.Add(0x565440, (void*)&rt3::VisitGrid_Free, (void*)&Rec_VisitFree);
        t.Add(0x5549b0, (void*)&rt3::ReleaseResourceD9399c, (void*)&Rec_RelD9399c);
        t.Add(0x553aa0, (void*)&rt3::ReleaseResourceD93990, (void*)&Rec_RelD93990);
        t.Add(0x53a560, (void*)&rt3::ReleaseGlobalResource, (void*)&Rec_RelGlobal);
        t.Add(0x565da0, (void*)&rt3::ReleaseResourceD97ab0, (void*)&Rec_RelD97ab0);
        t.Add(0x53c000, (void*)&rt3::Pack_Release, (void*)&Rec_PackRelease);
        t.Add(0x5571d0, (void*)&rt3::PtrList_Destroy, (void*)&Rec_ListDestroy);
        t.Add(0x54be60, (void*)&rt3::Render_SaveAndShutdown, (void*)&Rec_SaveShutdown);
        t.Add(0x53b920, (void*)&rt3::Pack_ReleaseFontHandles, (void*)&Rec_FontHandles);
        t.Add(0x543e60, (void*)&rt3::Mouse_Destroy, (void*)&Rec_MouseDestroy);
        t.Add(0x55dd80, (void*)&rt3::Display_DestroyRenderer, (void*)&Rec_DestroyRenderer);
        t.Add(0x535b40, (void*)&rt3::Terrain_FreeBlockObjects, (void*)&Rec_FreeBlocks);
        t.Add(0x535c70, (void*)&rt3::Terrain_DestroyCell1681, (void*)&Rec_Cell1681);
        t.Add(0x535d30, (void*)&rt3::Terrain_DestroyCell1685, (void*)&Rec_Cell1685);
        t.Add(0x535df0, (void*)&rt3::Terrain_DestroyCell1679, (void*)&Rec_Cell1679);
        t.Add(0x535a10, (void*)&rt3::Terrain_DestroyCell1675, (void*)&Rec_Cell1675);
        t.Add(0x535f20, (void*)&rt3::Terrain_DestroyCell167d, (void*)&Rec_Cell167d);
        t.Add(0x453890, (void*)&rt3::FreeBuffers6acd20, (void*)&Rec_FreeBuffers);
        t.Add(0x534430, (void*)&rt3::Terrain_DeleteMarkers, (void*)&Rec_DeleteMarkers);
        t.Add(0x455650, (void*)&rt3::NamedObj_Destructor, (void*)&Rec_NamedDtor);
        t.Add(0x475100, (void*)&rt3::Effect_Free, (void*)&Rec_EffectFree);
        t.Add(0x4dab60, (void*)&rt3::RulesDialog_SwitchPage, (void*)&Rec_SwitchPage);
        t.Add(0x4d9f50, (void*)&rt3::RulesDialog_ShowMessage, (void*)&Rec_ShowMessage);
        t.Add(0x4d9970, (void*)&rt3::RulesDialog_UpdateScope, (void*)&Rec_UpdateScope);
        t.Add(0x4da9a0, (void*)&rt3::RulesDialog_ShowSlot, (void*)&Rec_ShowSlot);
        t.Add(0x4d8120, (void*)&rt3::RulesDialog_FillTriggers, (void*)&Rec_FillTriggers);
        t.Add(0x4d88f0, (void*)&rt3::RulesDialog_FillConditions, (void*)&Rec_FillConditions);
        t.Add(0x556c50, (void*)&rt3::EmitterLibrary_Register, (void*)&Rec_Register);
        t.Add(0x530680, (void*)&rt3::SceneObj_Destructor, (void*)&Rec_SceneDtor2);
        t.Add(0x53b080, nullptr, (void*)&Rec_Delete2);
        static void* comVtbl[3] = { nullptr, nullptr, (void*)&FakeRelease };
        static void* com[1] = { comVtbl };
        static uint8_t scene[0x3700], engine[0x60], terrain[0x2200], map[0x2200], obj[0x300], dlg[0x100], effects[0x10 + 8 * 0x140];
        hook::Global<uint32_t>(0x6d4020) = 0x4020400;
        hook::Global<uint32_t>(0x6d4030) = 0x4030400;
        size_t total[6] = {};
        for (int it = 0; it < 1800; ++it) {
            int op = it % 6;
            uint32_t present = Rnd();
            int32_t which = (int32_t)(Rnd() % 6);
            int nRules = (int)(Rnd() % 4);
            uint32_t ruleSel = Rnd() % 5;
            uint8_t ruleBytes[4];
            for (uint8_t& b : ruleBytes) b = (uint8_t)(Rnd() % 13);
            int nEffects = (int)(Rnd() % 8);
            bool full = Rnd() % 2, haveHook = Rnd() % 2;
            std::string logs[2];
            uint32_t outA[2][4];
            for (int side = 0; side < 2; ++side) {
                memset(scene, 0, sizeof scene);
                *(int32_t*)(scene + 1) = (int32_t)((present >> 23) & 1) | (int32_t)((present >> 29) & 1);
                *(void**)(scene + 0x5d) = com;
                *(uint32_t*)(scene + 0x2d) = 0x2d2d0;
                uint32_t bit = 2;
                for (uint32_t off : { 0x95u, 0x99u, 0x9du, 0xa1u, 0xa5u, 0x3665u, 0x364du, 0x65u, 0x3651u, 0x3655u, 0x3659u, 0x365du, 0x3661u, 0x79u, 0x7du, 0x81u,
                                      0x85u, 0x89u, 0x8du }) {
                    if (present & bit) *(uint32_t*)(scene + off) = 0x900000 + off;
                    bit <<= 1;
                }
                if (!(present & 0x80000000)) *(uint32_t*)(scene + 0x2d) = 0;
                memset(engine, 0, sizeof engine);
                *(uint32_t*)engine = 0xe0e0;
                *(uint32_t*)(engine + 4) = 0x4444;
                static uint8_t idleScene[0x10]; // not started: Scene_Shutdown returns at once
                *(uint8_t**)(engine + 0x18) = present & 2 ? idleScene : nullptr;
                *(uint32_t*)(engine + 0x20) = present & 4 ? 0x202020 : 0;
                engine[0x55] = 1;
                hook::Global<void*>(0x6d4010) = haveHook ? (void*)&FakeShutdownHook : nullptr;
                logs[side] = t.Run([&] {
                    memset(terrain, 0, sizeof terrain);
                    uint32_t b2 = 1;
                    for (uint32_t off = 0x15d1; off < 0x166d; off += 4) {
                        if (present & b2) *(void**)(terrain + off) = ArenaAlloc(8);
                        b2 = b2 << 1 | b2 >> 31;
                    }
                    memset(map, 0, sizeof map);
                    *(int32_t*)(map + 0x2169) = present & 8 ? 1 : 0;
                    memset(obj, 0, sizeof obj);
                    *(uint32_t*)(obj + 0x23a) = present & 16 ? 0x23a23a : 0;
                    memset(dlg, 0, sizeof dlg);
                    *(int32_t*)(dlg + 0x84) = ruleSel ? (int32_t)ruleSel : -1;
                    memset(effects, 0, sizeof effects);
                    *(int32_t*)(effects + 8) = nEffects;
                    for (int k = 0; k < nEffects; ++k) {
                        uint8_t* e = effects + 0x10 + k * 0x140;
                        sprintf((char*)e, "Effect%d", k);
                        *(float*)(e + 0x84) = (float)(k * 13 % 7) + 0.5f;
                        *(float*)(e + 0x8c) = (float)(k % 3) + 0.25f;
                        *(int32_t*)(e + 0xb4) = (int32_t)((present >> k) & 1);
                        *(uint32_t*)(e + 0xb8) = present >> 8;
                    }
                    hook::Global<uint8_t*>(0x6cea60) = effects;
                    switch (op) {
                    case 0: side ? rt3::Scene_Shutdown(scene) : ((void(__fastcall*)(uint8_t*))0x55e060)(scene); break;
                    case 1:
                        side ? rt3::Engine_Shutdown(engine, 0, (uint8_t)full) : ((void(__fastcall*)(uint8_t*, int, uint8_t))0x51ffb0)(engine, 0, (uint8_t)full);
                        break;
                    case 2: side ? rt3::Terrain_Free(terrain) : ((void(__fastcall*)(uint8_t*))0x536030)(terrain); break;
                    case 3:
                        side ? rt3::Map_DropMarkers(map) : ((void(__fastcall*)(uint8_t*))0x4539c0)(map);
                        switch (which) {
                        case 0: side ? rt3::NamedObj41f590_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x41f5b0)(obj); break;
                        case 1: side ? rt3::NamedObj456900_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x456170)(obj); break;
                        case 2: side ? rt3::ModelObj_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x45b040)(obj); break;
                        case 3: side ? rt3::PartModel_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x45c460)(obj); break;
                        case 4: side ? rt3::NamedObj4611e0_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x461210)(obj); break;
                        case 5: side ? rt3::NamedObj474110_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x4743d0)(obj); break;
                        }
                        break;
                    case 4: {
                        FakeTable rules(nRules, 0x88f, [&](uint8_t* r, int) {
                            r[0x7ef] = ruleBytes[0];
                            r[0x7f9] = (uint8_t)(ruleBytes[1] % 5);
                            r[0x7fa] = (uint8_t)(ruleBytes[2] % 5);
                            r[0x7f4] = (uint8_t)(ruleBytes[3] % 4);
                            *(uint32_t*)(r + 0x7f5) = ruleBytes[0] & 1;
                            *(uint32_t*)(r + 0x7f0) = ruleBytes[1] * 7u;
                        });
                        hook::Global<Collection*>(0x62be18) = &rules.c;
                        if (ruleSel == 0 || (int)ruleSel <= nRules)
                            side ? rt3::RulesDialog_Refresh(dlg) : ((void(__fastcall*)(uint8_t*))0x4db120)(dlg);
                        break;
                    }
                    case 5: side ? rt3::Effects_RegisterEmitters() : ((void(__fastcall*)())0x474690)(); break;
                    }
                });
                outA[side][0] = (uint32_t)FirstNonZero(terrain, sizeof terrain);
                outA[side][1] = *(uint32_t*)(map + 0x2169) ^ *(uint32_t*)obj ^ *(uint32_t*)(obj + 0x23a);
                outA[side][2] = *(uint32_t*)engine ^ engine[0x55] ^ *(uint32_t*)(engine + 0x18) ^ *(uint32_t*)(engine + 0x20) << 1;
                outA[side][3] = *(uint32_t*)(scene + 1) ^ *(uint32_t*)(dlg + 0x88);
            }
            static const char* names[] = { "scene shutdown", "engine shutdown", "terrain free", "markers / destructors", "rules refresh",
                                           "emitter templates" };
            CompareLogs(logs[0], logs[1], names[op], it);
            EXPECT_EQ(memcmp(outA[0], outA[1], sizeof outA[0]), 0, "%s state iteration %d", names[op], it);
            total[op] += logs[0].size();
            if (it == 1799)
                for (int k = 0; k < 6; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
        }
    }
    SetIat(0x5c82c4, savedIat[0], nullptr);
    SetIat(0x5c82c8, savedIat[1], nullptr);
}

namespace rt3 {
void __fastcall RulesDialog_MoveRuleUp(uint8_t* w);
void __fastcall RulesDialog_MoveRuleDown(uint8_t* w);
void __fastcall RulesDialog_Init(uint8_t* w);
void __fastcall Terrain_Resize(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Terrain_Destroy(uint8_t* r);
void __fastcall ModelObj40c950_Destructor(uint8_t* o);
void __fastcall PartModel41aae0_Destructor(uint8_t* o);
void __fastcall ModelObj_Destructor(uint8_t* o);
void __fastcall PartModel_Destructor(uint8_t* o);
int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
void __fastcall RulesDialog_FillList(uint8_t* w);
void __fastcall RulesDialog_Refresh(uint8_t* w);
void CityDialog_ShowMapParameters();
void __fastcall RulesDialog_FillTerritories(uint8_t* w);
void __fastcall RulesDialog_Reset(uint8_t* w);
void __fastcall Terrain_Free(uint8_t* r);
void __fastcall Render_AllocGrid(uint8_t* r, int, int32_t rows, int32_t cols);
void __fastcall Terrain_AllocMips(uint8_t* t);
void __fastcall Terrain_FreeFixedChannels(uint8_t* r);
void __fastcall Render_ReleaseTextureSlots(uint8_t* r);
void __fastcall Texture_Destroy(uint8_t* t);
}

namespace {

int32_t __fastcall Rec_StoreRule(uint8_t* w) {
    Logf("store rule %p", w);
    return 1;
}
void __fastcall Rec_FillList(uint8_t* w) {
    Logf("fill list %p sel %d", w, *(int32_t*)(w + 0x84));
    *(int32_t*)(w + 0x84) = 77; // filling resets the selection
}
REC1(Refresh) REC0(MapParams) REC1(FillTerritories) REC1(ResetDlg) REC1(TerrainFree) REC1(AllocMips) REC1(FreeChannels) REC1(ReleaseSlots)
REC1(TexDestroy) REC1(ModelDtor) REC1(PartDtor)
void __fastcall Rec_AllocGrid(uint8_t* r, int, int32_t a, int32_t b) { Logf("alloc grid %p %d %d", r, a, b); }

} // namespace

DIFFTEST(ui_RulesAndTerrain) {
    SavedGlobals sg({ 0x62be18, 0x622070 });
    UiTrace t;
    t.Add(0x4d8ea0, (void*)&rt3::RulesDialog_StoreRule, (void*)&Rec_StoreRule);
    t.Add(0x4d7ef0, (void*)&rt3::RulesDialog_FillList, (void*)&Rec_FillList);
    t.Add(0x4db120, (void*)&rt3::RulesDialog_Refresh, (void*)&Rec_Refresh);
    t.Add(0x4d76a0, (void*)&rt3::CityDialog_ShowMapParameters, (void*)&Rec_MapParams);
    t.Add(0x4d7fe0, (void*)&rt3::RulesDialog_FillTerritories, (void*)&Rec_FillTerritories);
    t.Add(0x4daf40, (void*)&rt3::RulesDialog_Reset, (void*)&Rec_ResetDlg);
    t.Add(0x536030, (void*)&rt3::Terrain_Free, (void*)&Rec_TerrainFree);
    t.Add(0x532c80, (void*)&rt3::Render_AllocGrid, (void*)&Rec_AllocGrid);
    t.Add(0x5336e0, (void*)&rt3::Terrain_AllocMips, (void*)&Rec_AllocMips);
    t.Add(0x5359c0, (void*)&rt3::Terrain_FreeFixedChannels, (void*)&Rec_FreeChannels);
    t.Add(0x532b30, (void*)&rt3::Render_ReleaseTextureSlots, (void*)&Rec_ReleaseSlots);
    t.Add(0x542c90, (void*)&rt3::Texture_Destroy, (void*)&Rec_TexDestroy);
    t.Add(0x534430, (void*)&rt3::Terrain_DeleteMarkers, (void*)&Rec_DeleteMarkers);
    t.Add(0x5571d0, (void*)&rt3::PtrList_Destroy, (void*)&Rec_ListDestroy);
    t.Add(0x53b080, nullptr, (void*)&Rec_Delete2);
    t.Add(0x45b040, (void*)&rt3::ModelObj_Destructor, (void*)&Rec_ModelDtor);
    t.Add(0x45c460, (void*)&rt3::PartModel_Destructor, (void*)&Rec_PartDtor);
    static uint8_t dlg[0x100], terrain[0x1700], obj[0x40];
    size_t total[4] = {};
    for (int it = 0; it < 1600; ++it) {
        int op = it % 4;
        int nRules = 1 + (int)(Rnd() % 5);
        uint32_t sel = Rnd() % 7;
        uint32_t swaps = Rnd();
        int32_t row = (int32_t)(Rnd() % 6) - 2;
        int32_t cw = (int32_t)(Rnd() % 3) * 16, ch = (int32_t)(Rnd() % 3) * 16, nw = (int32_t)(Rnd() % 3) * 16, nh = (int32_t)(Rnd() % 3) * 16;
        uint32_t bits = Rnd();
        std::string logs[2];
        uint32_t outA[2][8];
        for (int side = 0; side < 2; ++side) {
            FakeTable rules(nRules, 0x88f, [](uint8_t*, int) {}, 1);
            for (int i = 1; i <= nRules; ++i) *(uint32_t*)((uint8_t*)rt3::Collection_Get(&rules.c, 0, i) + 0x14) = (uint32_t)i;
            for (int k = 0; k < 3; ++k) {
                uint32_t a = 1 + (swaps >> (k * 4)) % nRules, b = 1 + (swaps >> (k * 4 + 2)) % nRules;
                if (a != b) rt3::Collection_SwapOrder(&rules.c, 0, a, b, 2);
            }
            hook::Global<Collection*>(0x62be18) = &rules.c;
            hook::Global<int32_t>(0x622070) = row;
            memset(dlg, 0, sizeof dlg);
            *(int32_t*)(dlg + 0x84) = sel <= (uint32_t)nRules && sel ? (int32_t)sel : -1;
            *(int32_t*)(dlg + 0x90) = nRules;
            memset(terrain, 0, sizeof terrain);
            *(int32_t*)(terrain + 0x15d9) = cw;
            *(int32_t*)(terrain + 0x15d5) = ch;
            *(int32_t*)(terrain + 0x154c) = bits & 1;
            *(uint32_t*)(terrain + 0x478) = bits & 2 ? 0x478478 : 0;
            terrain[0x159b] = 1;
            *(uint32_t*)(terrain + 0x166d) = bits & 4 ? 0x166d166d : 0;
            memset(obj, 0, sizeof obj);
            logs[side] = t.Run([&] {
                switch (op) {
                case 0:
                    if (bits & 8) side ? rt3::RulesDialog_MoveRuleUp(dlg) : ((void(__fastcall*)(uint8_t*))0x4db520)(dlg);
                    else side ? rt3::RulesDialog_MoveRuleDown(dlg) : ((void(__fastcall*)(uint8_t*))0x4db5e0)(dlg);
                    break;
                case 1: side ? rt3::RulesDialog_Init(dlg) : ((void(__fastcall*)(uint8_t*))0x4db6f0)(dlg); break;
                case 2:
                    side ? rt3::Terrain_Resize(terrain, 0, nw, nh) : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x5361d0)(terrain, 0, nw, nh);
                    side ? rt3::Terrain_Destroy(terrain) : ((void(__fastcall*)(uint8_t*))0x537330)(terrain);
                    break;
                case 3:
                    side ? rt3::ModelObj40c950_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x40c970)(obj);
                    Logf("vt %08x", *(uint32_t*)obj);
                    side ? rt3::PartModel41aae0_Destructor(obj) : ((void(__fastcall*)(uint8_t*))0x41ab00)(obj);
                    Logf("vt %08x", *(uint32_t*)obj);
                    break;
                }
            });
            for (int k = 0; k < 6; ++k) outA[side][k] = rt3::Collection_IdAt(&rules.c, 0, (uint32_t)k % nRules, 2);
            outA[side][6] = *(uint32_t*)(dlg + 0x84) ^ *(uint32_t*)(dlg + 0x9c) << 8;
            outA[side][7] = (uint32_t)FirstNonZero(terrain, sizeof terrain) ^ *(uint32_t*)(terrain + 0x15dd) << 12;
        }
        static const char* names[] = { "move rule", "rules init", "terrain resize", "subclass destructors" };
        CompareLogs(logs[0], logs[1], names[op], it);
        EXPECT_EQ(memcmp(outA[0], outA[1], sizeof outA[0]), 0, "%s state iteration %d", names[op], it);
        total[op] += logs[0].size();
        if (it == 1599)
            for (int k = 0; k < 4; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

namespace rt3 {
void __fastcall Company_AiBuyIndustry(uint8_t* c);
void __fastcall Dialog_AddTwoEntryFields(uint8_t* win, int, float x, float y, float w, int32_t);
void __fastcall CompanyView_DrawTerms(void*, int, float x, float y, float w, float, int32_t);
void __fastcall LocoList_DrawRow(void*, uint32_t id, float x, float y, float w, float h, int32_t);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
void* __fastcall Company_Owner(void* c);
int32_t __fastcall Obj_ArgMax67(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
float __fastcall Building_Price(uint8_t* b, int, uint32_t company);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Chain_HasValue(void* o, int, int32_t v);
void __fastcall Company_TakeBuilding(void* c, int, uint32_t building, float price);
const char* __fastcall LocalizeName(const char* name);
double __fastcall Obj_CellY(void* o);
double __fastcall Obj_CellX(void* o);
const char* __fastcall Table_NearestName(Collection* t, int, float x, float y);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
}

namespace {

struct AiFakes {
    int32_t sites, kind4Mask, servedMask, argmax;
    double cash, remaining[8];
    float prices[8];
    uint8_t owner[0x300];
    bool haveOwner;
} g_ai;

int32_t __fastcall FakeSites(void* c, int, int32_t a) {
    Logf("sites %p %d", c, a);
    return g_ai.sites;
}
double __fastcall FakeStat(uint8_t* c, int, int32_t id, int32_t year) {
    Logf("stat %p %x %x", c, id, year);
    return g_ai.cash;
}
void* __fastcall FakeOwner(void* c) { return g_ai.haveOwner ? g_ai.owner : nullptr; }
int32_t __fastcall FakeArgMax(void* o) { return g_ai.argmax; }
int32_t __fastcall FakeKind4(void* o) { return (g_ai.kind4Mask >> (*(uint32_t*)((uint8_t*)o + 0x2a4) & 7)) & 1; }
float __fastcall FakePrice(uint8_t* b, int, uint32_t company) {
    Logf("price %p %x", b, company);
    return g_ai.prices[*(uint32_t*)(b + 0x2a4) & 7];
}
double __fastcall FakeRemaining(void* o, int, int32_t i) { return g_ai.remaining[(i + *(uint32_t*)((uint8_t*)o + 0x2a4)) & 7]; }
uint8_t* __fastcall FakeRegion(void* o) { return (uint8_t*)o + 1; }
int32_t __fastcall FakeServed(void* o, int, int32_t v) {
    Logf("served %p %d", o, v);
    return (g_ai.servedMask >> (*(uint32_t*)((uint8_t*)o - 1 + 0x2a4) & 7)) & 1;
}
void __fastcall RecTake(void* c, int, uint32_t b, float price) { Logf("take %p %x %08x", c, b, B(price)); }
const char* __fastcall FakeLocalize(const char* n) { return n; }
double __fastcall FakeCellY(void* o) { return 12.5; }
double __fastcall FakeCellX(void* o) { return 7.25; }
const char* __fastcall FakeNearest(Collection* t, int, float x, float y) {
    Logf("nearest %p %08x %08x", t, B(x), B(y));
    return "Springfield";
}
uint32_t __fastcall RecLog2(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d) {
    Logf("log %p \"%s\" %d %d %d", t, name, b, c, d);
    return 0;
}
uint8_t g_state[0x60];
uint8_t* __fastcall FakeStateObj(uint8_t* b) { return g_state; }

} // namespace

DIFFTEST(ui_AiAndPanels) {
    SavedGlobals sg({ 0x6cec78, 0x62b26c, 0x62bae0, 0x6acd34, 0x6d0818, 0x62be10, 0x6cfe50, 0x6cfe64, 0x6cfe68, 0x6ada84, 0x62ba88, 0x6cec74, 0x6d3abc,
                      0x6d3ac0 });
    UiTrace t;
    t.Add(0x426590, (void*)&rt3::Company_CountCargoSites, (void*)&FakeSites);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeStat);
    t.Add(0x426ef0, (void*)&rt3::Company_Owner, (void*)&FakeOwner);
    t.Add(0x42a0e0, (void*)&rt3::Obj_ArgMax67, (void*)&FakeArgMax);
    t.Add(0x40d360, (void*)&rt3::Obj_StateKindIs4, (void*)&FakeKind4);
    t.Add(0x40d540, (void*)&rt3::Building_Price, (void*)&FakePrice);
    t.Add(0x40cac0, (void*)&rt3::Obj_Remaining, (void*)&FakeRemaining);
    t.Add(0x455f60, (void*)&rt3::Obj_Region, (void*)&FakeRegion);
    t.Add(0x42c820, (void*)&rt3::Chain_HasValue, (void*)&FakeServed);
    t.Add(0x4269b0, (void*)&rt3::Company_TakeBuilding, (void*)&RecTake);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    t.Add(0x455810, (void*)&rt3::Obj_CellY, (void*)&FakeCellY);
    t.Add(0x455800, (void*)&rt3::Obj_CellX, (void*)&FakeCellX);
    t.Add(0x4220b0, (void*)&rt3::Table_NearestName, (void*)&FakeNearest);
    t.Add(0x4554e0, (void*)&rt3::Log_Add, (void*)&RecLog2);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    static uint8_t world[0x4b00], company[0x200], view[0x100], options[0x200], win[0x40];
    static void* bvt[0x40];
    bvt[0x80 / 4] = (void*)&FakeStateObj;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint32_t>(0x62bae0) = 0x62bae00;
    hook::Global<uint32_t>(0x6acd34) = 0x6acd340;
    hook::Global<uint8_t*>(0x6d0818) = view;
    hook::Global<uint8_t*>(0x6cec74) = options;
    hook::Global<uint32_t>(0x6cfe50) = 0x6cfe500;
    hook::Global<uint32_t>(0x6d3abc) = 0x6d3abc0;
    hook::Global<uint32_t>(0x6d3ac0) = 0x6d3ac00;
    size_t total[4] = {};
    int bought = 0;
    for (int it = 0; it < 4000; ++it) {
        int op = it % 4;
        g_ai.sites = (int32_t)(Rnd() % 4);
        g_ai.cash = Rnd() % 4 ? (double)(Rnd() % 900000) : 300000.0;
        g_ai.kind4Mask = (int32_t)(Rnd() & 0xff);
        g_ai.servedMask = (int32_t)(Rnd() & 0xff);
        g_ai.argmax = (int32_t)(Rnd() % 4);
        g_ai.haveOwner = Rnd() % 2;
        for (int k = 0; k < 8; ++k) {
            g_ai.prices[k] = Rnd() % 6 ? RndF(0.0f, 400000.0f) : RndF(0.0f, 1.0f);
            g_ai.remaining[k] = (double)RndF(-20000.0f, 90000.0f);
        }
        g_state[0x3f] = 0;
        memset(g_ai.owner, 0, sizeof g_ai.owner);
        g_ai.owner[0x291] = (uint8_t)(Rnd() % 40);
        memset(company, 0, sizeof company);
        *(uint32_t*)company = 3;
        strcpy((char*)company + 4, "Acme Rail");
        *(int32_t*)(company + 0x5b) = (int32_t)(Rnd() % 3) - 1;
        for (int k = 0; k < 4; ++k) *(float*)(company + 0x67 + k * 12) = RndF(0.0f, 0.3f);
        *(uint16_t*)(world + 0xd) = 1900;
        *(int32_t*)(world + 0x4abf) = Rnd() % 8 == 0;
        uint32_t ownedMask = Rnd(), stateMask = Rnd();
        FakeTable buildings(Rnd() % 8, 0x400, [&](uint8_t* r, int i) {
            *(void***)r = bvt;
            *(int32_t*)(r + 0x276) = (int32_t)((ownedMask >> i) & 1);
            *(int32_t*)(r + 0x3d5) = 1890 + (int32_t)(Rnd() % 12);
            *(uint32_t*)(r + 0x2a4) = (uint32_t)i + 1;
        });
        hook::Global<Collection*>(0x62b26c) = &buildings.c;
        *(int32_t*)(g_state + 0x3f) = (int32_t)(stateMask & 3);
        sprintf((char*)g_state + 0x4f, "Mine");
        FakeTable companies(1 + Rnd() % 2, 0x100, [](uint8_t*, int) {});
        hook::Global<Collection*>(0x62be10) = &companies.c;
        *(uint32_t*)(view + 0x90) = Rnd() % 4;
        hook::Global<int32_t>(0x6cfe64) = (int32_t)(Rnd() % 50);
        hook::Global<int32_t>(0x6cfe68) = (int32_t)(Rnd() % 100000);
        FakeTable types(3, 0x300, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x45, "Type%d", i);
            *(int32_t*)(r + 0x105) = 1830 + i * 10 + (int32_t)(Rnd() % 9);
            *(int32_t*)(r + 0x109) = 1900 + (int32_t)(Rnd() % 60);
            *(int32_t*)(r + 0x262) = Rnd() % 2 ? -1 : (int32_t)(Rnd() % 3);
            *(uint32_t*)(r + 0x12a) = 0x12a000 + i;
            for (int k = 0; k < 3; ++k) *(uint32_t*)(r + 0x19a + k * 4) = 0x19a000 + i * 16 + k;
            *(int32_t*)(r + 0x3d) = (int32_t)(Rnd() % 5);
        });
        FakeTable locos(3, 0x100, [](uint8_t* r, int) {
            *(uint32_t*)(r + 0x72) = 1 + Rnd() % 3;
            r[0x30] = Rnd() % 2;
            r[0x31] = Rnd() % 2;
            r[0x32] = Rnd() % 2;
        });
        hook::Global<Collection*>(0x62ba88) = &types.c;
        hook::Global<Collection*>(0x6ada84) = &locos.c;
        *(int32_t*)(options + 0x180) = Rnd() % 2;
        uint32_t loco = 1 + Rnd() % 3;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.1f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            logs[side] = t.Run([&] {
                switch (op) {
                case 0: side ? rt3::Company_AiBuyIndustry(company) : ((void(__fastcall*)(uint8_t*))0x4014b0)(company); break;
                case 1:
                    side ? rt3::Dialog_AddTwoEntryFields(win, 0, x, y, w, 0)
                         : ((void(__fastcall*)(uint8_t*, int, float, float, float, int32_t))0x4ed100)(win, 0, x, y, w, 0);
                    break;
                case 2:
                    side ? rt3::CompanyView_DrawTerms(nullptr, 0, x, y, w, h, 0)
                         : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4c4300)(nullptr, 0, x, y, w, h, 0);
                    break;
                case 3:
                    side ? rt3::LocoList_DrawRow(nullptr, loco, x, y, w, h, 0)
                         : ((void(__fastcall*)(void*, uint32_t, float, float, float, float, int32_t))0x50fc00)(nullptr, loco, x, y, w, h, 0);
                    break;
                }
            });
        }
        if (op == 0 && logs[0].find("take ") != std::string::npos) ++bought;
        static const char* names[] = { "ai buy industry", "entry fields", "terms box", "loco row" };
        CompareLogs(logs[0], logs[1], names[op], it);
        total[op] += logs[0].size();
        if (it == 3999)
            for (int k = 0; k < 4; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
        if (it == 3999) fprintf(g_out, "  [ai purchases: %d]\n", bought);
    }
}

namespace rt3 {
void __fastcall Slider_Layout(uint8_t* s);
void __fastcall Terrain_Destructor(uint8_t* r);
void __fastcall Map_Resize(uint8_t* m, int, int32_t w, int32_t h);
void __fastcall Terrain_Destroy(uint8_t* r);
void __fastcall Terrain_Resize(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Map_Free(uint8_t* m);
void __fastcall Map_AllocGrids(uint8_t* m);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Engine_Set1C(void* e, int, void* v);
}

namespace {

REC1(TerrainDestroy) REC1(MapFree) REC1(MapAlloc)
void __fastcall Rec_TerrainResize(uint8_t* r, int, int32_t w, int32_t h) { Logf("terrain resize %p %d %d", r, w, h); }
int32_t g_acqNext;
int32_t __fastcall FakeAcquire2(uint8_t* t, int, uint16_t id) {
    Logf("acquire %p %x", t, id);
    return 0x200 + (g_acqNext++ & 0xff);
}
double __fastcall FakeStepA2(uint8_t* s) { return (double)((uintptr_t)s & 0xff) * 0.001; }
double __fastcall FakeStepB2(uint8_t* s) { return (double)(((uintptr_t)s >> 4) & 0xff) * 0.0013; }

} // namespace

DIFFTEST(ui_SliderAndMap) {
    SavedGlobals sg({ 0x6d401c, 0x6d4024 });
    UiTrace t;
    t.Add(0x537330, (void*)&rt3::Terrain_Destroy, (void*)&Rec_TerrainDestroy);
    t.Add(0x5361d0, (void*)&rt3::Terrain_Resize, (void*)&Rec_TerrainResize);
    t.Add(0x4483d0, (void*)&rt3::Map_Free, (void*)&Rec_MapFree);
    t.Add(0x448470, (void*)&rt3::Map_AllocGrids, (void*)&Rec_MapAlloc);
    t.Add(0x538890, (void*)&rt3::HandleTable_Acquire, (void*)&FakeAcquire2);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeEntry);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeStepA2);
    t.Add(0x5521a0, (void*)&rt3::Slider_StepB, (void*)&FakeStepB2);
    static uint8_t slider[0x1c0], terrain[0x1700], engine[0x40];
    hook::Global<uint32_t>(0x6d401c) = 0x6d401c0;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    size_t total[2] = {};
    for (int it = 0; it < 3000; ++it) {
        int op = it % 2;
        uint8_t start[0x1c0];
        for (uint8_t& b : start) b = (uint8_t)Rnd();
        for (uint32_t o : { 0xbu, 0xfu, 0x13u, 0x17u }) *(float*)(start + o) = RndF(0.0f, 1.0f);
        for (uint32_t o = 0x145; o <= 0x16d; o += 4) *(float*)(start + o) = RndF(-0.1f, 0.5f);
        for (uint32_t o : { 0x109u, 0x111u, 0x119u, 0x11du, 0x121u, 0x125u }) *(int32_t*)(start + o) = Rnd() % 3 ? (int32_t)(Rnd() % 9) : 0;
        *(uint32_t*)(start + 0x92) = 0x9200;
        for (uint32_t o : { 0x129u, 0x12du, 0x131u, 0x135u, 0x139u, 0x13du }) *(int32_t*)(start + o) = (int32_t)(Rnd() % 12);
        *(uint32_t*)(start + 0x47) = 0;
        *(uint32_t*)(start + 0x43) = 0;
        static const char* names2[] = { "Slider.imb", "Arrows.imb" };
        int haveSkin = Rnd() % 3, haveTip = Rnd() % 2;
        int32_t w = (int32_t)(Rnd() % 3) * 16, h = (int32_t)(Rnd() % 3) * 16;
        bool current = Rnd() % 2;
        std::string logs[2];
        uint8_t out[2][0x1c0];
        uint32_t outE[2];
        for (int side = 0; side < 2; ++side) {
            memcpy(slider, start, sizeof slider);
            g_acqNext = it;
            if (haveSkin) *(const char**)(slider + 0x47) = names2[haveSkin - 1];
            if (haveTip) *(const char**)(slider + 0x43) = "tip";
            memset(terrain, 0, sizeof terrain);
            *(int32_t*)(terrain + 0x15d9) = 32;
            *(int32_t*)(terrain + 0x15d5) = 16;
            *(uint8_t**)(engine + 0x1c) = current ? terrain : nullptr;
            logs[side] = t.Run([&] {
                if (op == 0) {
                    side ? rt3::Slider_Layout(slider) : ((void(__fastcall*)(uint8_t*))0x55a170)(slider);
                } else {
                    side ? rt3::Map_Resize(terrain, 0, w, h) : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4494c0)(terrain, 0, w, h);
                    side ? rt3::Terrain_Destructor(terrain) : ((void(__fastcall*)(uint8_t*))0x537e30)(terrain);
                }
            });
            memcpy(out[side], slider, sizeof slider);
            outE[side] = *(uint32_t*)(engine + 0x1c) ^ *(uint32_t*)terrain;
        }
        static const char* names[] = { "slider layout", "map resize / terrain dtor" };
        CompareLogs(logs[0], logs[1], names[op], it);
        int d = 0;
        while (d < 0x1c0 && out[0][d] == out[1][d]) ++d;
        EXPECT_EQ(d, 0x1c0, "%s slider iteration %d at +%x", names[op], it, d);
        EXPECT_EQ(outE[0], outE[1], "%s engine iteration %d", names[op], it);
        total[op] += logs[0].size();
        if (it == 2999)
            for (int k = 0; k < 2; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

namespace rt3 {
void __fastcall Lighting_Rebuild(uint8_t* opts);
void __fastcall Scene_SetLightPair1(uint8_t* s, int, int32_t i, float a, float b);
void __fastcall Scene_SetLightPair0(uint8_t* s, int, int32_t i, float a, float b);
void __fastcall Scene_SetLightRange(uint8_t* s, int, int32_t i, float v);
void __fastcall Scene_ApplyLighting(uint8_t* s);
}

namespace {

void __fastcall RecPair1(uint8_t* s, int, int32_t i, float a, float b) { Logf("pair1 %p %d %08x %08x", s, i, B(a), B(b)); }
void __fastcall RecPair0(uint8_t* s, int, int32_t i, float a, float b) { Logf("pair0 %p %d %08x %08x", s, i, B(a), B(b)); }
void __fastcall RecRange(uint8_t* s, int, int32_t i, float v) { Logf("range %p %d %08x", s, i, B(v)); }
REC1(ApplyLighting)

} // namespace

DIFFTEST(ui_Lighting) {
    SavedGlobals sg({ 0x6d4024, 0x6cec74 });
    UiTrace t;
    t.Add(0x527510, (void*)&rt3::Scene_SetLightPair1, (void*)&RecPair1);
    t.Add(0x5274e0, (void*)&rt3::Scene_SetLightPair0, (void*)&RecPair0);
    t.Add(0x527560, (void*)&rt3::Scene_SetLightRange, (void*)&RecRange);
    t.Add(0x523d90, (void*)&rt3::Scene_ApplyLighting, (void*)&Rec_ApplyLighting);
    static uint8_t engine[0x40], opts[0x400];
    *(uint32_t*)(engine + 0x18) = 0x18181800;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    uint8_t savedKeys[0x198 * 2 + 0x20];
    DWORD prot;
    VirtualProtect((void*)0x620ea0, sizeof savedKeys, PAGE_READWRITE, &prot);
    memcpy(savedKeys, (void*)0x620ea0, sizeof savedKeys);
    size_t total = 0;
    for (int it = 0; it < 300; ++it) {
        memcpy((void*)0x620ea0, savedKeys, sizeof savedKeys);
        if (it % 3 == 1) {
            // Perturbed keys: random values, times rising but possibly ending early.
            float* f = (float*)0x620ea0;
            for (int set = 0; set < 2; ++set) {
                float time = RndF(-2.0f, 3.0f);
                for (int k = 0; k < 17; ++k) {
                    float* e = f + set * 0x66 + k * 6;
                    e[0] = time;
                    time += RndF(0.0f, 2.5f);
                    for (int j = 1; j < 6; ++j) e[j] = RndF(-0.5f, 1.5f);
                }
            }
            for (int j = 0; j < 8; ++j) ((float*)0x620f64)[j] = RndF(-0.5f, 1.5f);
        }
        *(int32_t*)(opts + 0x2c3) = (int32_t)(Rnd() % 3);
        *(int32_t*)(opts + 0x319) = Rnd() % 4 ? (int32_t)(Rnd() % 200) - 20 : 0;
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] { side ? rt3::Lighting_Rebuild(opts) : ((void(__fastcall*)(uint8_t*))0x4821d0)(opts); });
        CompareLogs(logs[0], logs[1], "lighting", it);
        total += logs[0].size();
    }
    memcpy((void*)0x620ea0, savedKeys, sizeof savedKeys);
    VirtualProtect((void*)0x620ea0, sizeof savedKeys, prot, &prot);
    fprintf(g_out, "  [lighting: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
void __fastcall StockWindow_DrawHolders(void*, int, float x, float y, float w, float, int32_t);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
}

namespace {

uint8_t g_players[8][0x200];
int32_t g_nPlayers;
int32_t __fastcall FakeCountPlayers(Collection* t) { return g_nPlayers; }
void* __fastcall FakeNthPlayer(Collection* t, int, int32_t n) { return g_players[n & 7]; }

} // namespace

DIFFTEST(ui_StockHolders) {
    SavedGlobals sg({ 0x62be10, 0x6d1a0c, 0x6d1a30, 0x6d1a34, 0x6d1a00, 0x6d1a04, 0x6d1a20, 0x6d1a24 });
    UiTrace t;
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeCountPlayers);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeNthPlayer);
    hook::Global<uint32_t>(0x6d1a34) = 0x6d1a340;
    size_t total = 0;
    for (int it = 0; it < 1500; ++it) {
        FakeTable companies(3, 0x100, [](uint8_t* r, int) { *(int32_t*)(r + 0x47) = (int32_t)(Rnd() % 200000); });
        hook::Global<Collection*>(0x62be10) = &companies.c;
        hook::Global<uint16_t>(0x6d1a0c) = (uint16_t)(1 + Rnd() % 3);
        hook::Global<int32_t>(0x6d1a30) = (int32_t)(Rnd() % 200000);
        hook::Global<int32_t>(0x6d1a00) = Rnd() % 3 ? (int32_t)(Rnd() % 100000) : (int32_t)(Rnd() % 100000000);
        hook::Global<int32_t>(0x6d1a04) = (int32_t)(Rnd() % 300000);
        g_nPlayers = (int32_t)(Rnd() % 8);
        for (int i = 0; i < 8; ++i) {
            memset(g_players[i], 0, sizeof g_players[i]);
            sprintf((char*)g_players[i] + 8, "Player %d", i);
            for (int c = 0; c < 4; ++c) *(int32_t*)(g_players[i] + 0x15d + c * 4) = Rnd() % 3 ? (int32_t)(Rnd() % 5000) - 500 : 0;
            ((uint8_t*)0x6d1a20)[i] = Rnd() % 2;
        }
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 0.5f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                side ? rt3::StockWindow_DrawHolders(nullptr, 0, x, y, w, 0.0f, 0)
                     : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x50c500)(nullptr, 0, x, y, w, 0.0f, 0);
            });
        CompareLogs(logs[0], logs[1], "stock holders", it);
        total += logs[0].size();
    }
    fprintf(g_out, "  [stock holders: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
void __fastcall Dropdown_Open(uint8_t* c);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
uint8_t* __fastcall Widget_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall TreeView_Init(uint8_t* w);
int32_t __fastcall Widget_Send(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);
}

namespace {

void __fastcall RecSound(int32_t id, int, uint32_t pan) { Logf("sound %d %08x mute %d", id, pan, hook::Global<int32_t>(0xccba14)); }
uint8_t* __fastcall RecWidgetCtor(uint8_t* w, int, const uint8_t* def) {
    LogDef("widget", w, def, 0x12f);
    return w;
}
REC1(TreeInit)
int32_t __fastcall RecWidgetSend(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    Logf("send %p %x %x %s %s %s", w, a, b, Arg(c).c_str(), Arg(d).c_str(), Arg(e).c_str());
    return 0;
}

} // namespace

DIFFTEST(ui_Dropdown) {
    SavedGlobals sg({ 0x6d401c, 0xccba14 });
    UiTrace t;
    t.Add(0x45ea20, (void*)&rt3::Ui_PlaySound, (void*)&RecSound);
    t.Add(0x558580, (void*)&rt3::Widget_Construct, (void*)&RecWidgetCtor);
    t.Add(0x55f9f0, (void*)&rt3::TreeView_Init, (void*)&Rec_TreeInit);
    t.Add(0x558530, (void*)&rt3::Widget_Send, (void*)&RecWidgetSend);
    t.Add(0x538890, (void*)&rt3::HandleTable_Acquire, (void*)&FakeAcquire2);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeEntry);
    t.Add(0x5521a0, (void*)&rt3::Slider_StepB, (void*)&FakeStepB2);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeClock);
    static uint8_t dd[0x1e0], items[16 * 0x20];
    hook::Global<uint32_t>(0x6d401c) = 0x6d401c0;
    size_t total = 0;
    for (int it = 0; it < 2000; ++it) {
        uint8_t start[0x1e0];
        for (uint8_t& b : start) b = (uint8_t)Rnd();
        for (uint32_t o : { 0xbu, 0xfu, 0x13u, 0x17u, 0xb2u, 0xb6u }) *(float*)(start + o) = RndF(0.0f, 0.2f);
        *(int32_t*)(start + 0x1a8) = Rnd() % 6 == 0;
        *(int32_t*)(start + 0x1d2) = (int32_t)(Rnd() % 12);
        *(int32_t*)(start + 0xa2) = (int32_t)(Rnd() % 10);
        *(int32_t*)(start + 0x1d6) = Rnd() % 3 ? (int32_t)(Rnd() % 12) : -1;
        *(uint32_t*)(start + 0x92) = 0x9200;
        *(uint8_t**)(start + 0x176) = items;
        *(const char**)(start + 0x47) = Rnd() % 2 ? "Drop.imb" : nullptr;
        static const char* texts[] = { "One", "Two", "Three", "Four" };
        for (int k = 0; k < 16; ++k) {
            *(const char**)(items + k * 0x20) = texts[k & 3];
            // High bit set: never mistaken for a stack string by the logger.
            *(uint32_t*)(items + k * 0x20 + 4) = Rnd() | 0x80000000;
            *(uint32_t*)(items + k * 0x20 + 8) = Rnd() | 0x80000000;
            *(uint32_t*)(items + k * 0x20 + 0xc) = Rnd() % 2 ? Rnd() : 0;
        }
        g_sf.clock = (int32_t)Rnd();
        int32_t mute = (int32_t)(Rnd() % 2);
        std::string logs[2];
        uint8_t out[2][0x1e0];
        int32_t outMute[2];
        for (int side = 0; side < 2; ++side) {
            memcpy(dd, start, sizeof dd);
            g_acqNext = it;
            hook::Global<int32_t>(0xccba14) = mute;
            logs[side] = t.Run([&] { side ? rt3::Dropdown_Open(dd) : ((void(__fastcall*)(uint8_t*))0x5611a0)(dd); });
            memcpy(out[side], dd, sizeof dd);
            outMute[side] = hook::Global<int32_t>(0xccba14);
        }
        CompareLogs(logs[0], logs[1], "dropdown", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "dropdown object iteration %d", it);
        EXPECT_EQ(outMute[0], outMute[1], "dropdown mute iteration %d", it);
        total += logs[0].size();
    }
    fprintf(g_out, "  [dropdown: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
void __fastcall BuildingView_DrawIncomeTable(uint8_t* b, int32_t kind, float x, float y, float, float, int32_t);
void __fastcall MessagePanel_Refresh(uint8_t* w);
void __fastcall CompanyPicker_Build(uint8_t* win, int, float x, float y, float, float);
void __fastcall TrackPanel_Refresh(uint8_t* w);
void __fastcall MergerWindow_DrawVotes(void*, int, float x, float y, float w, float, int32_t);
void __fastcall Table_MarkStale(Collection* t);
void* __fastcall Table_Oldest(Collection* t, int, uint8_t all);
void* __fastcall Table_FindField4Next(Collection* t, int, int32_t v);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o);
int32_t __fastcall Company_NameIndex(void* c);
int32_t __fastcall Widget_TextLines(uint8_t* w);
int32_t __fastcall Company_HasAssets(void* c);
int32_t __fastcall Game_EffectiveVersion();
int32_t IsMultiplayer();
int32_t __stdcall Net_AllHaveVersion(float v);
int32_t __fastcall Obj_Has98(uint8_t* o);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Scene_PushBusy(uint8_t* s);
void __fastcall Scene_PopBusy(uint8_t* s);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
int32_t __fastcall Company_LiveIndex(void* c);
double __fastcall Building_Total2e8(uint8_t* b);
double __fastcall Building_Total2c0(uint8_t* b);
double __fastcall Building_Total310(uint8_t* b);
double __fastcall Building_Total338(uint8_t* b);
double __fastcall Building_Total360(uint8_t* b);
double __fastcall Building_TotalRemaining(uint8_t* b);
double __fastcall Obj_Float2e8(void* o, int, int32_t i);
double __fastcall Obj_Float2c0(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
double __fastcall Obj_Float338(void* o, int, int32_t i);
double __fastcall Obj_Float360(void* o, int, int32_t i);
}

namespace {

// Message log entries as a fixed array walked by the fakes.
uint8_t g_msgs[10][0x220];
int g_nMsgs;
void __fastcall RecMarkStale(Collection* t) { Logf("mark stale %p", t); }
void* __fastcall FakeOldest(Collection* t, int, uint8_t all) { return g_nMsgs ? g_msgs[0] : nullptr; }
void* __fastcall FakeNext(Collection* t, int, int32_t v) { return v + 1 < g_nMsgs ? g_msgs[v + 1] : nullptr; }
uint32_t __fastcall RecSetFlags(uint8_t* w, int, uint32_t m, uint32_t a, uint32_t b) {
    Logf("flags %p %x %x %x", w, m, a, b);
    return 0;
}
uint8_t g_kids[16][0x120];
uint8_t* __fastcall FakeFindChild(uint8_t* w, int, int32_t id) {
    Logf("find %x", id);
    return g_kids[id & 15];
}
uint8_t* __fastcall FakeNamed20(uint8_t* o) { return (uint8_t*)"Face.imb"; }
int32_t __fastcall FakeNameIndex(void* c) { return 1; }
int32_t __fastcall FakeLines(uint8_t* w) { return 1 + (int32_t)(((uintptr_t)w >> 8) & 3); }
int32_t g_flagsMask;
int32_t __fastcall FakeHasAssets(void* c) { return (g_flagsMask >> (*(uint32_t*)c & 7)) & 1; }
int32_t FakeVersion() { return (g_flagsMask >> 8) & 1 ? 0x67 : 0x66; }
int32_t FakeMp() { return (g_flagsMask >> 9) & 1; }
int32_t __stdcall FakeAllHave(float v) {
    Logf("all have %08x", B(v));
    return (g_flagsMask >> 10) & 1;
}
int32_t __fastcall FakeHas98(uint8_t* o) { return (g_flagsMask >> 11) & 1; }
void __fastcall RecCursor(uint8_t* c, int, int32_t i, int32_t f, int32_t n) { Logf("cursor %p %d %d %d", c, i, f, n); }
REC1(PushBusy) REC1(PopBusy)
void __fastcall RecBrush(uint8_t* s, int, float v) { Logf("brush %p %08x", s, B(v)); }
int32_t __fastcall RecLiveIndex(void* c) {
    Logf("live index %p", c);
    return 0;
}
double __fastcall FakeTotal(uint8_t* b) { return (double)(((uintptr_t)b >> 3) & 0xff) * 13.5 - 900.0; }
double __fastcall FakeYearValue(void* o, int, int32_t i) { return (double)(i * 37 % 11) * 97.25 - 600.0; }

} // namespace

DIFFTEST(ui_PanelsAndTables) {
    SavedGlobals sg({ 0x62b26c, 0x6d0818, 0x6cfcf8, 0x6cec78, 0x6cec7c, 0x6d3c84, 0x6acd34, 0x6cec74, 0x62be68, 0x6ceb9c, 0x62be10, 0x6d4020, 0x6d1118,
                      0x6d10d0, 0x6d10d4, 0x6cfca8, 0x622b0c, 0xccbb20, 0x6d4024, 0x6d10de, 0x6d10dc, 0x6d110c, 0x6d111c, 0x6d1108, 0x6d1104,
                      0x6d10f0, 0x6d10f4 });
    UiTrace t;
    t.Add(0x558580, (void*)&rt3::Widget_Construct, (void*)&RecWidgetCtor);
    t.Add(0x55f9f0, (void*)&rt3::TreeView_Init, (void*)&Rec_TreeInit);
    t.Add(0x4555b0, (void*)&rt3::Table_MarkStale, (void*)&RecMarkStale);
    t.Add(0x4553d0, (void*)&rt3::Table_Oldest, (void*)&FakeOldest);
    t.Add(0x455490, (void*)&rt3::Table_FindField4Next, (void*)&FakeNext);
    t.Add(0x558980, (void*)&rt3::Widget_SetFlags, (void*)&RecSetFlags);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeFindChild);
    t.Add(0x476b30, (void*)&rt3::Obj_NamedEntry20, (void*)&FakeNamed20);
    t.Add(0x424230, (void*)&rt3::Company_NameIndex, (void*)&FakeNameIndex);
    t.Add(0x55b8e0, (void*)&rt3::Widget_TextLines, (void*)&FakeLines);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeClock);
    t.Add(0x427380, (void*)&rt3::Company_HasAssets, (void*)&FakeHasAssets);
    t.Add(0x482e00, (void*)&rt3::Game_EffectiveVersion, (void*)&FakeVersion);
    t.Add(0x434050, (void*)&rt3::IsMultiplayer, (void*)&FakeMp);
    t.Add(0x46d2b0, (void*)&rt3::Net_AllHaveVersion, (void*)&FakeAllHave);
    t.Add(0x4a3cf0, (void*)&rt3::Obj_Has98, (void*)&FakeHas98);
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&RecCursor);
    t.Add(0x523e10, (void*)&rt3::Scene_PushBusy, (void*)&Rec_PushBusy);
    t.Add(0x523e20, (void*)&rt3::Scene_PopBusy, (void*)&Rec_PopBusy);
    t.Add(0x527de0, (void*)&rt3::Scene_SetBrushRadius, (void*)&RecBrush);
    t.Add(0x426890, (void*)&rt3::Company_LiveIndex, (void*)&RecLiveIndex);
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeCountPlayers);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeNthPlayer);
    t.Add(0x4b35d0, (void*)&rt3::Building_Total2e8, (void*)&FakeTotal);
    t.Add(0x4b35b0, (void*)&rt3::Building_Total2c0, (void*)&FakeTotal);
    t.Add(0x4b35f0, (void*)&rt3::Building_Total310, (void*)&FakeTotal);
    t.Add(0x4b3610, (void*)&rt3::Building_Total338, (void*)&FakeTotal);
    t.Add(0x4b3630, (void*)&rt3::Building_Total360, (void*)&FakeTotal);
    t.Add(0x4b3650, (void*)&rt3::Building_TotalRemaining, (void*)&FakeTotal);
    t.Add(0x40ca80, (void*)&rt3::Obj_Float2e8, (void*)&FakeYearValue);
    t.Add(0x40ca70, (void*)&rt3::Obj_Float2c0, (void*)&FakeYearValue);
    t.Add(0x40ca90, (void*)&rt3::Obj_Float310, (void*)&FakeYearValue);
    t.Add(0x40caa0, (void*)&rt3::Obj_Float338, (void*)&FakeYearValue);
    t.Add(0x40cab0, (void*)&rt3::Obj_Float360, (void*)&FakeYearValue);
    t.Add(0x40cac0, (void*)&rt3::Obj_Remaining, (void*)&FakeYearValue);
    static uint8_t view[0x100], world[0x4d00], newGame[0x100], opts[0x400], cam[0x80], pack[0x43000], panel[0x100], bld[0x400],
        renderer[0x3800], hasObj[0x100];
    hook::Global<uint8_t*>(0x6d0818) = view;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x62be68) = cam;
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint32_t>(0x6cfcf8) = 0x6cfcf80;
    hook::Global<uint32_t>(0x6d111c) = 0x6d111c0;
    hook::Global<uint32_t>(0x6acd34) = 0x6acd340;
    hook::Global<uint8_t*>(0x6cfca8) = hasObj;
    hook::Global<uint32_t>(0xccbb20) = 0xccbb200;
    // Full size: Num_Format reads the engine's +0x114732.
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    *(uint8_t**)(engine + 0x18) = renderer;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    size_t total[5] = {};
    for (int it = 0; it < 3000; ++it) {
        int op = it % 5;
        g_flagsMask = (int32_t)Rnd();
        FakeTable buildings(2, 0x100, [](uint8_t*, int) {});
        hook::Global<Collection*>(0x62b26c) = &buildings.c;
        *(uint32_t*)(view + 0x90) = Rnd() % 3;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1860 + Rnd() % 10);
        *(int32_t*)(world + 0x4aef) = Rnd() % 6 == 0;
        *(int32_t*)(world + 0x4aaf) = Rnd() % 2;
        *(int32_t*)(world + 0x4c74) = Rnd() % 2;
        newGame[0x82] = Rnd() % 6 == 0;
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 3);
        *(int32_t*)(bld + 0x3d5) = 1858 + (int32_t)(Rnd() % 12);
        int32_t kind = (int32_t)(Rnd() % 0x18);
        // messages
        g_nMsgs = (int)(Rnd() % 10);
        for (int k = 0; k < 10; ++k) {
            memset(g_msgs[k], 0, sizeof g_msgs[k]);
            *(int32_t*)(g_msgs[k] + 4) = k;
            *(int32_t*)(g_msgs[k] + 8) = Rnd() % 2 ? 0 : (int32_t)Rnd();
            sprintf((char*)g_msgs[k] + 0xd, "Message %d", k);
            *(int32_t*)(g_msgs[k] + 0x20e) = (int32_t)(Rnd() % 0x16) - 1;
            *(uint32_t*)(g_msgs[k] + 0x212) = Rnd() % 4;
        }
        for (int k = 0; k < 0x18; ++k) opts[0x18c + k] = Rnd() % 4 != 0;
        *(int32_t*)(opts + 0x68) = Rnd() % 2;
        *(int32_t*)(cam + 0x60) = Rnd() % 2;
        FakeTable players(3, 0x200, [](uint8_t* r, int) { r[0x15c] = Rnd() % 3 ? (uint8_t)(Rnd() % 0x30) : 0x29; });
        FakeTable companies(3, 0x100, [](uint8_t* r, int i) {
            *(uint32_t*)r = (uint32_t)i + 1;
            *(int32_t*)(r + 0x37) = Rnd() % 2 ? -1 : (int32_t)(Rnd() % 9);
            *(int32_t*)(r + 0x3f) = Rnd() % 2;
            *(int32_t*)(r + 0x47) = (int32_t)(Rnd() % 100000);
        });
        FakeTable names(3, 0x40, [](uint8_t* r, int i) { sprintf((char*)r, "Logo%d.imb", i); });
        hook::Global<Collection*>(0x6ceb9c) = &players.c;
        hook::Global<Collection*>(0x62be10) = &companies.c;
        *(Collection**)(pack + 0x429b0) = &names.c;
        hook::Global<int32_t>(0x6d10d4) = (int32_t)(Rnd() % 4);
        // track panel
        *(int32_t*)(opts + 0x138) = (int32_t)(Rnd() % 4);
        *(int32_t*)(opts + 0x144) = Rnd() % 2;
        *(int32_t*)(opts + 0x140) = (int32_t)(Rnd() % 5) - 1;
        *(int32_t*)(opts + 0x13c) = (int32_t)(Rnd() % 5) - 1;
        static const int32_t tools[] = { 1, 4, 0x40, 2 };
        hook::Global<int32_t>(0x622b0c) = tools[Rnd() % 4];
        // merger
        hook::Global<uint16_t>(0x6d10de) = (uint16_t)(1 + Rnd() % 3);
        hook::Global<uint16_t>(0x6d10dc) = (uint16_t)(1 + Rnd() % 3);
        hook::Global<int32_t>(0x6d110c) = (int32_t)(Rnd() % 100000);
        hook::Global<int32_t>(0x6d1108) = (int32_t)(Rnd() % 100000000);
        hook::Global<int32_t>(0x6d1104) = (int32_t)(Rnd() % 100000000);
        g_nPlayers = (int32_t)(Rnd() % 8);
        for (int i = 0; i < 8; ++i) {
            memset(g_players[i], 0, sizeof g_players[i]);
            sprintf((char*)g_players[i] + 8, "Player %d", i);
            for (int c = 0; c < 4; ++c) *(int32_t*)(g_players[i] + 0x15d + c * 4) = Rnd() % 3 ? (int32_t)(Rnd() % 5000) - 500 : 0;
            ((uint8_t*)0x6d10f2)[i] = Rnd() % 2;
        }
        int32_t busy = Rnd() % 2;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 0.5f);
        std::string logs[2];
        uint8_t outMsgs[2][10][0x10];
        uint8_t outKids[2][16][0x120];
        uint32_t outG[2][3];
        for (int side = 0; side < 2; ++side) {
            uint8_t msgsStart[10][0x10];
            for (int k = 0; k < 10; ++k) memcpy(msgsStart[k], g_msgs[k], 0x10);
            memset(g_kids, 0, sizeof g_kids);
            memset(panel, 0, sizeof panel);
            *(int32_t*)(renderer + 0x3741) = busy;
            int32_t mode = *(int32_t*)(opts + 0x138);
            logs[side] = t.Run([&] {
                switch (op) {
                case 0:
                    side ? rt3::BuildingView_DrawIncomeTable(bld, kind, x, y, w, 0.0f, 0)
                         : ((void(__fastcall*)(uint8_t*, int32_t, float, float, float, float, int32_t))0x4b3820)(bld, kind, x, y, w, 0.0f, 0);
                    break;
                case 1: side ? rt3::MessagePanel_Refresh(panel) : ((void(__fastcall*)(uint8_t*))0x4e0210)(panel); break;
                case 2:
                    side ? rt3::CompanyPicker_Build(panel, 0, x, y, 0.0f, 0.0f)
                         : ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4ec1d0)(panel, 0, x, y, 0.0f, 0.0f);
                    break;
                case 3: side ? rt3::TrackPanel_Refresh(panel) : ((void(__fastcall*)(uint8_t*))0x50d2d0)(panel); break;
                case 4:
                    side ? rt3::MergerWindow_DrawVotes(nullptr, 0, x, y, w, 0.0f, 0)
                         : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4eb890)(nullptr, 0, x, y, w, 0.0f, 0);
                    break;
                }
            });
            for (int k = 0; k < 10; ++k) {
                memcpy(outMsgs[side][k], g_msgs[k], 0x10);
                memcpy(g_msgs[k], msgsStart[k], 0x10);
            }
            memcpy(outKids[side], g_kids, sizeof g_kids);
            outG[side][0] = *(uint32_t*)(opts + 0x138);
            outG[side][1] = hook::Global<uint32_t>(0x6d10d0) ^ hook::Global<uint32_t>(0x6d1118);
            outG[side][2] = *(uint32_t*)(panel + 0x78) ^ *(uint32_t*)(panel + 0x7c) << 1 ^ *(uint32_t*)(panel + 0x90) << 2;
            *(int32_t*)(opts + 0x138) = mode;
        }
        static const char* tn[] = { "building income", "message panel", "company picker", "track panel", "merger votes" };
        CompareLogs(logs[0], logs[1], tn[op], it);
        EXPECT_EQ(memcmp(outMsgs[0], outMsgs[1], sizeof outMsgs[0]), 0, "%s messages iteration %d", tn[op], it);
        EXPECT_EQ(memcmp(outKids[0], outKids[1], sizeof outKids[0]), 0, "%s widgets iteration %d", tn[op], it);
        EXPECT_EQ(memcmp(outG[0], outG[1], sizeof outG[0]), 0, "%s globals iteration %d", tn[op], it);
        total[op] += logs[0].size();
        if (it == 2999)
            for (int k = 0; k < 5; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", tn[k], (unsigned)total[k]);
    }
}

namespace rt3 {
void __fastcall MiniMap_DrawView(uint8_t* m);
void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall Engine_QueueGradientQuad(uint32_t c0, uint32_t c1, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                         uint32_t x3, uint32_t y3, uint32_t c2, uint32_t c3, int32_t clip, uint32_t cx0, uint32_t cy0,
                                         uint32_t cx1, uint32_t cy1);
void __fastcall Obj_CellPos(void* o, int, float* x, float* y);
}

namespace {

float g_corners[12], g_camPos[2];
void __fastcall FakeVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    memcpy(a, g_corners, 16);
    memcpy(b, g_corners + 4, 16);
    memcpy(c, g_corners + 8, 16);
}
void __fastcall RecQuad(uint32_t c0, uint32_t c1, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3,
                        uint32_t c2, uint32_t c3, int32_t clip, uint32_t cx0, uint32_t cy0, uint32_t cx1, uint32_t cy1) {
    Logf("quad %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %d %08x %08x %08x %08x", c0, c1, x0, y0, x1, y1, x2, y2, x3, y3, c2, c3, clip, cx0,
         cy0, cx1, cy1);
}
void __fastcall FakeCellPos(void* o, int, float* x, float* y) {
    *x = g_camPos[0];
    *y = g_camPos[1];
}

} // namespace

DIFFTEST(ui_MiniMapView) {
    SavedGlobals sg({ 0x62c120, 0x62be68, 0x6d4024, 0xd93850, 0x625464 });
    UiTrace t;
    t.Add(0x524170, (void*)&rt3::Scene_GetVectors, (void*)&FakeVectors);
    t.Add(0x54fed0, (void*)&rt3::Engine_QueueGradientQuad, (void*)&RecQuad);
    t.Add(0x439a90, (void*)&rt3::Obj_CellPos, (void*)&FakeCellPos);
    static uint8_t map[0x2200], mm[0x40], engine[0x40];
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint32_t>(0x62be68) = 0x62be680;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    size_t total = 0;
    for (int it = 0; it < 4000; ++it) {
        int32_t W = 64 + (int32_t)(Rnd() % 4) * 64, H = 64 + (int32_t)(Rnd() % 4) * 64;
        *(int32_t*)(map + 0x2155) = W;
        *(int32_t*)(map + 0x2159) = H;
        for (int k = 0; k < 12; ++k) g_corners[k] = RndF(-500.0f, (float)(W > H ? W : H) * 92.0f + 500.0f);
        if (Rnd() % 3 == 0) // a compact view (both edges short)
            for (int k = 1; k < 4; ++k) {
                g_corners[k] = g_corners[0] + RndF(-5.0f, 5.0f);
                g_corners[8 + k] = g_corners[8] + RndF(-5.0f, 5.0f);
            }
        g_camPos[0] = RndF(0.0f, (float)W);
        g_camPos[1] = RndF(0.0f, (float)H);
        for (uint32_t o : { 0xbu, 0xfu, 0x13u, 0x17u }) *(float*)(mm + o) = RndF(0.0f, 0.3f);
        float fade = RndF(-0.1f, 1.0f);
        uint32_t saved = Rnd() % 2 ? 0 : 0xffffffff;
        std::string logs[2];
        uint32_t outF[2][2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<float>(0xd93850) = fade;
            hook::Global<uint32_t>(0x625464) = saved;
            logs[side] = t.Run([&] { side ? rt3::MiniMap_DrawView(mm) : ((void(__fastcall*)(uint8_t*))0x477d80)(mm); });
            outF[side][0] = hook::Global<uint32_t>(0xd93850);
            outF[side][1] = hook::Global<uint32_t>(0x625464);
        }
        CompareLogs(logs[0], logs[1], "minimap view", it);
        EXPECT_EQ(memcmp(outF[0], outF[1], sizeof outF[0]), 0, "minimap fade iteration %d", it);
        total += logs[0].size();
    }
    fprintf(g_out, "  [minimap view: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
void __fastcall MessagePanel_RefreshOpen();
void __fastcall TrackPanel_Init(uint8_t* w);
void __fastcall OptionsDialog_SetLightingSet(uint8_t* d, int, uint8_t on, uint8_t apply);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
}

namespace {

REC1(PanelRefresh) REC1(TrackRefresh) REC1(LightRebuild)
void __fastcall RecOptionsSave(uint8_t* o, int, int32_t e) { Logf("save options %p %d set %d", o, e, *(int32_t*)(o + 0x2c3)); }

} // namespace

DIFFTEST(ui_SmallPanels) {
    SavedGlobals sg({ 0x6d081c, 0x6cec78, 0x6cec74 });
    UiTrace t;
    t.Add(0x4e0210, (void*)&rt3::MessagePanel_Refresh, (void*)&Rec_PanelRefresh);
    t.Add(0x50d2d0, (void*)&rt3::TrackPanel_Refresh, (void*)&Rec_TrackRefresh);
    t.Add(0x484910, (void*)&rt3::Options_Save, (void*)&RecOptionsSave);
    t.Add(0x4821d0, (void*)&rt3::Lighting_Rebuild, (void*)&Rec_LightRebuild);
    static uint8_t world[0x20], opts[0x400], dlg[0x100], panel[0x10];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    for (int it = 0; it < 600; ++it) {
        int op = it % 3;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1850 + Rnd() % 70);
        hook::Global<uint8_t*>(0x6d081c) = Rnd() % 2 ? panel : nullptr;
        uint8_t on = (uint8_t)(Rnd() % 3), apply = (uint8_t)(Rnd() % 2);
        std::string logs[2];
        uint32_t out[2][2];
        for (int side = 0; side < 2; ++side) {
            memset(dlg, 0, sizeof dlg);
            *(int32_t*)(opts + 0x2c3) = 7;
            logs[side] = t.Run([&] {
                switch (op) {
                case 0: side ? rt3::MessagePanel_RefreshOpen() : ((void(__fastcall*)())0x4e0720)(); break;
                case 1: side ? rt3::TrackPanel_Init(panel) : ((void(__fastcall*)(uint8_t*))0x50dc00)(panel); break;
                case 2:
                    side ? rt3::OptionsDialog_SetLightingSet(dlg, 0, on, apply)
                         : ((void(__fastcall*)(uint8_t*, int, uint8_t, uint8_t))0x4dd390)(dlg, 0, on, apply);
                    break;
                }
            });
            out[side][0] = dlg[0x95];
            out[side][1] = *(uint32_t*)(opts + 0x2c3);
        }
        static const char* names[] = { "panel refresh", "track init", "lighting toggle" };
        CompareLogs(logs[0], logs[1], names[op], it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "%s state iteration %d", names[op], it);
    }
}

namespace rt3 {
void __fastcall StationView_DrawCargoGrid(void*, int, float x, float y, float, float);
int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o);
uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
}

namespace {

int32_t g_nCargo;
float g_stock[16], g_quota[16];
uint32_t g_cargoIds[16];
int32_t __fastcall FakeCountCargo(uint8_t* o) { return g_nCargo; }
uint32_t __fastcall FakeNthCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t z) {
    Logf("nth cargo %p %d %d", o, n, z);
    *stock = g_stock[n & 15];
    *quota = g_quota[n & 15];
    return g_cargoIds[n & 15];
}
void __fastcall RecClearFlags(uint8_t* w, int, uint32_t m) { Logf("clear flags %p %x", w, m); }

} // namespace

DIFFTEST(ui_CargoGrid) {
    SavedGlobals sg({ 0x6cec20, 0x6d0818, 0x62ba8c, 0x6d16f4, 0x6d16f8, 0x6d16f0, 0x6d16ec, 0x6d16d8, 0x6d4020, 0x6d4024 });
    UiTrace t;
    t.Add(0x47e330, (void*)&rt3::Obj_CountAvailableCargos, (void*)&FakeCountCargo);
    t.Add(0x47e620, (void*)&rt3::Obj_NthAvailableCargo, (void*)&FakeNthCargo);
    t.Add(0x558980, (void*)&rt3::Widget_SetFlags, (void*)&RecSetFlags);
    t.Add(0x5588b0, (void*)&rt3::Widget_ClearFlags, (void*)&RecClearFlags);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeContainsName);
    static uint8_t view[0x100];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d0818) = view;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint32_t>(0x6d16ec) = 0x6d16ec0;
    hook::Global<uint32_t>(0x6d16d8) = 0x6d16d80;
    hook::Global<uint32_t>(0x6d4020) = 0x40204000;
    size_t total = 0;
    for (int it = 0; it < 1500; ++it) {
        FakeTable stations(2, 0x100, [](uint8_t*, int) {});
        FakeTable cargos(6, 0x80, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Cargo%d", i);
            r[0x46] = Rnd() % 4 == 0;
        });
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        hook::Global<Collection*>(0x62ba8c) = &cargos.c;
        *(uint32_t*)(view + 0x90) = Rnd() % 3;
        g_nCargo = (int32_t)(Rnd() % 14);
        for (int k = 0; k < 16; ++k) {
            g_stock[k] = Rnd() % 4 ? RndF(0.0f, 30.0f) : RndF(0.0f, 0.1f);
            g_quota[k] = RndF(0.0f, 90000.0f);
            g_cargoIds[k] = 1 + Rnd() % 6; // a missing type makes the original read address 0x46
        }
        hook::Global<uint32_t>(0x6d16f4) = Rnd() % 7;
        hook::Global<uint32_t>(0x6d16f8) = Rnd() % 7;
        hook::Global<int32_t>(0x6d16f0) = Rnd() % 4 == 0;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                side ? rt3::StationView_DrawCargoGrid(nullptr, 0, x, y, 0.0f, 0.0f)
                     : ((void(__fastcall*)(void*, int, float, float, float, float))0x505760)(nullptr, 0, x, y, 0.0f, 0.0f);
            });
        CompareLogs(logs[0], logs[1], "cargo grid", it);
        total += logs[0].size();
    }
    fprintf(g_out, "  [cargo grid: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
void __fastcall CompanyCard_Draw(uint8_t* co, int, float x, float y, float w, float h);
int32_t __fastcall Obj_Get25(void* o);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
uint32_t __fastcall Company_Logo(uint8_t* c);
}

namespace {

int32_t g_me;
int32_t __fastcall FakeGet25(void* o) { return g_me; }
void __fastcall RecFrame(uint32_t color, int32_t snap, float x, float y, float w, float h, float t, uint32_t f) {
    Logf("frame %08x %d %08x %08x %08x %08x %08x %x", color, snap, B(x), B(y), B(w), B(h), B(t), f);
}
uint32_t __fastcall FakeLogo(uint8_t* c) { return 0x10600 + c[4]; }

} // namespace

DIFFTEST(ui_CompanyCard) {
    SavedGlobals sg({ 0x6cec78, 0x6cec74, 0x6d4024 });
    UiTrace t;
    t.Add(0x4337b0, (void*)&rt3::Obj_Get25, (void*)&FakeGet25);
    t.Add(0x54f9f0, (void*)&rt3::Ui_FrameRect, (void*)&RecFrame);
    t.Add(0x426b10, (void*)&rt3::Company_Logo, (void*)&FakeLogo);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    static uint8_t world[0x46b00], opts[0x100], co[0x100];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    *(uint32_t*)(world + 0x46a98) = 0x46a980;
    for (int it = 0; it < 1500; ++it) {
        memset(co, 0, sizeof co);
        *(int32_t*)co = (int32_t)(Rnd() % 40);
        sprintf((char*)co + 4, "Company %c", 'A' + (int)(Rnd() % 26));
        *(int32_t*)(co + 0x3b) = (int32_t)(Rnd() % 4);
        g_me = (int32_t)(Rnd() % 4);
        *(int32_t*)(opts + 0x68) = Rnd() % 2;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.2f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                side ? rt3::CompanyCard_Draw(co, 0, x, y, w, h)
                     : ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4bfb30)(co, 0, x, y, w, h);
            });
        CompareLogs(logs[0], logs[1], "company card", it);
    }
}

namespace rt3 {
void __fastcall ScenarioVars_Draw(void*, int, float x, float y, float, float, int32_t);
void __fastcall CompanyReport_Draw(uint8_t* w, int, float x, float y, float wd, float h, int32_t mode);
void __fastcall CargoStats_Draw(void*, int, float x, float y, float, float, int32_t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
int32_t __fastcall Table_CountField3f(Collection* t);
int32_t __fastcall Obj_YearOrNow(void* o);
float __fastcall Company_BondRate(uint8_t* c);
float __fastcall Company_CashRate(uint8_t* c);
int32_t __fastcall Obj_YearReached(void* o);
void __fastcall Cargo_IndustryTotals(uint8_t* o, int, float* base, float* share, float* in, float* inShare);
float __fastcall Obj_Ratio76(uint8_t* o);
}

namespace {

template <typename T> T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
uint8_t g_bigCos[3][0xc00], g_bigPlayers[3][0xc00];
int32_t g_nBigCos, g_nBigPlayers;
void* __fastcall FakeNthCo(Collection* t, int, int32_t n) { return g_bigCos[n % 3]; }
int32_t __fastcall FakeCountCos(Collection* t) { return g_nBigCos; }
void* __fastcall FakeNthBigPlayer(Collection* t, int, int32_t n) { return g_bigPlayers[n % 3]; }
int32_t __fastcall FakeCountBigPlayers(Collection* t) { return g_nBigPlayers; }
int32_t g_year;
int32_t __fastcall FakeYearOrNow(void* o) { return g_year; }
float __fastcall FakeBond(uint8_t* c) { return 0.0625f; }
float __fastcall FakeCash(uint8_t* c) { return 0.03125f; }
int32_t __fastcall FakeReached(void* o) { return (((uintptr_t)o >> 4) & 3) != 0; }
void __fastcall FakeTotals(uint8_t* o, int, float* a, float* b, float* c, float* d) {
    *a = F<float>(o, 0x10);
    *b = F<float>(o, 0x14);
    *c = F<float>(o, 0x18);
    *d = F<float>(o, 0x1c);
}
float __fastcall FakeRatio(uint8_t* o) { return F<float>(o, 0x20) * 3.0f; }

} // namespace

DIFFTEST(ui_ReportTables) {
    SavedGlobals sg({ 0x6cec78, 0x62be10, 0x6ceb9c, 0x6cfc9c, 0x6cff90, 0x6cff8c, 0x6d3c84, 0x62ba8c, 0x6d4024 });
    UiTrace t;
    t.Add(0x429a90, (void*)&rt3::Table_NthField3f, (void*)&FakeNthCo);
    t.Add(0x429a50, (void*)&rt3::Table_CountField3f, (void*)&FakeCountCos);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeNthBigPlayer);
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeCountBigPlayers);
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x425880, (void*)&rt3::Company_BondRate, (void*)&FakeBond);
    t.Add(0x4258c0, (void*)&rt3::Company_CashRate, (void*)&FakeCash);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x41e220, (void*)&rt3::Obj_YearReached, (void*)&FakeReached);
    t.Add(0x41e6e0, (void*)&rt3::Cargo_IndustryTotals, (void*)&FakeTotals);
    t.Add(0x4d03d0, (void*)&rt3::Obj_Ratio76, (void*)&FakeRatio);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    static uint8_t world[0x1000], win[0x1000];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint32_t>(0x6cff90) = 0x6cff900;
    hook::Global<uint32_t>(0x6cff8c) = 0x6cff8c0;
    *(uint32_t*)(win + 0xe55) = 0xe550;
    size_t total[3] = {};
    for (int it = 0; it < 600; ++it) {
        int op = it % 3;
        auto sprinkle = [](uint8_t* base, uint32_t off) {
            for (int32_t type = 0x26; type <= 0x1f6; ++type) *(int32_t*)(base + off + type * 4) = Rnd() % 40 == 0 ? (int32_t)(Rnd() % 9000) - 100 : 0;
        };
        sprinkle(world, 0x8a);
        g_nBigCos = (int32_t)(Rnd() % 4);
        g_nBigPlayers = (int32_t)(Rnd() % 4);
        for (int k = 0; k < 3; ++k) {
            sprintf((char*)g_bigCos[k] + 4, "Co%d", k);
            sprintf((char*)g_bigPlayers[k] + 8, "Pl%d", k);
            sprinkle(g_bigCos[k], 0x35b);
            sprinkle(g_bigPlayers[k], 0x2ab);
        }
        FakeTable cities(Rnd() % 4, 0xc00, [&](uint8_t* r, int i) {
            sprintf((char*)r + 4, "City%d", i);
            *(int32_t*)(r + 0x3d) = (int32_t)(Rnd() % 3) - 1;
            for (int32_t type = 0x26; type <= 0x1f6; ++type) *(int32_t*)(r + 0x3b5 + type * 4) = Rnd() % 40 == 0 ? (int32_t)(Rnd() % 9000) : 0;
        });
        hook::Global<Collection*>(0x6cfc9c) = &cities.c;
        FakeTable companies(2, 0x200, [](uint8_t* r, int) { *(uint16_t*)(r + 0x157) = (uint16_t)(1890 + Rnd() % 12); });
        hook::Global<Collection*>(0x62be10) = &companies.c;
        *(uint32_t*)(win + 0x88) = Rnd() % 3;
        g_year = 1895 + (int32_t)(Rnd() % 10);
        int32_t mode = (int32_t)(Rnd() % 2);
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 5);
        FakeTable cargos(1 + Rnd() % 12, 0x100, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Cargo%d", i);
            r[0x46] = Rnd() % 3 == 0;
            for (uint32_t o = 0x10; o < 0x90; o += 4) *(float*)(r + o) = RndF(-50.0f, 9000.0f);
        });
        hook::Global<Collection*>(0x62ba8c) = &cargos.c;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), wd = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.2f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            logs[side] = t.Run([&] {
                switch (op) {
                case 0:
                    side ? rt3::ScenarioVars_Draw(nullptr, 0, x, y, wd, h, 0)
                         : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d30c0)(nullptr, 0, x, y, wd, h, 0);
                    break;
                case 1:
                    side ? rt3::CompanyReport_Draw(win, 0, x, y, wd, h, mode)
                         : ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e9b20)(win, 0, x, y, wd, h, mode);
                    break;
                case 2:
                    side ? rt3::CargoStats_Draw(nullptr, 0, x, y, wd, h, 0)
                         : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d2a60)(nullptr, 0, x, y, wd, h, 0);
                    break;
                }
            });
        }
        hook::Global<int32_t>(0x6d3c84) = 0;
        static const char* names[] = { "scenario vars", "company report", "cargo stats" };
        CompareLogs(logs[0], logs[1], names[op], it);
        total[op] += logs[0].size();
        if (it == 599)
            for (int k = 0; k < 3; ++k) fprintf(g_out, "  [%s: %u log bytes]\n", names[k], (unsigned)total[k]);
    }
}

namespace rt3 {
char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
}

DIFFTEST(text_DateFormat) {
    SavedGlobals sg({ 0x6d3c84, 0x6d4024 });
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d4024) = engine;
    for (int it = 0; it < 6000; ++it) {
        uint8_t d[8];
        *(uint16_t*)d = (uint16_t)(1800 + Rnd() % 250);
        d[2] = (uint8_t)(Rnd() % 12);
        d[3] = 0;
        d[4] = (uint8_t)(1 + Rnd() % 31);
        d[5] = (uint8_t)(Rnd() % 24);
        d[6] = (uint8_t)(Rnd() % 60);
        int32_t mode = (int32_t)(Rnd() % 7);
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 9);
        char a[0x200], b[0x200];
        memset(a, 0x55, sizeof a);
        memset(b, 0x55, sizeof b);
        char* ra = ((char*(__fastcall*)(char*, const uint8_t*, int32_t))0x51ca00)(a, d, mode);
        char* rb = rt3::Date_Format(b, d, mode);
        EXPECT_EQ(memcmp(a, b, sizeof a), 0, "date mode %d lang %d iteration %d: '%.40s' vs '%.40s'", mode, hook::Global<int32_t>(0x6d3c84), it, a, b);
        EXPECT_EQ(ra == a, rb == b, "date return iteration %d", it);
    }
    hook::Global<int32_t>(0x6d3c84) = 0;
}

namespace rt3 {
void __fastcall StatusBar_ShowDate(uint8_t* w);
void __fastcall StatusBar_ShowCash(uint8_t* w);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
void* __fastcall Obj_Company21(void* o);
}

namespace {

int32_t g_clock[3];
void __fastcall FakeSplit(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s) {
    *h = g_clock[0];
    *m = g_clock[1];
    *s = g_clock[2];
}
uint8_t g_myCo[0x100];
bool g_haveCo;
void* __fastcall FakeCompany21(void* o) { return g_haveCo ? g_myCo : nullptr; }

} // namespace

DIFFTEST(ui_StatusBar) {
    SavedGlobals sg({ 0x6cec74, 0x6cec78, 0x6cec7c, 0x6d4024, 0x6d3c84 });
    UiTrace t;
    t.Add(0x527cc0, (void*)&rt3::Clock_SplitD, (void*)&FakeSplit);
    t.Add(0x434870, (void*)&rt3::Obj_Company21, (void*)&FakeCompany21);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    static uint8_t opts[0x200], world[0x4c00], newGame[0x100], bar[0x40];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    for (int it = 0; it < 3000; ++it) {
        *(int32_t*)(opts + 0xa4) = (int32_t)(Rnd() % 4);
        *(uint16_t*)(world + 5) = (uint16_t)(1830 + Rnd() % 40);
        *(uint16_t*)(world + 0xd) = (uint16_t)(*(uint16_t*)(world + 5) + Rnd() % 900);
        world[0xf] = (uint8_t)(Rnd() % 12);
        world[0x11] = (uint8_t)(1 + Rnd() % 28);
        *(int32_t*)(world + 0x4aeb) = Rnd() % 3 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 6 == 0;
        *(int32_t*)(world + 0x4b43) = (int32_t)(Rnd() % 400);
        newGame[0x82] = Rnd() % 4 == 0;
        g_clock[0] = (int32_t)(Rnd() % 24);
        g_clock[1] = (int32_t)(Rnd() % 60);
        g_clock[2] = (int32_t)(Rnd() % 60);
        g_haveCo = Rnd() % 4 != 0;
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 8);
        int op = it % 2;
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                if (op == 0) side ? rt3::StatusBar_ShowDate(bar) : ((void(__fastcall*)(uint8_t*))0x4df9e0)(bar);
                else side ? rt3::StatusBar_ShowCash(bar) : ((void(__fastcall*)(uint8_t*))0x4dd710)(bar);
            });
        hook::Global<int32_t>(0x6d3c84) = 0;
        CompareLogs(logs[0], logs[1], op ? "status cash" : "status date", it);
    }
}

namespace rt3 {
void __fastcall ScenarioRecords_Draw(void* self, int edx, float x, float y, float w, float h, int32_t a5);
void __fastcall Players_DrawTable(void*, int, float x, float y, float, float h, int32_t);
}

namespace {

void __fastcall RecPlayersTable(void* s, int e, float x, float y, float w, float h, int32_t a) {
    Logf("players table %p %x %08x %08x %08x %08x %d", s, e, B(x), B(y), B(w), B(h), a);
}
void __fastcall RecScenarioVars(void* s, int e, float x, float y, float w, float h, int32_t a) {
    Logf("scenario vars %p %x %08x %08x %08x %08x %d", s, e, B(x), B(y), B(w), B(h), a);
}

} // namespace

DIFFTEST(ui_ScenarioRecords) {
    SavedGlobals sg({ 0x6cec78, 0x62be10, 0x6ceb9c, 0x6cfc9c, 0x6cff90, 0x6d4024 });
    UiTrace t;
    t.Add(0x4d3600, (void*)&rt3::Players_DrawTable, (void*)&RecPlayersTable);
    t.Add(0x4d30c0, (void*)&rt3::ScenarioVars_Draw, (void*)&RecScenarioVars);
    t.Add(0x429a90, (void*)&rt3::Table_NthField3f, (void*)&FakeNthCo);
    t.Add(0x429a50, (void*)&rt3::Table_CountField3f, (void*)&FakeCountCos);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeNthBigPlayer);
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeCountBigPlayers);
    static uint8_t world[0x200];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint32_t>(0x6cff90) = 0x6cff900;
    for (int it = 0; it < 400; ++it) {
        for (int k = 0; k < 4; ++k) *(int32_t*)(world + 0x126 + k * 4) = (int32_t)(Rnd() % 100000) - 500;
        g_nBigCos = (int32_t)(Rnd() % 4);
        g_nBigPlayers = (int32_t)(Rnd() % 4);
        for (int k = 0; k < 3; ++k) {
            sprintf((char*)g_bigCos[k] + 4, "Co%d", k);
            sprintf((char*)g_bigPlayers[k] + 8, "Pl%d", k);
            for (int j = 0; j < 4; ++j) {
                *(int32_t*)(g_bigCos[k] + 0x407 + j * 4) = (int32_t)(Rnd() % 99999);
                *(int32_t*)(g_bigPlayers[k] + 0x367 + j * 4) = (int32_t)(Rnd() % 99999);
            }
        }
        FakeTable cities(Rnd() % 4, 0x500, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "City%d", i);
            *(int32_t*)(r + 0x3d) = (int32_t)(Rnd() % 3) - 1;
            for (int j = 0; j < 4; ++j) *(int32_t*)(r + 0x481 + j * 4) = (int32_t)(Rnd() % 99999);
        });
        hook::Global<Collection*>(0x6cfc9c) = &cities.c;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.2f);
        int32_t a5 = (int32_t)Rnd();
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                side ? rt3::ScenarioRecords_Draw((void*)0x1234, 0x77, x, y, w, h, a5)
                     : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d38c0)((void*)0x1234, 0x77, x, y, w, h, a5);
            });
        CompareLogs(logs[0], logs[1], "scenario records", it);
    }
}

namespace rt3 {
void __fastcall Company_YearEnd(uint8_t* c);
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
int32_t __fastcall Obj_Get21(void* o);
int32_t CountHumanPlayers();
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
}

namespace {

const uint32_t kCoSize = 0x1da0 + 0x19 * 0x38d + 0x40;
float g_price[2];
int g_priceCall;
float __fastcall FakeSharePrice(uint8_t* c, int, int32_t issued, int32_t store) {
    Logf("share price %p %d %d", c, issued, store);
    return g_price[g_priceCall++ & 1];
}
void __fastcall RecRemoveEntry(void* c, int, int32_t idx, int32_t post) { Logf("remove entry %p %d %d", c, idx, post); }
int32_t g_meCo, g_humans;
int32_t __fastcall FakeGet21(void* o) { return g_meCo; }
int32_t FakeHumans() { return g_humans; }
uint32_t __fastcall RecNews(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b) {
    Logf("news %p \"%s\" \"%s\" %d %d", t, title, text, a, b);
    return 0;
}

} // namespace

DIFFTEST(sim_CompanyYearEnd) {
    SavedGlobals sg({ 0x6cec78, 0x6cec7c, 0x6ceb9c, 0x6cea4c, 0x6acd34, 0x6d4024 });
    UiTrace t;
    t.Add(0x424fd0, (void*)&rt3::Company_SharePrice, (void*)&FakeSharePrice);
    t.Add(0x423d70, (void*)&rt3::Company_RemoveEntry, (void*)&RecRemoveEntry);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x4337a0, (void*)&rt3::Obj_Get21, (void*)&FakeGet21);
    t.Add(0x4349a0, (void*)&rt3::CountHumanPlayers, (void*)&FakeHumans);
    t.Add(0x473f80, (void*)&rt3::News_Add, (void*)&RecNews);
    t.Add(0x4554e0, (void*)&rt3::Log_Add, (void*)&RecLog2);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeNthBigPlayer);
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeCountBigPlayers);
    static uint8_t world[0x46d00], newGame[0x100];
    static uint8_t* co = (uint8_t*)VirtualAlloc(nullptr, kCoSize * 3, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* start = co + kCoSize, *out = co + 2 * kCoSize;
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint32_t>(0x6cea4c) = 0x6cea4c0;
    hook::Global<uint32_t>(0x6acd34) = 0x6acd340;
    for (int it = 0; it < 400; ++it) {
        for (uint32_t o = 0; o < kCoSize; o += 4) *(float*)(start + o) = RndF(-1000.0f, 50000.0f);
        *(int32_t*)start = (int32_t)(Rnd() % 4);
        sprintf((char*)start + 4, "Company %d", it);
        *(int32_t*)(start + 0x3f) = Rnd() % 8 != 0;
        *(uint16_t*)(start + 0x157) = (uint16_t)(1890 + Rnd() % 12);
        *(int32_t*)(start + 0x5b) = (int32_t)(Rnd() % 6);
        *(int32_t*)(start + 0x47) = (int32_t)(Rnd() % 100000);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1895 + Rnd() % 8);
        for (int k = 0; k < 6; ++k) *(int32_t*)(start + 0x63 + k * 12) = 1895 + (int32_t)(Rnd() % 8);
        *(float*)(start + 0xd7b) = RndF(0.0f, 400.0f);
        *(int32_t*)(world + 0x46c38) = Rnd() % 8 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 8 == 0;
        newGame[0x82] = Rnd() % 8 == 0;
        g_price[0] = Rnd() % 2 ? RndF(0.0f, 99.0f) : RndF(100.5f, 400.0f);
        g_price[1] = RndF(0.0f, 50.0f);
        g_meCo = (int32_t)(Rnd() % 4);
        g_humans = (int32_t)(Rnd() % 3);
        g_nBigPlayers = (int32_t)(Rnd() % 4);
        uint8_t playersStart[3][0x200];
        for (int k = 0; k < 3; ++k) {
            for (int j = 0; j < 8; ++j) *(int32_t*)(g_bigPlayers[k] + 0x15d + j * 4) = (int32_t)(Rnd() % 9000);
            memcpy(playersStart[k], g_bigPlayers[k], sizeof playersStart[k]);
        }
        std::string logs[2];
        uint8_t outPlayers[2][3][0x200];
        for (int side = 0; side < 2; ++side) {
            memcpy(co, start, kCoSize);
            for (int k = 0; k < 3; ++k) memcpy(g_bigPlayers[k], playersStart[k], sizeof playersStart[k]);
            g_priceCall = 0;
            logs[side] = t.Run([&] { side ? rt3::Company_YearEnd(co) : ((void(__fastcall*)(uint8_t*))0x425bc0)(co); });
            if (!side) memcpy(out, co, kCoSize);
            for (int k = 0; k < 3; ++k) memcpy(outPlayers[side][k], g_bigPlayers[k], sizeof outPlayers[side][k]);
        }
        CompareLogs(logs[0], logs[1], "company year end", it);
        uint32_t d = 0;
        while (d < kCoSize && out[d] == co[d]) ++d;
        EXPECT_EQ(d, kCoSize, "company year end iteration %d first diff +%x: %08x vs %08x", it, d, *(uint32_t*)(out + (d & ~3u)), *(uint32_t*)(co + (d & ~3u)));
        EXPECT_EQ(memcmp(outPlayers[0], outPlayers[1], sizeof outPlayers[0]), 0, "company year end players iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Company_MonthEnd(uint8_t* c);
int32_t __fastcall Company_HumanHoldings(int32_t* o);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
float __fastcall Company_QualityFactor(void* c);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
float __fastcall Company_CashInterestRate(uint8_t* c);
double __fastcall Obj_WeightedAverage(void* o);
float __fastcall Company_MaxDividend(uint8_t* c);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
}

namespace {

struct MonthFakes {
    int32_t holdings;
    float mult, quality, rate, maxDiv[2];
    double weighted;
    int maxCall;
} g_mf;
int32_t __fastcall FakeHoldings(int32_t* o) { return g_mf.holdings; }
float __fastcall FakeEffects(uint8_t* w, int, int32_t idx, uint32_t co, uint32_t pl, uint32_t rg) {
    Logf("effects %p %x %u %u %u", w, idx, co, pl, rg);
    return g_mf.mult + idx * 0.01f;
}
float __fastcall FakeQuality(void* c) { return g_mf.quality; }
void __fastcall RecPostStat(void* c, int, int32_t stat, float amount, int32_t with) { Logf("post %p %x %08x %d", c, stat, B(amount), with); }
float __fastcall FakeCashRate(uint8_t* c) { return g_mf.rate; }
double __fastcall FakeWeighted(void* o) { return g_mf.weighted; }
float __fastcall FakeMaxDiv(uint8_t* c) {
    Logf("max dividend %p", c);
    return g_mf.maxDiv[g_mf.maxCall++ & 1];
}
void __fastcall RecNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f) {
    Logf("entry %p \"%s\" %d %d %d %d %d %d", w, name, a, b, c, d, e, f);
}
void __fastcall RecAddStat(void* c, int, int32_t stat, float amount) { Logf("add stat %p %x %08x", c, stat, B(amount)); }
void __fastcall RecAddDouble(void* o, int, int32_t amount) { Logf("pay %p %d", o, amount); }

} // namespace

DIFFTEST(sim_CompanyMonthEnd) {
    SavedGlobals sg({ 0x6cec78, 0x6cec7c, 0x6ceb9c, 0x6d4024 });
    UiTrace t;
    t.Add(0x426200, (void*)&rt3::Company_HumanHoldings, (void*)&FakeHoldings);
    t.Add(0x436590, (void*)&rt3::Effects_Multiplier, (void*)&FakeEffects);
    t.Add(0x424100, (void*)&rt3::Company_QualityFactor, (void*)&FakeQuality);
    t.Add(0x42a080, (void*)&rt3::Company_PostStat, (void*)&RecPostStat);
    t.Add(0x425a50, (void*)&rt3::Company_CashInterestRate, (void*)&FakeCashRate);
    t.Add(0x423e40, (void*)&rt3::Obj_WeightedAverage, (void*)&FakeWeighted);
    t.Add(0x426260, (void*)&rt3::Company_MaxDividend, (void*)&FakeMaxDiv);
    t.Add(0x4337c0, (void*)&rt3::World_AddNamedEntry, (void*)&RecNamedEntry);
    t.Add(0x42a040, (void*)&rt3::Company_AddStat, (void*)&RecAddStat);
    t.Add(0x476050, (void*)&rt3::Obj_AddDouble154, (void*)&RecAddDouble);
    t.Add(0x424fd0, (void*)&rt3::Company_SharePrice, (void*)&FakeSharePrice);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x4337a0, (void*)&rt3::Obj_Get21, (void*)&FakeGet21);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeNthBigPlayer);
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeCountBigPlayers);
    static uint8_t world[0x46d00], newGame[0x100], co[0x1000], start[0x1000];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    for (int it = 0; it < 3000; ++it) {
        for (uint32_t o = 0; o < sizeof start; o += 4) *(float*)(start + o) = RndF(-100.0f, 5000.0f);
        *(int32_t*)start = (int32_t)(Rnd() % 4);
        *(int32_t*)(start + 0x3f) = Rnd() % 8 != 0;
        *(int32_t*)(start + 0x3b) = Rnd() % 3 ? (int32_t)(1 + Rnd() % 3) : 0;
        *(int32_t*)(start + 0x47) = (int32_t)(Rnd() % 100000);
        *(float*)(start + 0x4b) = Rnd() % 3 ? RndF(0.0f, 5.0f) : 0.0f;
        *(int32_t*)(start + 0x153) = (int32_t)(Rnd() % 9000000);
        *(int32_t*)(start + 0x14f) = (int32_t)(Rnd() % 200000);
        world[0xf] = (uint8_t)(Rnd() % 12);
        *(int32_t*)(world + 0x46c38) = Rnd() % 10 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 8 == 0;
        newGame[0x82] = Rnd() % 8 == 0;
        g_mf.holdings = (int32_t)(Rnd() % 100000);
        g_mf.mult = RndF(0.5f, 1.5f);
        g_mf.quality = RndF(0.5f, 1.5f);
        g_mf.rate = RndF(0.0f, 0.1f);
        g_mf.weighted = (double)RndF(0.0f, 0.1f);
        g_mf.maxDiv[0] = Rnd() % 3 ? RndF(0.0f, 6.0f) : 0.005f;
        g_mf.maxDiv[1] = RndF(0.0f, 6.0f);
        g_meCo = (int32_t)(Rnd() % 4);
        g_nBigPlayers = (int32_t)(Rnd() % 4);
        FakeTable players(3, 0x200, [](uint8_t* r, int) { *(int32_t*)(r + 0x1e1) = Rnd() % 4 == 0; });
        hook::Global<Collection*>(0x6ceb9c) = &players.c;
        uint8_t playersStart[3][0x200];
        for (int k = 0; k < 3; ++k) {
            for (int j = 0; j < 8; ++j) *(int32_t*)(g_bigPlayers[k] + 0x15d + j * 4) = Rnd() % 3 ? (int32_t)(Rnd() % 9000) : 0;
            memcpy(playersStart[k], g_bigPlayers[k], sizeof playersStart[k]);
        }
        std::string logs[2];
        uint8_t out[2][0x1000];
        for (int side = 0; side < 2; ++side) {
            memcpy(co, start, sizeof co);
            g_mf.maxCall = 0;
            g_priceCall = 0;
            logs[side] = t.Run([&] { side ? rt3::Company_MonthEnd(co) : ((void(__fastcall*)(uint8_t*))0x4276d0)(co); });
            memcpy(out[side], co, sizeof co);
        }
        CompareLogs(logs[0], logs[1], "company month end", it);
        int d = 0;
        while (d < (int)sizeof co && out[0][d] == out[1][d]) ++d;
        EXPECT_EQ(d, (int)sizeof co, "company month end iteration %d at +%x", it, d);
    }
}

namespace rt3 {
void __fastcall Camera_UpdateViewSettings(uint8_t* c);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r);
}

namespace {

float g_camHeight, g_camFog;
double __fastcall FakeHeightAt(uint8_t* r, int, float x, float z) {
    Logf("height %p %08x %08x", r, B(x), B(z));
    return g_camHeight;
}
double __fastcall FakeCamFog(uint8_t* s, int, uint8_t* r) {
    Logf("fog %p %p", s, r);
    return g_camFog;
}

} // namespace

DIFFTEST(sim_CameraUpdateDetail) {
    SavedGlobals sg({ 0x62c120, 0x6cec74, 0x6d4024 });
    UiTrace t;
    t.Add(0x534490, (void*)&rt3::Terrain_HeightAt, (void*)&FakeHeightAt);
    t.Add(0x5246b0, (void*)&rt3::Scene_FogFactor, (void*)&FakeCamFog);
    static uint8_t cam[0x400], dev[0x2000], opts[0x200], terrain[0x10];
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    static uint8_t* scene = (uint8_t*)VirtualAlloc(nullptr, 0x23000, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x62c120) = dev;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    *(uint8_t**)(engine + 0x18) = scene;
    *(uint8_t**)(cam + 0x6e) = terrain;
    for (int it = 0; it < 5000; ++it) {
        *(float*)(cam + 0x1c) = RndF(0.0f, 10000.0f);
        *(float*)(cam + 0x24) = RndF(0.0f, 10000.0f);
        *(float*)(cam + 0x20) = Rnd() % 2 ? RndF(-50.0f, 400.0f) : RndF(-100.0f, 5000.0f);
        *(float*)(cam + 0x14) = Rnd() % 2 ? RndF(-1.0f, 2.0f) : RndF(-10.0f, 100.0f);
        g_camHeight = RndF(-50.0f, 200.0f);
        g_camFog = Rnd() % 2 ? RndF(-0.5f, 1.5f) : RndF(-100.0f, 3000.0f);
        if (Rnd() % 50 == 0) { uint32_t nan = 0x7fc00000; g_camFog = *(float*)&nan; }
        *(float*)(dev + 0x14e0) = Rnd() % 2 ? RndF(0.0f, 2.0f) : RndF(-100.0f, 3000.0f);
        *(float*)(dev + 0x1540) = RndF(0.0f, 3000.0f);
        for (int k = 0; k < 12; ++k) *(int32_t*)(opts + 0xac + k * 4) = (int32_t)(Rnd() % 16);
        *(int32_t*)(opts + 0xac + 7 * 4) = (int32_t)(Rnd() % 4);
        engine[0x1146eb] = Rnd() % 4 != 0;
        std::string logs[2];
        uint32_t res[2][8];
        for (int side = 0; side < 2; ++side) {
            memset(scene + 0x20, 0, 0x10);
            logs[side] = t.Run([&] { side ? rt3::Camera_UpdateViewSettings(cam) : ((void(__fastcall*)(uint8_t*))0x43b350)(cam); });
            res[side][0] = *(uint32_t*)(cam + 0x38a);
            res[side][1] = *(uint32_t*)(scene + 0x22b5b);
            res[side][2] = *(uint32_t*)(scene + 0x22b5f);
            res[side][3] = *(uint32_t*)(scene + 0x25);
            res[side][4] = *(uint32_t*)(dev + 0x1534);
            res[side][5] = *(uint32_t*)(dev + 0x1544);
            res[side][6] = *(uint32_t*)(scene + 0x22b8b);
            res[side][7] = 0;
        }
        CompareLogs(logs[0], logs[1], "camera detail", it);
        for (int k = 0; k < 7; ++k)
            EXPECT_EQ(res[0][k], res[1][k], "camera detail iteration %d field %d: %08x vs %08x", it, k, res[0][k], res[1][k]);
    }
}

namespace rt3 {
int32_t __fastcall Sound_BuildCapsReport(uint8_t* o);
}

namespace {

struct FakeDsound {
    void** vtbl;
    int32_t createHr, coopHr;
    uint32_t caps[0x18];
} g_ds;
uint32_t __stdcall DsRelease(FakeDsound* d) { Logf("ds release %p", d); return 0; }
int32_t __stdcall DsGetCaps(FakeDsound* d, uint32_t* caps) {
    Logf("ds caps %p %u", d, caps[0]);
    memcpy(caps + 1, g_ds.caps + 1, 0x5c);
    return 0;
}
int32_t __stdcall DsCoop(FakeDsound* d, HWND w, uint32_t level) { Logf("ds coop %p %p %u", d, w, level); return g_ds.coopHr; }
void* g_dsVtbl[8] = { 0, 0, (void*)&DsRelease, 0, (void*)&DsGetCaps, 0, (void*)&DsCoop, 0 };
int32_t __stdcall FakeDsCreate(void* guid, FakeDsound** out, void* outer) {
    Logf("ds create %p %p", guid, outer);
    *out = &g_ds;
    return g_ds.createHr;
}

} // namespace

DIFFTEST(ui_SoundCapsReport) {
    SavedGlobals sg({ 0x6d4024 });
    UiTrace t;
    t.Add(0x58bc36, nullptr, (void*)&FakeDsCreate);
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d4024) = engine;
    *(uint32_t*)engine = 0x1234;
    g_ds.vtbl = g_dsVtbl;
    static uint8_t obj[0x900];
    for (int it = 0; it < 300; ++it) {
        g_ds.createHr = Rnd() % 8 == 0 ? (int32_t)0x80004005 : 0;
        g_ds.coopHr = Rnd() % 8 == 0 ? (int32_t)0x80004005 : 0;
        for (int k = 0; k < 0x18; ++k) g_ds.caps[k] = Rnd() % 3 ? Rnd() % 100000 : Rnd();
        g_ds.caps[1] = Rnd() & 0xfff;
        std::string logs[2];
        int32_t r[2];
        std::string text[2];
        for (int side = 0; side < 2; ++side) {
            memset(obj, 0x55, sizeof obj);
            logs[side] = t.Run([&] { r[side] = side ? rt3::Sound_BuildCapsReport(obj) : ((int32_t(__fastcall*)(uint8_t*))0x545e30)(obj); });
            text[side].assign((char*)obj, sizeof obj);
        }
        CompareLogs(logs[0], logs[1], "sound caps", it);
        EXPECT_EQ(r[0], r[1], "sound caps result iteration %d", it);
        EXPECT_EQ(text[0] == text[1], true, "sound caps text iteration %d", it);
    }
}

namespace rt3 {
void __fastcall BuildingRow_AddCargoButtons(uint8_t* w, uint8_t* list, int32_t type, float x, float y, int32_t, int32_t);
}

DIFFTEST(ui_BuildingCargoButtons) {
    SavedGlobals sg({ 0x62ba8c, 0x6d4020 });
    UiTrace t;
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeContainsName);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    static uint8_t win[0x300], list[0x10], recs[8 * 0x33], images[0x10];
    hook::Global<uint8_t*>(0x6d4020) = images;
    *(uint8_t**)list = recs;
    *(uint8_t**)(win + 0x86) = (uint8_t*)0x6860000;
    for (int it = 0; it < 600; ++it) {
        FakeTable cargo(1 + Rnd() % 5, 0x80, [](uint8_t* r, int i) { sprintf((char*)r + 4, "Cargo%d%s", i, Rnd() % 2 ? "x" : ""); });
        hook::Global<Collection*>(0x62ba8c) = &cargo.c;
        for (uint32_t k = 0; k < sizeof recs; ++k) recs[k] = (uint8_t)Rnd();
        auto pick = [] { uint32_t r = Rnd() % 10; return r < 6 ? (int32_t)(Rnd() % 7) : r < 9 ? 0x384 + (int32_t)(Rnd() % 3) : (int32_t)(Rnd() % 1000); };
        for (int k = 0; k < 8; ++k) {
            uint8_t* e = recs + k * 0x33;
            for (int j = 0; j < 8; ++j) *(int32_t*)(e + 1 + j * 4) = pick();
            *(int32_t*)(e + 0x24) = pick();
            e[0x23] = (uint8_t)(Rnd() % 3);
            e[0x28] = (uint8_t)(Rnd() % 6 == 0 ? 0x80 : Rnd() & 0x70);
        }
        uint16_t kinds[] = { 0x65a4, 0x2aff, 0x1234 };
        *(uint16_t*)(win + 4) = kinds[Rnd() % 3];
        *(int32_t*)(win + 0x1e2) = (int32_t)(Rnd() % 4);
        int32_t type = (int32_t)(Rnd() % 8);
        *(int32_t*)(list + 8) = Rnd() % 2 ? type : (int32_t)(Rnd() % 8);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, uint8_t*, int32_t, float, float, int32_t, int32_t))0x4be8d0)(win, list, type, x, y, 0, 0); });
        std::string b = t.Run([&] { rt3::BuildingRow_AddCargoButtons(win, list, type, x, y, 0, 0); });
        CompareLogs(a, b, "building cargo buttons", it);
        EXPECT_EQ(a.size() > 200, true, "building cargo buttons log iteration %d", it);
    }
}

namespace rt3 {
void __fastcall BuildingCounts_DrawTable(void*, int, float x, float y, float, float, int32_t);
int32_t __fastcall Table_CountField3d0(Collection* t, int, int32_t v);
int32_t __fastcall Obj_AllUnusedSomeUndefined(void* o);
}

namespace {
int32_t __fastcall FakeCount3d0(Collection* t, int, int32_t v) {
    Logf("count3d0 %p %d", t, v);
    return (v * 37 + 11) % 5 == 0 ? 0 : (v * 13) % 9;
}
int32_t __fastcall FakeAllUnused(void* o) { return (int32_t)(((uintptr_t)o >> 4) % 3 == 0); }
} // namespace

DIFFTEST(ui_BuildingCountsTable) {
    SavedGlobals sg({ 0x62b268, 0x62b26c, 0x6d4034, 0x6d3c84, 0x6cec78 });
    UiTrace t;
    t.Add(0x4138f0, (void*)&rt3::Table_CountField3d0, (void*)&FakeCount3d0);
    t.Add(0x411d50, (void*)&rt3::Obj_AllUnusedSomeUndefined, (void*)&FakeAllUnused);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    static uint8_t world[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint32_t>(0x62b26c) = 0x62b26c0;
    for (int it = 0; it < 400; ++it) {
        FakeTable types(Rnd() % 40, 0x800, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x794) = Rnd() % 8 == 0;
            r[0x32] = (uint8_t)(Rnd() % 6);
            r[0xba] = Rnd() % 5 == 0;
            r[0xbb] = Rnd() % 5 == 0;
            *(int32_t*)(r + 0xe4) = (int32_t)(Rnd() % 50);
            *(int32_t*)(r + 0x78c) = Rnd() % 4 == 0;
            *(int32_t*)(r + 0x7ac) = Rnd() % 2;
            *(uint16_t*)(r + 0x26) = (uint16_t)(1880 + Rnd() % 40);
            sprintf((char*)r + 0x4f, "Building %d", i);
        });
        hook::Global<Collection*>(0x62b268) = &types.c;
        hook::Global<int32_t>(0x6d4034) = Rnd() % 2;
        hook::Global<int32_t>(0x6d3c84) = Rnd() % 3;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1880 + Rnd() % 40);
        float x = RndF(0.0f, 300.0f), y = RndF(0.0f, 300.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d2430)(nullptr, 0, x, y, 0, 0, 0); });
        std::string b = t.Run([&] { rt3::BuildingCounts_DrawTable(nullptr, 0, x, y, 0, 0, 0); });
        CompareLogs(a, b, "building counts", it);
        EXPECT_EQ(a.size() > 100, true, "building counts log iteration %d", it);
    }
}

namespace rt3 {
void __fastcall HighScores_Draw(void*, int, float, float y, float w, float, int32_t);
}

DIFFTEST(ui_HighScores) {
    SavedGlobals sg({ 0x6d1084, 0x6d107c, 0x6d108c, 0x6d1090 });
    static uint8_t saved[0x8 + 20 * 0x6a], savedFlag;
    memcpy(saved, (void*)0x6d0828, sizeof saved);
    savedFlag = hook::Global<uint8_t>(0x6d1088);
    UiTrace t;
    hook::Global<uint32_t>(0x6d1084) = 0x6d10840;
    hook::Global<uint32_t>(0x6d107c) = 0x6d107c0;
    for (int it = 0; it < 300; ++it) {
        hook::Global<int32_t>(0x6d0828) = (int32_t)(Rnd() % 13) - 1;
        hook::Global<int32_t>(0x6d082c) = (int32_t)(Rnd() % 13) - 1;
        for (int r = 0; r < 20; ++r) {
            uint8_t* rec = (uint8_t*)0x6d0830 + r * 0x6a;
            sprintf((char*)rec, "Name%d", (int)(Rnd() % 1000));
            sprintf((char*)rec + 0x1f, "Scenario%d", (int)(Rnd() % 1000));
            *(int32_t*)(rec + 0x3e) = (int32_t)(Rnd() % 5);
            *(int32_t*)(rec + 0x42) = (int32_t)(Rnd() % 4);
            for (int k = 0; k < 4; ++k) *(int32_t*)(rec + 0x46 + k * 4) = (int32_t)(Rnd() % 2000000) - 1000;
        }
        hook::Global<uint8_t>(0x6d1088) = Rnd() % 2;
        hook::Global<int32_t>(0x6d108c) = (int32_t)(Rnd() % 2);
        hook::Global<int32_t>(0x6d1090) = (int32_t)(Rnd() % 10);
        float y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4e0e90)(nullptr, 0, 0.0f, y, w, 0, 0); });
        std::string b = t.Run([&] { rt3::HighScores_Draw(nullptr, 0, 0.0f, y, w, 0, 0); });
        CompareLogs(a, b, "high scores", it);
        EXPECT_EQ(a.size() > 100, true, "high scores log iteration %d", it);
    }
    memcpy((void*)0x6d0828, saved, sizeof saved);
    hook::Global<uint8_t>(0x6d1088) = savedFlag;
}

namespace rt3 {
void __fastcall CompanyRanking_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
void* __fastcall Obj_Ref25(void* o);
int32_t World_AnyCondition();
int32_t __fastcall Obj_Get25(void* o);
void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t,
                            float, float, float, float);
}

namespace {
void __fastcall RecGlyphQueue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t a, int32_t b, int32_t c,
                              float d, float e, float f, float h) {
    Logf("glyphq %p %08x %08x %08x %08x %08x %08x %d %d %d %08x %08x %08x %08x", g, B(x), B(y), tint, B(sx), B(sy), color, a, b, c, B(d), B(e), B(f), B(h));
}
void __fastcall RecFillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags) {
    Logf("fill %08x %d %08x %08x %08x %08x %x", color, snap, B(x), B(y), B(w), B(h), flags);
}
uint8_t g_ref25[0x300];
int32_t g_anyCond, g_rankGet25;
void* __fastcall FakeRef25(void* o) { Logf("ref25 %p", o); return g_ref25; }
int32_t FakeAnyCondition() { Logf("any condition"); return g_anyCond; }
int32_t __fastcall FakeRankGet25(void* o) { return g_rankGet25; }
double __fastcall FakeRankStat(uint8_t* c, int, int32_t id, int32_t year) {
    Logf("stat %p %x %d", c, id, year);
    return (double)((c[4] * 7919 + id * 31) % 2001) - (c[5] % 3 == 0 ? 0 : 500) + (c[6] % 4 == 0 ? 0.0 : 0.5);
}
} // namespace

DIFFTEST(ui_CompanyRanking) {
    SavedGlobals sg({ 0x62be10, 0x6cec78 });
    UiTrace t;
    t.Add(0x426b10, (void*)&rt3::Company_Logo, (void*)&FakeLogo);
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x4348c0, (void*)&rt3::Obj_Ref25, (void*)&FakeRef25);
    t.Add(0x476a60, (void*)&rt3::World_AnyCondition, (void*)&FakeAnyCondition);
    t.Add(0x4337b0, (void*)&rt3::Obj_Get25, (void*)&FakeRankGet25);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeRankStat);
    t.Add(0x54f710, (void*)&rt3::Ui_FillRect, (void*)&RecFillRect);
    t.Add(0x552560, (void*)&rt3::Glyph_Queue, (void*)&RecGlyphQueue);
    static uint8_t panel[0x1000], world[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    for (int it = 0; it < 400; ++it) {
        FakeTable cos(Rnd() % 6 == 0 ? 0 : 1 + Rnd() % 30, 0x100, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x3f) = Rnd() % 5 != 0;
            *(int32_t*)(r + 0x3b) = (int32_t)(Rnd() % 4);
            sprintf((char*)r + 4, "%c%cCo %d", 'A' + (int)(Rnd() % 26), 'A' + (int)(Rnd() % 26), i);
        });
        hook::Global<Collection*>(0x62be10) = &cos.c;
        g_anyCond = Rnd() % 2;
        *(int32_t*)(g_ref25 + 0x2a7) = Rnd() % 2;
        g_rankGet25 = (int32_t)(Rnd() % 4);
        g_year = 1900 + (int32_t)(Rnd() % 20);
        panel[0x760] = Rnd() % 2;
        *(int32_t*)(panel + 0x7c) = (int32_t)(Rnd() % 3);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        int32_t key = (int32_t)(Rnd() % 6);
        uint8_t out[2][0x640];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e68e0)(panel, 0, x, y, w, h, key); });
        memcpy(out[0], panel + 0x11c, 0x640);
        std::string b = t.Run([&] { rt3::CompanyRanking_Draw(panel, 0, x, y, w, h, key); });
        memcpy(out[1], panel + 0x11c, 0x640);
        CompareLogs(a, b, "company ranking", it);
        EXPECT_EQ(memcmp(out[0], out[1], 0x640), 0, "company ranking slots iteration %d", it);
        EXPECT_EQ(a.size() > 30, true, "company ranking log iteration %d", it);
    }
}

namespace rt3 {
void __fastcall PlayerRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey);
double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
int32_t __fastcall Obj_NameIndex(void* o);
}

namespace {
double PlayerFig(uint8_t* p, int k) { return (double)((p[8 + k] * 7919 + p[9] * 31) % 4001) - 1500.0 + (p[10] % 3 ? 0.25 : 0.0); }
double __fastcall FakeHoldings2(uint8_t* p, int, int32_t s) { Logf("holdings %p %d", p, s); return PlayerFig(p, 1); }
double __fastcall FakeNetWorth(uint8_t* p, int, int32_t s) { Logf("networth %p %d", p, s); return PlayerFig(p, 2); }
double __fastcall FakeCollateral(uint8_t* p, int, int32_t s) { Logf("collateral %p %d", p, s); return PlayerFig(p, 3); }
int32_t __fastcall FakeRankNameIndex(void* o) { Logf("name index %p", o); return ((uint8_t*)o)[11] % 3 == 0 ? -1 : ((uint8_t*)o)[11] % 4; }
} // namespace

DIFFTEST(ui_PlayerRanking) {
    SavedGlobals sg({ 0x6ceb9c, 0x6cec78 });
    UiTrace t;
    t.Add(0x476320, (void*)&rt3::Player_HoldingsValue, (void*)&FakeHoldings2);
    t.Add(0x476780, (void*)&rt3::Player_NetWorth, (void*)&FakeNetWorth);
    t.Add(0x476c20, (void*)&rt3::Player_CollateralWorth, (void*)&FakeCollateral);
    t.Add(0x476bb0, (void*)&rt3::Obj_NameIndex, (void*)&FakeRankNameIndex);
    t.Add(0x4337b0, (void*)&rt3::Obj_Get25, (void*)&FakeRankGet25);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x54f710, (void*)&rt3::Ui_FillRect, (void*)&RecFillRect);
    t.Add(0x552560, (void*)&rt3::Glyph_Queue, (void*)&RecGlyphQueue);
    static uint8_t panel[0x1000], world[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    *(uint32_t*)(panel + 0xb25) = 0xb250000;
    for (int k = 0; k < 4; ++k) *(uint32_t*)(panel + 0x995 + k * 4) = 0x9950000 + k * 0x100;
    for (int it = 0; it < 400; ++it) {
        FakeTable pls(Rnd() % 8 == 0 ? 0 : 1 + Rnd() % 25, 0x200, [](uint8_t* r, int i) {
            *(int32_t*)(r + 4) = Rnd() % 5 != 0;
            sprintf((char*)r + 8, "%c%c%cPl %d", 'A' + (int)(Rnd() % 26), 'A' + (int)(Rnd() % 26), 'A' + (int)(Rnd() % 26), i);
            *(double*)(r + 0x154) = (double)((int)(Rnd() % 100000) - 20000) * 0.5;
            r[0x15c] = Rnd() % 3 == 0 ? 0x29 : (uint8_t)(Rnd() % 50);
        }, 1);
        hook::Global<Collection*>(0x6ceb9c) = &pls.c;
        g_rankGet25 = (int32_t)(Rnd() % 6);
        panel[0x760] = Rnd() % 2;
        *(int32_t*)(panel + 0x7c) = (int32_t)(Rnd() % 3);
        *(int32_t*)(panel + 0x98) = 0x3f48 + (int32_t)(Rnd() % 10);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f);
        int32_t key = (int32_t)(Rnd() % 6);
        uint8_t snap[0x200 * 30];
        int n = (int)pls.c.count;
        (void)snap; (void)n;
        uint8_t out[2][0x640];
        uint8_t faces0[32];
        for (int k = 0; k < (int)pls.c.count && k < 32; ++k) faces0[k] = ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e6ef0)(panel, 0, x, y, w, 0, key); });
        memcpy(out[0], panel + 0x11c, 0x640);
        for (int k = 0; k < (int)pls.c.count && k < 32; ++k) ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c] = faces0[k];
        std::string b = t.Run([&] { rt3::PlayerRanking_Draw(panel, 0, x, y, w, 0, key); });
        memcpy(out[1], panel + 0x11c, 0x640);
        CompareLogs(a, b, "player ranking", it);
        EXPECT_EQ(memcmp(out[0], out[1], 0x640), 0, "player ranking slots iteration %d", it);
    }
}

namespace rt3 {
void __fastcall LocoStats_DrawTable(void*, int, float x, float y, float, float, int32_t);
int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind);
}

namespace {
int32_t __fastcall FakeCountOfKind(void* o, int, int32_t kind) { Logf("count kind %p %d", o, kind); return ((uint8_t*)o)[0x357] % 7; }
} // namespace

DIFFTEST(ui_LocoStatsTable) {
    SavedGlobals sg({ 0x62bae0, 0x6cff7c, 0x6cff90 });
    UiTrace t;
    t.Add(0x4228b0, (void*)&rt3::Obj_CountBuildingsOfKind, (void*)&FakeCountOfKind);
    hook::Global<uint32_t>(0x6cff90) = 0x6cff900;
    for (int it = 0; it < 200; ++it) {
        FakeTable locos(Rnd() % 8 == 0 ? 0 : 1 + Rnd() % 90, 0x400, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x242) = Rnd() % 6 != 0;
            *(int32_t*)(r + 0x23e) = Rnd() % 4 == 0;
            sprintf((char*)r + 0x356, "%c%c Loco %d", 'A' + (int)(Rnd() % 6), 'a' + (int)(Rnd() % 26), i % 7);
            for (uint32_t o = 0x2ca; o <= 0x2ee; o += 4) *(float*)(r + o) = RndF(0.0f, 900.0f);
            *(float*)(r + 0x25a) = RndF(0.0f, 300.0f);
            *(float*)(r + 0x25e) = RndF(0.0f, 300.0f);
            *(int32_t*)(r + 0x286) = (int32_t)(Rnd() % 200000);
            *(int32_t*)(r + 0x296) = (int32_t)(Rnd() % 200000);
        });
        hook::Global<Collection*>(0x62bae0) = &locos.c;
        int32_t page = (int32_t)(Rnd() % 7) - 2;
        float x = RndF(0.0f, 300.0f), y = RndF(0.0f, 300.0f);
        int32_t pg[2];
        hook::Global<int32_t>(0x6cff7c) = page;
        std::string a = t.Run([&] { ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d1d60)(nullptr, 0, x, y, 0, 0, 0); });
        pg[0] = hook::Global<int32_t>(0x6cff7c);
        hook::Global<int32_t>(0x6cff7c) = page;
        std::string b = t.Run([&] { rt3::LocoStats_DrawTable(nullptr, 0, x, y, 0, 0, 0); });
        pg[1] = hook::Global<int32_t>(0x6cff7c);
        CompareLogs(a, b, "loco stats", it);
        EXPECT_EQ(pg[0], pg[1], "loco stats page iteration %d", it);
        EXPECT_EQ(a.size() > 100, true, "loco stats log iteration %d", it);
    }
}

namespace rt3 {
void __fastcall EngineStats_Draw(void*, int, float x, float y, float w, float h, int32_t);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
float __fastcall Obj_ProductionRate(void* o, int, int32_t i);
}

namespace {
int32_t __fastcall FakeCellLow3(void* m, int, int32_t x, int32_t y) { return (x * 7 + y * 13) % 5 == 0; }
float __fastcall FakeProdRate(void* o, int, int32_t i) {
    Logf("rate %p %d", o, i);
    uint8_t* r = (uint8_t*)o;
    return (float)((r[0x357] * 31 + i * 17) % 1000) * 0.37f;
}
} // namespace

DIFFTEST(ui_EngineStats) {
    SavedGlobals sg({ 0x62bae0, 0x62c120, 0x6cff88, 0x6d3c84, 0x6cffb0, 0x6cffb4, 0x6cffc8, 0x6cffac, 0x6cffcc, 0x6cffc0, 0x6cffd0, 0x6cffd4 });
    UiTrace t;
    t.Add(0x448f00, (void*)&rt3::Map_CellLow3, (void*)&FakeCellLow3);
    t.Add(0x4234e0, (void*)&rt3::Obj_ProductionRate, (void*)&FakeProdRate);
    static uint8_t map[0x2200];
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint32_t>(0x6cff88) = 0x6cff880;
    for (int it = 0; it < 300; ++it) {
        FakeTable locos(Rnd() % 8 == 0 ? 0 : 1 + Rnd() % 40, 0x400, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x23e) = Rnd() % 3 == 0;
            *(int32_t*)(r + 0x242) = (int32_t)(Rnd() % 5);
            *(float*)(r + 0x25a) = RndF(0.0f, 300.0f);
            r[0x357] = (uint8_t)Rnd();
        });
        hook::Global<Collection*>(0x62bae0) = &locos.c;
        *(int32_t*)(map + 0x2155) = (int32_t)(Rnd() % 40);
        *(int32_t*)(map + 0x2159) = (int32_t)(Rnd() % 40);
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 2);
        int32_t cached = Rnd() % 4 == 0;
        float x = RndF(0.0f, 300.0f), y = RndF(0.0f, 300.0f), w = RndF(0.0f, 300.0f), h = RndF(0.0f, 300.0f);
        uint32_t st[2][11];
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < 11; ++k) hook::Global<uint32_t>(0x6cffac + k * 4) = 0x1234 + k;
            hook::Global<int32_t>(0x6cffb0) = cached;
            logs[side] = t.Run([&] { side ? rt3::EngineStats_Draw(nullptr, 0, x, y, w, h, 0)
                                          : ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x4d0420)(nullptr, 0, x, y, w, h, 0); });
            for (int k = 0; k < 11; ++k) st[side][k] = hook::Global<uint32_t>(0x6cffac + k * 4);
        }
        CompareLogs(logs[0], logs[1], "engine stats", it);
        EXPECT_EQ(memcmp(st[0], st[1], sizeof st[0]), 0, "engine stats globals iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Movie_Stop(uint8_t* w, int, int32_t release);
void __fastcall Engine_SaveConfig(uint8_t* e);
void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall FreeAndNull(void** p);
void __fastcall Picture_FreeTexture(uint8_t* w);
}

namespace {
void __fastcall RecSaveConfig(uint8_t* e) { Logf("save config %p", e); }
void __fastcall RecSpeakers(uint8_t* s, int, int32_t type, int32_t n) { Logf("speakers %p %d %d", s, type, n); }
void __fastcall RecCursorSet(uint8_t* c, int, int32_t idx, int32_t f, int32_t now) { Logf("cursor %p %d %d %d", c, idx, f, now); }
void __fastcall RecFreeAndNull(void** p) { Logf("free and null %p %p", p, *p); *p = nullptr; }
void __fastcall RecFreeTexture(uint8_t* w) { Logf("free texture %p", w); }
void __stdcall RecBinkClose(uint32_t h) { Logf("bink close %x", h); }
} // namespace

DIFFTEST(ui_MovieStop) {
    SavedGlobals sg({ 0xd97980, 0xd9796c, 0x6d4024, 0x6d402c, 0xccbb20 });
    UiTrace t;
    t.Add(0x51eea0, (void*)&rt3::Engine_SaveConfig, (void*)&RecSaveConfig);
    t.Add(0x531750, (void*)&rt3::Sound_Speakers, (void*)&RecSpeakers);
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&RecCursorSet);
    t.Add(0x53afd0, (void*)&rt3::FreeAndNull, (void*)&RecFreeAndNull);
    t.Add(0x55ed40, (void*)&rt3::Picture_FreeTexture, (void*)&RecFreeTexture);
    DWORD prot;
    VirtualProtect((void*)0x5c8380, 4, PAGE_READWRITE, &prot);
    void* savedBink = *(void**)0x5c8380;
    *(void**)0x5c8380 = (void*)&RecBinkClose;
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    static uint8_t w[0x200], sound[0x10];
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint32_t>(0xccbb20) = 0xccbb200;
    for (int it = 0; it < 300; ++it) {
        int32_t release = Rnd() % 2;
        uint32_t f2 = Rnd() % 3 ? 0xb1c0000 + Rnd() % 100 : 0, fe = Rnd() % 2 ? 0x0fe0000 : 0, kind = Rnd() % 8;
        int32_t pending = Rnd() % 2, hasBuf = Rnd() % 2;
        uint8_t* snd = Rnd() % 2 ? sound : nullptr;
        *(int32_t*)(engine + 0x11470a) = (int32_t)(Rnd() % 5);
        std::string logs[2];
        uint8_t out[2][0x200];
        for (int side = 0; side < 2; ++side) {
            memset(w, 0, sizeof w);
            *(uint32_t*)(w + 0xf2) = f2;
            *(uint32_t*)(w + 0xfe) = fe;
            *(uint32_t*)(w + 0xea) = kind;
            hook::Global<int32_t>(0xd97980) = pending;
            hook::Global<int32_t>(0xd9796c) = 7;
            hook::Global<uint8_t*>(0x6d402c) = snd;
            *(int32_t*)(engine + 0x114746) = 5;
            logs[side] = t.Run([&] {
                *(void**)(w + 0xbe) = hasBuf ? ArenaAlloc(16) : nullptr;
                side ? rt3::Movie_Stop(w, 0, release) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x55ed90)(w, 0, release);
            });
            memcpy(out[side], w, sizeof w);
            out[side][0] = (uint8_t)hook::Global<int32_t>(0xd97980);
            out[side][1] = (uint8_t)hook::Global<int32_t>(0xd9796c);
            out[side][2] = (uint8_t)*(int32_t*)(engine + 0x114746);
        }
        CompareLogs(logs[0], logs[1], "movie stop", it);
        EXPECT_EQ(memcmp(out[0] + 3, out[1] + 3, sizeof out[0] - 3) == 0 && out[0][0] == out[1][0] && out[0][1] == out[1][1] && out[0][2] == out[1][2], true,
                  "movie stop state iteration %d", it);
    }
    *(void**)0x5c8380 = savedBink;
    VirtualProtect((void*)0x5c8380, 4, prot, &prot);
}

namespace rt3 {
void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild);
int32_t Global6d3b30();
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall List_RowPos(uint8_t* w, int, int32_t row, float* x, float* y);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);
uint32_t __fastcall Car_SkinTexture(uint8_t* car);
double One();
double __fastcall Slider_StepA(uint8_t* s);
void* __fastcall Obj_City352(void* o);
}

namespace {
int32_t g_trFlag, g_trFound, g_trCars;
uint8_t g_trCar[8][0x400], g_trKind[4][0x80];
int32_t FakeTr6d3b30() { return g_trFlag; }
uint8_t* __fastcall FakeTrFind(uint8_t* w, int, int32_t id) { Logf("find %p %x", w, id); return g_trFound ? (uint8_t*)0x7770000 + id : nullptr; }
void __fastcall FakeTrRowPos(uint8_t* w, int, int32_t row, float* x, float* y) { Logf("rowpos %p %d", w, row); *x = 0.125f * row; *y = 0.0625f * row + 0.01f; }
char* __fastcall FakeTrName(uint8_t* t, int, int32_t n) { Logf("train name %p %d", t, n); return (char*)"Train 7"; }
double __fastcall FakeTrTextH(uint8_t* f, int, const char* text, float w, float* tw) { Logf("texth %p %s %08x", f, text, B(w)); *tw = 0.3125f; return 9.0; }
void __fastcall RecTrDelete(uint8_t* w, int, int32_t lo, int32_t hi, uint32_t p) { Logf("delete %p %x %x %x", w, lo, hi, p); }
void* __fastcall FakeTrFirst(void* o) { return g_trCars ? g_trCar[0] : nullptr; }
void* __fastcall FakeTrNext(void* o) { int i = (int)(((uint8_t*)o - g_trCar[0]) / 0x400) + 1; return i < g_trCars ? g_trCar[i] : nullptr; }
void* __fastcall FakeTrKind(void* o) { return g_trKind[((uint8_t*)o)[0x10] % 4]; }
uint32_t __fastcall FakeTrSkin(uint8_t* c) { return 0x5100000 + c[0x11]; }
double FakeTrOne() { return 1.0; }
double __fastcall FakeTrStep(uint8_t* s) { return (double)(((uintptr_t)s & 0xff) % 7) * 0.0123; }
} // namespace

DIFFTEST(ui_TrainRow) {
    SavedGlobals sg({ 0x6cfcbc, 0x6cec78, 0x6cec7c });
    UiTrace t;
    t.Add(0x5157e0, (void*)&rt3::Global6d3b30, (void*)&FakeTr6d3b30);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeTrFind);
    t.Add(0x560350, (void*)&rt3::List_RowPos, (void*)&FakeTrRowPos);
    t.Add(0x4a9360, (void*)&rt3::Train_DisplayName, (void*)&FakeTrName);
    t.Add(0x550b50, (void*)&rt3::Font_TextHeight, (void*)&FakeTrTextH);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecTrDelete);
    t.Add(0x4a77b0, (void*)&rt3::Obj_City41, (void*)&FakeTrFirst);
    t.Add(0x41ad90, (void*)&rt3::Obj_City352, (void*)&FakeTrNext);
    t.Add(0x41adb0, (void*)&rt3::Obj_Ref388, (void*)&FakeTrKind);
    t.Add(0x41c060, (void*)&rt3::Car_SkinTexture, (void*)&FakeTrSkin);
    t.Add(0x41d540, (void*)&rt3::One, (void*)&FakeTrOne);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeTrStep);
    static uint8_t world[0x46b00], newGame[0x100], win[0x200];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    *(uint32_t*)(world + 0x46a98) = 0x46a9800;
    for (int k = 0; k < 4; ++k) *(int32_t*)(g_trKind[k] + 0x41) = k;
    for (int it = 0; it < 500; ++it) {
        FakeTable trains(1 + Rnd() % 3, 0x400, [](uint8_t* r, int) {
            *(int32_t*)(r + 0xd8) = (int32_t)(Rnd() % 5);
            r[0x1a1] = Rnd() % 2;
            r[0x1ce] = (uint8_t)Rnd();
        });
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        g_trFlag = Rnd() % 2;
        g_trFound = Rnd() % 2;
        g_trCars = (int32_t)(Rnd() % 9);
        for (int k = 0; k < 8; ++k) for (int j = 0x10; j < 0x12; ++j) g_trCar[k][j] = (uint8_t)Rnd();
        newGame[0x82] = Rnd() % 4 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 4 == 0;
        uint32_t id = (uint32_t)(Rnd() % 5);
        int32_t compact = Rnd() % 2, slot = (int32_t)(Rnd() % 8), rebuild = Rnd() % 2;
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, uint32_t, int32_t, int32_t, int32_t))0x4bf320)(win, id, compact, slot, rebuild); });
        std::string b = t.Run([&] { rt3::TrainRow_Build(win, id, compact, slot, rebuild); });
        CompareLogs(a, b, "train row", it);
    }
}

namespace rt3 {
void __fastcall CargoRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey);
int32_t __fastcall Obj_YearReached(void* o);
double __fastcall Cargo_DemandWeight(void* c);
}

namespace {
int32_t __fastcall FakeYearReached(void* o) { return ((uint8_t*)o)[0x70] % 5 != 0; }
double __fastcall FakeDemand(void* c) { Logf("demand %p", c); return (double)(((uint8_t*)c)[0x71] * 37 % 500) * 1.5; }
} // namespace

DIFFTEST(ui_CargoRanking) {
    SavedGlobals sg({ 0x62ba8c, 0x6d3c84 });
    UiTrace t;
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x41e220, (void*)&rt3::Obj_YearReached, (void*)&FakeYearReached);
    t.Add(0x41e650, (void*)&rt3::Cargo_DemandWeight, (void*)&FakeDemand);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    static uint8_t panel[0x1000];
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    for (int k = 0; k < 40; ++k) *(uint32_t*)(panel + 0xb35 + k * 4) = 0xb350000 + k * 0x100;
    for (int it = 0; it < 400; ++it) {
        FakeTable cargo(Rnd() % 8 == 0 ? 0 : 1 + Rnd() % 35, 0x100, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Cargo %d", i);
            r[0x46] = Rnd() % 4 == 0;
            *(float*)(r + 0x5e) = RndF(0.0f, 5000.0f);
            *(float*)(r + 0x62) = RndF(0.0f, 5000.0f);
            *(float*)(r + 0x6a) = RndF(0.0f, 50000.0f);
            r[0x70] = (uint8_t)Rnd();
            r[0x71] = (uint8_t)Rnd();
        });
        hook::Global<Collection*>(0x62ba8c) = &cargo.c;
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 4);
        panel[0x760] = Rnd() % 2;
        *(int32_t*)(panel + 0x7c) = (int32_t)(Rnd() % 3);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f);
        int32_t key = (int32_t)(Rnd() % 6);
        uint8_t out[2][0x640];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e9460)(panel, 0, x, y, w, 0, key); });
        memcpy(out[0], panel + 0x11c, 0x640);
        std::string b = t.Run([&] { rt3::CargoRanking_Draw(panel, 0, x, y, w, 0, key); });
        memcpy(out[1], panel + 0x11c, 0x640);
        CompareLogs(a, b, "cargo ranking", it);
        EXPECT_EQ(memcmp(out[0], out[1], 0x640), 0, "cargo ranking slots iteration %d", it);
    }
}

namespace rt3 {
void __fastcall PlayerFinance_Draw(void*, int, float x, float y, float w, float, int32_t);
}

namespace {
int32_t g_pfSelf;
void* __fastcall FakePfRef25(void* o) {
    Collection* t = hook::Global<Collection*>(0x6ceb9c);
    return g_pfSelf < rt3::Table_CountField4(t) ? rt3::Table_NthField4(t, 0, g_pfSelf) : nullptr;
}
} // namespace

DIFFTEST(ui_PlayerFinance) {
    SavedGlobals sg({ 0x6ceb9c, 0x6cec78, 0x6cec74, 0x6d19f4, 0x6d19f0, 0x6d19e8, 0x6d19d8, 0x6d19dc });
    UiTrace t;
    t.Add(0x476320, (void*)&rt3::Player_HoldingsValue, (void*)&FakeHoldings2);
    t.Add(0x476780, (void*)&rt3::Player_NetWorth, (void*)&FakeNetWorth);
    t.Add(0x476c20, (void*)&rt3::Player_CollateralWorth, (void*)&FakeCollateral);
    t.Add(0x476bb0, (void*)&rt3::Obj_NameIndex, (void*)&FakeRankNameIndex);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeTrFind);
    t.Add(0x4348c0, (void*)&rt3::Obj_Ref25, (void*)&FakePfRef25);
    static uint8_t world[0x100], opts[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint32_t>(0x6d19f0) = 0x6d19f00;
    hook::Global<uint32_t>(0x6d19e8) = 0x6d19e80;
    hook::Global<uint32_t>(0x6d19d8) = 0x6d19d80;
    hook::Global<uint32_t>(0x6d19dc) = 0x6d19dc0;
    for (int it = 0; it < 400; ++it) {
        FakeTable pls(1 + Rnd() % 10, 0x200, [](uint8_t* r, int i) {
            *(int32_t*)(r + 4) = Rnd() % 6 != 0;
            sprintf((char*)r + 8, "%cPl %d", 'A' + (int)(Rnd() % 26), i);
            *(double*)(r + 0x154) = (double)((int)(Rnd() % 100000) - 20000) * 0.5;
            r[0x15c] = Rnd() % 3 == 0 ? 0x29 : (uint8_t)(Rnd() % 50);
        }, 1);
        hook::Global<Collection*>(0x6ceb9c) = &pls.c;
        int32_t n = rt3::Table_CountField4(&pls.c);
        hook::Global<int32_t>(0x6d19f4) = n ? (int32_t)(Rnd() % n) : 0;
        if (!n) continue;
        g_pfSelf = (int32_t)(Rnd() % 12);
        g_trFound = Rnd() % 3 == 0;
        *(int32_t*)(opts + 0x68) = Rnd() % 2;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.1f, 1.0f);
        uint8_t faces0[32];
        for (int k = 0; k < (int)pls.c.count && k < 32; ++k) faces0[k] = ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c];
        std::string a = t.Run([&] { ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x50a730)(nullptr, 0, x, y, w, 0, 0); });
        for (int k = 0; k < (int)pls.c.count && k < 32; ++k) ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c] = faces0[k];
        std::string b = t.Run([&] { rt3::PlayerFinance_Draw(nullptr, 0, x, y, w, 0, 0); });
        CompareLogs(a, b, "player finance", it);
    }
}

namespace rt3 {
int32_t __fastcall Finance_SyncMessage(uint8_t* msg, int32_t mode, void** out);
}

DIFFTEST(net_FinanceSync) {
    SavedGlobals sg({ 0x62be10, 0x6ceb9c, 0x6cec78 });
    UiTrace t;
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x42a040, (void*)&rt3::Company_AddStat, (void*)&RecAddStat);
    static uint8_t world[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    for (int it = 0; it < 400; ++it) {
        int nc = (int)(Rnd() % 8), np = (int)(Rnd() % 8);
        FakeTable cos(nc, 0x1400, [](uint8_t* r, int) { *(int32_t*)(r + 0x47) = (int32_t)(Rnd() % 100000); });
        FakeTable pls(np, 0x200, [](uint8_t* r, int) {
            *(double*)(r + 0x154) = (double)((int)(Rnd() % 100000) - 2000) * 0.25;
            for (int k = 0; k < 10; ++k) *(int32_t*)(r + 0x15d + k * 4) = (int32_t)(Rnd() % 5000);
        });
        bool nullCo = Rnd() % 10 == 0, nullPl = Rnd() % 10 == 0;
        hook::Global<Collection*>(0x62be10) = nullCo ? nullptr : &cos.c;
        hook::Global<Collection*>(0x6ceb9c) = nullPl ? nullptr : &pls.c;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1900 + Rnd() % 20);
        int32_t mode = Rnd() % 2 ? 1 : (int32_t)(Rnd() % 3) * 2;
        static uint8_t msg[2][0x1000], start[0x1000];
        for (uint32_t k = 0; k < sizeof start; ++k) start[k] = (uint8_t)Rnd();
        start[0xd] = (uint8_t)(Rnd() % 3 ? nc : 0);
        start[0xe] = (uint8_t)(Rnd() % 3 ? np : 0);
        for (int k = 0; k < 0x200; k += 4) if (Rnd() % 3 == 0) *(float*)(start + 0x10 + k) = 0.0f;
        static uint8_t coSave[8][0x1400], plSave[8][0x200], coOut[2][8][0x1400], plOut[2][8][0x200];
        for (int k = 0; k < nc; ++k) memcpy(coSave[k], rt3::Collection_Get(&cos.c, 0, rt3::Collection_IdAt(&cos.c, 0, k, 0)), 0x1400);
        for (int k = 0; k < np; ++k) memcpy(plSave[k], rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)), 0x200);
        std::string logs[2];
        int32_t ret[2];
        std::string bufs[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < nc; ++k) memcpy(rt3::Collection_Get(&cos.c, 0, rt3::Collection_IdAt(&cos.c, 0, k, 0)), coSave[k], 0x1400);
            for (int k = 0; k < np; ++k) memcpy(rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)), plSave[k], 0x200);
            memcpy(msg[side], start, sizeof start);
            void* out = nullptr;
            logs[side] = t.Run([&] { ret[side] = side ? rt3::Finance_SyncMessage(msg[side], mode, &out)
                                                      : ((int32_t(__fastcall*)(uint8_t*, int32_t, void**))0x46e5c0)(msg[side], mode, &out); });
            if (mode == 1 && out) {
                bufs[side].assign((char*)out, ret[side]);
                ((void(__cdecl*)(void*))0x5a1145)(out);
            }
            for (int k = 0; k < nc; ++k) memcpy(coOut[side][k], rt3::Collection_Get(&cos.c, 0, rt3::Collection_IdAt(&cos.c, 0, k, 0)), 0x1400);
            for (int k = 0; k < np; ++k) memcpy(plOut[side][k], rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)), 0x200);
        }
        CompareLogs(logs[0], logs[1], "finance sync", it);
        EXPECT_EQ(ret[0], ret[1], "finance sync ret iteration %d", it);
        EXPECT_EQ(bufs[0] == bufs[1], true, "finance sync buffer iteration %d", it);
        EXPECT_EQ(memcmp(msg[0], msg[1], sizeof start), 0, "finance sync msg iteration %d", it);
        EXPECT_EQ(memcmp(coOut[0], coOut[1], sizeof coOut[0]) == 0 && memcmp(plOut[0], plOut[1], sizeof plOut[0]) == 0, true, "finance sync objects iteration %d", it);
    }
}

namespace rt3 {
int32_t __fastcall Track_InfoText(uint8_t* t, int, char* dst, int32_t n, uint32_t* color, float* offset, char* dst2, int32_t extra);
int32_t __fastcall Routes_Component(Collection* c, int, int32_t id);
int32_t __fastcall Camera_FlagA4(uint8_t* o, int, int32_t);
int32_t __fastcall Camera_FlagAc(uint8_t* o, int, int32_t);
int32_t __fastcall Camera_FlagA0(uint8_t* o, int, int32_t);
int32_t __fastcall Camera_FlagA8(uint8_t* o, int, int32_t);
int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t);
float __fastcall Vehicle_Tilt(uint8_t* o, int, int32_t absolute);
}

namespace {
int32_t g_camFlags;
float g_tilt;
int32_t __fastcall FakeRouteComp(Collection* c, int, int32_t id) { Logf("component %p %d", c, id); return id * 3 + 1; }
int32_t __fastcall FakeFlagA4(uint8_t* o, int, int32_t a) { Logf("a4 %p %d", o, a); return g_camFlags & 1; }
int32_t __fastcall FakeFlagAc(uint8_t* o, int, int32_t a) { Logf("ac %p %d", o, a); return (g_camFlags >> 1) & 1; }
int32_t __fastcall FakeFlagA0(uint8_t* o, int, int32_t a) { Logf("a0 %p %d", o, a); return (g_camFlags >> 2) & 1; }
int32_t __fastcall FakeFlagA8(uint8_t* o, int, int32_t a) { Logf("a8 %p %d", o, a); return (g_camFlags >> 3) & 1; }
int32_t __fastcall FakeDebugTip(uint8_t* t, int, char* d, int32_t n, int32_t a, int32_t b, int32_t c, int32_t e) {
    Logf("debug tip %p %d %x %x %x %d", t, n, a, b, c, e);
    return 7;
}
float __fastcall FakeTilt(uint8_t* o, int, int32_t a) { Logf("tilt %p %d", o, a); return g_tilt; }
} // namespace

DIFFTEST(ui_TrackInfoText) {
    SavedGlobals sg({ 0x6cec74, 0x6cfca8, 0x6cfcb4, 0x6d4024 });
    static uint8_t* engine = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d4024) = engine;
    UiTrace t;
    t.Add(0x4a6320, (void*)&rt3::Routes_Component, (void*)&FakeRouteComp);
    t.Add(0x4934e0, (void*)&rt3::Camera_FlagA4, (void*)&FakeFlagA4);
    t.Add(0x493560, (void*)&rt3::Camera_FlagAc, (void*)&FakeFlagAc);
    t.Add(0x4934a0, (void*)&rt3::Camera_FlagA0, (void*)&FakeFlagA0);
    t.Add(0x493520, (void*)&rt3::Camera_FlagA8, (void*)&FakeFlagA8);
    t.Add(0x53a450, (void*)&rt3::Track_DebugTooltip, (void*)&FakeDebugTip);
    t.Add(0x48a790, (void*)&rt3::Vehicle_Tilt, (void*)&FakeTilt);
    static uint8_t opts[0x300], track[0x300];
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint32_t>(0x6cfca8) = 0x6cfca80;
    hook::Global<uint32_t>(0x6cfcb4) = 0x6cfcb40;
    static const uint32_t flags[] = { 0x80, 0x293, 0x2b3, 0x27f, 0x2af, 0x84, 0x88 };
    for (int it = 0; it < 1500; ++it) {
        memset(opts, 0, sizeof opts);
        int which = (int)(Rnd() % 9);
        if (which < 7) *(int32_t*)(opts + flags[which]) = 1;
        for (uint32_t o = 0x200; o < 0x240; o += 4) *(int32_t*)(track + o) = Rnd() % 3 == 0 ? -1 : (int32_t)(Rnd() % 100000) - (Rnd() % 4 == 0 ? 50000 : 0);
        track[0x216] = Rnd() % 2;
        track[0x219] = (uint8_t)Rnd();
        g_camFlags = (int32_t)(Rnd() % 16);
        g_tilt = RndF(-0.2f, 0.2f);
        int32_t n = 4 + (int32_t)(Rnd() % 40);
        std::string logs[2], outs[2];
        int32_t ret[2];
        for (int side = 0; side < 2; ++side) {
            char dst[0x100], dst2[0x100];
            uint32_t color = 0x1234;
            float off = 3.0f;
            memset(dst, 0x41, sizeof dst);
            memset(dst2, 0x42, sizeof dst2);
            logs[side] = t.Run([&] { ret[side] = side ? rt3::Track_InfoText(track, 0, dst, n, &color, &off, dst2, 9)
                                                      : ((int32_t(__fastcall*)(uint8_t*, int, char*, int32_t, uint32_t*, float*, char*, int32_t))0x48d540)(track, 0, dst, n, &color, &off, dst2, 9); });
            outs[side].assign(dst, sizeof dst);
            outs[side].append(dst2, sizeof dst2);
            outs[side].append((char*)&color, 4);
            outs[side].append((char*)&off, 4);
        }
        CompareLogs(logs[0], logs[1], "track info", it);
        EXPECT_EQ(ret[0], ret[1], "track info ret iteration %d", it);
        EXPECT_EQ(outs[0] == outs[1], true, "track info output iteration %d", it);
    }
}

namespace rt3 {
void __fastcall PlayerDetail_Draw(uint8_t* w, int, float x, float y, float, float);
double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
double __fastcall Slider_StepB(uint8_t* s);
}

namespace {
double __fastcall FakeWealth(uint8_t* p, int, int32_t k) { Logf("wealth %p %d", p, k); return PlayerFig(p, 0); }
double __fastcall FakePdStepB(uint8_t* s) { return (double)(((uintptr_t)s >> 4) % 9) * 0.031; }
} // namespace

DIFFTEST(ui_PlayerDetail) {
    SavedGlobals sg({ 0x6ceb9c, 0x62be10, 0x6cec78 });
    UiTrace t;
    t.Add(0x476bb0, (void*)&rt3::Obj_NameIndex, (void*)&FakeRankNameIndex);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x550b50, (void*)&rt3::Font_TextHeight, (void*)&FakeTrTextH);
    t.Add(0x4348c0, (void*)&rt3::Obj_Ref25, (void*)&FakePfRef25);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeTrStep);
    t.Add(0x5521a0, (void*)&rt3::Slider_StepB, (void*)&FakePdStepB);
    t.Add(0x4767a0, (void*)&rt3::Player_Wealth, (void*)&FakeWealth);
    t.Add(0x476c20, (void*)&rt3::Player_CollateralWorth, (void*)&FakeCollateral);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    static uint8_t world[0x100], panel[0x1000];
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    for (int k = 0; k < 0x40; ++k) *(uint32_t*)(panel + 0x761 + k * 4) = 0x7610000 + k * 0x100;
    for (int it = 0; it < 400; ++it) {
        FakeTable pls(1 + Rnd() % 4, 0x200, [](uint8_t* r, int i) {
            *(int32_t*)(r + 4) = 1;
            sprintf((char*)r + 8, "Pl %d", i);
            sprintf((char*)r + 0x27, "Title %d", i);
            *(double*)(r + 0x154) = (double)((int)(Rnd() % 100000) - 2000) * 0.25;
            for (int k = 0; k < 8; ++k) *(int32_t*)(r + 0x15d + k * 4) = Rnd() % 3 ? 0 : (int32_t)(Rnd() % 5000);
            r[0x15c] = Rnd() % 3 == 0 ? 0x29 : (uint8_t)(Rnd() % 50);
        }, 1);
        FakeTable cos(Rnd() % 6, 0x100, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x3f) = Rnd() % 4 != 0;
            sprintf((char*)r + 4, "Co %d", i);
        });
        hook::Global<Collection*>(0x6ceb9c) = &pls.c;
        hook::Global<Collection*>(0x62be10) = &cos.c;
        *(uint32_t*)(panel + 0x8c) = rt3::Collection_IdAt(&pls.c, 0, Rnd() % pls.c.count, 0);
        g_pfSelf = (int32_t)(Rnd() % 5);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        uint8_t faces[8];
        for (int k = 0; k < (int)pls.c.count; ++k) faces[k] = ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4e5300)(panel, 0, x, y, 0, 0); });
        for (int k = 0; k < (int)pls.c.count; ++k) ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c] = faces[k];
        std::string b = t.Run([&] { rt3::PlayerDetail_Draw(panel, 0, x, y, 0, 0); });
        CompareLogs(a, b, "player detail", it);
    }
}

namespace rt3 {
void __fastcall MapSizePanel_Build(uint8_t* w, int, float x, float y, float width, float);
void MapSizeDialog_Show();
}

namespace {
void RecMapSizeShow() { Logf("map size show"); }
} // namespace

DIFFTEST(ui_MapSizePanel) {
    SavedGlobals sg({ 0x6d10bc, 0x6d10c0, 0x6d10c4, 0x6d10c8 });
    UiTrace t;
    t.Add(0x4ea780, (void*)&rt3::MapSizeDialog_Show, (void*)&RecMapSizeShow);
    static uint8_t win[0x200];
    for (int it = 0; it < 300; ++it) {
        hook::Global<int32_t>(0x6d10c0) = Rnd() % 2;
        hook::Global<int32_t>(0x6d10c4) = 32 + (int32_t)(Rnd() % 600);
        hook::Global<int32_t>(0x6d10c8) = 32 + (int32_t)(Rnd() % 600);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f);
        uint32_t g[2];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4ea940)(win, 0, x, y, w, 0); });
        g[0] = hook::Global<uint32_t>(0x6d10bc);
        hook::Global<uint32_t>(0x6d10bc) = 0;
        std::string b = t.Run([&] { rt3::MapSizePanel_Build(win, 0, x, y, w, 0); });
        g[1] = hook::Global<uint32_t>(0x6d10bc);
        CompareLogs(a, b, "map size panel", it);
        EXPECT_EQ(g[0], g[1], "map size panel global iteration %d", it);
    }
}

namespace rt3 {
void __fastcall CompanyWindow_Refresh(uint8_t* win);
void* SelectedCompany();
uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o);
void* __fastcall Obj_Company21(void* o);
}

namespace {
uint8_t* g_cwCo;
void* g_cwOwn;
uint8_t g_cwWidget[0x200];
void* FakeSelCompany() { Logf("selected company"); return g_cwCo; }
uint8_t* __fastcall FakeCwNamed20(uint8_t* o) { Logf("named20 %p", o); return (uint8_t*)"Face_Custom"; }
void* __fastcall FakeCwCompany21(void* o) { return g_cwOwn; }
uint8_t* __fastcall FakeCwFind(uint8_t* w, int, int32_t id) { Logf("find %p %x", w, id); return g_cwWidget; }
} // namespace

DIFFTEST(ui_CompanyWindowRefresh) {
    SavedGlobals sg({ 0x6d0818, 0x62be10, 0x6ceb9c, 0x6cfc9c, 0x6cec78, 0x6cec74, 0x6cfe60, 0x6d4020 });
    UiTrace t;
    t.Add(0x4c16f0, (void*)&rt3::SelectedCompany, (void*)&FakeSelCompany);
    t.Add(0x476b30, (void*)&rt3::Obj_NamedEntry20, (void*)&FakeCwNamed20);
    t.Add(0x434870, (void*)&rt3::Obj_Company21, (void*)&FakeCwCompany21);
    t.Add(0x4337b0, (void*)&rt3::Obj_Get25, (void*)&FakeRankGet25);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeContainsName);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeCwFind);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecTrDelete);
    static uint8_t world[0x4b00], opts[0x100], view[0x100], win[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint32_t>(0x6d4020) = 0x6d40200;
    for (int it = 0; it < 500; ++it) {
        FakeTable cos(1 + Rnd() % 3, 0x100, [](uint8_t* r, int) {
            *(int32_t*)(r + 0x3b) = Rnd() % 4 ? 1 + (int32_t)(Rnd() % 3) : 0;
            *(int32_t*)(r + 0x5b) = (int32_t)(Rnd() % 4);
        });
        FakeTable pls(3, 0x200, [](uint8_t* r, int i) {
            sprintf((char*)r + 8, "Pl %d", i);
            r[0x15c] = Rnd() % 3 == 0 ? 0x29 : (uint8_t)(Rnd() % 4);
        });
        FakeTable sts(Rnd() % 6, 0x80, [](uint8_t* r, int) {
            *(int32_t*)(r + 0x31) = Rnd() % 3 != 0;
            *(int32_t*)(r + 0x3d) = Rnd() % 3 != 0;
        });
        hook::Global<Collection*>(0x62be10) = &cos.c;
        hook::Global<Collection*>(0x6ceb9c) = &pls.c;
        hook::Global<Collection*>(0x6cfc9c) = &sts.c;
        hook::Global<uint8_t*>(0x6d0818) = Rnd() % 10 ? view : nullptr;
        *(int32_t*)(view + 0x90) = (int32_t)(Rnd() % 5) - 1;
        g_cwCo = (uint8_t*)rt3::Collection_Get(&cos.c, 0, rt3::Collection_IdAt(&cos.c, 0, Rnd() % cos.c.count, 0));
        g_cwOwn = Rnd() % 2 ? (void*)g_cwCo : nullptr;
        g_rankGet25 = (int32_t)(Rnd() % 4);
        hook::Global<int32_t>(0x6cfe60) = (int32_t)(Rnd() % 5);
        for (uint32_t o = 0x4a87; o <= 0x4adb; o += 4) *(int32_t*)(world + o) = Rnd() % 4 == 0;
        *(int32_t*)(opts + 0x68) = Rnd() % 2;
        uint8_t faces[3];
        for (int k = 0; k < 3; ++k) faces[k] = ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*))0x4c2ca0)(win); });
        std::string wa((char*)g_cwWidget, sizeof g_cwWidget);
        memset(g_cwWidget, 0, sizeof g_cwWidget);
        for (int k = 0; k < 3; ++k) ((uint8_t*)rt3::Collection_Get(&pls.c, 0, rt3::Collection_IdAt(&pls.c, 0, k, 0)))[0x15c] = faces[k];
        std::string b = t.Run([&] { rt3::CompanyWindow_Refresh(win); });
        std::string wb((char*)g_cwWidget, sizeof g_cwWidget);
        memset(g_cwWidget, 0, sizeof g_cwWidget);
        CompareLogs(a, b, "company window refresh", it);
        EXPECT_EQ(wa == wb, true, "company window widget iteration %d", it);
    }
}

namespace rt3 {
void __fastcall IndustryFinance_Draw(uint8_t* ind, int, float x, float y, float w, float h, int32_t);
int32_t __fastcall Obj_StateB9(void* o);
int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i);
void* __fastcall Obj_Ref3cc(void* o);
void* __fastcall Obj_Building(void* o);
double __fastcall Obj_Float310(void* o, int, int32_t i);
double __fastcall Obj_Float360(void* o, int, int32_t i);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
}

namespace {
int32_t g_ifKind;
uint8_t g_ifPairs[16][8], g_ifType[0x100], g_ifQ[0x100];
struct IfBuilding { void** vtbl; } g_ifBuilding;
void* g_ifVtbl[0x40];
int32_t __fastcall FakeIfKind(void* o) { return g_ifKind; }
int32_t __fastcall FakeIfCount(void* o, int, int32_t i) { Logf("count590 %p %d", o, i); return ((uint8_t*)o)[0x10] % 4; }
uint8_t* __fastcall FakeIfPair(uint8_t* o, int, int32_t list, int32_t i) { Logf("pair %p %d %d", o, list, i); return g_ifPairs[(o[0x11] + i) % 16]; }
void* __fastcall FakeIfType(void* o) { Logf("type %p", o); return g_ifType; }
void* __fastcall FakeIfBuilding(void* o) { Logf("building %p", o); return &g_ifBuilding; }
uint8_t* __fastcall FakeIfVcall(void* o) { Logf("vcall %p", o); return g_ifQ; }
double __fastcall FakeIf310(void* o, int, int32_t i) { Logf("f310 %d", i); return (double)((i * 7919 + 13) % 4001) * 37.5 - 20000.0; }
double __fastcall FakeIf360(void* o, int, int32_t i) { Logf("f360 %d", i); return (double)((i * 31 + 7) % 2001) * 11.0; }
double __fastcall FakeIfRem(void* o, int, int32_t i) { Logf("rem %d", i); return (double)((i * 17 + 3) % 901) * 3.5 - 900.0; }
} // namespace

DIFFTEST(ui_IndustryFinance) {
    SavedGlobals sg({ 0x6d0818, 0x62b26c, 0x6cec20, 0x6cec78, 0x6cfd08, 0x6cfd04, 0x6cfd18, 0x6cfcf8, 0x6cfcfc });
    static uint32_t savedIds[10];
    memcpy(savedIds, (void*)0x6cfccc, sizeof savedIds);
    UiTrace t;
    t.Add(0x40d350, (void*)&rt3::Obj_StateB9, (void*)&FakeIfKind);
    t.Add(0x47dc90, (void*)&rt3::Obj_Array590, (void*)&FakeIfCount);
    t.Add(0x47dcb0, (void*)&rt3::Pairs_At, (void*)&FakeIfPair);
    t.Add(0x40cec0, (void*)&rt3::Obj_Ref3cc, (void*)&FakeIfType);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeIfBuilding);
    t.Add(0x40ca90, (void*)&rt3::Obj_Float310, (void*)&FakeIf310);
    t.Add(0x40cab0, (void*)&rt3::Obj_Float360, (void*)&FakeIf360);
    t.Add(0x40cac0, (void*)&rt3::Obj_Remaining, (void*)&FakeIfRem);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeTrStep);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecTrDelete);
    g_ifVtbl[0x80 / 4] = (void*)&FakeIfVcall;
    g_ifBuilding.vtbl = g_ifVtbl;
    static uint8_t world[0x46b00], view[0x100], ind[0x400];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d0818) = view;
    *(uint32_t*)(world + 0x46a9c) = 0x46a9c00;
    hook::Global<uint32_t>(0x6cfd04) = 0x6cfd040;
    hook::Global<uint32_t>(0x6cfd18) = 0x6cfd180;
    hook::Global<uint32_t>(0x6cfcf8) = 0x6cfcf80;
    hook::Global<uint32_t>(0x6cfcfc) = 0x6cfcfc0;
    for (int it = 0; it < 400; ++it) {
        FakeTable inds(2, 0x100, [](uint8_t*, int) {});
        FakeTable sts(Rnd() % 9, 0x500, [](uint8_t* r, int i) {
            *(uint32_t*)(r + 4) = 0;
            r[0x10] = (uint8_t)Rnd();
            r[0x11] = (uint8_t)Rnd();
            sprintf((char*)r + 0x46b, "Station %d", i);
        });
        hook::Global<Collection*>(0x62b26c) = &inds.c;
        hook::Global<Collection*>(0x6cec20) = &sts.c;
        *(uint32_t*)(view + 0x90) = Rnd() % 8 ? rt3::Collection_IdAt(&inds.c, 0, 0, 0) : 77;
        for (int k = 0; k < 16; ++k) *(int32_t*)g_ifPairs[k] = (int32_t)(Rnd() % 4);
        *(int32_t*)(ind + 0x2a4) = (int32_t)(Rnd() % 4);
        *(uint32_t*)(g_ifType + 0xc3) = Rnd() % 8 == 0 ? 0 : (uint32_t)Rnd() & 0x3f;
        g_ifQ[0x8c] = (uint8_t)(Rnd() % 3);
        g_ifKind = (int32_t)(Rnd() % 6);
        *(uint16_t*)(world + 0xd) = 1900;
        *(int32_t*)(ind + 0x3d5) = 1900 - (int32_t)(Rnd() % 6) + (g_ifKind == 5 ? 0 : (int32_t)(Rnd() % 3));
        for (uint32_t o = 0x2b4; o <= 0x2bc; o += 4) *(float*)(ind + o) = RndF(-5000.0f, 5000.0f);
        uint8_t done = Rnd() % 3 == 0;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        uint32_t ids[2][10];
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<uint8_t>(0x6cfd08) = done;
            memset((void*)0x6cfccc, 0, 40);
            logs[side] = t.Run([&] { side ? rt3::IndustryFinance_Draw(ind, 0, x, y, w, h, 0)
                                          : ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4b6090)(ind, 0, x, y, w, h, 0); });
            memcpy(ids[side], (void*)0x6cfccc, 40);
        }
        CompareLogs(logs[0], logs[1], "industry finance", it);
        EXPECT_EQ(memcmp(ids[0], ids[1], 40), 0, "industry finance ids iteration %d", it);
    }
    memcpy((void*)0x6cfccc, savedIds, sizeof savedIds);
}

namespace rt3 {
int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall NameTable_PushPos(uint8_t* t);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall NameTable_PopPosition(uint8_t* t);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Widget_SetState(uint8_t* w, int, uint32_t mask);
void __fastcall Widget_ClearState(uint8_t* w, int, uint32_t mask);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Widget_SetFont(uint8_t* w, int, int32_t font, int32_t temporary);
}

namespace {
int32_t g_weEnabled, g_weHit;
void* g_weVtbl[0x20];
void __fastcall RecWePackRelease(uint8_t* p, int, void* o) { Logf("pack release %p %p", p, o); }
void __fastcall RecWePushPos(uint8_t* t) { Logf("push pos %p", t); }
uint8_t* __fastcall FakeWeAcquire(uint8_t* p, int, const char* n) {
    Logf("acquire %p \"%s\"", p, n);
    return (uint8_t*)0x9200000 + strlen(n);
}
void __fastcall RecWePopPos(uint8_t* t) { Logf("pop pos %p", t); }
uint32_t __fastcall RecWeSetFlags(uint8_t* w, int, uint32_t m, uint32_t a, uint32_t b) {
    Logf("set flags %x %x %x", m, a, b);
    return 0;
}
void __fastcall RecWeClearFlags(uint8_t* w, int, uint32_t m) { Logf("clear flags %x", m); }
void __fastcall RecWeSetState(uint8_t* w, int, uint32_t m) { Logf("set state %x", m); }
void __fastcall RecWeClearState(uint8_t* w, int, uint32_t m) { Logf("clear state %x", m); }
void __fastcall RecWeSound(int32_t id, int, uint32_t pan) { Logf("sound %d %08x", id, pan); }
int32_t __fastcall RecWeTooltip(uint8_t* ui, int, const char* text, int32_t a, int32_t b, int32_t c) {
    Logf("tooltip %p %s %d %d %d", ui, text ? text : "(null)", a, b, c);
    return 0;
}
void __fastcall RecWeSetFont(uint8_t* w, int, int32_t f, int32_t t) { Logf("set font %d %d", f, t); }
int32_t __fastcall WeEnabled(uint8_t* w) {
    Logf("enabled");
    return g_weEnabled;
}
int32_t __fastcall WeHit(uint8_t* w, int, uint32_t x, uint32_t y, int32_t inside) {
    Logf("hit %08x %08x %d", x, y, inside);
    return g_weHit;
}
void __fastcall WeRedraw(uint8_t* w) { Logf("redraw"); }
void __fastcall WeDrag(uint8_t* w, int, uint32_t x, uint32_t y, int32_t a, int32_t b) { Logf("drag %08x %08x %d %d", x, y, a, b); }
void __fastcall WeClick(uint32_t id, uint32_t self, float x, float y) { Logf("click %x %08x %08x", id, B(x), B(y)); }
} // namespace

DIFFTEST(ui_WidgetHandleEvent) {
    SavedGlobals sg({ 0x6d4020, 0x6d401c });
    UiTrace t;
    t.Add(0x53c000, (void*)&rt3::Pack_Release, (void*)&RecWePackRelease);
    t.Add(0x53ca20, (void*)&rt3::NameTable_PushPos, (void*)&RecWePushPos);
    t.Add(0x53d110, (void*)&rt3::Pack_AcquireNamed, (void*)&FakeWeAcquire);
    t.Add(0x53ca90, (void*)&rt3::NameTable_PopPosition, (void*)&RecWePopPos);
    t.Add(0x558980, (void*)&rt3::Widget_SetFlags, (void*)&RecWeSetFlags);
    t.Add(0x5588b0, (void*)&rt3::Widget_ClearFlags, (void*)&RecWeClearFlags);
    t.Add(0x558470, (void*)&rt3::Widget_SetState, (void*)&RecWeSetState);
    t.Add(0x5584a0, (void*)&rt3::Widget_ClearState, (void*)&RecWeClearState);
    t.Add(0x45ea20, (void*)&rt3::Ui_PlaySound, (void*)&RecWeSound);
    t.Add(0x538c70, (void*)&rt3::Ui_ShowTooltip, (void*)&RecWeTooltip);
    t.Add(0x558180, (void*)&rt3::Widget_SetFont, (void*)&RecWeSetFont);
    g_weVtbl[0xc / 4] = (void*)&WeDrag;
    g_weVtbl[0x20 / 4] = (void*)&WeHit;
    g_weVtbl[0x2c / 4] = (void*)&WeEnabled;
    g_weVtbl[0x30 / 4] = (void*)&WeEnabled;
    g_weVtbl[0x34 / 4] = (void*)&WeRedraw;
    static uint8_t ui[0x1000], parent[0x40];
    hook::Global<uint32_t>(0x6d4020) = 0x6d40200;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    static const int32_t types[] = { 0x9f, 0x75, 0x91, 0x92, 0x7c, 0x65, 0x87, 0x6d, 0x86, 0xb6, 0xb5, 0xb2, 0xb3, 0xb4, 0x97, 0x98, 0x99, 0x70, 0xa7, 0x9e, 0x9a,
                                     0x9b, 0x7b, 0x81, 0x82, 0xa3, 0x6e, 0x77, 0xa2, 0xa0, 0xa1, 0x1e, 0x20, 0x22, 0x6, 0x7, 0xcb, 0x2, 0x78, 0x3, 0x50, 0xcc };
    static const char* names[] = { nullptr, "ImgA", "ImgB", "LongerImageName" };
    for (int it = 0; it < 6000; ++it) {
        uint8_t start[0x200], ev0[0x40];
        for (int k = 0; k < 0x200; ++k) start[k] = (uint8_t)Rnd();
        *(void***)start = g_weVtbl;
        *(uint16_t*)(start + 4) = (uint16_t)(Rnd() % 3);
        *(uint16_t*)(start + 9) = (uint16_t)(Rnd() % 3);
        *(uint16_t*)(start + 0x7c) = (uint16_t)(Rnd() % 3);
        *(uint16_t*)(start + 6) = (uint16_t)(Rnd() % 3 == 0 ? (Rnd() % 2 ? 1 : 8) : 2);
        *(const char**)(start + 0x47) = nullptr;
        *(void**)(start + 0x43) = nullptr;
        *(uint32_t*)(start + 0x92) = Rnd() % 2 ? 0x920 : 0;
        *(uint8_t**)(start + 0x4b) = Rnd() % 2 ? parent : nullptr;
        *(uint16_t*)(parent + 4) = (uint16_t)(Rnd() % 3);
        *(uint8_t**)(start + 0x86) = parent;
        *(float*)(parent + 5) = RndF(0.0f, 1.0f);
        *(float*)(parent + 9) = RndF(0.0f, 1.0f);
        for (uint32_t o = 0xb; o <= 0x17; o += 4) *(float*)(start + o) = RndF(0.0f, 1.0f);
        start[0x2d] = Rnd() % 2 ? (uint8_t)(Rnd() % 9) : 0;
        start[0x2e] = Rnd() % 2 ? (uint8_t)(Rnd() % 9) : 0;
        *(void**)(start + 0x9a) = Rnd() % 2 ? (void*)&WeClick : nullptr;
        for (int k = 0; k < 0x40; ++k) ev0[k] = (uint8_t)Rnd();
        int32_t type = types[Rnd() % (sizeof types / sizeof *types)];
        *(int32_t*)ev0 = type;
        *(uint32_t*)(ev0 + 4) = Rnd() % 3;
        if (Rnd() % 3 == 0) *(uint32_t*)(ev0 + 8) = 0;
        int nameK = (int)(Rnd() % 4), curK = (int)(Rnd() % 4);
        bool nameEv = type == 0x75 || type == 0x70 || type == 0x6e || type == 0x77 || type == 0xa3;
        *(float*)(ev0 + 0x24) = RndF(0.0f, 1.0f);
        *(float*)(ev0 + 0x28) = RndF(0.0f, 1.0f);
        int evTarget = (int)(Rnd() % 3);
        g_weEnabled = Rnd() % 4 != 0;
        g_weHit = Rnd() % 3 != 0;
        bool hovered = Rnd() % 2 != 0;
        uint8_t w[2][0x200], ev[2][0x40];
        std::string logs[2];
        int32_t ret[2];
        for (int side = 0; side < 2; ++side) {
            memcpy(w[side], start, sizeof start);
            memcpy(ev[side], ev0, sizeof ev0);
            *(uint8_t**)(ui + 0xc6d) = hovered ? w[side] : nullptr;
            *(uint8_t**)(ev[side] + 0x2c) = evTarget == 0 ? nullptr : evTarget == 1 ? w[side] : parent;
            logs[side] = t.Run([&] {
                if (curK) rt3::String_Assign((char**)(w[side] + 0x47), names[curK]);
                if (nameEv) *(const char**)(ev[side] + 8) = names[nameK];
                ret[side] = side ? rt3::Widget_HandleEvent(w[side], 0, ev[side]) : ((int32_t(__fastcall*)(uint8_t*, int, uint8_t*))0x558a10)(w[side], 0, ev[side]);
            });
        }
        CompareLogs(logs[0], logs[1], "widget event", it);
        EXPECT_EQ(ret[0], ret[1], "widget event ret iteration %d type %x", it, type);
        for (int side = 0; side < 2; ++side) {
            *(uint32_t*)(w[side] + 0x43) = *(uint32_t*)(w[side] + 0x43) ? 1 : 0;
            *(uint32_t*)(w[side] + 0x47) = *(uint32_t*)(w[side] + 0x47) ? 1 : 0;
            if (*(uint8_t**)(ev[side] + 0x2c) == w[side]) *(uint32_t*)(ev[side] + 0x2c) = 1;
            if (nameEv) *(uint32_t*)(ev[side] + 8) = 0;
        }
        EXPECT_EQ(memcmp(w[0], w[1], sizeof w[0]), 0, "widget event state iteration %d type %x", it, type);
        EXPECT_EQ(memcmp(ev[0], ev[1], sizeof ev[0]), 0, "widget event event iteration %d type %x", it, type);
    }
}

namespace rt3 {
void __fastcall Movie_StopIfHidden(uint8_t* w);
uint8_t* __fastcall Picture_Destroy(uint8_t* w, int, uint32_t flags);
int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag);
void __fastcall CompanyCmd_Bankrupt(uint32_t* cmd);
void __fastcall CompanyCmd_SetDividend(uint32_t* cmd);
void __fastcall CompanyCmd_RepayBond(uint32_t* cmd);
void __fastcall CompanyCmd_IssueShares(uint32_t* cmd);
void __fastcall CompanyCmd_IssueBond(uint32_t* cmd);
void __fastcall CompanyCmd_BuyBack(uint32_t* cmd);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
void __fastcall Widget_Destruct(uint8_t* w);
void __fastcall Movie_Stop(uint8_t* w, int, int32_t release);
void __fastcall Company_GoBankrupt(uint8_t* c);
void __fastcall Obj_Set4b(void* o, int, int32_t v);
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
int32_t __fastcall Company_FreeShares(void* c);
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
void __fastcall CompanyWindow_Refresh(uint8_t* win);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company,
                                  uint32_t* foreign, int32_t anyTrack);
}

namespace {
int32_t g_ccVisible, g_ccFree, g_ccStep;
uint32_t g_ccCost;
int32_t __fastcall FakeCcVisible(uint8_t* w) { Logf("visible"); return g_ccVisible; }
void __fastcall RecCcDestruct(uint8_t* w) { Logf("destruct"); }
void __fastcall RecCcMovieStop(uint8_t* w, int, int32_t r) { Logf("movie stop %d", r); }
void __cdecl RecCcDelete(void* p) { Logf("delete"); }
void __fastcall RecCcBankrupt(uint8_t* c) { Logf("bankrupt %p", c); }
void __fastcall RecCcSet4b(void* o, int, int32_t v) { Logf("dividend %p %d", o, v); }
void __fastcall RecCcRemove(void* c, int, int32_t i, int32_t p) { Logf("repay %p %d %d", c, i, p); }
int32_t __fastcall RecCcBond(uint8_t* c, int, int32_t a, int32_t y, int32_t m, uint32_t r) {
    Logf("bond %p %d %d %d %08x", c, a, y, m, r);
    return 0;
}
void __fastcall RecCcShares(uint8_t* c, int, int32_t n, int32_t at, int32_t p) { Logf("shares %p %d %d %d", c, n, at, p); }
int32_t __fastcall FakeCcFree(void* c) {
    Logf("free shares");
    return g_ccFree;
}
void __fastcall RecCcBuyBack(uint8_t* c, int, int32_t n) { Logf("buy back %p %d", c, n); }
void __fastcall RecCcRefresh(uint8_t* w) { Logf("refresh %p", w); }
void __fastcall RecCcCursor(uint8_t* c, int, int32_t i, int32_t f, int32_t n) { Logf("cursor %p %d %d %d", c, i, f, n); }
int32_t __fastcall FakeCcNextStep(Collection* r, int, int32_t from, int32_t to, uint32_t* cost, int32_t nf, uint32_t* co, uint32_t* fo, int32_t at) {
    Logf("next step %p %d %d %d %p %p %d", r, from, to, nf, co, fo, at);
    *cost = g_ccCost;
    return g_ccStep;
}
} // namespace

DIFFTEST(ui_CompanyCommands) {
    SavedGlobals sg({ 0x62be10, 0x6cec78, 0x6cfe4c, 0xccbb20, 0xd9797c, 0x6cfcb4, 0x6cfca8 });
    UiTrace t;
    t.Add(0x558670, (void*)&rt3::Widget_IsVisible, (void*)&FakeCcVisible);
    t.Add(0x558000, (void*)&rt3::Widget_Destruct, (void*)&RecCcDestruct);
    t.Add(0x55ed90, (void*)&rt3::Movie_Stop, (void*)&RecCcMovieStop);
    t.Add(0x53b080, nullptr, (void*)&RecCcDelete);
    t.Add(0x425a90, (void*)&rt3::Company_GoBankrupt, (void*)&RecCcBankrupt);
    t.Add(0x423fb0, (void*)&rt3::Obj_Set4b, (void*)&RecCcSet4b);
    t.Add(0x423d70, (void*)&rt3::Company_RemoveEntry, (void*)&RecCcRemove);
    t.Add(0x4275c0, (void*)&rt3::Company_IssueBond, (void*)&RecCcBond);
    t.Add(0x427450, (void*)&rt3::Company_IssueShares, (void*)&RecCcShares);
    t.Add(0x4261b0, (void*)&rt3::Company_FreeShares, (void*)&FakeCcFree);
    t.Add(0x4273c0, (void*)&rt3::Company_BuyBackShares, (void*)&RecCcBuyBack);
    t.Add(0x4c2ca0, (void*)&rt3::CompanyWindow_Refresh, (void*)&RecCcRefresh);
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&RecCcCursor);
    t.Add(0x4a6630, (void*)&rt3::Route_NextStep, (void*)&FakeCcNextStep);
    t.Add(0x4337b0, (void*)&rt3::Obj_Get25, (void*)&FakeRankGet25);
    static uint8_t world[0x100], widget[0x200];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint32_t>(0xccbb20) = 0xccbb200;
    hook::Global<uint32_t>(0x6cfcb4) = 0x6cfcb40;
    typedef void(__fastcall * CmdFn)(uint32_t*);
    static const uint32_t exe[] = { 0x4c6840, 0x4c6990, 0x4c67d0, 0x4c68a0, 0x4c6760, 0x4c6910 };
    static const CmdFn ours[] = { rt3::CompanyCmd_Bankrupt, rt3::CompanyCmd_SetDividend, rt3::CompanyCmd_RepayBond, rt3::CompanyCmd_IssueShares,
                                  rt3::CompanyCmd_IssueBond, rt3::CompanyCmd_BuyBack };
    for (int it = 0; it < 1200; ++it) {
        FakeTable cos(1 + Rnd() % 3, 0x100, [](uint8_t*, int) {});
        FakeTable nodes(1 + Rnd() % 3, 0x40, [](uint8_t*, int) {});
        hook::Global<Collection*>(0x62be10) = &cos.c;
        hook::Global<Collection*>(0x6cfca8) = &nodes.c;
        hook::Global<uint32_t>(0x6cfe4c) = Rnd() % 2 ? 0x6cfe4c0 : 0;
        g_rankGet25 = (int32_t)(Rnd() % 3);
        g_ccVisible = Rnd() % 2;
        g_ccFree = (int32_t)(Rnd() % 1000);
        g_ccStep = (int32_t)(Rnd() % 6) - 1;
        g_ccCost = Rnd() % 4 == 0 ? 0xffffffff : Rnd() % 50;
        uint32_t cmd[5] = { Rnd() % 3, Rnd() % 5, Rnd() % 1200, Rnd() % 30, Rnd() };
        int op = it % 9;
        int32_t from = (int32_t)(Rnd() % 9), to = (int32_t)(Rnd() % 9), nf = Rnd() % 2;
        uint32_t flags = Rnd() % 2;
        int32_t ret[2] = {};
        uintptr_t pret[2] = {};
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<int32_t>(0xd9797c) = 5;
            logs[side] = t.Run([&] {
                if (op < 6) side ? ours[op](cmd) : ((CmdFn)exe[op])(cmd);
                else if (op == 6) side ? rt3::Movie_StopIfHidden(widget) : ((void(__fastcall*)(uint8_t*))0x55ee60)(widget);
                else if (op == 7)
                    pret[side] = (uintptr_t)(side ? rt3::Picture_Destroy(widget, 0, flags) : ((uint8_t*(__fastcall*)(uint8_t*, int, uint32_t))0x55f9b0)(widget, 0, flags));
                else
                    ret[side] = side ? rt3::Route_CanReach(from, to, nf) : ((int32_t(__stdcall*)(int32_t, int32_t, int32_t))0x4a6e00)(from, to, nf);
            });
            ret[side] += hook::Global<int32_t>(0xd9797c) * 100 + (int32_t)*(uint32_t*)widget * 0;
            if (op == 7) ret[side] += *(uint32_t*)widget == 0x5de2a8 ? 1000 : 0;
        }
        CompareLogs(logs[0], logs[1], "company commands", it);
        EXPECT_EQ(ret[0], ret[1], "company commands result iteration %d op %d", it, op);
        EXPECT_EQ(pret[0], pret[1], "company commands pointer iteration %d op %d", it, op);
    }
}

namespace rt3 {
void __fastcall Train_RefreshRows(uint8_t* t);
void __fastcall CompanyCmd_BuyTerritory(uint32_t* cmd);
int32_t __fastcall Train_CanReachOrder(uint8_t* t, int, int32_t anyElectric);
uint8_t* __fastcall Node_NextTowards(uint8_t* n, int, int32_t target, int32_t car, int32_t prev, uint8_t* train, int32_t anyTrack);
uint8_t* __fastcall CompanyWindow_Construct(uint8_t* w);
uint32_t __fastcall TreeView_NodeData(uint8_t* w, int, int32_t i);
void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
void* __fastcall Array33_Current(void* a);
void* __fastcall Obj_CityRef(void* o);
void* __fastcall Train_Company(void* t);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild);
}

namespace {
uint8_t g_b2List[0x300], g_b2Order[0x40], g_b2City[0x20];
int32_t g_b2Rand;
void __fastcall RecB2Row(uint8_t* w, uint32_t id, int32_t c, int32_t s, int32_t r) { Logf("row %p %u %d %d %d", w, id, c, s, r); }
uint8_t* __fastcall FakeB2Find(uint8_t* w, int, int32_t id) { Logf("find %p %x", w, id); return g_b2List; }
uint32_t __fastcall FakeB2NodeData(uint8_t* w, int, int32_t i) { Logf("node data %d", i); return (uint32_t)(i % 5); }
void __fastcall RecB2EntryFlag(void* c, int, int32_t i, uint8_t v) { Logf("entry flag %p %d %d", c, i, v); }
void* __fastcall FakeB2Order(void* a) { return g_b2Order; }
void* __fastcall FakeB2City(void* o) { Logf("cityref"); return g_b2City; }
void* __fastcall FakeB2Company(void* t) { Logf("train company"); return (void*)0x7c0000; }
int32_t __fastcall FakeB2Rand(int32_t lo, int32_t hi) { Logf("rand %d %d", lo, hi); return g_b2Rand; }
uint8_t* __fastcall RecB2Vtbl(uint8_t* w) { Logf("base ctor"); return w; }
uint8_t* __fastcall FakeB2Font(uint8_t* p, int, int32_t i) { Logf("font %p %d", p, i); return (uint8_t*)0x50f0000; }
void __fastcall RecB2Load(uint8_t* w, int, float x, float y, const char* name) { Logf("load %08x %08x %s", B(x), B(y), name); }
} // namespace

DIFFTEST(ui_TrainAndCompanyHelpers) {
    SavedGlobals sg({ 0x62be10, 0x6cec78, 0x6cfe4c, 0xccbb20, 0x6cfcb4, 0x6cfca8, 0x6cec20, 0x62ba84, 0x6d0818, 0x6d3b20, 0x6d3b34, 0x6d4020, 0x6cfe50,
                      0x6cfe54, 0x6cfe58, 0x6cfe5c });
    UiTrace t;
    t.Add(0x4bf320, (void*)&rt3::TrainRow_Build, (void*)&RecB2Row);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB2Find);
    t.Add(0x5157e0, (void*)&rt3::Global6d3b30, (void*)&FakeTr6d3b30);
    t.Add(0x55fb80, (void*)&rt3::TreeView_NodeData, (void*)&FakeB2NodeData);
    t.Add(0x424030, (void*)&rt3::Company_SetEntryFlag, (void*)&RecB2EntryFlag);
    t.Add(0x42a040, (void*)&rt3::Company_AddStat, (void*)&RecAddStat);
    t.Add(0x4c2ca0, (void*)&rt3::CompanyWindow_Refresh, (void*)&RecCcRefresh);
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&RecCcCursor);
    t.Add(0x4337b0, (void*)&rt3::Obj_Get25, (void*)&FakeRankGet25);
    t.Add(0x4b2bd0, (void*)&rt3::Array33_Current, (void*)&FakeB2Order);
    t.Add(0x4a8840, (void*)&rt3::Obj_CityRef, (void*)&FakeB2City);
    t.Add(0x4a8bd0, (void*)&rt3::Train_Company, (void*)&FakeB2Company);
    t.Add(0x4a6630, (void*)&rt3::Route_NextStep, (void*)&FakeCcNextStep);
    t.Add(0x518d70, (void*)&rt3::Rand, (void*)&FakeB2Rand);
    t.Add(0x53f7e0, (void*)&rt3::Widget_SetVtblRet, (void*)&RecB2Vtbl);
    t.Add(0x53d110, (void*)&rt3::Pack_AcquireNamed, (void*)&FakeWeAcquire);
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeB2Font);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecB2Load);
    static uint8_t world[0x100], view[0x100], train[0x100], node[0x300], win[0x200];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint32_t>(0xccbb20) = 0xccbb200;
    hook::Global<uint32_t>(0x6cfcb4) = 0x6cfcb40;
    hook::Global<uint32_t>(0x6d4020) = 0x6d40200;
    for (int it = 0; it < 2000; ++it) {
        FakeTable cos(1 + Rnd() % 3, 0x100, [](uint8_t* r, int i) { *(uint32_t*)r = (uint32_t)i; });
        FakeTable nodes(6, 0x260, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0x202) = i;
            *(int32_t*)(r + 0x212) = (int32_t)(Rnd() % 3) - (Rnd() % 6 == 0 ? 2 : 0);
            (void)i;
        });
        FakeTable sts(3, 0x40, [](uint8_t* r, int) { *(int32_t*)(r + 8) = (int32_t)(Rnd() % 8) - 1; });
        FakeTable cars(3, 0x400, [](uint8_t* r, int) { *(int32_t*)(r + 0x356) = (int32_t)(Rnd() % 7) - 1; });
        hook::Global<Collection*>(0x62be10) = &cos.c;
        hook::Global<Collection*>(0x6cfca8) = &nodes.c;
        hook::Global<Collection*>(0x6cec20) = &sts.c;
        hook::Global<Collection*>(0x62ba84) = &cars.c;
        // ids the original looks up without a validity check must be -1 or real
        auto nodeId = [&]() { return Rnd() % 5 == 0 ? -1 : (int32_t)rt3::Collection_IdAt(&nodes.c, 0, Rnd() % 6, 0); };
        for (int k = 0; k < 6; ++k) {
            uint8_t* r = (uint8_t*)rt3::Collection_Get(&nodes.c, 0, rt3::Collection_IdAt(&nodes.c, 0, k, 0));
            *(int32_t*)(r + 0x202) = (int32_t)rt3::Collection_IdAt(&nodes.c, 0, k, 0);
        }
        for (int k = 0; k < 3; ++k) *(int32_t*)((uint8_t*)rt3::Collection_Get(&cars.c, 0, rt3::Collection_IdAt(&cars.c, 0, k, 0)) + 0x356) = nodeId();
        hook::Global<uint8_t*>(0x6d0818) = Rnd() % 8 ? view : nullptr;
        *(int32_t*)(view + 0x8c) = (int32_t)(Rnd() % 3);
        *(int32_t*)(view + 0x90) = (int32_t)(Rnd() % 4);
        hook::Global<uint32_t>(0x6d3b20) = Rnd() % 2 ? 0x6d3b200 : 0;
        hook::Global<uint32_t>(0x6d3b34) = Rnd() % 2 ? 0x6d3b340 : 0;
        hook::Global<uint32_t>(0x6cfe4c) = Rnd() % 2 ? (uint32_t)(uintptr_t)win : 0;
        *(int32_t*)(g_b2List + 0xa2) = (int32_t)(Rnd() % 6);
        *(int32_t*)(g_b2List + 0x1e2) = (int32_t)(Rnd() % 4);
        *(int32_t*)(g_b2List + 0x1d2) = (int32_t)(Rnd() % 9);
        g_trFlag = Rnd() % 2;
        g_rankGet25 = (int32_t)(Rnd() % 3);
        g_ccStep = Rnd() % 6 == 0 ? (int32_t)(Rnd() % 9) : nodeId();
        g_ccCost = Rnd() % 4 == 0 ? 0xffffffff : Rnd() % 50;
        g_b2Rand = (int32_t)(Rnd() % 100);
        *(int32_t*)(train + 0x25) = (int32_t)(Rnd() % 4);
        *(int32_t*)(train + 4) = (int32_t)(Rnd() % 3) - 1;
        *(int32_t*)(train + 0x49) = (int32_t)(Rnd() % 8) - 1;
        *(uint32_t*)(train + 0x51) = Rnd() % 3;
        g_b2Order[0x28] = Rnd() % 2 ? 0x80 : 0;
        *(int32_t*)(g_b2Order + 0x2b) = (int32_t)(Rnd() % 8) - 1;
        *(uint16_t*)(g_b2Order + 0x29) = (uint16_t)(Rnd() % 4);
        *(int32_t*)(g_b2City + 0x10) = (int32_t)(Rnd() % 3);
        node[0x201] = (uint8_t)Rnd();
        for (int k = 0; k < 4; ++k) *(int32_t*)(node + 0x202 + k * 4) = nodeId();
        uint32_t cmd[4] = { Rnd() % 3, Rnd() % 5, Rnd() % 4, Rnd() % 100000 };
        int32_t target = nodeId(), car = Rnd() % 4 == 0 ? -1 : (int32_t)rt3::Collection_IdAt(&cars.c, 0, Rnd() % 3, 0), prev = nodeId(), flag = Rnd() % 2;
        uint8_t* tr = Rnd() % 3 ? train : nullptr;
        int op = it % 5;
        uintptr_t ret[2] = {};
        uint32_t globs[2][4];
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            logs[side] = t.Run([&] {
                switch (op) {
                case 0: side ? rt3::Train_RefreshRows(train) : ((void(__fastcall*)(uint8_t*))0x4a8f50)(train); break;
                case 1: side ? rt3::CompanyCmd_BuyTerritory(cmd) : ((void(__fastcall*)(uint32_t*))0x4c6a00)(cmd); break;
                case 2:
                    ret[side] = (uintptr_t)(side ? rt3::Train_CanReachOrder(train, 0, flag) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x4a91b0)(train, 0, flag));
                    break;
                case 3:
                    ret[side] = (uintptr_t)(side ? rt3::Node_NextTowards(node, 0, target, car, prev, tr, flag)
                                                 : ((uint8_t*(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, uint8_t*, int32_t))0x48cad0)(node, 0, target, car,
                                                                                                                                    prev, tr, flag));
                    break;
                case 4: ret[side] = (uintptr_t)(side ? rt3::CompanyWindow_Construct(win) : ((uint8_t*(__fastcall*)(uint8_t*))0x4c5540)(win)); break;
                }
            });
            for (int k = 0; k < 4; ++k) globs[side][k] = hook::Global<uint32_t>(0x6cfe4c + k * 4);
            globs[side][0] ^= *(uint32_t*)win;
        }
        CompareLogs(logs[0], logs[1], "train and company helpers", it);
        EXPECT_EQ(ret[0], ret[1], "helpers result iteration %d op %d", it, op);
        EXPECT_EQ(memcmp(globs[0], globs[1], sizeof globs[0]), 0, "helpers globals iteration %d op %d", it, op);
    }
}

namespace rt3 {
void __fastcall Movie_Tick(uint8_t* w, int, int32_t force);
uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
int32_t ClockMs();
int32_t __fastcall Obj_BuildingStateActive(void* o);
}

namespace {
int32_t g_mtClock, g_mtWait;
uint8_t g_mtBink[0x300];
int32_t FakeMtClock() { return g_mtClock; }
void __stdcall MtGoto(uint8_t* b, uint32_t f, uint32_t fl) {
    Logf("goto %u %u", f, fl);
    *(uint32_t*)(b + 0x10) = f;
}
int32_t __stdcall MtDo(uint8_t* b) { Logf("do"); return 0; }
int32_t __stdcall MtNext(uint8_t* b) {
    Logf("next");
    ++*(uint32_t*)(b + 0x10);
    return 0;
}
int32_t __stdcall MtWait(uint8_t* b) {
    Logf("wait");
    return (g_mtWait = (g_mtWait * 5 + 3) % 7) < 3;
}
uint32_t __stdcall MtKey(uint8_t* b, uint32_t k, uint32_t fl) { Logf("key %u %u", k, fl); return 9; }
void RecMtEnd() { Logf("end callback"); }
int32_t __fastcall FakeActiveSt(void* o) { return ((uint8_t*)o)[0x20] % 3 != 0; }
} // namespace

DIFFTEST(ui_MovieTickAndOrders) {
    SavedGlobals sg({ 0xd97970, 0x6d4000, 0x62be10, 0x6cec20, 0x6cfca8, 0x6cfcb4 });
    UiTrace t;
    t.Add(0x558670, (void*)&rt3::Widget_IsVisible, (void*)&FakeCcVisible);
    t.Add(0x55ed90, (void*)&rt3::Movie_Stop, (void*)&RecCcMovieStop);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeMtClock);
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeActiveSt);
    t.Add(0x4a8bd0, (void*)&rt3::Train_Company, (void*)&FakeB2Company);
    t.Add(0x4a8840, (void*)&rt3::Obj_CityRef, (void*)&FakeB2City);
    t.Add(0x4a6630, (void*)&rt3::Route_NextStep, (void*)&FakeCcNextStep);
    struct Iat {
        uint32_t at;
        void* fn;
        void* saved;
    } iats[] = { { 0x5c836c, (void*)&MtGoto }, { 0x5c8370, (void*)&MtDo }, { 0x5c8374, (void*)&MtNext }, { 0x5c8378, (void*)&MtWait }, { 0x5c837c, (void*)&MtKey } };
    DWORD prot;
    VirtualProtect((void*)0x5c836c, 0x14, PAGE_READWRITE, &prot);
    for (Iat& i : iats) {
        i.saved = *(void**)i.at;
        *(void**)i.at = i.fn;
    }
    static uint8_t w[0x200], train[0x100];
    hook::Global<uint32_t>(0x6cfcb4) = 0x6cfcb40;
    for (int it = 0; it < 3000; ++it) {
        int op = it % 2;
        std::string logs[2];
        uint32_t res[2][8] = {};
        if (op == 0) {
            int32_t force = Rnd() % 2;
            g_ccVisible = Rnd() % 4 != 0;
            hook::Global<int32_t>(0xd97970) = Rnd() % 8 == 0;
            hook::Global<int32_t>(0x6d4000) = Rnd() % 4 == 0;
            uint32_t total = 2 + Rnd() % 40, cur = Rnd() % 4 == 0 ? total : 1 + Rnd() % total, rate = 1 + Rnd() % 30, den = Rnd() % 3;
            int32_t last = Rnd() % 6 == 0 ? 0 : 1000 + (int32_t)(Rnd() % 1000), clock = last + (int32_t)(Rnd() % 900);
            int32_t kind = (int32_t)(Rnd() % 8), ae = Rnd() % 2, old = Rnd() % 3 == 0, cbOn = Rnd() % 2, playing = Rnd() % 8 != 0, ended = Rnd() % 8 == 0;
            for (int side = 0; side < 2; ++side) {
                memset(g_mtBink, 0, sizeof g_mtBink);
                *(uint32_t*)(g_mtBink + 8) = total;
                *(uint32_t*)(g_mtBink + 0x10) = cur;
                *(uint32_t*)(g_mtBink + 0x18) = rate;
                *(uint32_t*)(g_mtBink + 0x14) = den;
                *(uint32_t*)(g_mtBink + 0x2a0) = ended;
                memset(w, 0, sizeof w);
                *(uint8_t**)(w + 0xf2) = playing ? g_mtBink : nullptr;
                *(int32_t*)(w + 0x102) = last;
                *(int32_t*)(w + 0xea) = kind;
                *(int32_t*)(w + 0xae) = ae;
                *(int32_t*)(w + 0x106) = old;
                *(void**)(w + 0xd6) = cbOn ? (void*)&RecMtEnd : nullptr;
                g_mtClock = clock;
                g_mtWait = 0;
                logs[side] = t.Run([&] { side ? rt3::Movie_Tick(w, 0, force) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x55ee80)(w, 0, force); });
                res[side][0] = *(uint32_t*)(w + 0x102);
                res[side][1] = *(uint32_t*)(w + 0xda);
                res[side][2] = *(uint32_t*)(w + 0xae);
                res[side][3] = *(uint32_t*)(w + 0x106);
                res[side][4] = *(uint32_t*)(g_mtBink + 0x10);
            }
        } else {
            int n = (int)(Rnd() % 5);
            static uint8_t recs[5 * 0x33];
            FakeTable cos(2, 0x100, [](uint8_t* r, int i) { *(uint32_t*)r = (uint32_t)i; });
            FakeTable sts(4, 0x40, [](uint8_t* r, int) {
                *(int32_t*)(r + 8) = (int32_t)(Rnd() % 6) - 1;
                r[0x20] = (uint8_t)Rnd();
            });
            FakeTable nodes(4, 0x40, [](uint8_t*, int) {});
            hook::Global<Collection*>(0x62be10) = &cos.c;
            hook::Global<Collection*>(0x6cec20) = &sts.c;
            hook::Global<Collection*>(0x6cfca8) = &nodes.c;
            for (int k = 0; k < 5; ++k) {
                uint8_t* r = recs + k * 0x33;
                r[0x28] = Rnd() % 4 == 0 ? 0x80 : 0;
                *(int32_t*)(r + 0x2b) = (int32_t)(Rnd() % 6) - 1;
                *(uint16_t*)(r + 0x29) = (uint16_t)(Rnd() % 6);
            }
            uint32_t orders[2] = { (uint32_t)(uintptr_t)recs, (uint32_t)n };
            uint8_t* tr = Rnd() % 4 ? train : nullptr;
            int32_t start = (int32_t)(Rnd() % 6) - 1, fe = Rnd() % 4 == 0, ne = Rnd() % 4 == 0, cid = (int32_t)(Rnd() % 5) - 1;
            *(int32_t*)(g_b2City + 0x10) = (int32_t)(Rnd() % 3);
            g_ccStep = 1;
            g_ccCost = Rnd() % 6 == 0 ? 0xffffffff : 5;
            for (int side = 0; side < 2; ++side) {
                logs[side] = t.Run([&] {
                    res[side][0] = side ? rt3::Orders_Drivable(orders, 0, tr, start, fe, ne, cid)
                                        : ((uint8_t(__fastcall*)(uint32_t*, int, uint8_t*, int32_t, int32_t, int32_t, int32_t))0x4b2c10)(orders, 0, tr, start, fe, ne, cid);
                });
            }
        }
        CompareLogs(logs[0], logs[1], "movie tick / orders", it);
        EXPECT_EQ(memcmp(res[0], res[1], sizeof res[0]), 0, "movie tick / orders result iteration %d op %d", it, op);
    }
    for (int k = 4; k >= 0; --k) *(void**)iats[k].at = iats[k].saved;
    VirtualProtect((void*)0x5c836c, 0x14, prot, &prot);
}

namespace rt3 {
void __fastcall TrainWindow_Refresh(uint8_t* win);
int32_t __fastcall Stations_CargoLinked(Collection* sts, int, int32_t a, int32_t b, int32_t aIsRegion, int32_t bIsRegion, int32_t value);
uint8_t __fastcall Obj_Flag3d4(void* o);
uint32_t __fastcall Obj_BuildingRegionIndex(void* o);
int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_BuildingValue(void* o);
void __fastcall Window_UpdateListButtons(uint8_t* w);
void* __fastcall Obj_Building(void* o);
int32_t __fastcall Game_EffectiveVersion();
}

namespace {
int32_t g_scVersion;
void __fastcall RecListButtons(uint8_t* w) { Logf("list buttons %p", w); }
void* __fastcall FakeScBuilding(void* o) { return o; }
uint8_t __fastcall FakeScFlag(void* o) { return ((uint8_t*)o)[0x20] % 4 == 0 ? 0 : 1; }
int32_t __fastcall FakeScVersion() { Logf("version"); return g_scVersion; }
uint32_t __fastcall FakeScRegion(void* o) { Logf("region %p", o); return ((uint8_t*)o)[0x21] % 4; }
int32_t __fastcall FakeScHasKey(void* o, int, uint16_t k) { Logf("has key %p %u", o, k); return (k + ((uint8_t*)o)[0] * 0) % 3 != 0; }
int32_t __fastcall FakeScValue(void* o) { Logf("value %p", o); return ((uint8_t*)o)[0x22] % 3; }
} // namespace

DIFFTEST(ui_TrainWindowAndStationLinks) {
    SavedGlobals sg({ 0x6d3b24, 0x6d0818, 0x6cfcbc, 0x62bae0, 0x6cfcb4 });
    UiTrace t;
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB2Find);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecTrDelete);
    t.Add(0x4bf320, (void*)&rt3::TrainRow_Build, (void*)&RecB2Row);
    t.Add(0x513720, (void*)&rt3::Window_UpdateListButtons, (void*)&RecListButtons);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeScBuilding);
    t.Add(0x40c990, (void*)&rt3::Obj_Flag3d4, (void*)&FakeScFlag);
    t.Add(0x482e00, (void*)&rt3::Game_EffectiveVersion, (void*)&FakeScVersion);
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeActiveSt);
    t.Add(0x47f170, (void*)&rt3::Obj_BuildingRegionIndex, (void*)&FakeScRegion);
    t.Add(0x41f6e0, (void*)&rt3::Obj_RegionHasKey, (void*)&FakeScHasKey);
    t.Add(0x47efe0, (void*)&rt3::Obj_BuildingValue, (void*)&FakeScValue);
    t.Add(0x4a6630, (void*)&rt3::Route_NextStep, (void*)&FakeCcNextStep);
    static uint8_t view[0x100], win[0x100];
    hook::Global<uint32_t>(0x6cfcb4) = 0x6cfcb40;
    for (int it = 0; it < 1500; ++it) {
        int op = it % 2;
        std::string logs[2];
        int32_t ret[2] = {};
        if (op == 0) {
            FakeTable trains(1 + Rnd() % 3, 0x100, [](uint8_t* r, int i) {
                *(int32_t*)(r + 4) = (int32_t)(Rnd() % 5);
                *(int32_t*)(r + 0x25) = i;
            });
            hook::Global<Collection*>(0x6cfcbc) = &trains.c;
            hook::Global<uint8_t*>(0x6d0818) = Rnd() % 8 ? view : nullptr;
            *(int32_t*)(view + 0x90) = (int32_t)(Rnd() % 5) - 1;
            hook::Global<int32_t>(0x6d3b24) = Rnd() % 2;
            *(int32_t*)(g_b2List + 0x1d6) = (int32_t)(Rnd() % 8) - 2;
            *(int32_t*)(g_b2List + 0x1e2) = (int32_t)(Rnd() % 5);
            for (int side = 0; side < 2; ++side)
                logs[side] = t.Run([&] { side ? rt3::TrainWindow_Refresh(win) : ((void(__fastcall*)(uint8_t*))0x514160)(win); });
        } else {
            FakeTable sts(Rnd() % 7, 0x40, [](uint8_t* r, int) {
                *(int32_t*)(r + 8) = (int32_t)(Rnd() % 4);
                r[0x20] = (uint8_t)Rnd();
                r[0x21] = (uint8_t)Rnd();
                r[0x22] = (uint8_t)Rnd();
            });
            FakeTable cargo(3, 0x40, [](uint8_t*, int) {});
            hook::Global<Collection*>(0x62bae0) = &cargo.c;
            g_scVersion = Rnd() % 2 ? 0x66 : 0x68;
            g_ccStep = 1;
            g_ccCost = Rnd() % 3 == 0 ? 0xffffffff : 5;
            int32_t a = (int32_t)rt3::Collection_IdAt(&cargo.c, 0, Rnd() % 3, 0), b = Rnd() % 4 == 0 ? a : (int32_t)rt3::Collection_IdAt(&cargo.c, 0, Rnd() % 3, 0);
            int32_t ar = Rnd() % 2, br = Rnd() % 2, value = Rnd() % 3 == 0 ? -1 : (int32_t)(Rnd() % 3);
            for (int side = 0; side < 2; ++side)
                logs[side] = t.Run([&] {
                    ret[side] = side ? rt3::Stations_CargoLinked(&sts.c, 0, a, b, ar, br, value)
                                     : ((int32_t(__fastcall*)(Collection*, int, int32_t, int32_t, int32_t, int32_t, int32_t))0x4815d0)(&sts.c, 0, a, b, ar, br, value);
                });
        }
        CompareLogs(logs[0], logs[1], "train window / station links", it);
        EXPECT_EQ(ret[0], ret[1], "station links result iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Company_BuildStationLinks(int32_t* co);
int32_t __fastcall Station_ConnectedToHome(uint8_t* s, int, uint32_t company);
double __fastcall Obj_BuildingCellY(void* o);
double __fastcall Obj_BuildingCellX(void* o);
float __fastcall Company_NearestSiteDistance(int32_t* c, int, int32_t x, int32_t y);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
void __fastcall Company_RateStations(int32_t* co);
}

namespace {
int32_t __fastcall FakeSlHome(uint8_t* s, int, uint32_t c) { Logf("home %p %u", s, c); return s[0x23] % 3 != 0; }
double __fastcall FakeSlY(void* o) { Logf("cell y"); return ((uint8_t*)o)[0x24] * 1.5; }
double __fastcall FakeSlX(void* o) { Logf("cell x"); return ((uint8_t*)o)[0x25] * 2.25; }
float __fastcall FakeSlDist(int32_t* c, int, int32_t x, int32_t y) { Logf("dist %d %d", x, y); return (float)((x * 7 + y * 3) % 40); }
int32_t __fastcall FakeSlSame(uint8_t* n, int, uint32_t id) { Logf("same %p %u", n, id); return (id + ((uintptr_t)n >> 4)) % 4 != 0; }
void __fastcall RecSlRate(int32_t* c) { Logf("rate %d", c[0]); }
int32_t __fastcall FakeSlStep(Collection* r, int, int32_t from, int32_t to, uint32_t* cost, int32_t nf, uint32_t* co, uint32_t* fo, int32_t at) {
    Logf("step %d %d %d %u %d", from, to, nf, co ? *co : 99, at);
    *cost = (from * 13 + to * 7) % 9 == 0 ? 0xffffffff : (uint32_t)((from + to) % 6);
    if (fo) *fo = (uint32_t)((from * to) % 4);
    return 1;
}
} // namespace

DIFFTEST(ai_StationLinks) {
    SavedGlobals sg({ 0x6cec78, 0x6cec20, 0x6cfca8, 0x6cfcb4 });
    UiTrace t;
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeActiveSt);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeScBuilding);
    t.Add(0x40c990, (void*)&rt3::Obj_Flag3d4, (void*)&FakeScFlag);
    t.Add(0x47efe0, (void*)&rt3::Obj_BuildingValue, (void*)&FakeScValue);
    t.Add(0x4801a0, (void*)&rt3::Station_ConnectedToHome, (void*)&FakeSlHome);
    t.Add(0x47df00, (void*)&rt3::Obj_BuildingCellY, (void*)&FakeSlY);
    t.Add(0x47ded0, (void*)&rt3::Obj_BuildingCellX, (void*)&FakeSlX);
    t.Add(0x405920, (void*)&rt3::Company_NearestSiteDistance, (void*)&FakeSlDist);
    t.Add(0x48e3c0, (void*)&rt3::Node_SameComponent, (void*)&FakeSlSame);
    t.Add(0x407bd0, (void*)&rt3::Company_RateStations, (void*)&RecSlRate);
    t.Add(0x4a6630, (void*)&rt3::Route_NextStep, (void*)&FakeSlStep);
    static uint8_t world[0x100], entries[2][8][3][0x10];
    static int32_t co[0x400];
    static uint8_t* entryPtrs[2][8][3];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint32_t>(0x6cfcb4) = 0x6cfcb40;
    for (int it = 0; it < 600; ++it) {
        int n = (int)(Rnd() % 8);
        FakeTable sts(n, 0x600, [](uint8_t* r, int i) {
            *(int32_t*)r = i + 1;
            *(int32_t*)(r + 8) = (int32_t)(Rnd() % 9);
            for (int k = 0x20; k < 0x26; ++k) r[k] = (uint8_t)Rnd();
        });
        FakeTable nodes(9, 0x40, [](uint8_t*, int) {});
        hook::Global<Collection*>(0x6cec20) = &sts.c;
        hook::Global<Collection*>(0x6cfca8) = &nodes.c;
        *(uint32_t*)(world + 0x15) = Rnd();
        co[0] = (int32_t)(Rnd() % 3);
        uint8_t snap[2][8][3][0x10];
        std::string recs[2], logs[2];
        uint32_t d3e[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < n; ++k) {
                uint8_t* s = (uint8_t*)rt3::Collection_Get(&sts.c, 0, rt3::Collection_IdAt(&sts.c, 0, k, 0));
                for (int c = 0; c < 3; ++c) {
                    memset(entries[side][k][c], 0x33, 0x10);
                    *(void**)(entries[side][k][c] + 6) = nullptr;
                    entryPtrs[side][k][c] = entries[side][k][c];
                }
                *(uint8_t***)(s + 0x5bd) = entryPtrs[side][k];
            }
            logs[side] = t.Run([&] { side ? rt3::Company_BuildStationLinks(co) : ((void(__fastcall*)(int32_t*))0x4093d0)(co); });
            d3e[side] = *(uint32_t*)((uint8_t*)co + 0xd3e);
            for (int k = 0; k < n; ++k) {
                uint8_t* e = entries[side][k][co[0]];
                uint8_t* rp = *(uint8_t**)(e + 6);
                if (rp) recs[side].append((char*)rp, *(int32_t*)(e + 2) * 0xd + 1);
                memcpy(snap[side][k], entries[side][k], sizeof snap[side][k]);
                for (int c = 0; c < 3; ++c) *(uint32_t*)(snap[side][k][c] + 6) = *(uint32_t*)(entries[side][k][c] + 6) ? 1 : 0;
            }
        }
        CompareLogs(logs[0], logs[1], "station links", it);
        EXPECT_EQ(recs[0] == recs[1], true, "station link records iteration %d", it);
        EXPECT_EQ(memcmp(snap[0], snap[1], sizeof(uint8_t) * n * 3 * 0x10), 0, "station link entries iteration %d", it);
        EXPECT_EQ(d3e[0], d3e[1], "station links d3e iteration %d", it);
    }
}

namespace rt3 {
void TrainWindow_RefreshOpen();
void __fastcall Company_UpdateStationLinks(int32_t* co);
void __fastcall Companies_BuildStationLinks(Collection* cos);
int32_t __fastcall TrainCmd_Apply(uint32_t* cmd);
uint8_t* __fastcall TrainWindow_Construct(uint8_t* w);
void __fastcall TrainWindow_Refresh(uint8_t* win);
void __fastcall Company_BuildStationLinks(int32_t* co);
void __fastcall Train_SetBuffer1c6(uint8_t* t, int, const uint8_t* src);
int32_t Global6d40e4();
void __fastcall Window_SetLargeLayout(uint8_t* w, int, int32_t large);
const char* __fastcall GetString(int32_t id);
}

namespace {
int32_t g_twPlayer;
void __fastcall RecTwRefresh(uint8_t* w) { Logf("train window refresh %p", w); }
void __fastcall RecTwBuild(int32_t* co) { Logf("build links %d", co[0]); }
void __fastcall RecTwBuffer(uint8_t* t, int, const uint8_t* src) { Logf("train buffer %p %p", t, src); }
int32_t FakeTwPlayer() { return g_twPlayer; }
void __fastcall RecTwLarge(uint8_t* w, int, int32_t l) { Logf("large %d", l); }
// The exe's and the rewrite's string tables return different pointers for the
// same text: hand out an arena copy so events log the text.
const char* __fastcall FakeTwString(int32_t id) {
    char* s = (char*)ArenaAlloc(16);
    sprintf(s, "str%x", id);
    return s;
}
} // namespace

DIFFTEST(ui_TrainWindowMisc) {
    SavedGlobals sg({ 0x6d3b20, 0x6d3b14, 0x6d3b18, 0x6d3b1c, 0x6d3b24, 0x6d3b28, 0x6cec78, 0x6cec7c, 0x6cfcbc, 0xccbb20, 0x6d4020 });
    UiTrace t;
    t.Add(0x514160, (void*)&rt3::TrainWindow_Refresh, (void*)&RecTwRefresh);
    t.Add(0x4093d0, (void*)&rt3::Company_BuildStationLinks, (void*)&RecTwBuild);
    t.Add(0x407bd0, (void*)&rt3::Company_RateStations, (void*)&RecSlRate);
    t.Add(0x4a7390, (void*)&rt3::Train_SetBuffer1c6, (void*)&RecTwBuffer);
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeTwPlayer);
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&RecCcCursor);
    t.Add(0x53f7e0, (void*)&rt3::Widget_SetVtblRet, (void*)&RecB2Vtbl);
    t.Add(0x53d110, (void*)&rt3::Pack_AcquireNamed, (void*)&FakeWeAcquire);
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeB2Font);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecB2Load);
    t.Add(0x512f80, (void*)&rt3::Window_SetLargeLayout, (void*)&RecTwLarge);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeTwString);
    static uint8_t world[0x4b00], newGame[0x100], win[0x200];
    static int32_t co[0x400];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint32_t>(0xccbb20) = 0xccbb200;
    hook::Global<uint32_t>(0x6d4020) = 0x6d40200;
    for (int it = 0; it < 1500; ++it) {
        int op = it % 5;
        FakeTable trains(1 + Rnd() % 3, 0x100, [](uint8_t*, int) {});
        FakeTable cos(Rnd() % 5, 0x100, [](uint8_t* r, int i) {
            *(int32_t*)r = i;
            *(int32_t*)(r + 0x3f) = Rnd() % 3 != 0;
        });
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        *(int32_t*)(world + 0x15) = 0x100000 + (int32_t)(Rnd() % 0x100000);
        *(int32_t*)(world + 0x4aef) = Rnd() % 4 == 0;
        newGame[0x82] = Rnd() % 4 == 0;
        g_twPlayer = (int32_t)(Rnd() % 3);
        hook::Global<int32_t>(0x6d3b24) = Rnd() % 2;
        hook::Global<int32_t>(0x6d3b28) = Rnd() % 2;
        uint32_t open = Rnd() % 2 ? 0x6d3b200 : 0;
        uint8_t* c8 = (uint8_t*)co;
        *(int32_t*)(c8 + 0xd3e) = Rnd() % 4 == 0 ? 0 : *(int32_t*)(world + 0x15) - (int32_t)(Rnd() % 0x100000);
        *(int32_t*)(c8 + 0xd3a) = Rnd() % 4 == 0 ? 0 : *(int32_t*)(world + 0x15) - (int32_t)(Rnd() % 0x60000);
        uint32_t cmd[4] = { Rnd() % 3, Rnd() % 5, Rnd(), Rnd() };
        int32_t ret[2] = {};
        uint32_t g[2][3];
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<uint32_t>(0x6d3b20) = open;
            hook::Global<int32_t>(0x6d3b28) = it & 8 ? 1 : 0;
            logs[side] = t.Run([&] {
                switch (op) {
                case 0: side ? rt3::TrainWindow_RefreshOpen() : ((void(*)())0x514410)(); break;
                case 1: side ? rt3::Company_UpdateStationLinks(co) : ((void(__fastcall*)(int32_t*))0x409720)(co); break;
                case 2: side ? rt3::Companies_BuildStationLinks(&cos.c) : ((void(__fastcall*)(Collection*))0x429c10)(&cos.c); break;
                case 3: ret[side] = side ? rt3::TrainCmd_Apply(cmd) : ((int32_t(__fastcall*)(uint32_t*))0x514620)(cmd); break;
                case 4: ret[side] = (int32_t)(uintptr_t)(side ? rt3::TrainWindow_Construct(win) : ((uint8_t*(__fastcall*)(uint8_t*))0x514420)(win)); break;
                }
            });
            g[side][0] = hook::Global<uint32_t>(0x6d3b20) ^ *(uint32_t*)win;
            g[side][1] = hook::Global<uint32_t>(0x6d3b28);
            g[side][2] = hook::Global<uint32_t>(0x6d3b14) ^ hook::Global<uint32_t>(0x6d3b18) ^ hook::Global<uint32_t>(0x6d3b1c);
        }
        CompareLogs(logs[0], logs[1], "train window misc", it);
        EXPECT_EQ(ret[0], ret[1], "train window misc result iteration %d op %d", it, op);
        EXPECT_EQ(memcmp(g[0], g[1], sizeof g[0]), 0, "train window misc globals iteration %d op %d", it, op);
    }
}

namespace rt3 {
int32_t __fastcall Node_Advance(uint8_t* n, int, float pos, float dist, float* x, float* z, float* outPos, int32_t* dir, int32_t* count, float* accum,
                                int32_t car, int32_t, int32_t front);
int32_t __fastcall Train_OrderTarget(uint8_t* t, int, int32_t* dist, int32_t preferStop);
int32_t __fastcall Train_CurrentOrderNode(uint8_t* t);
void* __fastcall Obj_Train390(void* o);
void __fastcall Vehicle_PointAlong(uint8_t* o, int, float t, float* x, float* z);
int32_t __fastcall Node_IsRouteEnd(uint8_t* n);
}

namespace {
Collection* g_naNodes;
int32_t g_naT1, g_naT2;
int32_t __fastcall FakeNaTarget(uint8_t* t, int, int32_t* d, int32_t p) { Logf("order target %p %p %d", t, d, p); return g_naT1; }
int32_t __fastcall FakeNaCurrent(uint8_t* t) { Logf("current node %p", t); return g_naT2; }
void* __fastcall FakeNaTrain(void* c) { return (void*)0x7a0000; }
void __fastcall RecNaPoint(uint8_t* o, int, float t, float* x, float* z) {
    Logf("point %p %08x", o, B(t));
    *x = t * 2;
    *z = t * 3;
}
int32_t __fastcall FakeNaEnd(uint8_t* n) { return n[0x30] % 2; }
uint8_t* __fastcall FakeNaTowards(uint8_t* n, int, int32_t target, int32_t car, int32_t prev, uint8_t* train, int32_t any) {
    Logf("towards %p %d %d %d %p %d", n, target, car, prev, train, any);
    uint32_t k = (uint32_t)(target * 7 + car * 3 + prev) % 5;
    return k == 0 ? nullptr : (uint8_t*)rt3::Collection_Get(g_naNodes, 0, rt3::Collection_IdAt(g_naNodes, 0, k % 6, 0));
}
} // namespace

DIFFTEST(game_NodeAdvance) {
    SavedGlobals sg({ 0x6cfca8, 0x62ba84, 0x6cfcbc });
    UiTrace t;
    t.Add(0x4a83a0, (void*)&rt3::Train_OrderTarget, (void*)&FakeNaTarget);
    t.Add(0x4a8470, (void*)&rt3::Train_CurrentOrderNode, (void*)&FakeNaCurrent);
    t.Add(0x41b8f0, (void*)&rt3::Obj_Train390, (void*)&FakeNaTrain);
    t.Add(0x48a440, (void*)&rt3::Vehicle_PointAlong, (void*)&RecNaPoint);
    t.Add(0x48b870, (void*)&rt3::Node_IsRouteEnd, (void*)&FakeNaEnd);
    t.Add(0x48cad0, (void*)&rt3::Node_NextTowards, (void*)&FakeNaTowards);
    for (int it = 0; it < 4000; ++it) {
        FakeTable nodes(6, 0x260, [](uint8_t* r, int) {
            r[0x216] = Rnd() % 6 ? 2 : 1;
            r[0x201] = (uint8_t)Rnd();
            r[0x30] = (uint8_t)Rnd();
        });
        FakeTable cars(2, 0x400, [](uint8_t* r, int) {
            *(uint32_t*)(r + 0x390) = 1;
            *(int32_t*)(r + 0x34e) = (int32_t)(Rnd() % 4);
            *(int32_t*)(r + 0x352) = (int32_t)(Rnd() % 4);
        });
        FakeTable trains(1, 0x100, [](uint8_t*, int) {});
        g_naNodes = &nodes.c;
        hook::Global<Collection*>(0x6cfca8) = &nodes.c;
        hook::Global<Collection*>(0x62ba84) = &cars.c;
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        auto id = [&](int k) { return (int32_t)rt3::Collection_IdAt(&nodes.c, 0, k, 0); };
        for (int k = 0; k < 6; ++k) {
            uint8_t* r = (uint8_t*)rt3::Collection_Get(&nodes.c, 0, id(k));
            *(int32_t*)(r + 0x202) = id(k);
            *(int32_t*)(r + 0x206) = Rnd() % 8 == 0 ? -1 : id((int)(Rnd() % 6));
            *(int32_t*)(r + 0x20a) = Rnd() % 8 == 0 ? -1 : id((int)(Rnd() % 6));
            *(int32_t*)(r + 0x20e) = Rnd() % 2 ? -1 : id((int)(Rnd() % 6));
        }
        g_naT1 = (int32_t)(Rnd() % 4);
        g_naT2 = Rnd() % 2 ? g_naT1 : (int32_t)(Rnd() % 4);
        uint8_t* start = (uint8_t*)rt3::Collection_Get(&nodes.c, 0, id((int)(Rnd() % 6)));
        float pos = RndF(0.0f, 0.362f), dist = Rnd() % 3 ? RndF(0.0f, 0.5f) : RndF(0.0f, 3.0f);
        int32_t dir0 = Rnd() % 2, car = Rnd() % 3 == 0 ? -1 : (int32_t)rt3::Collection_IdAt(&cars.c, 0, Rnd() % 2, 0), front = Rnd() % 2;
        bool useCount = Rnd() % 2, useAccum = Rnd() % 2;
        std::string logs[2];
        int32_t ret[2];
        uint32_t out[2][6];
        for (int side = 0; side < 2; ++side) {
            float x = 7, z = 8, op = 9, acc = 1.5f;
            int32_t dir = dir0, cnt = 3;
            logs[side] = t.Run([&] {
                ret[side] = side ? rt3::Node_Advance(start, 0, pos, dist, &x, &z, &op, &dir, useCount ? &cnt : nullptr, useAccum ? &acc : nullptr, car, 0, front)
                                 : ((int32_t(__fastcall*)(uint8_t*, int, float, float, float*, float*, float*, int32_t*, int32_t*, float*, int32_t, int32_t,
                                                          int32_t))0x48edf0)(start, 0, pos, dist, &x, &z, &op, &dir, useCount ? &cnt : nullptr,
                                                                             useAccum ? &acc : nullptr, car, 0, front);
            });
            out[side][0] = B(x);
            out[side][1] = B(z);
            out[side][2] = B(op);
            out[side][3] = (uint32_t)dir;
            out[side][4] = (uint32_t)cnt;
            out[side][5] = B(acc);
        }
        CompareLogs(logs[0], logs[1], "node advance", it);
        EXPECT_EQ(ret[0], ret[1], "node advance result iteration %d", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "node advance outputs iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Train_RefreshWindowIfShown(uint8_t* t);
}

namespace {
void RecTwOpen() { Logf("refresh open"); }
} // namespace

DIFFTEST(ui_TrainRefreshIfShown) {
    SavedGlobals sg({ 0x6d0818 });
    UiTrace t;
    t.Add(0x514410, (void*)&rt3::TrainWindow_RefreshOpen, (void*)&RecTwOpen);
    static uint8_t view[0x100], train[0x40];
    for (int it = 0; it < 300; ++it) {
        hook::Global<uint8_t*>(0x6d0818) = Rnd() % 6 ? view : nullptr;
        *(int32_t*)(view + 0x8c) = (int32_t)(Rnd() % 3);
        *(int32_t*)(view + 0x90) = (int32_t)(Rnd() % 3);
        *(int32_t*)(train + 0x25) = (int32_t)(Rnd() % 3);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*))0x4a8370)(train); });
        std::string b = t.Run([&] { rt3::Train_RefreshWindowIfShown(train); });
        CompareLogs(a, b, "train refresh if shown", it);
    }
}

namespace rt3 {
void __fastcall Car_UpdatePose(uint8_t* car, int, float dist);
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
double __fastcall Effect_Blend(uint8_t* e, int, float t);
int32_t IsMultiplayer();
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Obj_MoveToCell(uint8_t* o, int, float cx, float cz, float y);
void __fastcall Model_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Model_SetPartAngle2(uint8_t* o, int, int32_t part, uint32_t v);
void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
}

namespace {
struct PoseCase {
    int32_t ret[2], flip[2];
    float x[2], z[2], at[2];
    int32_t hidden, mp, ver, contains;
    float head[8];
} g_pc;
int g_pcCall;
Collection* g_pcNodes;
int32_t __fastcall FakePcAdvance(uint8_t* n, int, float pos, float dist, float* x, float* z, float* op, int32_t* dir, int32_t* cnt, float* acc, int32_t car,
                                 int32_t u, int32_t front) {
    Logf("advance %p %08x %08x %d %p %p %d %d %d", n, B(pos), B(dist), *dir, cnt, acc, car, u, front);
    int k = g_pcCall++ & 1;
    *x = g_pc.x[k];
    *z = g_pc.z[k];
    *op = g_pc.at[k];
    if (g_pc.flip[k]) *dir = !*dir;
    return g_pc.ret[k];
}
float __fastcall FakePcHeading(uint8_t* o, int, int32_t fwd) {
    Logf("heading %p %d", o, fwd);
    return g_pc.head[((uintptr_t)o / 0x10 + fwd) % 8];
}
double __fastcall FakePcBlend(uint8_t* e, int, float t) {
    Logf("blend %p %08x", e, B(t));
    return (double)t * 37.0 + ((uintptr_t)e & 0xff) * 0.25;
}
int32_t FakePcMp() { return g_pc.mp; }
int32_t __stdcall FakePcVer(float v) { Logf("version %08x", B(v)); return g_pc.ver; }
void __fastcall RecPcMove(uint8_t* o, int, float x, float z, float y) { Logf("move %08x %08x %08x", B(x), B(z), B(y)); }
void __fastcall RecPcAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) { Logf("angles %08x %08x %08x", a, b, c); }
void __fastcall RecPcPart(uint8_t* o, int, int32_t p, uint32_t v) { Logf("part %d %08x", p, v); }
void __fastcall RecPcRoll(uint8_t* o, int, float d) { Logf("roll %08x", B(d)); }
int32_t __fastcall FakePcContains(void* m, int, float x, float y) { Logf("contains %08x %08x", B(x), B(y)); return g_pc.contains; }
int32_t __fastcall FakePcHidden(uint8_t* c) { return g_pc.hidden; }
void* g_pcVtbl[0x10];
} // namespace

DIFFTEST(game_CarUpdatePose) {
    SavedGlobals sg({ 0x6cfca8, 0x6cec78, 0x62c120 });
    UiTrace t;
    t.Add(0x48edf0, (void*)&rt3::Node_Advance, (void*)&FakePcAdvance);
    t.Add(0x48bb40, (void*)&rt3::Vehicle_ScreenHeading, (void*)&FakePcHeading);
    t.Add(0x539f90, (void*)&rt3::Effect_Blend, (void*)&FakePcBlend);
    t.Add(0x434050, (void*)&rt3::IsMultiplayer, (void*)&FakePcMp);
    t.Add(0x46d2b0, (void*)&rt3::Net_AllHaveVersion, (void*)&FakePcVer);
    t.Add(0x4556f0, (void*)&rt3::Obj_MoveToCell, (void*)&RecPcMove);
    t.Add(0x45c470, (void*)&rt3::Model_SetAngles, (void*)&RecPcAngles);
    t.Add(0x45c4d0, (void*)&rt3::Model_SetPartAngle2, (void*)&RecPcPart);
    t.Add(0x45c990, (void*)&rt3::Vehicle_RollWheels, (void*)&RecPcRoll);
    t.Add(0x414bd0, (void*)&rt3::Map_Contains, (void*)&FakePcContains);
    static uint8_t car[0x400], world[0x100];
    g_pcVtbl[0x14 / 4] = (void*)&FakePcHidden;
    for (int it = 0; it < 6000; ++it) {
        FakeTable nodes(6, 0x260, [](uint8_t* r, int) {
            r[0x218] = (uint8_t)(Rnd() % 5);
            r[0x201] = (uint8_t)Rnd();
        });
        g_pcNodes = &nodes.c;
        hook::Global<Collection*>(0x6cfca8) = &nodes.c;
        hook::Global<uint8_t*>(0x6cec78) = Rnd() % 4 ? world : nullptr;
        auto id = [&](int k) { return (int32_t)rt3::Collection_IdAt(&nodes.c, 0, k, 0); };
        for (int k = 0; k < 6; ++k) {
            uint8_t* r = (uint8_t*)rt3::Collection_Get(&nodes.c, 0, id(k));
            *(int32_t*)(r + 0x202) = id(k);
            for (int j = 0; j < 3; ++j) *(int32_t*)(r + 0x206 + j * 4) = Rnd() % 5 == 0 ? -1 : id((int)(Rnd() % 6));
        }
        memset(car, 0, sizeof car);
        *(void***)car = g_pcVtbl;
        *(int32_t*)(car + 0x356) = Rnd() % 10 == 0 ? 999 : id((int)(Rnd() % 6));
        *(int32_t*)(car + 0x35a) = Rnd() % 2;
        *(float*)(car + 0x35e) = RndF(0.0f, 0.362f);
        *(float*)(car + 0x242) = RndF(0.0f, 30.0f);
        *(float*)(car + 0x246) = RndF(0.0f, 30.0f);
        *(int32_t*)(car + 0x31a) = (int32_t)(Rnd() % 4);
        for (int k = 0; k < 2; ++k) {
            g_pc.ret[k] = Rnd() % 8 == 0 ? -1 : id((int)(Rnd() % 6));
            g_pc.flip[k] = Rnd() % 2;
            g_pc.x[k] = RndF(0.0f, 300.0f);
            g_pc.z[k] = RndF(0.0f, 300.0f);
            g_pc.at[k] = Rnd() % 3 ? RndF(0.0f, 0.362f) : (Rnd() % 2 ? 0.181f : 0.0f);
        }
        for (int k = 0; k < 8; ++k) g_pc.head[k] = Rnd() % 4 == 0 ? g_pc.head[(k + 7) % 8] : RndF(-7.0f, 7.0f);
        g_pc.hidden = Rnd() % 10 == 0;
        g_pc.mp = Rnd() % 2;
        g_pc.ver = Rnd() % 2;
        g_pc.contains = Rnd() % 3 != 0;
        float dist = RndF(0.0f, 2.0f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            g_pcCall = 0;
            logs[side] = t.Run([&] { side ? rt3::Car_UpdatePose(car, 0, dist) : ((void(__fastcall*)(uint8_t*, int, float))0x41b080)(car, 0, dist); });
        }
        CompareLogs(logs[0], logs[1], "car pose", it);
    }
}

namespace rt3 {
void __fastcall Camera_Update(uint8_t* c);
void __fastcall View_AccumulateMotion(uint8_t* o);
double __fastcall SceneObj_GroundHeight(uint8_t* o);
void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v);
void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Camera_UpdateViewSettings(uint8_t* c);
}

namespace {
float g_cuGround;
void __fastcall RecCuMotion(uint8_t* o) { Logf("motion"); }
double __fastcall FakeCuGround(uint8_t* o) { Logf("ground"); return g_cuGround; }
void __fastcall RecCuLayer(uint8_t* r, int, int32_t i, float v) { Logf("layer %d %08x", i, B(v)); }
void __fastcall RecCuHaze(uint8_t* c, int, float h) { Logf("haze %08x", B(h)); }
int32_t __fastcall FakeCuWater(uint8_t* r, int, int32_t x, int32_t y) { Logf("water %d %d", x, y); return (x * 3 + y) % 4 == 0; }
void __fastcall RecCuView(uint8_t* c) { Logf("view settings"); }
void __fastcall RecCuStep(uint8_t* c) { Logf("step"); }
void* g_cuVtbl[4] = { (void*)&RecCuStep };
} // namespace

DIFFTEST(game_CameraUpdate) {
    SavedGlobals sg({ 0x6cec74, 0x62c120 });
    UiTrace t;
    t.Add(0x439be0, (void*)&rt3::View_AccumulateMotion, (void*)&RecCuMotion);
    t.Add(0x52cec0, (void*)&rt3::SceneObj_GroundHeight, (void*)&FakeCuGround);
    t.Add(0x533100, (void*)&rt3::Render_SetLayerValue, (void*)&RecCuLayer);
    t.Add(0x43a940, (void*)&rt3::Camera_UpdateHaze, (void*)&RecCuHaze);
    t.Add(0x534e50, (void*)&rt3::Terrain_KindIs1Or4, (void*)&FakeCuWater);
    t.Add(0x43b350, (void*)&rt3::Camera_UpdateViewSettings, (void*)&RecCuView);
    static uint8_t cam[0x400], opts[0x200], map[0x2200];
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x62c120) = map;
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x400];
        memset(start, 0, sizeof start);
        *(void***)start = g_cuVtbl;
        start[0x64] = Rnd() % 20 == 0;
        for (uint32_t o = 4; o <= 0x24; o += 4) *(float*)(start + o) = RndF(-500.0f, 20000.0f);
        *(float*)(start + 0x10) = RndF(-15.0f, 15.0f);
        *(float*)(start + 0x14) = Rnd() % 2 ? RndF(-0.5f, 1.5f) : RndF(0.0f, 1.0f);
        *(float*)(start + 0x18) = RndF(0.0f, 300.0f);
        for (uint32_t o = 0x38; o <= 0x4c; o += 4) {
            float cur = *(float*)(start + 4 + (o - 0x38));
            *(float*)(start + o) = Rnd() % 3 == 0 ? cur : cur + RndF(-50.0f, 50.0f);
        }
        if (Rnd() % 4 == 0) *(float*)(start + 0x40) = *(float*)(start + 0xc);
        *(float*)(start + 0x44) = RndF(-15.0f, 15.0f);
        *(float*)(start + 0x48) = RndF(-0.5f, 1.5f);
        *(float*)(start + 0x4c) = RndF(0.0f, 300.0f);
        for (uint32_t o = 0x72; o <= 0x86; o += 4) *(float*)(start + o) = RndF(-100.0f, 3000.0f);
        *(int32_t*)(start + 0xa2) = Rnd() % 2;
        *(float*)(start + 0x38a) = RndF(0.0f, 3000.0f);
        *(float*)(start + 0x20) = Rnd() % 2 ? RndF(0.0f, 300.0f) : RndF(0.0f, 4000.0f);
        g_cuGround = RndF(-50.0f, 100.0f);
        *(int32_t*)(opts + 0x10c) = (int32_t)(Rnd() % 4);
        for (int k = 4; k <= 5; ++k) *(int32_t*)(opts + 0xac + k * 4) = (int32_t)(Rnd() % 4);
        *(int32_t*)(map + 0x2155) = 100 + (int32_t)(Rnd() % 200);
        *(int32_t*)(map + 0x2159) = 100 + (int32_t)(Rnd() % 200);
        std::string logs[2];
        uint8_t out[2][0x400];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] { side ? rt3::Camera_Update(cam) : ((void(__fastcall*)(uint8_t*))0x43bde0)(cam); });
            memcpy(out[side], cam, sizeof cam);
        }
        CompareLogs(logs[0], logs[1], "camera update", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera update state iteration %d", it);
    }
}

namespace rt3 {
void __fastcall IndustryEditor_BuildCargoPanel(uint8_t* w);
}

DIFFTEST(ui_IndustryEditorCargoPanel) {
    SavedGlobals sg({ 0x6cfe04, 0x62ba8c, 0x6d4020 });
    UiTrace t;
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeTrFind);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecTrDelete);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeContainsName);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeTwString);
    static uint8_t rec[0x40], win[0x100];
    hook::Global<uint8_t*>(0x6cfe04) = rec;
    hook::Global<uint32_t>(0x6d4020) = 0x6d40200;
    for (int it = 0; it < 500; ++it) {
        FakeTable cargo(1 + Rnd() % 4, 0x80, [](uint8_t* r, int i) { sprintf((char*)r + 4, "Cargo%d%s", i, Rnd() % 2 ? "x" : ""); });
        hook::Global<Collection*>(0x62ba8c) = &cargo.c;
        rec[0] = (uint8_t)(Rnd() % 8);
        for (int k = 0; k < 8; ++k) {
            uint32_t r = Rnd() % 10;
            *(int32_t*)(rec + 1 + k * 4) = r < 5 ? (int32_t)rt3::Collection_IdAt(&cargo.c, 0, Rnd() % cargo.c.count, 0) : r < 9 ? 0x384 + (int32_t)(Rnd() % 3) : 77;
        }
        rec[0x21] = (uint8_t)(Rnd() % 8);
        rec[0x28] = (uint8_t)(Rnd() & 0x30);
        g_trFound = Rnd() % 2;
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*))0x4ba3d0)(win); });
        std::string b = t.Run([&] { rt3::IndustryEditor_BuildCargoPanel(win); });
        CompareLogs(a, b, "industry editor cargo panel", it);
    }
}

namespace rt3 {
void __fastcall CompanyStations_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
uint32_t __fastcall Company_NthSite(int32_t* o, int, int32_t n, int32_t activeOnly);
void* __fastcall Obj_Building(void* o);
}

namespace {
int32_t g_csCount;
int32_t __fastcall FakeCsCount(void* c, int, int32_t a) { return a == 1 ? g_csCount : -1; }
uint32_t __fastcall FakeCsNth(int32_t* o, int, int32_t n, int32_t a) {
    Collection* st = hook::Global<Collection*>(0x6cec20);
    int32_t cnt = rt3::Collection_Count(st);
    return rt3::Collection_IdAt(st, 0, ((n - 1) * 7 + a) % cnt, 0);
}
void* __fastcall FakeCsBuilding(void* o) { return *(void**)((uint8_t*)o + 0x5f0); }
} // namespace

DIFFTEST(ui_CompanyStations) {
    SavedGlobals sg({ 0x62be10, 0x6cec20, 0x6d3c84 });
    UiTrace t;
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x426590, (void*)&rt3::Company_CountCargoSites, (void*)&FakeCsCount);
    t.Add(0x426620, (void*)&rt3::Company_NthSite, (void*)&FakeCsNth);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeCsBuilding);
    static uint8_t panel[0x1000], bld[40][0x400];
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    for (int it = 0; it < 400; ++it) {
        int32_t ns = 1 + Rnd() % 30;
        FakeTable stations(ns, 0x600, [](uint8_t* r, int i) {
            *(uint32_t*)r = 0x1000 + i;
            sprintf((char*)r + 0x46b, "Station %d", (int)(Rnd() % 12));
            for (uint32_t o = 0x4c4; o < 0x560; o += 4) *(float*)(r + o) = Rnd() % 10 == 0 ? RndF(-2e6f, 0.0f) : RndF(-500.0f, 50000.0f);
            *(int32_t*)(bld[i] + 0x3d5) = (int32_t)(Rnd() % 20000) - 100;
            *(uint32_t*)(bld[i] + 0x3cc) = Rnd() % 3 == 0 ? 0x80000000u + Rnd() : Rnd() % 5000;
            *(void**)(r + 0x5f0) = bld[i];
        });
        FakeTable companies(3, 0x100, [](uint8_t* r, int i) { r[0] = (uint8_t)i; });
        hook::Global<Collection*>(0x62be10) = &companies.c;
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        *(uint32_t*)(panel + 0x88) = Rnd() % 10 == 0 ? 0x7777 : rt3::Collection_IdAt(&companies.c, 0, Rnd() % 3, 0);
        g_csCount = Rnd() % 10 == 0 ? 0 : 1 + Rnd() % (ns + 5);
        if (g_csCount > 199) g_csCount = 199;
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 6);
        panel[0x760] = Rnd() % 2;
        *(int32_t*)(panel + 0x7c) = (int32_t)(Rnd() % 3);
        *(int32_t*)(panel + 0x98) = 0x3f48 + (int32_t)(Rnd() % 10);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        int32_t key = (int32_t)(Rnd() % 7);
        uint8_t out[2][0x640];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e8bb0)(panel, 0, x, y, w, h, key); });
        memcpy(out[0], panel + 0x11c, 0x640);
        std::string b = t.Run([&] { rt3::CompanyStations_Draw(panel, 0, x, y, w, h, key); });
        memcpy(out[1], panel + 0x11c, 0x640);
        CompareLogs(a, b, "company stations", it);
        EXPECT_EQ(memcmp(out[0], out[1], 0x640), 0, "company stations slots iteration %d", it);
    }
}

namespace rt3 {
float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom);
void __fastcall Camera_KeepAboveGround(uint8_t* c);
void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget);
void __fastcall Camera_SmoothHeight(uint8_t* c);
void __fastcall Camera_ClampToGround(uint8_t* c);
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
void __fastcall Camera_UpdateEye(uint8_t* c);
void __fastcall Camera_Turn(uint8_t* c, int, float d);
void __fastcall Camera_MoveTo(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall Camera_SetAngles(uint8_t* c, int, uint32_t yaw, float pitch, uint32_t zoom);
void __fastcall Camera_Orbit(uint8_t* c, int, float d);
void __fastcall Camera_SnapTurn(uint8_t* c, int, int32_t dir);
void __fastcall TrainOrders_RefreshOwner(uint8_t* o);
void __fastcall Train_RefreshWindowIfShown(uint8_t* t);
void __fastcall TrainList_Refill(uint8_t* w);
}

namespace {
float __fastcall FakeCsPitch(uint8_t* c, int, float z) { Logf("pitch %08x", B(z)); return z * 0.01f + 0.1f; }
void __fastcall RecCsAbove(uint8_t* c) { Logf("above %08x %08x %08x", B(*(float*)(c + 0x10)), B(*(float*)(c + 0x14)), B(*(float*)(c + 0x18))); }
void __fastcall RecCsClampMap(uint8_t* c, int, int32_t k) { Logf("clampmap %d %08x %08x", k, B(*(float*)(c + 4)), B(*(float*)(c + 0xc))); }
void __fastcall RecCsSmooth(uint8_t* c) { Logf("smooth %08x", B(*(float*)(c + 8))); }
void __fastcall RecCsGround(uint8_t* c) { Logf("ground"); }
void __fastcall RecCsTarget(uint8_t* c, int, float x, float y, float z) { Logf("target %08x %08x %08x", B(x), B(y), B(z)); }
void __fastcall RecCsEye(uint8_t* c) { Logf("eye %08x", B(*(float*)(c + 0x106))); }
void __fastcall RecCsUpdate(uint8_t* c) { Logf("update %08x", B(*(float*)(c + 0x10))); }
int32_t g_csMaxZoom;
int32_t __fastcall FakeCsMaxZoom(uint8_t* c) { Logf("maxzoom"); return g_csMaxZoom; }
void* g_csVtbl[4] = { nullptr, nullptr, nullptr, (void*)&FakeCsMaxZoom };
void __fastcall RecCsTrainWin(uint8_t* t) { Logf("trainwin %p", t); }
void __fastcall RecCsRefill(uint8_t* w) { Logf("refill %p", w); }
} // namespace

DIFFTEST(game_CameraSetters) {
    UiTrace t;
    t.Add(0x43a750, (void*)&rt3::Camera_PitchForZoom, (void*)&FakeCsPitch);
    t.Add(0x52d640, (void*)&rt3::Camera_KeepAboveGround, (void*)&RecCsAbove);
    t.Add(0x43b0e0, (void*)&rt3::Camera_ClampToMap, (void*)&RecCsClampMap);
    t.Add(0x43cec0, (void*)&rt3::Camera_SmoothHeight, (void*)&RecCsSmooth);
    t.Add(0x439820, (void*)&rt3::Camera_ClampToGround, (void*)&RecCsGround);
    t.Add(0x52d1e0, (void*)&rt3::Camera_SetTarget, (void*)&RecCsTarget);
    t.Add(0x52d400, (void*)&rt3::Camera_UpdateEye, (void*)&RecCsEye);
    t.Add(0x43bde0, (void*)&rt3::Camera_Update, (void*)&RecCsUpdate);
    static uint8_t cam[0x200];
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x200];
        memset(start, 0, sizeof start);
        *(void***)start = g_csVtbl;
        for (uint32_t o = 4; o < 0x1f0; o += 4) *(float*)(start + o) = RndF(-10.0f, 10.0f);
        *(float*)(start + 0x18) = Rnd() % 3 == 0 ? RndF(0.0f, 60.0f) : RndF(30.0f, 3000.0f);
        *(int32_t*)(start + 0xea) = Rnd() % 4 != 0;
        for (uint32_t o : { 0xeeu, 0xf2u, 0xf6u, 0xfau, 0x106u, 0x10au }) *(float*)(start + o) = RndF(-100.0f, 100.0f);
        g_csMaxZoom = 30 + (int32_t)(Rnd() % 3000);
        int which = (int)(Rnd() % 5);
        float f1 = Rnd() % 4 == 0 ? RndF(-0.01f, 0.01f) : RndF(-10.0f, 10.0f);
        if (Rnd() % 20 == 0) f1 = (float)(Rnd() % 9 - 4);
        uint32_t u1 = B(RndF(-5000.0f, 5000.0f)), u2 = B(RndF(-5000.0f, 5000.0f)), u3 = B(RndF(-5000.0f, 5000.0f));
        if (which == 2) {
            if (Rnd() % 3 == 0) u1 = B(*(float*)(start + 0x10));
            if (Rnd() % 2 == 0) u3 = 0xbf800000;
            else if (Rnd() % 2 == 0) u3 = B(RndF(0.0f, 4000.0f));
        }
        int32_t dir = (int32_t)(Rnd() % 5) - 2;
        std::string logs[2];
        uint8_t out[2][0x200];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] {
                switch (which) {
                case 0: side ? rt3::Camera_Turn(cam, 0, f1) : ((void(__fastcall*)(uint8_t*, int, float))0x43c810)(cam, 0, f1); break;
                case 1: side ? rt3::Camera_MoveTo(cam, 0, u1, u2, u3) : ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t))0x43d050)(cam, 0, u1, u2, u3); break;
                case 2: side ? rt3::Camera_SetAngles(cam, 0, u1, f1, u3) : ((void(__fastcall*)(uint8_t*, int, uint32_t, float, uint32_t))0x43c610)(cam, 0, u1, f1, u3); break;
                case 3: side ? rt3::Camera_Orbit(cam, 0, f1) : ((void(__fastcall*)(uint8_t*, int, float))0x43c860)(cam, 0, f1); break;
                default: side ? rt3::Camera_SnapTurn(cam, 0, dir) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x43cb00)(cam, 0, dir); break;
                }
            });
            memcpy(out[side], cam, sizeof cam);
        }
        CompareLogs(logs[0], logs[1], "camera setters", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera setters %d state iteration %d", which, it);
    }
}

DIFFTEST(ui_TrainOrdersRefreshOwner) {
    SavedGlobals sg({ 0x6cfcbc, 0x6d3ac8 });
    UiTrace t;
    t.Add(0x4a8370, (void*)&rt3::Train_RefreshWindowIfShown, (void*)&RecCsTrainWin);
    t.Add(0x512080, (void*)&rt3::TrainList_Refill, (void*)&RecCsRefill);
    FakeTable trains(4, 0x100, [](uint8_t* r, int i) { r[4] = (uint8_t)i; });
    hook::Global<Collection*>(0x6cfcbc) = &trains.c;
    static uint8_t list[0x100];
    for (int it = 0; it < 200; ++it) {
        uint8_t* o = (uint8_t*)(Rnd() % 2 ? 0x6d3ae8 : (uintptr_t)list);
        uint32_t saved = *(uint32_t*)(o + 0xd);
        int k = (int)(Rnd() % 3);
        *(uint32_t*)(o + 0xd) = k == 0 ? 0 : k == 1 ? 0x9999 : rt3::Collection_IdAt(&trains.c, 0, Rnd() % 4, 0);
        hook::Global<uint8_t*>(0x6d3ac8) = Rnd() % 3 ? list : nullptr;
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*))0x4b2ea0)(o); });
        std::string b = t.Run([&] { rt3::TrainOrders_RefreshOwner(o); });
        *(uint32_t*)(o + 0xd) = saved;
        CompareLogs(a, b, "train orders owner", it);
    }
}

namespace rt3 {
void __fastcall IndustryEditor_ShowTab(uint8_t* w, int, int32_t tab);
void __fastcall TrainOrders_UpdateButtons(uint8_t* w);
void __fastcall IndustryEditor_BuildCargoPanel(uint8_t* w);
}

namespace {
void __fastcall RecIeButtons(uint8_t* w) { Logf("buttons"); }
void __fastcall RecIeCargo(uint8_t* w) { Logf("cargo panel"); }
} // namespace

DIFFTEST(ui_IndustryEditorShowTab) {
    SavedGlobals sg({ 0x6cfe04 });
    UiTrace t;
    t.Add(0x4b9a20, (void*)&rt3::TrainOrders_UpdateButtons, (void*)&RecIeButtons);
    t.Add(0x4ba3d0, (void*)&rt3::IndustryEditor_BuildCargoPanel, (void*)&RecIeCargo);
    static uint8_t rec[0x40], win[0x100];
    hook::Global<uint8_t*>(0x6cfe04) = rec;
    for (int it = 0; it < 300; ++it) {
        for (auto& b : rec) b = (uint8_t)Rnd();
        int32_t tab = (int32_t)(Rnd() % 3);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, int32_t))0x4bad20)(win, 0, tab); });
        std::string b = t.Run([&] { rt3::IndustryEditor_ShowTab(win, 0, tab); });
        CompareLogs(a, b, "industry editor tab", it);
    }
}

namespace rt3 {
void __fastcall Camera_ApplyMomentum(uint8_t* c, int, float dt, int32_t move);
void __fastcall Camera_UpdateTarget(uint8_t* c);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
}

namespace {
void __fastcall RecCmTarget(uint8_t* c) { Logf("target %08x %08x %08x", B(*(float*)(c + 4)), B(*(float*)(c + 8)), B(*(float*)(c + 0xc))); }
double __fastcall FakeCmHeight(uint8_t* r, int, float x, float z) { Logf("height %p %08x %08x", r, B(x), B(z)); return x * 0.25 - z * 0.125; }
void __fastcall FakeCmFollow(uint8_t* o, int, float* x, float* y, float* z) {
    Logf("follow %08x %08x %08x", B(*x), B(*y), B(*z));
    *x = 12.5f;
    *y = 3.25f;
    *z = -7.0f;
}
void* g_cmVtbl[3] = { nullptr, nullptr, (void*)&FakeCmFollow };
} // namespace

DIFFTEST(game_CameraMomentum) {
    SavedGlobals sg({ 0x6cec74 });
    UiTrace t;
    t.Add(0x52d640, (void*)&rt3::Camera_KeepAboveGround, (void*)&RecCsAbove);
    t.Add(0x43b0e0, (void*)&rt3::Camera_ClampToMap, (void*)&RecCsClampMap);
    t.Add(0x439820, (void*)&rt3::Camera_ClampToGround, (void*)&RecCsGround);
    t.Add(0x52d450, (void*)&rt3::Camera_UpdateTarget, (void*)&RecCmTarget);
    t.Add(0x534490, (void*)&rt3::Terrain_HeightAt, (void*)&FakeCmHeight);
    t.Add(0x43bde0, (void*)&rt3::Camera_Update, (void*)&RecCsUpdate);
    static uint8_t cam[0x200], opts[0x200], follow[0x10];
    *(void***)follow = g_cmVtbl;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x200];
        memset(start, 0, sizeof start);
        for (uint32_t o = 4; o < 0x1f0; o += 4) *(float*)(start + o) = RndF(-2.0f, 2.0f);
        *(float*)(start + 0x18) = Rnd() % 4 == 0 ? RndF(0.0f, 60.0f) : RndF(30.0f, 40000.0f);
        for (uint32_t o : { 0x8au, 0x8eu, 0x92u, 0x96u, 0x9au }) *(float*)(start + o) = RndF(-50.0f, 50.0f);
        *(uint8_t**)(start + 0x60) = Rnd() % 2 ? follow : nullptr;
        *(uint32_t*)(start + 0x6e) = 0x6e0000;
        *(int32_t*)(opts + 0x10c) = Rnd() % 3 == 0 ? 4 : (int32_t)(Rnd() % 4);
        *(int32_t*)(opts + 0x104) = Rnd() % 2;
        float dt = RndF(0.0f, 0.2f);
        int32_t move = Rnd() % 2;
        std::string logs[2];
        uint8_t out[2][0x200];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] { side ? rt3::Camera_ApplyMomentum(cam, 0, dt, move) : ((void(__fastcall*)(uint8_t*, int, float, int32_t))0x43e2c0)(cam, 0, dt, move); });
            memcpy(out[side], cam, sizeof cam);
        }
        CompareLogs(logs[0], logs[1], "camera momentum", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera momentum state iteration %d", it);
    }
}

namespace rt3 {
void __fastcall CompanyIndustries_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
int32_t __fastcall Company_CountIdleBuildings(void* c);
void __fastcall Company_CacheBuildings(int32_t* o);
void __stdcall SetGlobal62bae8(uint8_t v);
uint32_t __fastcall Company_NthBuilding(int32_t* o, int, int32_t n);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
int32_t __fastcall Obj_StateKindIs4(void* o);
}

namespace {
int32_t g_ciCount;
int32_t __fastcall FakeCiCount(void* c) { Logf("count"); return g_ciCount; }
void __fastcall RecCiCache(int32_t* o) { Logf("cache"); }
void __stdcall RecCiFlag(uint8_t v) { Logf("flag %d", v); }
uint32_t __fastcall FakeCiNth(int32_t* o, int, int32_t n) {
    Collection* b = hook::Global<Collection*>(0x62b26c);
    return rt3::Collection_IdAt(b, 0, (n * 5) % rt3::Collection_Count(b), 0);
}
double __fastcall FakeCiRemaining(void* o, int, int32_t i) { return *(float*)((uint8_t*)o + 0x200) * 3.0 + i; }
int32_t __fastcall FakeCiIdle(void* o) { return ((uint8_t*)o)[0x204] & 1; }
uint8_t g_ciTypes[4][0x100];
uint8_t* __fastcall FakeCiType(uint8_t* b) { return g_ciTypes[b[0x205] & 3]; }
void* g_ciVtbl[0x21];
} // namespace

DIFFTEST(ui_CompanyIndustries) {
    SavedGlobals sg({ 0x62be10, 0x62b26c, 0x6d3c84 });
    UiTrace t;
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x4266c0, (void*)&rt3::Company_CountIdleBuildings, (void*)&FakeCiCount);
    t.Add(0x4267f0, (void*)&rt3::Company_CacheBuildings, (void*)&RecCiCache);
    t.Add(0x423ff0, (void*)&rt3::SetGlobal62bae8, (void*)&RecCiFlag);
    t.Add(0x426740, (void*)&rt3::Company_NthBuilding, (void*)&FakeCiNth);
    t.Add(0x40cac0, (void*)&rt3::Obj_Remaining, (void*)&FakeCiRemaining);
    t.Add(0x40d360, (void*)&rt3::Obj_StateKindIs4, (void*)&FakeCiIdle);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    g_ciVtbl[0x20] = (void*)&FakeCiType;
    for (int k = 0; k < 4; ++k) sprintf((char*)g_ciTypes[k] + 0x4f, "Type %d", k);
    static uint8_t panel[0x1000];
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    for (int it = 0; it < 400; ++it) {
        int32_t nb = 1 + Rnd() % 30;
        FakeTable inds(nb, 0x300, [](uint8_t* r, int i) {
            *(void***)r = g_ciVtbl;
            *(int32_t*)(r + 0x3d5) = (int32_t)(Rnd() % 20000) - 100;
            *(uint32_t*)(r + 0x3cc) = Rnd() % 3 == 0 ? 0x80000000u + Rnd() : Rnd() % 5000;
            *(float*)(r + 0x27a) = RndF(-100.0f, 1000.0f);
            *(float*)(r + 0x200) = RndF(-100.0f, 1000.0f);
            for (uint32_t o = 0x2c0; o < 0x2f0; o += 4) *(float*)(r + o) = RndF(-500.0f, 50000.0f);
            r[0x204] = (uint8_t)Rnd();
            r[0x205] = (uint8_t)Rnd();
            *(uint32_t*)(r + 0x2a4) = 0x2a40000 + i;
        });
        FakeTable companies(3, 0x100, [](uint8_t* r, int i) { r[0] = (uint8_t)i; });
        hook::Global<Collection*>(0x62be10) = &companies.c;
        hook::Global<Collection*>(0x62b26c) = &inds.c;
        *(uint32_t*)(panel + 0x88) = Rnd() % 10 == 0 ? 0x7777 : rt3::Collection_IdAt(&companies.c, 0, Rnd() % 3, 0);
        g_ciCount = Rnd() % 10 == 0 ? 0 : Rnd() % 20 == 0 ? 450 : 1 + Rnd() % (nb + 5);
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 6);
        panel[0x760] = Rnd() % 2;
        *(int32_t*)(panel + 0x7c) = (int32_t)(Rnd() % 3);
        *(int32_t*)(panel + 0x98) = 0x3f48 + (int32_t)(Rnd() % 10);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        int32_t key = (int32_t)(Rnd() % 7);
        uint8_t out[2][0x640];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e8270)(panel, 0, x, y, w, h, key); });
        memcpy(out[0], panel + 0x11c, 0x640);
        std::string b = t.Run([&] { rt3::CompanyIndustries_Draw(panel, 0, x, y, w, h, key); });
        memcpy(out[1], panel + 0x11c, 0x640);
        CompareLogs(a, b, "company industries", it);
        EXPECT_EQ(memcmp(out[0], out[1], 0x640), 0, "company industries slots iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Newspaper_Fill(uint8_t* w, int, uint8_t* rec);
int32_t __fastcall Widget_TextLines(uint8_t* w);
void __fastcall Widget_SetClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
}

namespace {
uint8_t g_npWidgets[3][0x200];
int32_t g_npLines;
uint8_t* __fastcall FakeNpFind(uint8_t* w, int, int32_t id) { Logf("find %x", id); return g_npWidgets[(id - 0x88ba) % 3]; }
int32_t __fastcall FakeNpLines(uint8_t* w) { Logf("lines"); return 1 + (g_npLines++ * 7) % 9; }
void __fastcall RecNpClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d) { Logf("clip %08x %08x %08x %08x", a, b, c, d); }
void __fastcall RecNpDelete(uint8_t* w, int, int32_t lo, int32_t hi, uint32_t p) { Logf("delete %x %x %x", lo, hi, p); }
} // namespace

DIFFTEST(ui_NewspaperFill) {
    UiTrace t;
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeNpFind);
    t.Add(0x55b8e0, (void*)&rt3::Widget_TextLines, (void*)&FakeNpLines);
    t.Add(0x559f50, (void*)&rt3::Widget_SetClip, (void*)&RecNpClip);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecNpDelete);
    static uint8_t win[0x100], rec[0x200];
    for (int it = 0; it < 300; ++it) {
        for (auto& b : rec) b = (uint8_t)Rnd();
        sprintf((char*)rec + 0x19, "Headline %d", it);
        sprintf((char*)rec + 0x92, "Story text %d", it);
        *(uint16_t*)(rec + 0xc) = (uint16_t)(1800 + Rnd() % 250);
        rec[0xe] = (uint8_t)(Rnd() % 12);
        rec[0xf] = (uint8_t)(Rnd() % 28);
        uint8_t* r = Rnd() % 20 == 0 ? nullptr : rec;
        std::string logs[2];
        uint8_t out[2][0x100];
        for (int side = 0; side < 2; ++side) {
            memset(win, 0, sizeof win);
            if (r) r[0x18] = 0;
            g_npLines = it;
            logs[side] = t.Run([&] { side ? rt3::Newspaper_Fill(win, 0, r) : ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x4f2ea0)(win, 0, r); });
            memcpy(out[side], win, sizeof win);
            if (r) out[side][0] ^= r[0x18];
        }
        CompareLogs(logs[0], logs[1], "newspaper", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "newspaper window iteration %d", it);
    }
}

namespace rt3 {
void __fastcall CompanyWindow_ShowTab(uint8_t* w, int, int32_t tab, int32_t page, int32_t sel);
int32_t __fastcall Company_TrainCount(void* c);
int32_t __fastcall Company_NthHolder(int32_t* o, int, int32_t n, int32_t flag);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
void __fastcall PortraitWindow_Layout(uint8_t* w);
}

namespace {
int32_t g_swCounts[5], g_swHolders;
uint8_t g_swFrame[0x40], g_swPlayer[4][0x40];
void __fastcall RecSwSound(int32_t id, int, uint32_t pan) { Logf("sound %x %08x", id, pan); }
uint8_t* __fastcall FakeSwFind(uint8_t* w, int, int32_t id) { Logf("find %x", id); return g_swFrame; }
int32_t __fastcall FakeSwTrains(void* c) { Logf("trains"); return g_swCounts[0]; }
int32_t __fastcall FakeSwSites(void* c, int, int32_t a) { Logf("sites %d", a); return g_swCounts[1]; }
int32_t __fastcall FakeSwIdle(void* c) { Logf("idle"); return g_swCounts[2]; }
int32_t __fastcall FakeSwPlayers(Collection* t) { Logf("players"); return g_swCounts[3]; }
int32_t __fastcall FakeSwCos(Collection* t) { Logf("companies"); return g_swCounts[4]; }
int32_t __fastcall FakeSwHolder(int32_t* o, int, int32_t n, int32_t f) { Logf("holder %d %d", n, f); return n <= g_swHolders ? n % 4 : -1; }
void* __fastcall FakeSwNth(Collection* t, int, int32_t n) { return g_swPlayer[n & 3]; }
void __fastcall RecSwLayout(uint8_t* w) { Logf("layout"); }
void __fastcall RecSwDelete(uint8_t* w, int, int32_t lo, int32_t hi, uint32_t p) { Logf("delete %x %x %x", lo, hi, p); }
} // namespace

DIFFTEST(ui_CompanyWindowShowTab) {
    SavedGlobals sg({ 0x62be10, 0x6ceb9c, 0x62ba8c, 0x6cec78, 0x625a94, 0x6d10ac });
    UiTrace t;
    t.Add(0x45ea20, (void*)&rt3::Ui_PlaySound, (void*)&RecSwSound);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeSwFind);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecSwDelete);
    t.Add(0x4264c0, (void*)&rt3::Company_TrainCount, (void*)&FakeSwTrains);
    t.Add(0x426590, (void*)&rt3::Company_CountCargoSites, (void*)&FakeSwSites);
    t.Add(0x4266c0, (void*)&rt3::Company_CountIdleBuildings, (void*)&FakeSwIdle);
    t.Add(0x477820, (void*)&rt3::Table_CountField4, (void*)&FakeSwPlayers);
    t.Add(0x429a50, (void*)&rt3::Table_CountField3f, (void*)&FakeSwCos);
    t.Add(0x4245e0, (void*)&rt3::Company_NthHolder, (void*)&FakeSwHolder);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeSwNth);
    t.Add(0x4e1d60, (void*)&rt3::PortraitWindow_Layout, (void*)&RecSwLayout);
    for (int k = 0; k < 4; ++k) {
        *(uint32_t*)g_swPlayer[k] = 0x500 + k;
        sprintf((char*)g_swPlayer[k] + 8, "Player %d", k);
    }
    FakeTable companies(3, 0x200, [](uint8_t* r, int i) { *(uint16_t*)(r + 0x157) = (uint16_t)(1850 + i * 3); });
    FakeTable players(3, 0x200, [](uint8_t* r, int i) { *(uint32_t*)(r + 0x1dd) = 0; });
    static uint8_t world[0x100];
    hook::Global<Collection*>(0x62be10) = &companies.c;
    hook::Global<Collection*>(0x6ceb9c) = &players.c;
    hook::Global<uint8_t*>(0x6cec78) = world;
    for (int i = 0; i < 3; ++i)
        *(uint32_t*)((uint8_t*)rt3::Collection_Get(&players.c, 0, rt3::Collection_IdAt(&players.c, 0, i, 0)) + 0x1dd) = rt3::Collection_IdAt(&companies.c, 0, i, 0);
    static uint8_t start[0x800], cwin[0x800], win[2][0x800];
    for (int it = 0; it < 5000; ++it) {
        int32_t nCargo = (int32_t)(Rnd() % 40);
        FakeTable cg(nCargo, 0x20, [](uint8_t* r, int i) {});
        hook::Global<Collection*>(0x62ba8c) = &cg.c;
        for (auto& c : g_swCounts) c = Rnd() % 6 == 0 ? 0 : Rnd() % 3 == 0 ? (int32_t)(Rnd() % 500) : (int32_t)(Rnd() % 40);
        g_swHolders = (int32_t)(Rnd() % 5);
        hook::Global<uint8_t>(0x6d10ac) = Rnd() % 5 == 0;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1850 + Rnd() % 12);
        *(int32_t*)(world + 0x21) = (int32_t)rt3::Collection_IdAt(&companies.c, 0, 0, 0);
        for (auto& b : start) b = (uint8_t)Rnd();
        *(int32_t*)(start + 0x78) = Rnd() % 8 == 0 ? -1 : (int32_t)(Rnd() % 16);
        *(int32_t*)(start + 0x7c) = Rnd() % 8 == 0 ? -1 : (int32_t)(Rnd() % 6);
        int32_t co = Rnd() % 4;
        *(uint32_t*)(start + 0x88) = co == 3 ? 0x7777 : rt3::Collection_IdAt(&companies.c, 0, co, 0);
        if (Rnd() % 10 == 0) *(uint32_t*)(start + 0x88) = 0;
        *(uint32_t*)(start + 0x8c) = rt3::Collection_IdAt(&players.c, 0, Rnd() % 3, 0);
        *(int32_t*)(start + 0x118) = (int32_t)(Rnd() % 12) - 1;
        for (int k = 0; k < 10; ++k)
            if (Rnd() % 3 == 0) *(int32_t*)(start + 0xac + k * 12) = -1;
        int32_t tab = Rnd() % 6 == 0 ? -1 : Rnd() % 2 ? *(int32_t*)(start + 0x78) : (int32_t)(Rnd() % 16);
        if (tab == -1 && *(int32_t*)(start + 0x78) == -1) tab = 3;
        int32_t page = Rnd() % 4 == 0 ? -1 : (int32_t)(Rnd() % 6);
        int32_t sel = Rnd() % 2 ? -1 : (int32_t)(Rnd() % 3);
        if (tab == 8 || (tab == -1 && *(int32_t*)(start + 0x78) == 8)) sel = sel == -1 ? -1 : (int32_t)rt3::Collection_IdAt(&players.c, 0, sel, 0);
        else if (sel > 0) sel = (int32_t)rt3::Collection_IdAt(&companies.c, 0, sel, 0);
        std::string logs[2];
        uint8_t frame[2][0x40];
        for (int side = 0; side < 2; ++side) {
            memcpy(cwin, start, sizeof start);
            memset(g_swFrame, 0, sizeof g_swFrame);
            hook::Global<int32_t>(0x625a94) = 5;
            logs[side] = t.Run([&] { side ? rt3::CompanyWindow_ShowTab(cwin, 0, tab, page, sel) : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x4e2c10)(cwin, 0, tab, page, sel); });
            memcpy(win[side], cwin, sizeof cwin);
            memcpy(frame[side], g_swFrame, sizeof g_swFrame);
            logs[side] += std::to_string(hook::Global<int32_t>(0x625a94));
        }
        CompareLogs(logs[0], logs[1], "company tab", it);
        EXPECT_EQ(memcmp(win[0], win[1], sizeof start), 0, "company tab window iteration %d", it);
        EXPECT_EQ(memcmp(frame[0], frame[1], sizeof frame[0]), 0, "company tab frame iteration %d", it);
    }
}

namespace rt3 {
void __fastcall TreeView_Draw(uint8_t* w, int, int32_t, int32_t, int32_t, int32_t);
int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
}

namespace {
int32_t g_tvNodes;
int32_t __fastcall FakeTvVisible(uint8_t* w) { return w[0x300]; }
int32_t __fastcall FakeTvNode(uint8_t* w, int, int32_t row) { Logf("node %d", row); return (row * 3) % g_tvNodes; }
void __fastcall RecTvCb(int32_t id, int32_t data, float x, float y, float w, uint32_t h, int32_t sel) {
    Logf("cb %x %x %08x %08x %08x %08x %d", id, data, B(x), B(y), B(w), h, sel);
}
} // namespace

DIFFTEST(ui_TreeViewDraw) {
    SavedGlobals sg({ 0x6d4024 });
    UiTrace t;
    t.Add(0x558670, (void*)&rt3::Widget_IsVisible, (void*)&FakeTvVisible);
    t.Add(0x560300, (void*)&rt3::TreeView_NodeOfRow, (void*)&FakeTvNode);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x54f710, (void*)&rt3::Ui_FillRect, (void*)&RecFillRect);
    t.Add(0x54f9f0, (void*)&rt3::Ui_FrameRect, (void*)&RecFrame);
    static uint8_t screen[0x114240], fonts[8][0x15], win[0x400], rect[0x20], nodes[16][32];
    static char texts[16][16];
    hook::Global<uint8_t*>(0x6d4024) = screen;
    *(uint8_t**)(screen + 0x34) = &fonts[0][0];
    for (int k = 0; k < 16; ++k) sprintf(texts[k], "Node %d", k);
    for (int it = 0; it < 5000; ++it) {
        *(int32_t*)(screen + 0x11421e) = (int32_t)(Rnd() % 8);
        for (int k = 0; k < 8; ++k) {
            *(int32_t*)fonts[k] = Rnd() % 4 == 0 ? 0x500 + (int32_t)(Rnd() % 3) - 1 : 400 + (int32_t)(Rnd() % 1600);
            *(int32_t*)(fonts[k] + 4) = 300 + (int32_t)(Rnd() % 1200);
        }
        g_tvNodes = 1 + (int32_t)(Rnd() % 16);
        for (int k = 0; k < 16; ++k) {
            *(const char**)nodes[k] = texts[k];
            *(int32_t*)(nodes[k] + 8) = 0x800 + k;
            *(uint32_t*)(nodes[k] + 0xc) = Rnd() % 4 == 0 ? 0xff00ff00 : 0;
            *(int32_t*)(nodes[k] + 0x10) = k > 0 && Rnd() % 2;
            *(int32_t*)(nodes[k] + 0x14) = k > 0 ? (int32_t)(Rnd() % k) : 0;
            *(int32_t*)(nodes[k] + 0x18) = Rnd() % 2;
            *(int32_t*)(nodes[k] + 0x1c) = Rnd() % 2;
        }
        for (auto& b : win) b = (uint8_t)Rnd();
        win[0x300] = Rnd() % 10 != 0;
        for (uint32_t o : { 0xbu, 0xfu, 0xa6u, 0xaau, 0xaeu, 0xb2u, 0xb6u, 0x146u, 0x14au, 0x14eu, 0x152u, 0x156u, 0x15au }) *(float*)(win + o) = RndF(0.0f, 0.5f);
        *(float*)(rect + 5) = RndF(0.0f, 0.5f);
        *(float*)(rect + 9) = RndF(0.0f, 0.5f);
        *(uint8_t**)(win + 0x86) = rect;
        *(uint8_t**)(win + 0x176) = &nodes[0][0];
        *(uint32_t*)(win + 0x63) = 0x630000;
        *(uint32_t*)(win + 0x92) = 0x920000;
        *(int32_t*)(win + 0x126) = Rnd() % 4 == 0;
        *(int32_t*)(win + 0x1a8) = Rnd() % 2;
        *(int32_t*)(win + 0x1d6) = Rnd() % 4 == 0 ? -1 : (int32_t)(Rnd() % g_tvNodes);
        *(int32_t*)(win + 0xa2) = (int32_t)(Rnd() % 12);
        *(int32_t*)(win + 0x1de) = (int32_t)(Rnd() % 12);
        *(int32_t*)(win + 0x1e2) = (int32_t)(Rnd() % 5);
        *(int32_t*)(win + 0xe6) = Rnd() % 2;
        *(int32_t*)(win + 0xe2) = Rnd() % 2;
        *(int32_t*)(win + 0x122) = Rnd() % 2;
        *(int32_t*)(win + 0x11e) = Rnd() % 2 ? 0 : 7;
        *(uint32_t*)(win + 0xf6) = Rnd() % 2 ? 0 : 0xffff0000;
        *(void**)(win + 0x1ce) = Rnd() % 4 == 0 ? (void*)&RecTvCb : nullptr;
        win[0x2b] = (uint8_t)(Rnd() % 4);
        win[0x2c] = (uint8_t)(Rnd() % 4);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, int32_t))0x5603e0)(win, 0, 0, 0, 0, 0); });
        std::string b = t.Run([&] { rt3::TreeView_Draw(win, 0, 0, 0, 0, 0); });
        CompareLogs(a, b, "tree view", it);
    }
}

namespace rt3 {
void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom);
void __fastcall Camera_ResetView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall Camera_MoveToCell(uint8_t* c, int, float x, float z);
void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz);
void __fastcall Camera_ZoomStep(uint8_t* c, int, int32_t dir);
void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);
void __fastcall CompanyWindow_StepCompany(uint8_t* w, int, int32_t delta);
int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id);
uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n);
void __fastcall TreeView_Render(uint8_t* w);
}

namespace {
void __fastcall RecC2Angles(uint8_t* c, int, uint32_t yaw, float pitch, uint32_t zoom) { Logf("angles %08x %08x %08x", yaw, B(pitch), zoom); }
void __fastcall RecC2Move(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z) { Logf("move %08x %08x %08x", x, y, z); }
void __fastcall RecC2Setup(uint8_t* c, int, uint32_t t, uint32_t x, uint32_t y, uint32_t z, uint32_t d) { Logf("setup %x %x %x %x %x", t, x, y, z, d); }
void __fastcall RecC2Bounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e) { Logf("bounds %x %x %x %x", a, b, d, e); }
void __fastcall RecC2Notify(uint8_t* c) { Logf("notify"); }
void* g_c2Vtbl[5] = { nullptr, nullptr, nullptr, nullptr, (void*)&RecC2Notify };
} // namespace

DIFFTEST(game_CameraSetters2) {
    SavedGlobals sg({ 0x62be4c, 0x62be50, 0x62be54, 0x62be58, 0x62be5c, 0x62be60, 0x62be64 });
    UiTrace t;
    t.Add(0x43a750, (void*)&rt3::Camera_PitchForZoom, (void*)&FakeCsPitch);
    t.Add(0x52d640, (void*)&rt3::Camera_KeepAboveGround, (void*)&RecCsAbove);
    t.Add(0x439820, (void*)&rt3::Camera_ClampToGround, (void*)&RecCsGround);
    t.Add(0x43bde0, (void*)&rt3::Camera_Update, (void*)&RecCsUpdate);
    t.Add(0x43c610, (void*)&rt3::Camera_SetAngles, (void*)&RecC2Angles);
    t.Add(0x43d050, (void*)&rt3::Camera_MoveTo, (void*)&RecC2Move);
    t.Add(0x52d6f0, (void*)&rt3::Camera_Setup, (void*)&RecC2Setup);
    t.Add(0x52d140, (void*)&rt3::Camera_SetBounds, (void*)&RecC2Bounds);
    t.Add(0x534490, (void*)&rt3::Terrain_HeightAt, (void*)&FakeCmHeight);
    static uint8_t cam[0x200];
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x200];
        memset(start, 0, sizeof start);
        *(void***)start = g_c2Vtbl;
        for (uint32_t o = 4; o < 0x1f0; o += 4) *(float*)(start + o) = RndF(-10.0f, 10.0f);
        *(float*)(start + 0x18) = Rnd() % 3 == 0 ? RndF(-5.0f, 60.0f) : RndF(30.0f, 40000.0f);
        *(uint32_t*)(start + 0x6e) = Rnd() % 4 == 0 ? 0 : 0x6e0000;
        int which = (int)(Rnd() % 5);
        float f1 = RndF(-300.0f, 300.0f), f2 = RndF(-300.0f, 300.0f);
        uint32_t u[5];
        for (auto& v : u) v = Rnd();
        int32_t dir = (int32_t)(Rnd() % 5) - 2;
        std::string logs[2];
        uint8_t out[2][0x200];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] {
                switch (which) {
                case 0: side ? rt3::Camera_SetZoom(cam, 0, u[0]) : ((void(__fastcall*)(uint8_t*, int, uint32_t))0x43c980)(cam, 0, u[0]); break;
                case 1:
                    side ? rt3::Camera_ResetView(cam, 0, u[0], u[1], u[2], u[3], u[4])
                         : ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))0x43cdf0)(cam, 0, u[0], u[1], u[2], u[3], u[4]);
                    break;
                case 2: side ? rt3::Camera_MoveToCell(cam, 0, f1, f2) : ((void(__fastcall*)(uint8_t*, int, float, float))0x43d0c0)(cam, 0, f1, f2); break;
                case 3: side ? rt3::Camera_Pan(cam, 0, f1, f2) : ((void(__fastcall*)(uint8_t*, int, float, float))0x43d130)(cam, 0, f1, f2); break;
                default: side ? rt3::Camera_ZoomStep(cam, 0, dir) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x43cc30)(cam, 0, dir); break;
                }
            });
            memcpy(out[side], cam, sizeof cam);
            for (int k = 0; k < 7; ++k) logs[side] += " " + std::to_string(hook::Global<uint32_t>(0x62be4c + k * 4));
        }
        CompareLogs(logs[0], logs[1], "camera setters 2", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera setters 2 (%d) state iteration %d", which, it);
    }
}

namespace {
int32_t g_scCount;
int32_t __fastcall FakeScCount(Collection* t) { Logf("count"); return g_scCount; }
int32_t __fastcall FakeScRank(Collection* t, int, uint32_t id) { Logf("rank %x", id); return (int32_t)(id % 7); }
uint32_t __fastcall FakeScNth(Collection* t, int, int32_t n) { Logf("nth %d", n); return 0x100 + n; }
void __fastcall RecScShow(uint8_t* w, int, int32_t tab, int32_t page, int32_t sel) { Logf("show %d %d %x", tab, page, sel); }
void __fastcall RecTrDraw(uint8_t* w, int, int32_t a, int32_t b, int32_t c, int32_t d) { Logf("draw %d %d %d %d", a, b, c, d); }
int32_t __fastcall FakeTrVisible(uint8_t* w) { Logf("visible"); return w[0x300]; }
} // namespace

DIFFTEST(ui_CompanyStepAndTreeRender) {
    SavedGlobals sg({ 0x62be10, 0x6d401c });
    UiTrace t;
    t.Add(0x429a50, (void*)&rt3::Table_CountField3f, (void*)&FakeScCount);
    t.Add(0x4299f0, (void*)&rt3::Table_LiveRankOf, (void*)&FakeScRank);
    t.Add(0x429990, (void*)&rt3::Table_NthIdField3f, (void*)&FakeScNth);
    t.Add(0x4e2c10, (void*)&rt3::CompanyWindow_ShowTab, (void*)&RecScShow);
    t.Add(0x5603e0, (void*)&rt3::TreeView_Draw, (void*)&RecTrDraw);
    t.Add(0x558670, (void*)&rt3::Widget_IsVisible, (void*)&FakeTrVisible);
    t.Add(0x54f9f0, (void*)&rt3::Ui_FrameRect, (void*)&RecFrame);
    FakeTable companies(4, 0x100, [](uint8_t* r, int i) {});
    hook::Global<Collection*>(0x62be10) = &companies.c;
    static uint8_t win[0x400], rect[0x20], dbg[0xd00];
    hook::Global<uint8_t*>(0x6d401c) = dbg;
    for (int it = 0; it < 3000; ++it) {
        for (auto& b : win) b = (uint8_t)Rnd();
        g_scCount = (int32_t)(Rnd() % 6);
        *(uint32_t*)(win + 0x88) = Rnd() % 3 == 0 ? 0x7777 : rt3::Collection_IdAt(&companies.c, 0, Rnd() % 4, 0);
        int32_t delta = (int32_t)(Rnd() % 5) - 2;
        uint8_t out[2][0x400], keep[0x400];
        memcpy(keep, win, sizeof win);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, int32_t))0x4e3a00)(win, 0, delta); });
        memcpy(out[0], win, sizeof win);
        memcpy(win, keep, sizeof win);
        std::string b = t.Run([&] { rt3::CompanyWindow_StepCompany(win, 0, delta); });
        memcpy(out[1], win, sizeof win);
        CompareLogs(a, b, "company step", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof win), 0, "company step iteration %d", it);
        for (uint32_t o : { 0xbu, 0xfu, 0x13u, 0x17u, 0x1bu, 0x1fu, 0x23u, 0x27u }) *(float*)(win + o) = RndF(0.0f, 900.0f);
        if (Rnd() % 4 == 0) *(float*)(win + 0x23) = 0.0f;
        *(float*)(rect + 5) = RndF(0.0f, 900.0f);
        *(float*)(rect + 9) = RndF(0.0f, 900.0f);
        *(uint8_t**)(win + 0x86) = rect;
        *(uint32_t*)(dbg + 0xc79) = Rnd() % 2 ? 0 : Rnd();
        a = t.Run([&] { ((void(__fastcall*)(uint8_t*))0x561740)(win); });
        b = t.Run([&] { rt3::TreeView_Render(win); });
        CompareLogs(a, b, "tree render", it);
    }
}

namespace rt3 {
void __fastcall Ledger_Init(uint8_t* w, int, int32_t tab, int32_t year);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
}

namespace {
uint8_t* __fastcall FakeLiFont(uint8_t* p, int, int32_t i) { Logf("font %d", i); return (uint8_t*)0xf0000 + i; }
uint8_t* __fastcall FakeLiNamed(uint8_t* p, int, const char* n) {
    Logf("named %s", n);
    uint32_t h = 0;
    for (const char* s = n; *s; ++s) h = h * 31 + (uint8_t)*s;
    return (uint8_t*)(uintptr_t)(h | 1);
}
int32_t __fastcall FakeLiHas(uint8_t* t, int, const char* n, int32_t f, const char* s) { Logf("has %s %d %p", n, f, s); return strlen(n) % 2; }
void __fastcall RecLiLoad(uint8_t* w, int, float x, float y, const char* n) { Logf("load %08x %08x %s", B(x), B(y), n); }
void __fastcall RecLiShow(uint8_t* w, int, int32_t tab, int32_t page, int32_t sel) { Logf("show %d %d %x %d", tab, page, sel, hook::Global<int32_t>(0xccba14)); }
} // namespace

DIFFTEST(ui_LedgerInit) {
    SavedGlobals sg({ 0x6cec78, 0x6d4020, 0x62ba8c, 0xccba14 });
    UiTrace t;
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeLiFont);
    t.Add(0x53d110, (void*)&rt3::Pack_AcquireNamed, (void*)&FakeLiNamed);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeLiHas);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLiLoad);
    t.Add(0x4e2c10, (void*)&rt3::CompanyWindow_ShowTab, (void*)&RecLiShow);
    static uint8_t pack[0x429c0], world[0x100];
    static uint8_t win[2][0x1000];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    for (int it = 0; it < 200; ++it) {
        FakeTable types(Rnd() % 6, 0x80, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x20, "TypeA%d.imb", i);
            sprintf((char*)r + 0x40, "TypeB%d.imb", i);
        });
        FakeTable cargo(Rnd() % 12, 0x80, [](uint8_t* r, int i) { sprintf((char*)r + 4, "Cargo%d", i * 7); });
        *(Collection**)(pack + 0x429b4) = &types.c;
        hook::Global<Collection*>(0x62ba8c) = &cargo.c;
        hook::Global<uint8_t*>(0x6cec78) = Rnd() % 5 == 0 ? nullptr : world;
        *(int32_t*)(world + 0x21) = (int32_t)Rnd();
        *(int32_t*)(world + 0x25) = (int32_t)Rnd();
        hook::Global<int32_t>(0xccba14) = (int32_t)(Rnd() % 3);
        int32_t tab = Rnd() % 3 == 0 ? (int32_t)(Rnd() % 16) : 0;
        int32_t year = Rnd() % 10 == 0 ? (int32_t)(Rnd() % 100) : 1800 + (int32_t)(Rnd() % 250);
        uint8_t start[0x1000];
        for (auto& b : start) b = (uint8_t)Rnd();
        std::string logs[2];
        static uint8_t cw[0x1000];
        for (int side = 0; side < 2; ++side) {
            memcpy(cw, start, sizeof cw);
            logs[side] = t.Run([&] { side ? rt3::Ledger_Init(cw, 0, tab, year) : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4e4b10)(cw, 0, tab, year); });
            memcpy(win[side], cw, sizeof cw);
            logs[side] += std::to_string(hook::Global<int32_t>(0xccba14));
        }
        CompareLogs(logs[0], logs[1], "ledger init", it);
        EXPECT_EQ(memcmp(win[0], win[1], sizeof cw), 0, "ledger init window iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Camera_Step(uint8_t* c, int, float dt, int32_t move);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
}

namespace {
float g_stFollow[3];
void __fastcall FakeStFollow(uint8_t* o, int, float* x, float* y, float* z) {
    Logf("follow");
    *x = g_stFollow[0];
    *y = g_stFollow[1];
    *z = g_stFollow[2];
}
void* g_stVtbl[3] = { nullptr, nullptr, (void*)&FakeStFollow };
void __fastcall RecStAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    Logf("getangles");
    *a = 1;
    *b = 2;
    *c = 3;
}
void __fastcall RecStOrbit(uint8_t* c, int, float d) { Logf("orbit %08x", B(d)); }
} // namespace

DIFFTEST(game_CameraStep) {
    SavedGlobals sg({ 0x6cec74 });
    UiTrace t;
    t.Add(0x43c860, (void*)&rt3::Camera_Orbit, (void*)&RecStOrbit);
    t.Add(0x43cec0, (void*)&rt3::Camera_SmoothHeight, (void*)&RecCsSmooth);
    t.Add(0x43c610, (void*)&rt3::Camera_SetAngles, (void*)&RecC2Angles);
    t.Add(0x43d050, (void*)&rt3::Camera_MoveTo, (void*)&RecC2Move);
    t.Add(0x43b0e0, (void*)&rt3::Camera_ClampToMap, (void*)&RecCsClampMap);
    t.Add(0x439820, (void*)&rt3::Camera_ClampToGround, (void*)&RecCsGround);
    t.Add(0x43bde0, (void*)&rt3::Camera_Update, (void*)&RecCsUpdate);
    t.Add(0x52e880, (void*)&rt3::SceneObj_GetAngles, (void*)&RecStAngles);
    static uint8_t cam[0x200], opts[0x200], follow[0x10];
    *(void***)follow = g_stVtbl;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x200];
        memset(start, 0, sizeof start);
        for (uint32_t o = 4; o < 0x1f0; o += 4) *(float*)(start + o) = RndF(-2.0f, 2.0f);
        *(float*)(start + 0x18) = Rnd() % 4 == 0 ? RndF(0.0f, 60.0f) : RndF(30.0f, 40000.0f);
        for (uint32_t o : { 0x8au, 0x8eu, 0x92u, 0x96u, 0x9au })
            *(float*)(start + o) = Rnd() % 3 == 0 ? 0.0f : RndF(-50.0f, 50.0f);
        *(int32_t*)(start + 0x9e) = Rnd() % 4 == 0;
        *(uint8_t**)(start + 0x60) = Rnd() % 2 ? follow : nullptr;
        *(int32_t*)(opts + 0x108) = Rnd() % 2;
        for (int k = 0; k < 3; ++k) g_stFollow[k] = *(float*)(start + 4 + k * 4) + (Rnd() % 3 == 0 ? RndF(-0.4f, 0.4f) : RndF(-5.0f, 5.0f));
        float dt = RndF(0.0f, 0.2f);
        int32_t move = Rnd() % 3 != 0;
        std::string logs[2];
        uint8_t out[2][0x200];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] { side ? rt3::Camera_Step(cam, 0, dt, move) : ((void(__fastcall*)(uint8_t*, int, float, int32_t))0x43f050)(cam, 0, dt, move); });
            memcpy(out[side], cam, sizeof cam);
        }
        CompareLogs(logs[0], logs[1], "camera step", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera step state iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Camera_DragPick(uint8_t* c, int, uint32_t x0, uint32_t y0, float x1, float y1, int32_t nested);
double __fastcall Obj_Scene36f9(uint8_t* o);
float __fastcall Obj_ScaledSpanOffset(uint8_t* o, int, float t);
int32_t __fastcall Camera_PickGround(uint8_t* cam, int, float sx, float sy, float height, float* cx, float* cz);
int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy);
}

namespace {
int32_t g_dgFail, g_dgTile;
float g_dgAlt;
double __fastcall FakeDgScene(uint8_t* o) { Logf("scene"); return 1.5; }
float __fastcall FakeDgSpan(uint8_t* o, int, float t) { Logf("span %08x", B(t)); return g_dgAlt; }
int32_t __fastcall FakeDgPick(uint8_t* cam, int, float sx, float sy, float h, float* cx, float* cz) {
    Logf("pick %08x %08x %08x", B(sx), B(sy), B(h));
    uint32_t k = B(sx) ^ (B(sy) * 3) ^ (B(h) * 7);
    if ((k >> 3) % 7 < (uint32_t)g_dgFail) return 0;
    *cx = sx * 3.0f + h * 0.01f;
    *cz = sy * 2.0f - h * 0.02f;
    return 1;
}
int32_t __stdcall FakeDgTile(float tx, float ty, float h, float* ox, float* oy) {
    Logf("tile %08x %08x %08x", B(tx), B(ty), B(h));
    if (!g_dgTile) return 0;
    *ox = tx * 0.001f + 0.5f;
    *oy = ty * 0.001f + 0.5f;
    return 1;
}
} // namespace

DIFFTEST(game_CameraDrag) {
    SavedGlobals sg({ 0x6d4024 });
    UiTrace t;
    t.Add(0x528180, (void*)&rt3::Obj_Scene36f9, (void*)&FakeDgScene);
    t.Add(0x43aef0, (void*)&rt3::Obj_ScaledSpanOffset, (void*)&FakeDgSpan);
    t.Add(0x43d320, (void*)&rt3::Camera_PickGround, (void*)&FakeDgPick);
    t.Add(0x439b20, (void*)&rt3::Map_TileToScreen, (void*)&FakeDgTile);
    t.Add(0x43d050, (void*)&rt3::Camera_MoveTo, (void*)&RecC2Move);
    t.Add(0x43bde0, (void*)&rt3::Camera_Update, (void*)&RecCsUpdate);
    static uint8_t screen[0x40], cam[0x400];
    hook::Global<uint8_t*>(0x6d4024) = screen;
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x400];
        memset(start, 0, sizeof start);
        for (uint32_t o = 4; o < 0x3f0; o += 4) *(float*)(start + o) = RndF(-2.0f, 2.0f);
        for (uint32_t o : { 0x20u, 0x31au }) *(float*)(start + o) = RndF(-100.0f, 300.0f);
        for (uint32_t o : { 0x54u, 0x5cu, 0x326u, 0x32au }) *(float*)(start + o) = RndF(0.0f, 1.0f);
        for (uint32_t o : { 0x31eu, 0x322u }) *(float*)(start + o) = RndF(-200.0f, 200.0f);
        *(int32_t*)(start + 0x312) = Rnd() % 2;
        g_dgFail = (int32_t)(Rnd() % 5);
        g_dgTile = Rnd() % 3 != 0;
        g_dgAlt = Rnd() % 3 == 0 ? RndF(-0.5f, 0.5f) : RndF(0.0f, 500.0f);
        uint32_t x0 = B(RndF(0.0f, 1.0f)), y0 = B(RndF(0.0f, 1.0f));
        float x1 = RndF(0.0f, 1.0f), y1 = RndF(0.0f, 1.0f);
        if (g_dgTile && Rnd() % 2) {
            x1 = *(float*)(start + 0x31e) * 0.001f + 0.5f + RndF(-0.05f, 0.05f);
            y1 = *(float*)(start + 0x322) * 0.001f + 0.5f + RndF(-0.05f, 0.05f);
        }
        int32_t nested = Rnd() % 5 == 0;
        std::string logs[2];
        uint8_t out[2][0x400];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] {
                side ? rt3::Camera_DragPick(cam, 0, x0, y0, x1, y1, nested)
                     : ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, float, float, int32_t))0x43ea40)(cam, 0, x0, y0, x1, y1, nested);
            });
            memcpy(out[side], cam, sizeof cam);
        }
        CompareLogs(logs[0], logs[1], "camera drag", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera drag state iteration %d", it);
    }
}

namespace rt3 {
void __fastcall IndustryEditor_Build(uint8_t* w);
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
}

namespace {
int32_t __fastcall FakeIbReached(void* o) { return ((uint8_t*)o)[0x50] & 1; }
int32_t __fastcall FakeIbStock(uint8_t* o, int, int32_t i, float* stock, float* quota) {
    Logf("stock %d", i);
    if (i % 3 == 0) return 0;
    *stock = (float)(i % 4) * 0.5f;
    *quota = 1.0f;
    return 1;
}
uint8_t g_ibName[0x200];
uint8_t* __fastcall FakeIbFind(uint8_t* w, int, int32_t id) { Logf("find %x", id); return g_ibName; }
void __fastcall RecIbTab(uint8_t* w, int, int32_t tab) { Logf("tab %d", tab); }
} // namespace

DIFFTEST(ui_IndustryEditorBuild) {
    SavedGlobals sg({ 0x6cfe04, 0x6cfe08, 0x62ba8c, 0x6cec20, 0x6d4020, 0x6d3c84, 0x6cfe14 });
    uint8_t keepShown[200];
    memcpy(keepShown, (void*)0x6cfd38, 200);
    UiTrace t;
    t.Add(0x41e220, (void*)&rt3::Obj_YearReached, (void*)&FakeIbReached);
    t.Add(0x47e240, (void*)&rt3::Obj_CargoStockAndQuota, (void*)&FakeIbStock);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeLiHas);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeIbFind);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecSwDelete);
    t.Add(0x4ba3d0, (void*)&rt3::IndustryEditor_BuildCargoPanel, (void*)&RecIeCargo);
    t.Add(0x4b9a20, (void*)&rt3::TrainOrders_UpdateButtons, (void*)&RecIeButtons);
    t.Add(0x4bad20, (void*)&rt3::IndustryEditor_ShowTab, (void*)&RecIbTab);
    static uint8_t rec[0x40], rec2[0x20], pack[0x10], win[0x100];
    hook::Global<uint8_t*>(0x6cfe04) = rec;
    hook::Global<uint8_t*>(0x6cfe08) = rec2;
    hook::Global<uint8_t*>(0x6d4020) = pack;
    for (int it = 0; it < 300; ++it) {
        FakeTable cargo(Rnd() % 50, 0x80, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Cargo%d", i);
            r[0x50] = Rnd() % 4 != 0;
        });
        FakeTable stations(3, 0x500, [](uint8_t* r, int i) { sprintf((char*)r + 0x46b, "Station %d", i); });
        hook::Global<Collection*>(0x62ba8c) = &cargo.c;
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        for (auto& b : rec) b = (uint8_t)Rnd();
        *(uint16_t*)(rec + 0x29) = (uint16_t)(Rnd() % 4 == 0 ? 0x7777 : rt3::Collection_IdAt(&stations.c, 0, Rnd() % 3, 0));
        if (*(uint16_t*)(rec + 0x29) == 0x7777) *(uint16_t*)(rec + 0x29) = (uint16_t)rt3::Collection_IdAt(&stations.c, 0, 0, 0);
        rec2[0xc] = (uint8_t)Rnd();
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 5);
        std::string logs[2];
        uint8_t shown[2][200];
        int32_t counts[2];
        for (int side = 0; side < 2; ++side) {
            memset((void*)0x6cfd38, 0xcc, 200);
            hook::Global<int32_t>(0x6cfe14) = 77;
            logs[side] = t.Run([&] { side ? rt3::IndustryEditor_Build(win) : ((void(__fastcall*)(uint8_t*))0x4baef0)(win); });
            memcpy(shown[side], (void*)0x6cfd38, 200);
            counts[side] = hook::Global<int32_t>(0x6cfe14);
        }
        CompareLogs(logs[0], logs[1], "industry editor build", it);
        EXPECT_EQ(memcmp(shown[0], shown[1], 200), 0, "industry editor shown iteration %d", it);
        EXPECT_EQ(counts[0], counts[1], "industry editor count iteration %d", it);
    }
    memcpy((void*)0x6cfd38, keepShown, 200);
}

namespace rt3 {
void __fastcall CompanyTrains_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
uint32_t __fastcall Company_NthTrain(int32_t* o, int, int32_t n);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);
double __fastcall Obj_Sum3(void* o, int, int32_t i);
uint32_t __fastcall Car_SkinTexture(uint8_t* car);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
}

namespace {
int32_t g_ctCount;
uint8_t g_ctEngines[8][0x200];
int32_t __fastcall FakeCtCount(void* c) { return g_ctCount; }
uint32_t __fastcall FakeCtNth(int32_t* o, int, int32_t n) {
    Collection* t = hook::Global<Collection*>(0x6cfcbc);
    return rt3::Collection_IdAt(t, 0, (n * 3) % rt3::Collection_Count(t), 0);
}
void* __fastcall FakeCtLoco(void* o) { return g_ctEngines[((uint8_t*)o)[0x1f0] & 7]; }
void* __fastcall FakeCtType(void* o) { return o; }
double __fastcall FakeCtSum(void* o, int, int32_t i) { return *(float*)((uint8_t*)o + 0x1f4) * (i + 1) - 50.0; }
uint32_t __fastcall FakeCtSkin(uint8_t* c) { return 0x51000 + (uint32_t)(c - &g_ctEngines[0][0]) / 0x200; }
char* __fastcall FakeCtName(uint8_t* t, int, int32_t n) {
    static char name[32];
    sprintf(name, "Train %d", t[0x1f0]);
    return name;
}
} // namespace

DIFFTEST(ui_CompanyTrains) {
    SavedGlobals sg({ 0x62be10, 0x6cfcbc, 0x6d3c84, 0x6cec78 });
    UiTrace t;
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x4264c0, (void*)&rt3::Company_TrainCount, (void*)&FakeCtCount);
    t.Add(0x426520, (void*)&rt3::Company_NthTrain, (void*)&FakeCtNth);
    t.Add(0x4a77b0, (void*)&rt3::Obj_City41, (void*)&FakeCtLoco);
    t.Add(0x41adb0, (void*)&rt3::Obj_Ref388, (void*)&FakeCtType);
    t.Add(0x4a7270, (void*)&rt3::Obj_Sum3, (void*)&FakeCtSum);
    t.Add(0x41c060, (void*)&rt3::Car_SkinTexture, (void*)&FakeCtSkin);
    t.Add(0x4a9360, (void*)&rt3::Train_DisplayName, (void*)&FakeCtName);
    static uint8_t panel[0x1000], world[0x4b00];
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    hook::Global<uint8_t*>(0x6cec78) = world;
    for (int k = 0; k < 8; ++k) *(int32_t*)(g_ctEngines[k] + 0x126) = (int32_t)(Rnd() % 300);
    for (int it = 0; it < 400; ++it) {
        FakeTable trains(1 + Rnd() % 30, 0x200, [](uint8_t* r, int i) {
            *(int32_t*)(r + 0xd4) = 1830 + (int32_t)(Rnd() % 100);
            for (uint32_t o : { 0xdcu, 0xe0u, 0xe4u, 0x18eu, 0xfdu, 0x101u, 0x105u, 0x1f4u }) *(float*)(r + o) = RndF(-10.0f, 500.0f);
            r[0x1f0] = (uint8_t)Rnd();
        });
        FakeTable companies(3, 0x100, [](uint8_t* r, int i) {});
        hook::Global<Collection*>(0x62be10) = &companies.c;
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        *(uint32_t*)(panel + 0x88) = Rnd() % 10 == 0 ? 0x7777 : rt3::Collection_IdAt(&companies.c, 0, Rnd() % 3, 0);
        g_ctCount = Rnd() % 10 == 0 ? 0 : 1 + (int32_t)(Rnd() % 40);
        g_year = 1850 + (int32_t)(Rnd() % 100);
        *(uint16_t*)(world + 0xd) = (uint16_t)(g_year + Rnd() % 5);
        *(int32_t*)(world + 0x4aeb) = Rnd() % 4 == 0;
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 6);
        panel[0x760] = Rnd() % 2;
        *(int32_t*)(panel + 0x7c) = (int32_t)(Rnd() % 3);
        *(int32_t*)(panel + 0x98) = 0x3f48 + (int32_t)(Rnd() % 10);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        int32_t key = (int32_t)(Rnd() % 11);
        uint8_t out[2][0x640];
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4e7670)(panel, 0, x, y, w, h, key); });
        memcpy(out[0], panel + 0x11c, 0x640);
        std::string b = t.Run([&] { rt3::CompanyTrains_Draw(panel, 0, x, y, w, h, key); });
        memcpy(out[1], panel + 0x11c, 0x640);
        CompareLogs(a, b, "company trains", it);
        EXPECT_EQ(memcmp(out[0], out[1], 0x640), 0, "company trains slots iteration %d", it);
    }
}

namespace rt3 {
void __fastcall LocoDetail_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t);
double __stdcall SpeedFactor(float v);
float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
}

namespace {
float __fastcall FakeLdScale(uint8_t* o, int, uint32_t c, uint32_t p) { Logf("scale %x %x", c, p); return 1.25f + (float)(c % 3); }
float __fastcall FakeLdMul(uint8_t* w, int, int32_t i, uint32_t c, uint32_t p, uint32_t r) { Logf("mul %d %x %x %d", i, c, p, r); return 0.5f + (float)(i % 4) * 0.25f; }
int32_t __fastcall FakeLdBonus(void* o, int, int32_t i, uint32_t c, uint32_t p, uint32_t r) { Logf("bonus %d %x %x %d", i, c, p, r); return (i % 5) - 2; }
double __stdcall FakeLdSpeed(float v) { Logf("speed %08x", B(v)); return 1.0 + v * 0.5; }
float __fastcall FakeLdPay(uint8_t* o, int32_t year, float v, uint32_t c, uint32_t r) { Logf("pay %d %08x %x %d", year, B(v), c, r); return v * 0.01f - 0.5f; }
} // namespace

DIFFTEST(ui_LocoDetail) {
    SavedGlobals sg({ 0x6ada84, 0x6d129c, 0x62ba88, 0x6cec78, 0x6226d8, 0x6226dc, 0x6d12a4, 0x6d12a8, 0x6d12ac });
    UiTrace t;
    t.Add(0x4619c0, (void*)&rt3::Obj_EffectScale, (void*)&FakeLdScale);
    t.Add(0x436590, (void*)&rt3::Effects_Multiplier, (void*)&FakeLdMul);
    t.Add(0x436710, (void*)&rt3::Obj_CargoBonus, (void*)&FakeLdBonus);
    t.Add(0x4a6f20, (void*)&rt3::SpeedFactor, (void*)&FakeLdSpeed);
    t.Add(0x4a7c10, (void*)&rt3::Cargo_DeliveryPayment, (void*)&FakeLdPay);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    static uint8_t world[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint32_t>(0x6d12a4) = 0x12a40000;
    hook::Global<uint32_t>(0x6d12a8) = 0x12a80000;
    hook::Global<uint32_t>(0x6d12ac) = 0x12ac0000;
    for (int it = 0; it < 300; ++it) {
        FakeTable types(4, 0x200, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x45, "Engine %d", i);
            *(float*)(r + 0xfd) = RndF(10.0f, 120.0f);
            *(int32_t*)(r + 0x105) = (int32_t)(Rnd() % 100);
        });
        FakeTable recs(3, 0x100, [&](uint8_t* r, int i) {
            *(uint32_t*)(r + 0x72) = rt3::Collection_IdAt(&types.c, 0, Rnd() % 4, 0);
            *(uint32_t*)(r + 0x76) = rt3::Collection_IdAt(&types.c, 0, Rnd() % 4, 0);
            r[0x7a] = Rnd() % 2;
            *(float*)(r + 8) = RndF(0.0f, 300.0f);
            *(int32_t*)(r + 0xc) = (int32_t)(Rnd() % 14) - 2;
            *(int32_t*)(r + 0x10) = (int32_t)(Rnd() % 3);
            *(int32_t*)(r + 0x14) = (int32_t)(Rnd() % 4);
            *(int32_t*)(r + 0x18) = (int32_t)(Rnd() % 14) - 2;
            *(int32_t*)(r + 0x1c) = (int32_t)(Rnd() % 14) - 2;
        });
        hook::Global<Collection*>(0x62ba88) = &types.c;
        hook::Global<Collection*>(0x6ada84) = &recs.c;
        hook::Global<uint32_t>(0x6d129c) = Rnd() % 10 == 0 ? 0x7777 : rt3::Collection_IdAt(&recs.c, 0, Rnd() % 3, 0);
        hook::Global<uint8_t>(0x6226d8) = Rnd() % 2;
        hook::Global<int32_t>(0x6226dc) = (int32_t)(Rnd() % 3);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1830 + Rnd() % 150);
        *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 9);
        *(int32_t*)(world + 0x25) = (int32_t)(Rnd() % 9);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x4f14d0)(nullptr, 0, x, y, w, h, 0); });
        std::string b = t.Run([&] { rt3::LocoDetail_Draw(nullptr, 0, x, y, w, h, 0); });
        CompareLogs(a, b, "loco detail", it);
    }
}

namespace rt3 {
void __fastcall LocoWindow_Build(uint8_t* w, int, float x, float y, float wd, float);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
}

namespace {
uint8_t g_lwCity[0x400];
void* __fastcall FakeLwCity(void* o) { return g_lwCity; }
uint8_t* __fastcall RecLwButton(uint8_t* w, int, const uint8_t* def) {
    std::string s;
    for (int i = 0; i < 0xa1; ++i) {
        char h[4];
        sprintf(h, "%02x", def[i]);
        s += h;
    }
    Logf("button %s", s.c_str());
    return w;
}
} // namespace

DIFFTEST(ui_LocoWindowBuild) {
    SavedGlobals sg({ 0x6ada84, 0x6d129c, 0x62ba88, 0x6cec78, 0x6226dc, 0x6d12a0, 0x6d4020 });
    UiTrace t;
    t.Add(0x4a77b0, (void*)&rt3::Obj_City41, (void*)&FakeLwCity);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeLiHas);
    t.Add(0x563d70, (void*)&rt3::Button_Construct, (void*)&RecLwButton);
    static uint8_t world[0x100], pack[0x429c0], win[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d4020) = pack;
    for (int it = 0; it < 300; ++it) {
        FakeTable types(3, 0x200, [](uint8_t* r, int i) { sprintf((char*)r + 0x82, "Loco%d", i); });
        FakeTable recs(3, 0x100, [&](uint8_t* r, int i) {
            *(uint32_t*)(r + 0x72) = rt3::Collection_IdAt(&types.c, 0, Rnd() % 3, 0);
            *(int32_t*)(r + 0x10) = (int32_t)(Rnd() % 4);
        });
        FakeTable regions(Rnd() % 4, 0x100, [](uint8_t* r, int i) {
            sprintf((char*)r, "Region%d", i);
            sprintf((char*)r + 0x80, "Variant%d_%%1.imb", i);
        });
        hook::Global<Collection*>(0x62ba88) = &types.c;
        hook::Global<Collection*>(0x6ada84) = &recs.c;
        *(Collection**)(pack + 0x429b8) = &regions.c;
        hook::Global<uint32_t>(0x6d129c) = rt3::Collection_IdAt(&recs.c, 0, Rnd() % 3, 0);
        hook::Global<uint8_t*>(0x6d12a0) = Rnd() % 4 == 0 ? nullptr : world;
        g_lwCity[0x3cc] = Rnd() % 2;
        sprintf((char*)g_lwCity + 0x3ac, "Region%d", (int)(Rnd() % 5));
        hook::Global<int32_t>(0x6226dc) = (int32_t)(Rnd() % 3);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1830 + Rnd() % 150);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), wd = RndF(0.0f, 1.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4f1f90)(win, 0, x, y, wd, 0); });
        std::string b = t.Run([&] { rt3::LocoWindow_Build(win, 0, x, y, wd, 0); });
        CompareLogs(a, b, "loco window", it);
    }
}

namespace rt3 {
void __fastcall PickerWindow_Build(uint8_t* w);
int32_t CountFreeCompanySlots();
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t __fastcall NthFreeCompanySlot(int32_t n);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
double __fastcall Cargo_DemandWeight(void* c);
}

namespace {
int32_t g_pkCos;
int32_t __fastcall FakePkCount(Collection* t) { return g_pkCos; }
uint32_t __fastcall FakePkNth(Collection* t, int, int32_t n) { return rt3::Collection_IdAt(hook::Global<Collection*>(0x62be10), 0, n % rt3::Collection_Count(hook::Global<Collection*>(0x62be10)), 0); }
int32_t FakePkFree() { Logf("free slots"); return 5; }
int32_t __fastcall FakePkRand(int32_t lo, int32_t hi) { Logf("rand %d %d", lo, hi); return hi / 2; }
int32_t __fastcall FakePkSlot(int32_t n) { Logf("slot %d", n); return 40 + n; }
uint32_t __fastcall RecPkSet(uint8_t* w, int, uint32_t m, uint32_t a, uint32_t b) { Logf("set %x %x %x", m, a, b); return 0; }
void __fastcall RecPkClear(uint8_t* w, int, uint32_t m) { Logf("clear %x", m); }
double __fastcall FakePkDemand(void* c) { return *(float*)((uint8_t*)c + 0x60) * 2.0; }
} // namespace

DIFFTEST(ui_PickerWindowBuild) {
    SavedGlobals sg({ 0x62be10, 0x62ba8c, 0x6d4020 });
    UiTrace t;
    t.Add(0x429a50, (void*)&rt3::Table_CountField3f, (void*)&FakePkCount);
    t.Add(0x429990, (void*)&rt3::Table_NthIdField3f, (void*)&FakePkNth);
    t.Add(0x4242a0, (void*)&rt3::CountFreeCompanySlots, (void*)&FakePkFree);
    t.Add(0x518d70, (void*)&rt3::Rand, (void*)&FakePkRand);
    t.Add(0x424430, (void*)&rt3::NthFreeCompanySlot, (void*)&FakePkSlot);
    t.Add(0x558980, (void*)&rt3::Widget_SetFlags, (void*)&RecPkSet);
    t.Add(0x5588b0, (void*)&rt3::Widget_ClearFlags, (void*)&RecPkClear);
    t.Add(0x41e650, (void*)&rt3::Cargo_DemandWeight, (void*)&FakePkDemand);
    t.Add(0x41e220, (void*)&rt3::Obj_YearReached, (void*)&FakeIbReached);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    t.Add(0x53c930, (void*)&rt3::NameTable_ContainsName, (void*)&FakeLiHas);
    t.Add(0x53fe00, (void*)&rt3::Window_DeleteChildren, (void*)&RecSwDelete);
    static uint8_t pack[0x10], win[0x100], bigCargo[0x400];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    for (int it = 0; it < 300; ++it) {
        FakeTable cos(4, 0xe00, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Company %d", i);
            sprintf((char*)r + 0xd65, "Logo%d.imb", i);
            *(int32_t*)(r + 0x37) = Rnd() % 2 ? -1 : (int32_t)(Rnd() % 30);
        });
        FakeTable cargo(Rnd() % 30, 0x80, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Cargo%d", i);
            r[0x50] = Rnd() % 4 != 0;
            *(float*)(r + 0x5a) = RndF(0.0f, 500.0f);
            *(float*)(r + 0x60) = RndF(0.0f, 300.0f);
        });
        memset(bigCargo, 0, sizeof bigCargo);
        memcpy(bigCargo, &cargo.c, sizeof(Collection));
        for (int k = 0; k < 40; ++k) *(int32_t*)(bigCargo + 0x9a + k * 4) = k == 0 || Rnd() % 3 == 0 ? -1 : k;
        *(int32_t*)(bigCargo + 0x242) = (int32_t)(Rnd() % 30);
        hook::Global<Collection*>(0x62be10) = &cos.c;
        hook::Global<Collection*>(0x62ba8c) = (Collection*)bigCargo;
        g_pkCos = (int32_t)(Rnd() % 16);
        uint8_t start[0x100];
        for (auto& b : start) b = (uint8_t)Rnd();
        *(int32_t*)(start + 0x78) = Rnd() % 2 ? 0 : Rnd() % 2 ? 3 : (int32_t)(Rnd() % 5);
        *(uint32_t*)(start + 0x7c) = Rnd() % 3 == 0 ? 0 : Rnd() % 2 ? rt3::Collection_IdAt(&cos.c, 0, Rnd() % 4, 0) : (uint32_t)(Rnd() % 30);
        uint8_t logo[2][4];
        std::string logs[2];
        uint8_t out[2][0x100];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < 4; ++k) *(int32_t*)((uint8_t*)rt3::Collection_Get(&cos.c, 0, rt3::Collection_IdAt(&cos.c, 0, k, 0)) + 0x37) = k % 2 ? -1 : k;
            memcpy(win, start, sizeof win);
            logs[side] = t.Run([&] { side ? rt3::PickerWindow_Build(win) : ((void(__fastcall*)(uint8_t*))0x4f3d90)(win); });
            memcpy(out[side], win, sizeof win);
            for (int k = 0; k < 4; ++k) logo[side][k] = *((uint8_t*)rt3::Collection_Get(&cos.c, 0, rt3::Collection_IdAt(&cos.c, 0, k, 0)) + 0x37);
        }
        CompareLogs(logs[0], logs[1], "picker", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof win), 0, "picker window iteration %d", it);
        EXPECT_EQ(memcmp(logo[0], logo[1], 4), 0, "picker logos iteration %d", it);
    }
}

namespace rt3 {
uint8_t* __fastcall Camera_ConstructView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
uint8_t* __fastcall Camera_Construct(uint8_t* c);
void __fastcall Camera_Reset(uint8_t* c);
void __fastcall Camera_DragToMouse(uint8_t* c);
void __fastcall Camera_MouseInput(uint8_t* c, int, float dx, float dy, float wheel);
int32_t __fastcall Tool_RemoveModifier();
int32_t __fastcall Tool_AddModifier(uint8_t* t);
void __fastcall Camera_UpdateFocus(uint8_t* c);
void __fastcall Camera_Drag(uint8_t* c, int, float dx, float dy, int32_t);
uint8_t* __fastcall IndustryEditor_Construct(uint8_t* w);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall MapScroll_Tick(uint8_t* w);
uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id);
uint32_t __fastcall Input_Flag800();
uint32_t __fastcall Input_Flag2000();
int32_t ClockMs();
int32_t __fastcall Engine_IsForeground();
int32_t __fastcall Cursor_InWindow();
void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz);
}

namespace {
uint8_t* __fastcall FakeMiConstruct(uint8_t* c) { Logf("construct"); *(uint32_t*)c = 0x5dd1c8; return c; }
void __fastcall RecMiReset(uint8_t* c) { Logf("reset %x", *(uint32_t*)c); }
void __fastcall RecMiView(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e, uint32_t f) { Logf("view %x %x %x %x %x", a, b, d, e, f); }
void __fastcall RecMiPick(uint8_t* c, int, uint32_t x0, uint32_t y0, float x1, float y1, int32_t n) { Logf("pick %x %x %08x %08x %d", x0, y0, B(x1), B(y1), n); }
void __fastcall RecMiDrag(uint8_t* c, int, float dx, float dy, int32_t w) { Logf("drag %08x %08x %x", B(dx), B(dy), w); }
void __fastcall RecMiFocus(uint8_t* c) { Logf("focus"); }
int32_t g_miFree, g_miGrab, g_miClock, g_miFlags;
int32_t __fastcall FakeMiFree() { return g_miFree; }
int32_t __fastcall FakeMiGrab(uint8_t* t) { return g_miGrab; }
uint8_t* __fastcall RecMiVtbl(uint8_t* w) { Logf("base"); return w; }
void __fastcall RecMiBuild(uint8_t* w) { Logf("build %x", *(uint32_t*)w); }
uint8_t g_miButton[0x40];
uint8_t* __fastcall FakeMiFind(uint8_t* o, int, int32_t id) { Logf("findchild %x", id); return id & 1 ? g_miButton : nullptr; }
uint32_t __fastcall FakeMiF800() { return g_miFlags & 1; }
uint32_t __fastcall FakeMiF2000() { return g_miFlags & 2; }
int32_t FakeMiClock() { Logf("clock"); return g_miClock; }
int32_t __fastcall FakeMiFg() { return (g_miFlags & 4) == 0; }
int32_t __fastcall FakeMiCursor() { return (g_miFlags & 8) == 0; }
void __fastcall RecMiPan(uint8_t* c, int, float dx, float dz) { Logf("pan %p %08x %08x", c, B(dx), B(dz)); }
} // namespace

DIFFTEST(game_CameraMouseAndScroll) {
    SavedGlobals sg({ 0x6d4018, 0x6d4024, 0x6cec74, 0x6d401c, 0x6d3b4c, 0x62be68 });
    UiTrace t;
    t.Add(0x52d6d0, (void*)&rt3::Camera_Construct, (void*)&FakeMiConstruct);
    t.Add(0x43a5a0, (void*)&rt3::Camera_Reset, (void*)&RecMiReset);
    t.Add(0x43cdf0, (void*)&rt3::Camera_ResetView, (void*)&RecMiView);
    t.Add(0x43ea40, (void*)&rt3::Camera_DragPick, (void*)&RecMiPick);
    t.Add(0x43e830, (void*)&rt3::Camera_Drag, (void*)&RecMiDrag);
    t.Add(0x43d480, (void*)&rt3::Camera_UpdateFocus, (void*)&RecMiFocus);
    t.Add(0x43b030, (void*)&rt3::Tool_RemoveModifier, (void*)&FakeMiFree);
    t.Add(0x43af60, (void*)&rt3::Tool_AddModifier, (void*)&FakeMiGrab);
    t.Add(0x53f7e0, (void*)&rt3::Widget_SetVtblRet, (void*)&RecMiVtbl);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLiLoad);
    t.Add(0x4baef0, (void*)&rt3::IndustryEditor_Build, (void*)&RecMiBuild);
    t.Add(0x538810, (void*)&rt3::Windows_FindChild, (void*)&FakeMiFind);
    t.Add(0x543e40, (void*)&rt3::Input_Flag800, (void*)&FakeMiF800);
    t.Add(0x543e20, (void*)&rt3::Input_Flag2000, (void*)&FakeMiF2000);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeMiClock);
    t.Add(0x54e630, (void*)&rt3::Engine_IsForeground, (void*)&FakeMiFg);
    t.Add(0x54f540, (void*)&rt3::Cursor_InWindow, (void*)&FakeMiCursor);
    t.Add(0x43d130, (void*)&rt3::Camera_Pan, (void*)&RecMiPan);
    static uint8_t screen[0x114240], mouse[0xb00], opts[0x200], ui[0xd00], focus[0x100], cam[0x400], win[0x100];
    hook::Global<uint8_t*>(0x6d4024) = screen;
    hook::Global<uint8_t*>(0x6d4018) = mouse;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    hook::Global<uint8_t*>(0x62be68) = cam;
    for (int it = 0; it < 20000; ++it) {
        int which = (int)(Rnd() % 5);
        uint8_t start[0x400];
        for (auto& b : start) b = (uint8_t)Rnd();
        for (uint32_t o : { 0x92u, 0x32eu, 0x332u }) *(float*)(start + o) = RndF(-5.0f, 5.0f);
        *(float*)(mouse + 0xa94) = RndF(0.0f, 1.0f);
        *(float*)(mouse + 0xa98) = RndF(0.0f, 1.0f);
        screen[0x1146d7] = Rnd() % 2;
        screen[0x114226] = Rnd() % 4 != 0;
        *(int32_t*)(opts + 0x118) = (int32_t)(Rnd() % 4);
        *(int32_t*)(opts + 0x114) = (int32_t)(Rnd() % 4);
        g_miFree = Rnd() % 2;
        g_miGrab = Rnd() % 2;
        g_miFlags = (int32_t)(Rnd() % 16);
        if (Rnd() % 2) g_miFlags = 0;
        g_miClock = (int32_t)(Rnd() % 3000);
        *(uint8_t**)(ui + 0xc6d) = Rnd() % 6 == 0 ? nullptr : focus;
        *(uint16_t*)(focus + 4) = (uint16_t)(Rnd() % 3 == 0 ? Rnd() : 0x7918 + Rnd() % 10);
        *(int32_t*)(focus + 0xd2) = g_miClock - (int32_t)(Rnd() % 1500);
        *(int32_t*)(start + 0x78) = Rnd() % 2 || !*(uint8_t**)(ui + 0xc6d) ? 0 : 0x7918 + (int32_t)(Rnd() % 10);
        hook::Global<int32_t>(0x6d3b4c) = Rnd() % 2;
        float dx = RndF(-3.0f, 3.0f), dy = RndF(-3.0f, 3.0f), wheel = Rnd() % 3 == 0 ? 0.0f : RndF(-2.0f, 2.0f);
        uint32_t u[5];
        for (auto& v : u) v = Rnd();
        std::string logs[2];
        uint8_t out[2][0x400], fout[2][0x100];
        uint8_t focusStart[0x100];
        memcpy(focusStart, focus, sizeof focus);
        for (int side = 0; side < 2; ++side) {
            uint8_t* target = which >= 3 ? win : cam;
            memcpy(target, start, which >= 3 ? sizeof win : sizeof cam);
            memcpy(focus, focusStart, sizeof focus);
            g_miButton[0x33] = 0xff;
            uintptr_t ret = 0;
            logs[side] = t.Run([&] {
                switch (which) {
                case 0: ret = (uintptr_t)(side ? rt3::Camera_ConstructView(cam, 0, u[0], u[1], u[2], u[3], u[4]) : ((uint8_t*(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))0x43d260)(cam, 0, u[0], u[1], u[2], u[3], u[4])); break;
                case 1: side ? rt3::Camera_DragToMouse(cam) : ((void(__fastcall*)(uint8_t*))0x43f4c0)(cam); break;
                case 2: side ? rt3::Camera_MouseInput(cam, 0, dx, dy, wheel) : ((void(__fastcall*)(uint8_t*, int, float, float, float))0x43f4f0)(cam, 0, dx, dy, wheel); break;
                case 3: ret = (uintptr_t)(side ? rt3::IndustryEditor_Construct(win) : ((uint8_t*(__fastcall*)(uint8_t*))0x4bc0a0)(win)); break;
                default: side ? rt3::MapScroll_Tick(win) : ((void(__fastcall*)(uint8_t*))0x4e0780)(win); break;
                }
            });
            logs[side] += " " + std::to_string(ret) + " " + std::to_string(g_miButton[0x33]);
            memcpy(out[side], which >= 3 ? win : cam, which >= 3 ? sizeof win : sizeof cam);
            memcpy(fout[side], focus, sizeof focus);
        }
        CompareLogs(logs[0], logs[1], "camera mouse", it);
        EXPECT_EQ(memcmp(out[0], out[1], which >= 3 ? sizeof win : sizeof cam), 0, "camera mouse (%d) state iteration %d", which, it);
        EXPECT_EQ(memcmp(fout[0], fout[1], sizeof focus), 0, "camera mouse focus iteration %d", it);
    }
}

namespace rt3 {
void __fastcall CompanyOverview_Draw(uint8_t* p, int, float x, float y, float w, float h);
int32_t World_AnyCondition();
void* __fastcall Obj_Ref25(void* o);
uint32_t __fastcall Company_Logo(uint8_t* c);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
void* __fastcall Obj_Company21(void* o);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void* __fastcall Company_Owner(void* c);
int32_t __fastcall Player_OwnsAllOf(uint8_t* p, int, uint32_t id);
int32_t __fastcall Obj_Level34b(void* o);
}

namespace {
uint8_t g_coPlayer[0x300], g_coOwner[0x100];
int32_t g_coCond, g_coAll;
uint8_t* g_coMine;
void* __fastcall FakeCoRef25(void* o) { Logf("ref25"); return g_coPlayer; }
int32_t FakeCoCond() { Logf("cond"); return g_coCond; }
uint32_t __fastcall FakeCoLogo(uint8_t* c) { Logf("logo"); return 0x10a0000; }
double __fastcall FakeCoStepA(uint8_t* s) { Logf("stepA %p", s); return 0.125; }
double __fastcall FakeCoStepB(uint8_t* s) { Logf("stepB %p", s); return 0.0625; }
void* __fastcall FakeCoMine(void* o) { Logf("mine"); return g_coMine; }
double __fastcall FakeCoHeight(uint8_t* f, int, const char* t, float w, float* tw) { Logf("height %s %08x %p", t, B(w), tw); return strlen(t) * 0.001; }
void* __fastcall FakeCoOwner(void* c) { Logf("owner"); return g_coOwner; }
int32_t __fastcall FakeCoAll(uint8_t* p, int, uint32_t id) { Logf("all %x", id); return g_coAll; }
int32_t __fastcall FakeCoLevel(void* o) { Logf("level"); return 2; }
} // namespace

DIFFTEST(ui_CompanyOverview) {
    SavedGlobals sg({ 0x62be10, 0x6cec78 });
    UiTrace t;
    t.Add(0x4e39e0, (void*)&rt3::Obj_YearOrNow, (void*)&FakeYearOrNow);
    t.Add(0x4348c0, (void*)&rt3::Obj_Ref25, (void*)&FakeCoRef25);
    t.Add(0x476a60, (void*)&rt3::World_AnyCondition, (void*)&FakeCoCond);
    t.Add(0x426b10, (void*)&rt3::Company_Logo, (void*)&FakeCoLogo);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeCoStepA);
    t.Add(0x5521a0, (void*)&rt3::Slider_StepB, (void*)&FakeCoStepB);
    t.Add(0x434870, (void*)&rt3::Obj_Company21, (void*)&FakeCoMine);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x550b50, (void*)&rt3::Font_TextHeight, (void*)&FakeCoHeight);
    t.Add(0x426ef0, (void*)&rt3::Company_Owner, (void*)&FakeCoOwner);
    t.Add(0x4768c0, (void*)&rt3::Player_OwnsAllOf, (void*)&FakeCoAll);
    t.Add(0x4240c0, (void*)&rt3::Obj_Level34b, (void*)&FakeCoLevel);
    static uint8_t panel[0x1000], world[0x4b00];
    *(uint32_t*)(panel + 0xe55) = 0xe550000;
    hook::Global<uint8_t*>(0x6cec78) = world;
    sprintf((char*)g_coOwner + 8, "Owner");
    for (int it = 0; it < 2000; ++it) {
        FakeTable cos(3, 0xe00, [](uint8_t* r, int i) {
            sprintf((char*)r + 4, "Co%d", i);
            *(int32_t*)(r + 0x3b) = Rnd() % 3 == 0 ? 0 : (int32_t)(Rnd() % 3);
            *(int32_t*)(r + 0x14f) = (int32_t)(Rnd() % 100000);
            *(int32_t*)(r + 0xd59) = Rnd() % 3 == 0 ? *(int32_t*)(r + 0x14f) : (int32_t)(Rnd() % 100000);
            *(int32_t*)(r + 0x34f) = 1850 + (int32_t)(Rnd() % 3);
            *(int32_t*)(r + 0x353) = (int32_t)(Rnd() % 5000);
        });
        hook::Global<Collection*>(0x62be10) = &cos.c;
        *(uint32_t*)(panel + 0x88) = Rnd() % 5 == 0 ? 0 : rt3::Collection_IdAt(&cos.c, 0, Rnd() % 3, 0);
        g_coMine = Rnd() % 2 && *(uint32_t*)(panel + 0x88) ? (uint8_t*)rt3::Collection_Get(&cos.c, 0, *(uint32_t*)(panel + 0x88)) : nullptr;
        g_coCond = Rnd() % 2;
        g_coAll = Rnd() % 2;
        *(int32_t*)(g_coPlayer + 0x2a7) = Rnd() % 2;
        *(int32_t*)(world + 0x25) = (int32_t)(Rnd() % 3);
        *(int32_t*)(world + 0x2d) = (int32_t)(Rnd() % 4);
        *(int32_t*)(world + 0x4a7f) = Rnd() % 2;
        g_year = 1850 + (int32_t)(Rnd() % 3);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4e5a70)(panel, 0, x, y, w, h); });
        std::string b = t.Run([&] { rt3::CompanyOverview_Draw(panel, 0, x, y, w, h); });
        CompareLogs(a, b, "company overview", it);
    }
}

namespace rt3 {
void __fastcall Minimap_DrawRouteMarkers(uint8_t* m);
void* SelectedTrain();
void __fastcall Engine_ResetTextureQueue(void* e);
float __fastcall Obj_MapY(uint8_t* o, int, float y);
double __fastcall Obj_MapX(uint8_t* o, int, float x);
int32_t __fastcall Obj_BuildingStateActive(void* o);
float __fastcall Obj_NodeCenterX(void* o);
float __fastcall Obj_NodeCenterY(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y);
float __fastcall SceneObj_Angle2(uint8_t* o);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
}

namespace {
void* g_mmSel;
uint8_t g_mmType[2][0x100], g_mmBld[0x10], g_mmLoco[0x10];
void* g_mmBldVtbl[0x21];
void* FakeMmSel() { return g_mmSel; }
void __fastcall RecMmReset(void* e) { Logf("reset texq"); }
float __fastcall FakeMmY(uint8_t* o, int, float y) { Logf("mapy %08x", B(y)); return y * 0.01f + 0.2f; }
double __fastcall FakeMmX(uint8_t* o, int, float x) { Logf("mapx %08x", B(x)); return x * 0.01 + 0.1; }
int32_t __fastcall FakeMmActive(void* o) { return ((uint8_t*)o)[0x40] & 1; }
float __fastcall FakeMmNx(void* o) { return *(float*)((uint8_t*)o + 0x44); }
float __fastcall FakeMmNy(void* o) { return *(float*)((uint8_t*)o + 0x48); }
int32_t __fastcall FakeMmValue(void* o) { return ((uint8_t*)o)[0x4c] & 1; }
void* __fastcall FakeMmBuilding(void* o) { g_mmBld[8] = ((uint8_t*)o)[0x4d] & 1; return g_mmBld; }
uint8_t* __fastcall FakeMmBldType(uint8_t* b) { return g_mmType[b[8]]; }
void __fastcall FakeMmRear(uint8_t* t, int, float* x, float* y) { *x = *(float*)(t + 0x44); *y = *(float*)(t + 0x48); }
float __fastcall FakeMmAngle(uint8_t* o) { return 1.7f; }
void* __fastcall FakeMmLoco(void* o) { return g_mmLoco; }
void __fastcall FakeMmCell(void* o, int, float* x, float* y) { *x = *(float*)((uint8_t*)o + 0x44); *y = *(float*)((uint8_t*)o + 0x48); }
} // namespace

DIFFTEST(ui_MinimapRouteMarkers) {
    SavedGlobals sg({ 0x62c120, 0x6cfca8, 0x6cec20, 0x6cfcbc, 0x6cec78 });
    UiTrace t;
    t.Add(0x5130f0, (void*)&rt3::SelectedTrain, (void*)&FakeMmSel);
    t.Add(0x51f1d0, (void*)&rt3::Engine_ResetTextureQueue, (void*)&RecMmReset);
    t.Add(0x477a10, (void*)&rt3::Obj_MapY, (void*)&FakeMmY);
    t.Add(0x4779c0, (void*)&rt3::Obj_MapX, (void*)&FakeMmX);
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeMmActive);
    t.Add(0x47df30, (void*)&rt3::Obj_NodeCenterX, (void*)&FakeMmNx);
    t.Add(0x47df50, (void*)&rt3::Obj_NodeCenterY, (void*)&FakeMmNy);
    t.Add(0x47efe0, (void*)&rt3::Obj_BuildingValue, (void*)&FakeMmValue);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeMmBuilding);
    t.Add(0x4a77d0, (void*)&rt3::Train_RearPoint, (void*)&FakeMmRear);
    t.Add(0x455730, (void*)&rt3::SceneObj_Angle2, (void*)&FakeMmAngle);
    t.Add(0x4a77b0, (void*)&rt3::Obj_City41, (void*)&FakeMmLoco);
    t.Add(0x48a170, (void*)&rt3::Obj_CenterCell, (void*)&FakeMmCell);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x552160, (void*)&rt3::Slider_StepA, (void*)&FakeCoStepA);
    t.Add(0x5521a0, (void*)&rt3::Slider_StepB, (void*)&FakeCoStepB);
    g_mmBldVtbl[0x20] = (void*)&FakeMmBldType;
    *(void***)g_mmBld = g_mmBldVtbl;
    g_mmType[0][0x8c] = 3;
    g_mmType[1][0x8c] = 1;
    static uint8_t m[0x200], map[0x2200], world[0x100], orders[0x33 * 12], listObj[8], rect[0x10];
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(uint8_t**)listObj = orders;
    for (int it = 0; it < 1000; ++it) {
        auto fillPos = [](uint8_t* r) {
            *(float*)(r + 0x44) = RndF(-20.0f, 300.0f);
            *(float*)(r + 0x48) = RndF(-20.0f, 300.0f);
            *(float*)(r + 0x2c) = RndF(0.0f, 600.0f);
            *(float*)(r + 0x14) = RndF(0.0f, 600.0f);
            *(float*)(r + 0x34) = RndF(0.0f, 600.0f);
            *(float*)(r + 0x1c) = RndF(0.0f, 600.0f);
        };
        FakeTable nodes(6, 0x80, [&](uint8_t* r, int i) { fillPos(r); });
        FakeTable stations(1 + Rnd() % 12, 0x80, [&](uint8_t* r, int i) {
            fillPos(r);
            r[0x40] = (uint8_t)Rnd();
            r[0x4c] = (uint8_t)Rnd();
            r[0x4d] = (uint8_t)Rnd();
        });
        FakeTable trains(Rnd() % 8, 0x200, [&](uint8_t* r, int i) {
            fillPos(r);
            r[0xf9] = Rnd() % 3;
        });
        hook::Global<Collection*>(0x6cfca8) = &nodes.c;
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        int32_t n = (int32_t)(Rnd() % 10);
        *(int32_t*)(listObj + 4) = n;
        for (int k = 0; k < n; ++k) {
            orders[k * 0x33 + 0x28] = Rnd() % 2 ? 0x80 : 0;
            *(uint32_t*)(orders + k * 0x33 + 0x2b) = rt3::Collection_IdAt(&nodes.c, 0, Rnd() % 6, 0);
            *(uint16_t*)(orders + k * 0x33 + 0x29) = (uint16_t)rt3::Collection_IdAt(&stations.c, 0, Rnd() % rt3::Collection_Count(&stations.c), 0);
        }
        g_mmSel = Rnd() % 6 == 0 ? nullptr : (void*)listObj;
        for (auto& b : m) b = (uint8_t)Rnd();
        *(uint32_t*)(m + 0x33) = Rnd() % 6 == 0 ? 0 : 0x10000;
        *(int32_t*)(m + 0xc3) = (int32_t)(Rnd() % 50);
        *(int32_t*)(m + 0xc7) = (int32_t)(Rnd() % 50);
        *(int32_t*)(m + 0xcb) = 100 + (int32_t)(Rnd() % 200);
        *(int32_t*)(m + 0xcf) = 100 + (int32_t)(Rnd() % 200);
        *(float*)(m + 0x13) = RndF(0.5f, 8.0f);
        *(float*)(m + 0xb) = RndF(0.0f, 1.0f);
        *(uint8_t**)(m + 0x86) = rect;
        *(float*)(rect + 5) = RndF(0.0f, 1.0f);
        *(uint32_t*)(m + 0xaf) = 0xaf0000;
        *(uint32_t*)(m + 0x10f) = rt3::Collection_IdAt(&stations.c, 0, Rnd() % rt3::Collection_Count(&stations.c), 0);
        *(uint32_t*)(m + 0x113) = Rnd() % 3 == 0 ? 0 : Rnd() % 4 == 0 ? 0x7777 : rt3::Collection_IdAt(&nodes.c, 0, Rnd() % 6, 0);
        *(int32_t*)(map + 0x2155) = 64 + (int32_t)(Rnd() % 512);
        *(int32_t*)(map + 0x2159) = 64 + (int32_t)(Rnd() % 512);
        *(int32_t*)(world + 0x21) = Rnd() % 2;
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*))0x478330)(m); });
        std::string b = t.Run([&] { rt3::Minimap_DrawRouteMarkers(m); });
        CompareLogs(a, b, "minimap markers", it);
    }
}

namespace rt3 {
void __fastcall SceneStats_Draw(uint8_t* s, int, float dt);
int32_t __fastcall Render_TriangleCount(uint8_t* r);
void __stdcall Textures_MemoryTotals(int32_t* video, int32_t* resident, int32_t* count);
int32_t __fastcall Engine_TotalTextureMem(void* e);
int32_t __fastcall Engine_FreeTextureMem(void* e);
void __stdcall Overlay_DrawText(const char* text, int32_t shadow);
}

namespace {
int32_t g_ssTris, g_ssTotal, g_ssFree;
int32_t __fastcall FakeSsTris(uint8_t* r) { return g_ssTris; }
void __stdcall FakeSsTex(int32_t* v, int32_t* r, int32_t* c) { Logf("texmem %d", c != nullptr); *v = 123456; *r = -654321; }
int32_t __fastcall FakeSsTotal(void* e) { return g_ssTotal; }
int32_t __fastcall FakeSsFree(void* e) { return g_ssFree; }
void __stdcall RecSsDraw(const char* t, int32_t sh) { Logf("overlay %d [%s]", sh, t); }
} // namespace

DIFFTEST(ui_SceneStats) {
    SavedGlobals sg({ 0xcc3b44, 0xcc3b48, 0x6d4024, 0xcc3b34, 0xccd81c, 0xccd818, 0xcc3b38, 0xcc3b3c });
    UiTrace t;
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeMiClock);
    t.Add(0x545d50, (void*)&rt3::Render_TriangleCount, (void*)&FakeSsTris);
    t.Add(0x5207a0, (void*)&rt3::Textures_MemoryTotals, (void*)&FakeSsTex);
    t.Add(0x51fa40, (void*)&rt3::Engine_TotalTextureMem, (void*)&FakeSsTotal);
    t.Add(0x51fa00, (void*)&rt3::Engine_FreeTextureMem, (void*)&FakeSsFree);
    t.Add(0x566980, (void*)&rt3::Overlay_DrawText, (void*)&RecSsDraw);
    static uint8_t screen[0x114240], scene[0x22c00], render[0x3800], opts[0x100];
    hook::Global<uint8_t*>(0x6d4024) = screen;
    *(uint8_t**)(scene + 0x2d) = render;
    *(uint8_t**)(scene + 0x29) = opts;
    for (int it = 0; it < 3000; ++it) {
        g_miClock = (int32_t)(Rnd() % 100000);
        int32_t last = g_miClock - (int32_t)(Rnd() % 400);
        float held = RndF(0.0f, 0.2f);
        g_ssTris = (int32_t)(Rnd() % 200000);
        g_ssTotal = (int32_t)(Rnd() % 100000000);
        g_ssFree = (int32_t)(Rnd() % 100000000);
        screen[0x114206] = Rnd() % 4 != 0;
        *(int32_t*)(opts + 0x8c) = Rnd() % 3 == 0;
        opts[0x18] = Rnd() % 2;
        opts[0x1a] = Rnd() % 2;
        *(float*)(scene + 0x22b87) = RndF(0.0f, 3.0f);
        *(float*)(scene + 0x22b83) = RndF(0.0f, 3.0f);
        *(int32_t*)(scene + 0x22b7b) = (int32_t)(Rnd() % 50000);
        *(int32_t*)(scene + 0xd) = (int32_t)(Rnd() % 50000);
        *(int32_t*)(render + 0x370d) = (int32_t)(Rnd() % 5000);
        hook::Global<uint16_t>(0xcc3b34) = (uint16_t)Rnd();
        for (uint32_t a : { 0xccd81cu, 0xccd818u, 0xcc3b38u, 0xcc3b3cu }) hook::Global<int32_t>(a) = (int32_t)(Rnd() % 100000);
        float dt = Rnd() % 8 == 0 ? 0.0f : RndF(0.001f, 0.2f);
        std::string logs[2];
        uint32_t keep[2][2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<float>(0xcc3b44) = held;
            hook::Global<int32_t>(0xcc3b48) = last;
            logs[side] = t.Run([&] { side ? rt3::SceneStats_Draw(scene, 0, dt) : ((void(__fastcall*)(uint8_t*, int, float))0x527650)(scene, 0, dt); });
            keep[side][0] = hook::Global<uint32_t>(0xcc3b44);
            keep[side][1] = hook::Global<uint32_t>(0xcc3b48);
        }
        CompareLogs(logs[0], logs[1], "scene stats", it);
        EXPECT_EQ(memcmp(keep[0], keep[1], sizeof keep[0]), 0, "scene stats hold iteration %d", it);
    }
}

namespace rt3 {
void __fastcall TrainBuy_EngineDraw(uint8_t* p, int, float x, float y, float w, float h, int32_t);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
}

namespace {
uint8_t g_tbList[0x200];
uint32_t g_tbPick;
uint8_t* __fastcall FakeTbFind(uint8_t* w, int, int32_t id) { Logf("find %x", id); return g_tbList; }
uint32_t __fastcall FakeTbSel(uint8_t* w) { Logf("selected"); return g_tbPick; }
} // namespace

DIFFTEST(ui_TrainBuyEngine) {
    SavedGlobals sg({ 0x6ada84, 0x62ba88, 0x6cec78, 0x6cec7c, 0x6d3ac8, 0x6d3abc, 0x6d3ac0, 0x6d3ac4, 0x6d3ae2, 0x622b28, 0x6d3aac, 0x6d3ab0, 0x6d3ab4 });
    UiTrace t;
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeTbFind);
    t.Add(0x55fba0, (void*)&rt3::TreeView_SelectedData, (void*)&FakeTbSel);
    t.Add(0x4619c0, (void*)&rt3::Obj_EffectScale, (void*)&FakeLdScale);
    t.Add(0x436590, (void*)&rt3::Effects_Multiplier, (void*)&FakeLdMul);
    t.Add(0x436710, (void*)&rt3::Obj_CargoBonus, (void*)&FakeLdBonus);
    t.Add(0x4a6f20, (void*)&rt3::SpeedFactor, (void*)&FakeLdSpeed);
    t.Add(0x4a7c10, (void*)&rt3::Cargo_DeliveryPayment, (void*)&FakeLdPay);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    static uint8_t world[0x4b00], sb[0x100], win[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = sb;
    hook::Global<uint32_t>(0x6d3abc) = 0x3abc0000;
    hook::Global<uint32_t>(0x6d3ac0) = 0x3ac00000;
    hook::Global<uint32_t>(0x6d3ac4) = 0x3ac40000;
    for (int it = 0; it < 300; ++it) {
        FakeTable types(4, 0x200, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x45, "Engine %d", i);
            *(float*)(r + 0xfd) = RndF(10.0f, 120.0f);
        });
        FakeTable recs(3, 0x100, [&](uint8_t* r, int i) {
            *(uint32_t*)(r + 0x72) = rt3::Collection_IdAt(&types.c, 0, Rnd() % 4, 0);
            *(uint32_t*)(r + 0x76) = rt3::Collection_IdAt(&types.c, 0, Rnd() % 4, 0);
            r[0x7a] = Rnd() % 2;
            *(float*)(r + 8) = RndF(0.0f, 300.0f);
            *(int32_t*)(r + 0xc) = (int32_t)(Rnd() % 14) - 2;
            *(int32_t*)(r + 0x10) = (int32_t)(Rnd() % 3);
            *(int32_t*)(r + 0x14) = (int32_t)(Rnd() % 4);
            *(int32_t*)(r + 0x18) = (int32_t)(Rnd() % 14) - 2;
            *(int32_t*)(r + 0x1c) = (int32_t)(Rnd() % 14) - 2;
        });
        hook::Global<Collection*>(0x62ba88) = &types.c;
        hook::Global<Collection*>(0x6ada84) = &recs.c;
        hook::Global<uint8_t*>(0x6d3ac8) = Rnd() % 10 == 0 ? nullptr : win;
        *(int32_t*)(g_tbList + 0x1d6) = Rnd() % 8 == 0 ? -1 : 0;
        g_tbPick = Rnd() % 8 == 0 ? 0 : rt3::Collection_IdAt(&recs.c, 0, Rnd() % 3, 0);
        *(float*)(win + 5) = RndF(0.0f, 0.5f);
        *(float*)(win + 9) = RndF(0.0f, 0.5f);
        sb[0x82] = Rnd() % 3 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 3 == 0;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1830 + Rnd() % 150);
        *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 9);
        *(int32_t*)(world + 0x25) = (int32_t)(Rnd() % 9);
        hook::Global<int32_t>(0x622b28) = (int32_t)(Rnd() % 3);
        for (int k = 0; k < 3; ++k) hook::Global<int32_t>(0x6d3aac + k * 4) = 5 + (int32_t)(Rnd() % 40);
        uint8_t flag = Rnd() % 2;
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        std::string logs[2];
        uint8_t flags[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<uint8_t>(0x6d3ae2) = flag;
            logs[side] = t.Run([&] { side ? rt3::TrainBuy_EngineDraw(nullptr, 0, x, y, w, h, 0) : ((void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t))0x50fff0)(nullptr, 0, x, y, w, h, 0); });
            flags[side] = hook::Global<uint8_t>(0x6d3ae2);
        }
        CompareLogs(logs[0], logs[1], "train buy engine", it);
        EXPECT_EQ(flags[0], flags[1], "train buy flag iteration %d", it);
    }
}

namespace rt3 {
void __fastcall TrainBuy_Init(uint8_t* w);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
uint32_t __fastcall EngineTypes_PickForYear(Collection* c);
void __fastcall ListView_RefreshVisible(uint8_t* tv);
void UpgradeDialog_UpdateCostText();
void __fastcall TrainList_UpdateButtons(uint8_t* w);
}

namespace {
uint8_t g_tiWidgets[4][0x200];
int32_t __fastcall FakeTiAvail(uint8_t* o, int, uint32_t c, uint32_t p) { Logf("avail %x %x", c, p); return o[0x50] & 1; }
uint32_t __fastcall FakeTiPick(Collection* c) { Logf("pick"); return 0x4242; }
void __fastcall RecTiRefresh(uint8_t* tv) { Logf("refresh"); }
void RecTiCost() { Logf("cost text"); }
void __fastcall RecTiButtons(uint8_t* w) { Logf("buttons"); }
bool g_tiNoList;
uint8_t* __fastcall FakeTiFind(uint8_t* w, int, int32_t id) { Logf("find %x", id); return id == 0x2afe && g_tiNoList ? nullptr : g_tiWidgets[id & 3]; }
uint8_t g_tiLocos[4][0x10], g_tiTypes[4][0x200];
void* __fastcall FakeTiLoco(void* t) { return g_tiLocos[((uint8_t*)t)[0x60] & 3]; }
void* __fastcall FakeTiType(void* l) { return g_tiTypes[((uint8_t*)l)[0] & 3]; }
} // namespace

DIFFTEST(ui_TrainBuyInit) {
    SavedGlobals sg({ 0x6ada84, 0x62ba88, 0x6cfcbc, 0x6cec78, 0x6cec7c, 0x6d3ac8, 0x6d3abc, 0x6d3ac0, 0x6d3ac4, 0x6d1aa0, 0x6d3acc, 0x6d3aec,
                      0x6d3aac, 0x6d3ab0, 0x6d3ab4, 0x6d3b4c, 0x6d3c84, 0x6d4020 });
    static uint8_t keep[0x2000];
    memcpy(keep, (void*)0x6d1aa8, sizeof keep);
    UiTrace t;
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLiLoad);
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeLiFont);
    t.Add(0x41d550, (void*)&rt3::CarType_Available, (void*)&FakeTiAvail);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    t.Add(0x461cd0, (void*)&rt3::EngineTypes_PickForYear, (void*)&FakeTiPick);
    t.Add(0x560e50, (void*)&rt3::ListView_RefreshVisible, (void*)&RecTiRefresh);
    t.Add(0x50f890, (void*)&rt3::UpgradeDialog_UpdateCostText, (void*)&RecTiCost);
    t.Add(0x511170, (void*)&rt3::TrainList_UpdateButtons, (void*)&RecTiButtons);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeTiFind);
    t.Add(0x4619c0, (void*)&rt3::Obj_EffectScale, (void*)&FakeLdScale);
    t.Add(0x4a77b0, (void*)&rt3::Obj_City41, (void*)&FakeTiLoco);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeTwString);
    t.Add(0x41adb0, (void*)&rt3::Obj_Ref388, (void*)&FakeTiType);
    static uint8_t world[0x4b00], sb[0x100], pack[0x10], win[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = sb;
    hook::Global<uint8_t*>(0x6d4020) = pack;
    for (int k = 0; k < 4; ++k) {
        g_tiLocos[k][0] = (uint8_t)k;
        sprintf((char*)g_tiTypes[k] + 0x45, "Type%d", k);
    }
    for (int it = 0; it < 400; ++it) {
        FakeTable types(5, 0x200, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x45, "Loco%d", (int)(Rnd() % 3));
            *(int32_t*)(r + 0x105) = (int32_t)(Rnd() % 4);
            r[0x50] = Rnd() % 4 != 0;
        });
        FakeTable recs(Rnd() % 8, 0x100, [&](uint8_t* r, int i) { *(uint32_t*)(r + 0x72) = rt3::Collection_IdAt(&types.c, 0, Rnd() % 5, 0); });
        FakeTable trains(Rnd() % 8, 0x100, [&](uint8_t* r, int i) {
            r[0x60] = (uint8_t)Rnd();
            *(int32_t*)(r + 0x51) = (int32_t)(Rnd() % 2);
            *(uint32_t*)(r + 0x25) = Rnd() % 5 == 0 ? 0x3000 : Rnd() % 20;
        }, 1);
        hook::Global<Collection*>(0x62ba88) = &types.c;
        hook::Global<Collection*>(0x6ada84) = &recs.c;
        hook::Global<Collection*>(0x6cfcbc) = &trains.c;
        for (int k = 0; k < 4; ++k) *(uint32_t*)(g_tiTypes[k] + 0x126) = Rnd() % 2 && rt3::Collection_Count(&recs.c) ? rt3::Collection_IdAt(&recs.c, 0, Rnd() % rt3::Collection_Count(&recs.c), 0) : 0x99;
        hook::Global<int32_t>(0x6d3b4c) = Rnd() % 2;
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 5);
        hook::Global<uint32_t>(0x6d1aa0) = rt3::Collection_Count(&recs.c) ? rt3::Collection_IdAt(&recs.c, 0, Rnd() % rt3::Collection_Count(&recs.c), 0) : 0;
        bool upgrade = Rnd() % 2 && rt3::Collection_Count(&recs.c);
        hook::Global<uint32_t>(0x6d3acc) = upgrade ? (Rnd() % 3 == 0 || !rt3::Collection_Count(&trains.c) ? 0x7777 : rt3::Collection_IdAt(&trains.c, 0, Rnd() % rt3::Collection_Count(&trains.c), 0)) : 0;
        hook::Global<int32_t>(0x6d3aec) = (int32_t)(Rnd() % 5);
        sb[0x82] = Rnd() % 3 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 3 == 0;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1830 + Rnd() % 150);
        *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 2);
        *(int32_t*)(world + 0x25) = (int32_t)(Rnd() % 3);
        g_tiNoList = Rnd() % 4 == 0;
        std::string logs[2];
        uint8_t marks[2][0x20];
        for (int side = 0; side < 2; ++side) {
            memset((void*)0x6d1aa8, 0xcc, 0x2000);
            logs[side] = t.Run([&] { side ? rt3::TrainBuy_Init(win) : ((void(__fastcall*)(uint8_t*))0x511500)(win); });
            memcpy(marks[side], (void*)0x6d1aa8, sizeof marks[side]);
            for (uint32_t a = 0x6d3aac; a <= 0x6d3ab4; a += 4) logs[side] += " " + std::to_string(hook::Global<int32_t>(a));
        }
        CompareLogs(logs[0], logs[1], "train buy init", it);
        EXPECT_EQ(memcmp(marks[0], marks[1], sizeof marks[0]), 0, "train buy marks iteration %d", it);
    }
    memcpy((void*)0x6d1aa8, keep, sizeof keep);
}

namespace rt3 {
void __fastcall PlayerSetup_Build(uint8_t* w, int, float x, float y, float wd, float h);
void __fastcall Dialog_SetupWorthSlider(int32_t select);
void __fastcall MoneySlider_Setup(int32_t reset);
void __fastcall MoneySliders_Read(int32_t rerange);
}

namespace {
int32_t g_psSlot;
void __fastcall RecPsWorth(int32_t s) { Logf("worth %d", s); }
void __fastcall RecPsSetup(int32_t r) { Logf("slider %d", r); }
void __fastcall RecPsRead(int32_t r) { Logf("read %d", r); }
int32_t __fastcall FakePsSlot(int32_t n) { Logf("slot %d", n); return g_psSlot; }
} // namespace

DIFFTEST(ui_PlayerSetupBuild) {
    SavedGlobals sg({ 0x6cec18, 0x5f2c48, 0x6d4020 });
    UiTrace t;
    t.Add(0x47c070, (void*)&rt3::Dialog_SetupWorthSlider, (void*)&RecPsWorth);
    t.Add(0x47bea0, (void*)&rt3::MoneySlider_Setup, (void*)&RecPsSetup);
    t.Add(0x47c220, (void*)&rt3::MoneySliders_Read, (void*)&RecPsRead);
    t.Add(0x424430, (void*)&rt3::NthFreeCompanySlot, (void*)&FakePsSlot);
    static uint8_t pack[0x429c0], win[0x100];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    for (int it = 0; it < 300; ++it) {
        FakeTable logos(6, 0x40, [](uint8_t* r, int i) { sprintf((char*)r, "ExtraLogo%d.imb", i); });
        *(Collection**)(pack + 0x429b0) = &logos.c;
        hook::Global<int32_t>(0x5f2c48) = (int32_t)(Rnd() % 50);
        g_psSlot = Rnd() % 2 ? (int32_t)(Rnd() % 0x24) : 0x24 + (int32_t)(Rnd() % 6);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), wd = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        std::string a = t.Run([&] { ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x47c590)(win, 0, x, y, wd, h); });
        std::string b = t.Run([&] { rt3::PlayerSetup_Build(win, 0, x, y, wd, h); });
        CompareLogs(a, b, "player setup", it);
    }
}

namespace rt3 {
void __fastcall Camera_MouseInputMomentum(uint8_t* c, int, float dx, float dy, float wheel);
}

namespace {
void __fastcall RecMmDragMouse(uint8_t* c) { Logf("drag to mouse"); }
} // namespace

DIFFTEST(game_CameraMouseMomentum) {
    SavedGlobals sg({ 0x6cec74 });
    UiTrace t;
    t.Add(0x43b030, (void*)&rt3::Tool_RemoveModifier, (void*)&FakeMiFree);
    t.Add(0x43af60, (void*)&rt3::Tool_AddModifier, (void*)&FakeMiGrab);
    t.Add(0x534490, (void*)&rt3::Terrain_HeightAt, (void*)&FakeCmHeight);
    t.Add(0x43f4c0, (void*)&rt3::Camera_DragToMouse, (void*)&RecMmDragMouse);
    static uint8_t cam[0x200], opts[0x200];
    hook::Global<uint8_t*>(0x6cec74) = opts;
    for (int it = 0; it < 20000; ++it) {
        uint8_t start[0x200];
        for (auto& b : start) b = (uint8_t)Rnd();
        for (uint32_t o : { 4u, 0xcu, 0x10u, 0x20u, 0x8au, 0x8eu, 0x92u, 0x96u, 0x9au }) *(float*)(start + o) = RndF(-100.0f, 100.0f);
        *(uint32_t*)(start + 0x6e) = 0x6e0000;
        *(int32_t*)(opts + 0x10c) = Rnd() % 2 ? 4 : (int32_t)(Rnd() % 4);
        *(int32_t*)(opts + 0x118) = (int32_t)(Rnd() % 4);
        g_miFree = Rnd() % 2;
        g_miGrab = Rnd() % 2;
        float dx = RndF(-3.0f, 3.0f), dy = RndF(-3.0f, 3.0f), wheel = Rnd() % 3 == 0 ? 0.0f : RndF(-2.0f, 2.0f);
        std::string logs[2];
        uint8_t out[2][0x200];
        for (int side = 0; side < 2; ++side) {
            memcpy(cam, start, sizeof cam);
            logs[side] = t.Run([&] { side ? rt3::Camera_MouseInputMomentum(cam, 0, dx, dy, wheel) : ((void(__fastcall*)(uint8_t*, int, float, float, float))0x43e610)(cam, 0, dx, dy, wheel); });
            memcpy(out[side], cam, sizeof cam);
        }
        CompareLogs(logs[0], logs[1], "camera mouse momentum", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "camera mouse momentum state iteration %d", it);
    }
}

namespace rt3 {
void __fastcall CompanyStock_DrawChart(uint8_t* win, uint32_t linkBase, float x, float y, float, float, int32_t compact, uint32_t coId,
                                       uint8_t* faces, uint8_t** custom, int32_t, const uint8_t* show);
}

namespace {
uint8_t g_scPlayers[4][0x400];
int32_t g_scHolders;
int32_t __fastcall FakeCcHolder(int32_t* o, int, int32_t n, int32_t f) { Logf("holder %d", n); return n <= g_scHolders ? n % 4 : -1; }
void* __fastcall FakeCcNth(Collection* t, int, int32_t n) { return g_scPlayers[n & 3]; }
int32_t __fastcall FakeCcNameIndex(void* o) { Logf("nameindex"); return ((uint8_t*)o)[0x300] % 3 - 1; }
} // namespace

DIFFTEST(ui_CompanyStockChart) {
    SavedGlobals sg({ 0x62be10, 0x6cec78, 0x6d401c });
    UiTrace t;
    t.Add(0x426b10, (void*)&rt3::Company_Logo, (void*)&FakeCoLogo);
    t.Add(0x4245e0, (void*)&rt3::Company_NthHolder, (void*)&FakeCcHolder);
    t.Add(0x477860, (void*)&rt3::Table_NthField4, (void*)&FakeCcNth);
    t.Add(0x476bb0, (void*)&rt3::Obj_NameIndex, (void*)&FakeCcNameIndex);
    t.Add(0x54f9f0, (void*)&rt3::Ui_FrameRect, (void*)&RecFrame);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeCompanyStat);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    static uint8_t world[0x46b00], ui[0xd00], focus[0x10], win[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    *(uint32_t*)(world + 0x46a98) = 0x46a90000;
    *(uint32_t*)(world + 0x46a9c) = 0x46a9c000;
    static uint8_t* custom[3] = { (uint8_t*)0x7100, (uint8_t*)0x7200, (uint8_t*)0x7300 };
    for (int it = 0; it < 2000; ++it) {
        FakeTable cos(3, 0x200, [](uint8_t* r, int i) {
            *(uint16_t*)(r + 0x157) = (uint16_t)(1830 + Rnd() % 40);
            r[0x159] = (uint8_t)(Rnd() % 12 + 1);
            r[0x15b] = (uint8_t)(Rnd() % 28 + 1);
            *(int32_t*)(r + 0x47) = (int32_t)(Rnd() % 20000000);
        });
        hook::Global<Collection*>(0x62be10) = &cos.c;
        uint32_t coId = rt3::Collection_IdAt(&cos.c, 0, Rnd() % 3, 0);
        for (int k = 0; k < 4; ++k) {
            g_scPlayers[k][0x15c] = Rnd() % 3 == 0 ? 0x29 : (uint8_t)(Rnd() % 40);
            g_scPlayers[k][0x300] = (uint8_t)Rnd();
            sprintf((char*)g_scPlayers[k] + 8, "Player %d", k);
            for (int c = 0; c < 0x60; ++c) *(int32_t*)(g_scPlayers[k] + 0x15d + c * 4) = (int32_t)(Rnd() % 12000000);
        }
        g_scHolders = (int32_t)(Rnd() % 5);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1850 + Rnd() % 30);
        world[0xf] = Rnd() % 3 == 0 ? 1 : (uint8_t)(Rnd() % 12 + 1);
        world[0x11] = Rnd() % 3 == 0 ? 1 : (uint8_t)(Rnd() % 28 + 1);
        *(uint8_t**)(ui + 0xc6d) = Rnd() % 3 == 0 ? nullptr : focus;
        *(uint16_t*)(focus + 4) = (uint16_t)(0x3f0f + Rnd() % 4);
        uint8_t show[6];
        for (auto& s : show) s = Rnd() % 2;
        int32_t compact = Rnd() % 2;
        uint32_t linkBase = 0x5000 + Rnd() % 100;
        float x = RndF(0.0f, 0.5f), y = RndF(0.0f, 0.5f);
        uint8_t keepFaces[4];
        for (int k = 0; k < 4; ++k) keepFaces[k] = g_scPlayers[k][0x15c];
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < 4; ++k) g_scPlayers[k][0x15c] = keepFaces[k];
            logs[side] = t.Run([&] {
                if (side) rt3::CompanyStock_DrawChart(win, linkBase, x, y, 0, 0, compact, coId, (uint8_t*)0x7000, custom, 0, show);
                else ((void(__fastcall*)(uint8_t*, uint32_t, float, float, float, float, int32_t, uint32_t, uint8_t*, uint8_t**, int32_t, const uint8_t*))0x4c0160)(
                         win, linkBase, x, y, 0, 0, compact, coId, (uint8_t*)0x7000, custom, 0, show);
            });
            for (int k = 0; k < 4; ++k) logs[side] += " " + std::to_string(g_scPlayers[k][0x15c]);
        }
        CompareLogs(logs[0], logs[1], "company stock chart", it);
    }
}

namespace rt3 {
void __fastcall Dialog_Setup(uint8_t* w, int, const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                             void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), float extraH, uint32_t buttonText,
                             int32_t count, const char** choices, int32_t radio, int32_t check, int32_t* checked);
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
double __fastcall Scroll_Total(uint8_t* s);
uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
}

namespace {
double __fastcall FakeDsHeight(uint8_t* f, int, const char* t, float w, float* tw) {
    Logf("height [%s] %08x %d", t, B(w), tw != nullptr);
    if (tw) *tw = strlen(t) * 0.01f;
    return strlen(t) * 0.004 + 0.01;
}
int32_t __fastcall FakeDsLines(uint8_t* f, int, const char* t, float w, float* tw) { Logf("lines"); return (int32_t)strlen(t) / 5; }
void __fastcall RecDsRect(uint8_t* w, int, float x, float y, float wd, float ht) { Logf("rect %08x %08x %08x %08x", B(x), B(y), B(wd), B(ht)); }
int32_t __fastcall RecDsEvent(uint8_t* w, int, uint32_t* ev) { Logf("event %x %x %x %x", ev[0], ev[1], ev[2], ev[3]); return 0; }
double __fastcall FakeDsLine(uint8_t* s) { return 0.012; }
uint8_t* __fastcall RecDsTree(uint8_t* w, int, const uint8_t* def) { LogDef("tree", w, def, 0x12f); return w; }
uint8_t g_dsBox[0x40];
uint8_t* __fastcall FakeDsFind(uint8_t* w, int, int32_t id) { Logf("find %x", id); return g_dsBox; }
void __fastcall RecDsPlace(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, float d) { Logf("place %x %x %x %08x", a, b, c, B(d)); }
} // namespace

DIFFTEST(ui_DialogSetup) {
    SavedGlobals sg({ 0x6d4020, 0x621f40, 0x6d4034 });
    UiTrace t;
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeLiFont);
    t.Add(0x550b50, (void*)&rt3::Font_TextHeight, (void*)&FakeDsHeight);
    t.Add(0x550470, (void*)&rt3::Font_CountLines, (void*)&FakeDsLines);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLiLoad);
    t.Add(0x53f8d0, (void*)&rt3::Widget_SetRect, (void*)&RecDsRect);
    t.Add(0x53fe90, (void*)&rt3::Window_HandleEvent, (void*)&RecDsEvent);
    t.Add(0x550090, (void*)&rt3::Scroll_Total, (void*)&FakeDsLine);
    t.Add(0x55ff70, (void*)&rt3::TreeView_Construct, (void*)&RecDsTree);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeDsFind);
    t.Add(0x563d70, (void*)&rt3::Button_Construct, (void*)&RecLwButton);
    static uint8_t pack[0x10], win[0x200];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static const char* choices[16] = { "Alpha", "Bravo choice", "C", "Delta long choice text", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P" };
    static int32_t checked[16];
    static const char* texts[4] = { "", "Short", "A much longer message that wraps over a few lines in the dialog box", "Medium length text" };
    for (int it = 0; it < 3000; ++it) {
        int32_t type = (int32_t)(Rnd() % 15);
        uint32_t px = Rnd() % 3 == 0 ? 0xbf800000 : B(RndF(0.0f, 0.5f));
        uint32_t py = Rnd() % 3 == 0 ? 0xbf800000 : B(RndF(0.0f, 0.5f));
        float extraH = Rnd() % 2 ? 0.0f : RndF(0.0f, 0.2f);
        int32_t count = Rnd() % 3 == 0 ? 0 : (int32_t)(Rnd() % 16);
        int32_t radio = Rnd() % 3 == 0, check = !radio && Rnd() % 2;
        for (auto& c : checked) c = Rnd() % 2;
        const char* text = texts[Rnd() % 4];
        hook::Global<int32_t>(0x621f40) = (int32_t)(Rnd() % 6);
        hook::Global<int32_t>(0x6d4034) = Rnd() % 2;
        auto place = Rnd() % 2 ? &RecDsPlace : nullptr;
        uint32_t btn = 0x7700 + Rnd() % 10;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(win, 0, sizeof win);
            memset(g_dsBox, 0, sizeof g_dsBox);
            logs[side] = t.Run([&] {
                if (side) rt3::Dialog_Setup(win, 0, text, type, px, py, 5, place, extraH, btn, count, choices, radio, check, checked);
                else ((void(__fastcall*)(uint8_t*, int, const char*, int32_t, uint32_t, uint32_t, int32_t, void*, float, uint32_t, int32_t, const char**, int32_t, int32_t, int32_t*))0x4c86c0)(
                         win, 0, text, type, px, py, 5, (void*)place, extraH, btn, count, choices, radio, check, checked);
            });
            char tail[128];
            sprintf(tail, " %x %x %x %08x", *(int32_t*)(win + 0x94), *(int32_t*)(win + 0x80), *(int32_t*)(win + 0x9c), *(uint32_t*)(g_dsBox + 0x17));
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "dialog setup", it);
    }
}

namespace rt3 {
uint8_t* __fastcall SettingsWindow_Construct(uint8_t* w);
}

DIFFTEST(ui_SettingsWindow) {
    SavedGlobals sg({ 0x6d4020, 0x6d1358, 0x6cec7c, 0x6d4024, 0x6cec74, 0x6d3c84 });
    UiTrace t;
    t.Add(0x53f7e0, (void*)&rt3::Widget_SetVtblRet, (void*)&RecSetVtbl);
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeLiFont);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLiLoad);
    t.Add(0x55ff70, (void*)&rt3::TreeView_Construct, (void*)&RecDsTree);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeDsFind);
    t.Add(0x563d70, (void*)&rt3::Button_Construct, (void*)&RecLwButton);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeTwString);
    static uint8_t pack[0x10], win[0x200], game[0x100], ui[0x114800], opts[0x400];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint8_t*>(0x6cec7c) = game;
    hook::Global<uint8_t*>(0x6d4024) = ui;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    for (int it = 0; it < 200; ++it) {
        game[0x82] = Rnd() % 2;
        *(uint32_t*)(ui + 0x11474a) = Rnd();
        *(uint32_t*)(opts + 0x31d) = Rnd() % 11;
        *(uint32_t*)(opts + 0x319) = Rnd() % 11;
        hook::Global<int32_t>(0x6d3c84) = Rnd() % 2 ? 4 : (int32_t)(Rnd() % 6);
        float boxH = RndF(0.0f, 1.0f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(win, 0, sizeof win);
            memset(g_dsBox, 0, sizeof g_dsBox);
            *(float*)(g_dsBox + 0xf) = boxH;
            hook::Global<uint8_t*>(0x6d1358) = nullptr;
            logs[side] = t.Run([&] {
                uint8_t* r = side ? rt3::SettingsWindow_Construct(win) : ((uint8_t*(__fastcall*)(uint8_t*))0x500640)(win);
                Logf("ret %d", r == win);
            });
            char tail[64];
            sprintf(tail, " %08x %08x %p", *(uint32_t*)win, *(uint32_t*)(g_dsBox + 0xf), hook::Global<uint8_t*>(0x6d1358));
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "settings window", it);
    }
}

namespace rt3 {
void __fastcall StationCargo_DrawCompare(void*, int, float, float, float, float, int32_t);
void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged);
}

namespace {
int32_t __fastcall FakeScStock(uint8_t* o, int, int32_t i, float* stock, float* quota) {
    Logf("stock %p %d", o, i);
    *stock = *(float*)(o + 0x30);
    *quota = *(float*)(o + 0x34);
    return 1;
}
void __fastcall FakeScGather(uint8_t* o, int, int32_t sim, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* n, float* amt, float* fl) {
    Logf("gather %p %p %d %d %08x %d", o, to, sim, cargo, B(cap), fl != nullptr);
    uint8_t k = (uint8_t)(o[0x40] ^ to[0x41]);
    *n = k % 4 == 0 ? 0.0f : (float)(k % 7) * 0.5f;
    *amt = (float)(k * 37 % 2000);
}
double __fastcall FakeScHeight(uint8_t* f, int, const char* t, float w, float* tw) {
    Logf("measure [%s] %08x", t, B(w));
    *tw = strlen(t) * 0.004f;
    return 0.02;
}
} // namespace

DIFFTEST(ui_StationCargoCompare) {
    SavedGlobals sg({ 0x6cec78, 0x6d0818, 0x6cec20, 0x62ba8c, 0x5ee4fc, 0x5ee500, 0x6cec7c, 0x6d401c, 0x6d3e34, 0x6d07b4, 0x6d3c84, 0xd93850, 0x625464 });
    UiTrace t;
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeStateActive);
    t.Add(0x47e240, (void*)&rt3::Obj_CargoStockAndQuota, (void*)&FakeScStock);
    t.Add(0x47e690, (void*)&rt3::Train_GatherLoads, (void*)&FakeScGather);
    t.Add(0x47efe0, (void*)&rt3::Obj_BuildingValue, (void*)&FakeScValue);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x550b50, (void*)&rt3::Font_TextHeight, (void*)&FakeScHeight);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeLocalize);
    t.Add(0x54f9f0, (void*)&rt3::Ui_FrameRect, (void*)&RecFrame);
    static uint8_t world[0x46b00], game[0x100], ui[0xd00];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = game;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    hook::Global<int32_t>(0x6d0818) = 1;
    hook::Global<void*>(0x6d07b4) = nullptr;
    *(uint32_t*)(world + 0x46a94) = 0x46a940;
    *(uint32_t*)(world + 0x46a98) = 0x46a980;
    *(uint32_t*)(world + 0x46a8c) = 0x46a8c0;
    size_t total = 0;
    int drawn = 0;
    for (int it = 0; it < 1500; ++it) {
        int n = Rnd() % 6 == 0 ? 0 : 1 + Rnd() % 30;
        FakeTable stations(n == 0 ? 1 : n, 0x500, [](uint8_t* r, int i) {
            r[0x10] = Rnd() % 6 != 0;
            r[0x22] = (uint8_t)Rnd();
            r[0x40] = (uint8_t)Rnd();
            r[0x41] = (uint8_t)Rnd();
            *(float*)(r + 0x30) = Rnd() % 5 == 0 ? 0.0f : RndF(0.0f, 3.0f);
            *(float*)(r + 0x34) = Rnd() % 4 == 0 ? 20000.0f : (float)(Rnd() % 40000);
            sprintf((char*)r + 0x46b, "Station %d", i);
        });
        FakeTable cargos(3, 0x500, [](uint8_t* r, int i) {
            r[0x46] = Rnd() % 2;
            sprintf((char*)r + 4, "Cargo%d", i);
            sprintf((char*)r + 0x46b, "Load%d", i);
        });
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        hook::Global<Collection*>(0x62ba8c) = &cargos.c;
        hook::Global<uint32_t>(0x5ee4fc) = rt3::Collection_IdAt(&stations.c, 0, Rnd() % stations.c.count, 0);
        hook::Global<uint32_t>(0x5ee500) = Rnd() % 8 == 0 ? 0 : rt3::Collection_IdAt(&cargos.c, 0, Rnd() % 3, 0);
        game[0x82] = Rnd() % 10 == 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 10 == 0;
        *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 3);
        *(int32_t*)(ui + 0xc64) = (int32_t)(Rnd() % 3) - 1;
        hook::Global<int32_t>(0x6d3e34) = (int32_t)(Rnd() % 4);
        hook::Global<int32_t>(0x6d3c84) = Rnd() % 2 ? 0 : 3;
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                if (side) rt3::StationCargo_DrawCompare(nullptr, 0, 0, 0, 0, 0, 0);
                else ((void(__fastcall*)(void*, int, float, float, float, float, int32_t))0x43f640)(nullptr, 0, 0, 0, 0, 0, 0);
                Logf("fade %x %x", hook::Global<uint32_t>(0xd93850), hook::Global<uint32_t>(0x625464));
            });
        CompareLogs(logs[0], logs[1], "station cargo compare", it);
        total += logs[0].size();
        if (logs[0].find("Station") != std::string::npos) ++drawn;
    }
    fprintf(g_out, "  [station cargo compare: %u log bytes, %d with rows]\n", (unsigned)total, drawn);
    EXPECT_EQ(total > 500000, true, "station cargo compare log size %u", (unsigned)total);
}

namespace rt3 {
void __fastcall CompanyWindow_DrawStockPage(uint8_t* w, int, float x, float y, float wd, float h);
}

namespace {
void __fastcall RecSpChart(uint8_t* win, uint32_t link, float x, float y, float w, float h, int32_t compact, uint32_t co, uint8_t* faces,
                           uint8_t** custom, int32_t n, const uint8_t* show) {
    Logf("chart %p %x %08x %08x %08x %08x %d %x %p %p %d %p", win, link, B(x), B(y), B(w), B(h), compact, co, faces, custom, n, show);
}
} // namespace

DIFFTEST(ui_CompanyStockPage) {
    SavedGlobals sg({ 0x62be10 });
    UiTrace t;
    t.Add(0x4c0160, (void*)&rt3::CompanyStock_DrawChart, (void*)&RecSpChart);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeTwString);
    static uint8_t win[0x1000];
    for (int it = 0; it < 300; ++it) {
        FakeTable cos(3, 0x100, [](uint8_t*, int) {});
        hook::Global<Collection*>(0x62be10) = &cos.c;
        for (auto& b : win) b = (uint8_t)Rnd();
        *(uint32_t*)(win + 0x88) = Rnd() % 3 == 0 ? 0x77 : rt3::Collection_IdAt(&cos.c, 0, Rnd() % 3, 0);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f), w = RndF(0.0f, 1.0f), h = RndF(0.0f, 1.0f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side)
            logs[side] = t.Run([&] {
                if (side) rt3::CompanyWindow_DrawStockPage(win, 0, x, y, w, h);
                else ((void(__fastcall*)(uint8_t*, int, float, float, float, float))0x4e5130)(win, 0, x, y, w, h);
            });
        CompareLogs(logs[0], logs[1], "company stock page", it);
    }
}

namespace rt3 {
void __fastcall Map_DrawCellOverlay(uint8_t* m, int, float x, float y, uint32_t fade, int32_t cx, int32_t cy);
float __fastcall Station_TotalAvailable(void* o);
float __fastcall City_CargoOutput(uint8_t* c, int, int32_t cargo);
double __fastcall Grid_Float1605(uint8_t* g, int, int32_t x, int32_t y);
double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
int32_t Tutorial_Blink348();
int32_t Tutorial_Blink352();
void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7,
                                 uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex, uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15,
                                 uint32_t p16, uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);
}

namespace {
float g_coDist;
int32_t g_coBlink;
float __fastcall FakeCoTotal(void* o) {
    Logf("total %p", o);
    return *(float*)((uint8_t*)o + 8);
}
float __fastcall FakeCoOutput(uint8_t* c, int, int32_t cargo) {
    Logf("output %p %d", c, cargo);
    return *(float*)(c + 0xc);
}
double __fastcall FakeCoGrid(uint8_t* g, int, int32_t x, int32_t y) {
    Logf("grid %d %d", x, y);
    return x * 3.5 + y;
}
double __stdcall FakeCoDist(float a, float b, float c, float d, float e, float f) {
    Logf("dist %08x %08x %08x %08x %08x %08x", B(a), B(b), B(c), B(d), B(e), B(f));
    return g_coDist;
}
int32_t FakeCoBlink348() { return g_coBlink & 1; }
int32_t FakeCoBlink352() { return g_coBlink & 2; }
void __fastcall RecCoQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8,
                          uint32_t p9, uint32_t p10, uint32_t tex, uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16, uint32_t p17,
                          uint32_t p18, uint32_t p19, uint32_t p20) {
    Logf("quad %p %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x", e, p1, p2, p3, p4, p5, p6, p7,
         p8, p9, p10, tex, p12, p13, p14, p15, p16, p17, p18, p19, p20);
}
} // namespace

DIFFTEST(ui_MapCellOverlay) {
    SavedGlobals sg({ 0x62be68, 0x62c120, 0x6cec78, 0x6d4024, 0xd93850, 0x625464 });
    UiTrace t;
    t.Add(0x42be20, (void*)&rt3::Station_TotalAvailable, (void*)&FakeCoTotal);
    t.Add(0x42bf20, (void*)&rt3::City_CargoOutput, (void*)&FakeCoOutput);
    t.Add(0x448c00, (void*)&rt3::Grid_Float1605, (void*)&FakeCoGrid);
    t.Add(0x51db40, (void*)&rt3::Distance3D, (void*)&FakeCoDist);
    t.Add(0x5164f0, (void*)&rt3::Tutorial_Blink348, (void*)&FakeCoBlink348);
    t.Add(0x516540, (void*)&rt3::Tutorial_Blink352, (void*)&FakeCoBlink352);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x54f710, (void*)&rt3::Ui_FillRect, (void*)&RecFillRect);
    t.Add(0x51f370, (void*)&rt3::Engine_QueueQuad, (void*)&RecCoQuad);
    static uint8_t map[0x2200], cells[16 * 0x1d7], view[0x40], world[0x46b00], modes[0x100];
    static uint8_t engine[0x114300];
    hook::Global<uint8_t*>(0x62be68) = view;
    hook::Global<uint8_t*>(0x62c120) = (uint8_t*)0x62c1200;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    *(uint8_t**)(map + 0x2129) = cells;
    *(int32_t*)(map + 0x2145) = 4;
    *(uint8_t**)(engine + 0x34) = modes;
    *(uint32_t*)(world + 0x46a90) = 0x46a900;
    size_t total = 0;
    for (int it = 0; it < 6000; ++it) {
        int32_t cx = (int32_t)(Rnd() % 16), cy = (int32_t)(Rnd() % 16);
        uint8_t* cell = cells + ((cy / 4) * 4 + cx / 4) * 0x1d7;
        int32_t mode = Rnd() % 3 == 0 ? 0 : (int32_t)(1 + Rnd() % 60);
        *(int32_t*)(map + 0x2179) = mode;
        float count = Rnd() % 5 == 0 ? 0.0f : (Rnd() % 3 == 0 ? (float)(Rnd() % 5) : RndF(0.0f, 5.0f));
        *(float*)(cell + 8) = count;
        if (mode) {
            *(float*)(cell + 0x103 + mode % 0x35 * 4) = count;
            *(uint16_t*)(cell + 0x6a + mode * 2) = Rnd() % 3 == 0 ? 0 : (uint16_t)(Rnd() % 4000);
            *(float*)(cell + 0xc) = Rnd() % 3 == 0 ? 0.0f : RndF(0.0f, 8.0f);
        }
        *(float*)(view + 0x14) = RndF(0.3f, 1.5f);
        *(float*)(view + 0x1c) = RndF(0.0f, 1000.0f);
        *(float*)(view + 0x20) = Rnd() % 4 == 0 ? (float)(Rnd() % 20000) : RndF(0.0f, 20000.0f);
        *(float*)(view + 0x24) = RndF(0.0f, 1000.0f);
        g_coDist = Rnd() % 8 == 0 ? RndF(0.0f, 20.0f) : RndF(500.0f, 30000.0f);
        g_coBlink = Rnd() % 6 == 0 ? (int32_t)(Rnd() % 4) : 0;
        int32_t modeIdx = (int32_t)(Rnd() % 3);
        *(int32_t*)(engine + 0x11421e) = modeIdx;
        *(int32_t*)(modes + modeIdx * 0x15 + 4) = 480 + (int32_t)(Rnd() % 800);
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        uint32_t fade = B(RndF(0.0f, 1.0f));
        float fade0 = RndF(0.0f, 1.0f), fadeSaved = RndF(0.0f, 1.0f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<float>(0xd93850) = fade0;
            hook::Global<float>(0x625464) = fadeSaved;
            logs[side] = t.Run([&] {
                if (side) rt3::Map_DrawCellOverlay(map, 0, x, y, fade, cx, cy);
                else ((void(__fastcall*)(uint8_t*, int, float, float, uint32_t, int32_t, int32_t))0x4515c0)(map, 0, x, y, fade, cx, cy);
                Logf("fade %08x %08x", hook::Global<uint32_t>(0xd93850), hook::Global<uint32_t>(0x625464));
            });
        }
        CompareLogs(logs[0], logs[1], "map cell overlay", it);
        total += logs[0].size();
    }
    fprintf(g_out, "  [map cell overlay: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
int32_t __fastcall Company_PickTrainOrder(int32_t* c, int, uint8_t* o, uint32_t from, uint8_t* train);
int32_t __fastcall Company_BestSite(int32_t* c);
int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o);
uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t);
double __stdcall Effects_CargoMultiplier(int32_t idx);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag);
void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y);
double __fastcall Obj_BuildingCellY(void* o);
double __fastcall Obj_BuildingCellX(void* o);
}

namespace {
int32_t g_ptBest, g_ptReach;
int32_t __fastcall FakePtBest(int32_t* c) { Logf("best site %d", *c); return g_ptBest; }
int32_t __fastcall FakePtCount(uint8_t* o) { Logf("count %p", o); return o[0x30]; }
uint32_t __fastcall FakePtNth(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t z) {
    Logf("nth %p %d %d", o, n, z);
    *stock = *(float*)(o + 0x80 + n * 4);
    *quota = *(float*)(o + 0x100 + n * 4);
    return o[0x40 + n];
}
double __stdcall FakePtMult(int32_t i) { Logf("mult %d", i); return i % 5 == 0 ? 0.0 : 0.5 + (i % 7) * 0.25; }
uint8_t* __fastcall FakePtRegion(void* o) { Logf("pays %p", o); return (uint8_t*)o + 0x180; }
void* __fastcall FakePtCityRef(void* o) { return (uint8_t*)o + 0x200; }
int32_t __stdcall FakePtReach(int32_t from, int32_t to, int32_t flag) {
    Logf("reach %d %d %d", from, to, flag);
    return (g_ptReach >> ((from + to * 3 + flag) & 15)) & 1;
}
void __fastcall FakePtRear(uint8_t* t, int, float* x, float* y) { *x = *(float*)(t + 0x10); *y = *(float*)(t + 0x14); }
double __fastcall FakePtCellY(void* o) { return *(float*)((uint8_t*)o + 0x24); }
double __fastcall FakePtCellX(void* o) { return *(float*)((uint8_t*)o + 0x28); }
} // namespace

DIFFTEST(game_CompanyPickTrainOrder) {
    SavedGlobals sg({ 0x6cec20, 0x62ba8c, 0x6cec78 });
    UiTrace t;
    t.Add(0x408280, (void*)&rt3::Company_BestSite, (void*)&FakePtBest);
    t.Add(0x47e330, (void*)&rt3::Obj_CountAvailableCargos, (void*)&FakePtCount);
    t.Add(0x47e620, (void*)&rt3::Obj_NthAvailableCargo, (void*)&FakePtNth);
    t.Add(0x437d20, (void*)&rt3::Effects_CargoMultiplier, (void*)&FakePtMult);
    t.Add(0x47de20, (void*)&rt3::Obj_BuildingRegion, (void*)&FakePtRegion);
    t.Add(0x4a8840, (void*)&rt3::Obj_CityRef, (void*)&FakePtCityRef);
    t.Add(0x4a6e00, (void*)&rt3::Route_CanReach, (void*)&FakePtReach);
    t.Add(0x4a77d0, (void*)&rt3::Train_RearPoint, (void*)&FakePtRear);
    t.Add(0x47df00, (void*)&rt3::Obj_BuildingCellY, (void*)&FakePtCellY);
    t.Add(0x47ded0, (void*)&rt3::Obj_BuildingCellX, (void*)&FakePtCellX);
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeStateActive);
    t.Add(0x47efe0, (void*)&rt3::Obj_BuildingValue, (void*)&FakeScValue);
    static uint8_t world[0x100], cargos[0x200], train[0x300];
    static uint8_t lists[12][3][0x20];
    static uint8_t entries[12][3][12 * 0xd];
    static uint8_t* listPtrs[12][3];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x62ba8c) = cargos;
    for (int it = 0; it < 4000; ++it) {
        int n = 1 + Rnd() % 12;
        static int s_k;
        s_k = 0;
        FakeTable stations(n, 0x600, [&](uint8_t* r, int i) {
            *(int32_t*)r = i + 1;
            *(int32_t*)(r + 8) = (int32_t)(Rnd() % 6);
            r[0x10] = Rnd() % 5 != 0;
            r[0x22] = (uint8_t)Rnd();
            *(float*)(r + 0x24) = RndF(0.0f, 300.0f);
            *(float*)(r + 0x28) = RndF(0.0f, 300.0f);
            r[0x30] = (uint8_t)(Rnd() % 8);
            for (int k = 1; k <= 8; ++k) {
                r[0x40 + k] = (uint8_t)(Rnd() % 0x35);
                *(float*)(r + 0x80 + k * 4) = Rnd() % 4 == 0 ? 0.0f : RndF(0.0f, 6.0f);
                *(float*)(r + 0x100 + k * 4) = RndF(0.0f, 30000.0f);
            }
            for (int k = 0; k < 0x35; ++k) *(uint16_t*)(r + 0x180 + k * 2) = (uint16_t)(Rnd() % 2000);
            for (int co = 0; co < 3; ++co) {
                uint8_t* l = lists[i][co];
                memset(l, 0, 0x20);
                int m = (int)(Rnd() % 12);
                *(int32_t*)(l + 2) = m;
                *(uint8_t**)(l + 6) = entries[i][co];
                *(float*)(l + 0x12) = RndF(0.0f, 5.0f);
                *(float*)(l + 0x16) = RndF(0.0f, 20000.0f);
                for (int k = 0; k < m; ++k) {
                    uint8_t* e = entries[i][co] + k * 0xd;
                    e[0] = Rnd() % 5 != 0;
                    *(int32_t*)(e + 1) = 1 + (int32_t)(Rnd() % n);
                    *(int32_t*)(e + 5) = Rnd() % 20 == 0 ? -(int32_t)(Rnd() % 50) : (int32_t)(Rnd() % 300);
                    *(float*)(e + 9) = Rnd() % 6 == 0 ? (float)(Rnd() % 3) * 0.25f : RndF(0.0f, 1.2f);
                }
                listPtrs[i][co] = l;
            }
            *(uint8_t***)(r + 0x5bd) = listPtrs[i];
            *(int32_t*)(r + 0x5c1) = Rnd() % 3 == 0 ? (int32_t)(Rnd() % 4) : 0;
            *(int32_t*)(r + 0x5c5) = Rnd() % 3 == 0 ? 0 : (int32_t)(Rnd() % 4000000);
        });
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        for (int k = 0; k < 0x35; ++k) *(int32_t*)(cargos + 0x9a + k * 4) = Rnd() % 6 == 0 ? 0 : -1;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1840 + Rnd() % 170);
        *(int32_t*)(world + 0x15) = (int32_t)(Rnd() % 3000000);
        int32_t company = (int32_t)(Rnd() % 3);
        uint32_t from = Rnd() % 8 == 0 ? 0x99 : 1 + Rnd() % n;
        g_ptBest = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % n);
        g_ptReach = (int32_t)Rnd();
        *(int32_t*)(train + 0x49) = (int32_t)(Rnd() % 6);
        *(int32_t*)(train + 0xa0) = 1 + (int32_t)(Rnd() % n);
        *(int32_t*)(train + 0x210) = Rnd() % 2 ? 2 : 1;
        *(float*)(train + 0x10) = RndF(0.0f, 300.0f);
        *(float*)(train + 0x14) = RndF(0.0f, 300.0f);
        uint8_t* tr = Rnd() % 3 == 0 ? nullptr : train;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t o[0x40];
            memset(o, 0xcd, sizeof o);
            int32_t r = 0;
            logs[side] = t.Run([&] {
                r = side ? rt3::Company_PickTrainOrder(&company, 0, o, from, tr)
                         : ((int32_t(__fastcall*)(int32_t*, int, uint8_t*, uint32_t, uint8_t*))0x408380)(&company, 0, o, from, tr);
            });
            char tail[0x100];
            int p = sprintf(tail, " ret %d", r);
            for (int k = 0; k < 0x34; ++k) p += sprintf(tail + p, " %02x", o[k]);
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "company pick train order", it);
    }
}

namespace rt3 {
void __fastcall Map_DrawCellLabel(uint8_t* m, int, float x, float y, float fade, int32_t cx, int32_t cy);
int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y);
int32_t __fastcall Chain_Contains(void* o, int, uint16_t k);
int32_t __fastcall Chain_AnyActive(void* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
void* __fastcall Obj_FirstActiveInChain(void* o);
void* __fastcall Obj_Building(void* o);
int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y);
double __fastcall Camera_TargetAboveGround(uint8_t* c);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y);
float __fastcall Obj_Level(void* o, int, int32_t i);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength);
}

namespace {
int32_t g_clFlags;
uint8_t g_clBuilding[0x200], g_clSite[0x100], g_clRegion[0x400];
int32_t __fastcall FakeClInBounds(uint8_t* c, int, float x, float y) { Logf("inbounds %08x %08x", B(x), B(y)); return g_clFlags & 1; }
void __fastcall RecClOverlay(uint8_t* m, int, float x, float y, uint32_t f, int32_t cx, int32_t cy) {
    Logf("overlay %08x %08x %08x %d %d", B(x), B(y), f, cx, cy);
}
int32_t __fastcall FakeClContains(void* o, int, uint16_t k) { Logf("contains %x", k); return (g_clFlags >> 1) & 1; }
int32_t __fastcall FakeClAny(void* o) { return (g_clFlags >> 2) & 1; }
int32_t __fastcall FakeClQuota(uint8_t* o, int, int32_t i) { Logf("quota %p %d", o, i); return (int32_t)((uintptr_t)o % 97) * 37 + i * 11; }
void* __fastcall FakeClFirst(void* o) { return (g_clFlags >> 3) & 1 ? g_clSite : nullptr; }
void* __fastcall FakeClBuilding(void* o) { Logf("building %p", o); return g_clBuilding; }
int32_t __fastcall FakeClNearest(Collection* t, int, float x, float y) {
    Logf("nearest %08x %08x", B(x), B(y));
    return (g_clFlags >> 4) & 1 ? (int32_t)rt3::Collection_IdAt(t, 0, 0, 0) : 0;
}
double __fastcall FakeClAbove(uint8_t* c) { return 123.25; }
void* __fastcall FakeClRenderer(void* e) { return (void*)0x5150; }
int32_t __fastcall FakeClProject(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    Logf("project %p %08x %08x %08x", s, B(x), B(y), B(z));
    *ox = x * 0.001f;
    *oy = y * 0.002f;
    *oz = 0.5f;
    return 1;
}
uint8_t* __fastcall FakeClRegionAt(void* g, int, int32_t x, int32_t y) { Logf("region %d %d", x, y); return g_clRegion; }
float __fastcall FakeClLevel(void* o, int, int32_t i) { Logf("level %d", i); return i * 0.37f; }
void* __fastcall FakeClGet1C(void* e) { return (void*)0x1c1c; }
void __fastcall FakeClFog(uint8_t* o, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength) {
    Logf("fog %d %d %08x", x, z, B(bias));
    *scale = 0.75f + bias * 0.01f;
    *color = 0xf0f0;
    *strength = 0.25f;
}
} // namespace

DIFFTEST(ui_MapCellLabel) {
    SavedGlobals sg({ 0x62be68, 0x6cec20, 0x62bae0, 0x62ba8c, 0x6d4024, 0x6d401c, 0x6cfe18, 0xd93850, 0x625464 });
    UiTrace t;
    t.Add(0x52d0f0, (void*)&rt3::Camera_InBounds, (void*)&FakeClInBounds);
    t.Add(0x4515c0, (void*)&rt3::Map_DrawCellOverlay, (void*)&RecClOverlay);
    t.Add(0x42c7d0, (void*)&rt3::Chain_Contains, (void*)&FakeClContains);
    t.Add(0x42c8d0, (void*)&rt3::Chain_AnyActive, (void*)&FakeClAny);
    t.Add(0x42c960, (void*)&rt3::City_CargoQuota, (void*)&FakeClQuota);
    t.Add(0x47de20, (void*)&rt3::Obj_BuildingRegion, (void*)&FakePtRegion);
    t.Add(0x47df00, (void*)&rt3::Obj_BuildingCellY, (void*)&FakePtCellY);
    t.Add(0x47ded0, (void*)&rt3::Obj_BuildingCellX, (void*)&FakePtCellX);
    t.Add(0x42c880, (void*)&rt3::Obj_FirstActiveInChain, (void*)&FakeClFirst);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeClBuilding);
    t.Add(0x422010, (void*)&rt3::Table_NearestValue23a, (void*)&FakeClNearest);
    t.Add(0x439d30, (void*)&rt3::Camera_TargetAboveGround, (void*)&FakeClAbove);
    t.Add(0x51f070, (void*)&rt3::Engine_Renderer, (void*)&FakeClRenderer);
    t.Add(0x527450, (void*)&rt3::Scene_ProjectNormalised, (void*)&FakeClProject);
    t.Add(0x40ccd0, (void*)&rt3::Grid_RegionAt, (void*)&FakeClRegionAt);
    t.Add(0x42b380, (void*)&rt3::Obj_Level, (void*)&FakeClLevel);
    t.Add(0x51f090, (void*)&rt3::Engine_Get1C, (void*)&FakeClGet1C);
    t.Add(0x5371f0, (void*)&rt3::Fog_ParamsAt, (void*)&FakeClFog);
    t.Add(0x47e690, (void*)&rt3::Train_GatherLoads, (void*)&FakeScGather);
    static uint8_t map[0x2200], cells[16 * 0x1d7], view[0x40], cargos[0x200], ui[0xd00], preview[0x100];
    hook::Global<uint8_t*>(0x62be68) = view;
    hook::Global<uint8_t*>(0x62ba8c) = cargos;
    static uint8_t engine[0x114800]; // Num_Format reads +0x114732
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    *(uint8_t**)(map + 0x2129) = cells;
    *(int32_t*)(map + 0x2145) = 4;
    *(int32_t*)(map + 0x15d9) = 14;
    *(int32_t*)(map + 0x15d5) = 13;
    size_t total = 0;
    for (int it = 0; it < 6000; ++it) {
        FakeTable stations(3, 0x600, [](uint8_t* r, int i) {
            sprintf((char*)r + 0x46b, "Station %d", i);
            *(float*)(r + 0x24) = (float)(Rnd() % 16);
            *(float*)(r + 0x28) = (float)(Rnd() % 16);
            r[0x40] = (uint8_t)Rnd();
            r[0x41] = (uint8_t)Rnd();
        });
        FakeTable cities(2, 0x100, [](uint8_t*, int) {});
        hook::Global<Collection*>(0x6cec20) = &stations.c;
        hook::Global<Collection*>(0x62bae0) = &cities.c;
        g_clFlags = Rnd() % 8 == 0 ? 0 : (int32_t)(Rnd() | 1);
        int32_t cx = (int32_t)(Rnd() % 16), cy = (int32_t)(Rnd() % 16);
        uint8_t* cell = cells + ((cy / 4) * 4 + cx / 4) * 0x1d7;
        cell[0xe6] = Rnd() % 2 ? 0x10 : 0;
        *(int32_t*)(cell + 0xeb) = (int32_t)(Rnd() % 3) - 1;
        *(int16_t*)(cell + 0xd4) = (int16_t)(Rnd() % 2);
        *(int32_t*)(map + 0x2171) = 1 + (int32_t)(Rnd() % 6); // other modes draw an unset buffer
        *(int32_t*)(map + 0x2179) = Rnd() % 6 == 0 ? 0 : (int32_t)(Rnd() % 60);
        *(int32_t*)(map + 0x2181) = Rnd() % 6 == 0 ? 0 : 1 + (int32_t)(Rnd() % 3);
        for (int k = 0; k < 0x35; ++k) *(int32_t*)(cargos + 0x9a + k * 4) = Rnd() % 2 ? 0 : -1;
        for (int k = 0; k < 8; ++k) *(uint16_t*)(g_clRegion + 0xf3 + k * 2) = (uint16_t)Rnd();
        for (int k = 0; k < 0x35; ++k) *(float*)(g_clRegion + 0x103 + k * 4) = RndF(0.0f, 9.0f);
        *(float*)(g_clBuilding + 0x1e2) = RndF(0.0f, 500.0f);
        *(float*)(g_clBuilding + 0x1e6) = RndF(0.0f, 50.0f);
        *(float*)(g_clBuilding + 0x1ea) = RndF(0.0f, 500.0f);
        g_clSite[0x40] = (uint8_t)Rnd();
        g_clSite[0x41] = (uint8_t)Rnd();
        *(float*)(view + 0x14) = RndF(0.2f, 2.0f);
        *(int32_t*)(ui + 0xc64) = (int32_t)(Rnd() % 3) - 1;
        hook::Global<uint8_t*>(0x6cfe18) = Rnd() % 6 == 0 ? nullptr : preview;
        *(float*)(preview + 0x80) = (float)(Rnd() % 16) + RndF(-0.6f, 0.6f);
        *(float*)(preview + 0x84) = (float)(Rnd() % 16) + RndF(-0.6f, 0.6f);
        *(int32_t*)(preview + 0x78) = (int32_t)(Rnd() % 4);
        float x = RndF(0.0f, 1.0f), y = Rnd() % 4 == 0 ? 0.05f : RndF(0.0f, 1.0f), fade = RndF(0.0f, 1.0f);
        float f0 = RndF(0.0f, 1.0f), f1 = RndF(0.0f, 1.0f);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<float>(0xd93850) = f0;
            hook::Global<float>(0x625464) = f1;
            logs[side] = t.Run([&] {
                if (side) rt3::Map_DrawCellLabel(map, 0, x, y, fade, cx, cy);
                else ((void(__fastcall*)(uint8_t*, int, float, float, float, int32_t, int32_t))0x452100)(map, 0, x, y, fade, cx, cy);
                Logf("fade %08x %08x", hook::Global<uint32_t>(0xd93850), hook::Global<uint32_t>(0x625464));
            });
        }
        CompareLogs(logs[0], logs[1], "map cell label", it);
        total += logs[0].size();
    }
    fprintf(g_out, "  [map cell label: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
struct StringList;
StringList* __fastcall Strings_LoadFile(const char* path);
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void BuildNameIndex();
}

namespace {
std::string g_lfText;
size_t g_lfPos;
int32_t __fastcall FakeLfOpen(const char* path, int32_t mode, int32_t size, int32_t fatal) {
    Logf("open %s %d %d %d", path, mode, size, fatal);
    g_lfPos = 0;
    return 7;
}
int32_t __fastcall FakeLfRead(int32_t s, void* dst, uint32_t size, int32_t a, int32_t b) {
    if (g_lfPos >= g_lfText.size()) return 0;
    *(char*)dst = g_lfText[g_lfPos++];
    return 1;
}
void __fastcall RecLfClose(int32_t s) { Logf("close %d", s); }
void __fastcall RecLfLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i) {
    Logf("log [%s] %d %d", text, a, i);
}
void RecLfIndex() { Logf("index"); }
void __fastcall RecLfAssign(char** d, const char* src) {
    Logf("assign [%s]", src ? src : "(null)");
    if (*d) ((void(__cdecl*)(void*))0x5a1145)(*d);
    *d = nullptr;
    if (!src) return;
    size_t n = strlen(src) + 1;
    *d = (char*)((void*(__cdecl*)(size_t))0x5a125d)(n);
    memcpy(*d, src, n);
}
} // namespace

DIFFTEST(text_LoadLanguageFile) {
    SavedGlobals sg({ 0x6d3c8c });
    UiTrace t;
    t.Add(0x530c80, (void*)&rt3::StreamOpen, (void*)&FakeLfOpen);
    t.Add(0x531150, (void*)&rt3::StreamRead, (void*)&FakeLfRead);
    t.Add(0x530a00, (void*)&rt3::StreamClose, (void*)&RecLfClose);
    t.Add(0x51d680, (void*)&rt3::DebugLog, (void*)&RecLfLog);
    t.Add(0x51b660, (void*)&rt3::BuildNameIndex, (void*)&RecLfIndex);
    t.Add(0x51d820, (void*)&rt3::String_Assign, (void*)&RecLfAssign);
    static const char* pieces[] = { " ", "\t", "x", "{", "}", "[", "]", "\\n", "\\", "n", "\"", ";", "/", "a b", "Hello", "\xe9" };
    for (int it = 0; it < 1500; ++it) {
        std::string f;
        int lines = (int)(Rnd() % 14);
        const char* eol = Rnd() % 2 ? "\r\n" : "\n";
        for (int k = 0; k < lines; ++k) {
            int kind = (int)(Rnd() % 12);
            char tmp[64];
            if (kind == 0) f += "; comment";
            else if (kind == 1) f += "// another";
            else if (kind == 2) f += "";
            else if (kind == 3) f += "garbage";
            else if (kind == 4) { sprintf(tmp, "%d \"", Rnd() % 2 ? 0 : 20000); f += tmp; f += "bad\""; }
            else if (kind == 5) f += Rnd() % 2 ? "4040 \"\"" : "4040 \"raw\"";
            else {
                sprintf(tmp, "%s%d%s", Rnd() % 4 == 0 ? "  " : "", 1 + Rnd() % 60, Rnd() % 5 == 0 ? "" : " ");
                f += tmp;
                if (Rnd() % 8) f += "\"";
                int m = (int)(Rnd() % 6);
                for (int q = 0; q < m; ++q) f += pieces[Rnd() % 16];
                if (Rnd() % 6) f += "\"";
                if (Rnd() % 4 == 0) f += Rnd() % 2 ? "  ; note" : " junk";
            }
            if (k + 1 < lines || Rnd() % 2) f += Rnd() % 6 == 0 ? "\r" : eol;
        }
        g_lfText = f;
        hook::Global<int32_t>(0x6d3c8c) = Rnd() % 3 == 0 ? 3 : 0;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            int32_t* l = nullptr;
            logs[side] = t.Run([&] {
                l = side ? (int32_t*)rt3::Strings_LoadFile("lang.txt") : ((int32_t*(__fastcall*)(const char*))0x51c020)("lang.txt");
            });
            char tmp[64];
            sprintf(tmp, " count %d", l[0]);
            logs[side] += tmp;
            char** items = (char**)l[1];
            for (int k = 0; k < l[0]; ++k) {
                if (!items[k]) continue;
                sprintf(tmp, " %d:", k);
                logs[side] += tmp;
                logs[side] += items[k];
                ((void(__cdecl*)(void*))0x5a1145)(items[k]);
            }
            ((void(__cdecl*)(void*))0x5a1145)(items);
            ((void(__cdecl*)(void*))0x5a1145)(l);
        }
        CompareLogs(logs[0], logs[1], "load language file", it);
    }
}

namespace rt3 {
void Language_Load();
void Language_Select();
void __fastcall DebugLog_SetFile(const char* name);
void DebugLog_WriteHeader();
void __fastcall StringList_Free(StringList* l);
void __fastcall FreeAndNull(void** p);
}

namespace {
struct LlList { int32_t count; char** items; };
LlList g_llList;
char* g_llItems[0x1000];
void RecLlSelect() { Logf("select"); }
void __fastcall RecLlSetFile(const char* n) { Logf("logfile %s", n); }
void RecLlHeader() { Logf("header"); }
void __fastcall RecLlFree(rt3::StringList* l) { Logf("free list %p", l); }
void __fastcall RecLlFreeNull(void** p) {
    Logf("free and null %p", p);
    *p = nullptr;
}
void* __fastcall FakeLlLoad(const char* path) {
    Logf("load [%s]", path);
    return &g_llList;
}
char* __cdecl FakeLlLocale(int cat, const char* name) {
    Logf("setlocale %d %s", cat, name);
    return nullptr;
}
} // namespace

DIFFTEST(text_LanguageLoad) {
    SavedGlobals sg({ 0x6d3c9c, 0x6d3c90, 0x6d3c94, 0x6d3c84, 0x6d3c8c, 0x6d4034, 0x6d3c88, 0x624328, 0x62432c });
    UiTrace t;
    t.Add(0x518fa0, (void*)&rt3::Language_Select, (void*)&RecLlSelect);
    t.Add(0x51e720, (void*)&rt3::DebugLog_SetFile, (void*)&RecLlSetFile);
    t.Add(0x51e740, (void*)&rt3::DebugLog_WriteHeader, (void*)&RecLlHeader);
    t.Add(0x518f60, (void*)&rt3::StringList_Free, (void*)&RecLlFree);
    t.Add(0x53afd0, (void*)&rt3::FreeAndNull, (void*)&RecLlFreeNull);
    t.Add(0x51c020, (void*)&rt3::Strings_LoadFile, (void*)&FakeLlLoad);
    t.Add(0x5a3c18, nullptr, (void*)&FakeLlLocale);
    static const char* bonds[] = { "Issue Bonds", "xx Anleihen ausgeben", "Emettre des titres", "Emetti obbligazioni", "Emitir bonos",
                                   "Emituj obligacje", "nothing", (const char*)0x5dcd5c, (const char*)0x5dcd50, (const char*)0x5cec80, "" };
    static const char* charsets[] = { "", "128", "129", "134", "136", "222", "0", "130" };
    static const char* seps[] = { "", ",", ".", " ", "'" };
    for (int it = 0; it < 600; ++it) {
        memset(g_llItems, 0, sizeof g_llItems);
        g_llList.count = 0x1000;
        g_llList.items = g_llItems;
        g_llItems[0x5be] = (char*)bonds[Rnd() % 11];
        g_llItems[0xfc8] = (char*)(Rnd() % 3 == 0 ? "ab" : Rnd() % 2 ? "a" : "");
        g_llItems[0xfc9] = (char*)charsets[Rnd() % 8];
        g_llItems[0x283] = (char*)seps[Rnd() % 5];
        g_llItems[0x284] = (char*)seps[Rnd() % 5];
        int32_t loaded = Rnd() % 8 == 0;
        const char* folder = Rnd() % 2 ? "english" : nullptr;
        bool hadList = Rnd() % 2;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<int32_t>(0x6d3c9c) = loaded;
            hook::Global<const char*>(0x6d3c90) = folder;
            hook::Global<void*>(0x6d3c94) = hadList ? (void*)0x1234 : nullptr;
            hook::Global<int32_t>(0x6d3c84) = 7;
            hook::Global<int32_t>(0x6d3c8c) = 0;
            hook::Global<int32_t>(0x6d4034) = 0;
            hook::Global<int32_t>(0x6d3c88) = 5;
            hook::Global<int32_t>(0x624328) = 0;
            hook::Global<uint16_t>(0x62432c) = 0x2c2e;
            logs[side] = t.Run([&] { side ? rt3::Language_Load() : ((void(*)())0x51cf80)(); });
            char tail[128];
            sprintf(tail, " %d %d %d %d %d %x %p", hook::Global<int32_t>(0x6d3c84), hook::Global<int32_t>(0x6d3c8c), hook::Global<int32_t>(0x6d4034),
                    hook::Global<int32_t>(0x6d3c88), hook::Global<int32_t>(0x624328), hook::Global<uint16_t>(0x62432c), hook::Global<void*>(0x6d3c94));
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "language load", it);
    }
}

namespace rt3 {
void __fastcall TerrainLod_Build(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEnd, int32_t vEnd);
void __fastcall TerrainLod_Mark(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEndArg, int32_t vEndArg);
int32_t __fastcall Render_ProjectClip(uint8_t* r, int, float x, float y, float z, float limit, float maxZ, float* out);
int32_t __fastcall Render_GridCell(uint8_t* r, int, int32_t x, int32_t y);
void* __fastcall Terrain_HeightsSlot(uint8_t* r);
int32_t __fastcall Terrain_Rows(uint8_t* r);
uint32_t __fastcall Terrain_Get1619(uint8_t* r, int, int8_t i);
}

namespace {
uint32_t g_tlSeed;
float* g_tlSlots[4];
uint16_t g_tlVar[0x2000];
void __fastcall RecTlMark(uint8_t* self, int, int32_t level, uint8_t* t, int32_t u0, int32_t v0, int32_t u1, int32_t v1) {
    Logf("mark %d %d %d %d %d", (int8_t)level, u0, v0, u1, v1);
}
int32_t __fastcall FakeTlClip(uint8_t* r, int, float x, float y, float z, float limit, float maxZ, float* out) {
    Logf("clip %08x %08x %08x %08x %08x %p", B(x), B(y), B(z), B(limit), B(maxZ), out);
    return ((B(x) ^ B(z) * 7 ^ g_tlSeed) >> 5) % 3 == 0;
}
int32_t __fastcall FakeTlCell(uint8_t* r, int, int32_t x, int32_t y) { return x * 1000 + y; }
void* __fastcall FakeTlSlots(uint8_t* r) { return g_tlSlots; }
int32_t __fastcall FakeTlRows(uint8_t* r) { return 63; }
uint32_t __fastcall FakeTl1619(uint8_t* r, int, int8_t i) { Logf("var %d", i); return (uint32_t)(uintptr_t)g_tlVar; }
} // namespace

DIFFTEST(render_TerrainLodBuild) {
    UiTrace t;
    t.Add(0x526670, (void*)&rt3::TerrainLod_Mark, (void*)&RecTlMark);
    t.Add(0x5468c0, (void*)&rt3::Render_ProjectClip, (void*)&FakeTlClip);
    t.Add(0x5329e0, (void*)&rt3::Render_GridCell, (void*)&FakeTlCell);
    t.Add(0x533b10, (void*)&rt3::Terrain_HeightsSlot, (void*)&FakeTlSlots);
    t.Add(0x533970, (void*)&rt3::Terrain_Rows, (void*)&FakeTlRows);
    t.Add(0x533b80, (void*)&rt3::Terrain_Get1619, (void*)&FakeTl1619);
    const int DIST = 0x22000, THR = 0x400;
    static uint16_t savedDist[DIST];
    static int16_t savedThr[THR];
    memcpy(savedDist, (void*)0x6d9098, sizeof savedDist);
    memcpy(savedThr, (void*)0xc33d38, sizeof savedThr);
    float sScale = hook::Global<float>(0x6d9090), s518 = hook::Global<float>(0x8f2518);
    int32_t sOffset = hook::Global<int32_t>(0xaf3528);
    static uint8_t self[0x22c00], settings[0x1f420], grid[2][0x8000], tt[0x100];
    static float heights[4][0x4000];
    for (int l = 0; l < 4; ++l) g_tlSlots[l] = heights[l];
    *(uint8_t**)(self + 0x65) = settings;
    *(uint8_t**)(self + 0x2d) = (uint8_t*)0x2d2d;
    for (int it = 0; it < 800; ++it) {
        uint16_t* dist = (uint16_t*)0x6d9098;
        for (int k = 0; k < DIST; ++k) dist[k] = (uint16_t)(Rnd() % 4 == 0 ? Rnd() % 0x40 : Rnd() % 3000);
        int16_t* thr = (int16_t*)0xc33d38;
        for (int k = 0; k < THR; ++k) thr[k] = (int16_t)(Rnd() % 1500);
        hook::Global<float>(0x6d9090) = 0.1f;
        hook::Global<float>(0x8f2518) = RndF(0.0f, 3.0f); // times 185 (0x5dd000)
        for (auto& lv : heights)
            for (float& h : lv) h = RndF(0.0f, 80.0f);
        for (uint16_t& v : g_tlVar) v = (uint16_t)(Rnd() % 2000);
        *(int32_t*)(settings + 0x1f418) = 1 + (int32_t)(Rnd() % 2);
        *(int32_t*)(self + 0x3682) = (int32_t)(Rnd() % 64);
        *(int32_t*)(self + 0x3686) = (int32_t)(Rnd() % 64);
        *(float*)(self + 0x22b5b) = RndF(0.5f, 2.0f);
        g_tlSeed = Rnd();
        int32_t level = 1 + (int32_t)(Rnd() % 3);
        int32_t u0 = (int32_t)(Rnd() % 4), v0 = (int32_t)(Rnd() % 4);
        int32_t uEnd = u0 + (int32_t)(Rnd() % 5), vEnd = v0 + (int32_t)(Rnd() % 5);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(grid[side], 0, sizeof grid[side]);
            *(uint8_t**)(settings + 0x1f414) = grid[side];
            memset(self + 0x374f, 0xcc, 0x400);
            *(int32_t*)(self + 0x22b53) = 0;
            logs[side] = t.Run([&] {
                if (side) rt3::TerrainLod_Build(self, 0, level, tt, u0, v0, uEnd, vEnd);
                else ((void(__fastcall*)(uint8_t*, int, int32_t, uint8_t*, int32_t, int32_t, int32_t, int32_t))0x526b70)(self, 0, level, tt, u0, v0,
                                                                                                                     uEnd, vEnd);
            });
            char tail[64];
            sprintf(tail, " n %d off %d", *(int32_t*)(self + 0x22b53), hook::Global<int32_t>(0xaf3528));
            logs[side] += tail;
            for (int k = 0; k < *(int32_t*)(self + 0x22b53) * 4; ++k) {
                sprintf(tail, " %d", *(int32_t*)(self + 0x374f + k * 4));
                logs[side] += tail;
            }
        }
        CompareLogs(logs[0], logs[1], "terrain lod build", it);
        EXPECT_EQ(memcmp(grid[0], grid[1], sizeof grid[0]), 0, "terrain lod grid iteration %d", it);
    }
    memcpy((void*)0x6d9098, savedDist, sizeof savedDist);
    memcpy((void*)0xc33d38, savedThr, sizeof savedThr);
    hook::Global<float>(0x6d9090) = sScale;
    hook::Global<float>(0x8f2518) = s518;
    hook::Global<int32_t>(0xaf3528) = sOffset;
}

namespace rt3 {
uint8_t* __fastcall Game_InitStart(uint8_t* g);
void StreamInit();
void __fastcall Fatal_Exit(const char* msg);
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
}

namespace {
uint32_t g_giDrives, g_giFiles;
uint32_t __stdcall FakeGiDriveType(const char* root) {
    Logf("drive %s", root);
    return (g_giDrives >> (root[0] - 'A')) & 1 ? 5 : 3;
}
int __cdecl FakeGiOpen(const char* path, int mode) {
    Logf("open %s %x", path, mode);
    uint32_t h = 0;
    for (const char* p = path; *p; ++p) h = h * 31 + (uint8_t)*p;
    return (g_giFiles >> (h % 32)) & 1 ? (int)(h % 100) : -1;
}
int __cdecl RecGiClose(int fd) { Logf("close %d", fd); return 0; }
void RecGiStreams() { Logf("streams"); }
void RecGiLanguage() { Logf("language"); }
void __fastcall RecGiFatal(const char* m) { Logf("fatal %s", m); }
int32_t g_giTaiwan;
const char* __fastcall FakeGiFind(const char* h, const char* n, int32_t cs) { Logf("find"); return g_giTaiwan ? h : nullptr; }
} // namespace

DIFFTEST(game_InitStart) {
    SavedGlobals sg({ 0x625a90, 0xd97968, 0x6d3c84 });
    static uint8_t table[0x25b * 4 + 3];
    memcpy(table, (void*)0x5ee8e0, sizeof table);
    UiTrace t;
    t.Add(0x5a4c57, nullptr, (void*)&FakeGiOpen);
    t.Add(0x5a440d, nullptr, (void*)&RecGiClose);
    t.Add(0x5309d0, (void*)&rt3::StreamInit, (void*)&RecGiStreams);
    t.Add(0x51cf80, (void*)&rt3::Language_Load, (void*)&RecGiLanguage);
    t.Add(0x51e980, (void*)&rt3::Fatal_Exit, (void*)&RecGiFatal);
    t.Add(0x51d900, (void*)&rt3::StrFind, (void*)&FakeGiFind);
    void** iat = (void**)0x5c806c;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeGiDriveType;
    static uint8_t g[0x600];
    for (int it = 0; it < 2000; ++it) {
        g_giDrives = Rnd() % 4 == 0 ? 0 : Rnd() & Rnd();
        g_giFiles = Rnd() & Rnd();
        g_giTaiwan = Rnd() % 5 == 0;
        int32_t lang = Rnd() % 6 == 0 ? 12 : (int32_t)(Rnd() % 4);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (uint8_t& b : g) b = 0x5a;
            hook::Global<int32_t>(0x625a90) = -7;
            hook::Global<int32_t>(0x6d3c84) = lang;
            memset((void*)0x5ee8e0, 0, sizeof table);
            uint8_t* r = nullptr;
            logs[side] = t.Run([&] { r = side ? rt3::Game_InitStart(g) : ((uint8_t*(__fastcall*)(uint8_t*))0x481fd0)(g); });
            char tail[96];
            sprintf(tail, " ret %d drive %d %x %d %d %d %02x", r == g, hook::Global<int32_t>(0x625a90), hook::Global<uint32_t>(0xd97968),
                    *(int32_t*)(g + 0x18), *(int32_t*)(g + 0x1c), *(int32_t*)(g + 0x5c), *(uint8_t*)0x5ee8e0);
            logs[side] += tail;
            uint32_t h = 0;
            for (uint8_t b : g) h = h * 33 + b;
            logs[side] += " " + std::to_string(h);
        }
        CompareLogs(logs[0], logs[1], "game init start", it);
    }
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    memcpy((void*)0x5ee8e0, table, sizeof table);
}

namespace rt3 {
void __fastcall TerrainSkirt_Build(uint8_t* self, int, uint8_t* t);
int32_t __fastcall VisitGrid_Log2(uint8_t* g);
uint8_t* __fastcall VisitGrid_Flags(uint8_t* g);
uint32_t __fastcall Terrain_Get15e5(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
uint32_t __fastcall Terrain_Get165d(uint8_t* r);
}

namespace {
int32_t g_tsShift, g_tsW, g_tsH;
uint8_t g_tsGrid[0x4000], g_tsAttr[0x4000];
uint32_t g_tsXs[0x100];
float* g_tsSlots[8];
int32_t __fastcall FakeTsShift(uint8_t* g) { return g_tsShift; }
uint8_t* __fastcall FakeTsGrid(uint8_t* g) { return g_tsGrid; }
void* __fastcall FakeTsSlots(uint8_t* r) { return g_tsSlots; }
uint32_t __fastcall FakeTsXs(uint8_t* r) { return (uint32_t)(uintptr_t)g_tsXs; }
int32_t __fastcall FakeTsW(uint8_t* r) { return g_tsW; }
int32_t __fastcall FakeTsH(uint8_t* r) { return g_tsH; }
uint32_t __fastcall FakeTsAttr(uint8_t* r) { return (uint32_t)(uintptr_t)(g_tsAttr + 3); }
} // namespace

DIFFTEST(render_TerrainSkirt) {
    UiTrace t;
    t.Add(0x5654f0, (void*)&rt3::VisitGrid_Log2, (void*)&FakeTsShift);
    t.Add(0x565500, (void*)&rt3::VisitGrid_Flags, (void*)&FakeTsGrid);
    t.Add(0x533b10, (void*)&rt3::Terrain_HeightsSlot, (void*)&FakeTsSlots);
    t.Add(0x533b70, (void*)&rt3::Terrain_Get15e5, (void*)&FakeTsXs);
    t.Add(0x533970, (void*)&rt3::Terrain_Rows, (void*)&FakeTsW);
    t.Add(0x533980, (void*)&rt3::Terrain_Cols, (void*)&FakeTsH);
    t.Add(0x533ae0, (void*)&rt3::Terrain_Get165d, (void*)&FakeTsAttr);
    static float heights[8][0x2000];
    for (int l = 0; l < 8; ++l) g_tsSlots[l] = heights[l];
    static uint8_t self[0x3ac00], settings[0x10], tt[0x200];
    *(uint8_t**)(self + 0x65) = settings;
    static uint8_t outs[2][0x18000];
    for (int it = 0; it < 1500; ++it) {
        g_tsShift = 5 + (int32_t)(Rnd() % 2);
        g_tsW = 4 + (int32_t)(Rnd() % 20);
        g_tsH = 4 + (int32_t)(Rnd() % 20);
        for (uint8_t& g : g_tsGrid) g = Rnd() % 3 == 0 ? (uint8_t)(1 + Rnd() % 4) : 0;
        for (uint8_t& a : g_tsAttr) a = (uint8_t)Rnd();
        for (uint32_t& x : g_tsXs) x = B(RndF(0.0f, 5000.0f));
        for (auto& lv : heights)
            for (float& h : lv) h = RndF(0.0f, 100.0f);
        *(float*)(self + 0x25) = RndF(0.0f, 10.0f);
        *(float*)(self + 0x369c) = RndF(-50.0f, 2500.0f);
        *(float*)(self + 0x36a4) = RndF(-50.0f, 2500.0f);
        uint8_t* tp = tt + 1 + Rnd() % 3; // the low byte seeds a carried level
        for (int side = 0; side < 2; ++side) {
            memset(self + 0x22bb7, 0xcd, sizeof outs[0]);
            for (uint32_t o = 0x3abb7; o <= 0x3abd7; o += 4) *(int32_t*)(self + o) = 0x77;
            t.Run([&] {
                if (side) rt3::TerrainSkirt_Build(self, 0, tp);
                else ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x524e60)(self, 0, tp);
            });
            memcpy(outs[side], self + 0x22bb7, sizeof outs[0]);
            memcpy(outs[side] + sizeof outs[0] - 0x24, self + 0x3abb7, 0x24);
        }
        int d = -1;
        for (int k = 0; k < (int)sizeof outs[0]; ++k)
            if (outs[0][k] != outs[1][k]) {
                d = k;
                break;
            }
        EXPECT_EQ(d, -1, "terrain skirt iteration %d: first difference +0x%x", it, d);
    }
}

namespace rt3 {
int32_t __fastcall Track_PlanCurve(uint8_t* t, int, uint32_t aId, uint32_t bId, uint32_t p4, uint32_t p5, uint32_t** out);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
int32_t __fastcall Track_HasLink(uint8_t* t, int, uint8_t end);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
double __stdcall AngleOf(float x0, float y0, float x1, float y1);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
}

namespace {
void __fastcall FakePcCenter(void* o, int, float* x, float* y) {
    Logf("center %p", o);
    *x = *(float*)((uint8_t*)o + 0x10);
    *y = *(float*)((uint8_t*)o + 0x14);
}
void __fastcall FakePcVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
    Logf("vectors %p %d", e, c != nullptr);
    memcpy(a, e + 0x18, 12);
    memcpy(b, e + 0x24, 12);
    memcpy(d, e + 0x30, 12);
}
int32_t __fastcall FakePcLink(uint8_t* t, int, uint8_t end) {
    int32_t r = (t[0xc] >> end) & 1;
    Logf("link %p %d -> %d", t, end, r);
    return r;
}
// Not logged: with an overlapping crossing tangent the original asks for the
// angle to its unset end points (stack garbage), results unused.
double __stdcall FakePcAngle(float x0, float y0, float x1, float y1) {
    return atan2((double)x1 - x0, (double)y1 - y0);
}
double __fastcall FakePcDelta(int32_t* w, int, float a, float b) {
    Logf("delta %08x %08x", B(a), B(b));
    return (double)a - b;
}
double __stdcall FakePcDist(float x0, float y0, float x1, float y1) {
    Logf("dist %08x %08x %08x %08x", B(x0), B(y0), B(x1), B(y1));
    return sqrt(((double)x1 - x0) * ((double)x1 - x0) + ((double)y1 - y0) * ((double)y1 - y0));
}
double __stdcall FakePcDistSq(float x0, float y0, float x1, float y1) {
    Logf("distsq %08x %08x %08x %08x", B(x0), B(y0), B(x1), B(y1));
    return ((double)x1 - x0) * ((double)x1 - x0) + ((double)y1 - y0) * ((double)y1 - y0);
}
// The pieces live in the test arena, so the lists get a buffer of their own
// (reset before each side), addressed by offset in the log.
uint8_t g_pcHeap[0x400];
uint32_t g_pcUsed;
void* __cdecl RecPcMalloc(size_t n) {
    void* p = g_pcHeap + g_pcUsed;
    g_pcUsed += (uint32_t)n;
    Logf("malloc %x -> +%x", (uint32_t)n, (uint32_t)((uint8_t*)p - g_pcHeap));
    return p;
}
} // namespace

DIFFTEST(game_TrackPlanCurve) {
    UiTrace t;
    t.Add(0x48a170, (void*)&rt3::Obj_CenterCell, (void*)&FakePcCenter);
    t.Add(0x5394b0, (void*)&rt3::Effect_GetVectors, (void*)&FakePcVectors);
    t.Add(0x48a130, (void*)&rt3::Track_HasLink, (void*)&FakePcLink);
    t.Add(0x5a125d, nullptr, (void*)&RecPcMalloc);
    t.Add(0x4952f0, (void*)&rt3::AngleOf, (void*)&FakePcAngle);
    t.Add(0x4953c0, (void*)&rt3::AngleDelta, (void*)&FakePcDelta);
    t.Add(0x51db80, (void*)&rt3::Distance2D, (void*)&FakePcDist);
    t.Add(0x51dbb0, (void*)&rt3::DistanceSq2D, (void*)&FakePcDistSq);
    static uint8_t builder[0x200];
    int found = 0;
    for (int it = 0; it < 4000; ++it) {
        float ox = RndF(500.0f, 4000.0f), oz = RndF(500.0f, 4000.0f);
        FakeTable pieces(3, 0x40, [&](uint8_t* r, int i) {
            r[0xc] = (uint8_t)(Rnd() % 8);
            float bx = (ox + RndF(-1200.0f, 1200.0f)) / 92.0f, bz = (oz + RndF(-1200.0f, 1200.0f)) / 92.0f;
            *(float*)(r + 0x10) = bx;
            *(float*)(r + 0x14) = bz;
            float ang = RndF(-3.2f, 3.2f), len = RndF(20.0f, 200.0f);
            float* q = (float*)(r + 0x18);
            q[0] = bx * 92.0f - sinf(ang) * len;
            q[1] = 0.0f;
            q[2] = bz * 92.0f - cosf(ang) * len;
            q[3] = RndF(-50.0f, 50.0f) + q[0];
            q[4] = 0.0f;
            q[5] = RndF(-50.0f, 50.0f) + q[2];
            q[6] = bx * 92.0f + sinf(ang) * len;
            q[7] = 0.0f;
            q[8] = bz * 92.0f + cosf(ang) * len;
            if (Rnd() % 10 == 0) q[8] = q[2]; // equal coordinates hit the 0.0001 guards
        });
        memcpy(builder, &pieces.c, sizeof pieces.c);
        float ang = RndF(-3.2f, 3.2f), len = RndF(20.0f, 200.0f);
        *(float*)(builder + 0x139) = ox - sinf(ang) * len;
        *(float*)(builder + 0x141) = oz - cosf(ang) * len;
        *(float*)(builder + 0x15d) = ox + sinf(ang) * len;
        *(float*)(builder + 0x165) = Rnd() % 10 == 0 ? *(float*)(builder + 0x141) : oz + cosf(ang) * len;
        *(float*)(builder + 0x151) = *(float*)(builder + 0x15d);
        *(float*)(builder + 0x159) = *(float*)(builder + 0x165);
        *(float*)(builder + 0x145) = RndF(-50.0f, 50.0f) + *(float*)(builder + 0x139);
        *(float*)(builder + 0x14d) = RndF(-50.0f, 50.0f) + *(float*)(builder + 0x141);
        uint32_t a = rt3::Collection_IdAt(&pieces.c, 0, Rnd() % 3, 0), b = Rnd() % 12 == 0 ? a : rt3::Collection_IdAt(&pieces.c, 0, Rnd() % 3, 0);
        uint32_t p4 = Rnd(), p5 = Rnd();
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            uint32_t* out = (uint32_t*)0x1111;
            int32_t ret = 0;
            g_pcUsed = 0;
            logs[side] = t.Run([&] {
                ret = side ? rt3::Track_PlanCurve(builder, 0, a, b, p4, p5, &out)
                           : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t**))0x498320)(builder, 0, a, b, p4, p5, &out);
            });
            char tail[160];
            bool own = (uint8_t*)out >= g_pcHeap && (uint8_t*)out < g_pcHeap + sizeof g_pcHeap;
            sprintf(tail, " ret %d out %x", ret, own ? (uint32_t)((uint8_t*)out - g_pcHeap) : (uint32_t)(uintptr_t)out);
            logs[side] += tail;
            if (own) {
                for (uint32_t* n = out; n; n = (uint32_t*)(uintptr_t)n[2]) {
                    sprintf(tail, " [%08x %08x]", n[0], n[1]);
                    logs[side] += tail;
                }
                found += side;
            }
        }
        CompareLogs(logs[0], logs[1], "track plan curve", it);
    }
    fprintf(g_out, "  [track plan curve: %d curves]\n", found);
}

namespace rt3 {
void __fastcall Company_YearFinance(uint8_t* c);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
float __fastcall Company_MaxDividend(uint8_t* c);
int32_t __fastcall Company_FreeShares(void* c);
void __fastcall Company_GoBankrupt(uint8_t* c);
void __fastcall CompanyWindow_Refresh(uint8_t* win);
int32_t Screen90();
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
void* __fastcall Company_Owner(void* c);
int32_t __fastcall Game_EffectiveVersion();
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
}

namespace {
// A small finance model kept inside the company record (+0x700..), so the
// function's copy-and-restore simulation works on it.
float& YfCash(uint8_t* c) { return *(float*)(c + 0x700); }
int32_t g_yfVersion, g_yfScreen;
uint8_t g_yfOwner[0x300];
double __fastcall FakeYfStat(uint8_t* c, int, int32_t id, int32_t year) {
    Logf("stat %d %d", id, year);
    switch (id) {
    case 0xd: return YfCash(c);
    case 0x12: return *(float*)(c + 0x704);
    case 9: return *(float*)(c + 0x708);
    case 0x1d: return *(float*)(c + 0x70c);
    case 0x2b: return *(float*)(c + 0x710 + (year & 3) * 4);
    case 0x2c: return *(float*)(c + 0x720 + (year & 3) * 4);
    }
    return 0.0;
}
float __fastcall FakeYfPrice(uint8_t* c, int, int32_t issued, int32_t store) {
    Logf("price %d %d", issued, store);
    return *(float*)(c + 0x730) + issued * 0.0001f;
}
void __fastcall FakeYfRemove(void* cv, int, int32_t idx, int32_t post) {
    uint8_t* c = (uint8_t*)cv;
    Logf("remove %d %d", idx, post);
    int32_t n = *(int32_t*)(c + 0x5b);
    int32_t a = *(int32_t*)(c + 0x5f + idx * 0xc);
    YfCash(c) -= (float)(a < 0 ? -a : a);
    memmove(c + 0x5f + idx * 0xc, c + 0x5f + (idx + 1) * 0xc, (size_t)(n - idx - 1) * 0xc);
    *(int32_t*)(c + 0x5b) = n - 1;
}
int32_t __fastcall FakeYfBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t comm, float rate) {
    Logf("bond %d %d %d %08x", amount, years, comm, B(rate));
    int32_t n = *(int32_t*)(c + 0x5b);
    if (n >= 12 || c[0x740] == n) return 0;
    *(int32_t*)(c + 0x5f + n * 0xc) = amount;
    *(float*)(c + 0x67 + n * 0xc) = 0.04f + (n % 5) * 0.015f;
    *(int32_t*)(c + 0x5b) = n + 1;
    YfCash(c) += (float)amount;
    return 1;
}
void __fastcall RecYfShares(uint8_t* c, int, int32_t n, int32_t at, int32_t price) { Logf("issue shares %d %d %d", n, at, price); }
float __fastcall FakeYfMaxDiv(uint8_t* c) {
    Logf("max dividend");
    return *(float*)(c + 0x734);
}
int32_t __fastcall FakeYfFree(void* c) {
    Logf("free shares");
    return *(int32_t*)((uint8_t*)c + 0x738);
}
void __fastcall RecYfBankrupt(uint8_t* c) { Logf("bankrupt"); }
void __fastcall RecYfRefresh(uint8_t* w) { Logf("refresh %p", w); }
int32_t FakeYfScreen() { return g_yfScreen; }
void __fastcall FakeYfBuy(uint8_t* c, int, int32_t n) {
    Logf("buy back %d", n);
    *(int32_t*)(c + 0x738) -= n;
    YfCash(c) -= n * *(float*)(c + 0x730);
}
void* __fastcall FakeYfOwner(void* c) { return ((uint8_t*)c)[0x741] ? g_yfOwner : nullptr; }
int32_t __fastcall FakeYfVersion() { return g_yfVersion; }
uint8_t g_yfHeap[0x7800];
void* __cdecl FakeYfMalloc(size_t n) {
    Logf("malloc %x", (uint32_t)n);
    return g_yfHeap;
}
void __cdecl FakeYfRelease(void* p) { Logf("free %d", p == g_yfHeap); }
uint32_t __fastcall RecYfLog(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d) {
    Logf("news [%s] %d %d %d", name, b, c, d);
    return 1;
}
} // namespace

DIFFTEST(ai_CompanyYearFinance) {
    SavedGlobals sg({ 0x6cec78, 0x6cfe4c });
    UiTrace t;
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeYfStat);
    t.Add(0x424fd0, (void*)&rt3::Company_SharePrice, (void*)&FakeYfPrice);
    t.Add(0x423d70, (void*)&rt3::Company_RemoveEntry, (void*)&FakeYfRemove);
    t.Add(0x4275c0, (void*)&rt3::Company_IssueBond, (void*)&FakeYfBond);
    t.Add(0x427450, (void*)&rt3::Company_IssueShares, (void*)&RecYfShares);
    t.Add(0x426260, (void*)&rt3::Company_MaxDividend, (void*)&FakeYfMaxDiv);
    t.Add(0x4261b0, (void*)&rt3::Company_FreeShares, (void*)&FakeYfFree);
    t.Add(0x425a90, (void*)&rt3::Company_GoBankrupt, (void*)&RecYfBankrupt);
    t.Add(0x4c2ca0, (void*)&rt3::CompanyWindow_Refresh, (void*)&RecYfRefresh);
    t.Add(0x4c16e0, (void*)&rt3::Screen90, (void*)&FakeYfScreen);
    t.Add(0x4273c0, (void*)&rt3::Company_BuyBackShares, (void*)&FakeYfBuy);
    t.Add(0x426ef0, (void*)&rt3::Company_Owner, (void*)&FakeYfOwner);
    t.Add(0x482e00, (void*)&rt3::Game_EffectiveVersion, (void*)&FakeYfVersion);
    t.Add(0x4554e0, (void*)&rt3::Log_Add, (void*)&RecYfLog);
    t.Add(0x5a125d, nullptr, (void*)&FakeYfMalloc);
    t.Add(0x5a1145, nullptr, (void*)&FakeYfRelease);
    static uint8_t world[0x4d00], start[0x7800], co[2][0x7800];
    hook::Global<uint8_t*>(0x6cec78) = world;
    int news = 0, bankrupts = 0, issues = 0, buys = 0;
    for (int it = 0; it < 6000; ++it) {
        memset(start, 0, sizeof start);
        *(int32_t*)start = 1 + (int32_t)(Rnd() % 4);
        strcpy((char*)start + 4, "Test Railroad");
        *(int32_t*)(start + 0x47) = 20000 + (int32_t)(Rnd() % 200000);
        *(float*)(start + 0x4b) = RndF(0.0f, 3.0f);
        int nb = (int)(Rnd() % 8);
        *(int32_t*)(start + 0x5b) = nb;
        for (int k = 0; k < nb; ++k) {
            *(int32_t*)(start + 0x5f + k * 0xc) = 100000 * (1 + (int32_t)(Rnd() % 10));
            *(float*)(start + 0x67 + k * 0xc) = Rnd() % 6 == 0 ? 0.05f : RndF(0.03f, 0.16f);
        }
        uint16_t year = (uint16_t)(1850 + Rnd() % 100);
        *(uint16_t*)(start + 0x157) = (uint16_t)(year - Rnd() % 20);
        *(int32_t*)(start + 0x163) = year - (int32_t)(Rnd() % 20);
        start[0xd18] = Rnd() % 2;
        *(int32_t*)(start + 0xd2d) = year - (int32_t)(Rnd() % 3);
        start[0xd56] = Rnd() % 2;
        *(float*)(start + 0x700) = Rnd() % 3 == 0 ? RndF(-2000000.0f, 0.0f) : RndF(-400000.0f, 3000000.0f);
        *(float*)(start + 0x704) = RndF(-2000000.0f, 500000.0f);
        *(float*)(start + 0x708) = RndF(-300000.0f, 300000.0f);
        *(float*)(start + 0x70c) = RndF(-2.0f, 10.0f);
        for (int k = 0; k < 4; ++k) {
            *(float*)(start + 0x710 + k * 4) = RndF(-200000.0f, 400000.0f);
            *(float*)(start + 0x720 + k * 4) = RndF(0.0f, 500000.0f);
        }
        *(float*)(start + 0x730) = Rnd() % 3 == 0 ? RndF(5.0f, 30.0f) : RndF(10.0f, 90.0f);
        *(float*)(start + 0x734) = RndF(0.0f, 4.0f);
        *(int32_t*)(start + 0x738) = (int32_t)(Rnd() % 8000);
        start[0x740] = Rnd() % 4 == 0 ? (uint8_t)(Rnd() % 8) : 0xff;
        start[0x741] = Rnd() % 2;
        g_yfOwner[0x291] = (uint8_t)(Rnd() % 4);
        *(uint16_t*)(world + 0xd) = year;
        world[0xf] = Rnd() % 4 == 0 ? (uint8_t)(1 + Rnd() % 11) : 12;
        *(int32_t*)(world + 0x15) = 100000000 + (int32_t)(Rnd() % 1000000);
        *(int32_t*)(start + 0x16b) = 0;
        for (uint32_t o : { 0x4a87u, 0x4a8bu, 0x4a8fu, 0x4a93u }) *(int32_t*)(world + o) = Rnd() % 6 == 0;
        *(int32_t*)(world + 0x4c7c) = (int32_t)(Rnd() % 3);
        g_yfVersion = Rnd() % 4 == 0 ? 0x66 : 0x68;
        g_yfScreen = Rnd() % 2 ? *(int32_t*)start : 0;
        hook::Global<uint8_t*>(0x6cfe4c) = Rnd() % 2 ? world : nullptr;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memcpy(co[side], start, sizeof start);
            logs[side] = t.Run([&] {
                if (side) rt3::Company_YearFinance(co[side]);
                else ((void(__fastcall*)(uint8_t*))0x401c50)(co[side]);
            });
        }
        CompareLogs(logs[0], logs[1], "company year finance", it);
        int d = -1;
        for (int k = 0; k < 0x7800; ++k)
            if (co[0][k] != co[1][k]) {
                d = k;
                break;
            }
        EXPECT_EQ(d, -1, "company year finance record iteration %d: +0x%x", it, d);
        news += logs[0].find("news") != std::string::npos;
        bankrupts += logs[0].find("bankrupt") != std::string::npos;
        issues += logs[0].find("issue shares") != std::string::npos;
        buys += logs[0].find("buy back") != std::string::npos;
    }
    fprintf(g_out, "  [company year finance: %d with news, %d bankrupt, %d share issues, %d buybacks]\n", news, bankrupts, issues, buys);
}

namespace rt3 {
void Ambient_Update();
uint32_t __fastcall Sound_ListenerIdle(uint8_t* s);
int32_t ClockMs();
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
float __cdecl Sky_NightFactor();
int32_t __fastcall Rand(int32_t lo, int32_t hi);
float SceneClock_MinutesSinceDawn();
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol, int32_t mode, int32_t paused, int32_t prio,
                                 float maxDist);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
}

namespace {
uint32_t g_amIdle, g_amSeed, g_amRand, g_amNext;
int32_t g_amClock, g_amClockStep;
float g_amNight, g_amDawn;
uint8_t g_amSamples[8][0x60];
uint32_t AmHash(int32_t x, int32_t y) { return (uint32_t)(x * 73856093) ^ (uint32_t)(y * 19349663) ^ g_amSeed; }
uint32_t __fastcall FakeAmIdle(uint8_t* s) { return g_amIdle; }
int32_t FakeAmClock() { return g_amClock += g_amClockStep; }
int32_t __fastcall FakeAmWater(uint8_t* r, int, int32_t x, int32_t y) { return (AmHash(x, y) >> 3) % 5 == 0; }
uint32_t __fastcall FakeAmTrees(uint8_t* r, int, int32_t x, int32_t y) { return (AmHash(x, y) >> 7) % 3 == 0; }
uint32_t __fastcall FakeAmCount(uint8_t* t, int, int32_t x, int32_t y) { return (AmHash(x, y) >> 11) % 6; }
float __cdecl FakeAmNight() { return g_amNight; }
int32_t __fastcall FakeAmRand(int32_t lo, int32_t hi) {
    g_amRand = g_amRand * 1103515245u + 12345u;
    int32_t r = hi <= lo ? lo : lo + (int32_t)((g_amRand >> 8) % (uint32_t)(hi - lo + 1));
    Logf("rand %d %d -> %d", lo, hi, r);
    return r;
}
float FakeAmDawn() { return g_amDawn; }
double __fastcall FakeAmHeight(uint8_t* r, int, float x, float z) {
    Logf("height %08x %08x", B(x), B(z));
    return x * 0.5 + z * 0.25;
}
uint32_t __fastcall RecAmPlay(uint8_t* snd, int, const char* name, float x, float y, float z, float vol, int32_t mode, int32_t paused, int32_t prio,
                              float maxDist) {
    Logf("play [%s] %08x %08x %08x %08x %d %d %d %08x", name ? name : "", B(x), B(y), B(z), B(vol), mode, paused, prio, B(maxDist));
    return ++g_amNext;
}
int32_t __fastcall FakeAmLookup(uint8_t* s, int, uint32_t key, int32_t kind) {
    Logf("lookup %u %d", key, kind);
    if (!key || key % 5 == 4) return 0;
    return (int32_t)(uintptr_t)g_amSamples[key % 8];
}
void __fastcall RecAmStop(uint8_t* s, int, uint32_t key, int32_t kind) { Logf("stop %u %d", key, kind); }
} // namespace

DIFFTEST(sound_AmbientUpdate) {
    const uint32_t kTables = 0x5ed088, kTablesEnd = 0x5ed344;
    static uint8_t savedTables[kTablesEnd - kTables], startTables[kTablesEnd - kTables], outTables[2][kTablesEnd - kTables];
    memcpy(savedTables, (void*)kTables, sizeof savedTables);
    SavedGlobals sg({ 0x62b248, 0x62b24c, 0x62b254, 0x62b258, 0x62b25c, 0x62b260, 0x62be68, 0x62c120, 0x6d402c });
    UiTrace t;
    t.Add(0x531dc0, (void*)&rt3::Sound_ListenerIdle, (void*)&FakeAmIdle);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeAmClock);
    t.Add(0x534e10, (void*)&rt3::Terrain_KindIs1Or3to5, (void*)&FakeAmWater);
    t.Add(0x534e90, (void*)&rt3::Terrain_RecBit9, (void*)&FakeAmTrees);
    t.Add(0x5340a0, (void*)&rt3::Terrain_CellCount, (void*)&FakeAmCount);
    t.Add(0x482690, (void*)&rt3::Sky_NightFactor, (void*)&FakeAmNight);
    t.Add(0x518d70, (void*)&rt3::Rand, (void*)&FakeAmRand);
    t.Add(0x4825d0, (void*)&rt3::SceneClock_MinutesSinceDawn, (void*)&FakeAmDawn);
    t.Add(0x448bd0, (void*)&rt3::Terrain_HeightAtTile, (void*)&FakeAmHeight);
    t.Add(0x531e50, (void*)&rt3::Sound_Play3D, (void*)&RecAmPlay);
    t.Add(0x531ef0, (void*)&rt3::Sound_LookupSample, (void*)&FakeAmLookup);
    t.Add(0x5317b0, (void*)&rt3::Sound_Stop, (void*)&RecAmStop);
    static uint8_t view[0x80], grid[0x2200], kinds[0x10000], snd[0x10];
    hook::Global<uint8_t*>(0x62be68) = view;
    hook::Global<uint8_t*>(0x62c120) = grid;
    hook::Global<uint8_t*>(0x6d402c) = snd;
    *(uint8_t**)(grid + 0x212d) = kinds;
    size_t total = 0;
    int shots = 0;
    for (int it = 0; it < 600; ++it) {
        int32_t W = 40 + (int32_t)(Rnd() % 80), H = 40 + (int32_t)(Rnd() % 80);
        *(int32_t*)(grid + 0x2155) = W;
        *(int32_t*)(grid + 0x2159) = H;
        *(int32_t*)(grid + 0x214d) = W / 4 + 1;
        for (uint8_t& k : kinds) k = (uint8_t)(Rnd() % 5);
        *(float*)(view + 0x1c) = RndF(-200.0f, W * 92.0f);
        *(float*)(view + 0x20) = RndF(0.0f, 300.0f);
        *(float*)(view + 0x24) = RndF(-200.0f, H * 92.0f);
        *(float*)(view + 0x66) = Rnd() % 6 == 0 ? 0.0f : RndF(0.0f, 4000.0f);
        g_amIdle = Rnd() % 10 == 0;
        g_amSeed = Rnd();
        g_amNight = Rnd() % 3 == 0 ? 0.0f : RndF(0.0f, 1.0f);
        g_amDawn = RndF(0.0f, 1500.0f);
        g_amClockStep = (int32_t)(Rnd() % 400);
        int32_t clock0 = 100000 + (int32_t)(Rnd() % 100000);
        uint32_t rand0 = Rnd();
        hook::Global<int32_t>(0x62b248) = Rnd() % 10 != 0;
        int32_t last = clock0 - (int32_t)(Rnd() % 60);
        int32_t lastShot = clock0 - (int32_t)(Rnd() % 9000);
        float lx = Rnd() % 4 == 0 ? *(float*)(view + 0x1c) : RndF(0.0f, 1000.0f);
        memcpy(startTables, savedTables, sizeof startTables);
        for (int k = 0; k < 16; ++k) {
            uint8_t* e = startTables + k * 0x1b; // interval, counter, volume, range, height, allowed x3, wav
            *(int32_t*)(e + 0) = 50 + (int32_t)(Rnd() % 400);
            *(int32_t*)(e + 4) = k == 0 && Rnd() % 2 ? 85000 + (int32_t)(Rnd() % 100) : (int32_t)(Rnd() % 1000);
            *(float*)(e + 0xc) = RndF(0.5f, 5.0f);
            *(float*)(e + 0x10) = *(float*)(view + 0x66) / 92.0f + RndF(-20.0f, 20.0f);
            e[0x14] = Rnd() % 2;
            e[0x15] = Rnd() % 2;
            e[0x16] = Rnd() % 2;
        }
        for (int k = 0; k < 8; ++k) {
            uint8_t* e = startTables + 0x1b0 + k * 0x20; // handle, gain, target, current, prio, sample, stop, name
            *(float*)(e + 8) = RndF(0.0f, 1.0f);
            *(float*)(e + 0xc) = Rnd() % 3 == 0 ? *(float*)(e + 8) : RndF(0.0f, 1.0f);
            *(uint32_t*)(e + 0) = Rnd() % 3 == 0 ? 0 : 1 + Rnd() % 9;
            *(int32_t*)(e + 0x18) = Rnd() % 2;
        }
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memcpy((void*)kTables, startTables, sizeof startTables);
            memset(g_amSamples, 0, sizeof g_amSamples);
            g_amClock = clock0;
            g_amRand = rand0;
            g_amNext = 100;
            hook::Global<int32_t>(0x62b24c) = last;
            hook::Global<int32_t>(0x62b254) = lastShot;
            hook::Global<float>(0x62b258) = lx;
            hook::Global<float>(0x62b25c) = *(float*)(view + 0x20);
            hook::Global<float>(0x62b260) = *(float*)(view + 0x24);
            logs[side] = t.Run([&] { side ? rt3::Ambient_Update() : ((void(*)())0x40b920)(); });
            memcpy(outTables[side], (void*)kTables, sizeof startTables);
            char tail[64];
            sprintf(tail, " %d %d", hook::Global<int32_t>(0x62b24c), hook::Global<int32_t>(0x62b254));
            logs[side] += tail;
            for (auto& smp : g_amSamples)
                for (int k = 0; k < 0x60; k += 4) logs[side] += " " + std::to_string(*(uint32_t*)(smp + k));
        }
        CompareLogs(logs[0], logs[1], "ambient update", it);
        int d = -1;
        for (int k = 0; k < (int)sizeof startTables; ++k)
            if (outTables[0][k] != outTables[1][k]) {
                d = k;
                break;
            }
        EXPECT_EQ(d, -1, "ambient tables iteration %d: +0x%x", it, d);
        total += logs[0].size();
        shots += logs[0].find("height") != std::string::npos;
    }
    memcpy((void*)kTables, savedTables, sizeof savedTables);
    fprintf(g_out, "  [ambient update: %u log bytes, %d one-shots]\n", (unsigned)total, shots);
}

namespace rt3 {
int32_t __stdcall RuleVar_Get(int32_t var, uint8_t* pl, uint8_t* co, uint8_t* st, int32_t p5, int32_t p6, int32_t p7, int32_t p8);
int32_t CountHumanPlayers();
int32_t __fastcall Table_CountField3f(Collection* t);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
int32_t __fastcall Company_Get7680(void* c);
int32_t __fastcall Cities_CountConnected(Collection* c, int, int32_t company);
int32_t __fastcall Territories_CountLinked(Collection* c, int, int32_t company);
int32_t __fastcall Company_Sum42e300(void* c);
int32_t __fastcall Company_Sum42e320(void* c);
double __fastcall Company_YearPair1da0(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1da8(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1db0(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1db8(uint8_t* c, int, int32_t y);
double __fastcall Company_YearStatPair(uint8_t* c, int, int32_t y, int32_t i);
double __fastcall Company_YearStatPair2(uint8_t* c, int, int32_t y, int32_t i);
int32_t __fastcall Company_YearSum3(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_YearNet1(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_YearNet3(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_TotalNet1(void* c);
int32_t __fastcall Company_TotalNet3(void* c);
int32_t __fastcall Obj_NetBc1(void* o);
int32_t __fastcall Obj_NetBc3(void* o);
int32_t __fastcall Table_CountActiveSites(Collection* t, int, int32_t highlighted);
int32_t __fastcall Table_CountFlag201_40(Collection* t);
double __fastcall Company_TrackLength(void* c);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
int32_t __fastcall Stations_CargoLinked(Collection* sts, int, int32_t a, int32_t b, int32_t aIsRegion, int32_t bIsRegion, int32_t value);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall Game_EffectiveVersion();
int32_t __fastcall Rand(int32_t lo, int32_t hi);
}

namespace {
uint32_t g_rvSeed;
int32_t g_rvVersion;
// A value from the call's arguments: mostly moderate, sometimes huge or NaN.
double RvVal(uint32_t a, uint32_t b) {
    uint32_t h = (a * 2654435761u) ^ (b * 40503u) ^ g_rvSeed;
    h ^= h >> 13;
    h *= 0x5bd1e995u;
    h ^= h >> 15;
    switch (h % 16) {
    case 0: return std::numeric_limits<double>::quiet_NaN();
    case 1: return 3.0e10 * ((h >> 8) % 2 ? 1 : -1);
    case 2: return 0.0;
    case 3: return -0.75;
    }
    return ((int32_t)(h >> 4) % 2000000) * 0.37;
}
int32_t RvInt(uint32_t a, uint32_t b) {
    double v = RvVal(a, b);
    return v == v ? (int32_t)fmod(v, 100000.0) : 7;
}
uint32_t Id(const void* p) { return (uint32_t)(uintptr_t)p & 0xffff; }

int32_t FakeRvHumans() { Logf("humans"); return RvInt(1, 0); }
int32_t __fastcall FakeRvField3f(Collection* t) { Logf("field3f %x", Id(t)); return RvInt(2, 0); }
int32_t __fastcall FakeRvEntryFlag(void* c, int, int32_t i) { Logf("entryflag %x %d", Id(c), i); return RvInt(3, i); }
int32_t __fastcall FakeRv7680(void* c) { Logf("7680 %x", Id(c)); return RvInt(4, 0); }
int32_t __fastcall FakeRvCities(Collection* c, int, int32_t co) { Logf("cities %x %d", Id(c), co); return RvInt(5, co); }
int32_t __fastcall FakeRvTerr(Collection* c, int, int32_t co) { Logf("terr %x %d", Id(c), co); return RvInt(6, co); }
int32_t __fastcall FakeRvSum300(void* c) { Logf("sum300 %x", Id(c)); return RvInt(7, 0); }
int32_t __fastcall FakeRvSum320(void* c) { Logf("sum320 %x", Id(c)); return RvInt(8, 0); }
double __fastcall FakeRvPair0(uint8_t* c, int, int32_t y) { Logf("pair0 %x %d", Id(c), y); return RvVal(9, y); }
double __fastcall FakeRvPair8(uint8_t* c, int, int32_t y) { Logf("pair8 %x %d", Id(c), y); return RvVal(10, y); }
double __fastcall FakeRvPair10(uint8_t* c, int, int32_t y) { Logf("pair10 %x %d", Id(c), y); return RvVal(11, y); }
double __fastcall FakeRvPair18(uint8_t* c, int, int32_t y) { Logf("pair18 %x %d", Id(c), y); return RvVal(12, y); }
double __fastcall FakeRvStatPair(uint8_t* c, int, int32_t y, int32_t i) { Logf("statpair %x %d %d", Id(c), y, i); return RvVal(13, y * 64 + i); }
double __fastcall FakeRvStatPair2(uint8_t* c, int, int32_t y, int32_t i) { Logf("statpair2 %x %d %d", Id(c), y, i); return RvVal(14, y * 64 + i); }
int32_t __fastcall FakeRvSum3(uint8_t* c, int, int32_t y) { Logf("sum3 %x %d", Id(c), y); return RvInt(15, y); }
int32_t __fastcall FakeRvNet1(uint8_t* c, int, int32_t y) { Logf("net1 %x %d", Id(c), y); return RvInt(16, y); }
int32_t __fastcall FakeRvNet3(uint8_t* c, int, int32_t y) { Logf("net3 %x %d", Id(c), y); return RvInt(17, y); }
int32_t __fastcall FakeRvTotal1(void* c) { Logf("total1 %x", Id(c)); return RvInt(18, 0); }
int32_t __fastcall FakeRvTotal3(void* c) { Logf("total3 %x", Id(c)); return RvInt(19, 0); }
int32_t __fastcall FakeRvBc1(void* c) { Logf("bc1 %x", Id(c)); return RvInt(20, 0); }
int32_t __fastcall FakeRvBc3(void* c) { Logf("bc3 %x", Id(c)); return RvInt(21, 0); }
int32_t __fastcall FakeRvSites(Collection* t, int, int32_t h) { Logf("sites %x %d", Id(t), h); return RvInt(22, h); }
int32_t __fastcall FakeRvFlag40(Collection* t) { Logf("flag40 %x", Id(t)); return RvInt(23, 0); }
double __fastcall FakeRvTrack(void* c) { Logf("track %x", Id(c)); return RvVal(24, 0); }
double __fastcall FakeRvStat(uint8_t* c, int, int32_t id, int32_t year) { Logf("stat %x %d %d", Id(c), id, year); return RvVal(25 + id, year); }
double __fastcall FakeRvWealth(uint8_t* p, int, int32_t kind) { Logf("wealth %x %d", Id(p), kind); return RvVal(90, kind); }
int32_t __fastcall FakeRvLinked(Collection* s, int, int32_t a, int32_t b, int32_t ar, int32_t br, int32_t v) {
    Logf("linked %x %d %d %d %d %d", Id(s), a, b, ar, br, v);
    return RvInt(91, a ^ b ^ v) % 3;
}
int32_t __fastcall FakeRvValid(Collection* c, int, uint32_t id) { Logf("valid %x %u", Id(c), id); return id % 3 != 0; }
int32_t __fastcall FakeRvVersion() { return g_rvVersion; }
int32_t __fastcall FakeRvRand(int32_t lo, int32_t hi) { Logf("rand %d %d", lo, hi); return lo + hi / 3; }
} // namespace

DIFFTEST(rules_RuleVarGet) {
    SavedGlobals sg({ 0x6cec78, 0x6cfca8, 0x6cec7c, 0x62c120, 0x6cd8d8, 0x62be10, 0x6ceb9c, 0x6cec20, 0x62bae0, 0x6cfc9c });
    UiTrace t;
    t.Add(0x4349a0, (void*)&rt3::CountHumanPlayers, (void*)&FakeRvHumans);
    t.Add(0x429a50, (void*)&rt3::Table_CountField3f, (void*)&FakeRvField3f);
    t.Add(0x424010, (void*)&rt3::Company_EntryFlag, (void*)&FakeRvEntryFlag);
    t.Add(0x4240a0, (void*)&rt3::Company_Get7680, (void*)&FakeRv7680);
    t.Add(0x422850, (void*)&rt3::Cities_CountConnected, (void*)&FakeRvCities);
    t.Add(0x488110, (void*)&rt3::Territories_CountLinked, (void*)&FakeRvTerr);
    t.Add(0x42e300, (void*)&rt3::Company_Sum42e300, (void*)&FakeRvSum300);
    t.Add(0x42e320, (void*)&rt3::Company_Sum42e320, (void*)&FakeRvSum320);
    t.Add(0x42e350, (void*)&rt3::Company_YearPair1da0, (void*)&FakeRvPair0);
    t.Add(0x42e370, (void*)&rt3::Company_YearPair1da8, (void*)&FakeRvPair8);
    t.Add(0x42e3f0, (void*)&rt3::Company_YearPair1db0, (void*)&FakeRvPair10);
    t.Add(0x42e410, (void*)&rt3::Company_YearPair1db8, (void*)&FakeRvPair18);
    t.Add(0x42e390, (void*)&rt3::Company_YearStatPair, (void*)&FakeRvStatPair);
    t.Add(0x42e3c0, (void*)&rt3::Company_YearStatPair2, (void*)&FakeRvStatPair2);
    t.Add(0x42e430, (void*)&rt3::Company_YearSum3, (void*)&FakeRvSum3);
    t.Add(0x42e460, (void*)&rt3::Company_YearNet1, (void*)&FakeRvNet1);
    t.Add(0x42e490, (void*)&rt3::Company_YearNet3, (void*)&FakeRvNet3);
    t.Add(0x42e4d0, (void*)&rt3::Company_TotalNet1, (void*)&FakeRvTotal1);
    t.Add(0x42e4f0, (void*)&rt3::Company_TotalNet3, (void*)&FakeRvTotal3);
    t.Add(0x42e520, (void*)&rt3::Obj_NetBc1, (void*)&FakeRvBc1);
    t.Add(0x42e540, (void*)&rt3::Obj_NetBc3, (void*)&FakeRvBc3);
    t.Add(0x4818b0, (void*)&rt3::Table_CountActiveSites, (void*)&FakeRvSites);
    t.Add(0x493770, (void*)&rt3::Table_CountFlag201_40, (void*)&FakeRvFlag40);
    t.Add(0x42a250, (void*)&rt3::Company_TrackLength, (void*)&FakeRvTrack);
    t.Add(0x42a5d0, (void*)&rt3::Company_Stat, (void*)&FakeRvStat);
    t.Add(0x4767a0, (void*)&rt3::Player_Wealth, (void*)&FakeRvWealth);
    t.Add(0x4815d0, (void*)&rt3::Stations_CargoLinked, (void*)&FakeRvLinked);
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeRvValid);
    t.Add(0x482e00, (void*)&rt3::Game_EffectiveVersion, (void*)&FakeRvVersion);
    t.Add(0x518d70, (void*)&rt3::Rand, (void*)&FakeRvRand);
    static uint8_t world[0x4d00], fin[0x100], ng[0x100], map[0x2200], mp[4], pl[0x1000], co[0x8000], st[0x400];
    static uint8_t colls[5][0x10];
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cfca8) = fin;
    hook::Global<uint8_t*>(0x6cec7c) = ng;
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x62be10) = colls[0];
    hook::Global<uint8_t*>(0x6ceb9c) = colls[1];
    hook::Global<uint8_t*>(0x6cec20) = colls[2];
    hook::Global<uint8_t*>(0x62bae0) = colls[3];
    hook::Global<uint8_t*>(0x6cfc9c) = colls[4];
    // Float fields: moderate values with some NaN and out-of-range ones.
    auto fill = [](uint8_t* p, size_t n) {
        for (size_t k = 0; k + 4 <= n; k += 4) {
            uint32_t r = Rnd();
            float f = r % 23 == 0 ? std::numeric_limits<float>::quiet_NaN() : r % 29 == 0 ? 5.0e9f : RndF(-90000.0f, 90000.0f);
            if (r % 7 == 0) *(int32_t*)(p + k) = (int32_t)(Rnd() % 200000) - 1000;
            else *(float*)(p + k) = f;
        }
    };
    int nonzero = 0;
    for (int it = 0; it < 120; ++it) {
        fill(world, sizeof world);
        fill(fin, sizeof fin);
        fill(ng, sizeof ng);
        fill(map, sizeof map);
        fill(pl, sizeof pl);
        fill(co, sizeof co);
        fill(st, sizeof st);
        *(uint16_t*)(world + 5) = (uint16_t)(1830 + Rnd() % 50);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1830 + Rnd() % 100);
        world[0xf] = (uint8_t)(Rnd() % 12);
        *(int32_t*)pl = Rnd() % 5 == 0 ? -(int32_t)(Rnd() % 4) - 1 : (int32_t)(Rnd() % 8);
        if (Rnd() % 4 == 0) *(uint32_t*)pl = 0x80000000u + Rnd() % 1000;
        uint32_t plIdx = *(uint32_t*)pl;
        bool plSafe = plIdx < 8;
        *(int32_t*)co = (int32_t)(Rnd() % 16);
        if (Rnd() % 4 == 0) *(int32_t*)(st + 0x1dd) = *(int32_t*)co;
        for (int var = -3; var <= 0x36c; ++var) {
            g_rvSeed = Rnd();
            g_rvVersion = Rnd() % 2 ? 0x66 : 0x69;
            hook::Global<uint8_t*>(0x6cd8d8) = Rnd() % 2 ? mp : nullptr;
            uint8_t* a2 = Rnd() % 6 == 0 ? nullptr : pl;
            uint8_t* a3 = Rnd() % 6 == 0 ? nullptr : co;
            uint8_t* a4 = Rnd() % 6 == 0 ? nullptr : st;
            // Company-year tables index by the player's id; keep that in the buffer.
            if (!plSafe && a3) a2 = nullptr;
            bool deref = (var >= 0x42 && var <= 0x4a) && a3 && !a2;
            if (deref) a3 = nullptr; // the original reads the player without checking it
            int32_t p5 = (int32_t)(Rnd() % 20) - 2, p6 = (int32_t)Rnd(), p7 = (int32_t)(Rnd() % 30), p8 = (int32_t)(Rnd() % 30);
            int32_t r[2];
            std::string logs[2];
            for (int side = 0; side < 2; ++side) {
                logs[side] = t.Run([&] {
                    r[side] = side ? rt3::RuleVar_Get(var, a2, a3, a4, p5, p6, p7, p8)
                                   : ((int32_t(__stdcall*)(int32_t, uint8_t*, uint8_t*, uint8_t*, int32_t, int32_t, int32_t, int32_t))0x42e580)(
                                         var, a2, a3, a4, p5, p6, p7, p8);
                });
                logs[side] += " = " + std::to_string(r[side]);
            }
            CompareLogs(logs[0], logs[1], "rule var", it * 0x1000 + var);
            nonzero += r[0] != 0;
        }
    }
    fprintf(g_out, "  [rule var: %d non-zero]\n", nonzero);
}

namespace rt3 {
void __fastcall Industry_Produce(uint8_t* ind, int, int32_t bulk);
uint32_t __fastcall Obj_GridByte(void* o);
void* __fastcall Obj_Ref3cc(void* o);
void* __fastcall Obj_OwnerCompany(void* o);
uint8_t* __fastcall Obj_RegionAt(void* o);
double __fastcall Recipe_NetValue(void* o, int, uint8_t* rec);
void __fastcall Building_SpawnLot(uint8_t* b, int, int32_t kind, float factor);
int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag);
int32_t __fastcall Obj_CountAvailableVehicles(void* o, int, int32_t flag);
void __fastcall Cargo_MoveTo62(void* c, int, float a);
const char* __fastcall Table_NearestName(Collection* t, int, float x, float y);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void __fastcall Obj_AddLevel(void* o, int, int32_t i, float f);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
double __fastcall City_ProduceCargo(uint8_t* o, int, int32_t i, float amt);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall Collection_Count(Collection* c);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);
const char* __fastcall GetString(int32_t id);
const char* __fastcall LocalizeName(const char* name);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall Game_EffectiveVersion();
}

namespace {
uint8_t g_ipRec[0x800], g_ipEntries[8 * 0xbc], g_ipRegion[0x400], g_ipRef[0x200], g_ipCo[0x1000], g_ipCity[0x400], g_ipCompanies[0x40],
    g_ipCargo[0x200], g_ipCities[0x40], g_ipCargoRecs[8][0x40], g_ipPlayerRec[0x40];
uint32_t g_ipSeed, g_ipRand;
int32_t g_ipVehicles, g_ipCount, g_ipVersion, g_ip70;
uint32_t IpHash(uint32_t a, uint32_t b) {
    uint32_t h = (a * 2654435761u) ^ (b * 0x9e3779b9u) ^ g_ipSeed;
    h ^= h >> 15;
    h *= 0x85ebca6bu;
    h ^= h >> 13;
    return h;
}
float IpF(uint32_t a, uint32_t b, float lo, float hi) { return lo + (hi - lo) * (float)(IpHash(a, b) % 10007) / 10007.0f; }
uint8_t* __fastcall FakeIpVRec(uint8_t* ind) { return g_ipRec; }
int32_t __fastcall FakeIpV70(uint8_t* ind) { return g_ip70; }
void __fastcall RecIpV74(uint8_t* ind) { Logf("vt74"); }
uint32_t __fastcall FakeIpGrid(void* o) { return 3; }
void* __fastcall FakeIpRef(void* o) { return g_ipRef; }
void* __fastcall FakeIpOwner(void* o) { Logf("owner"); return g_ipCo; }
uint8_t* __fastcall FakeIpRegion(void* o) { return g_ipRegion; }
double __fastcall FakeIpNet(void* o, int, uint8_t* e) {
    Logf("net %d", (int)(e - g_ipEntries));
    return IpF(1, (uint32_t)(e - g_ipEntries), -60000.0f, 120000.0f);
}
void __fastcall RecIpSpawn(uint8_t* b, int, int32_t kind, float f) { Logf("spawn %d %08x", kind, B(f)); }
int32_t __fastcall FakeIpAvail(void*, int, uint8_t* e, int32_t flag) {
    Logf("avail %d %d", (int)(e - g_ipEntries), flag);
    return e[0xb8];
}
int32_t __fastcall FakeIpCountAvail(void* o, int, int32_t flag) {
    Logf("countavail %d", flag);
    return g_ipVehicles;
}
void __fastcall RecIpMove(void* c, int, float a) { Logf("move %d %08x", (int)(((uint8_t*)c - g_ipCargoRecs[0]) / 0x40), B(a)); }
const char* __fastcall FakeIpNearest(Collection* t, int, float x, float y) {
    Logf("nearest %08x %08x", B(x), B(y));
    return "Town";
}
void __fastcall RecIpPost(void* c, int, int32_t stat, float amt, int32_t cash) { Logf("post %d %d %08x %d", c == g_ipCo, stat, B(amt), cash); }
void __fastcall RecIpLevel(void* o, int, int32_t i, float f) { Logf("level %d %08x", i, B(f)); }
int32_t __fastcall FakeIpQuota(uint8_t* o, int, int32_t i) {
    Logf("quota %d", i);
    return (int32_t)(IpHash(2, i) % 12) - 2;
}
double __fastcall FakeIpProduce(uint8_t* o, int, int32_t i, float amt) {
    Logf("produce %d %08x", i, B(amt));
    return amt * IpF(3, i, 0.0f, 900.0f);
}
float __fastcall FakeIpEffects(uint8_t* w, int, int32_t idx, uint32_t co, uint32_t pl, uint32_t rg) {
    Logf("effects %d %u %u %u", idx, co, pl, rg);
    return IpF(4, idx, 0.4f, 2.2f);
}
double __fastcall FakeIpCellX(void* o) { return 12.5; }
double __fastcall FakeIpCellY(void* o) { return 40.25; }
int32_t __fastcall FakeIpCount(Collection* c) { return g_ipCount; }
void* __fastcall FakeIpGet(Collection* c, int, uint32_t id) {
    Logf("get %x %u", (uint32_t)(uintptr_t)c & 0xfff, id);
    if ((uint8_t*)c == g_ipCompanies) return g_ipPlayerRec;
    if ((uint8_t*)c == g_ipCities) return g_ipCity;
    return g_ipCargoRecs[id % 8];
}
int32_t __fastcall FakeIpRand(int32_t lo, int32_t hi) {
    g_ipRand = g_ipRand * 1103515245u + 12345u;
    int32_t r = lo + (int32_t)((g_ipRand >> 8) % (uint32_t)(hi - lo + 1));
    Logf("rand %d %d -> %d", lo, hi, r);
    return r;
}
char* __fastcall FakeIpFormat(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                              const char* a6, const char* a7, const char* a8) {
    Logf("format [%s] [%s] [%s] %p %p %p %p %p %p", fmt, a1, a2, a3, a4, a5, a6, a7, a8);
    strcpy(dst, "expanding");
    return dst;
}
const char* __fastcall FakeIpString(int32_t id) {
    Logf("string %d", id);
    return "fmt";
}
const char* __fastcall FakeIpLocalize(const char* n) {
    Logf("localize [%s]", n);
    return n;
}
uint32_t __fastcall RecIpLog(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d) {
    Logf("news [%s] %d %d %d", name, b, c, d);
    return 1;
}
int32_t __fastcall FakeIpVersion() { return g_ipVersion; }
float IpRndF(float lo, float hi) {
    uint32_t r = Rnd();
    if (r % 97 == 0) return std::numeric_limits<float>::quiet_NaN();
    if (r % 13 == 0) return 0.0f;
    return RndF(lo, hi);
}
} // namespace

DIFFTEST(sim_IndustryProduce) {
    SavedGlobals sg({ 0x6cec78, 0x6cec7c, 0x6cfca8, 0x6cd8d8, 0x62be10, 0x62ba8c, 0x62bae0, 0x6acd34 });
    UiTrace t;
    t.Add(0x40cb70, (void*)&rt3::Obj_GridByte, (void*)&FakeIpGrid);
    t.Add(0x40cec0, (void*)&rt3::Obj_Ref3cc, (void*)&FakeIpRef);
    t.Add(0x40d210, (void*)&rt3::Obj_OwnerCompany, (void*)&FakeIpOwner);
    t.Add(0x40fa50, (void*)&rt3::Obj_RegionAt, (void*)&FakeIpRegion);
    t.Add(0x40faa0, (void*)&rt3::Recipe_NetValue, (void*)&FakeIpNet);
    t.Add(0x40fef0, (void*)&rt3::Building_SpawnLot, (void*)&RecIpSpawn);
    t.Add(0x412560, (void*)&rt3::Vehicle_Available, (void*)&FakeIpAvail);
    t.Add(0x4125f0, (void*)&rt3::Obj_CountAvailableVehicles, (void*)&FakeIpCountAvail);
    t.Add(0x41e1d0, (void*)&rt3::Cargo_MoveTo62, (void*)&RecIpMove);
    t.Add(0x4220b0, (void*)&rt3::Table_NearestName, (void*)&FakeIpNearest);
    t.Add(0x42a080, (void*)&rt3::Company_PostStat, (void*)&RecIpPost);
    t.Add(0x42b140, (void*)&rt3::Obj_AddLevel, (void*)&RecIpLevel);
    t.Add(0x42c960, (void*)&rt3::City_CargoQuota, (void*)&FakeIpQuota);
    t.Add(0x42cab0, (void*)&rt3::City_ProduceCargo, (void*)&FakeIpProduce);
    t.Add(0x436590, (void*)&rt3::Effects_Multiplier, (void*)&FakeIpEffects);
    t.Add(0x455800, (void*)&rt3::Obj_CellX, (void*)&FakeIpCellX);
    t.Add(0x455810, (void*)&rt3::Obj_CellY, (void*)&FakeIpCellY);
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeIpCount);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeIpGet);
    t.Add(0x518be0, (void*)&rt3::SimRand, (void*)&FakeIpRand);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeIpFormat);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeIpString);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeIpLocalize);
    t.Add(0x4554e0, (void*)&rt3::Log_Add, (void*)&RecIpLog);
    t.Add(0x482e00, (void*)&rt3::Game_EffectiveVersion, (void*)&FakeIpVersion);
    static uint8_t* world = new uint8_t[0x46d00];
    static uint8_t ng[0x100], mp[4], logs6acd34[0x40], fin[0x40];
    static uint8_t ind[2][0x400], startInd[0x400], region[2][0x400], startRegion[0x400], co[2][0x1000], city[2][0x400];
    static void* vt[0x40];
    vt[0x80 / 4] = (void*)&FakeIpVRec;
    vt[0x70 / 4] = (void*)&FakeIpV70;
    vt[0x74 / 4] = (void*)&RecIpV74;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = ng;
    hook::Global<uint8_t*>(0x6cfca8) = fin;
    hook::Global<uint8_t*>(0x62be10) = g_ipCompanies;
    hook::Global<uint8_t*>(0x62ba8c) = g_ipCargo;
    hook::Global<uint8_t*>(0x62bae0) = g_ipCities;
    hook::Global<uint8_t*>(0x6acd34) = logs6acd34;
    static const char* names[] = { "Nuclear Power Plant", "Electric Plant", "Recycling Plant", "Steel Mill", "Farm" };
    int news = 0, spawns = 0, posts = 0;
    for (int it = 0; it < 4000; ++it) {
        g_ipSeed = Rnd();
        g_ipVehicles = (int32_t)(Rnd() % 4);
        g_ipCount = (int32_t)(Rnd() % 9000);
        g_ipVersion = Rnd() % 3 == 0 ? 0x66 : Rnd() % 2 ? 0x68 : 0x69;
        g_ip70 = Rnd() % 4 == 0;
        hook::Global<uint8_t*>(0x6cd8d8) = Rnd() % 3 == 0 ? mp : nullptr;
        memset(world, 0, 0x46d00);
        bool sandbox = Rnd() % 6 == 0;
        *(int32_t*)(world + 0x4af7) = sandbox;
        *(int32_t*)(world + 0x46c38) = Rnd() % 8 != 0;
        *(int32_t*)(world + 0x4aef) = Rnd() % 20 == 0;
        world[0x4c8d] = Rnd() % 4 == 0;
        *(uint16_t*)(world + 0xd) = (uint16_t)(1830 + Rnd() % 180);
        *(int32_t*)(world + 0x2d) = (int32_t)(Rnd() % 5);
        *(float*)(world + 0x4caa) = RndF(0.0f, 1000.0f);
        ng[0x82] = Rnd() % 30 == 0;
        // Record and recipes.
        memset(g_ipRec, 0, sizeof g_ipRec);
        strcpy((char*)g_ipRec + 4, names[Rnd() % 5]);
        strcpy((char*)g_ipRec + 0x4f, "Mill");
        *(uint32_t*)(g_ipRec + 0x22) = Rnd() % 8 == 0 ? 0x80000000u + Rnd() % 1000 : Rnd() % 400000;
        *(float*)(g_ipRec + 0x2a) = IpRndF(0.0f, 30.0f);
        g_ipRec[0x32] = Rnd() % 2 ? 2 : 1;
        int n = (int)(Rnd() % 7);
        *(int32_t*)(g_ipRec + 0x33) = n;
        *(uint8_t**)(g_ipRec + 0x37) = g_ipEntries;
        *(int32_t*)(g_ipRec + 0x43) = (int32_t)(Rnd() % 50000);
        *(float*)(g_ipRec + 0x47) = RndF(0.0f, 4.0f);
        *(int32_t*)(g_ipRec + 0x4b) = Rnd() % 4 != 0;
        *(int32_t*)(g_ipRec + 0x78c) = Rnd() % 2;
        *(int32_t*)(g_ipRec + 0x790) = Rnd() % 2;
        memset(g_ipEntries, 0, sizeof g_ipEntries);
        for (int i = 0; i < n; ++i) {
            uint8_t* e = g_ipEntries + i * 0xbc;
            *(int32_t*)e = Rnd() % 3 == 0;
            *(float*)(e + 4) = IpRndF(0.0f, 3.0f);
            *(int32_t*)(e + 0x1c) = (int32_t)(Rnd() % 0x50);
            int ni = (int)(Rnd() % 4);
            *(int32_t*)(e + 0x2c) = ni;
            for (int j = 0; j < ni; ++j) {
                *(int32_t*)(e + 0x44 + j * 0x1c) = (int32_t)(Rnd() % 0x50);
                *(float*)(e + 0x48 + j * 0x1c) = IpRndF(0.0f, 2.0f);
            }
            e[0xb8] = Rnd() % 5 != 0;
        }
        for (int k = 0; k < 8; ++k) *(float*)(g_ipCargoRecs[k] + 0x18) = Rnd() % 9 == 0 ? 0.0f : RndF(0.0f, 900.0f);
        for (int k = 0; k < 3; ++k) *(int32_t*)(g_ipCargo + 0x8e + k * 4) = (int32_t)(Rnd() % 0x60) - 8;
        *(int32_t*)(g_ipRef + 0x141) = (int32_t)(Rnd() % 40);
        *(uint32_t*)(g_ipPlayerRec + 0x3b) = Rnd() % 7;
        // Industry and region.
        memset(startInd, 0, sizeof startInd);
        *(void***)startInd = vt;
        startInd[0x3d4] = Rnd() % 12 == 0 ? 0 : 1;
        *(int32_t*)(startInd + 0x246) = Rnd() % 4 == 0;
        *(uint16_t*)(startInd + 0x26e) = Rnd() % 3 == 0 ? 0 : (uint16_t)(1 + Rnd() % 50);
        *(uint32_t*)(startInd + 0x276) = Rnd() % 2 ? 0 : 1 + Rnd() % 6;
        for (int k = 0; k < 3; ++k) *(float*)(startInd + 0x28a + k * 4) = Rnd() % 3 == 0 ? 0.0f : RndF(-1.0f, 8.0f);
        *(float*)(startInd + 0x296) = RndF(-8.0f, 40.0f);
        *(float*)(startInd + 0x29a) = RndF(-8.0f, 40.0f);
        *(int32_t*)(startInd + 0x2a4) = (int32_t)(Rnd() % 100);
        for (uint32_t o : { 0x2c0u, 0x2e8u, 0x310u, 0x338u, 0x360u }) *(float*)(startInd + o) = RndF(0.0f, 50000.0f);
        startInd[0x391] = Rnd() % 3 == 0 ? 0 : (uint8_t)(Rnd() % 40);
        for (int k = 0; k < 0x400; k += 4) *(float*)(startRegion + k) = Rnd() % 5 == 0 ? 0.0f : RndF(0.0f, 300.0f);
        int32_t bulk = Rnd() % 5 == 0;
        if (it % 3 == 0) {
            // Aim at the expansion news: an unowned, growing industry with plenty of inputs.
            bulk = 0;
            *(uint32_t*)(startInd + 0x276) = 0;
            *(int32_t*)(startInd + 0x246) = 0;
            *(int32_t*)(g_ipRec + 0x4b) = 1;
            *(float*)(startInd + 0x296) = RndF(20.0f, 36.0f);
            *(float*)(startInd + 0x29a) = RndF(20.0f, 36.0f);
            for (int k = 0; k < 0x400; k += 4) *(float*)(startRegion + k) = RndF(2000.0f, 90000.0f);
        }
        uint32_t rand0 = Rnd();
        std::string lg[2];
        for (int side = 0; side < 2; ++side) {
            memcpy(ind[side], startInd, sizeof startInd);
            memcpy(region[side], startRegion, sizeof startRegion);
            memcpy(g_ipRegion, startRegion, sizeof startRegion);
            memset(g_ipCo, 0, sizeof g_ipCo);
            memset(g_ipCity, 0, sizeof g_ipCity);
            *(float*)(world + 0x4caa) = 100.0f;
            g_ipRand = rand0;
            lg[side] = t.Run([&] {
                if (side) rt3::Industry_Produce(ind[side], 0, bulk);
                else ((void(__fastcall*)(uint8_t*, int, int32_t))0x4101e0)(ind[side], 0, bulk);
            });
            memcpy(region[side], g_ipRegion, sizeof startRegion);
            memcpy(co[side], g_ipCo, sizeof g_ipCo);
            memcpy(city[side], g_ipCity, sizeof g_ipCity);
            char tail[32];
            sprintf(tail, " w%08x", *(uint32_t*)(world + 0x4caa));
            lg[side] += tail;
        }
        CompareLogs(lg[0], lg[1], "industry produce", it);
        auto firstDiff = [](const uint8_t* a, const uint8_t* b, size_t n) {
            for (size_t k = 0; k < n; ++k)
                if (a[k] != b[k]) return (int)k;
            return -1;
        };
        EXPECT_EQ(firstDiff(ind[0], ind[1], 0x400), -1, "industry iteration %d", it);
        EXPECT_EQ(firstDiff(region[0], region[1], 0x400), -1, "industry region iteration %d", it);
        EXPECT_EQ(firstDiff(co[0], co[1], 0x1000), -1, "industry company iteration %d", it);
        EXPECT_EQ(firstDiff(city[0], city[1], 0x400), -1, "industry city iteration %d", it);
        news += lg[0].find("news") != std::string::npos;
        spawns += lg[0].find("spawn") != std::string::npos;
        posts += lg[0].find("post") != std::string::npos;
    }
    fprintf(g_out, "  [industry produce: %d news, %d with spawns, %d posts]\n", news, spawns, posts);
}

namespace rt3 {
void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
int32_t __fastcall Engine_FreeTextureMem(void* e);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_ApplyLighting(uint8_t* r);
void __fastcall Scene_MarkDirty(uint8_t* r);
int32_t __fastcall Sound_GetSpeakers(uint8_t* s);
uint32_t __fastcall Sound_ActiveProvider(uint8_t* s);
}

namespace {
int32_t g_muTexMem, g_muSpeakers;
void* g_muRenderer;
char g_muProvider[32];
BOOL WINAPI FakeMuCheck(HMENU m, UINT id, UINT flag) {
    Logf("check %p %u %u", m, id, flag);
    return TRUE;
}
BOOL WINAPI FakeMuEnable(HMENU m, UINT id, UINT flag) {
    Logf("enable %p %u %u", m, id, flag);
    return TRUE;
}
int WINAPI FakeMuGetStr(HMENU m, UINT id, LPSTR buf, int n, UINT f) {
    Logf("getstr %u %d %u", id, n, f);
    sprintf(buf, "Prov%u", id % 4);
    return (int)strlen(buf);
}
int32_t __fastcall FakeMuTexMem(void* e) { return g_muTexMem; }
void* __fastcall FakeMuRenderer(void* e) { return g_muRenderer; }
void __fastcall RecMuLight(void* r) { Logf("light %p", r); }
void __fastcall RecMuDirty(void* r) { Logf("dirty %p", r); }
int32_t __fastcall FakeMuSpeakers(uint8_t* s) { return g_muSpeakers; }
uint32_t __fastcall FakeMuProvider(uint8_t* s) { return (uint32_t)(uintptr_t)g_muProvider; }
} // namespace

DIFFTEST(ui_MenuUpdateChecks) {
    SavedGlobals sg({ 0x6d4024, 0x6d401c, 0x6cec74, 0x6d402c, 0x6d3de8, 0x6d3dec, 0x6d3df0, 0x6d3df4, 0x6d3e30, 0x6d3e34, 0x6d3e38, 0x6d3e3c,
                      0x6d3f20, 0x6d3f24, 0x6d3f28, 0x6d3f2c, 0x6d3ff8, 0x62be84 });
    UiTrace t;
    t.Add(0x51fa00, (void*)&rt3::Engine_FreeTextureMem, (void*)&FakeMuTexMem);
    t.Add(0x51f070, (void*)&rt3::Engine_Renderer, (void*)&FakeMuRenderer);
    t.Add(0x523d90, (void*)&rt3::Scene_ApplyLighting, (void*)&RecMuLight);
    t.Add(0x5270d0, (void*)&rt3::Scene_MarkDirty, (void*)&RecMuDirty);
    t.Add(0x531770, (void*)&rt3::Sound_GetSpeakers, (void*)&FakeMuSpeakers);
    t.Add(0x531620, (void*)&rt3::Sound_ActiveProvider, (void*)&FakeMuProvider);
    void** iat = (void**)0x5c8230;
    DWORD prot;
    VirtualProtect(iat, 12, PAGE_READWRITE, &prot);
    void* savedIat[3] = { iat[0], iat[1], iat[2] };
    iat[0] = (void*)&FakeMuEnable;
    iat[1] = (void*)&FakeMuCheck;
    iat[2] = (void*)&FakeMuGetStr;
    static uint8_t* eng = new uint8_t[0x114800];
    static uint8_t ui[0xd00], opt[0x300], snd[0x40];
    auto bytes = [](uint8_t* p, size_t n) {
        for (size_t k = 0; k < n; ++k) {
            uint32_t r = Rnd();
            p[k] = r % 3 == 0 ? (uint8_t)(r >> 8) : r % 3 == 1 ? 0 : (uint8_t)(r % 4);
        }
    };
    size_t total = 0;
    for (int it = 0; it < 400; ++it) {
        bytes(eng + 0x114200, 0x600);
        bytes(ui + 0xc70, 0x10);
        bytes(opt, sizeof opt);
        for (uint32_t off : { 0x1146b3u, 0x114726u, 0x11420au, 0x114228u, 0x1146d9u, 0x1146ddu, 0x1146c6u, 0x1146cau, 0x1146ceu, 0x1146c2u,
                              0x1146beu })
            *(int32_t*)(eng + off) = (int32_t)(Rnd() % 8) - 1;
        if (Rnd() % 2) *(int32_t*)(eng + 0x114726) = Rnd() % 3 == 0 ? 0x46 : Rnd() % 2 ? 0x78 : 0xf0;
        *(float*)(eng + 0x1146af) = Rnd() % 3 == 0 ? 0.5f : RndF(0.0f, 1.0f);
        for (uint32_t off : { 0x1146fau, 0x1146feu, 0x114702u })
            *(float*)(eng + off) = Rnd() % 9 == 0 ? std::numeric_limits<float>::quiet_NaN() : RndF(-0.1f, 1.05f);
        for (uint32_t off : { 0x10cu, 0x178u, 0x253u, 0x22fu, 0x2bbu, 0x2bfu, 0x1ebu, 0xa4u, 0x128u, 0x1e3u })
            *(int32_t*)(opt + off) = (int32_t)(Rnd() % 7) - 1;
        for (uint32_t g : { 0x6d3de8u, 0x6d3decu, 0x6d3df0u, 0x6d3df4u, 0x6d3e30u, 0x6d3e34u, 0x6d3e38u, 0x6d3e3cu, 0x6d3f20u, 0x6d3f24u,
                            0x6d3f28u, 0x6d3f2cu })
            hook::Global<int32_t>(g) = (int32_t)(Rnd() % 16) - 3;
        hook::Global<int32_t>(0x6d3ff8) = Rnd() % 2;
        hook::Global<int32_t>(0x62be84) = Rnd() % 2;
        hook::Global<uint8_t*>(0x6d4024) = Rnd() % 20 == 0 ? nullptr : eng;
        hook::Global<uint8_t*>(0x6d401c) = Rnd() % 20 == 0 ? nullptr : ui;
        hook::Global<uint8_t*>(0x6cec74) = opt;
        hook::Global<uint8_t*>(0x6d402c) = snd;
        g_muTexMem = Rnd() % 2 ? 0x400000 : 0x1000000;
        g_muSpeakers = (int32_t)(Rnd() % 8) - 1;
        g_muRenderer = Rnd() % 4 == 0 ? nullptr : (void*)0x1234;
        sprintf(g_muProvider, "xxProv%u", Rnd() % 5);
        HMENU menu = Rnd() % 10 == 0 ? nullptr : (HMENU)(uintptr_t)0x5678;
        char relight = (char)(Rnd() % 2);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            HMENU holder = menu;
            logs[side] = t.Run([&] {
                if (side) rt3::Menu_UpdateChecks(&holder, 0, relight);
                else ((void(__fastcall*)(HMENU*, int, char))0x4625b0)(&holder, 0, relight);
            });
        }
        CompareLogs(logs[0], logs[1], "menu checks", it);
        total += logs[0].size();
    }
    iat[0] = savedIat[0];
    iat[1] = savedIat[1];
    iat[2] = savedIat[2];
    VirtualProtect(iat, 12, prot, &prot);
    fprintf(g_out, "  [menu checks: %u log bytes]\n", (unsigned)total);
}

namespace rt3 {
void __fastcall RuleText_Expand(uint8_t* self, int, const char* src, char* dst, int32_t size, uint8_t* pl, uint8_t* co, uint8_t* st);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void* __fastcall Company_Owner(void* c);
}

namespace {
uint8_t g_rtPlayer[0x400], g_rtCompany[0x100];
uint32_t g_rtSeed;
int32_t __stdcall FakeRtVar(int32_t var, uint8_t* pl, uint8_t* co, uint8_t* st, int32_t p5, int32_t p6, int32_t p7, int32_t p8) {
    Logf("var %d %p %p %p %d %d %d %d", var, pl, co, st, p5, p6, p7, p8);
    uint32_t h = (uint32_t)var * 2654435761u ^ g_rtSeed;
    if (*(uint8_t*)(0x5f3d98 + var * 0x81 + 0x4b) == 4 && h % 2) return (int32_t)(uintptr_t)"NameText";
    return (int32_t)(h >> 8) % 3 == 0 ? 0 : (int32_t)(h % 200000) - 50000;
}
char* __fastcall FakeRtNum(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM) {
    Logf("num %d %08x%08x %d %d %d %d", style, (uint32_t)(*(uint64_t*)&v >> 32), (uint32_t)*(uint64_t*)&v, width, unused, inK, inM);
    sprintf(buf, "<%d:%.2f>", style, v);
    return buf;
}
char* __fastcall FakeRtNumSimple(char* buf, int32_t style, double v, int32_t width) {
    Logf("nums %d %.3f %d", style, v, width);
    sprintf(buf, "{%d:%.0f}", style, v);
    return buf;
}
const char* __fastcall FakeRtString(int32_t id) {
    static char strs[8][32];
    static int k;
    char* s = strs[k++ & 7];
    if (id >= 0xb10 && id <= 0xb15) sprintf(s, "fmt%d", id - 0xb10);
    else sprintf(s, "S%x", id);
    return s;
}
void* __fastcall FakeRtOwner(void* c) {
    Logf("owner");
    return g_rtPlayer;
}
int32_t __fastcall FakeRtValid(Collection* c, int, uint32_t id) { return id % 2; }
void* __fastcall FakeRtGet(Collection* c, int, uint32_t id) { return g_rtCompany; }
char* __fastcall FakeRtFormat(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                              const char* a6, const char* a7, const char* a8) {
    Logf("format [%s] [%s] [%s]", fmt, a1, a2);
    strcpy(dst, "w");
    return dst;
}
LPSTR WINAPI FakeRtCharNext(WORD cp, LPCSTR p, DWORD f) {
    Logf("next %u", cp);
    if (!*p) return (LPSTR)p;
    return (LPSTR)(p + ((uint8_t)*p >= 0xe0 && p[1] ? 2 : 1));
}
} // namespace

DIFFTEST(text_RuleTextExpand) {
    SavedGlobals sg({ 0x6d4034, 0x624328, 0x6cec74, 0x62be10 });
    UiTrace t;
    t.Add(0x42e580, (void*)&rt3::RuleVar_Get, (void*)&FakeRtVar);
    t.Add(0x51b700, (void*)&rt3::Num_Format, (void*)&FakeRtNum);
    t.Add(0x51bfe0, (void*)&rt3::Num_FormatSimple, (void*)&FakeRtNumSimple);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeRtString);
    t.Add(0x426ef0, (void*)&rt3::Company_Owner, (void*)&FakeRtOwner);
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeRtValid);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeRtGet);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeRtFormat);
    void** iat = (void**)0x5c8244;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeRtCharNext;
    static uint8_t opt[0x100], self[0x100], pl[0x400];
    strcpy((char*)self + 0x18, "scenario.gmx");
    hook::Global<uint8_t*>(0x6cec74) = opt;
    size_t total = 0;
    int found = 0;
    for (int it = 0; it < 20000; ++it) {
        g_rtSeed = Rnd();
        hook::Global<int32_t>(0x6d4034) = Rnd() % 3 == 0;
        hook::Global<uint16_t>(0x624328) = (uint16_t)(Rnd() % 1000);
        *(int32_t*)(opt + 0x68) = Rnd() % 2;
        *(uint32_t*)(g_rtPlayer + 0x1dd) = Rnd() % 4;
        std::string src;
        int parts = (int)(Rnd() % 8);
        for (int k = 0; k < parts; ++k) {
            switch (Rnd() % 6) {
            case 0: src += "Hello "; break;
            case 1: src += "\xe3\x81\x82x"; break;
            case 2: case 3: {
                const char* name = (const char*)0x5f3d9c + (Rnd() % 0x385) * 0x81;
                src += "[";
                src += Rnd() % 5 == 0 ? "Unknown Thing" : name;
                if (Rnd() % 3 == 0) {
                    src += "~~";
                    char f[8];
                    sprintf(f, "fmt%u", Rnd() % 7);
                    src += f;
                }
                if (Rnd() % 12) src += "]";
                break;
            }
            case 4: src += "]~~["; break;
            case 5: src += " tail"; break;
            }
        }
        int32_t size = Rnd() % 5 == 0 ? (int32_t)(Rnd() % 20) + 1 : 300;
        uint8_t* a4 = Rnd() % 3 ? pl : nullptr;
        uint8_t* a5 = Rnd() % 3 ? g_rtCompany : nullptr;
        uint8_t* a6 = Rnd() % 3 ? g_rtPlayer : nullptr;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            char dst[400];
            memset(dst, 0x5a, sizeof dst);
            logs[side] = t.Run([&] {
                if (side) rt3::RuleText_Expand(self, 0, src.c_str(), dst, size, a4, a5, a6);
                else
                    ((void(__fastcall*)(uint8_t*, int, const char*, char*, int32_t, uint8_t*, uint8_t*, uint8_t*))0x430560)(self, 0, src.c_str(), dst,
                                                                                                                         size, a4, a5, a6);
            });
            logs[side] += " -> [" + std::string(dst, dst + size) + "]";
            logs[side] += std::string(dst + size, dst + size + 4);
        }
        CompareLogs(logs[0], logs[1], "rule text", it);
        found += logs[0].find("var ") != std::string::npos;
        total += logs[0].size();
    }
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    fprintf(g_out, "  [rule text: %d with variables, %u log bytes]\n", found, (unsigned)total);
}

namespace rt3 {
void __fastcall Menu_Attach(HMENU* m);
void __fastcall Industries_ProduceQuarter(Collection* t);
int32_t __fastcall Engine_StepSetting(int32_t delta);
HWND __fastcall Engine_Window(void* e);
uint32_t __fastcall Engine_IsFullscreen(void* e);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
uint8_t __fastcall Obj_Flag3d4(void* o);
void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
}

namespace {
uint8_t g_maFull;
int32_t g_pqCount;
uint8_t g_pqInds[64][0x400];
uint32_t g_pqRand;
uint32_t __fastcall FakeMaFull(void* e) { return g_maFull; }
HWND __fastcall FakeMaWindow(void* e) { return (HWND)(uintptr_t)0x777; }
void __fastcall RecMaChecks(HMENU* m, int, char relight) { Logf("checks %p %d", m ? *m : 0, relight); }
BOOL WINAPI FakeMaSetMenu(HWND w, HMENU m) {
    Logf("setmenu %p %p", w, m);
    return TRUE;
}
int32_t __fastcall FakePqCount(Collection* c) { return g_pqCount; }
uint32_t __fastcall FakePqIdAt(Collection* c, int, uint32_t pos, int list) {
    Logf("idat %u %d", pos, list);
    return pos * 3 + 1;
}
void* __fastcall FakePqGet(Collection* c, int, uint32_t id) { return g_pqInds[id % 64]; }
void __fastcall RecPqProduce(uint8_t* ind, int, int32_t bulk) { Logf("produce %d %d", (int)((ind - g_pqInds[0]) / 0x400), bulk); }
int32_t __fastcall FakePqRand(int32_t lo, int32_t hi) {
    g_pqRand = g_pqRand * 69069u + 1;
    Logf("rand %d %d", lo, hi);
    return lo + (int32_t)(g_pqRand >> 16) % (hi - lo + 1);
}
char* __fastcall FakeSsNum(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM) {
    Logf("num %d %.1f %d %d %d %d", style, v, width, unused, inK, inM);
    sprintf(buf, "%.0f", v);
    return buf;
}
const char* __fastcall FakeSsString(int32_t id) {
    Logf("string %d", id);
    return id == 0x2f7 ? "level %1" : "off";
}
char* __fastcall FakeSsFormat(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                              const char* a6, const char* a7, const char* a8) {
    Logf("format [%s] [%s] %p %p %p %p %p %p %p", fmt, a1, a2, a3, a4, a5, a6, a7, a8);
    sprintf(dst, "%s/%s", fmt, a1);
    return dst;
}
void __fastcall RecSsStatus(uint8_t* s, int, const char* text, int32_t ms) { Logf("status [%s] %d", text, ms); }
void __fastcall RecSsChecks(HMENU* m, int, char relight) { Logf("checks %p %d", m, relight); }
void __fastcall RecSsSave(uint8_t* o, int, int32_t e) { Logf("save %d", e); }
} // namespace

DIFFTEST(ui_MenuAttach) {
    SavedGlobals sg({ 0x6d4024 });
    UiTrace t;
    t.Add(0x51f1a0, (void*)&rt3::Engine_IsFullscreen, (void*)&FakeMaFull);
    t.Add(0x51f0f0, (void*)&rt3::Engine_Window, (void*)&FakeMaWindow);
    t.Add(0x4625b0, (void*)&rt3::Menu_UpdateChecks, (void*)&RecMaChecks);
    void** iat = (void**)0x5c822c;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* saved = *iat;
    *iat = (void*)&FakeMaSetMenu;
    static uint8_t eng[16];
    for (int it = 0; it < 64; ++it) {
        hook::Global<uint8_t*>(0x6d4024) = it & 1 ? eng : nullptr;
        g_maFull = (it >> 1) & 1;
        HMENU m0 = (it >> 2) & 1 ? (HMENU)(uintptr_t)0x4444 : nullptr;
        std::string logs[2];
        int32_t flag[2];
        for (int side = 0; side < 2; ++side) {
            HMENU holder[2] = { m0, (HMENU)(uintptr_t)0x99 };
            logs[side] = t.Run([&] {
                if (side) rt3::Menu_Attach(holder);
                else ((void(__fastcall*)(HMENU*))0x464170)(holder);
            });
            flag[side] = (int32_t)(uintptr_t)holder[1];
        }
        CompareLogs(logs[0], logs[1], "menu attach", it);
        EXPECT_EQ(flag[0], flag[1], "menu attach flag %d", it);
    }
    *iat = saved;
    VirtualProtect(iat, 4, prot, &prot);
}

DIFFTEST(sim_IndustriesProduceQuarter) {
    SavedGlobals sg({ 0x6cec78 });
    UiTrace t;
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakePqCount);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakePqIdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakePqGet);
    t.Add(0x4101e0, (void*)&rt3::Industry_Produce, (void*)&RecPqProduce);
    t.Add(0x518d70, (void*)&rt3::Rand, (void*)&FakePqRand);
    static uint8_t world[0x100], out[2][64 * 0x400];
    hook::Global<uint8_t*>(0x6cec78) = world;
    for (int it = 0; it < 300; ++it) {
        world[0x11] = (uint8_t)(Rnd() % 32);
        g_pqCount = (int32_t)(Rnd() % 60);
        for (auto& ind : g_pqInds) ind[0x3d4] = Rnd() % 4 != 0;
        uint32_t r0 = Rnd();
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            g_pqRand = r0;
            for (auto& ind : g_pqInds) *(uint64_t*)(ind + 0x3d9) = 0;
            logs[side] = t.Run([&] {
                if (side) rt3::Industries_ProduceQuarter((Collection*)world);
                else ((void(__fastcall*)(Collection*))0x413580)((Collection*)world);
            });
            for (int k = 0; k < 64; ++k) memcpy(out[side] + k * 0x400, g_pqInds[k], 0x400);
        }
        CompareLogs(logs[0], logs[1], "industries quarter", it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "industries quarter state %d", it);
    }
}

DIFFTEST(ui_EngineStepSetting) {
    SavedGlobals sg({ 0x6d4024, 0x6d401c, 0x6ada8c, 0x6cec74 });
    UiTrace t;
    t.Add(0x51b700, (void*)&rt3::Num_Format, (void*)&FakeSsNum);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeSsString);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeSsFormat);
    t.Add(0x5386e0, (void*)&rt3::StatusLine_Set, (void*)&RecSsStatus);
    t.Add(0x4625b0, (void*)&rt3::Menu_UpdateChecks, (void*)&RecSsChecks);
    t.Add(0x484910, (void*)&rt3::Options_Save, (void*)&RecSsSave);
    static uint8_t* eng = new uint8_t[0x114800];
    static uint8_t ui[0x40], opt[0x40], holder[8];
    hook::Global<uint8_t*>(0x6d4024) = eng;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    hook::Global<uint8_t*>(0x6ada8c) = holder;
    hook::Global<uint8_t*>(0x6cec74) = opt;
    for (int it = 0; it < 400; ++it) {
        int32_t v0 = (int32_t)(Rnd() % 12) - 4, delta = (int32_t)(Rnd() % 9) - 4;
        std::string logs[2];
        int32_t r[2], v[2];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(eng + 0x1146c2) = v0;
            logs[side] = t.Run([&] { r[side] = side ? rt3::Engine_StepSetting(delta) : ((int32_t(__fastcall*)(int32_t))0x482920)(delta); });
            v[side] = *(int32_t*)(eng + 0x1146c2);
        }
        CompareLogs(logs[0], logs[1], "step setting", it);
        EXPECT_EQ(r[0], r[1], "step setting result %d", it);
        EXPECT_EQ(v[0], v[1], "step setting value %d", it);
    }
}

namespace rt3 {
void Slots_Find();
void* __fastcall FreeList_Take(uint8_t* q);
int32_t __stdcall Peer_Think(int32_t);
int32_t __stdcall Peer_SendRoom(const char* msg);
int32_t __stdcall Peer_JoinNamed(const char* name);
void Peer_LeaveStaging();
int32_t __fastcall Lobby_ListGames(uint8_t* lobby);
int32_t __fastcall Lobby_UpdateList(uint8_t* lobby);
void Peer_Shutdown();
void* Engine_FirstListEntry();
uint8_t* __fastcall Engine_FindTagged(char tag);
int32_t __fastcall Engine_DeviceCall68(uint8_t* e, int, int32_t v);
}

namespace {
int32_t g_gsRet;
int32_t __fastcall FakeGsThink(void* p) {
    Logf("think %p", p);
    return ++g_gsRet;
}
int32_t __fastcall FakeGsSend(void* p, int32_t e, const char* m, int32_t z) {
    Logf("send %p %d [%s] %d", p, e, m, z);
    return ++g_gsRet;
}
void __fastcall FakeGsKey(char* key, const char* name) {
    Logf("key [%s]", name);
    sprintf(key, "#%s", name);
}
int32_t __fastcall FakeGsJoin(void* p, const char* key, uint32_t cb, int32_t a, int32_t b) {
    Logf("join %p [%s] %x %d %d", p, key, cb, a, b);
    return ++g_gsRet;
}
void __fastcall FakeGsLeave(void* p, int32_t room) { Logf("leave %p %d", p, room); }
void __fastcall FakeGsRoom(void* p, const char* s, int32_t a) { Logf("room %p [%s] %d", p, s, a); }
int32_t __fastcall FakeGsList(void* p, int32_t e, uint32_t cb, int32_t z) {
    Logf("list %p %d %x %d %u", p, e, cb, z, hook::Global<uint32_t>(0x6ae4d8 + 4 * 0x1234));
    return ++g_gsRet;
}
int32_t __fastcall FakeGsUpdate(void* p, uint8_t* keys, int32_t n, int32_t z, uint32_t cb, uint8_t* arg) {
    Logf("update %p %d %d %d %d %d %d %d %d | %d %d %x %p", p, keys[0], keys[1], keys[2], keys[3], keys[4], keys[5], keys[6], keys[7], n, z, cb, arg);
    return ++g_gsRet;
}
void __fastcall FakeGsStop(void* p) { Logf("stop %p", p); }
void __fastcall FakeGsDisc(void* p) { Logf("disconnect %p", p); }
void __fastcall FakeGsFree(void* p) { Logf("free %p", p); }
int32_t __stdcall FakeGsDev68(void* dev, int32_t v, int32_t a, int32_t b, int32_t c) {
    Logf("dev68 %d %d %d %d", v, a, b, c);
    return 77;
}
uint8_t* CallSlots(uint32_t fn, uint8_t* base, int32_t key, int32_t* edxOut) {
    uint8_t* r;
    int32_t e;
    __asm {
        mov eax, base
        mov edx, key
        call fn
        mov r, eax
        mov e, edx
    }
    *edxOut = e;
    return r;
}
} // namespace

DIFFTEST(net_GameSpyGlue) {
    SavedGlobals sg({ 0x6cd970, 0x6cd978, 0x6cd920, 0x6cd97c, 0x6d408c, 0x6d40b0, 0x6d40dc });
    static uint32_t savedList[0x7d00];
    memcpy(savedList, (void*)0x6ae4d8, sizeof savedList);
    UiTrace t;
    t.Add(0x58db00, nullptr, (void*)&FakeGsThink);
    t.Add(0x58d9e0, nullptr, (void*)&FakeGsSend);
    t.Add(0x58e650, nullptr, (void*)&FakeGsKey);
    t.Add(0x58df20, nullptr, (void*)&FakeGsJoin);
    t.Add(0x58d960, nullptr, (void*)&FakeGsLeave);
    t.Add(0x58e100, nullptr, (void*)&FakeGsRoom);
    t.Add(0x58da60, nullptr, (void*)&FakeGsList);
    t.Add(0x58d970, nullptr, (void*)&FakeGsUpdate);
    t.Add(0x58db20, nullptr, (void*)&FakeGsStop);
    t.Add(0x58d830, nullptr, (void*)&FakeGsDisc);
    t.Add(0x58de90, nullptr, (void*)&FakeGsFree);
    static uint8_t peer[0x100], lobby[0xa000], slots[0x80 * 0x11c], nodes[8][0x40], listHdr[12], q[0x14];
    static uint8_t dev[8], devHolder[0x10], eng[0x200];
    static void* devVt[0x20];
    devVt[0x68 / 4] = (void*)&FakeGsDev68;
    *(void***)dev = devVt;
    *(uint8_t**)(devHolder + 8) = dev;
    *(uint8_t**)(eng + 0x18e) = devHolder;
    static uint8_t* listHead;
    static uint8_t* tagHead;
    for (int it = 0; it < 3000; ++it) {
        int32_t g978 = Rnd() % 3 != 0, g920 = Rnd() % 3 != 0;
        void* peerPtr = Rnd() % 4 ? peer : nullptr;
        for (int k = 0; k < 0x80; ++k) {
            *(int32_t*)(slots + k * 0x11c + 0x104) = Rnd() % 3 == 0;
            *(int32_t*)(slots + k * 0x11c + 0x10c) = (int32_t)(Rnd() % 40);
        }
        int32_t key = (int32_t)(Rnd() % 40);
        int nn = (int)(Rnd() % 4);
        for (int k = 0; k < 8; ++k) *(uint8_t**)nodes[k] = k + 1 < nn ? nodes[k + 1] : nullptr;
        for (int k = 0; k < 8; ++k) nodes[k][0x1c] = (uint8_t)(Rnd() % 6);
        const char* msg = Rnd() % 4 ? "hello" : nullptr;
        int32_t devFlag = Rnd() % 2, devV = (int32_t)(Rnd() % 100);
        char tag = (char)(Rnd() % 7);
        bool listOn = it % 5 != 0;
        bool firstOn = Rnd() % 4 != 0;
        std::string logs[2];
        uint8_t hdrOut[2][12];
        for (int side = 0; side < 2; ++side) {
            hook::Global<void*>(0x6cd970) = peerPtr;
            hook::Global<int32_t>(0x6cd978) = g978;
            hook::Global<int32_t>(0x6cd920) = g920;
            hook::Global<int32_t>(0x6cd97c) = 5;
            hook::Global<uint32_t>(0x6ae4d8 + 4 * 0x1234) = 9;
            hook::Global<int32_t>(0x6d40dc) = devFlag;
            *(uint8_t**)listHdr = nn ? nodes[0] : nullptr;
            *(uint8_t**)(listHdr + 4) = nn ? nodes[nn - 1] : nullptr;
            *(int32_t*)(listHdr + 8) = nn;
            *(uint8_t**)q = listHdr;
            *(int32_t*)(q + 0xc) = listOn;
            *(int32_t*)(q + 0x10) = 0x40;
            for (int k = 0; k < 8; ++k) *(uint8_t**)nodes[k] = k + 1 < nn ? nodes[k + 1] : nullptr;
            listHead = nn ? nodes[0] : nullptr;
            tagHead = nn ? nodes[0] : nullptr;
            hook::Global<uint8_t**>(0x6d408c) = firstOn ? &listHead : nullptr;
            hook::Global<uint8_t**>(0x6d40b0) = &tagHead;
            *(int32_t*)(lobby + 0x9874) = 0;
            *(int32_t*)(lobby + 0x8f18) = 0;
            g_gsRet = 100;
            logs[side] = t.Run([&] {
                int32_t edx;
                uint8_t* s = CallSlots(side ? (uint32_t)(uintptr_t)&rt3::Slots_Find : 0x4691d0, slots, key, &edx);
                Logf("slot %d edx %d", s ? (int)((s - slots) / 0x11c) : -1, edx);
                if (listOn) {
                    void* n = side ? rt3::FreeList_Take(q) : ((void*(__fastcall*)(uint8_t*))0x4696d0)(q);
                    Logf("take %p", n);
                }
                Logf("think %d", side ? rt3::Peer_Think(3) : ((int32_t(__stdcall*)(int32_t))0x469680)(3));
                Logf("sendr %d", side ? rt3::Peer_SendRoom(msg) : ((int32_t(__stdcall*)(const char*))0x469660)(msg));
                Logf("joinr %d", side ? rt3::Peer_JoinNamed("rt3") : ((int32_t(__stdcall*)(const char*))0x469620)("rt3"));
                if (side) rt3::Peer_LeaveStaging();
                else ((void (*)())0x4694c0)();
                Logf("listr %d", side ? rt3::Lobby_ListGames(lobby) : ((int32_t(__fastcall*)(uint8_t*))0x469580)(lobby));
                Logf("updr %d", side ? rt3::Lobby_UpdateList(lobby) : ((int32_t(__fastcall*)(uint8_t*))0x469520)(lobby));
                Logf("first %p", side ? rt3::Engine_FirstListEntry() : ((void* (*)())0x521d40)());
                Logf("tag %p", side ? rt3::Engine_FindTagged(tag) : ((uint8_t*(__fastcall*)(char))0x521b40)(tag));
                Logf("dev %d", side ? rt3::Engine_DeviceCall68(eng, 0, devV) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x521d10)(eng, 0, devV));
                if (side) rt3::Peer_Shutdown();
                else ((void (*)())0x469450)();
            });
            char tail[64];
            sprintf(tail, " %p %d %d", hook::Global<void*>(0x6cd970), hook::Global<int32_t>(0x6cd97c),
                    *(int32_t*)(lobby + 0x9874) * 2 + *(int32_t*)(lobby + 0x8f18));
            logs[side] += tail;
            memcpy(hdrOut[side], listHdr, sizeof listHdr);
        }
        CompareLogs(logs[0], logs[1], "gamespy glue", it);
        EXPECT_EQ(memcmp(hdrOut[0], hdrOut[1], 12), 0, "free list header %d", it);
    }
    memcpy((void*)0x6ae4d8, savedList, sizeof savedList);
}

namespace rt3 {
void StreamA_Put();
void StreamB_Put();
void StreamA_PutPending();
void StreamA_Get();
void StreamB_Read();
int32_t Cpu_HasCpuid();
int32_t __stdcall Cpu_ProbeFeature(int32_t kind);
}

namespace {
uint32_t g_bsSeed;
int32_t g_bsRefillMode;
int32_t __fastcall RecBsFlush(uint8_t* buf, int32_t n) {
    uint32_t h = 0;
    for (int32_t k = 0; k < n; ++k) h = h * 31 + hook::Global<uint8_t*>(0xda7dd8)[k];
    Logf("flush %x %d %08x", (uint32_t)(uintptr_t)buf, n, h);
    return 55;
}
int32_t __fastcall FakeBsRefill(uint8_t* buf, int32_t n) {
    Logf("refill %x %d", (uint32_t)(uintptr_t)buf, n);
    if (g_bsRefillMode == 1) return -1;
    int32_t got = g_bsRefillMode == 2 ? 0 : (int32_t)(g_bsSeed % 40) + 1;
    for (int32_t k = 0; k < got; ++k) buf[k] = (uint8_t)(g_bsSeed >> (k % 24));
    return got;
}
int32_t CallPut(uint32_t fn, uint32_t b, void* cb) {
    int32_t r;
    __asm {
        push esi
        mov esi, cb
        push b
        call fn
        pop esi
        mov r, eax
    }
    return r;
}
int32_t CallPending(uint32_t fn, void* cb, uint8_t* c, int32_t eaxIn) {
    int32_t r;
    __asm {
        push ebx
        push edi
        mov ebx, cb
        mov edi, c
        mov eax, eaxIn
        call fn
        pop edi
        pop ebx
        mov r, eax
    }
    return r;
}
int32_t CallGet(uint32_t fn, void* cb) {
    int32_t r;
    __asm {
        push esi
        mov esi, cb
        call fn
        pop esi
        mov r, eax
    }
    return r;
}
int32_t CallRead(uint32_t fn, int32_t n, void* cb, uint8_t* dst) {
    int32_t r;
    __asm {
        push ebx
        push edi
        mov ebx, n
        mov edi, cb
        push dst
        call fn
        pop edi
        pop ebx
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(save_ByteStreams) {
    SavedGlobals sg({ 0xda7dd4, 0xda7dd8, 0xdafde4, 0xdb7dec, 0xda7dd0, 0xd9fdc8, 0xdafde0, 0xdb7de8 });
    static uint8_t bufA[0x8010], bufB[0x8010], coder[0x200], dst[64];
    static uint8_t savedIn[0x8000], savedInB[0x8000];
    memcpy(savedIn, (void*)0xd9fdd0, sizeof savedIn);
    memcpy(savedInB, (void*)0xdafde8, sizeof savedInB);
    UiTrace t;
    for (int it = 0; it < 4000; ++it) {
        g_bsSeed = Rnd();
        g_bsRefillMode = (int32_t)(Rnd() % 4);
        int32_t posA = Rnd() % 3 == 0 ? 0x7fff - (int32_t)(Rnd() % 4) : (int32_t)(Rnd() % 0x8000);
        int32_t posB = Rnd() % 3 == 0 ? 0x7fff : (int32_t)(Rnd() % 0x8000);
        int32_t leftA = (int32_t)(Rnd() % 4) - 1, leftB = (int32_t)(Rnd() % 5) - 1;
        for (int k = 0; k < 0x40; ++k) coder[0x3c + k] = (uint8_t)Rnd();
        *(int32_t*)(coder + 0x18) = (int32_t)(Rnd() % 8) - 2;
        uint32_t b = Rnd() & 0x1ff;
        bool cb = Rnd() % 5 != 0;
        int32_t n = (int32_t)(Rnd() % 12) - 2;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(bufA, 0, sizeof bufA);
            memset(bufB, 0, sizeof bufB);
            memset(dst, 0xee, sizeof dst);
            memcpy((void*)0xd9fdd0, savedIn, 0x100);
            memcpy((void*)0xdafde8, savedInB, 0x100);
            hook::Global<uint8_t*>(0xda7dd8) = bufA;
            hook::Global<uint8_t*>(0xdb7dec) = bufB;
            hook::Global<int32_t>(0xda7dd4) = posA;
            hook::Global<int32_t>(0xdafde4) = posB;
            hook::Global<int32_t>(0xd9fdc8) = leftA;
            hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fdd0 + 5;
            hook::Global<int32_t>(0xdb7de8) = leftB;
            hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafde8 + 7;
            void* fl = cb ? (void*)&RecBsFlush : nullptr;
            void* rf = cb ? (void*)&FakeBsRefill : nullptr;
            logs[side] = t.Run([&] {
                Logf("putA %d", CallPut(side ? (uint32_t)(uintptr_t)&rt3::StreamA_Put : 0x56b940, b, fl));
                Logf("putB %d", CallPut(side ? (uint32_t)(uintptr_t)&rt3::StreamB_Put : 0x56c320, b, fl));
                Logf("pend %d", CallPending(side ? (uint32_t)(uintptr_t)&rt3::StreamA_PutPending : 0x56bbb0, fl, coder, 1234));
                Logf("get %d", CallGet(side ? (uint32_t)(uintptr_t)&rt3::StreamA_Get : 0x56b8f0, rf));
                Logf("read %d", CallRead(side ? (uint32_t)(uintptr_t)&rt3::StreamB_Read : 0x56c370, n, rf, dst));
            });
            char tail[128];
            sprintf(tail, " %d %d %d %x %d %x", hook::Global<int32_t>(0xda7dd4), hook::Global<int32_t>(0xdafde4), hook::Global<int32_t>(0xd9fdc8),
                    hook::Global<uint32_t>(0xda7dd0), hook::Global<int32_t>(0xdb7de8), hook::Global<uint32_t>(0xdafde0));
            logs[side] += tail;
            for (int k = 0; k < 16; ++k) logs[side] += " " + std::to_string(dst[k]);
            logs[side] += " " + std::to_string(bufA[posA & 0x7fff]) + " " + std::to_string(bufB[posB & 0x7fff]);
        }
        CompareLogs(logs[0], logs[1], "byte streams", it);
    }
    memcpy((void*)0xd9fdd0, savedIn, sizeof savedIn);
    memcpy((void*)0xdafde8, savedInB, sizeof savedInB);
    EXPECT_EQ(rt3::Cpu_HasCpuid(), ((int32_t(*)())0x55ccc0)(), "cpuid probe");
    for (int32_t k = -1; k <= 9; ++k)
        EXPECT_EQ(rt3::Cpu_ProbeFeature(k), ((int32_t(__stdcall*)(int32_t))0x55cd40)(k), "cpu feature %d", k);
}

namespace rt3 {
void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
uint8_t* __stdcall Textures_Find(int32_t key);
uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep);
int32_t __fastcall Textures_QueryDesc(uint8_t* e, int, int32_t key, int32_t lock);
void __fastcall CmdQueue_Clear(uint8_t* q);
uint8_t* __fastcall CmdNode_New(int32_t clear, int32_t lock);
int32_t __fastcall CmdQueue_Broadcast(int32_t a, int32_t b, int32_t c);
int32_t __fastcall Music_CreateC(uint8_t* m);
int32_t __fastcall Music_Create10(uint8_t* m);
void __fastcall Music_Shutdown(uint8_t* m);
void File_Size();
}

namespace {
uint8_t g_elNodes[16][0x30], g_elTex[4][0x2800], g_elBlock[0x100];
uint8_t g_elCom[0x10];
void* g_elComVt[0x20];
int32_t g_elHr;
std::string ElPtr(const void* p) {
    char b[32];
    uint8_t* q = (uint8_t*)p;
    if (q >= g_elNodes[0] && q < g_elNodes[16]) sprintf(b, "node%d+%d", (int)((q - g_elNodes[0]) / 0x30), (int)((q - g_elNodes[0]) % 0x30));
    else if (q >= g_elTex[0] && q < g_elTex[4]) sprintf(b, "tex%d", (int)((q - g_elTex[0]) / 0x2800));
    else if (q >= g_elBlock && q < g_elBlock + 0x100) sprintf(b, "blk+%d", (int)(q - g_elBlock));
    else sprintf(b, "%p", p);
    return b;
}
void WINAPI FakeElEnter(LPCRITICAL_SECTION cs) { Logf("enter %p", cs); }
void WINAPI FakeElLeave(LPCRITICAL_SECTION cs) { Logf("leave %p", cs); }
void __cdecl RecElDelete(void* p) { Logf("delete %s", ElPtr(p).c_str()); }
HRESULT __stdcall FakeElDesc(void* dev, int32_t key, uint32_t* desc, int32_t z) {
    Logf("desc %d %u %d", key, desc[0], z);
    for (int k = 1; k < 0x17; ++k) desc[k] = (uint32_t)(key * 7 + k);
    return key % 3 == 0 ? -1 : 0;
}
HRESULT WINAPI FakeElCreate(const void* clsid, void* outer, DWORD ctx, const void* iid, void** out) {
    Logf("create %p %p %u %p", clsid, outer, ctx, iid);
    if (g_elHr < 0) return g_elHr;
    *out = g_elCom;
    return 0;
}
HRESULT __stdcall FakeElInit(void* o, const void* a) {
    Logf("init %p", a);
    return g_elHr == 1 ? -5 : 0;
}
void WINAPI FakeEl494(void* a, void* b) { Logf("co494 %p %p", a, b); }
HRESULT __stdcall FakeElStop(void* o, int32_t a) {
    Logf("stop %d", a);
    return 0;
}
ULONG __stdcall FakeElRelease(void* o) {
    Logf("release %p", o);
    return 0;
}
int32_t CallFileSize(uint32_t fn, const char* name) {
    int32_t r;
    __asm {
        mov eax, name
        call fn
        mov r, eax
    }
    return r;
}
struct IatSlot {
    void** at;
    void* saved;
    DWORD prot;
    IatSlot(uint32_t a, void* fn) : at((void**)a) {
        VirtualProtect(at, 4, PAGE_READWRITE, &prot);
        saved = *at;
        *at = fn;
    }
    ~IatSlot() {
        *at = saved;
        VirtualProtect(at, 4, prot, &prot);
    }
};
} // namespace

DIFFTEST(engine_NodePoolsAndLists) {
    SavedGlobals sg({ 0x6d408c, 0x6d40a8, 0x6d4064, 0x6d4084, 0x6d40b0, 0x6d40e4 });
    static uint8_t savedCopy[0x9bd * 4];
    memcpy(savedCopy, (void*)0x6d4220, sizeof savedCopy);
    IatSlot i1(0x5c8070, (void*)&FakeElEnter), i2(0x5c8074, (void*)&FakeElLeave), i3(0x5c81fc, (void*)&FakeElEnter),
        i4(0x5c81f8, (void*)&FakeElLeave), i5(0x5c848c, (void*)&FakeElCreate), i6(0x5c8494, (void*)&FakeEl494);
    UiTrace t;
    t.Add(0x53b080, nullptr, (void*)&RecElDelete);
    g_elComVt[0x34 / 4] = (void*)&FakeElInit;
    g_elComVt[0x60 / 4] = (void*)&FakeElStop;
    g_elComVt[8 / 4] = (void*)&FakeElRelease;
    *(void***)g_elCom = g_elComVt;
    static uint8_t list[12], pool[0x14], sess[0x9000], q[12], chans[3][0x10], dev[8], devHolder[0x10], eng[0x200], music[0x20];
    static void* devVt[0x40];
    devVt[0x88 / 4] = (void*)&FakeElDesc;
    *(void***)dev = devVt;
    *(uint8_t**)(devHolder + 8) = dev;
    *(uint8_t**)(eng + 0x18e) = devHolder;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_filesize.tmp");
    for (int it = 0; it < 3000; ++it) {
        int nPool = (int)(Rnd() % 5), nQ = (int)(Rnd() % 4), nTex = (int)(Rnd() % 5);
        bool poolOn = Rnd() % 4 != 0;
        int32_t size = Rnd() % 3 == 0 ? 0x400 : (int32_t)(Rnd() % 0x300) + 1;
        int32_t key = (int32_t)(Rnd() % 6), keep = (int32_t)(Rnd() % 2), lock = (int32_t)(Rnd() % 3);
        int32_t clear = (int32_t)(Rnd() % 3), plock = (int32_t)(Rnd() % 2);
        int32_t inBlock = (int32_t)(Rnd() % 3);
        g_elHr = (int32_t)(Rnd() % 3) - 1;
        uint32_t texIds[4];
        for (auto& id : texIds) id = Rnd() % 6;
        bool musicObj = Rnd() % 2 != 0;
        int32_t musicStopped = (int32_t)(Rnd() % 2);
        std::string logs[2], state[2];
        for (int side = 0; side < 2; ++side) {
            memset(g_elNodes, 0x11, sizeof g_elNodes);
            for (int k = 0; k < nPool; ++k) *(uint8_t**)g_elNodes[k] = k + 1 < nPool ? g_elNodes[k + 1] : nullptr;
            *(uint8_t**)list = nPool ? g_elNodes[0] : nullptr;
            *(uint8_t**)(list + 4) = nPool ? g_elNodes[nPool - 1] : nullptr;
            *(int32_t*)(list + 8) = nPool;
            *(uint8_t**)pool = list;
            *(uint8_t**)(pool + 4) = g_elNodes[0];
            *(uint8_t**)(pool + 8) = g_elNodes[12];
            *(int32_t*)(pool + 0xc) = poolOn;
            *(int32_t*)(pool + 0x10) = 0x28;
            for (int k = 0; k < nQ; ++k) *(uint8_t**)g_elNodes[8 + k] = k + 1 < nQ ? g_elNodes[9 + k] : nullptr;
            *(uint8_t**)q = nQ ? g_elNodes[8] : nullptr;
            *(uint8_t**)(q + 4) = nQ ? g_elNodes[7 + nQ] : nullptr;
            *(int32_t*)(q + 8) = nQ;
            *(uint8_t**)(g_elNodes[15]) = nullptr;
            *(uint8_t**)(sess + 0x8f3c) = pool;
            *(uint8_t**)(sess + 0x8f40) = g_elBlock;
            for (int k = 0; k < 4; ++k) {
                *(uint8_t**)g_elTex[k] = k + 1 < nTex ? g_elTex[k + 1] : nullptr;
                *(uint32_t*)(g_elTex[k] + 4) = texIds[k];
                memset(g_elTex[k] + 0x2698, 0, 0x5c);
            }
            static uint8_t* texHead;
            texHead = nTex ? g_elTex[0] : nullptr;
            hook::Global<uint8_t**>(0x6d408c) = &texHead;
            hook::Global<int32_t>(0x6d40a8) = 3;
            hook::Global<uint8_t*>(0x6d4064) = pool;
            hook::Global<int32_t>(0x6d4084) = 2;
            hook::Global<int32_t>(0x6d40e4) = 99;
            for (int k = 0; k < 3; ++k) {
                *(uint8_t**)chans[k] = k < 2 ? chans[k + 1] : nullptr;
                *(uint8_t**)(chans[k] + 4) = nullptr;
                *(uint8_t**)(chans[k] + 8) = k == 1 ? g_elNodes[15] : nullptr;
                *(int32_t*)(chans[k] + 0xc) = k;
            }
            static uint8_t* chanHead;
            chanHead = chans[0];
            hook::Global<uint8_t**>(0x6d40b0) = &chanHead;
            memset(music, 0, sizeof music);
            *(void**)(music + 8) = musicObj ? (void*)g_elCom : nullptr;
            *(int32_t*)(music + 4) = musicStopped;
            uint8_t texRef[0x20] = {};
            *(uint32_t*)(texRef + 0x18) = (uint32_t)key;
            logs[side] = t.Run([&] {
                void* p = side ? rt3::NetPacket_Alloc(sess, 0, size) : ((void*(__fastcall*)(uint8_t*, int, int32_t))0x469800)(sess, 0, size);
                Logf("alloc %s", ElPtr(p).c_str());
                uint8_t* fp = inBlock == 0 ? g_elBlock + 8 : inBlock == 1 ? g_elNodes[13] : g_elNodes[3];
                if (side) rt3::NetPacket_Free(sess, 0, fp);
                else ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x469890)(sess, 0, fp);
                Logf("find %s", ElPtr(side ? rt3::Textures_Find(key) : ((uint8_t*(__stdcall*)(int32_t))0x521d60)(key)).c_str());
                Logf("lookup %s", ElPtr(side ? rt3::Textures_Lookup(texRef, keep) : ((uint8_t*(__stdcall*)(uint8_t*, int32_t))0x522050)(texRef, keep)).c_str());
                Logf("query %d", side ? rt3::Textures_QueryDesc(eng, 0, key, lock)
                                      : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x521f90)(eng, 0, key, lock));
                Logf("cmdnew %s", ElPtr(side ? rt3::CmdNode_New(clear, plock) : ((uint8_t*(__fastcall*)(int32_t, int32_t))0x5220d0)(clear, plock)).c_str());
                Logf("bcast %d", side ? rt3::CmdQueue_Broadcast(5, 6, 7) : ((int32_t(__fastcall*)(int32_t, int32_t, int32_t))0x522240)(5, 6, 7));
                if (side) rt3::CmdQueue_Clear(q);
                else ((void(__fastcall*)(uint8_t*))0x5221a0)(q);
                Logf("mc %d", side ? rt3::Music_CreateC(music) : ((int32_t(__fastcall*)(uint8_t*))0x557af0)(music));
                Logf("m10 %d", side ? rt3::Music_Create10(music) : ((int32_t(__fastcall*)(uint8_t*))0x557b30)(music));
                if (side) rt3::Music_Shutdown(music);
                else ((void(__fastcall*)(uint8_t*))0x5579d0)(music);
            });
            char b[160];
            sprintf(b, " %s %s %d | %s %s %d | %d %d", ElPtr(*(void**)list).c_str(), ElPtr(*(void**)(list + 4)).c_str(), *(int32_t*)(list + 8),
                    ElPtr(*(void**)q).c_str(), ElPtr(*(void**)(q + 4)).c_str(), *(int32_t*)(q + 8), hook::Global<int32_t>(0x6d40a8),
                    hook::Global<int32_t>(0x6d4084));
            state[side] = b;
            for (int k = 0; k < 3; ++k)
                state[side] += " " + ElPtr(*(void**)(chans[k] + 4)) + "/" + ElPtr(*(void**)(chans[k] + 8)) + "/" + std::to_string(*(int32_t*)(chans[k] + 0xc));
            for (int k = 0; k < 16; ++k) state[side] += " " + ElPtr(*(void**)g_elNodes[k]) + ":" + std::to_string(*(int32_t*)(g_elNodes[k] + 0x18));
            for (int k = 0; k < 4; ++k) state[side] += " " + std::to_string(*(uint32_t*)(g_elTex[k] + 0x269c));
            state[side] += " " + std::to_string(*(int32_t*)(music + 4)) + " " + ElPtr(*(void**)(music + 0xc)) + " " + ElPtr(*(void**)(music + 0x10));
            state[side] += " " + std::to_string(hook::Global<uint32_t>(0x6d4220 + 4));
        }
        CompareLogs(logs[0], logs[1], "node pools", it);
        CompareLogs(state[0], state[1], "node pools state", it);
    }
    memcpy((void*)0x6d4220, savedCopy, sizeof savedCopy);
    // File size of a scratch file, and of a missing one.
    FILE* f = fopen(path, "wb");
    for (int k = 0; k < 1234; ++k) fputc(k, f);
    fclose(f);
    EXPECT_EQ(CallFileSize((uint32_t)(uintptr_t)&rt3::File_Size, path), CallFileSize(0x46c2f0, path), "file size");
    EXPECT_EQ(CallFileSize((uint32_t)(uintptr_t)&rt3::File_Size, "Z:\\nope\\none.bin"), CallFileSize(0x46c2f0, "Z:\\nope\\none.bin"), "file size missing");
    DeleteFileA(path);
}

namespace rt3 {
void NamedValue_Find();
void Models_FindByName();
void ObjFlags_Set();
void Map_BlockHasFlag2();
void Msg_MarkSeen();
void EntryWalk_Begin();
void Text_SplitPlus();
void EntryWalk_Find();
int32_t __fastcall HasString(int32_t id);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
struct HashMap;
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);
int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i);
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
}

namespace {
uint32_t g_raSeed;
void* g_raList;
int32_t __fastcall FakeRaHas(int32_t id) { return (id * 7 + (int32_t)g_raSeed) % 3 != 0 ? 0x101 : 0x100; }
const char* __fastcall FakeRaString(int32_t id) {
    static char b[4][16];
    static int k;
    char* s = b[k++ & 3];
    sprintf(s, id == 0xdf2 ? "None" : "Str%d", id % 9);
    return s;
}
int32_t __fastcall FakeRaCell(void* m, int, int32_t x, int32_t y, int32_t which) {
    Logf("cell %d %d %d", x, y, which);
    return (uint32_t)(x * 31 + y * 17 + (int32_t)g_raSeed) % 23 == 0;
}
int32_t __fastcall FakeRaHash(rt3::HashMap* m, int, uint32_t key) {
    Logf("hash %p %u", m, key);
    return (int32_t)(uintptr_t)g_raList;
}
uint32_t __fastcall FakeRaCrc(const uint8_t* d, uint32_t n) {
    Logf("crc %u", n);
    return g_raSeed % 4;
}
int32_t __fastcall FakeRaClear(uint8_t* o, int32_t i) {
    Logf("clear %p %d", o, i);
    return 4321;
}
uint32_t __fastcall FakeRaV10(uint8_t* o) {
    Logf("v10");
    return 10;
}
uint32_t __fastcall FakeRaVc(uint8_t* o) {
    Logf("vc");
    return 12;
}
// Calls fn with the given registers and stack argument count; returns EAX, ECX, EDX.
void RaCall(uint32_t fn, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t s, uint32_t s1, int nStack, uint32_t out[3]) {
    uint32_t ra, rc, rd;
    __asm {
        push ebx
        push esi
        mov eax, nStack
        test eax, eax
        jz nostack
        push s1
    nostack:
        mov eax, a
        mov ecx, b
        mov edx, c
        mov ebx, d
        mov esi, s
        call fn
        mov ra, eax
        mov rc, ecx
        mov rd, edx
        pop esi
        pop ebx
    }
    out[0] = ra;
    out[1] = rc;
    out[2] = rd;
}
} // namespace

DIFFTEST(misc_RegisterArgFunctions) {
    SavedGlobals sg({ 0x62c120, 0x6acd50, 0x6acd44, 0x6acd48, 0x6acd4c, 0x6acd54, 0x6cd8d8 });
    static uint8_t savedBuckets[0x6ad96c + 0xc - 0x6acd78];
    memcpy(savedBuckets, (void*)0x6acd78, sizeof savedBuckets);
    UiTrace t;
    t.Add(0x51c900, (void*)&rt3::HasString, (void*)&FakeRaHas);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeRaString);
    t.Add(0x448af0, (void*)&rt3::Map_CellFlag, (void*)&FakeRaCell);
    t.Add(0x53dae0, (void*)&rt3::Hash_Get, (void*)&FakeRaHash);
    t.Add(0x53d870, (void*)&rt3::Crc32, (void*)&FakeRaCrc);
    t.Add(0x4752b0, (void*)&rt3::Obj_EntryClear1d8, (void*)&FakeRaClear);
    static uint8_t map[0x2200], table[0x10 + 12 * 8], objs[8][16], models[4][0xa0], msg[0x20], msgHdr[0x20], other[0x2600], mp[0x9900];
    static void* vt[8];
    vt[0x10 / 4] = (void*)&FakeRaV10;
    vt[0xc / 4] = (void*)&FakeRaVc;
    for (auto& o : objs) *(void***)o = vt;
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6cd8d8) = mp;
    static void* nodes[3];
    for (int it = 0; it < 5000; ++it) {
        g_raSeed = Rnd();
        *(uint32_t*)(map + 0x215d) = 4 + Rnd() % 40;
        *(uint32_t*)(map + 0x2161) = 4 + Rnd() % 40;
        int32_t n = (int32_t)(Rnd() % 9) - 1;
        *(int32_t*)(table + 0xc) = n;
        for (int k = 0; k < 8; ++k) {
            *(uint8_t**)(table + 0x10 + k * 12) = Rnd() % 3 ? objs[Rnd() % 8] : nullptr;
            *(uint32_t*)(table + 0x14 + k * 12) = Rnd();
            *(uint32_t*)(table + 0x18 + k * 12) = Rnd() % 3 ? 0 : Rnd();
        }
        // Model buckets: a few models with name hashes.
        memset((void*)0x6acd78, 0, sizeof savedBuckets);
        const char* names[4] = { "Loco", "Steam Engine", "Boxcar", "VeryLongName_____________________________________________________________________________________________________________________________x" };
        for (int k = 0; k < 4; ++k) {
            *(uint32_t*)(models[k] + 0x90) = rt3::Crc32String(names[k], 1);
            uint32_t b = 0x6acd78 + (Rnd() % 0xd5) * 0xc;
            *(uint8_t**)models[k] = *(uint8_t**)b;
            *(uint8_t**)b = models[k];
        }
        // Message with a checksum list.
        static uint32_t firstWord;
        firstWord = Rnd() % 5;
        *(uint32_t**)(msg + 8) = Rnd() % 4 ? &firstWord : nullptr;
        if (!*(uint32_t**)(msg + 8)) *(uint32_t**)(msg + 8) = &firstWord, firstWord = 0;
        *(int32_t*)(msg + 0xc) = (int32_t)(Rnd() % 3) - 1;
        *(uint8_t**)(msg + 4) = msgHdr;
        nodes[0] = Rnd() % 2 ? &nodes[1] : nullptr;
        nodes[1] = Rnd() % 2 ? &nodes[2] : nullptr;
        nodes[2] = nullptr;
        g_raList = Rnd() % 4 ? (void*)nodes : nullptr;
        char nameBuf[64];
        static const char* lookups[] = { "", "Str1", "str4", "NoSuch", "Loco", "steam engine", "boxcar" };
        strcpy(nameBuf, lookups[Rnd() % 7]);
        static const char* splits[] = { "a+b", "+abc", "None", "plain", "x+y+z", "" };
        const char* split = splits[Rnd() % 6];
        uint32_t mask = 1u << (Rnd() % 16);
        int32_t clr = (int32_t)(Rnd() % 2);
        uint8_t* obj = Rnd() % 4 ? objs[Rnd() % 8] : nullptr;
        int32_t cx = (int32_t)(Rnd() % 12), cy = (int32_t)(Rnd() % 12);
        uint32_t edxIn = Rnd();
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t tableCopy[sizeof table];
            memcpy(tableCopy, table, sizeof table);
            msgHdr[0xb] = 0;
            char dst[64], dst2[64], src[64];
            strcpy(src, split);
            memset(dst, 1, sizeof dst);
            memset(dst2, 1, sizeof dst2);
            logs[side] = t.Run([&] {
                uint32_t r[3];
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::NamedValue_Find : 0x45ec70, 0, 0, 0, (uint32_t)(uintptr_t)nameBuf, 0, 0, 0, r);
                Logf("named %d", r[0]);
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::Models_FindByName : 0x45ede0, (uint32_t)(uintptr_t)nameBuf, 0, 0, 0, 0, 0, 0, r);
                Logf("model %p", (void*)r[0]);
                hook::Global<uint8_t*>(0x6acd50) = tableCopy;
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::ObjFlags_Set : 0x459bc0, mask, (uint32_t)(uintptr_t)(obj ? obj : objs[0]), 0, 0, 0, (uint32_t)clr, 1, r);
                Logf("flags %x", r[0] >= (uint32_t)(uintptr_t)tableCopy && r[0] < (uint32_t)(uintptr_t)tableCopy + 0x100 ? r[0] - (uint32_t)(uintptr_t)tableCopy : r[0]);
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::Map_BlockHasFlag2 : 0x414c50, (uint32_t)cx, 0x55, (uint32_t)cy, 0, 0, 0, 0, r);
                Logf("block %d ecx %x", r[0], r[1]);
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::Msg_MarkSeen : 0x472ea0, 0, (uint32_t)(uintptr_t)msg, (uint32_t)(uintptr_t)other, 0, 0, 0, 0, r);
                Logf("seen %d %d", r[0], msgHdr[0xb]);
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::EntryWalk_Begin : 0x45a5f0, 0x777, (uint32_t)(uintptr_t)tableCopy, edxIn, 0, 0, 0, 0, r);
                Logf("begin %d %x %x %x %x", r[0], hook::Global<uint32_t>(0x6acd44), hook::Global<uint32_t>(0x6acd48), hook::Global<uint32_t>(0x6acd4c) - (uint32_t)(uintptr_t)tableCopy,
                     hook::Global<uint32_t>(0x6acd54));
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::Text_SplitPlus : 0x45ec00, (uint32_t)(uintptr_t)dst, 0, 0, (uint32_t)(uintptr_t)dst2, (uint32_t)(uintptr_t)src, 0, 0, r);
                Logf("split [%s] [%s] [%s]", dst, dst2, src);
                RaCall(side ? (uint32_t)(uintptr_t)&rt3::EntryWalk_Find : 0x459c30, 0, 0, (uint32_t)(uintptr_t)tableCopy, (uint32_t)(uintptr_t)obj, 0, 0, 0, r);
                Logf("find %d edx %d %x %x %x", r[0], r[2] == (uint32_t)(uintptr_t)tableCopy, hook::Global<uint32_t>(0x6acd44),
                     hook::Global<uint32_t>(0x6acd48) ? hook::Global<uint32_t>(0x6acd48) - (uint32_t)(uintptr_t)tableCopy : 0,
                     hook::Global<uint32_t>(0x6acd4c) ? hook::Global<uint32_t>(0x6acd4c) - (uint32_t)(uintptr_t)tableCopy : 0);
            });
            for (int k = 0; k < (int)sizeof table; ++k) logs[side] += " " + std::to_string(tableCopy[k]);
        }
        CompareLogs(logs[0], logs[1], "register-arg functions", it);
    }
    memcpy((void*)0x6acd78, savedBuckets, sizeof savedBuckets);
}

namespace rt3 {
void Profiles_IndexOf();
void Companies_Clash();
void Train_StepsToNode();
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
}

namespace {
int32_t g_r2Count;
uint8_t g_r2Recs[16][0x400];
int32_t __fastcall FakeR2Count(Collection* c) { return g_r2Count; }
uint32_t __fastcall FakeR2IdAt(Collection* c, int, uint32_t pos, int list) { return pos * 5 + 3; }
void* __fastcall FakeR2Get(Collection* c, int, uint32_t id) {
    Logf("get %u", id);
    return g_r2Recs[id % 16];
}
int32_t R2Call(uint32_t fn, uint32_t a, uint32_t b, uint32_t s0, uint32_t s1, int nStack) {
    int32_t r;
    __asm {
        push ebx
        mov eax, nStack
        test eax, eax
        jz nostack
        push s1
        push s0
    nostack:
        mov eax, a
        mov ebx, b
        call fn
        mov r, eax
        pop ebx
    }
    return r;
}
} // namespace

DIFFTEST(misc_RegisterArgLookups) {
    UiTrace t;
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeR2Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeR2IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeR2Get);
    static const char* names[] = { "Alpha", "beta", "Gamma", "delta" };
    static uint8_t req[0x40], train[0x400];
    for (int it = 0; it < 4000; ++it) {
        g_r2Count = (int32_t)(Rnd() % 10) - 1;
        for (int k = 0; k < 16; ++k) {
            strcpy((char*)g_r2Recs[k] + 4, names[Rnd() % 4]);
            strcpy((char*)g_r2Recs[k] + 8 + 0x20, "");
            strcpy((char*)g_r2Recs[k] + 8, names[Rnd() % 4]);
            *(int32_t*)(g_r2Recs[k] + 0x37) = (int32_t)(Rnd() % 20);
            *(int32_t*)(g_r2Recs[k] + 0x202) = (int32_t)(Rnd() % 16);
            *(int32_t*)(g_r2Recs[k] + 0x206) = Rnd() % 6 == 0 ? -1 : (int32_t)(Rnd() % 16);
            *(int32_t*)(g_r2Recs[k] + 0x20a) = Rnd() % 6 == 0 ? -1 : (int32_t)(Rnd() % 16);
        }
        // Companies_Clash reads name +4 and player +0x37; Profiles_IndexOf name +8 (overlapping, so copy last).
        char look[16];
        strcpy(look, names[Rnd() % 4]);
        if (Rnd() % 3 == 0) strcpy(look, "nobody");
        *(int32_t*)(req + 4) = (int32_t)(Rnd() % 30);
        strcpy((char*)req + 0x10, names[Rnd() % 4]);
        *(uint32_t*)(train + 0x356) = Rnd() % 16;
        *(int32_t*)(train + 0x35a) = (int32_t)(Rnd() % 2);
        int32_t target = (int32_t)(Rnd() % 16), max = (int32_t)(Rnd() % 10) - 1;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            logs[side] = t.Run([&] {
                Logf("profile %d", R2Call(side ? (uint32_t)(uintptr_t)&rt3::Profiles_IndexOf : 0x46d980, 0, (uint32_t)(uintptr_t)look, 0, 0, 0));
                Logf("clash %d", R2Call(side ? (uint32_t)(uintptr_t)&rt3::Companies_Clash : 0x46ed30, 0, (uint32_t)(uintptr_t)req, 0, 0, 0));
                Logf("steps %d", R2Call(side ? (uint32_t)(uintptr_t)&rt3::Train_StepsToNode : 0x4a78d0, (uint32_t)(uintptr_t)train, 0, (uint32_t)target, (uint32_t)max, 1));
            });
        }
        CompareLogs(logs[0], logs[1], "register-arg lookups", it);
    }
}

namespace rt3 {
void Net_ApplyCash();
void __fastcall Car_MoveToSegment(uint8_t* car, int, int32_t seg, int32_t dir);
void __fastcall IndustryTypes_MarkAvailable(Collection* t);
float* __fastcall Camera_WorldCross(uint8_t* cam, int, float* out, float vx, float vy, float vz, float wx, float wy, float wz, float* len);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
void __fastcall Obj_UnlinkFromNode(void* o);
void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side);
void __fastcall Track_AddWear(uint8_t* t);
void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v);
int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
void __fastcall CargoTypes_MarkAvailable(Collection* c);
}

namespace {
int32_t g_b3Valid, g_b3Flag, g_b3Count;
uint8_t g_b3Recs[8][0x800];
int32_t __fastcall FakeB3Valid(Collection* c, int, uint32_t id) {
    Logf("valid %u", id);
    return g_b3Valid;
}
void* __fastcall FakeB3Get(Collection* c, int, uint32_t id) {
    Logf("get %u", id);
    return g_b3Recs[id % 8];
}
int32_t __fastcall FakeB3Flag(void* c) { return g_b3Flag; }
void __fastcall RecB3Stat(void* c, int, int32_t stat, float a) { Logf("stat %d %08x", stat, B(a)); }
void __fastcall RecB3Unlink(void* o) { Logf("unlink"); }
void __fastcall RecB3Link(uint8_t* car, int, int32_t id, int32_t side) { Logf("link %d %d", id, side); }
void __fastcall RecB3Wear(uint8_t* t) { Logf("wear %d", (int)((t - g_b3Recs[0]) / 0x800)); }
void __fastcall RecB3Block(uint8_t* o, int, uint8_t v) { Logf("block %d", v); }
int32_t __fastcall FakeB3Named(void* w, int, const char* name) {
    Logf("named [%s]", name);
    return (int32_t)strlen(name) % 3;
}
void __fastcall RecB3Cargo(Collection* c) { Logf("cargo %p", c); }
int32_t __fastcall FakeB3Count(Collection* c) { return g_b3Count; }
uint32_t __fastcall FakeB3IdAt(Collection* c, int, uint32_t pos, int list) { return pos; }
uint32_t CallCash(uint32_t fn, uint8_t* m, uint32_t ecxIn) {
    uint32_t rc;
    __asm {
        mov eax, m
        mov ecx, ecxIn
        call fn
        mov rc, ecx
    }
    return rc;
}
} // namespace

DIFFTEST(misc_CarSegmentsCashCamera) {
    SavedGlobals sg({ 0x6cec78, 0x62ba8c });
    UiTrace t;
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeB3Valid);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB3Get);
    t.Add(0x425b90, (void*)&rt3::Company_OwnerFlag1e1, (void*)&FakeB3Flag);
    t.Add(0x42a040, (void*)&rt3::Company_AddStat, (void*)&RecB3Stat);
    t.Add(0x41afc0, (void*)&rt3::Obj_UnlinkFromNode, (void*)&RecB3Unlink);
    t.Add(0x41aea0, (void*)&rt3::Car_LinkToNode, (void*)&RecB3Link);
    t.Add(0x48a590, (void*)&rt3::Track_AddWear, (void*)&RecB3Wear);
    t.Add(0x48e310, (void*)&rt3::Track_SetBlocked, (void*)&RecB3Block);
    t.Add(0x434ea0, (void*)&rt3::World_LookupNamed1, (void*)&FakeB3Named);
    t.Add(0x41eac0, (void*)&rt3::CargoTypes_MarkAvailable, (void*)&RecB3Cargo);
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB3Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB3IdAt);
    static uint8_t world[0x40], cargo[0x40], car[0x400], cam[0x400], types[0x40];
    static const char* names[] = { "Steel Mill", "Farm", "Lumber", "Oil" };
    for (int it = 0; it < 3000; ++it) {
        g_b3Valid = Rnd() % 4 != 0;
        g_b3Flag = Rnd() % 3 != 0;
        g_b3Count = (int32_t)(Rnd() % 8) - 1;
        hook::Global<uint8_t*>(0x6cec78) = Rnd() % 6 ? world : nullptr;
        hook::Global<uint8_t*>(0x62ba8c) = Rnd() % 4 ? cargo : nullptr;
        for (int k = 0; k < 8; ++k) {
            g_b3Recs[k][0xba] = Rnd() % 4 == 0;
            g_b3Recs[k][0xbb] = Rnd() % 4 == 0;
            g_b3Recs[k][0x32] = (uint8_t)(Rnd() % 3);
            strcpy((char*)g_b3Recs[k] + 4, names[Rnd() % 4]);
        }
        uint8_t msg[8];
        float amount = Rnd() % 9 == 0 ? std::numeric_limits<float>::quiet_NaN() : RndF(-5e6f, 5e6f);
        memcpy(msg, &amount, 4);
        msg[4] = (uint8_t)(Rnd() % 8);
        int32_t seg = Rnd() % 4 == 0 ? -1 : (int32_t)(Rnd() % 8), dir = (int32_t)(Rnd() % 2);
        int32_t curSeg = Rnd() % 3 == 0 ? seg : (int32_t)(Rnd() % 8);
        for (int k = 0; k < 16; ++k) *(float*)(cam + 0x18e + k * 4) = k % 5 == 0 ? RndF(0.5f, 2.0f) : RndF(-1.0f, 1.0f);
        float v[6];
        for (float& f : v) f = RndF(-10.0f, 10.0f);
        uint32_t ecxIn = Rnd();
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(car + 0x356) = curSeg;
            *(int32_t*)(car + 0x35a) = 1 - dir;
            *(int32_t*)(car + 0x35e) = 7;
            for (int k = 0; k < 8; ++k) *(int32_t*)(g_b3Recs[k] + 0x7ac) = 9;
            float out[3] = {}, len = 0;
            logs[side] = t.Run([&] {
                Logf("cash ecx %08x", CallCash(side ? (uint32_t)(uintptr_t)&rt3::Net_ApplyCash : 0x46f870, msg, ecxIn));
                if (side) rt3::Car_MoveToSegment(car, 0, seg, dir);
                else ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x41c700)(car, 0, seg, dir);
                if (side) rt3::IndustryTypes_MarkAvailable((Collection*)types);
                else ((void(__fastcall*)(Collection*))0x412c10)((Collection*)types);
                float* r = side ? rt3::Camera_WorldCross(cam, 0, out, v[0], v[1], v[2], v[3], v[4], v[5], &len)
                                : ((float*(__fastcall*)(uint8_t*, int, float*, float, float, float, float, float, float, float*))0x458140)(cam, 0, out, v[0], v[1], v[2],
                                                                                                                                    v[3], v[4], v[5], &len);
                Logf("cam %d %08x %08x %08x %08x", r == out, B(out[0]), B(out[1]), B(out[2]), B(len));
            });
            char tail[64];
            sprintf(tail, " %d %d", *(int32_t*)(car + 0x356), *(int32_t*)(car + 0x35e));
            logs[side] += tail;
            for (int k = 0; k < 8; ++k) logs[side] += " " + std::to_string(*(int32_t*)(g_b3Recs[k] + 0x7ac));
        }
        CompareLogs(logs[0], logs[1], "car segments / cash / camera", it);
    }
}

namespace rt3 {
void TeamName_Kind();
void Models_Unlink();
int32_t __stdcall Sounds_Has(uint8_t* rec, int32_t touch);
int32_t __fastcall Music_ReadParam(uint8_t* m);
int32_t ClockMs();
}

namespace {
int32_t g_b4Clock, g_b4Mode;
int32_t FakeB4Clock() { return ++g_b4Clock; }
const char* __fastcall FakeB4String(int32_t id) {
    switch (id) {
    case 0xdf3: return "Union";
    case 0xdf4: return "Confed";
    case 0xdf5: return "Neutral";
    }
    return "?";
}
void __cdecl RecB4Delete(void* p) { Logf("delete %p", p); }
HRESULT __stdcall FakeB4Get(void* o, const void* g, int32_t z, void* buf, uint32_t* size, uint32_t* got, int32_t z2) {
    Logf("get %p %d %p %u %u %d", g, z, buf, *size, *got, z2);
    switch (g_b4Mode) {
    case 0: return (HRESULT)0x80004005;
    case 1: *size = 40; *got = 0; return (HRESULT)0x80158100;
    case 2: *size = 40; *got = buf ? 1 : 0; return buf ? 0 : (HRESULT)0x80158100;
    case 3: *size = 40; *got = buf ? 2 : 0; return buf ? 0 : (HRESULT)0x80158100;
    }
    *size = 8;
    return buf ? -7 : (HRESULT)0x80158100;
}
int32_t B4CallEsi(uint32_t fn, const char* s) {
    int32_t r;
    __asm {
        push esi
        mov esi, s
        call fn
        pop esi
        mov r, eax
    }
    return r;
}
void B4CallUnlink(uint32_t fn, uint8_t* m, int32_t recycle) {
    __asm {
        push edi
        mov eax, m
        mov edi, recycle
        call fn
        pop edi
    }
}
} // namespace

DIFFTEST(misc_TeamModelsSoundsMusic) {
    SavedGlobals sg({ 0x6acd6c, 0x6d4044, 0x6d411c });
    static uint8_t savedBuckets[0x6ad96c + 0xc - 0x6acd78];
    memcpy(savedBuckets, (void*)0x6acd78, sizeof savedBuckets);
    UiTrace t;
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB4String);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeB4Clock);
    t.Add(0x53b080, nullptr, (void*)&RecB4Delete);
    static const char* names[] = { "union", "UNI army", "uni", "confed", "Neutral", "Other", "", "CONFEDERATES" };
    static uint8_t models[6][0x40], list[12], pool[0x14], sounds[4][0x120], rec[0x120], obj[8], music[0x20];
    static void* vt[8];
    vt[0x10 / 4] = (void*)&FakeB4Get;
    *(void***)obj = vt;
    for (int it = 0; it < 3000; ++it) {
        char name[32];
        strcpy(name, names[Rnd() % 8]);
        int nb = (int)(Rnd() % 4);
        int target = (int)(Rnd() % 6);
        int32_t recycle = (int32_t)(Rnd() % 3);
        bool poolOn = Rnd() % 2 != 0;
        int nSounds = (int)(Rnd() % 5);
        for (int k = 0; k < 4; ++k) {
            *(int32_t*)(sounds[k] + 0x104) = (int32_t)(Rnd() % 3);
            strcpy((char*)sounds[k] + 0x84, Rnd() % 2 ? "engine" : "horn");
        }
        *(int32_t*)(rec + 0x104) = (int32_t)(Rnd() % 3);
        strcpy((char*)rec + 0x84, Rnd() % 2 ? "engine" : "whistle");
        int32_t touch = (int32_t)(Rnd() % 2);
        g_b4Mode = (int32_t)(Rnd() % 5);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset((void*)0x6acd78, 0, sizeof savedBuckets);
            // Bucket 2 holds models 0..nb (and 5 is in bucket 3 alone).
            for (int k = 0; k < 6; ++k) {
                memset(models[k], 0, 0x40);
                *(int32_t*)(models[k] + 0xc) = k == 5 ? 3 : 2;
            }
            uint8_t* b = (uint8_t*)0x6acd78 + 2 * 0xc;
            for (int k = 0; k <= nb; ++k) *(uint8_t**)models[k] = k < nb ? models[k + 1] : nullptr;
            *(uint8_t**)b = models[0];
            *(uint8_t**)(b + 4) = models[nb];
            *(int32_t*)(b + 8) = nb + 1;
            *(uint8_t**)((uint8_t*)0x6acd78 + 3 * 0xc) = models[5];
            *(uint8_t**)((uint8_t*)0x6acd78 + 3 * 0xc + 4) = models[5];
            *(int32_t*)((uint8_t*)0x6acd78 + 3 * 0xc + 8) = 1;
            memset(list, 0, sizeof list);
            *(uint8_t**)pool = list;
            *(uint8_t**)(pool + 4) = models[0];
            *(uint8_t**)(pool + 8) = models[3];
            *(int32_t*)(pool + 0xc) = poolOn;
            hook::Global<uint8_t*>(0x6acd6c) = pool;
            for (int k = 0; k < 4; ++k) {
                *(uint8_t**)sounds[k] = k + 1 < nSounds ? sounds[k + 1] : nullptr;
                *(int32_t*)(sounds[k] + 0x118) = 0;
            }
            static uint8_t* soundHead;
            soundHead = nSounds ? sounds[0] : nullptr;
            hook::Global<uint8_t**>(0x6d4044) = &soundHead;
            hook::Global<int32_t>(0x6d411c) = 5;
            memset(music, 0, sizeof music);
            *(uint8_t**)(music + 8) = obj;
            g_b4Clock = 100;
            char nm[32];
            strcpy(nm, name);
            logs[side] = t.Run([&] {
                Logf("team %d [%s]", B4CallEsi(side ? (uint32_t)(uintptr_t)&rt3::TeamName_Kind : 0x45ecd0, nm), nm);
                B4CallUnlink(side ? (uint32_t)(uintptr_t)&rt3::Models_Unlink : 0x45ee40, models[target], recycle);
                Logf("sound %d", side ? rt3::Sounds_Has(rec, touch) : ((int32_t(__stdcall*)(uint8_t*, int32_t))0x521b60)(rec, touch));
                Logf("music %d", side ? rt3::Music_ReadParam(music) : ((int32_t(__fastcall*)(uint8_t*))0x557a30)(music));
            });
            char tail[200];
            sprintf(tail, " %d %d %d %d", *(int32_t*)(b + 8), *(int32_t*)(list + 8), hook::Global<int32_t>(0x6d411c), *(int32_t*)(music + 0x14) != 0);
            logs[side] += tail;
            for (int k = 0; k < 6; ++k) {
                uint8_t* nx = *(uint8_t**)models[k];
                logs[side] += " " + std::to_string(nx ? (int)((nx - models[0]) / 0x40) : -1);
            }
            for (int k = 0; k < 4; ++k) logs[side] += " " + std::to_string(*(int32_t*)(sounds[k] + 0x118));
            logs[side] += " " + std::to_string((*(uint8_t**)b - models[0]) / 0x40) + " " + std::to_string((*(uint8_t**)(b + 4) - models[0]) / 0x40);
            logs[side] += *(uint8_t**)list ? " L" + std::to_string((*(uint8_t**)list - models[0]) / 0x40) : " L-";
        }
        CompareLogs(logs[0], logs[1], "team / models / sounds / music", it);
    }
    memcpy((void*)0x6acd78, savedBuckets, sizeof savedBuckets);
}

namespace rt3 {
void DirtyCells_Add();
void Attach_SumVectors();
int32_t __fastcall Records_Dispatch(uint8_t* r);
void Attach_SetValue();
}

namespace {
int32_t __fastcall FakeB5Handler(uint8_t* r) {
    Logf("handler %d", *(int32_t*)(r + 8));
    return 55;
}
void B5Dirty(uint32_t fn, int32_t a, int32_t x, int32_t y, uint32_t out[2]) {
    uint32_t c, d;
    __asm {
        mov eax, a
        mov ecx, x
        mov edx, y
        call fn
        mov c, ecx
        mov d, edx
    }
    out[0] = c;
    out[1] = d;
}
void B5Sum(uint32_t fn, float* out, int32_t mode, int32_t obj) {
    __asm {
        push obj
        mov eax, out
        mov ecx, mode
        call fn
    }
}
int32_t B5Set(uint32_t fn, int32_t value, uint8_t* obj, int32_t v2) {
    int32_t r;
    __asm {
        push v2
        push obj
        mov eax, value
        call fn
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(misc_WalkTablesAndDirtyCells) {
    SavedGlobals sg({ 0x62b7a4, 0x62b7a8, 0x62b7ac, 0x62b7b0, 0x62b7b4, 0x6acd4c, 0x6acd50, 0x6cec74, 0x6d3b4c });
    static uint8_t savedCells[0x490], savedBuckets[0x6ad96c + 0xc - 0x6acd78];
    memcpy(savedCells, (void*)0x62b318, sizeof savedCells);
    memcpy(savedBuckets, (void*)0x6acd78, sizeof savedBuckets);
    UiTrace t;
    static uint8_t table[0x10 + 8 * 0x168], hdr[0x10], opt[0x300], handlers[4][0xa8], rec[0x30], obj[0xa8];
    for (int it = 0; it < 4000; ++it) {
        // Dirty cells: a short list.
        int n0 = (int)(Rnd() % 5);
        uint32_t cells[6];
        for (int k = 0; k < n0; ++k) cells[k] = (Rnd() % 6) << 16 | (Rnd() % 6);
        int32_t a = (int32_t)(Rnd() % 2), x = (int32_t)(Rnd() % 6), y = (int32_t)(Rnd() % 6);
        // Walk table entries.
        int ne = (int)(Rnd() % 8);
        uint8_t entries[8 * 0x168];
        for (int k = 0; k < 8; ++k) {
            uint8_t* e = entries + k * 0x168;
            memset(e, 0, 0x168);
            *(int32_t*)e = (int32_t)(Rnd() % 4) + 1;
            *(uint32_t*)(e + 0x8c) = Rnd() % 4 == 0 ? 0x400 : (Rnd() % 3 == 0 ? 0x10 : 0) | (Rnd() % 2 ? 2 : 0);
            for (int c = 0; c < 3; ++c) {
                *(int32_t*)(e + 0x34 + c * 4) = (int32_t)(Rnd() % 200) - 100;
                *(int32_t*)(e + 0x70 + c * 4) = (int32_t)(Rnd() % 200) - 100;
                *(float*)(e + 4 + c * 4) = RndF(-3.0f, 3.0f);
                *(float*)(e + 0x40 + c * 4) = RndF(-3.0f, 3.0f);
            }
        }
        int32_t mode = (int32_t)(Rnd() % 2), who = (int32_t)(Rnd() % 4) + 1;
        // Handlers in bucket of model id.
        int32_t id = Rnd() % 5 == 0 ? 0xff : (int32_t)(0x20 + Rnd() % 0x12);
        int nh = (int)(Rnd() % 4);
        for (int k = 0; k < 4; ++k) {
            memset(handlers[k], 0, sizeof handlers[k]);
            *(uint8_t**)handlers[k] = k + 1 < nh ? handlers[k + 1] : nullptr;
            *(uint32_t*)(handlers[k] + 0x98) = Rnd() % 4;
            *(uint32_t*)(handlers[k] + 0x94) = Rnd() % 8;
            *(int32_t*)(handlers[k] + 4) = Rnd() % 3 == 0;
            *(int32_t*)(handlers[k] + 8) = Rnd() % 3 == 0;
            *(void**)(handlers[k] + 0xa0) = Rnd() % 5 ? (void*)&FakeB5Handler : nullptr;
        }
        *(int32_t*)rec = (int32_t)(Rnd() % 5);
        *(int32_t*)(rec + 8) = id;
        *(uint32_t*)(rec + 0x20) = Rnd() % 32;
        *(int32_t*)(opt + 0x28f) = Rnd() % 2;
        int32_t g4c = Rnd() % 2;
        for (int k = 0; k < 4; ++k) *(int32_t*)(obj + 0x98 + k * 4) = Rnd() % 4 == 0 ? 0 : (int32_t)(Rnd() % 4) + 1;
        obj[0x8c] = (uint8_t)(Rnd() % 2 ? 0x10 : 0);
        int32_t value = (int32_t)Rnd(), v2 = (int32_t)(Rnd() % 100);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < n0; ++k) ((uint32_t*)0x62b318)[k] = cells[k];
            ((uint32_t*)0x62b318)[n0] = 0xffffffff;
            hook::Global<int32_t>(0x62b7a4) = n0;
            hook::Global<int32_t>(0x62b7a8) = 3;
            hook::Global<int32_t>(0x62b7ac) = 0;
            hook::Global<int32_t>(0x62b7b0) = 2;
            hook::Global<int32_t>(0x62b7b4) = 0;
            memcpy(table + 0x10, entries, sizeof entries);
            *(int32_t*)(hdr + 8) = ne;
            hook::Global<uint8_t*>(0x6acd50) = hdr;
            hook::Global<uint8_t*>(0x6acd4c) = table + 0x10;
            memset((void*)0x6acd78, 0, sizeof savedBuckets);
            if (id != 0xff && nh) *(uint8_t**)(0x6acd78 + id * 0xc) = handlers[0];
            hook::Global<uint8_t*>(0x6cec74) = opt;
            hook::Global<int32_t>(0x6d3b4c) = g4c;
            logs[side] = t.Run([&] {
                uint32_t cd[2];
                B5Dirty(side ? (uint32_t)(uintptr_t)&rt3::DirtyCells_Add : 0x413df0, a, x, y, cd);
                Logf("dirty ecx %d edx %d", cd[0], cd[1]);
                float out[3];
                B5Sum(side ? (uint32_t)(uintptr_t)&rt3::Attach_SumVectors : 0x45a700, out, mode, who);
                Logf("sum %08x %08x %08x", B(out[0]), B(out[1]), B(out[2]));
                Logf("dispatch %d", side ? rt3::Records_Dispatch(rec) : ((int32_t(__fastcall*)(uint8_t*))0x45f140)(rec));
                Logf("set %d", B5Set(side ? (uint32_t)(uintptr_t)&rt3::Attach_SetValue : 0x459d00, value, obj, v2));
            });
            char tail[128];
            sprintf(tail, " %d %d %d %d %d", hook::Global<int32_t>(0x62b7a4), hook::Global<int32_t>(0x62b7a8), hook::Global<int32_t>(0x62b7ac),
                    hook::Global<int32_t>(0x62b7b0), hook::Global<int32_t>(0x62b7b4));
            logs[side] += tail;
            for (int k = 0; k <= n0 + 1; ++k) logs[side] += " " + std::to_string(((uint32_t*)0x62b318)[k]);
            for (int k = 0; k < 8; ++k) {
                uint8_t* e = table + 0x10 + k * 0x168;
                logs[side] += " " + std::to_string(*(uint32_t*)(e + 0x8c)) + "/" + std::to_string(*(int32_t*)(e + 0x80)) + "/" + std::to_string(*(int32_t*)(e + 0x94));
            }
        }
        CompareLogs(logs[0], logs[1], "walk tables / dirty cells", it);
    }
    memcpy((void*)0x62b318, savedCells, sizeof savedCells);
    memcpy((void*)0x6acd78, savedBuckets, sizeof savedBuckets);
}

namespace rt3 {
void Map_RectHasFlag1();
void Map_NearestFlag2();
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
}

namespace {
uint32_t g_b6Seed;
int32_t __fastcall FakeB6Cell(void* m, int, int32_t x, int32_t y, int32_t which) {
    Logf("cell %d %d %d", x, y, which);
    return (uint32_t)(x * 7919 + y * 104729 + which * 31 + (int32_t)g_b6Seed) % 37 == 0;
}
double __stdcall FakeB6Dist(float x1, float y1, float x2, float y2) {
    Logf("dist %08x %08x %08x %08x", B(x1), B(y1), B(x2), B(y2));
    return sqrt((double)x2 * x2 + (double)y2 * y2) * (g_b6Seed % 7 == 0 ? -1.0 : 1.0);
}
int32_t B6Rect(uint32_t fn, const int32_t* r) {
    int32_t v;
    __asm {
        mov eax, r
        call fn
        mov v, eax
    }
    return v;
}
int32_t B6Near(uint32_t fn, float* out, int32_t cx, int32_t cy) {
    int32_t v;
    __asm {
        push ebx
        push cy
        push cx
        mov ebx, out
        call fn
        pop ebx
        mov v, eax
    }
    return v;
}
} // namespace

DIFFTEST(map_BlockFlagSearches) {
    SavedGlobals sg({ 0x62c120 });
    UiTrace t;
    t.Add(0x448af0, (void*)&rt3::Map_CellFlag, (void*)&FakeB6Cell);
    t.Add(0x51db80, (void*)&rt3::Distance2D, (void*)&FakeB6Dist);
    static uint8_t map[0x2200];
    hook::Global<uint8_t*>(0x62c120) = map;
    for (int it = 0; it < 3000; ++it) {
        g_b6Seed = Rnd();
        *(int32_t*)(map + 0x2155) = 2 + (int32_t)(Rnd() % 12);
        *(int32_t*)(map + 0x2159) = 2 + (int32_t)(Rnd() % 12);
        *(int32_t*)(map + 0x215d) = *(int32_t*)(map + 0x2155) * 4;
        *(int32_t*)(map + 0x2161) = *(int32_t*)(map + 0x2159) * 4;
        int32_t r[4] = { (int32_t)(Rnd() % 12) - 2, 0, (int32_t)(Rnd() % 12) - 2, 0 };
        r[1] = r[0] + (int32_t)(Rnd() % 4) - 1;
        r[3] = r[2] + (int32_t)(Rnd() % 4) - 1;
        int32_t cx = (int32_t)(Rnd() % 14) - 1, cy = (int32_t)(Rnd() % 14) - 1;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            float out = -1.0f;
            logs[side] = t.Run([&] {
                Logf("rect %d", B6Rect(side ? (uint32_t)(uintptr_t)&rt3::Map_RectHasFlag1 : 0x415900, r));
                Logf("near %d", B6Near(side ? (uint32_t)(uintptr_t)&rt3::Map_NearestFlag2 : 0x414cd0, &out, cx, cy));
            });
            char tail[32];
            sprintf(tail, " %08x", B(out));
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "block flag searches", it);
    }
}

namespace rt3 {
void DirtyCells_Apply();
void Tree_DeleteNode();
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void* __fastcall Engine_Renderer(void* e);
}

namespace {
uint8_t g_b7Scene[0x4000];
void __fastcall RecB7Fine(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update) { Logf("fine %d %d %d %d %d", x, y, on, which, update); }
void __fastcall RecB7Flag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) { Logf("flag %d %d %d", x, y, v); }
void* __fastcall FakeB7Renderer(void* e) { return g_b7Scene; }
uint32_t B7Apply(uint32_t fn, uint8_t* list) {
    uint32_t c;
    __asm {
        mov eax, list
        call fn
        mov c, ecx
    }
    return c;
}
void B7Delete(uint32_t fn, int32_t p, uint8_t* state) {
    __asm {
        push state
        mov edx, p
        call fn
    }
}
} // namespace

DIFFTEST(misc_DirtyCellsAndTree) {
    SavedGlobals sg({ 0x62ba78, 0x62c120, 0x6d4024 });
    static uint8_t savedCells[0x490];
    memcpy(savedCells, (void*)0x62b318, sizeof savedCells);
    UiTrace t;
    t.Add(0x4497a0, (void*)&rt3::Map_SetFineBit, (void*)&RecB7Fine);
    t.Add(0x533fe0, (void*)&rt3::Terrain_SetFlag, (void*)&RecB7Flag);
    t.Add(0x51f070, (void*)&rt3::Engine_Renderer, (void*)&FakeB7Renderer);
    static uint8_t map[0x2200], blocks[0x400], list[0x80], eng[16], st[0x40];
    static int32_t parent[40], left[40], right[40], p0[40], l0[40], r0[40];
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6d4024) = eng;
    *(uint8_t**)(map + 0x2131) = blocks;
    *(int32_t*)(map + 0x2155) = 16;
    for (int it = 0; it < 3000; ++it) {
        bool own = Rnd() % 2 != 0;
        int n = (int)(Rnd() % 8);
        uint32_t cells[8];
        for (int k = 0; k < n; ++k) cells[k] = (Rnd() % 2 ? 0x80000000u : 0) | (Rnd() % 32) << 16 | (Rnd() % 32);
        // A random binary tree over nodes 0..31, nil 32.
        const int nil = 32;
        for (int k = 0; k <= nil; ++k) p0[k] = l0[k] = r0[k] = nil;
        int root = 0;
        p0[root] = 33; // the tree hangs off a header node 33 (its right child)
        r0[33] = root;
        for (int k = 1; k < 32; ++k) {
            if (Rnd() % 4 == 0) continue;
            int at = root;
            for (;;) {
                if (Rnd() % 2) {
                    if (l0[at] == nil) { l0[at] = k; break; }
                    at = l0[at];
                } else {
                    if (r0[at] == nil) { r0[at] = k; break; }
                    at = r0[at];
                }
            }
            p0[k] = at;
        }
        int victim = (int)(Rnd() % 32);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            uint32_t* dst = own ? (uint32_t*)(list + 0x10) : (uint32_t*)0x62b318;
            for (int k = 0; k < n; ++k) dst[k] = cells[k];
            dst[n] = 0xffffffff;
            memset(blocks, 0, sizeof blocks);
            g_b7Scene[0x374a] = 0;
            hook::Global<int32_t>(0x62ba78) = 4;
            memcpy(parent, p0, sizeof parent);
            memcpy(left, l0, sizeof left);
            memcpy(right, r0, sizeof right);
            *(int32_t**)(st + 0x24) = parent;
            *(int32_t**)(st + 0x28) = left;
            *(int32_t**)(st + 0x2c) = right;
            *(int32_t*)(st + 8) = nil;
            logs[side] = t.Run([&] {
                uint32_t c = B7Apply(side ? (uint32_t)(uintptr_t)&rt3::DirtyCells_Apply : 0x415820, own ? list : nullptr);
                Logf("apply ecx %d", (int)(((uint32_t*)(uintptr_t)c) - dst));
                B7Delete(side ? (uint32_t)(uintptr_t)&rt3::Tree_DeleteNode : 0x56bb10, victim, st);
            });
            char tail[64];
            sprintf(tail, " %d %d", hook::Global<int32_t>(0x62ba78), g_b7Scene[0x374a]);
            logs[side] += tail;
            for (int k = 0; k < 0x400; ++k)
                if (blocks[k]) logs[side] += " b" + std::to_string(k);
            for (int k = 0; k < 34; ++k) logs[side] += " " + std::to_string(parent[k]) + "/" + std::to_string(left[k]) + "/" + std::to_string(right[k]);
        }
        CompareLogs(logs[0], logs[1], "dirty cells / tree", it);
    }
    memcpy((void*)0x62b318, savedCells, sizeof savedCells);
}

namespace rt3 {
void Terrain_RestoreRect();
void Footprint_Stamp();
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);
}

namespace {
uint32_t g_b8Seed;
uint8_t g_b8Scene[0x4000];
void __fastcall RecB8Flag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) { Logf("flag %d %d %d", x, y, v); }
void __fastcall RecB8Byte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) { Logf("byte %d %d %d", x, y, v); }
void __fastcall RecB8Count(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) { Logf("count %d %d %d", x, y, v); }
void __fastcall RecB8Points(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count) { Logf("points %d %d %d %d", x, z, kind, count); }
int32_t __fastcall FakeB8Cell(void* m, int, int32_t x, int32_t y, int32_t which) {
    Logf("cell %d %d %d", x, y, which);
    return (uint32_t)(x * 31 + y * 17 + (int32_t)g_b8Seed) % 5 == 0;
}
void* __fastcall FakeB8Renderer(void* e) { return g_b8Scene; }
void __fastcall RecB8View(void* v) { Logf("view"); }
void* __cdecl FakeB8Malloc(size_t n) {
    static uint8_t buf[0x10000];
    Logf("malloc %u", (unsigned)n);
    return buf;
}
void B8Restore(uint32_t fn, const uint8_t* src, const int32_t* r, const uint8_t* src2) {
    __asm {
        push src2
        push r
        mov eax, src
        call fn
    }
}
void B8Stamp(uint32_t fn, uint8_t* o, int32_t owner) {
    __asm {
        push owner
        mov eax, o
        call fn
    }
}
} // namespace

DIFFTEST(terrain_RestoreAndFootprint) {
    SavedGlobals sg({ 0x62c120, 0x6d4024, 0x62b300, 0x62be68 });
    static uint8_t savedB308[0x740];
    memcpy(savedB308, (void*)0x62b308, sizeof savedB308);
    UiTrace t;
    t.Add(0x533fe0, (void*)&rt3::Terrain_SetFlag, (void*)&RecB8Flag);
    t.Add(0x533f20, (void*)&rt3::Terrain_SetCellByte, (void*)&RecB8Byte);
    t.Add(0x534100, (void*)&rt3::Terrain_SetCellCount, (void*)&RecB8Count);
    t.Add(0x534160, (void*)&rt3::Terrain_AddPoints, (void*)&RecB8Points);
    t.Add(0x448af0, (void*)&rt3::Map_CellFlag, (void*)&FakeB8Cell);
    t.Add(0x51f070, (void*)&rt3::Engine_Renderer, (void*)&FakeB8Renderer);
    t.Add(0x5a125d, nullptr, (void*)&FakeB8Malloc);
    static uint8_t map[0x2200], eng[16], obj[0x740], type[16], view[8];
    static void* viewVt[4];
    viewVt[0] = (void*)&RecB8View;
    *(void***)view = viewVt;
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6d4024) = eng;
    hook::Global<uint8_t*>(0x62be68) = view;
    for (int it = 0; it < 3000; ++it) {
        g_b8Seed = Rnd();
        *(int32_t*)(map + 0x15d9) = 4 + (int32_t)(Rnd() % 20);
        *(int32_t*)(map + 0x15d5) = 4 + (int32_t)(Rnd() % 20);
        *(int32_t*)(map + 0x2155) = 4 + (int32_t)(Rnd() % 20);
        *(int32_t*)(map + 0x2159) = 4 + (int32_t)(Rnd() % 20);
        int32_t r[4] = { (int32_t)(Rnd() % 8) - 2, 0, (int32_t)(Rnd() % 8) - 2, 0 };
        r[1] = r[0] + (int32_t)(Rnd() % 6) - 1;
        r[3] = r[2] + (int32_t)(Rnd() % 6) - 1;
        uint8_t src[64], src2[64];
        for (int k = 0; k < 64; ++k) {
            src[k] = Rnd() % 6 == 0 ? 0xff : (uint8_t)Rnd();
            src2[k] = (uint8_t)Rnd();
        }
        bool useObj = Rnd() % 2 != 0;
        uint8_t* o = useObj ? obj : (uint8_t*)0x62b308;
        int32_t shape = Rnd() % 6 == 0 ? -1 : (int32_t)(Rnd() % 6);
        int32_t owner = (int32_t)Rnd();
        bool alloc = Rnd() % 4 == 0;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(o + 0x732) = shape;
            *(uint8_t**)(o + 0x736) = (uint8_t*)(uintptr_t)(0x5ef3e0 + 0xb0 * (it % 5));
            *(int32_t*)(o + 0x4a0) = (int32_t)(it % 7);
            *(int32_t*)(o + 0x4a8) = (int32_t)(it % 5);
            static int32_t owners[0x4000];
            hook::Global<int32_t*>(0x62b300) = alloc ? nullptr : owners;
            memset(owners, 0, sizeof owners);
            g_b8Scene[0x374a] = 0;
            logs[side] = t.Run([&] {
                B8Restore(side ? (uint32_t)(uintptr_t)&rt3::Terrain_RestoreRect : 0x414ee0, src, r, src2);
                B8Stamp(side ? (uint32_t)(uintptr_t)&rt3::Footprint_Stamp : 0x4156b0, useObj ? obj : nullptr, owner);
            });
            logs[side] += " " + std::to_string(g_b8Scene[0x374a]);
            int32_t* ow = hook::Global<int32_t*>(0x62b300);
            for (int k = 0; k < 0x400; ++k)
                if (ow[k]) logs[side] += " o" + std::to_string(k);
        }
        CompareLogs(logs[0], logs[1], "terrain restore / footprint", it);
    }
    memcpy((void*)0x62b308, savedB308, sizeof savedB308);
}

namespace rt3 {
void Block3c2_Save();
void Record_DisplayName();
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall StreamWriteU32(int32_t s, uint32_t value);
}

namespace {
void __fastcall RecB9Write(int32_t s, const void* data, uint32_t size, int32_t f) {
    uint32_t h = 0;
    for (uint32_t k = 0; k < size; ++k) h = h * 31 + ((const uint8_t*)data)[k];
    Logf("write %d %u %08x %d", s, size, h, f);
}
void __fastcall RecB9WriteU32(int32_t s, uint32_t v) { Logf("u32 %d %08x", s, v); }
int32_t __fastcall FakeB9Has(int32_t id) { return id % 3 ? 0x101 : 0x100; }
const char* __fastcall FakeB9String(int32_t id) {
    static char b[4][16];
    static int k;
    char* s = b[k++ & 3];
    sprintf(s, "<%x>", id);
    return s;
}
void B9Save(uint32_t fn, void* f, const uint8_t* data, int32_t s) {
    __asm {
        push esi
        push ebx
        push edi
        mov esi, f
        mov ebx, data
        mov edi, s
        call fn
        pop edi
        pop ebx
        pop esi
    }
}
void B9Name(uint32_t fn, char* out, const uint8_t* rec) {
    __asm {
        push rec
        mov eax, out
        call fn
    }
}
} // namespace

DIFFTEST(save_Block3c2AndRecordName) {
    UiTrace t;
    t.Add(0x531030, (void*)&rt3::StreamWrite, (void*)&RecB9Write);
    t.Add(0x531340, (void*)&rt3::StreamWriteU32, (void*)&RecB9WriteU32);
    t.Add(0x51c900, (void*)&rt3::HasString, (void*)&FakeB9Has);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB9String);
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_block3c2.tmp");
    static uint8_t data[0x3c2], rec[0x100];
    for (int it = 0; it < 400; ++it) {
        for (auto& b : data) b = (uint8_t)Rnd();
        int32_t stream = (int32_t)(Rnd() % 10);
        bool toFile = it % 3 == 0;
        *(int32_t*)(rec + 0xc) = Rnd() % 6 == 0 ? 0xff : Rnd() % 5 == 0 ? 12345 : *(int32_t*)(0x5f0a34 + (Rnd() % 0x5b) * 0x10);
        rec[0x94] = (uint8_t)(Rnd() % 8);
        std::string logs[2], files[2];
        for (int side = 0; side < 2; ++side) {
            void* f = toFile ? ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, "w+b") : nullptr;
            char out[256];
            memset(out, 0x77, sizeof out);
            logs[side] = t.Run([&] {
                B9Save(side ? (uint32_t)(uintptr_t)&rt3::Block3c2_Save : 0x441ec0, f, data, stream);
                B9Name(side ? (uint32_t)(uintptr_t)&rt3::Record_DisplayName : 0x45ea90, out, rec);
                Logf("name [%s]", out);
            });
            if (f) {
                ((int(__cdecl*)(void*))0x5a1740)(f);
                FILE* r = fopen(path, "rb");
                char buf[0x400];
                size_t n = fread(buf, 1, sizeof buf, r);
                fclose(r);
                files[side].assign(buf, n);
            }
        }
        CompareLogs(logs[0], logs[1], "block 3c2 / record name", it);
        EXPECT_EQ(files[0] == files[1] ? 1 : 0, 1, "block 3c2 file %d (%u bytes)", it, (unsigned)files[0].size());
    }
    DeleteFileA(path);
}

namespace rt3 {
void Str_InnerParens();
void Str_SplitKeyValue();
}

namespace {
int32_t B10Parens(uint32_t fn, char* s) {
    int32_t r;
    __asm {
        push edi
        mov edi, s
        call fn
        pop edi
        mov r, eax
    }
    return r;
}
int32_t B10Split(uint32_t fn, const char* line, uint8_t* out) {
    int32_t r;
    __asm {
        push edi
        push out
        mov edi, line
        call fn
        pop edi
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(text_ParensAndKeyValue) {
    static const char* pieces[] = { " ", "(", ")", "=", "abc", "  x y ", "\t", "Music", "loop", "" };
    for (int it = 0; it < 5000; ++it) {
        std::string s;
        int n = (int)(Rnd() % 7);
        for (int k = 0; k < n; ++k) s += pieces[Rnd() % 10];
        std::string out[2];
        for (int side = 0; side < 2; ++side) {
            char buf[128];
            strcpy(buf, s.c_str());
            int32_t a = B10Parens(side ? (uint32_t)(uintptr_t)&rt3::Str_InnerParens : 0x557330, buf);
            uint8_t kv[0x210];
            memset(kv, 0x55, sizeof kv);
            int32_t b = B10Split(side ? (uint32_t)(uintptr_t)&rt3::Str_SplitKeyValue : 0x557390, s.c_str(), kv);
            out[side] = std::to_string(a) + "[" + buf + "] " + std::to_string(b) + " [" + std::string((char*)kv + 4, strnlen((char*)kv + 4, 0x100)) + "] [" +
                        std::string((char*)kv + 0x104, strnlen((char*)kv + 0x104, 0x100)) + "]";
        }
        CompareLogs(out[0], out[1], "parens / key-value", it);
    }
}

namespace rt3 {
int32_t __fastcall Models_ValueByName(const char* name);
int32_t __fastcall Models_DisplayName(const char* name, char* out);
int32_t __fastcall Obj_ClearAttachSlot(uint8_t* o, int, int32_t slot);
int32_t __fastcall Net_TouchEntry(uint8_t* o);
void __fastcall Music_Destroy(uint8_t* m);
uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby);
int32_t __fastcall GameList_Refresh(uint8_t* w);
void __fastcall NetQueue_Drain(uint8_t* s);
void __fastcall NetQueue_Push(uint8_t* s, int, const uint32_t* m, int32_t tag);
int32_t __fastcall Sound_SetBufferMs(uint8_t* e, int, void* o, int32_t ms);
void __fastcall Music_Shutdown(uint8_t* m);
int32_t __fastcall Return1_521790(void*, int, int32_t, int32_t);
void __fastcall Net_ResetOutgoing();
const char* __fastcall Obj_NextBufferedLine(void* o);
int32_t __fastcall Lobby_UpdateList(uint8_t* lobby);
int32_t __fastcall Lobby_ListGames(uint8_t* lobby);
void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
void* Engine_FirstListEntry();
struct HashMap;
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i);
int32_t __fastcall HasString(int32_t id);
}

namespace {
int32_t g_b11Lines, g_b11Hash;
uint8_t g_b11Pkts[8][0x20];
int g_b11NextPkt;
uint8_t g_b11First[0x80];
void __fastcall RecB11Shutdown(uint8_t* m) { Logf("shutdown"); }
ULONG __stdcall FakeB11Release(void* o) { Logf("release"); return 0; }
void WINAPI FakeB11CoUninit() { Logf("couninit"); }
void __cdecl RecB11Delete(void* p) { Logf("delete %p", p); }
int32_t __fastcall RecB11Ret1(void* a, int, int32_t h, int32_t b) { Logf("ret1 %p %d %x", a, h, b & 0xffff); return 1; }
void __fastcall RecB11Reset() { Logf("reset"); }
const char* __fastcall FakeB11Line(void* o) {
    if (g_b11Lines <= 0) return nullptr;
    --g_b11Lines;
    return "line";
}
int32_t __fastcall RecB11Update(uint8_t* l) { Logf("update"); return 3; }
int32_t __fastcall RecB11List(uint8_t* l) { Logf("list"); return 4; }
int32_t __fastcall RecB11Event(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Logf("event %x %x [%s] %u %u", type, id, (const char*)(uintptr_t)a, b, c);
    return 0;
}
void* __fastcall FakeB11Alloc(uint8_t* s, int, int32_t size) {
    Logf("alloc %d", size);
    return g_b11Pkts[g_b11NextPkt++ & 7];
}
void __fastcall RecB11Free(uint8_t* s, int, uint8_t* p) { Logf("free %d", (int)((p - g_b11Pkts[0]) / 0x20)); }
void* FakeB11First() { return g_b11First; }
int32_t __fastcall FakeB11Hash(rt3::HashMap* m, int, uint32_t key) {
    Logf("hash %u", key);
    return g_b11Hash;
}
int32_t __fastcall RecB11Clear(uint8_t* o, int32_t i) {
    Logf("clear %d", i);
    return i == 2;
}
HRESULT __stdcall FakeB11Prop(void* o, const void* guid, int32_t* v, int32_t n, int32_t how) {
    Logf("prop %p %d %d %d", guid, *v, n, how);
    return *v % 2 ? 0 : -1;
}
const char* __fastcall FakeB11String(int32_t id) {
    static char b[4][16];
    static int k;
    char* s = b[k++ & 3];
    sprintf(s, "<%x>", id);
    return s;
}
int32_t __fastcall FakeB11Has(int32_t id) { return id % 2; }
} // namespace

DIFFTEST(misc_LobbyQueuesMusicModels) {
    SavedGlobals sg({ 0x6cd920, 0x6cd8d8, 0x6d40d0, 0x6acd50, 0x6acd4c });
    static uint8_t savedBuckets[0x6ad96c + 0xc - 0x6acd78];
    memcpy(savedBuckets, (void*)0x6acd78, sizeof savedBuckets);
    UiTrace t;
    t.Add(0x5579d0, (void*)&rt3::Music_Shutdown, (void*)&RecB11Shutdown);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    t.Add(0x521790, (void*)&rt3::Return1_521790, (void*)&RecB11Ret1);
    t.Add(0x5223d0, (void*)&rt3::Net_ResetOutgoing, (void*)&RecB11Reset);
    t.Add(0x4695d0, (void*)&rt3::Obj_NextBufferedLine, (void*)&FakeB11Line);
    t.Add(0x469520, (void*)&rt3::Lobby_UpdateList, (void*)&RecB11Update);
    t.Add(0x469580, (void*)&rt3::Lobby_ListGames, (void*)&RecB11List);
    t.Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&RecB11Event);
    t.Add(0x469800, (void*)&rt3::NetPacket_Alloc, (void*)&FakeB11Alloc);
    t.Add(0x469890, (void*)&rt3::NetPacket_Free, (void*)&RecB11Free);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB11First);
    t.Add(0x53dae0, (void*)&rt3::Hash_Get, (void*)&FakeB11Hash);
    t.Add(0x4752b0, (void*)&rt3::Obj_EntryClear1d8, (void*)&RecB11Clear);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    t.Add(0x51c900, (void*)&rt3::HasString, (void*)&FakeB11Has);
    void** iat = (void**)0x5c8490;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeB11CoUninit;
    static uint8_t models[3][0xa0], music[0x20], perf[8], lobby[0xa000], mp[0x9000], sess[0x9880], queue[12], o[0x300], table[0x10 + 4 * 0xc], dev[8], eng[0x220];
    static void* comVt[0x20];
    comVt[8 / 4] = (void*)&FakeB11Release;
    comVt[0x48 / 4] = (void*)&FakeB11Prop;
    *(void***)perf = comVt;
    *(void***)dev = comVt;
    static const char* names[] = { "Loco", "Boxcar", "Tanker", "Nope" };
    for (int it = 0; it < 3000; ++it) {
        int32_t hdrFlag = (int32_t)(Rnd() % 2), lockFlag = (int32_t)(Rnd() % 2), busy = (int32_t)(Rnd() % 3 == 0);
        int lines = (int)(Rnd() % 4);
        int nq = (int)(Rnd() % 4);
        uint32_t msg[4] = { Rnd() % 4 == 0 ? 0xf423fu : Rnd() % 100, Rnd(), Rnd(), Rnd() };
        int32_t tag = (int32_t)(Rnd() % 50), slot = (int32_t)(Rnd() % 6) - 1, ms = (int32_t)(Rnd() % 3) * 777;
        const char* name = names[Rnd() % 4];
        bool hasQueue = Rnd() % 4 != 0, hasTable = Rnd() % 4 != 0;
        int32_t hash = Rnd() % 3 ? (int32_t)(Rnd() % 100) : 0;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset((void*)0x6acd78, 0, sizeof savedBuckets);
            for (int k = 0; k < 3; ++k) {
                memset(models[k], 0, sizeof models[k]);
                *(uint32_t*)(models[k] + 0x90) = rt3::Crc32String(names[k], 1);
                *(int32_t*)(models[k] + 0xc) = k * 3 + 1;
                models[k][0x94] = (uint8_t)k;
                uint32_t b = 0x6acd78 + (k * 7) * 0xc;
                *(uint8_t**)b = models[k];
            }
            memset(music, 0, sizeof music);
            *(int32_t*)music = it % 3 == 0;
            *(uint8_t**)(music + 8) = it % 2 ? perf : nullptr;
            *(void**)(music + 0x14) = it % 4 ? (void*)0x1234 : nullptr;
            *(int32_t*)(lobby + 0x8f18) = hdrFlag;
            *(int32_t*)(lobby + 0x985c) = lockFlag;
            memset(lobby + 0x10, 0x11, 0x40);
            hook::Global<int32_t>(0x6cd920) = busy;
            hook::Global<uint8_t*>(0x6cd8d8) = mp;
            hook::Global<void*>(0x6d40d0) = (void*)0x5555;
            g_b11Lines = lines;
            g_b11NextPkt = 0;
            memset(g_b11Pkts, 0, sizeof g_b11Pkts);
            for (int k = 0; k < nq; ++k) *(uint8_t**)g_b11Pkts[4 + k] = k + 1 < nq ? g_b11Pkts[5 + k] : nullptr;
            *(uint8_t**)queue = nq ? g_b11Pkts[4] : nullptr;
            *(uint8_t**)(queue + 4) = nq ? g_b11Pkts[3 + nq] : nullptr;
            *(int32_t*)(queue + 8) = nq;
            *(uint8_t**)(sess + 0x9878) = hasQueue ? queue : nullptr;
            g_b11Hash = hash;
            static uint32_t idWord;
            idWord = (uint32_t)tag;
            *(uint32_t**)(o + 8) = &idWord;
            *(uint8_t**)(o + 0x256) = hasTable ? table : nullptr;
            *(int32_t*)(o + 0x75) = 1;
            *(int32_t*)(table + 0xc) = 2;
            hook::Global<uint8_t*>(0x6acd50) = table;
            char out[128];
            memset(out, 0x66, sizeof out);
            logs[side] = t.Run([&] {
                Logf("value %d", side ? rt3::Models_ValueByName(name) : ((int32_t(__fastcall*)(const char*))0x45f400)(name));
                Logf("disp %d", side ? rt3::Models_DisplayName(name, out) : ((int32_t(__fastcall*)(const char*, char*))0x45f3d0)(name, out));
                Logf("disp [%s]", out[0] == 0x66 ? "-" : out);
                Logf("slot %d", side ? rt3::Obj_ClearAttachSlot(o, 0, slot) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x45b4f0)(o, 0, slot));
                Logf("touch %d", side ? rt3::Net_TouchEntry(o) : ((int32_t(__fastcall*)(uint8_t*))0x4705f0)(o));
                if (side) rt3::Music_Destroy(music);
                else ((void(__fastcall*)(uint8_t*))0x557b70)(music);
                uint8_t* r = side ? rt3::Lobby_FreshList(lobby) : ((uint8_t*(__fastcall*)(uint8_t*))0x46a1a0)(lobby);
                Logf("fresh %d", (int)(r - lobby));
                Logf("refresh %d", side ? rt3::GameList_Refresh(lobby) : ((int32_t(__fastcall*)(uint8_t*))0x4ed670)(lobby));
                if (side) rt3::NetQueue_Push(sess, 0, msg, tag);
                else ((void(__fastcall*)(uint8_t*, int, const uint32_t*, int32_t))0x46a5f0)(sess, 0, msg, tag);
                if (side) rt3::NetQueue_Drain(sess);
                else ((void(__fastcall*)(uint8_t*))0x46a590)(sess);
                Logf("prop %d", side ? rt3::Sound_SetBufferMs(eng, 0, dev, ms) : ((int32_t(__fastcall*)(uint8_t*, int, void*, int32_t))0x521620)(eng, 0, dev, ms));
            });
            char tail[128];
            sprintf(tail, " %d %d %d %d %d %d", *(int32_t*)music, *(int32_t*)(music + 8) != 0, *(int32_t*)(music + 0x14) != 0, *(int32_t*)(lobby + 0x8f18),
                    *(int32_t*)(lobby + 0x10), *(int32_t*)(eng + 0x216));
            logs[side] += tail;
            for (int k = 0; k < 8; ++k) logs[side] += " " + std::to_string(*(int32_t*)(g_b11Pkts[k] + 0x14)) + "/" + std::to_string(*(uint32_t*)(g_b11Pkts[k] + 4));
            logs[side] += " " + std::to_string(*(int32_t*)(queue + 8));
        }
        CompareLogs(logs[0], logs[1], "lobby / queues / music / models", it);
    }
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    memcpy((void*)0x6acd78, savedBuckets, sizeof savedBuckets);
}

namespace rt3 {
struct GameDate;
int32_t __fastcall Net_MessageComplete(uint8_t* s);
uint8_t* __fastcall NetPacket_Copy(uint8_t* s, int, const uint8_t* m);
uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d);
void Coder_Emit();
int32_t __fastcall DateToMinutes(const GameDate* d);
void __stdcall Net_UnlockQueueArg(int32_t);
int32_t Global6d40e4();
uint8_t* __stdcall Textures_Find(int32_t key);
}

namespace {
uint8_t g_b12Pkt[0x200], g_b12Entry[0x2600], g_b12Hash[0x20];
int32_t g_b12Found, g_b12HashOn, g_b12Flushes;
int32_t __fastcall FakeB12Minutes(const rt3::GameDate* d) { return 4321; }
void __stdcall RecB12Unlock(int32_t v) { Logf("unlock %d", v); }
int32_t FakeB12Frame() { return 77; }
uint8_t* __stdcall FakeB12Find(int32_t k) {
    Logf("find %d", k);
    return g_b12Found ? g_b12Entry : nullptr;
}
int32_t __fastcall FakeB12Hash(rt3::HashMap* m, int, uint32_t key) {
    Logf("hash %u", key);
    return g_b12HashOn ? (int32_t)(uintptr_t)g_b12Hash : 0;
}
void* __fastcall FakeB12Alloc(uint8_t* s, int, int32_t size) {
    Logf("alloc %d", size);
    memset(g_b12Pkt, 0xcc, sizeof g_b12Pkt);
    return g_b12Pkt;
}
int32_t FakeB12Clock() { return 999; }
int32_t __fastcall RecB12Flush(uint8_t* buf, int32_t n) {
    ++g_b12Flushes;
    Logf("flush %d", n);
    return 0;
}
int32_t B12Emit(uint32_t fn, uint8_t* c, void* cb) {
    int32_t r;
    __asm {
        push cb
        mov eax, c
        call fn
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(net_MessagesAndCoderEmit) {
    SavedGlobals sg({ 0xda7dd4, 0xda7dd8 });
    UiTrace t;
    t.Add(0x51d3c0, (void*)&rt3::DateToMinutes, (void*)&FakeB12Minutes);
    t.Add(0x5216d0, (void*)&rt3::Net_UnlockQueueArg, (void*)&RecB12Unlock);
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeB12Frame);
    t.Add(0x521d60, (void*)&rt3::Textures_Find, (void*)&FakeB12Find);
    t.Add(0x53dae0, (void*)&rt3::Hash_Get, (void*)&FakeB12Hash);
    t.Add(0x469800, (void*)&rt3::NetPacket_Alloc, (void*)&FakeB12Alloc);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeB12Clock);
    static uint8_t sess[0x9100], msg[0x28], addr[12], payload[0x40], queue[12], node[0x30], coder[0x60], streamBuf[0x8010];
    for (int it = 0; it < 4000; ++it) {
        g_b12Found = Rnd() % 3 != 0;
        g_b12HashOn = Rnd() % 3 != 0;
        *(int32_t*)g_b12Hash = (int32_t)(Rnd() % 5);
        *(int32_t*)(g_b12Hash + 0x10) = (int32_t)(Rnd() % 5);
        for (auto& b : msg) b = (uint8_t)Rnd();
        for (auto& b : addr) b = (uint8_t)Rnd();
        for (auto& b : payload) b = (uint8_t)Rnd();
        *(uint8_t**)(msg + 4) = addr;
        *(uint8_t**)(msg + 8) = payload;
        *(int32_t*)(msg + 0xc) = (int32_t)(Rnd() % 0x30);
        bool tail = Rnd() % 2 != 0;
        uint8_t c0[0x60];
        for (auto& b : c0) b = (uint8_t)Rnd();
        *(int32_t*)(c0 + 0x20) = (int32_t)(Rnd() % 8);
        *(int32_t*)(c0 + 0x18) = (int32_t)(Rnd() % 12);
        *(int32_t*)(c0 + 0xc) = 8 + (int32_t)(Rnd() % 8);
        *(int32_t*)(c0 + 0x10) = (int32_t)(Rnd() % 10);
        c0[0x4d] = Rnd() % 3 == 0 ? 0x80 : (uint8_t)(1 << (Rnd() % 7));
        *(int32_t*)(c0 + 0x14) = (int32_t)(Rnd() % 16);
        static uint8_t window[0x40];
        *(uint8_t**)(c0 + 0x38) = window;
        int32_t pos = (int32_t)(Rnd() % 0x7ff0);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            *(uint8_t**)(queue + 4) = tail ? node : nullptr;
            *(uint8_t**)queue = tail ? node : nullptr;
            *(int32_t*)(queue + 8) = tail;
            *(uint8_t**)node = nullptr;
            *(uint8_t**)(sess + 0x8f2c) = queue;
            memcpy(coder, c0, sizeof c0);
            hook::Global<uint8_t*>(0xda7dd8) = streamBuf;
            hook::Global<int32_t>(0xda7dd4) = pos;
            g_b12Flushes = 0;
            std::string pkt1, pkt2;
            logs[side] = t.Run([&] {
                Logf("complete %d", side ? rt3::Net_MessageComplete(sess) : ((int32_t(__fastcall*)(uint8_t*))0x46a3d0)(sess));
                uint8_t* p = side ? rt3::NetPacket_Copy(sess, 0, msg) : ((uint8_t*(__fastcall*)(uint8_t*, int, const uint8_t*))0x46a310)(sess, 0, msg);
                pkt1.assign((char*)p, 0x34 + 0x30);
                *(uint8_t**)(pkt1.data() + 4) = nullptr;
                p = side ? rt3::Net_QueueText(sess, 0, "hello", 1, 2, 3, 4)
                         : ((uint8_t*(__fastcall*)(uint8_t*, int, const char*, int32_t, int32_t, int32_t, int32_t))0x469a50)(sess, 0, "hello", 1, 2, 3, 4);
                pkt2.assign((char*)p + 4, 0x30);
                Logf("emit %d", B12Emit(side ? (uint32_t)(uintptr_t)&rt3::Coder_Emit : 0x56bc00, coder, (void*)&RecB12Flush));
            });
            auto hex = [](const std::string& b) {
                std::string h;
                char x[4];
                for (unsigned char ch : b) sprintf(x, "%02x", ch), h += x;
                return h;
            };
            logs[side] += "\npkt1 " + hex(pkt1) + "\npkt2 " + hex(pkt2) + "\ncoder " + hex(std::string((char*)coder, sizeof coder)) + "\n";
            char tail2[64];
            sprintf(tail2, " %d %d %d", *(int32_t*)(queue + 8), *(uint8_t**)(queue + 4) == g_b12Pkt, hook::Global<int32_t>(0xda7dd4));
            logs[side] += tail2;
        }
        if (logs[0] != logs[1]) {
            CompareLogs(logs[0], logs[1], "net messages / coder emit", it);
            EXPECT_EQ(0, 1, "net messages / coder emit differ at %d", it);
        }
    }
}

namespace rt3 {
void __fastcall Objects_HighlightGroup(Collection* t, int, int32_t group);
int32_t __stdcall Profiles_IndexOfEntry(int32_t key);
void __stdcall Textures_RefreshQueued(int32_t);
uint32_t __stdcall HeatColor(float t);
void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b);
void Net_UnlockQueue();
int32_t Net_LockQueue();
int32_t Global6d4100();
int32_t __fastcall Textures_QueryDesc(uint8_t* e, int, int32_t key, int32_t lock);
}

namespace {
int32_t g_b13Count, g_b13Found, g_b13Frames, g_b13Lock, g_b13First;
uint8_t g_b13Objs[6][0x240], g_b13Entry[0x40], g_b13Nodes[3][0x26a0];
uint32_t __stdcall FakeB13Heat(float t) {
    Logf("heat %08x", B(t));
    return t > 0.5f ? 0x00ff8040 : 0x00102030;
}
void __fastcall RecB13Rgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b) { Logf("rgb %d %d %d %d", (int)((o - g_b13Objs[0]) / 0x240), r, g, b); }
int32_t __fastcall FakeB13Count(Collection* c) { return g_b13Count; }
uint32_t __fastcall FakeB13IdAt(Collection* c, int, uint32_t pos, int list) { return pos; }
void* __fastcall FakeB13Get(Collection* c, int, uint32_t id) { return g_b13Objs[id % 6]; }
uint8_t* __stdcall FakeB13Find(int32_t k) {
    Logf("find %d", k);
    return g_b13Found ? g_b13Entry : nullptr;
}
void __stdcall RecB13UnlockArg(int32_t v) { Logf("unlockarg %d", v); }
void RecB13Unlock() { Logf("unlock"); }
int32_t FakeB13Lock() {
    static uint8_t* head;
    head = g_b13Nodes[0];
    return g_b13Lock ? (int32_t)(uintptr_t)&head : 0;
}
int32_t FakeB13Frames() { return g_b13Frames; }
int32_t FakeB13Frame() { return 2; }
void* FakeB13First() { return g_b13First ? (void*)g_b13Entry : nullptr; }
int32_t __fastcall FakeB13Query(uint8_t* e, int, int32_t key, int32_t lock) {
    Logf("query %d %d", key, lock);
    return key % 2;
}
} // namespace

DIFFTEST(misc_HighlightProfilesTextures) {
    SavedGlobals sg({ 0x6cec78, 0x6cd910, 0x6d40d0, 0x6ceb9c });
    UiTrace t;
    t.Add(0x51dfc0, (void*)&rt3::HeatColor, (void*)&FakeB13Heat);
    t.Add(0x48a060, nullptr, (void*)&RecB13Rgb);
    t.Add(0x53a3a0, (void*)&rt3::Obj_SetRgb, (void*)&RecB13Rgb);
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB13Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB13IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB13Get);
    t.Add(0x521d60, (void*)&rt3::Textures_Find, (void*)&FakeB13Find);
    t.Add(0x5216d0, (void*)&rt3::Net_UnlockQueueArg, (void*)&RecB13UnlockArg);
    t.Add(0x5216a0, (void*)&rt3::Net_UnlockQueue, (void*)&RecB13Unlock);
    t.Add(0x521680, (void*)&rt3::Net_LockQueue, (void*)&FakeB13Lock);
    t.Add(0x521670, (void*)&rt3::Global6d4100, (void*)&FakeB13Frames);
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeB13Frame);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB13First);
    t.Add(0x521f90, (void*)&rt3::Textures_QueryDesc, (void*)&FakeB13Query);
    static uint8_t world[8], tbl[8];
    static const char* names[] = { "Ann", "bob", "Cy" };
    for (int it = 0; it < 3000; ++it) {
        g_b13Count = (int32_t)(Rnd() % 7) - 1;
        g_b13Found = Rnd() % 3 != 0;
        g_b13Frames = (int32_t)(Rnd() % 3);
        g_b13Lock = Rnd() % 4 != 0;
        g_b13First = Rnd() % 4 != 0;
        for (int k = 0; k < 6; ++k) {
            *(uint16_t*)(g_b13Objs[k] + 0x22e) = (uint16_t)(Rnd() % 3);
            strcpy((char*)g_b13Objs[k] + 8, names[Rnd() % 3]);
        }
        strcpy((char*)g_b13Entry + 8, names[Rnd() % 3]);
        int nn = (int)(Rnd() % 4);
        for (int k = 0; k < 3; ++k) {
            *(uint8_t**)g_b13Nodes[k] = k + 1 < nn ? g_b13Nodes[k + 1] : nullptr;
            *(int32_t*)(g_b13Nodes[k] + 4) = (int32_t)(Rnd() % 4);
            *(int32_t*)(g_b13Nodes[k] + 0x269c) = (int32_t)Rnd();
        }
        if (!nn) g_b13Lock = 0;
        int32_t group = (int32_t)(Rnd() % 3), key = (int32_t)(Rnd() % 9);
        hook::Global<uint8_t*>(0x6cec78) = Rnd() % 3 ? world : nullptr;
        hook::Global<int32_t>(0x6cd910) = Rnd() % 2;
        hook::Global<uint8_t*>(0x6ceb9c) = tbl;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (auto& n : g_b13Nodes) *(int32_t*)(n + 0x54) = 5;
            logs[side] = t.Run([&] {
                if (side) rt3::Objects_HighlightGroup((Collection*)tbl, 0, group);
                else ((void(__fastcall*)(Collection*, int, int32_t))0x493000)((Collection*)tbl, 0, group);
                Logf("idx %d", side ? rt3::Profiles_IndexOfEntry(key) : ((int32_t(__stdcall*)(int32_t))0x46d9e0)(key));
                if (side) rt3::Textures_RefreshQueued(0);
                else ((void(__stdcall*)(int32_t))0x469ca0)(0);
            });
            for (auto& n : g_b13Nodes) logs[side] += " " + std::to_string(*(int32_t*)(n + 0x54));
        }
        CompareLogs(logs[0], logs[1], "highlight / profiles / textures", it);
    }
}

namespace rt3 {
void __fastcall SetupWindow_Destroy(uint8_t* w);
int32_t __fastcall Music_Start(uint8_t* m);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);
void __fastcall IndustryTypes_MarkAvailable(Collection* t);
}

namespace {
int32_t g_b14Hr, g_b14Init, g_b14Read, g_b14C, g_b14D;
uint8_t g_b14Perf[8];
void __fastcall RecB14Release(uint8_t* p, int, void* o) { Logf("release %p", o); }
void __fastcall FakeB14Vtbl(uint8_t* w) { Logf("vtbl %08x", *(uint32_t*)w); }
void __fastcall RecB14Avail(Collection* t) { Logf("avail %p", t); }
void __fastcall RecB14Shutdown(uint8_t* m) { Logf("shutdown"); }
void WINAPI RecB14Co(void* a, void* b) { Logf("coinit"); }
HRESULT WINAPI FakeB14Create(const void* c, void* o, DWORD x, const void* i, void** out) {
    Logf("create %p %p", c, i);
    if (g_b14Hr < 0) return g_b14Hr;
    *out = g_b14Perf;
    return 0;
}
HRESULT __stdcall FakeB14Init(void* p, void* a, uint32_t cb, void* b) {
    Logf("init %x", cb);
    return g_b14Init;
}
int32_t __fastcall FakeB14Read(uint8_t* m) { Logf("read"); return g_b14Read; }
int32_t __fastcall FakeB14C(uint8_t* m) { Logf("c"); return g_b14C; }
int32_t __fastcall FakeB14D(uint8_t* m) { Logf("d"); return g_b14D; }
} // namespace

DIFFTEST(misc_SetupDestroyMusicStart) {
    SavedGlobals sg({ 0x6cff84, 0x6cff88, 0x6cff8c, 0x6cff90, 0x6d4020, 0x62b268, 0x6d411c });
    static uint8_t savedSlots[0xa4];
    memcpy(savedSlots, (void*)0x6cfed0, sizeof savedSlots);
    UiTrace t;
    t.Add(0x53c000, (void*)&rt3::Pack_Release, (void*)&RecB14Release);
    t.Add(0x53f7d0, (void*)&rt3::Widget_SetVtbl, (void*)&FakeB14Vtbl);
    t.Add(0x412c10, (void*)&rt3::IndustryTypes_MarkAvailable, (void*)&RecB14Avail);
    t.Add(0x5579d0, (void*)&rt3::Music_Shutdown, (void*)&RecB14Shutdown);
    t.Add(0x557a30, (void*)&rt3::Music_ReadParam, (void*)&FakeB14Read);
    t.Add(0x557af0, (void*)&rt3::Music_CreateC, (void*)&FakeB14C);
    t.Add(0x557b30, (void*)&rt3::Music_Create10, (void*)&FakeB14D);
    IatSlot i1(0x5c8494, (void*)&RecB14Co), i2(0x5c848c, (void*)&FakeB14Create);
    static void* vt[8];
    vt[0xc / 4] = (void*)&FakeB14Init;
    *(void***)g_b14Perf = vt;
    static uint8_t w[0x40], m[0x20];
    for (int it = 0; it < 2000; ++it) {
        uint32_t g[4] = { Rnd() % 2 ? 0x11u : 0, Rnd() % 2 ? 0x22u : 0, Rnd() % 2 ? 0x33u : 0, Rnd() % 2 ? 0x44u : 0 };
        g_b14Hr = Rnd() % 4 == 0 ? -5 : 0;
        g_b14Init = Rnd() % 4 == 0 ? -7 : 0;
        g_b14Read = (int32_t)(Rnd() % 2);
        g_b14C = (int32_t)(Rnd() % 3);
        g_b14D = (int32_t)(Rnd() % 2);
        int32_t inited = (int32_t)(Rnd() % 2);
        bool buf = Rnd() % 2 != 0;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < 4; ++k) hook::Global<uint32_t>(0x6cff84 + k * 4) = g[k];
            for (int k = 0; k < 41; ++k) ((uint32_t*)0x6cfed0)[k] = 0x100 + k;
            memset(m, 0, sizeof m);
            *(int32_t*)m = inited;
            *(uint8_t**)(m + 8) = g_b14Perf;
            *(void**)(m + 0x14) = buf ? (void*)0x99 : nullptr;
            hook::Global<int32_t>(0x6d411c) = 3;
            logs[side] = t.Run([&] {
                if (side) rt3::SetupWindow_Destroy(w);
                else ((void(__fastcall*)(uint8_t*))0x4c9cc0)(w);
                Logf("start %d", side ? rt3::Music_Start(m) : ((int32_t(__fastcall*)(uint8_t*))0x557bc0)(m));
            });
            char tail[96];
            sprintf(tail, " %x %x %x %x %d %d %d", hook::Global<uint32_t>(0x6cff84), hook::Global<uint32_t>(0x6cff88), hook::Global<uint32_t>(0x6cff8c),
                    hook::Global<uint32_t>(0x6cff90), *(int32_t*)m, *(int32_t*)(m + 4), hook::Global<int32_t>(0x6d411c));
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "setup destroy / music start", it);
    }
    memcpy((void*)0x6cfed0, savedSlots, sizeof savedSlots);
}

namespace rt3 {
uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
void __fastcall Net_DropOldMessages(uint8_t* s, int, uint8_t* e, int32_t minutes);
void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);
void* __stdcall NewListHeader(int32_t);
int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);
}

namespace {
int32_t g_b15Found, g_b15Have, g_b15Rounds, g_b15Removed;
int32_t g_b15Counter;
uint8_t g_b15Entry[0x2600], g_b15Lists[4][0x14], g_b15Items[8][0x10];
uint8_t* __stdcall FakeB15Find(int32_t k) {
    Logf("find %d", k);
    return g_b15Found ? g_b15Entry : nullptr;
}
int32_t __fastcall FakeB15Get(rt3::HashMap* m, int, uint32_t key) {
    Logf("get %u", key);
    return g_b15Have ? (int32_t)(uintptr_t)&g_b15Counter : 0;
}
void* __stdcall FakeB15New(int32_t k) {
    Logf("new %d", k);
    return &g_b15Counter;
}
void __fastcall RecB15Insert(rt3::HashMap* m, int, uint32_t key, int32_t v) { Logf("insert %u %d", key, v == (int32_t)(uintptr_t)&g_b15Counter); }
int32_t __fastcall FakeB15Minutes(const rt3::GameDate* d) { return 1234; }
int32_t __fastcall FakeB15ForEach(rt3::HashMap* m, int, uint32_t key, void* fn, int32_t* ctx) {
    Logf("foreach %u %p %d", key, fn, *ctx);
    if (g_b15Rounds <= 0) return 0;
    --g_b15Rounds;
    hook::Global<uint32_t>(0x6cd968) = (uint32_t)g_b15Rounds;
    return 1;
}
int32_t __fastcall FakeB15Remove(rt3::HashMap* m, int, uint32_t key) {
    Logf("remove %u", key);
    return key % 2 ? (int32_t)(uintptr_t)g_b15Lists[key % 4] : 0;
}
void __fastcall RecB15PktFree(uint8_t* s, int, uint8_t* p) { Logf("pkt %d", (int)((p - g_b15Items[0]) / 0x10)); }
void __cdecl RecB15Delete(void* p) { Logf("delete %d", (int)(((uint8_t*)p - g_b15Lists[0]) / 0x14)); }
} // namespace

DIFFTEST(net_MessagePartsAndDrops) {
    SavedGlobals sg({ 0x6cec78, 0x6cd968 });
    UiTrace t;
    t.Add(0x51d3c0, (void*)&rt3::DateToMinutes, (void*)&FakeB15Minutes);
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeB13Frame);
    t.Add(0x5216d0, (void*)&rt3::Net_UnlockQueueArg, (void*)&RecB13UnlockArg);
    t.Add(0x521d60, (void*)&rt3::Textures_Find, (void*)&FakeB15Find);
    t.Add(0x53dae0, (void*)&rt3::Hash_Get, (void*)&FakeB15Get);
    t.Add(0x46ab10, (void*)&rt3::NewListHeader, (void*)&FakeB15New);
    t.Add(0x53d960, (void*)&rt3::Hash_Insert, (void*)&RecB15Insert);
    t.Add(0x53db20, nullptr, (void*)&FakeB15ForEach);
    t.Add(0x53d9e0, (void*)&rt3::Hash_Remove, (void*)&FakeB15Remove);
    t.Add(0x469890, (void*)&rt3::NetPacket_Free, (void*)&RecB15PktFree);
    t.Add(0x53b080, nullptr, (void*)&RecB15Delete);
    static uint8_t world[0x40], sess[0x9100];
    for (int it = 0; it < 3000; ++it) {
        g_b15Found = Rnd() % 3 != 0;
        g_b15Have = Rnd() % 2;
        int rounds = (int)(Rnd() % 5);
        *(int32_t*)(world + 0x19) = Rnd() % 4 != 0;
        bool hasWorld = Rnd() % 5 != 0;
        int32_t count = (int32_t)(Rnd() % 2), minutes = Rnd() % 2 ? 0 : (int32_t)(Rnd() % 9999);
        int nitems[4];
        for (int& n : nitems) n = (int)(Rnd() % 3);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<uint8_t*>(0x6cec78) = hasWorld ? world : nullptr;
            g_b15Counter = 10;
            g_b15Rounds = rounds;
            int used = 0;
            for (int k = 0; k < 4; ++k) {
                uint8_t* l = g_b15Lists[k];
                memset(l, 0, 0x14);
                for (int j = 0; j < nitems[k]; ++j) {
                    uint8_t* item = g_b15Items[(used + j) % 8];
                    *(uint8_t**)item = j + 1 < nitems[k] ? g_b15Items[(used + j + 1) % 8] : nullptr;
                }
                if (nitems[k]) {
                    *(uint8_t**)(l + 8) = g_b15Items[used % 8];
                    *(uint8_t**)(l + 0xc) = g_b15Items[(used + nitems[k] - 1) % 8];
                }
                *(int32_t*)(l + 0x10) = nitems[k];
                used += nitems[k];
            }
            logs[side] = t.Run([&] {
                uint8_t* r = side ? rt3::Net_CountMessagePart(sess, 0, count) : ((uint8_t*(__fastcall*)(uint8_t*, int, int32_t))0x46b6d0)(sess, 0, count);
                Logf("count %d %d", r ? (int)(r - sess) : -1, g_b15Counter);
                if (side) rt3::Net_DropOldMessages(sess, 0, g_b15Entry, minutes);
                else ((void(__fastcall*)(uint8_t*, int, uint8_t*, int32_t))0x46b910)(sess, 0, g_b15Entry, minutes);
            });
            for (auto& l : g_b15Lists) logs[side] += " " + std::to_string(*(int32_t*)(l + 0x10));
        }
        CompareLogs(logs[0], logs[1], "message parts / drops", it);
    }
}

namespace rt3 {
void Companies_ForEntry();
void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m);
float __fastcall Region_ClassSupply(uint8_t* r, int, int32_t cls);
int32_t __fastcall Models_ValueByName(const char* name);
void __fastcall Keyboard_SetScrollLock(int32_t on);
void __fastcall Obj_Nudge92(void* o);
int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id);
}

namespace {
int32_t g_b16Owner, g_b16Profile, g_b16Model, g_b16IdAt;
uint8_t g_b16Co[0x60], g_b16Cargo[0x40][8];
int32_t __fastcall FakeB16Owner(void* c) { return g_b16Owner; }
int32_t __stdcall FakeB16Profile(int32_t k) {
    Logf("profile %d", k);
    return g_b16Profile;
}
uint32_t __fastcall FakeB16IdAt(Collection* c, int, uint32_t pos, int list) {
    Logf("idat %u", pos);
    return (uint32_t)g_b16IdAt + pos;
}
void* __fastcall FakeB16Get(Collection* c, int, uint32_t id) {
    if (c == hook::Global<Collection*>(0x62ba8c)) return g_b16Cargo[id % 0x40];
    Logf("getco %u", id);
    return id % 5 ? g_b16Co : nullptr;
}
void __fastcall RecB16Save(uint8_t* o, int, int32_t e) { Logf("save %d", e); }
void __fastcall RecB16Checks(HMENU* m, int, char r) { Logf("checks"); }
void __fastcall RecB16Nudge(void* o) { Logf("nudge"); }
int32_t __fastcall FakeB16Model(const char* n) {
    Logf("model [%s]", n);
    return g_b16Model;
}
void __fastcall RecB16Scroll(int32_t on) { Logf("scroll %d", on); }
int32_t __fastcall FakeB16Valid(Collection* c, int, uint32_t id) { return id % 7 != 3; }
int32_t __fastcall FakeB16Match(void* c, int, int32_t cls) { return ((uint8_t*)c - g_b16Cargo[0]) / 8 % 3 == cls - 0x384; }
uint8_t* B16Company(uint32_t fn, int32_t i, int32_t key) {
    uint8_t* r;
    __asm {
        push key
        mov eax, i
        call fn
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(misc_CompanyModeSupply) {
    SavedGlobals sg({ 0x62be10, 0x6ceb9c, 0x6ada8c, 0x62be68, 0x62ba8c, 0x62c120, 0x6cec78 });
    UiTrace t;
    t.Add(0x425b90, (void*)&rt3::Company_OwnerFlag1e1, (void*)&FakeB16Owner);
    t.Add(0x46d9e0, (void*)&rt3::Profiles_IndexOfEntry, (void*)&FakeB16Profile);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB16IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB16Get);
    t.Add(0x484910, (void*)&rt3::Options_Save, (void*)&RecB16Save);
    t.Add(0x4625b0, (void*)&rt3::Menu_UpdateChecks, (void*)&RecB16Checks);
    t.Add(0x439b70, (void*)&rt3::Obj_Nudge92, (void*)&RecB16Nudge);
    t.Add(0x45f400, (void*)&rt3::Models_ValueByName, (void*)&FakeB16Model);
    t.Add(0x54e660, (void*)&rt3::Keyboard_SetScrollLock, (void*)&RecB16Scroll);
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeB16Valid);
    t.Add(0x41e260, (void*)&rt3::Cargo_MatchesClass, (void*)&FakeB16Match);
    static uint8_t cos[8], profs[8], holder[8], view[8], opt[0x200], cargo[0x200], map[0x2200], world[0x40], region[0x40];
    static uint8_t cells[64 * 0x1d7];
    static int32_t pts[6 * 3];
    hook::Global<uint8_t*>(0x62be10) = cos;
    hook::Global<uint8_t*>(0x6ceb9c) = profs;
    hook::Global<uint8_t*>(0x62ba8c) = cargo;
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(uint8_t**)(map + 0x2129) = cells;
    *(int32_t*)(map + 0x2145) = 8;
    for (int k = 0; k < 64 * 0x1d7; k += 4) *(float*)(cells + k) = RndF(0.0f, 100.0f);
    for (int it = 0; it < 3000; ++it) {
        g_b16Owner = Rnd() % 4 != 0;
        g_b16Profile = Rnd() % 4 == 0 ? 0xff : (int32_t)(Rnd() % 5);
        g_b16IdAt = Rnd() % 6 == 0 ? -1 : (int32_t)(Rnd() % 3);
        g_b16Model = Rnd() % 2 ? 0x91 : 3;
        *(uint32_t*)(g_b16Co + 0x3b) = Rnd() % 6;
        int32_t i = (int32_t)(Rnd() % 9), key = (int32_t)(Rnd() % 50);
        int32_t mode = (int32_t)(Rnd() % 12);
        for (int k = 0; k < 0x35; ++k) *(int32_t*)(cargo + 0x16e + k * 4) = Rnd() % 4 != 0;
        int32_t cls = 0x384 + (int32_t)(Rnd() % 4) - (Rnd() % 5 == 0 ? 2 : 0);
        int32_t now = 100000 + (int32_t)(Rnd() % 10000);
        int npts = (int)(Rnd() % 7) - 1;
        for (int k = 0; k < 6; ++k) {
            pts[k * 3] = (int32_t)(Rnd() % 8);
            pts[k * 3 + 1] = (int32_t)(Rnd() % 8);
        }
        int32_t stamps[3] = { Rnd() % 3 ? now - (int32_t)(Rnd() % 6000) : 0, Rnd() % 3 ? now - (int32_t)(Rnd() % 6000) : 0, Rnd() % 3 ? now - (int32_t)(Rnd() % 6000) : 0 };
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<uint8_t*>(0x6ada8c) = it % 3 ? holder : nullptr;
            hook::Global<uint8_t*>(0x62be68) = it % 4 ? view : nullptr;
            *(int32_t*)(opt + 0x10c) = it % 5 == 0 ? mode : 1;
            *(int32_t*)(opt + 0x1b7) = *(int32_t*)(opt + 0x1bb) = -1;
            *(int32_t*)(world + 0x15) = now;
            memset(region, 0, sizeof region);
            *(int32_t**)(region + 0x34) = pts;
            *(int32_t*)(region + 0x38) = npts;
            *(int32_t*)(region + 0x20) = stamps[0];
            *(int32_t*)(region + 0x18) = stamps[1];
            *(int32_t*)(region + 0x1c) = stamps[2];
            *(float*)(region + 0x14) = 1.5f;
            *(float*)(region + 0xc) = 2.5f;
            *(float*)(region + 0x10) = 3.5f;
            logs[side] = t.Run([&] {
                uint8_t* c = B16Company(side ? (uint32_t)(uintptr_t)&rt3::Companies_ForEntry : 0x46f8f0, i, key);
                Logf("company %d", c == g_b16Co);
                if (side) rt3::Options_SetDisplayMode(opt, 0, mode);
                else ((void(__fastcall*)(uint8_t*, int, int32_t))0x482c90)(opt, 0, mode);
                float v = side ? rt3::Region_ClassSupply(region, 0, cls) : ((float(__fastcall*)(uint8_t*, int, int32_t))0x47e390)(region, 0, cls);
                Logf("supply %08x", B(v));
            });
            logs[side] += std::string((char*)opt + 0x10c, 4) + std::string((char*)opt + 0x1b7, 8) + std::string((char*)region, 0x24);
        }
        if (logs[0] != logs[1]) {
            CompareLogs(logs[0], logs[1], "company / mode / supply", it);
            EXPECT_EQ(0, 1, "company / mode / supply state %d", it);
        }
    }
}

namespace rt3 {
void __fastcall Container_Save(uint8_t* o, int, int32_t s);
void __fastcall SetupWindow_FillLabels(uint8_t* w);
void __fastcall Nop53a580(void*, int, int32_t);
}

namespace {
uint8_t g_b17Kids[5][8];
int g_b17Count, g_b17Cursor, g_b17HasName;
void __fastcall RecB17Nop(void* o, int, int32_t s) { Logf("nop %d", s); }
void __fastcall FakeB17Rewind(PtrList* l) {
    Logf("rewind");
    g_b17Cursor = 0;
}
void* __fastcall FakeB17Next(PtrList* l) { return g_b17Cursor < g_b17Count ? g_b17Kids[g_b17Cursor++] : nullptr; }
void __fastcall RecB17ChildSave(void* c, int, int32_t s) { Logf("child %d %d", (int)(((uint8_t*)c - g_b17Kids[0]) / 8), s); }
void __fastcall RecB17Write(int32_t s, const void* d, uint32_t n, int32_t f) {
    Logf("write %d %u %02x %d", s, n, *(const uint8_t*)d, f);
}
int32_t __fastcall FakeB17Disp(const char* name, char* out) {
    Logf("disp [%s]", name);
    if (!g_b17HasName) return 0;
    strcpy(out, "Engine");
    return 1;
}
char* __fastcall FakeB17Format(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                               const char* a6, const char* a7, const char* a8) {
    Logf("format [%s] [%s] %p", fmt, a1, a2);
    sprintf(dst, " (%s)", a1);
    return dst;
}
const char* __fastcall FakeB17String(int32_t id) {
    static char b[4][16];
    static int k;
    char* s = b[k++ & 3];
    sprintf(s, "S%x", id);
    return s;
}
} // namespace

DIFFTEST(ui_ContainerSaveSetupLabels) {
    SavedGlobals sg({ 0x6d4024, 0x6cec74 });
    UiTrace t;
    t.Add(0x53a580, (void*)&rt3::Nop53a580, (void*)&RecB17Nop);
    t.Add(0x556ef0, (void*)&rt3::PtrList_Rewind, (void*)&FakeB17Rewind);
    t.Add(0x556f00, (void*)&rt3::PtrList_Next, (void*)&FakeB17Next);
    t.Add(0x531030, (void*)&rt3::StreamWrite, (void*)&RecB17Write);
    t.Add(0x45f3d0, (void*)&rt3::Models_DisplayName, (void*)&FakeB17Disp);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeB17Format);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB17String);
    static void* vt[0x20];
    vt[0x44 / 4] = (void*)&RecB17ChildSave;
    for (auto& k : g_b17Kids) *(void***)k = vt;
    static uint8_t o[0x260], list[0x20], w[0x40], opt[0x200];
    static uint8_t* eng = new uint8_t[0x114800];
    hook::Global<uint8_t*>(0x6d4024) = eng;
    hook::Global<uint8_t*>(0x6cec74) = opt;
    for (int it = 0; it < 2000; ++it) {
        g_b17Count = (int)(Rnd() % 6);
        bool hasList = Rnd() % 4 != 0;
        int sel = (int)(Rnd() % 7) - 1;
        *(uint16_t*)(list + 0xc) = (uint16_t)g_b17Count;
        eng[0x114226] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(opt + 0x114) = (int32_t)(Rnd() % 2);
        g_b17HasName = (int)(Rnd() % 2);
        int32_t stream = (int32_t)(Rnd() % 10);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            *(uint8_t**)(o + 8) = hasList ? list : nullptr;
            *(uint8_t**)(o + 0x248) = hasList && sel >= 0 ? g_b17Kids[sel % 5] : nullptr;
            g_b17Cursor = 0;
            logs[side] = t.Run([&] {
                if (side) rt3::Container_Save(o, 0, stream);
                else ((void(__fastcall*)(uint8_t*, int, int32_t))0x48a6c0)(o, 0, stream);
                if (side) rt3::SetupWindow_FillLabels(w);
                else ((void(__fastcall*)(uint8_t*))0x4e0960)(w);
            });
        }
        CompareLogs(logs[0], logs[1], "container save / setup labels", it);
    }
}

namespace rt3 {
void __fastcall World_SetNamedValue(uint8_t* w, int, const char* name, int32_t value);
}

namespace {
int32_t g_b18Count;
uint8_t g_b18Recs[6][0x22];
int32_t __fastcall FakeB18Count(Collection* c) { return g_b18Count; }
uint32_t __fastcall FakeB18IdAt(Collection* c, int, uint32_t pos, int list) { return pos; }
void* __fastcall FakeB18Get(Collection* c, int, uint32_t id) { return g_b18Recs[id % 6]; }
void __fastcall RecB18Add(Collection* t, int, void* rec, int32_t a, int32_t b, int32_t c) {
    Logf("add [%s] %d %d %d %d", (const char*)rec, *(int32_t*)((uint8_t*)rec + 0x1e), a, b, c);
}
void __fastcall RecB18Avail(Collection* t) { Logf("avail %p", t); }
} // namespace

DIFFTEST(world_SetNamedValue) {
    SavedGlobals sg({ 0x62b268 });
    UiTrace t;
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB18Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB18IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB18Get);
    t.Add(0x412c10, (void*)&rt3::IndustryTypes_MarkAvailable, (void*)&RecB18Avail);
    static void* vt[4];
    vt[1] = (void*)&RecB18Add;
    static uint8_t list[16], world[0x6700], types[8];
    *(void***)list = vt;
    *(uint8_t**)(world + 0x66b2) = list;
    static const char* names[] = { "alpha", "Beta", "GAMMA", "delta", "Epsilon" };
    for (int it = 0; it < 3000; ++it) {
        g_b18Count = (int32_t)(Rnd() % 7) - 1;
        for (int k = 0; k < 6; ++k) strcpy((char*)g_b18Recs[k], names[Rnd() % 5]);
        char name[16];
        strcpy(name, names[Rnd() % 5]);
        if (Rnd() % 2) name[0] = (char)(name[0] ^ 0x20);
        int32_t value = (int32_t)Rnd();
        hook::Global<uint8_t*>(0x62b268) = Rnd() % 3 ? types : nullptr;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            for (auto& r : g_b18Recs) *(int32_t*)(r + 0x1e) = 7;
            logs[side] = t.Run([&] {
                if (side) rt3::World_SetNamedValue(world, 0, name, value);
                else ((void(__fastcall*)(uint8_t*, int, const char*, int32_t))0x434f20)(world, 0, name, value);
            });
            for (auto& r : g_b18Recs) logs[side] += " " + std::to_string(*(int32_t*)(r + 0x1e));
        }
        CompareLogs(logs[0], logs[1], "set named value", it);
    }
}

namespace rt3 {
void __fastcall Train_EnterBlock(uint8_t* t, int, uint8_t* car, uint8_t* other);
void __fastcall Train_LeaveBlock(uint8_t* t);
void __fastcall Obj_MarkBlockSlot(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode);
void* __fastcall Obj_Ref388(void* o);
int32_t __fastcall Obj_TestBlock(uint8_t* o, int, uint32_t mask, int32_t idx);
uint32_t __fastcall Obj_SetBlockFlags(uint8_t* o, int, uint32_t mask, int32_t mode);
uint32_t __fastcall Obj_BeginBlocks(uint8_t* o);
int32_t __fastcall Signal_IsBehind(uint8_t* o, int, uint8_t* car, uint8_t* other);
void __fastcall Obj_Dec58c(uint8_t* o);
int32_t __fastcall Obj_CityRef10(void* o);
void* __fastcall Obj_Building(void* o);
}

namespace {
int32_t g_b19Side, g_b19Begin[2], g_b19BeginN, g_b19Clear, g_b19Test, g_b19City, g_b19Valid, g_b19Ign;
uint8_t g_b19Eng[0x400], g_b19Type[0x200], g_b19Kind[0x20], g_b19Sta[0x600], g_b19Bld[0x300];
void* __fastcall FakeB19Get(Collection* c, int, uint32_t id) {
    Logf("get %u", id);
    if (c == hook::Global<Collection*>(0x62ba84)) return id % 4 ? g_b19Eng : nullptr;
    if (c == hook::Global<Collection*>(0x6ada84)) return g_b19Kind;
    return g_b19Sta;
}
void* __fastcall FakeB19Ref(void* o) { return g_b19Type; }
int32_t __fastcall FakeB19Valid(Collection* c, int, uint32_t id) { return (int32_t)(id % 3 != 0) & g_b19Valid; }
int32_t __fastcall FakeB19Side(uint8_t* o, int, uint8_t* car, uint8_t* other) { return g_b19Side; }
void __fastcall RecB19Mark(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode) { Logf("mark %d %d %d", on, idx, mode); }
uint32_t __fastcall RecB19Flags(uint8_t* o, int, uint32_t mask, int32_t mode) { Logf("flags %u %d", mask, mode); return 0; }
uint32_t __fastcall FakeB19Begin(uint8_t* o) { return (uint32_t)g_b19Begin[g_b19BeginN++ & 1]; }
int32_t __fastcall FakeB19Clear(uint8_t* o, int, int32_t slot) {
    Logf("clear %d", slot);
    return g_b19Clear;
}
int32_t __fastcall FakeB19Test(uint8_t* o, int, uint32_t mask, int32_t idx) {
    Logf("test %u %d", mask, idx);
    return g_b19Test;
}
void __fastcall RecB19Dec(uint8_t* o) {
    Logf("dec");
    --*(int32_t*)(o + 0x58c);
}
int32_t __fastcall FakeB19City(void* o) { return g_b19City; }
void* __fastcall FakeB19Building(void* o) { return g_b19Bld; }
} // namespace

DIFFTEST(train_EnterLeaveBlock) {
    SavedGlobals sg({ 0x62ba84, 0x6ada84, 0x6cec20 });
    UiTrace t;
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB19Get);
    t.Add(0x41adb0, (void*)&rt3::Obj_Ref388, (void*)&FakeB19Ref);
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeB19Valid);
    t.Add(0x4a7bc0, (void*)&rt3::Signal_IsBehind, (void*)&FakeB19Side);
    t.Add(0x45b490, (void*)&rt3::Obj_MarkBlockSlot, (void*)&RecB19Mark);
    t.Add(0x45b430, (void*)&rt3::Obj_SetBlockFlags, (void*)&RecB19Flags);
    t.Add(0x45b540, (void*)&rt3::Obj_BeginBlocks, (void*)&FakeB19Begin);
    t.Add(0x45b4f0, (void*)&rt3::Obj_ClearAttachSlot, (void*)&FakeB19Clear);
    t.Add(0x45b460, (void*)&rt3::Obj_TestBlock, (void*)&FakeB19Test);
    t.Add(0x4a73e0, (void*)&rt3::Obj_Dec58c, (void*)&RecB19Dec);
    t.Add(0x4a9020, (void*)&rt3::Obj_CityRef10, (void*)&FakeB19City);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeB19Building);
    static uint8_t engs[8], kinds[8], stas[8], train[0x200], car[8], other[8];
    hook::Global<uint8_t*>(0x62ba84) = engs;
    hook::Global<uint8_t*>(0x6ada84) = kinds;
    hook::Global<uint8_t*>(0x6cec20) = stas;
    for (int it = 0; it < 4000; ++it) {
        g_b19Side = (int32_t)(Rnd() % 3);
        g_b19Begin[0] = Rnd() % 3 == 0 ? -1 : (int32_t)(Rnd() % 3);
        g_b19Begin[1] = Rnd() % 3 == 0 ? -1 : (int32_t)(Rnd() % 3);
        g_b19Clear = (int32_t)(Rnd() % 2);
        g_b19Test = (int32_t)(Rnd() % 2);
        g_b19City = (int32_t)(Rnd() % 2);
        g_b19Valid = Rnd() % 4 != 0;
        *(int32_t*)(g_b19Kind + 0x10) = Rnd() % 4 == 0;
        *(int32_t*)(g_b19Bld + 0x262) = (int32_t)(Rnd() % 3);
        int32_t eng = Rnd() % 3 == 0 ? -1 : (int32_t)(Rnd() % 8);
        float speed = Rnd() % 4 == 0 ? 1.0f : RndF(-2.0f, 3.0f);
        int32_t ac = Rnd() % 4 == 0 ? -1 : (int32_t)(Rnd() % 9), a4 = (int32_t)(Rnd() % 9), side0 = (int32_t)(Rnd() % 3);
        uint8_t flag = (uint8_t)(Rnd() % 3 != 0);
        int32_t claims = (int32_t)(Rnd() % 3);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(train + 0x41) = eng;
            *(float*)(train + 0xe0) = speed;
            *(int32_t*)(train + 0x192) = side0;
            *(int32_t*)(train + 0xac) = ac;
            *(int32_t*)(train + 0xa4) = a4;
            train[0x1a2] = flag;
            *(int32_t*)(g_b19Sta + 0x58c) = claims;
            g_b19BeginN = 0;
            logs[side] = t.Run([&] {
                if (side) rt3::Train_EnterBlock(train, 0, car, other);
                else ((void(__fastcall*)(uint8_t*, int, uint8_t*, uint8_t*))0x4aaad0)(train, 0, car, other);
                Logf("side %d", *(int32_t*)(train + 0x192));
                *(int32_t*)(train + 0x192) = side0;
                g_b19BeginN = 0;
                if (side) rt3::Train_LeaveBlock(train);
                else ((void(__fastcall*)(uint8_t*))0x4aac00)(train);
            });
            char tail[64];
            sprintf(tail, " %d %d %d %d", *(int32_t*)(train + 0x192), *(int32_t*)(train + 0xa4), train[0x1a2], *(int32_t*)(g_b19Sta + 0x58c));
            logs[side] += tail;
        }
        CompareLogs(logs[0], logs[1], "train enter / leave block", it);
    }
}

namespace rt3 {
struct MaxHeap;
uint8_t* Models_NextInOrder();
void __fastcall Lobby_Tick(uint8_t* l, int, uint32_t now);
int32_t __fastcall Heap_PopMax(MaxHeap* h);
struct HeapNode;
HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value);
void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
void __fastcall Heap_Destroy2(MaxHeap* h);
void __stdcall Net_PurgeSentMessages(int32_t);
int32_t Net_LockList2();
void __fastcall Net_BroadcastGame(uint8_t* o, int, const char* name);
void Net_UnlockList2();
void __stdcall Textures_RefreshQueued(int32_t);
}

namespace {
uint8_t* g_b20Heap[16];
int g_b20N;
uint8_t g_b20Models[4][0xa0], g_b20Nodes[3][0x120];
int32_t g_b20Lock;
int32_t __fastcall FakeB20Pop(rt3::MaxHeap* h) {
    Logf("pop %d", g_b20N);
    return g_b20N > 0 ? (int32_t)(uintptr_t)g_b20Heap[--g_b20N] : 0;
}
rt3::HeapNode* __fastcall FakeB20Push(rt3::MaxHeap* h, int, int32_t key, int32_t v) {
    Logf("push %d %d", key, (int)(((uint8_t*)(uintptr_t)v - g_b20Models[0]) / 0xa0));
    g_b20Heap[g_b20N++ & 15] = (uint8_t*)(uintptr_t)v;
    return nullptr;
}
void __fastcall RecB20Init(rt3::MaxHeap* h, int, int32_t c) { Logf("init %d", c); }
void __fastcall RecB20Destroy(rt3::MaxHeap* h) { Logf("destroy"); }
void __stdcall RecB20Purge(int32_t v) { Logf("purge %d", v); }
int32_t FakeB20Lock() {
    static uint8_t* head;
    head = g_b20Nodes[0];
    Logf("lock");
    return g_b20Lock ? (int32_t)(uintptr_t)&head : 0;
}
void __fastcall RecB20Bcast(uint8_t* o, int, const char* name) { Logf("bcast %p", name); }
void RecB20Unlock() { Logf("unlock2"); }
void __stdcall RecB20Refresh(int32_t now) { Logf("refresh %d", now); }
} // namespace

DIFFTEST(misc_ModelOrderLobbyTick) {
    SavedGlobals sg({ 0x5f0fe0, 0x6acd74, 0x6ad978, 0x6ad97c, 0x6cec74, 0x6cec78, 0x6d40dc });
    static uint8_t savedBuckets[0x6ad96c + 0xc - 0x6acd78], savedRec[0x100];
    memcpy(savedBuckets, (void*)0x6acd78, sizeof savedBuckets);
    memcpy(savedRec, (void*)0x6ad978, sizeof savedRec);
    UiTrace t;
    t.Add(0x53d540, (void*)&rt3::Heap_PopMax, (void*)&FakeB20Pop);
    t.Add(0x53d4c0, (void*)&rt3::Heap_Push, (void*)&FakeB20Push);
    t.Add(0x53d740, (void*)&rt3::Heap_Init, (void*)&RecB20Init);
    t.Add(0x53d6f0, (void*)&rt3::Heap_Destroy2, (void*)&RecB20Destroy);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    t.Add(0x51c900, (void*)&rt3::HasString, (void*)&FakeB11Has);
    t.Add(0x522430, (void*)&rt3::Net_PurgeSentMessages, (void*)&RecB20Purge);
    t.Add(0x5217c0, (void*)&rt3::Net_LockList2, (void*)&FakeB20Lock);
    t.Add(0x521800, (void*)&rt3::Net_BroadcastGame, (void*)&RecB20Bcast);
    t.Add(0x5217e0, (void*)&rt3::Net_UnlockList2, (void*)&RecB20Unlock);
    t.Add(0x469ca0, (void*)&rt3::Textures_RefreshQueued, (void*)&RecB20Refresh);
    static uint8_t opt[0x300], world[8];
    static uint8_t* lobby = new uint8_t[0xa000];
    hook::Global<uint8_t*>(0x6cec74) = opt;
    for (int it = 0; it < 2000; ++it) {
        int32_t start = Rnd() % 3 ? 1 : 0;
        *(int32_t*)(opt + 0x28f) = (int32_t)(Rnd() % 2);
        int pre = (int)(Rnd() % 3);
        uint32_t now = 100000 + Rnd() % 20000;
        uint32_t f9054 = Rnd() % 2 ? now - Rnd() % 2000 : 0, f8f14 = now - Rnd() % 8000, f8f10 = now - Rnd() % 2000;
        int32_t f8f30 = (int32_t)(Rnd() % 2), f8f18 = (int32_t)(Rnd() % 2), f985c = (int32_t)(Rnd() % 4 == 0), host = (int32_t)(Rnd() % 2);
        bool hasWorld = Rnd() % 3 == 0;
        g_b20Lock = Rnd() % 4 != 0;
        uint32_t seen[3] = { now - Rnd() % 20000, now - Rnd() % 20000, now - Rnd() % 20000 };
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<int32_t>(0x5f0fe0) = start;
            hook::Global<void*>(0x6acd74) = start ? nullptr : (void*)0x4444;
            memset((void*)0x6acd78, 0, sizeof savedBuckets);
            for (int k = 0; k < 4; ++k) {
                memset(g_b20Models[k], 0, 0xa0);
                *(int32_t*)(g_b20Models[k] + 4) = k % 2;
                *(int32_t*)(g_b20Models[k] + 8) = k * 11;
                *(int32_t*)(g_b20Models[k] + 0xc) = k == 3 ? 0xff : k + 1;
                *(int32_t*)(g_b20Models[k] + 0x9c) = k * 5;
                sprintf((char*)g_b20Models[k] + 0x10, "m%d.3dp", k);
                *(uint8_t**)(0x6acd78 + k * 3 * 0xc) = g_b20Models[k];
            }
            g_b20N = start ? 0 : pre;
            for (int k = 0; k < pre; ++k) g_b20Heap[k] = g_b20Models[k];
            hook::Global<uint8_t*>(0x6cec78) = hasWorld ? world : nullptr;
            hook::Global<int32_t>(0x6d40dc) = host;
            *(uint32_t*)(lobby + 0x9054) = f9054;
            *(uint32_t*)(lobby + 0x8f14) = f8f14;
            *(uint32_t*)(lobby + 0x8f10) = f8f10;
            *(int32_t*)(lobby + 0x8f30) = f8f30;
            *(int32_t*)(lobby + 0x8f18) = f8f18;
            *(int32_t*)(lobby + 0x985c) = f985c;
            memset(lobby + 0x10, 0x5a, 0x40);
            for (int k = 0; k < 3; ++k) {
                *(uint8_t**)g_b20Nodes[k] = k < 2 ? g_b20Nodes[k + 1] : nullptr;
                *(uint32_t*)(g_b20Nodes[k] + 0x118) = seen[k];
                g_b20Nodes[k][4] = (uint8_t)(k + 1);
            }
            logs[side] = t.Run([&] {
                for (int k = 0; k < 6; ++k) {
                    uint8_t* r = side ? rt3::Models_NextInOrder() : ((uint8_t* (*)())0x45f250)();
                    if (!r) {
                        Logf("end");
                        break;
                    }
                    Logf("model [%s] [%s] %d %d", (char*)0x6ad980, (char*)0x6ada00, *(int32_t*)0x6ad978, *(int32_t*)0x6ad97c);
                }
                if (side) rt3::Lobby_Tick(lobby, 0, now);
                else ((void(__fastcall*)(uint8_t*, int, uint32_t))0x46af70)(lobby, 0, now);
            });
            char tail[96];
            sprintf(tail, " %u %u %u %d", *(uint32_t*)(lobby + 0x9054), *(uint32_t*)(lobby + 0x8f14), *(uint32_t*)(lobby + 0x8f10), *(int32_t*)(lobby + 0x10));
            logs[side] += tail;
            for (int k = 0; k < 3; ++k) logs[side] += " " + std::to_string(lobby[0x10 + k * 0x11c + 4]) + "/" + std::to_string(*(int32_t*)(g_b20Nodes[k] + 0x118));
        }
        CompareLogs(logs[0], logs[1], "model order / lobby tick", it);
    }
    memcpy((void*)0x6acd78, savedBuckets, sizeof savedBuckets);
    memcpy((void*)0x6ad978, savedRec, sizeof savedRec);
}

namespace rt3 {
int32_t __fastcall Lzss_Unpack(int32_t(__fastcall* read)(uint8_t*, int32_t), int32_t(__fastcall* write)(uint8_t*, int32_t));
}

namespace {
std::vector<uint8_t> g_lzIn;
size_t g_lzPos, g_lzChunk;
uint32_t g_lzHash, g_lzWritten;
int32_t __fastcall FakeLzRead(uint8_t* buf, int32_t n) {
    size_t take = g_lzIn.size() - g_lzPos;
    if (take > g_lzChunk) take = g_lzChunk;
    if (take > (size_t)n) take = (size_t)n;
    memcpy(buf, g_lzIn.data() + g_lzPos, take);
    g_lzPos += take;
    return take ? (int32_t)take : -1;
}
int32_t __fastcall FakeLzWrite(uint8_t* buf, int32_t n) {
    for (int32_t k = 0; k < n; ++k) g_lzHash = g_lzHash * 31 + hook::Global<uint8_t*>(0xda7dd8)[k];
    g_lzWritten += (uint32_t)n;
    return 1;
}
} // namespace

DIFFTEST(save_LzssUnpack) {
    SavedGlobals sg({ 0xd9fdc8, 0xda7dd0, 0xda7dd4, 0xda7dd8 });
    static uint8_t savedIn[0x8010], savedOut[0x8010];
    memcpy(savedIn, (void*)0xd9fdd0, sizeof savedIn);
    memcpy(savedOut, (void*)0xd97dc8, sizeof savedOut);
    for (int it = 0; it < 200; ++it) {
        g_lzIn.assign(0x10, 0);
        g_lzIn[0xc] = 0x10;
        g_lzIn[0xe] = 0x91;
        if (it % 17 == 0) g_lzIn[0xe] = 0x90;
        size_t n = Rnd() % 0x12000;
        for (size_t k = 0; k < n; ++k) g_lzIn.push_back((uint8_t)Rnd());
        size_t chunk = Rnd() % 3 == 0 ? 0x8000 : 1 + Rnd() % 0x3000;
        int32_t r[2];
        uint32_t h[2], w[2];
        for (int side = 0; side < 2; ++side) {
            g_lzPos = 0;
            g_lzChunk = chunk;
            g_lzHash = 7;
            g_lzWritten = 0;
            r[side] = side ? rt3::Lzss_Unpack(&FakeLzRead, &FakeLzWrite)
                           : ((int32_t(__fastcall*)(void*, void*))0x56be10)((void*)&FakeLzRead, (void*)&FakeLzWrite);
            h[side] = g_lzHash;
            w[side] = g_lzWritten;
        }
        EXPECT_EQ(r[0], r[1], "lzss result %d", it);
        EXPECT_EQ(h[0], h[1], "lzss output %d", it);
        EXPECT_EQ(w[0], w[1], "lzss length %d", it);
    }
    EXPECT_EQ(rt3::Lzss_Unpack(nullptr, &FakeLzWrite), ((int32_t(__fastcall*)(void*, void*))0x56be10)(nullptr, (void*)&FakeLzWrite), "lzss null");
    memcpy((void*)0xd9fdd0, savedIn, sizeof savedIn);
    memcpy((void*)0xd97dc8, savedOut, sizeof savedOut);
}

namespace rt3 {
void Tree_InsertNode();
void Tree_Init();
}

namespace {
void B22Init(uint32_t fn, uint8_t* c) {
    __asm {
        push esi
        mov esi, c
        call fn
        pop esi
    }
}
void B22Insert(uint32_t fn, uint8_t* c, int32_t r) {
    __asm {
        push ebx
        push r
        mov ebx, c
        call fn
        pop ebx
    }
}
} // namespace

DIFFTEST(save_LzssTreeInsert) {
    static uint8_t c[2][0x60];
    for (int it = 0; it < 60; ++it) {
        uint8_t text[0x241];
        int alpha = 2 + (int)(Rnd() % 6);
        for (auto& b : text) b = (uint8_t)('a' + Rnd() % alpha);
        int n = 50 + (int)(Rnd() % 0x170); // each position once: a repeat without DeleteNode corrupts the trees
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(c[side], 0, sizeof c[side]);
            B22Init(side ? (uint32_t)(uintptr_t)&rt3::Tree_Init : 0x56bca0, c[side]);
            memcpy(*(uint8_t**)(c[side] + 0x38), text, sizeof text);
            std::string& l = logs[side];
            char b[32];
            sprintf(b, "%d %d %d|", *(int32_t*)(c[side] + 8), *(int32_t*)(c[side] + 0xc), *(int32_t*)(c[side] + 0x10));
            l += b;
            for (int k = 0; k < n; ++k) {
                int32_t r = (int32_t)k;
                B22Insert(side ? (uint32_t)(uintptr_t)&rt3::Tree_InsertNode : 0x56b990, c[side], r);
                sprintf(b, "%d,%d ", *(int32_t*)(c[side] + 0x1c), *(int32_t*)(c[side] + 0x20));
                l += b;
            }
            for (int a = 0x24; a <= 0x2c; a += 4) {
                int32_t* arr = *(int32_t**)(c[side] + a);
                int cnt = a == 0x2c ? 0x301 : 0x200;
                for (int k = 0; k < cnt; ++k)
                    if (a != 0x2c || k != 0x200) l += std::to_string(arr[k]) + ",";
                l += "|";
            }
            for (int a = 0x24; a <= 0x38; a += 4)
                if (a != 0x30 && a != 0x34) ((void(__cdecl*)(void*))0x5a1145)(*(void**)(c[side] + a));
        }
        CompareLogs(logs[0], logs[1], "lzss tree insert", it);
    }
}

namespace rt3 {
int32_t __fastcall Rle_Pack(int32_t(__fastcall* read)(uint8_t*, int32_t), int32_t(__fastcall* write)(uint8_t*, int32_t));
int32_t __fastcall Rle_Unpack(int32_t(__fastcall* read)(uint8_t*, int32_t), int32_t(__fastcall* write)(uint8_t*, int32_t));
}

namespace {
std::vector<uint8_t> g_rleOut;
int32_t __fastcall FakeRleWrite(uint8_t* buf, int32_t n) {
    g_rleOut.insert(g_rleOut.end(), buf, buf + n);
    return 1;
}
} // namespace

DIFFTEST(save_RlePackUnpack) {
    SavedGlobals sg({ 0xdafde0, 0xdafde4, 0xdb7de8, 0xdb7dec });
    static uint8_t savedOut[0x8008], savedIn[0x8000];
    memcpy(savedOut, (void*)0xda7de0, sizeof savedOut);
    memcpy(savedIn, (void*)0xdafde8, sizeof savedIn);
    typedef int32_t(__fastcall * Fn)(void*, void*);
    for (int it = 0; it < 300; ++it) {
        bool unpack = it % 2 == 1;
        if (!unpack || Rnd() % 4 == 0) {
            g_lzIn.clear();
            if (unpack) {
                g_lzIn.assign(0x10, 0);
                g_lzIn[0xc] = 0x10;
                g_lzIn[0xe] = it % 13 == 0 ? 0x91 : 0x92;
            }
            size_t n = Rnd() % 0x14000;
            while (g_lzIn.size() < n) {
                uint8_t b = (uint8_t)(Rnd() % 4 ? Rnd() % 6 : Rnd());
                size_t rep = Rnd() % 3 ? 1 : 1 + Rnd() % 300;
                for (size_t k = 0; k < rep; ++k) g_lzIn.push_back(b);
            }
        } else {
            g_lzIn = g_rleOut;
        }
        size_t chunk = Rnd() % 3 == 0 ? 0x8000 : 1 + Rnd() % 0x3000;
        int32_t r[2];
        std::vector<uint8_t> out[2];
        for (int side = 0; side < 2; ++side) {
            g_lzPos = 0;
            g_lzChunk = chunk;
            g_rleOut.clear();
            hook::Global<int32_t>(0xdb7de8) = 0;
            if (unpack) r[side] = side ? rt3::Rle_Unpack(&FakeLzRead, &FakeRleWrite) : ((Fn)0x56c630)((void*)&FakeLzRead, (void*)&FakeRleWrite);
            else r[side] = side ? rt3::Rle_Pack(&FakeLzRead, &FakeRleWrite) : ((Fn)0x56c3e0)((void*)&FakeLzRead, (void*)&FakeRleWrite);
            out[side] = g_rleOut;
        }
        EXPECT_EQ(r[0], r[1], "rle result %d", it);
        EXPECT_EQ(out[0] == out[1], true, "rle output %d", it);
    }
    EXPECT_EQ(rt3::Rle_Pack(nullptr, &FakeRleWrite), ((Fn)0x56c3e0)(nullptr, (void*)&FakeRleWrite), "rle pack null");
    EXPECT_EQ(rt3::Rle_Unpack(&FakeLzRead, nullptr), ((Fn)0x56c630)((void*)&FakeLzRead, nullptr), "rle unpack null");
    memcpy((void*)0xda7de0, savedOut, sizeof savedOut);
    memcpy((void*)0xdafde8, savedIn, sizeof savedIn);
}

namespace rt3 {
int32_t __fastcall Net_PlayerLeft(int32_t, uint8_t* msg);
}

namespace {
int32_t __fastcall FakeB24Bcast(int32_t a, int32_t b, int32_t c) {
    Logf("bcast %d %d %p", a, b, (void*)(uintptr_t)c);
    return a + b + 3;
}
} // namespace

DIFFTEST(net_PlayerLeft) {
    SavedGlobals sg({ 0x6d408c, 0x6d40a8, 0x6d4100 });
    IatSlot i2(0x5c8074, (void*)&FakeElLeave), i3(0x5c81fc, (void*)&FakeElEnter), i4(0x5c81f8, (void*)&FakeElLeave);
    UiTrace t;
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeIpFormat);
    t.Add(0x522240, (void*)&rt3::CmdQueue_Broadcast, (void*)&FakeB24Bcast);
    static uint8_t players[3][0x100], head[8], msg[0x10];
    hook::Global<uint8_t*>(0x6d408c) = head;
    for (int it = 0; it < 500; ++it) {
        int np = (int)(Rnd() % 4);
        int32_t id = (int32_t)(Rnd() % 5), why = (int32_t)(Rnd() % 6);
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            *(uint8_t**)head = np ? players[0] : nullptr;
            for (int k = 0; k < 3; ++k) {
                memset(players[k], 0, 0x100);
                *(uint8_t**)players[k] = k + 1 < np ? players[k + 1] : nullptr;
                *(int32_t*)(players[k] + 4) = k + 1;
                sprintf((char*)players[k] + 8, "p%d", k);
            }
            *(int32_t*)(msg + 4) = id;
            *(int32_t*)(msg + 0xc) = why;
            hook::Global<int32_t>(0x6d40a8) = 2;
            hook::Global<int32_t>(0x6d4100) = 3;
            logs[side] = t.Run([&] {
                int32_t r = side ? rt3::Net_PlayerLeft(0, msg) : ((int32_t(__fastcall*)(int32_t, uint8_t*))0x522d20)(0, msg);
                Logf("r %d", r);
            });
            for (int k = 0; k < 3; ++k) logs[side] += " " + std::string((char*)players[k] + 0x78) + "/" + std::to_string(*(int32_t*)(players[k] + 0x4c));
            logs[side] += " " + std::to_string(hook::Global<int32_t>(0x6d40a8));
        }
        CompareLogs(logs[0], logs[1], "player left", it);
    }
}

namespace rt3 {
void Block_FinishSide();
void __stdcall Obj_SyncGroupFlags(uint8_t* o, int32_t full);
void __fastcall Obj_SwapBlockFlags(void*, int, uint8_t* o);
}

namespace {
void __stdcall RecB25Sync(uint8_t* o, int32_t full) { Logf("sync %p %d", o, full); }
void __fastcall RecB25Swap(void*, int, uint8_t* o) { Logf("swap %p", o); }
uint32_t __fastcall RecB25Vc(uint8_t* o) { Logf("vc"); return 1; }
uint32_t __fastcall RecB25V10(uint8_t* o) { Logf("v10"); return 2; }
int32_t CallB25(uint32_t fn, uint8_t* o, int32_t side) {
    int32_t r;
    __asm {
        push esi
        push side
        mov esi, o
        call fn
        pop esi
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(misc_BlockFinishSide) {
    SavedGlobals sg({ 0x6acd4c, 0x6acd50, 0x6acd54 });
    UiTrace t;
    t.Add(0x459e10, (void*)&rt3::Obj_SyncGroupFlags, (void*)&RecB25Sync);
    t.Add(0x45a000, (void*)&rt3::Obj_SwapBlockFlags, (void*)&RecB25Swap);
    static uint8_t ents[4][0x168], table[0x40], owner[8];
    static void* vt[8];
    vt[3] = (void*)&RecB25Vc;
    vt[4] = (void*)&RecB25V10;
    *(void***)owner = vt;
    hook::Global<uint8_t*>(0x6acd4c) = ents[0];
    hook::Global<uint8_t*>(0x6acd50) = table;
    hook::Global<uint8_t*>(0x6acd54) = owner;
    static const uint32_t bits[] = { 1, 2, 4, 0x10, 0x20, 0x40, 0x800, 0x1000, 0x2000, 0x4000, 0x10000 };
    for (int it = 0; it < 6000; ++it) {
        uint8_t init[4][0x168];
        for (int k = 0; k < 4; ++k) {
            memset(init[k], 0, 0x168);
            *(int32_t*)init[k] = (int32_t)(Rnd() % 3);
            uint32_t f = Rnd() % 5 ? 0x380 : (Rnd() % 8) << 7;
            for (uint32_t b : bits)
                if (Rnd() % 3 == 0) f |= b;
            *(uint32_t*)(init[k] + 0x8c) = f;
            for (int o = 0x28; o <= 0x3c; o += 4) *(int32_t*)(init[k] + o) = (int32_t)(Rnd() % 2);
            for (int o = 0x64; o <= 0x78; o += 4) *(int32_t*)(init[k] + o) = (int32_t)(Rnd() % 2);
            *(int32_t*)(init[k] + 0x80) = (int32_t)(Rnd() % 100);
            *(int32_t*)(init[k] + 0x84) = (int32_t)(Rnd() % 9) - 4;
            *(int32_t*)(init[k] + 0x88) = (int32_t)(Rnd() % 9) - 4;
            *(int32_t*)(init[k] + 0x90) = (int32_t)(Rnd() % 50);
            for (int a = 0; a < 4; ++a) *(int32_t*)(init[k] + 0x98 + a * 4) = (int32_t)(Rnd() % 4);
        }
        int which = (int)(Rnd() % 4);
        int32_t side = (int32_t)(Rnd() % 3);
        int32_t n = (int32_t)(Rnd() % 3);
        uint32_t tflags = Rnd() % 2 ? 0x800 : Rnd() % 2 ? 0x2000 : 0;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memcpy(ents, init, sizeof ents);
            memset(table, 0, sizeof table);
            *(int32_t*)(table + 8) = 4;
            *(int32_t*)(table + 0xc) = n;
            *(uint8_t**)(table + 0x10) = n == 2 ? ents[0] : owner;
            *(uint32_t*)(table + 0x14) = tflags;
            *(uint8_t**)(table + 0x1c) = owner;
            *(uint32_t*)(table + 0x20) = tflags ^ 0x2000;
            logs[s] = t.Run([&] {
                Logf("r %d", CallB25(s ? (uint32_t)(uintptr_t)&rt3::Block_FinishSide : 0x45a0d0, ents[which], side));
            });
            for (int k = 0; k < 4; ++k)
                for (int o = 0x28; o <= 0x98; o += 4) logs[s] += " " + std::to_string(*(int32_t*)(ents[k] + o));
            logs[s] += " " + std::to_string(*(uint32_t*)(table + 0x14)) + " " + std::to_string(*(uint32_t*)(table + 0x20));
        }
        CompareLogs(logs[0], logs[1], "block finish side", it);
    }
}

namespace rt3 {
void Shell_OpenCommand();
void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root);
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def);
uint32_t __fastcall HandleObj_Destruct(void* o);
}

namespace {
char g_b26Cmd[128];
bool g_b26Null;
void* __fastcall FakeB26Open(uint32_t* o, int, const char* key, void* root) {
    Logf("open [%s] %p", key, root);
    return o;
}
const char* __fastcall FakeB26Get(void* o, int, const char* sub, const char* name, const char* def) {
    Logf("get [%s] [%s] %p", sub, name, def);
    return g_b26Null ? nullptr : g_b26Cmd;
}
uint32_t __fastcall RecB26Close(void* o) {
    Logf("close");
    return 0;
}
char* CallB26(uint32_t fn, int32_t kind, const char* target) {
    char* r;
    __asm {
        push edi
        mov eax, kind
        mov edi, target
        call fn
        pop edi
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(misc_ShellOpenCommand) {
    static char saved[0x200];
    memcpy(saved, (void*)0x6d14d8, sizeof saved);
    UiTrace t;
    t.Add(0x4686f0, (void*)&rt3::RegKey_Construct, (void*)&FakeB26Open);
    t.Add(0x468a10, (void*)&rt3::RegKey_GetString, (void*)&FakeB26Get);
    t.Add(0x468740, (void*)&rt3::HandleObj_Destruct, (void*)&RecB26Close);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeIpFormat);
    static const char* cmds[] = { "\"C:\netscape\netscape.exe\" \"%1\"", "C:\NETSCAPE.exe -url %1", "iexplore.exe -nohome",
                                  "\"wordpad.exe\" \"%1\"", "write.exe \"%1", "netsc \"%1\" x", "plain" };
    for (int it = 0; it < 400; ++it) {
        int32_t kind = (int32_t)(Rnd() % 4) - 1;
        const char* pick = cmds[Rnd() % 7];
        g_b26Null = Rnd() % 6 == 0;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            strcpy(g_b26Cmd, pick);
            memset((void*)0x6d14d8, 0, 0x100);
            logs[s] = t.Run([&] {
                char* r = CallB26(s ? (uint32_t)(uintptr_t)&rt3::Shell_OpenCommand : 0x501f20, kind, "www.x.com");
                Logf("r %p [%s] [%s]", r, (char*)0x6d14d8, g_b26Cmd);
            });
        }
        CompareLogs(logs[0], logs[1], "shell open command", it);
    }
    memcpy((void*)0x6d14d8, saved, sizeof saved);
}

namespace rt3 {
void __fastcall Session_Close(uint8_t* s);
void CloseFile6ce994();
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Nop5217b0(void*);
int32_t Net_LockQueue();
void Net_UnlockQueue();
void* Engine_FirstListEntry();
void __fastcall Net_DropOldMessages(uint8_t* s, int, uint8_t* e, int32_t minutes);
void Peer_Shutdown();
int32_t __fastcall Net_ShutdownShared(uint8_t* net);
void __fastcall Music_Shutdown(uint8_t* m);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Music_Close(uint8_t* m);
}

namespace {
int32_t g_b27Lock;
void* g_b27First;
void RecB27CloseFile() { Logf("close file"); }
void __fastcall RecB27Release(uint8_t* pack, int, void* obj) { Logf("release %p %p", pack, obj); }
void __fastcall RecB27Nop(void* o) { Logf("nop %p", o); }
int32_t FakeB27Lock() { Logf("lock"); return g_b27Lock; }
void RecB27Unlock() { Logf("unlock"); }
void* FakeB27First() { Logf("first"); return g_b27First; }
void __fastcall RecB27Drop(uint8_t* s, int, uint8_t* e, int32_t m) { Logf("drop %p %d", e, m); }
void RecB27Peer() { Logf("peer"); }
int32_t __fastcall RecB27Shared(uint8_t* n) { Logf("shared %p", n); return 0; }
void __fastcall RecB27Music(uint8_t* m) { Logf("music %p", m); }
void __fastcall RecB27PFree(uint8_t* s, int, uint8_t* p) { Logf("pfree %p", p); }
void __fastcall RecB27Pool(NodePool* p) { Logf("pool %p", p); }
int32_t __fastcall RecB27Each(HashMap* m, int, uint32_t k, rt3::HashVisit fn, void* c) { Logf("each %p %u %p %p", m, k, fn, c); return 0; }
void __fastcall RecB27Hash(HashMap* m) { Logf("hash %p", m); }
void __fastcall RecB27MClose(uint8_t* m) { Logf("mclose %p", m); }
void __cdecl RecB27Free(void* p) { Logf("crt free %p", p); }
} // namespace

DIFFTEST(net_SessionClose) {
    SavedGlobals sg({ 0x6cd91c, 0x6d40d0, 0x6d4020, 0x6cd8e0 });
    UiTrace t;
    t.Add(0x46d240, (void*)&rt3::CloseFile6ce994, (void*)&RecB27CloseFile);
    t.Add(0x53c000, (void*)&rt3::Pack_Release, (void*)&RecB27Release);
    t.Add(0x5217b0, (void*)&rt3::Nop5217b0, (void*)&RecB27Nop);
    t.Add(0x521680, (void*)&rt3::Net_LockQueue, (void*)&FakeB27Lock);
    t.Add(0x5216a0, (void*)&rt3::Net_UnlockQueue, (void*)&RecB27Unlock);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB27First);
    t.Add(0x46b910, (void*)&rt3::Net_DropOldMessages, (void*)&RecB27Drop);
    t.Add(0x469450, (void*)&rt3::Peer_Shutdown, (void*)&RecB27Peer);
    t.Add(0x523200, (void*)&rt3::Net_ShutdownShared, (void*)&RecB27Shared);
    t.Add(0x5579d0, (void*)&rt3::Music_Shutdown, (void*)&RecB27Music);
    t.Add(0x469890, (void*)&rt3::NetPacket_Free, (void*)&RecB27PFree);
    t.Add(0x53dcb0, (void*)&rt3::Pool_Free, (void*)&RecB27Pool);
    t.Add(0x53db20, (void*)&rt3::Hash_ForEach, (void*)&RecB27Each);
    t.Add(0x53dbe0, (void*)&rt3::Hash_DestroyThunk, (void*)&RecB27Hash);
    t.Add(0x557c70, (void*)&rt3::Music_Close, (void*)&RecB27MClose);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    t.Add(0x5a1145, nullptr, (void*)&RecB27Free);
    static uint8_t* s = new uint8_t[0x9900];
    static uint8_t bufs[16][16], queue[12], pkts[4][8];
    static const uint32_t offs[] = { 4, 0x9884, 0x9878, 0x8f20, 0x8f24, 0x9050, 0x8f3c, 0x8f40, 0x8f28 };
    for (int it = 0; it < 2000; ++it) {
        int32_t open = Rnd() % 6 != 0, host = (int32_t)(Rnd() % 3);
        bool net = Rnd() % 3 != 0, stream = Rnd() % 2, hasQ = Rnd() % 4 != 0;
        uint32_t mask = Rnd();
        int np = (int)(Rnd() % 4);
        g_b27Lock = (int32_t)(Rnd() % 2);
        bool first = Rnd() % 2;
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset(s, 0, 0x9900);
            *(int32_t*)s = open ? 0x1234 : 0;
            *(int32_t*)(s + 0xc) = 5;
            *(int32_t*)(s + 0x8f18) = host;
            *(int32_t*)(s + 0x8f30) = 6;
            *(int32_t*)(s + 0x9054) = 7;
            for (int k = 0; k < 9; ++k)
                if (mask >> k & 1) *(uint8_t**)(s + offs[k]) = bufs[k];
            for (int k = 0; k < 4; ++k) *(uint8_t**)pkts[k] = k + 1 < np ? pkts[k + 1] : nullptr;
            *(uint8_t**)queue = np ? pkts[0] : nullptr;
            *(uint8_t**)(queue + 4) = np ? pkts[np - 1] : nullptr;
            *(int32_t*)(queue + 8) = np;
            if (hasQ) *(uint8_t**)(s + 0x8f2c) = queue;
            hook::Global<void*>(0x6d40d0) = net ? bufs[10] : nullptr;
            hook::Global<void*>(0x6d4020) = bufs[11];
            hook::Global<void*>(0x6cd8e0) = stream ? bufs[12] : nullptr;
            hook::Global<int32_t>(0x6cd91c) = 9;
            g_b27First = first ? bufs[13] : nullptr;
            logs[side] = t.Run([&] {
                if (side) rt3::Session_Close(s);
                else ((void(__fastcall*)(uint8_t*))0x46bc40)(s);
            });
            for (uint32_t o : { 0u, 4u, 0xcu, 0x8f18u, 0x8f20u, 0x8f24u, 0x8f28u, 0x8f2cu, 0x8f30u, 0x8f3cu, 0x8f40u, 0x9050u, 0x9054u, 0x9878u, 0x9884u })
                logs[side] += " " + std::to_string(*(int32_t*)(s + o));
            logs[side] += " " + std::to_string(*(int32_t*)(queue + 8)) + " " + std::to_string(hook::Global<int32_t>(0x6cd91c)) + " " +
                          std::to_string((int)(hook::Global<void*>(0x6cd8e0) != nullptr));
        }
        CompareLogs(logs[0], logs[1], "session close", it);
    }
}

namespace rt3 {
void Models_AddCopy();
}

namespace {
uint8_t g_b28Recs[8][0xa4], g_b28Pool[0x14], g_b28List[0xc];
int32_t CallB28(uint32_t fn, int32_t slot, int32_t key, const char* name, int32_t* ecxOut) {
    int32_t r, c;
    __asm {
        push name
        mov ecx, slot
        mov edx, key
        call fn
        mov r, eax
        mov c, ecx
    }
    *ecxOut = c;
    return r;
}
std::string B28Node(uint8_t* n) {
    for (int k = 0; k < 8; ++k)
        if (n == g_b28Recs[k]) return "r" + std::to_string(k);
    char b[96];
    sprintf(b, "new[%s %d %d %d %d %d %d]", (char*)n + 0x10, *(int32_t*)(n + 0xc), *(int32_t*)(n + 0x94), *(int32_t*)(n + 0x98),
            *(int32_t*)(n + 0x9c), *(int32_t*)(n + 4), *(int32_t*)(n + 8));
    return b;
}
std::string B28Bucket(uint32_t b) {
    std::string s = "{";
    for (uint8_t* n = *(uint8_t**)b; n; n = *(uint8_t**)n) s += B28Node(n) + "/" + std::to_string(*(int32_t*)(n + 0xc)) + " ";
    uint8_t* tail = *(uint8_t**)(b + 4);
    s += (tail ? B28Node(tail) : std::string("-")) + " " + std::to_string(*(int32_t*)(b + 8)) + "}";
    return s;
}
} // namespace

DIFFTEST(misc_ModelsAddCopy) {
    SavedGlobals sg({ 0x6acd6c });
    static uint8_t savedBuckets[0x6ad96c + 0xc - 0x6acd78];
    memcpy(savedBuckets, (void*)0x6acd78, sizeof savedBuckets);
    UiTrace t;
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    static const char* names[] = { "loco.3dp", "Car.3dp", "tree", "HOUSE" };
    static const int32_t slots[] = { 112, 113, 114, 0xff, 5 };
    for (int it = 0; it < 3000; ++it) {
        int32_t slot = slots[Rnd() % 5];
        int32_t key = Rnd() % 4 == 0 ? 6 : 1 << (Rnd() % 4);
        const char* name = Rnd() % 6 == 0 ? "none" : names[Rnd() % 4];
        int32_t enabled = (int32_t)(Rnd() % 3 != 0);
        int nfree = (int)(Rnd() % 3);
        uint32_t place[6];
        int32_t keys[6];
        for (int k = 0; k < 6; ++k) {
            place[k] = Rnd() % 5;
            keys[k] = 1 << (Rnd() % 4);
        }
        std::string logs[2];
        for (int side = 0; side < 2; ++side) {
            memset((void*)0x6acd78, 0, sizeof savedBuckets);
            for (int k = 0; k < 8; ++k) memset(g_b28Recs[k], 0, 0xa4);
            for (int k = 0; k < 6; ++k) {
                uint8_t* r = g_b28Recs[k];
                strcpy((char*)r + 0x10, names[k % 4]);
                *(uint32_t*)(r + 0x90) = rt3::Crc32String(names[k % 4], 1);
                *(int32_t*)(r + 4) = k;
                *(int32_t*)(r + 8) = k * 3;
                *(int32_t*)(r + 0x98) = k * 5;
                *(int32_t*)(r + 0x9c) = k * 7;
                *(int32_t*)(r + 0xa0) = k * 9;
                *(int32_t*)(r + 0x94) = keys[k];
                int32_t sl = slots[place[k]];
                *(int32_t*)(r + 0xc) = sl;
                uint32_t b = 0x6acd78 + sl * 0xc;
                *(uint8_t**)r = *(uint8_t**)b;
                *(uint8_t**)b = r;
                if (!*(uint8_t**)(b + 4)) *(uint8_t**)(b + 4) = r;
                ++*(int32_t*)(b + 8);
            }
            memset(g_b28Pool, 0, sizeof g_b28Pool);
            *(uint8_t**)g_b28Pool = g_b28List;
            *(uint8_t**)(g_b28Pool + 4) = g_b28Recs[0];
            *(uint8_t**)(g_b28Pool + 8) = g_b28Recs[7] + 0xa4;
            *(int32_t*)(g_b28Pool + 0xc) = enabled;
            *(int32_t*)(g_b28Pool + 0x10) = 0xa4;
            *(uint8_t**)g_b28Recs[6] = nfree > 1 ? g_b28Recs[7] : nullptr;
            *(uint8_t**)g_b28List = nfree ? g_b28Recs[6] : nullptr;
            *(uint8_t**)(g_b28List + 4) = nfree ? g_b28Recs[5 + nfree] : nullptr;
            *(int32_t*)(g_b28List + 8) = nfree;
            hook::Global<uint8_t*>(0x6acd6c) = g_b28Pool;
            if (!enabled || nfree) {
                logs[side] = t.Run([&] {
                    int32_t c;
                    int32_t r = CallB28(side ? (uint32_t)(uintptr_t)&rt3::Models_AddCopy : 0x45ef00, slot, key, name, &c);
                    Logf("r %d ecx %d", r, c);
                });
            }
            for (int32_t sl : slots) logs[side] += " " + B28Bucket(0x6acd78 + sl * 0xc);
            logs[side] += " pool " + B28Bucket((uint32_t)(uintptr_t)g_b28List);
        }
        CompareLogs(logs[0], logs[1], "models add copy", it);
    }
    memcpy((void*)0x6acd78, savedBuckets, sizeof savedBuckets);
}

namespace rt3 {
void Block_CountSide();
void __fastcall Session_Shutdown(uint8_t* s);
void Net_DeleteLocks();
void __fastcall Music_Destroy(uint8_t* m);
void __fastcall Train_PassSignal(uint8_t* t);
void __fastcall Vehicle_PassSignal(uint8_t* o);
}

namespace {
float CallB29(uint32_t fn, uint8_t* c, uint8_t* e, uint32_t mask, uint32_t* res, float v, int32_t side) {
    float r;
    __asm {
        push edi
        push side
        push v
        mov eax, c
        mov edx, e
        mov ecx, mask
        mov edi, res
        call fn
        fstp r
        pop edi
    }
    return r;
}
void __fastcall RecB29Close(uint8_t* s) { Logf("close %p", s); }
void RecB29Locks() { Logf("locks"); }
void __fastcall RecB29Music(uint8_t* m) { Logf("music destroy %p", m); }
void __fastcall RecB29Pass(uint8_t* o) { Logf("pass %p", o); }
void __fastcall RecB29Leave(uint8_t* t) { Logf("leave %p", t); }
} // namespace

DIFFTEST(misc_CountSideShutdownPass) {
    SavedGlobals sg({ 0x6acd4c, 0x6acd50, 0x6acd54, 0x6d40d0, 0xd973b4 });
    UiTrace t;
    t.Add(0x459e10, (void*)&rt3::Obj_SyncGroupFlags, (void*)&RecB25Sync);
    t.Add(0x45a000, (void*)&rt3::Obj_SwapBlockFlags, (void*)&RecB25Swap);
    t.Add(0x46bc40, (void*)&rt3::Session_Close, (void*)&RecB29Close);
    t.Add(0x521590, (void*)&rt3::Net_DeleteLocks, (void*)&RecB29Locks);
    t.Add(0x557b70, (void*)&rt3::Music_Destroy, (void*)&RecB29Music);
    t.Add(0x4aad20, (void*)&rt3::Vehicle_PassSignal, (void*)&RecB29Pass);
    t.Add(0x4aac00, (void*)&rt3::Train_LeaveBlock, (void*)&RecB29Leave);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    static uint8_t ent[0x168], table[0x40], cnt[0x40], bufs[2][8], train[0x200];
    hook::Global<uint8_t*>(0x6acd4c) = ent;
    hook::Global<uint8_t*>(0x6acd50) = table;
    for (int it = 0; it < 3000; ++it) {
        uint32_t flags = Rnd() % 2 ? 0x380 : Rnd() & 0x3ff, mask = 0x80u << (Rnd() % 3);
        int32_t a = (int32_t)(Rnd() % 4), b = (int32_t)(Rnd() % 4), side = (int32_t)(Rnd() % 2);
        bool hasC = Rnd() % 5 != 0;
        float v = RndF(-5.0f, 5.0f);
        uint32_t res0 = Rnd() % 4;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(ent, 0, sizeof ent);
            memset(table, 0, sizeof table);
            *(uint32_t*)(ent + 0x8c) = flags;
            *(int32_t*)(ent + 0x90) = 3;
            *(int32_t*)(cnt + 0x24) = a;
            *(int32_t*)(cnt + 0x30) = b;
            uint32_t res = res0;
            logs[s] = t.Run([&] {
                float r = CallB29(s ? (uint32_t)(uintptr_t)&rt3::Block_CountSide : 0x45a280, hasC ? cnt : nullptr, ent, mask, &res, v, side);
                Logf("r %08x res %u", B(r), res);
            });
            logs[s] += " " + std::to_string(*(uint32_t*)(ent + 0x8c)) + " " + std::to_string(*(int32_t*)(cnt + 0x24)) + " " +
                       std::to_string(*(int32_t*)(ent + 0x94));
        }
        CompareLogs(logs[0], logs[1], "block count side", it);
    }
    for (int it = 0; it < 200; ++it) {
        bool n = Rnd() % 2, m = Rnd() % 2;
        int32_t st = (int32_t)(Rnd() % 7);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<void*>(0x6d40d0) = n ? bufs[0] : nullptr;
            hook::Global<void*>(0xd973b4) = m ? bufs[1] : nullptr;
            memset(train, 0, sizeof train);
            *(int32_t*)(train + 0xe8) = st;
            *(int32_t*)(train + 0x94) = 77;
            logs[s] = t.Run([&] {
                if (s) rt3::Session_Shutdown(train);
                else ((void(__fastcall*)(uint8_t*))0x46c230)(train);
                if (s) rt3::Train_PassSignal(train);
                else ((void(__fastcall*)(uint8_t*))0x4aae90)(train);
            });
            logs[s] += " " + std::to_string((int)(hook::Global<void*>(0x6d40d0) != nullptr)) + std::to_string((int)(hook::Global<void*>(0xd973b4) != nullptr)) +
                       " " + std::to_string(*(int32_t*)(train + 0x8c)) + " " + std::to_string(*(int32_t*)(train + 0x94)) + " " +
                       std::to_string(*(int32_t*)(train + 0x1ba));
        }
        CompareLogs(logs[0], logs[1], "shutdown / pass signal", it);
    }
}

namespace rt3 {
void __fastcall World_MarkBuildingTypes(uint8_t* w);
void World_MarkKind2Types();
void __fastcall ContainerList_Save(Collection* c, int, Stream s, int32_t recordSize);
uint8_t* __fastcall SetupWindow_Construct(uint8_t* w);
uint8_t __fastcall Obj_Flag3d4(void* o);
void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize);
void __fastcall Container_Save(uint8_t* o, int, int32_t s);
void __fastcall SetupWindow_FillLabels(uint8_t* w);
}

namespace {
uint8_t g_b30Objs[6][0x400], g_b30Types[6][0xc0];
uint8_t* __fastcall FakeB30Type(uint8_t* o) { return g_b30Types[(o - g_b30Objs[0]) / 0x400]; }
bool g_b30TypeMode;
void* __fastcall FakeB30Get(Collection* c, int, uint32_t id) {
    Logf("get %u", id);
    return g_b30TypeMode ? (void*)g_b30Types[id % 6] : (void*)g_b30Objs[id % 6];
}
void __fastcall RecB30Set(uint8_t* w, int, const char* name, int32_t v) { Logf("set %p [%s] %d", w, name, v); }
void __fastcall RecB30CSave(Collection* c, int, int32_t s, int32_t n) { Logf("csave %p %d %d", c, s, n); }
void __fastcall RecB30Save(uint8_t* o, int, int32_t s) { Logf("save %d %d", (int)((o - g_b30Objs[0]) / 0x400), s); }
void __fastcall RecB30Fill(uint8_t* w) { Logf("fill %p", w); }
uint8_t* __fastcall RecB30Vtbl(uint8_t* w) { Logf("base %p", w); return w; }
} // namespace

DIFFTEST(misc_MarkTypesSaveSetup) {
    SavedGlobals sg({ 0x62b26c, 0x62b268, 0x6cec78, 0x6d0820 });
    UiTrace t;
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB18Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB18IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB30Get);
    t.Add(0x434f20, (void*)&rt3::World_SetNamedValue, (void*)&RecB30Set);
    t.Add(0x517d90, (void*)&rt3::Collection_Save, (void*)&RecB30CSave);
    t.Add(0x48a6c0, (void*)&rt3::Container_Save, (void*)&RecB30Save);
    t.Add(0x531340, (void*)&rt3::StreamWriteU32, (void*)&RecB9WriteU32);
    t.Add(0x531030, (void*)&rt3::StreamWrite, (void*)&RecB9Write);
    t.Add(0x53f7e0, (void*)&rt3::Widget_SetVtblRet, (void*)&RecB30Vtbl);
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&RecLoad);
    t.Add(0x4e0960, (void*)&rt3::SetupWindow_FillLabels, (void*)&RecB30Fill);
    static void* vt[0x40];
    vt[0x80 / 4] = (void*)&FakeB30Type;
    static uint8_t coll[0x100], world[8], win[0x100];
    hook::Global<uint8_t*>(0x62b26c) = coll;
    hook::Global<uint8_t*>(0x6cec78) = world;
    for (int it = 0; it < 1500; ++it) {
        g_b18Count = (int32_t)(Rnd() % 8) - 1;
        for (int k = 0; k < 6; ++k) {
            *(void***)g_b30Objs[k] = vt;
            g_b30Objs[k][0x3d4] = (uint8_t)(Rnd() % 3);
            g_b30Types[k][0x32] = (uint8_t)(Rnd() % 3);
            g_b30Types[k][0xba] = (uint8_t)(Rnd() % 4 == 0);
            g_b30Types[k][0xbb] = (uint8_t)(Rnd() % 4 == 0);
            sprintf((char*)g_b30Types[k] + 4, "type%d", k);
        }
        *(int32_t*)(coll + 0x90) = (int32_t)Rnd();
        int32_t sn = (int32_t)(Rnd() % 10), rs = (int32_t)(Rnd() % 500);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            logs[s] = t.Run([&] {
                if (s) rt3::World_MarkBuildingTypes(world);
                else ((void(__fastcall*)(uint8_t*))0x436a70)(world);
                if (s) rt3::ContainerList_Save((Collection*)coll, 0, sn, rs);
                else ((void(__fastcall*)(void*, int, int32_t, int32_t))0x491c60)(coll, 0, sn, rs);
            });
        }
        CompareLogs(logs[0], logs[1], "mark building types / container list save", it);
        g_b30TypeMode = true;
        for (int s = 0; s < 2; ++s) {
            logs[s] = t.Run([&] {
                if (s) rt3::World_MarkKind2Types();
                else ((void (*)())0x437740)();
            });
        }
        CompareLogs(logs[0], logs[1], "mark kind 2 types", it);
        g_b30TypeMode = false;
    }
    for (int s = 0; s < 2; ++s) {
        std::string l = t.Run([&] {
            uint8_t* r = s ? rt3::SetupWindow_Construct(win) : ((uint8_t * (__fastcall*)(uint8_t*))0x4e0b20)(win);
            Logf("r %p %08x %p %d", r, *(uint32_t*)win, hook::Global<void*>(0x6d0820), *(int32_t*)(win + 0x78));
        });
        static std::string first;
        if (!s) first = l;
        else CompareLogs(first, l, "setup window construct", 0);
    }
}

namespace rt3 {
void SetupWindow_Open();
void __fastcall Session_Restart(uint8_t* w, int, int32_t host);
uint8_t* __fastcall Session_Init(uint8_t* s);
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Obj_Incc60(uint8_t* o);
void __fastcall Thread_Start(uint8_t* t);
}

namespace {
uint8_t* g_b31Child;
uint8_t* __fastcall RecB31Construct(uint8_t* w) { Logf("construct"); return w + 1; }
void __fastcall RecB31Insert(uint8_t** L, int, uint8_t* w, int32_t sel) { Logf("insert %p %d", L, sel); }
void __fastcall RecB31Shutdown(uint8_t* s) { Logf("shutdown %p", s); }
uint8_t* __fastcall RecB31Init(uint8_t* s) { Logf("init"); return s; }
uint8_t* __fastcall FakeB31Find(uint8_t* w, int, int32_t id) { Logf("find %x", id); return g_b31Child; }
void __fastcall RecB31Cursor(uint8_t* c, int, int32_t i, int32_t f, int32_t n) { Logf("cursor %d %d %d", i, f, n); }
void __fastcall RecB31Inc(uint8_t* o) { Logf("inc %p", o); }
void __fastcall RecB31Thread(uint8_t* t) { Logf("thread %08x %d %d %d", *(uint32_t*)t, ((int32_t*)t)[1], ((int32_t*)t)[2], ((int32_t*)t)[3]); }
} // namespace

DIFFTEST(net_SetupOpenSessionRestart) {
    SavedGlobals sg({ 0x6d0820, 0x6d401c, 0x6cd8d8, 0x6d11a8, 0xccbb20, 0x6d1274, 0x6d1278, 0x6d127c, 0x6d1280, 0x6d1288, 0x6d128c, 0x6cd8dc,
                      0x6d1294 });
    UiTrace t;
    t.Add(0x4e0b20, (void*)&rt3::SetupWindow_Construct, (void*)&RecB31Construct);
    t.Add(0x538e50, (void*)&rt3::WidgetList_Insert, (void*)&RecB31Insert);
    t.Add(0x46c230, (void*)&rt3::Session_Shutdown, (void*)&RecB31Shutdown);
    t.Add(0x4ed800, (void*)&rt3::Session_Init, (void*)&RecB31Init);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB31Find);
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&RecB31Cursor);
    t.Add(0x538a30, (void*)&rt3::Obj_Incc60, (void*)&RecB31Inc);
    t.Add(0x563e30, (void*)&rt3::Thread_Start, (void*)&RecB31Thread);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    static uint8_t old[8], child[0x100], win[8];
    static char text[8];
    for (int it = 0; it < 200; ++it) {
        bool open = Rnd() % 2, hasOld = Rnd() % 2, hasChild = Rnd() % 3 != 0, hasText = Rnd() % 2;
        int32_t host = (int32_t)(Rnd() % 3);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<void*>(0x6d0820) = open ? win : nullptr;
            hook::Global<void*>(0x6d401c) = win;
            hook::Global<void*>(0x6cd8d8) = hasOld ? old : nullptr;
            hook::Global<void*>(0x6d1294) = nullptr;
            hook::Global<int32_t>(0x6cd8dc) = 5;
            hook::Global<char>(0x6d11a8) = 'x';
            strcpy(text, "abc");
            *(char**)(child + 0xba) = hasText ? text : nullptr;
            g_b31Child = hasChild ? child : nullptr;
            logs[s] = t.Run([&] {
                if (s) rt3::SetupWindow_Open();
                else ((void (*)())0x4e0e40)();
                if (s) rt3::Session_Restart(win, 0, host);
                else ((void(__fastcall*)(uint8_t*, int, int32_t))0x4edc40)(win, 0, host);
            });
            char tail[96];
            sprintf(tail, " %d %d [%s] %d %d %d", (int)(hook::Global<void*>(0x6d0820) != nullptr), (int)(hook::Global<void*>(0x6cd8d8) != nullptr), text,
                    hook::Global<char>(0x6d11a8), hook::Global<int32_t>(0x6d127c), hook::Global<int32_t>(0x6cd8dc));
            logs[s] += tail;
        }
        CompareLogs(logs[0], logs[1], "setup open / session restart", it);
    }
}

namespace rt3 {
int32_t __fastcall Lzss_Pack(int32_t(__fastcall* read)(uint8_t*, int32_t), int32_t(__fastcall* write)(uint8_t*, int32_t));
}

DIFFTEST(save_LzssPack) {
    SavedGlobals sg({ 0xd9fdc8, 0xda7dd0, 0xda7dd4, 0xda7dd8 });
    static uint8_t savedIn[0x8010], savedOut[0x8010];
    memcpy(savedIn, (void*)0xd9fdd0, sizeof savedIn);
    memcpy(savedOut, (void*)0xd97dc8, sizeof savedOut);
    typedef int32_t(__fastcall * Fn)(void*, void*);
    for (int it = 0; it < 120; ++it) {
        g_lzIn.clear();
        size_t n = Rnd() % 0x12000;
        int alpha = 1 + (int)(Rnd() % 255);
        while (g_lzIn.size() < n) {
            if (Rnd() % 3 == 0 && g_lzIn.size() > 10) {
                size_t back = 1 + Rnd() % std::min<size_t>(g_lzIn.size(), 600), len = 1 + Rnd() % 80;
                for (size_t k = 0; k < len; ++k) g_lzIn.push_back(g_lzIn[g_lzIn.size() - back]);
            } else g_lzIn.push_back((uint8_t)(Rnd() % alpha));
        }
        size_t chunk = Rnd() % 3 == 0 ? 0x8000 : 1 + Rnd() % 0x3000;
        int32_t r[2];
        std::vector<uint8_t> out[2];
        for (int side = 0; side < 2; ++side) {
            g_lzPos = 0;
            g_lzChunk = chunk;
            g_rleOut.clear();
            hook::Global<int32_t>(0xd9fdc8) = 0;
            r[side] = side ? rt3::Lzss_Pack(&FakeLzRead, &FakeRleWrite) : ((Fn)0x56bfe0)((void*)&FakeLzRead, (void*)&FakeRleWrite);
            out[side] = g_rleOut;
        }
        EXPECT_EQ(r[0], r[1], "lzss pack result %d", it);
        EXPECT_EQ(out[0] == out[1], true, "lzss pack output %d (%u vs %u bytes)", it, (unsigned)out[0].size(), (unsigned)out[1].size());
    }
    EXPECT_EQ(rt3::Lzss_Pack(nullptr, &FakeRleWrite), ((Fn)0x56bfe0)(nullptr, (void*)&FakeRleWrite), "lzss pack null");
    memcpy((void*)0xd9fdd0, savedIn, sizeof savedIn);
    memcpy((void*)0xd97dc8, savedOut, sizeof savedOut);
}

namespace rt3 {
void __fastcall GameList_Fill(uint8_t* w);
uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby);
uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i);
}

namespace {
uint8_t g_b33List[0x200], g_b33Status[0x100];
uint8_t* g_b33Games;
char g_b33Sel[32];
uint8_t* __fastcall FakeB33Find(uint8_t* w, int, int32_t id) {
    Logf("find %x", id);
    return id == 0xe47 ? g_b33Status : g_b33List;
}
uint32_t __fastcall FakeB33Node(uint8_t* w, int, int32_t i) {
    Logf("node %d", i);
    return (uint32_t)(uintptr_t)g_b33Sel;
}
uint8_t* __fastcall FakeB33Fresh(uint8_t* l) {
    Logf("fresh %p", l);
    return g_b33Games;
}
} // namespace

DIFFTEST(net_GameListFill) {
    SavedGlobals sg({ 0x6cd920, 0x6d1290, 0x6cd8d8 });
    UiTrace t;
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB33Find);
    t.Add(0x55fca0, (void*)&rt3::TreeView_NodeValue, (void*)&FakeB33Node);
    t.Add(0x46a1a0, (void*)&rt3::Lobby_FreshList, (void*)&FakeB33Fresh);
    static uint8_t win[0x100], sess[8];
    static char text[8];
    g_b33Games = new uint8_t[0x11c * 0x80 + 0x20];
    static const char* names[] = { "Alpha", "beta", "GAMMA", "alpha", "Delta" };
    for (int it = 0; it < 1500; ++it) {
        int32_t conn = Rnd() % 6 == 0, busy = Rnd() % 6 == 0;
        int32_t page = Rnd() % 2 ? 0xe13 : 0xe14;
        int32_t sel = (int32_t)(Rnd() % 4) - 2;
        const char* selName = names[Rnd() % 5];
        bool blank = Rnd() % 4 != 0;
        int n = (int)(Rnd() % 6);
        int pick[6];
        for (int& p : pick) p = (int)(Rnd() % 5);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<int32_t>(0x6cd920) = conn;
            hook::Global<char>(0x6d1290) = (char)busy;
            hook::Global<void*>(0x6cd8d8) = sess;
            *(int32_t*)(win + 0x7c) = page;
            *(int32_t*)(g_b33List + 0x1d6) = sel;
            strcpy(g_b33Sel, selName);
            strcpy(text, blank ? "  " : "x");
            *(char**)(g_b33Status + 0xba) = text;
            memset(g_b33Games, 0, 0x11c * 0x80 + 0x20);
            for (int k = 0; k < n; ++k) {
                strcpy((char*)g_b33Games + k * 0x11c + 4, names[pick[k]]);
                *(int32_t*)(g_b33Games + k * 0x11c + 0x104) = 1;
            }
            logs[s] = t.Run([&] {
                if (s) rt3::GameList_Fill(win);
                else ((void(__fastcall*)(uint8_t*))0x4ee6a0)(win);
            });
            logs[s] += " " + std::to_string(hook::Global<char>(0x6d1290));
        }
        CompareLogs(logs[0], logs[1], "game list fill", it);
    }
}

namespace rt3 {
void __fastcall Lobby_AddPlayerButton(int32_t, int32_t slot, int32_t key, float x, float y, float w, float h);
uint8_t* __stdcall Textures_Find(int32_t key);
void __stdcall Net_UnlockQueueArg(int32_t);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
}

namespace {
uint8_t g_b34Player[0x100];
bool g_b34Found;
uint8_t* __stdcall FakeB34Find(int32_t k) {
    Logf("find %d", k);
    return g_b34Found ? g_b34Player : nullptr;
}
void __stdcall RecB34Unlock(int32_t k) { Logf("unlock %d", k); }
uint8_t* __fastcall RecB34Button(uint8_t* w, int, const uint8_t* d) {
    std::string s = "button";
    char b[8];
    for (int k = 0; k < 0xa1; ++k) {
        sprintf(b, " %02x", d[k]);
        s += b;
    }
    Logf("%s", s.c_str());
    return w;
}
void __cdecl RecB34Free(void* p) { Logf("crt free"); }
} // namespace

DIFFTEST(net_LobbyAddPlayerButton) {
    SavedGlobals sg({ 0x6d40d0, 0x6d1268 });
    UiTrace t;
    t.Add(0x521d60, (void*)&rt3::Textures_Find, (void*)&FakeB34Find);
    t.Add(0x5216d0, (void*)&rt3::Net_UnlockQueueArg, (void*)&RecB34Unlock);
    t.Add(0x563d70, (void*)&rt3::Button_Construct, (void*)&RecB34Button);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    t.Add(0x5a1145, nullptr, (void*)&RecB34Free);
    static uint8_t net[8], panel[0x100];
    hook::Global<uint8_t*>(0x6d1268) = panel;
    static const int32_t pages[] = { 0xe15, 0xe16, 0xe17, 0 };
    for (int it = 0; it < 2000; ++it) {
        bool hasNet = Rnd() % 5 != 0;
        g_b34Found = Rnd() % 4 != 0;
        *(uint32_t*)(g_b34Player + 0x5c) = Rnd() % 4;
        *(int32_t*)(panel + 0x78) = pages[Rnd() % 4];
        int32_t slot = (int32_t)(Rnd() % 16), key = (int32_t)Rnd();
        float x = RndF(-2.0f, 2.0f), y = RndF(-2.0f, 2.0f), w = RndF(0.0f, 1.0f);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<uint8_t*>(0x6d40d0) = hasNet ? net : nullptr;
            logs[s] = t.Run([&] {
                if (s) rt3::Lobby_AddPlayerButton(0, slot, key, x, y, w, 0.5f);
                else ((void(__fastcall*)(int32_t, int32_t, int32_t, float, float, float, float))0x4edd00)(0, slot, key, x, y, w, 0.5f);
            });
        }
        CompareLogs(logs[0], logs[1], "lobby add player button", it);
    }
}

namespace rt3 {
int32_t __fastcall Net_Connect(uint8_t* o, int, const char* host, const char* name, const char* password, int32_t ms);
int32_t __fastcall Net_InitShared(uint8_t* net);
int32_t __fastcall Net_SetPlayerName(uint8_t* o, int, const char* name);
int32_t __stdcall DpAddress_AddString(void* addr, const char* text);
int32_t __fastcall Sound_SetBufferMs(uint8_t* e, int, void* o, int32_t ms);
uint32_t* __stdcall Net_GetGuid(uint32_t* out);
wchar_t* __fastcall Str_ToWide(wchar_t* dst, const char* src, int32_t n);
}

namespace {
int32_t g_b35Ok[4], g_b35Hr;
void* g_b35Host;
int32_t __fastcall RecB35Init(uint8_t* n) { Logf("init"); return 1; }
int32_t __fastcall FakeB35Name(uint8_t* o, int, const char* name) { Logf("name [%s]", name); return g_b35Ok[0]; }
int32_t __stdcall FakeB35Addr(void* a, const char* t) { Logf("addr %p [%s]", a, t); return g_b35Ok[1]; }
int32_t __fastcall FakeB35Ms(uint8_t* e, int, void* o, int32_t ms) {
    Logf("ms %p %d", o, ms);
    return o == g_b35Host ? g_b35Ok[3] : g_b35Ok[2];
}
uint32_t* __stdcall FakeB35Guid(uint32_t* out) {
    static uint32_t g[4] = { 0x11111111, 0x22222222, 0x33333333, 0x44444444 };
    Logf("guid");
    return g;
}
wchar_t* __fastcall FakeB35Wide(wchar_t* d, const char* s, int32_t n) {
    Logf("wide [%s] %d", s, n);
    d[0] = 'P';
    d[1] = 0;
    return d;
}
int32_t __stdcall FakeB35Caps(void* o, const void* g, int32_t* v, int32_t n, int32_t f) {
    Logf("caps %p %p %d %d %d", o, g, *v, n, f);
    return 0;
}
int32_t __stdcall FakeB35Connect(void* p, uint32_t* desc, void* host, void* dev, void* sec, void* cred, const char* user, uint32_t n, void* ctx,
                                 void* actx, uint32_t* h, uint32_t flags) {
    std::string s;
    for (int k = 0; k < 0x12; ++k) s += " " + std::to_string(k == 0xd ? (desc[k] ? *(wchar_t*)(uintptr_t)desc[k] : 0) : desc[k]);
    Logf("connect%s | %p %p %p %p [%s] %u %p %p %u", s.c_str(), host, dev, sec, cred, user, n, ctx, actx, flags);
    *h = 0x777;
    return g_b35Hr;
}
} // namespace

DIFFTEST(net_Connect) {
    SavedGlobals sg({ 0x6d408c, 0x6d40dc });
    UiTrace t;
    t.Add(0x5236a0, (void*)&rt3::Net_InitShared, (void*)&RecB35Init);
    t.Add(0x521c80, (void*)&rt3::Net_SetPlayerName, (void*)&FakeB35Name);
    t.Add(0x5215c0, (void*)&rt3::DpAddress_AddString, (void*)&FakeB35Addr);
    t.Add(0x521620, (void*)&rt3::Sound_SetBufferMs, (void*)&FakeB35Ms);
    t.Add(0x521aa0, (void*)&rt3::Net_GetGuid, (void*)&FakeB35Guid);
    t.Add(0x557930, (void*)&rt3::Str_ToWide, (void*)&FakeB35Wide);
    t.Add(0x522240, (void*)&rt3::CmdQueue_Broadcast, (void*)&FakeB24Bcast);
    static void* vt[0x20];
    vt[0x48 / 4] = (void*)&FakeB35Caps;
    vt[0x18 / 4] = (void*)&FakeB35Connect;
    static uint8_t objs[3][0x20] = {}, dp[0x20], o[0x300], list[8];
    alignas(16) static uint8_t devs[0x40];
    for (auto& x : objs) *(void***)x = vt;
    *(void**)(dp + 8) = objs[0];
    *(void**)(dp + 0xc) = devs;
    *(void**)(dp + 0x10) = devs + 0x10;
    *(void***)devs = vt;
    *(void***)(devs + 0x10) = vt;
    g_b35Host = devs + 0x10;
    *(uint8_t**)(o + 0x18e) = dp;
    for (int it = 0; it < 1500; ++it) {
        for (auto& k : g_b35Ok) k = Rnd() % 6 != 0;
        g_b35Hr = Rnd() % 3 == 0 ? (int32_t)0x80004005 : (int32_t)(Rnd() % 2);
        bool hasList = Rnd() % 8 != 0, busy = Rnd() % 8 == 0, pw = Rnd() % 2;
        int32_t ms = (int32_t)(Rnd() % 20000);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<void*>(0x6d408c) = hasList ? list : nullptr;
            hook::Global<int32_t>(0x6d40dc) = busy;
            memset(o + 0x21a, 0, 0x40);
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Net_Connect(o, 0, "1.2.3.4", "Bob", pw ? "secret" : nullptr, ms)
                              : ((int32_t(__fastcall*)(uint8_t*, int, const char*, const char*, const char*, int32_t))0x523af0)(
                                    o, 0, "1.2.3.4", "Bob", pw ? "secret" : nullptr, ms);
                Logf("r %08x [%s]", r, (char*)o + 0x21a);
            });
        }
        CompareLogs(logs[0], logs[1], "net connect", it);
    }
}

namespace rt3 {
int32_t __fastcall Net_Host(uint8_t* o, int, const char* game, const char* name, const char* password, int32_t ms, int32_t maxPlayers);
}

namespace {
int32_t __stdcall FakeB35Host(void* p, uint32_t* desc, void** devs, uint32_t n, void* a, void* b, void* c, uint32_t flags) {
    std::string s;
    for (int k = 0; k < 0x12; ++k) {
        uint32_t v = desc[k];
        if ((k == 0xc || k == 0xd) && v) v = *(wchar_t*)(uintptr_t)v;
        s += " " + std::to_string(v);
    }
    Logf("host%s | %p %u %p %p %p %u", s.c_str(), *devs, n, a, b, c, flags);
    return g_b35Hr;
}
} // namespace

DIFFTEST(net_Host) {
    SavedGlobals sg({ 0x6d408c, 0x6d40dc });
    UiTrace t;
    t.Add(0x5236a0, (void*)&rt3::Net_InitShared, (void*)&RecB35Init);
    t.Add(0x521c80, (void*)&rt3::Net_SetPlayerName, (void*)&FakeB35Name);
    t.Add(0x521620, (void*)&rt3::Sound_SetBufferMs, (void*)&FakeB35Ms);
    t.Add(0x557930, (void*)&rt3::Str_ToWide, (void*)&FakeB35Wide);
    t.Add(0x522240, (void*)&rt3::CmdQueue_Broadcast, (void*)&FakeB24Bcast);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    static void* vt[0x20];
    vt[0x48 / 4] = (void*)&FakeB35Caps;
    vt[0x24 / 4] = (void*)&FakeB35Host;
    static uint8_t peer[8], dev[8], dp[0x20], o[0x300], list[8];
    *(void***)peer = vt;
    *(void***)dev = vt;
    *(void**)(dp + 8) = peer;
    *(void**)(dp + 0xc) = dev;
    *(uint8_t**)(o + 0x18e) = dp;
    for (int it = 0; it < 1500; ++it) {
        for (auto& k : g_b35Ok) k = Rnd() % 6 != 0;
        g_b35Hr = Rnd() % 3 == 0 ? (int32_t)0x80004005 : (int32_t)(Rnd() % 2);
        bool hasList = Rnd() % 8 != 0, pw = Rnd() % 2;
        int32_t ms = (int32_t)(Rnd() % 20000), maxp = (int32_t)(Rnd() % 9);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<void*>(0x6d408c) = hasList ? list : nullptr;
            hook::Global<int32_t>(0x6d40dc) = 0;
            memset(o + 0x192, 0, 0x120);
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Net_Host(o, 0, "My Game", "Bob", pw ? "secret" : nullptr, ms, maxp)
                              : ((int32_t(__fastcall*)(uint8_t*, int, const char*, const char*, const char*, int32_t, int32_t))0x5238e0)(
                                    o, 0, "My Game", "Bob", pw ? "secret" : nullptr, ms, maxp);
                Logf("r %d [%s] [%s] %d %d", r, (char*)o + 0x192, (char*)o + 0x21a, *(int32_t*)(o + 0x212), hook::Global<int32_t>(0x6d40dc));
            });
        }
        CompareLogs(logs[0], logs[1], "net host", it);
    }
}

namespace rt3 {
int32_t __fastcall Net_PlayerJoined(int32_t, uint8_t* msg);
void* __fastcall FreeList_Take(uint8_t* q);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
uint32_t __fastcall SysInfo_CpuSignature();
}

namespace {
int32_t g_b36Mode, g_b36Flags;
bool g_b36Rec;
uint8_t* g_b36Player;
int32_t __stdcall FakeB36Info(void* peer, int32_t id, uint32_t* info, uint32_t* size, uint32_t f) {
    Logf("info %d %p %u %u", id, (void*)(info ? 1 : 0), *size, f);
    if (!info) {
        *size = 0x40;
        return g_b36Mode == 0 ? (int32_t)0x80158100 : g_b36Mode == 1 ? 0 : (int32_t)0x80004005;
    }
    Logf("info in %u %u %u", info[0], info[2], info[5]);
    static const wchar_t* names[] = { L"Bob", L"Alice Smith", L"x" };
    info[2] = (uint32_t)(uintptr_t)names[id % 3];
    info[5] = (uint32_t)g_b36Flags;
    return g_b36Mode == 3 ? (int32_t)0x80004005 : 0;
}
void* __fastcall FakeB36Take(uint8_t* q) {
    Logf("take %p", q);
    return g_b36Rec ? g_b36Player : nullptr;
}
int32_t __fastcall RecB36Hash(HashMap* m, int, uint32_t b, int32_t n) {
    Logf("hash init %u %d", b, n);
    return 1;
}
void __fastcall RecB36Heap(rt3::MaxHeap* h, int, int32_t c) { Logf("heap init %d", c); }
void __fastcall RecB36Insert(void** l, int, void** after, void** node) { Logf("insert %p %p %p", l, after, node); }
uint32_t __fastcall FakeB36Cpu() { return 0x683; }
void WINAPI FakeB36Inc(LONG* p) { Logf("inc %p", p); }
} // namespace

DIFFTEST(net_PlayerJoined) {
    SavedGlobals sg({ 0x6d40e0, 0x6d408c, 0x6d4088, 0x6d40a8, 0x6d40e4, 0x6d4108 });
    IatSlot i1(0x5c8070, (void*)&FakeB36Inc), i3(0x5c81fc, (void*)&FakeElEnter), i4(0x5c81f8, (void*)&FakeElLeave);
    UiTrace t;
    t.Add(0x4696d0, (void*)&rt3::FreeList_Take, (void*)&FakeB36Take);
    t.Add(0x53dbf0, (void*)&rt3::Hash_Init, (void*)&RecB36Hash);
    t.Add(0x53d740, (void*)&rt3::Heap_Init, (void*)&RecB36Heap);
    t.Add(0x469690, (void*)&rt3::SList_InsertAfter, (void*)&RecB36Insert);
    t.Add(0x55d3d0, (void*)&rt3::SysInfo_CpuSignature, (void*)&FakeB36Cpu);
    t.Add(0x522240, (void*)&rt3::CmdQueue_Broadcast, (void*)&FakeB24Bcast);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    static void* vt[0x20];
    vt[0x54 / 4] = (void*)&FakeB36Info;
    static uint8_t peer[8], list[8], pool[8], msg[0x10];
    *(void***)peer = vt;
    g_b36Player = new uint8_t[0x9bd * 4];
    hook::Global<void*>(0x6d40e0) = peer;
    hook::Global<void*>(0x6d4088) = pool;
    hook::Global<int32_t>(0x6d4108) = 42;
    for (int it = 0; it < 600; ++it) {
        g_b36Mode = (int32_t)(Rnd() % 6);
        if (g_b36Mode > 3) g_b36Mode = 0;
        g_b36Flags = (int32_t)(Rnd() % 8);
        g_b36Rec = Rnd() % 5 != 0;
        bool hasList = Rnd() % 6 != 0;
        *(int32_t*)(msg + 4) = (int32_t)(Rnd() % 9);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<void*>(0x6d408c) = hasList ? list : nullptr;
            *(void**)(list + 4) = list;
            hook::Global<int32_t>(0x6d40a8) = 3;
            hook::Global<int32_t>(0x6d40e4) = -1;
            memset(g_b36Player, 0xcc, 0x9bd * 4);
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Net_PlayerJoined(0, msg) : ((int32_t(__fastcall*)(int32_t, uint8_t*))0x522ab0)(0, msg);
                Logf("r %d", r);
            });
            char tail[160];
            uint8_t* p = g_b36Player;
            sprintf(tail, " [%.16s] %d %08x %d %d %d %d %d %d %d %d", (char*)p + 8, *(int32_t*)(p + 4), *(uint32_t*)(p + 0x48), *(int32_t*)(p + 0x50),
                    *(int32_t*)(p + 0x5c), *(int32_t*)(p + 0x68), *(int32_t*)(p + 0x70), *(int32_t*)(p + 0x268c), *(int32_t*)(p + 0x2694),
                    *(int32_t*)(p + 0x2690), hook::Global<int32_t>(0x6d40e4));
            logs[s] += tail;
            logs[s] += " " + std::to_string(hook::Global<int32_t>(0x6d40a8));
        }
        CompareLogs(logs[0], logs[1], "player joined", it);
    }
}

namespace rt3 {
void __fastcall Engine_Destroy(uint8_t* e);
void __fastcall PtrList_Destroy(PtrList* l);
void __fastcall Terrain_Destructor(uint8_t* r);
void __fastcall Engine_Shutdown(uint8_t* e, int, uint8_t full);
void __fastcall Windows_Shutdown(uint8_t* m);
void __fastcall Nop564170();
void __fastcall Cursor_Shutdown(uint8_t* c);
void __fastcall Mouse_Destroy(uint8_t* m);
void __fastcall Pack_Shutdown(uint8_t* t);
void __fastcall Nop54e780();
void __fastcall Sound_Destruct(uint8_t* snd);
void __fastcall ModelLibrary_Free(uint32_t* o);
void FreeLanguageStrings();
void StreamReleaseAll();
}

namespace {
void __fastcall RecB37List(PtrList* l) { Logf("list %p", l); }
void __fastcall RecB37Terrain(uint8_t* r) { Logf("terrain %p", r); }
void __fastcall RecB37Shutdown(uint8_t* e, int, uint8_t f) { Logf("shutdown %p %d", e, f); }
void __fastcall RecB37Windows(uint8_t* m) { Logf("windows %p", m); }
void __fastcall RecB37Nop1() { Logf("nop1"); }
void __fastcall RecB37Cursor(uint8_t* c) { Logf("cursor %p", c); }
void __fastcall RecB37Mouse(uint8_t* m) { Logf("mouse %p", m); }
void __fastcall RecB37Pack(uint8_t* t) { Logf("pack %p", t); }
void __fastcall RecB37Nop2() { Logf("nop2"); }
void __fastcall RecB37Sound(uint8_t* s) { Logf("sound %p", s); }
void __fastcall RecB37Models(uint32_t* o) { Logf("models %p", o); }
void RecB37Lang() { Logf("lang"); }
void RecB37Streams() { Logf("streams"); }
void __fastcall RecB37Release(uint8_t* pack, int, void* obj) { Logf("release %p %p", pack, obj); }
} // namespace

DIFFTEST(engine_Destroy) {
    SavedGlobals sg({ 0x6d401c, 0xccbb20, 0x6d4020, 0x6d4018, 0x6d402c, 0x6d4030, 0x6d4024 });
    UiTrace t;
    t.Add(0x5571d0, (void*)&rt3::PtrList_Destroy, (void*)&RecB37List);
    t.Add(0x537e30, (void*)&rt3::Terrain_Destructor, (void*)&RecB37Terrain);
    t.Add(0x51ffb0, (void*)&rt3::Engine_Shutdown, (void*)&RecB37Shutdown);
    t.Add(0x53c000, (void*)&rt3::Pack_Release, (void*)&RecB37Release);
    t.Add(0x538a70, (void*)&rt3::Windows_Shutdown, (void*)&RecB37Windows);
    t.Add(0x564170, (void*)&rt3::Nop564170, (void*)&RecB37Nop1);
    t.Add(0x53f3d0, (void*)&rt3::Cursor_Shutdown, (void*)&RecB37Cursor);
    t.Add(0x543e60, (void*)&rt3::Mouse_Destroy, (void*)&RecB37Mouse);
    t.Add(0x53b9e0, (void*)&rt3::Pack_Shutdown, (void*)&RecB37Pack);
    t.Add(0x54e780, (void*)&rt3::Nop54e780, (void*)&RecB37Nop2);
    t.Add(0x532200, (void*)&rt3::Sound_Destruct, (void*)&RecB37Sound);
    t.Add(0x556620, (void*)&rt3::ModelLibrary_Free, (void*)&RecB37Models);
    t.Add(0x5193d0, (void*)&rt3::FreeLanguageStrings, (void*)&RecB37Lang);
    t.Add(0x530c50, (void*)&rt3::StreamReleaseAll, (void*)&RecB37Streams);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    t.Add(0x5a1145, nullptr, (void*)&RecB27Free);
    static uint8_t* e = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    static uint8_t objs[20][8];
    static const uint32_t offs[] = { 0x11369d, 0x1136a1, 0x1136a5, 0x1c, 0x1141f5, 0x1141f9, 0x1141fd, 0x24, 0x20, 0x34 };
    static const uint32_t globs[] = { 0x6d401c, 0xccbb20, 0x6d4020, 0x6d4018, 0x6d402c, 0x6d4030 };
    for (int it = 0; it < 400; ++it) {
        uint32_t mask = Rnd();
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            for (int k = 0; k < 10; ++k) *(uint8_t**)(e + offs[k]) = mask >> k & 1 ? objs[k] : nullptr;
            for (int k = 0; k < 6; ++k) hook::Global<uint8_t*>(globs[k]) = mask >> (10 + k) & 1 ? objs[10 + k] : nullptr;
            *(int32_t*)(e + 0x114201) = 9;
            hook::Global<void*>(0x6d4024) = e;
            logs[s] = t.Run([&] {
                if (s) rt3::Engine_Destroy(e);
                else ((void(__fastcall*)(uint8_t*))0x521390)(e);
            });
            for (uint32_t o : offs) logs[s] += " " + std::to_string((int)(*(void**)(e + o) != nullptr));
            for (uint32_t g : globs) logs[s] += " " + std::to_string((int)(hook::Global<void*>(g) != nullptr));
            logs[s] += " " + std::to_string(*(int32_t*)(e + 0x114201)) + " " + std::to_string((int)(hook::Global<void*>(0x6d4024) != nullptr));
        }
        CompareLogs(logs[0], logs[1], "engine destroy", it);
    }
}

namespace rt3 {
int32_t __fastcall Lobby_Connect(uint8_t* lobby, int, const char* nick);
int32_t __fastcall Lobby_UpdateList(uint8_t* lobby);
}

namespace {
int32_t g_b38Peer, g_b38Title, g_b38Conn, g_b38Join;
void* __fastcall FakeB38Init(uint32_t* cb) {
    std::string s;
    for (int k = 0; k < 0x1c; ++k) s += " " + std::to_string(cb[k]);
    Logf("init%s", s.c_str());
    return g_b38Peer ? (void*)0x1234 : nullptr;
}
int32_t __fastcall FakeB38Title(void* p, const char* t, const char* k, const char* n, const char* k2, int32_t v, int32_t m, int32_t nat, int32_t* ping,
                                int32_t* cross) {
    Logf("title %p %p %p %p %p %d %d %d %d%d%d %d%d%d", p, t, k, n, k2, v, m, nat, ping[0], ping[1], ping[2], cross[0], cross[1], cross[2]);
    return g_b38Title;
}
void __fastcall FakeB38Key(char* dst, const char* src) {
    Logf("key [%s]", src);
    strcpy(dst, src);
}
void __fastcall FakeB38Conn(void* p, const char* nick, int32_t a, uint32_t cb1, uint32_t cb2, int32_t b, int32_t c) {
    Logf("connect %p [%s] %d %x %x %d %d", p, nick, a, cb1, cb2, b, c);
    hook::Global<int32_t>(0x6cd974) = g_b38Conn;
}
void __fastcall FakeB38Join(void* p, int32_t a, uint32_t cb, int32_t b, int32_t c) {
    Logf("join %p %d %x %d %d", p, a, cb, b, c);
    hook::Global<int32_t>(0x6cd978) = g_b38Join;
}
int32_t __fastcall RecB38List(uint8_t* l) {
    Logf("list");
    return 0;
}
} // namespace

DIFFTEST(net_LobbyConnect) {
    SavedGlobals sg({ 0x6cd970, 0x6cd974, 0x6cd978, 0x6cd920, 0x624324 });
    UiTrace t;
    t.Add(0x58dc50, nullptr, (void*)&FakeB38Init);
    t.Add(0x58dce0, nullptr, (void*)&FakeB38Title);
    t.Add(0x58e650, nullptr, (void*)&FakeB38Key);
    t.Add(0x58e200, nullptr, (void*)&FakeB38Conn);
    t.Add(0x58dfb0, nullptr, (void*)&FakeB38Join);
    t.Add(0x469520, (void*)&rt3::Lobby_UpdateList, (void*)&RecB38List);
    static uint8_t* lobby = new uint8_t[0x9900];
    for (int it = 0; it < 1000; ++it) {
        g_b38Peer = Rnd() % 6 != 0;
        g_b38Title = Rnd() % 5 != 0;
        g_b38Conn = Rnd() % 4 != 0;
        g_b38Join = Rnd() % 4 != 0;
        int32_t connected = Rnd() % 5 == 0;
        uint32_t lcg = Rnd();
        int nk = (int)(Rnd() % 3);
        const char* nick = nk == 0 ? nullptr : nk == 1 ? "" : "Florian";
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<int32_t>(0x6cd920) = connected;
            hook::Global<uint32_t>(0x624324) = lcg;
            hook::Global<int32_t>(0x6cd974) = 7;
            hook::Global<int32_t>(0x6cd978) = 7;
            *(int32_t*)(lobby + 0x9874) = 0;
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Lobby_Connect(lobby, 0, nick) : ((int32_t(__fastcall*)(uint8_t*, int, const char*))0x46a900)(lobby, 0, nick);
                Logf("r %d %p %d %d %d %08x", r, hook::Global<void*>(0x6cd970), hook::Global<int32_t>(0x6cd974), hook::Global<int32_t>(0x6cd978),
                     *(int32_t*)(lobby + 0x9874), hook::Global<uint32_t>(0x624324));
            });
        }
        CompareLogs(logs[0], logs[1], "lobby connect", it);
    }
}

namespace rt3 {
void __fastcall Lobby_HostGame(uint8_t* lobby, int, const char* game, const char* name, const char* password, int32_t ms);
void __fastcall Lobby_JoinGame(uint8_t* lobby, int, const char* host, const char* name, const char* password);
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
}

namespace {
int32_t g_b39Hr;
int32_t __fastcall FakeB39Host(uint8_t* o, int, const char* g, const char* n, const char* pw, int32_t ms, int32_t mp) {
    Logf("host %p [%s] [%s] [%s] %d %d", o, g, n, pw ? pw : "-", ms, mp);
    return 1;
}
int32_t __fastcall FakeB39Connect(uint8_t* o, int, const char* h, const char* n, const char* pw, int32_t ms) {
    Logf("connect %p [%s] [%s] [%s] %d", o, h, n, pw ? pw : "-", ms);
    return g_b39Hr;
}
void __fastcall RecB39Peer1(void* p) { Logf("peer leave %p", p); }
void __fastcall RecB39Peer2(void* p) { Logf("peer stop %p", p); }
} // namespace

DIFFTEST(net_LobbyHostJoin) {
    SavedGlobals sg({ 0x6d40dc, 0x6d40d0, 0x6cd928, 0x6cd970 });
    UiTrace t;
    t.Add(0x5238e0, (void*)&rt3::Net_Host, (void*)&FakeB39Host);
    t.Add(0x523af0, (void*)&rt3::Net_Connect, (void*)&FakeB39Connect);
    t.Add(0x5217b0, (void*)&rt3::Nop5217b0, (void*)&RecB27Nop);
    t.Add(0x58d9d0, nullptr, (void*)&RecB39Peer1);
    t.Add(0x58e1f0, nullptr, (void*)&RecB39Peer2);
    static uint8_t* lobby = new uint8_t[0x9900];
    static uint8_t net[8], peer[8];
    static const char* addrs[] = { "1.2.3.4", "5.6.7.8", "10.0.0.1" };
    for (int it = 0; it < 1500; ++it) {
        int32_t busy = Rnd() % 6 == 0, listing = (int32_t)(Rnd() % 3), connected = (int32_t)(Rnd() % 2);
        bool hasPeer = Rnd() % 2;
        int pwk = (int)(Rnd() % 3);
        const char* pw = pwk == 0 ? nullptr : pwk == 1 ? "" : "pass";
        int32_t ms = (int32_t)(Rnd() % 9999);
        g_b39Hr = Rnd() % 3 == 0 ? -5 : 0;
        int nslots = (int)(Rnd() % 4);
        const char* host = addrs[Rnd() % 3];
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(lobby, 0, 0x9900);
            for (int k = 0; k < nslots; ++k) {
                sprintf((char*)lobby + 0x14 + k * 0x11c, "Game %d", k);
                strcpy((char*)lobby + 0x14 + k * 0x11c + 0x80, addrs[k % 3]);
            }
            *(int32_t*)(lobby + 0x8f18) = listing;
            *(int32_t*)(lobby + 0x985c) = connected;
            *(int32_t*)(lobby + 0x8f10) = 5;
            hook::Global<int32_t>(0x6d40dc) = busy;
            hook::Global<void*>(0x6d40d0) = net;
            hook::Global<void*>(0x6cd970) = hasPeer ? peer : nullptr;
            hook::Global<int32_t>(0x6cd928) = 9;
            logs[s] = t.Run([&] {
                if (s) rt3::Lobby_HostGame(lobby, 0, "My Game", "Bob", pw, ms);
                else ((void(__fastcall*)(uint8_t*, int, const char*, const char*, const char*, int32_t))0x46a030)(lobby, 0, "My Game", "Bob", pw, ms);
                Logf("after host %d %d %d [%s] [%s] %d", *(int32_t*)(lobby + 0x8f18), *(int32_t*)(lobby + 0x8f10), hook::Global<int32_t>(0x6cd928),
                     (char*)lobby + 0x8e10, (char*)lobby + 0x8f48, 0);
                memset(lobby + 0x8e10, 0, 0x20);
                if (s) rt3::Lobby_JoinGame(lobby, 0, host, "Bob", pw);
                else ((void(__fastcall*)(uint8_t*, int, const char*, const char*, const char*))0x46a110)(lobby, 0, host, "Bob", pw);
                Logf("after join %d [%s]", *(int32_t*)(lobby + 0xc), (char*)lobby + 0x8e10);
            });
        }
        CompareLogs(logs[0], logs[1], "lobby host / join", it);
    }
}

DIFFTEST(save_PackCompress) {
    SavedGlobals sg({ 0xd93984, 0xd9fdc8, 0xda7dd0, 0xda7dd4, 0xda7dd8, 0xdafde0, 0xdafde4, 0xdb7de8, 0xdb7dec });
    static uint8_t saved1[0x8010], saved2[0x8010], saved3[0x8008], saved4[0x8000];
    memcpy(saved1, (void*)0xd9fdd0, sizeof saved1);
    memcpy(saved2, (void*)0xd97dc8, sizeof saved2);
    memcpy(saved3, (void*)0xda7de0, sizeof saved3);
    memcpy(saved4, (void*)0xdafde8, sizeof saved4);
    typedef int32_t(__fastcall * Fn)(int32_t*, int, const void*, int32_t, uint8_t**, int32_t*);
    for (int it = 0; it < 120; ++it) {
        int32_t method = Rnd() % 8 == 0 ? 0x90 : Rnd() % 2 ? 0x91 : 0x92;
        std::vector<uint8_t> src;
        size_t n = 1 + Rnd() % 0x9000;
        int alpha = 1 + (int)(Rnd() % 255);
        while (src.size() < n) {
            uint8_t b = (uint8_t)(Rnd() % alpha);
            size_t rep = Rnd() % 4 ? 1 : 1 + Rnd() % 200;
            for (size_t k = 0; k < rep && src.size() < n; ++k) src.push_back(b);
        }
        int32_t r[2], sz[2];
        std::vector<uint8_t> out[2];
        for (int s = 0; s < 2; ++s) {
            uint8_t* o = (uint8_t*)1;
            sz[s] = 7;
            hook::Global<int32_t>(0xd9fdc8) = 0;
            hook::Global<int32_t>(0xdb7de8) = 0;
            r[s] = s ? rt3::Pack_Compress(&method, 0, src.data(), (int32_t)n, &o, &sz[s]) : ((Fn)0x553000)(&method, 0, src.data(), (int32_t)n, &o, &sz[s]);
            if (o && o != (uint8_t*)1) {
                out[s].assign(o, o + sz[s]);
                ((void(__cdecl*)(void*))0x5a1145)(o);
            } else out[s].push_back(o ? 1 : 0);
        }
        EXPECT_EQ(r[0], r[1], "pack compress result %d", it);
        EXPECT_EQ(sz[0], sz[1], "pack compress size %d", it);
        EXPECT_EQ(out[0] == out[1], true, "pack compress output %d", it);
    }
    memcpy((void*)0xd9fdd0, saved1, sizeof saved1);
    memcpy((void*)0xd97dc8, saved2, sizeof saved2);
    memcpy((void*)0xda7de0, saved3, sizeof saved3);
    memcpy((void*)0xdafde8, saved4, sizeof saved4);
}

namespace rt3 {
int32_t __fastcall Session_Open(uint8_t* s, int, int32_t gamespy, const char* nick);
int32_t __fastcall Lobby_Connect(uint8_t* lobby, int, const char* nick);
int32_t __fastcall Music_Start(uint8_t* m);
uint8_t* __fastcall Net_Construct(uint8_t* o);
float __fastcall Game_VersionNumber();
void __stdcall SetGlobal6d4108(int32_t v);
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void MpSession_Reset();
void RegisterSyncChecks();
int32_t __fastcall PackSrc_Read(uint8_t* buf, int32_t n);
int32_t __fastcall PackDst_Write(uint8_t* buf, int32_t n);
}

namespace {
int32_t g_b40Open, g_b40Music, g_b40Init, g_b40Lobby;
void __fastcall FakeB40Close(uint8_t* s) {
    Logf("close");
    *(int32_t*)s = g_b40Open;
}
int32_t __fastcall FakeB40Music(uint8_t* m) {
    Logf("music start %d", (int)(m != nullptr));
    return g_b40Music;
}
uint8_t* __fastcall FakeB40Net(uint8_t* o) {
    Logf("net construct");
    return o;
}
int32_t __fastcall FakeB40Init(uint8_t* n) {
    Logf("net init %d", (int)(n != nullptr));
    return g_b40Init;
}
float __fastcall FakeB40Ver() { return 1.06f; }
void __stdcall RecB40Ver(int32_t v) { Logf("ver %08x", v); }
void __fastcall RecB40Pool(NodePool* p, int, void* st, int32_t sz, int32_t n, int32_t f) { Logf("pool init %d %d %d %d", (int)(st != nullptr), sz, n, f); }
uint8_t* __fastcall FakeB40Font(uint8_t* pack, int, int32_t i) {
    Logf("font %d", i);
    return (uint8_t*)0x5150;
}
void RecB40Reset() { Logf("mp reset"); }
void RecB40Sync() { Logf("sync checks"); }
int32_t __fastcall FakeB40Lobby(uint8_t* l, int, const char* nick) {
    Logf("lobby [%s]", nick ? nick : "-");
    return g_b40Lobby;
}
void* __cdecl FakeB40Malloc(size_t n) {
    Logf("malloc %x", (unsigned)n);
    return ArenaAlloc(n);
}
int g_b40Progress;
void __fastcall RecB40Progress(int32_t pct) { Logf("progress %d", pct); }
} // namespace

DIFFTEST(net_SessionOpen) {
    SavedGlobals sg({ 0x6ae4cc, 0x6ae4d0, 0x6ae4d4, 0x6cd96c, 0x6d10a0, 0x622090, 0x622094, 0x6cd92c, 0x6cd928, 0x6cd924, 0x6cd91c, 0x6cd8e4, 0x6cd8e8,
                      0x5f2b38, 0x6cd8f4, 0x6cd8f8, 0x6cd8fc, 0x6cd908, 0x6cd90c, 0x6cd918, 0x6cd8ec, 0x6cd8f0, 0x6cd900, 0x5f2b3c, 0x6cd904, 0x5f2b40,
                      0xd973b4, 0x6d40d0, 0x6257e8, 0x6257ec, 0x6257f0, 0x6257f4 });
    UiTrace t;
    t.Add(0x46bc40, (void*)&rt3::Session_Close, (void*)&FakeB40Close);
    t.Add(0x557bc0, (void*)&rt3::Music_Start, (void*)&FakeB40Music);
    t.Add(0x521bf0, (void*)&rt3::Net_Construct, (void*)&FakeB40Net);
    t.Add(0x5236a0, (void*)&rt3::Net_InitShared, (void*)&FakeB40Init);
    t.Add(0x482d10, (void*)&rt3::Game_VersionNumber, (void*)&FakeB40Ver);
    t.Add(0x521910, (void*)&rt3::SetGlobal6d4108, (void*)&RecB40Ver);
    t.Add(0x53dbf0, (void*)&rt3::Hash_Init, (void*)&RecB36Hash);
    t.Add(0x53dcf0, (void*)&rt3::Pool_Init, (void*)&RecB40Pool);
    t.Add(0x53c6f0, (void*)&rt3::Pack_AcquireFont, (void*)&FakeB40Font);
    t.Add(0x473280, (void*)&rt3::MpSession_Reset, (void*)&RecB40Reset);
    t.Add(0x473a60, (void*)&rt3::RegisterSyncChecks, (void*)&RecB40Sync);
    t.Add(0x46a900, (void*)&rt3::Lobby_Connect, (void*)&FakeB40Lobby);
    t.Add(0x5a125d, nullptr, (void*)&FakeB40Malloc);
    static uint8_t* s = new uint8_t[0x9900];
    static uint8_t music[0x18], net[0x260];
    for (int it = 0; it < 800; ++it) {
        g_b40Open = Rnd() % 6 == 0;
        g_b40Music = Rnd() % 6 ? 1 : 0;
        g_b40Init = Rnd() % 6 ? 1 : 0;
        g_b40Lobby = (int32_t)(Rnd() % 3);
        bool hasMusic = Rnd() % 2, hasNet = Rnd() % 2;
        int32_t gs = (int32_t)(Rnd() % 3);
        std::string logs[2];
        for (int k = 0; k < 2; ++k) {
            memset(s, 0x5a, 0x9900);
            hook::Global<void*>(0xd973b4) = hasMusic ? music : nullptr;
            hook::Global<void*>(0x6d40d0) = hasNet ? net : nullptr;
            hook::Global<int32_t>(0x5f2b38) = 7;
            logs[k] = t.Run([&] {
                int32_t r = k ? rt3::Session_Open(s, 0, gs, "Bob") : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, const char*))0x46be80)(s, 0, gs, "Bob");
                Logf("r %d", r);
            });
            uint32_t h = 7;
            for (uint32_t o = 0x8; o < 0x9900; o += 4) {
                uint32_t v = *(uint32_t*)(s + o);
                if (o == 0x4 || o == 0x8f20 || o == 0x8f24 || o == 0x8f28 || o == 0x8f2c || o == 0x8f3c || o == 0x8f40 || o == 0x9050 || o == 0x9878) v = v != 0;
                h = h * 31 + v;
            }
            char tail[64];
            sprintf(tail, " %08x %d %d %d %08x", h, (int)(*(void**)s != nullptr), hook::Global<int32_t>(0x5f2b38), hook::Global<int32_t>(0x622090),
                    hook::Global<uint32_t>(0x6257e8));
            logs[k] += tail;
        }
        CompareLogs(logs[0], logs[1], "session open", it);
    }
}

DIFFTEST(save_PackCallbacks) {
    SavedGlobals sg({ 0xd93984, 0xd93988 });
    UiTrace t;
    static uint8_t src[0x400], dst[0x400], buf[0x400];
    for (auto& b : src) b = (uint8_t)Rnd();
    for (int it = 0; it < 3000; ++it) {
        int32_t pos = (int32_t)(Rnd() % 0x401), size = 1 + (int32_t)(Rnd() % 0x400), n = (int32_t)(Rnd() % 0x300);
        int32_t used = (int32_t)(Rnd() % 0x400), cap = (int32_t)(Rnd() % 0x401);
        bool prog = Rnd() % 2;
        if (pos > size) pos = size;
        std::string logs[2];
        for (int k = 0; k < 2; ++k) {
            int32_t ctx[6] = { (int32_t)(uintptr_t)src, pos, size, (int32_t)(uintptr_t)dst, used, cap };
            hook::Global<int32_t*>(0xd93984) = ctx;
            hook::Global<void*>(0xd93988) = prog ? (void*)&RecB40Progress : nullptr;
            memset(buf, 0, sizeof buf);
            memset(dst, 0, sizeof dst);
            logs[k] = t.Run([&] {
                int32_t r = k ? rt3::PackSrc_Read(buf, n) : ((int32_t(__fastcall*)(uint8_t*, int32_t))0x552f30)(buf, n);
                int32_t w = n + used <= 0x400 ? n : 0;
                int32_t r2 = k ? rt3::PackDst_Write(src, w) : ((int32_t(__fastcall*)(uint8_t*, int32_t))0x552fa0)(src, w);
                Logf("r %d %d ctx %d %d", r, r2, ctx[1], ctx[4]);
            });
            uint32_t h = 7;
            for (int j = 0; j < 0x400; ++j) h = h * 31 + buf[j] + dst[j] * 7;
            logs[k] += " " + std::to_string(h);
        }
        CompareLogs(logs[0], logs[1], "pack callbacks", it);
    }
}

namespace rt3 {
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);
}

DIFFTEST(save_PackDecompress) {
    SavedGlobals sg({ 0xd93984, 0xd93988, 0xd9fdc8, 0xda7dd0, 0xda7dd4, 0xda7dd8, 0xdafde0, 0xdafde4, 0xdb7de8, 0xdb7dec });
    static uint8_t saved1[0x8010], saved2[0x8010], saved3[0x8008], saved4[0x8000];
    memcpy(saved1, (void*)0xd9fdd0, sizeof saved1);
    memcpy(saved2, (void*)0xd97dc8, sizeof saved2);
    memcpy(saved3, (void*)0xda7de0, sizeof saved3);
    memcpy(saved4, (void*)0xdafde8, sizeof saved4);
    hook::Global<void*>(0xd93988) = nullptr;
    typedef int32_t(__fastcall * Fn)(int32_t*, int, const void*, int32_t, uint8_t**, int32_t*);
    for (int it = 0; it < 120; ++it) {
        int32_t method = Rnd() % 2 ? 0x91 : 0x92;
        std::vector<uint8_t> src;
        size_t n = 1 + Rnd() % 0x9000;
        while (src.size() < n) {
            uint8_t b = (uint8_t)(Rnd() % 7);
            size_t rep = Rnd() % 4 ? 1 : 1 + Rnd() % 200;
            for (size_t k = 0; k < rep && src.size() < n; ++k) src.push_back(b);
        }
        uint8_t* packed = nullptr;
        int32_t psz = 0;
        hook::Global<int32_t>(0xd9fdc8) = 0;
        hook::Global<int32_t>(0xdb7de8) = 0;
        if (((Fn)0x553000)(&method, 0, src.data(), (int32_t)n, &packed, &psz) != 1) continue;
        int kind = (int)(Rnd() % 6);
        if (kind == 0) ((int32_t*)packed)[0] = 0x1234;
        if (kind == 1) ((uint16_t*)packed)[7] = 0x90;
        int32_t arg = Rnd() % 2 ? 0 : psz;
        int32_t r[2], sz[2], m[2];
        std::vector<uint8_t> out[2];
        for (int s = 0; s < 2; ++s) {
            uint8_t* o = (uint8_t*)1;
            sz[s] = 7;
            m[s] = 0x55;
            hook::Global<int32_t>(0xd9fdc8) = 0;
            hook::Global<int32_t>(0xdb7de8) = 0;
            r[s] = s ? rt3::Pack_Decompress(&m[s], 0, (const int32_t*)packed, arg, &o, &sz[s]) : ((Fn)0x553120)(&m[s], 0, packed, arg, &o, &sz[s]);
            if (o && o != (uint8_t*)1) {
                out[s].assign(o, o + sz[s]);
                ((void(__cdecl*)(void*))0x5a1145)(o);
            } else out[s].push_back(o ? 1 : 0);
        }
        ((void(__cdecl*)(void*))0x5a1145)(packed);
        EXPECT_EQ(r[0], r[1], "pack decompress result %d", it);
        EXPECT_EQ(sz[0], sz[1], "pack decompress size %d", it);
        EXPECT_EQ(m[0], m[1], "pack decompress method %d", it);
        EXPECT_EQ(out[0] == out[1], true, "pack decompress output %d", it);
        if (r[1] == 1) EXPECT_EQ(out[1] == src, true, "pack round trip %d", it);
    }
    memcpy((void*)0xd9fdd0, saved1, sizeof saved1);
    memcpy((void*)0xd97dc8, saved2, sizeof saved2);
    memcpy((void*)0xda7de0, saved3, sizeof saved3);
    memcpy((void*)0xdafde8, saved4, sizeof saved4);
}

#include "jpeg_samples.h"

namespace rt3 {
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset);
}

DIFFTEST(render_JpegLoad) {
    struct Sample {
        const uint8_t* data;
        size_t n;
    } samples[] = { { g_jpeg0, sizeof g_jpeg0 }, { g_jpeg1, sizeof g_jpeg1 }, { g_jpeg2, sizeof g_jpeg2 }, { g_jpeg3, sizeof g_jpeg3 } };
    char dir[MAX_PATH], path[MAX_PATH];
    GetTempPathA(sizeof dir, dir);
    sprintf(path, "%srt3re_jpeg_test.bin", dir);
    typedef uint8_t*(__fastcall * Fn)(const char*, int32_t*, int32_t*, int32_t);
    for (int it = 0; it < 12; ++it) {
        const Sample& s = samples[it % 4];
        int32_t offset = it < 4 ? 0 : (int32_t)(1 + Rnd() % 300);
        FILE* f = fopen(path, "wb");
        for (int32_t k = 0; k < offset; ++k) fputc((int)(Rnd() & 0xff), f);
        fwrite(s.data, 1, s.n, f);
        fclose(f);
        int32_t w[2] = { -1, -1 }, h[2] = { -1, -1 };
        std::vector<uint8_t> px[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* b = side ? rt3::Jpeg_Load(path, &w[side], &h[side], offset) : ((Fn)0x552e20)(path, &w[side], &h[side], offset);
            if (b) {
                // A grayscale image fills only w bytes of each 3w-byte row; the rest is uninitialised.
                int32_t used = it % 4 == 2 ? w[side] : w[side] * 3;
                for (int32_t y = 0; y < h[side]; ++y) px[side].insert(px[side].end(), b + y * w[side] * 3, b + y * w[side] * 3 + used);
                ((void(__cdecl*)(void*))0x5a1145)(b);
            }
        }
        EXPECT_EQ(w[0], w[1], "jpeg width %d", it);
        EXPECT_EQ(h[0], h[1], "jpeg height %d", it);
        EXPECT_EQ(px[0] == px[1], true, "jpeg pixels %d", it);
    }
    remove(path);
    int32_t w = 5, h = 6;
    EXPECT_EQ(rt3::Jpeg_Load("Z:\no\such.jpg", &w, &h, 0) == nullptr, true, "jpeg missing");
    EXPECT_EQ(w * 10 + h, 56, "jpeg missing untouched");
}

namespace rt3 {
void SysInfo_Init();
uint64_t __fastcall SysInfo_CpuMhz();
}

namespace {
uint64_t __fastcall FakeB44Mhz() { return 0x123456789ull; }
void __stdcall FakeB44Mem(void* p) {
    for (int k = 0; k < 8; ++k) ((uint32_t*)p)[k] = 0x1000 + k;
}
void __cdecl RecB44Crt(int32_t v) { Logf("crt %d", v); }
} // namespace

DIFFTEST(misc_SysInfoInit) {
    static uint8_t saved[0xd97568 - 0xd973f8];
    memcpy(saved, (void*)0xd973f8, sizeof saved);
    IatSlot mem(0x5c81d8, (void*)&FakeB44Mem);
    UiTrace t;
    t.Add(0x55d2f0, (void*)&rt3::SysInfo_CpuMhz, (void*)&FakeB44Mhz);
    t.Add(0x5a4376, nullptr, (void*)&RecB44Crt);
    std::string logs[2];
    for (int s = 0; s < 2; ++s) {
        memset((void*)0xd973f8, 0x77, sizeof saved);
        logs[s] = t.Run([&] {
            if (s) rt3::SysInfo_Init();
            else ((void (*)())0x55da40)();
        });
        uint32_t h = 7;
        for (size_t k = 0; k < sizeof saved; ++k) h = h * 31 + ((uint8_t*)0xd973f8)[k];
        logs[s] += " " + std::to_string(h) + " [" + std::string((char*)0xd973f8) + "] [" + std::string((char*)0xd97405) + "]";
    }
    CompareLogs(logs[0], logs[1], "sysinfo init", 0);
    memcpy((void*)0xd973f8, saved, sizeof saved);
}

namespace rt3 {
int32_t __fastcall Config_Load(uint8_t* c, int, const char* path);
void __fastcall Holder_Alloc(uint8_t* h);
void __fastcall Pool_Release(int32_t* p, int, void** node);
}

namespace {
uint8_t g_b45Pools[2][0x14], g_b45List[0xc];
void __fastcall FakeB45Alloc(uint8_t* c) {
    Logf("alloc");
    memset(g_b45Pools, 0, sizeof g_b45Pools);
    memset(g_b45List, 0, sizeof g_b45List);
    *(int32_t*)(g_b45Pools[0] + 0x10) = 0x204;
    *(int32_t*)(g_b45Pools[1] + 0x10) = 0x114;
    *(uint8_t**)c = g_b45Pools[0];
    *(uint8_t**)(c + 4) = g_b45Pools[1];
    *(uint8_t**)(c + 8) = g_b45List;
}
void __fastcall RecB45Release(int32_t* p, int, void** node) { Logf("release %p", p); }
std::string B45Dump(uint8_t* c) {
    std::string s;
    char b[64];
    uint8_t* list = *(uint8_t**)(c + 8);
    if (!list) return "nolist";
    sprintf(b, "secs %d:", *(int32_t*)(list + 8));
    s += b;
    for (uint8_t* sec = *(uint8_t**)list; sec; sec = *(uint8_t**)sec) {
        s += " [" + std::string((char*)sec + 4) + "] " + std::to_string(*(int32_t*)(sec + 0x104)) + "/" + std::to_string(*(int32_t*)(sec + 0x110)) + "(";
        for (uint8_t* e = *(uint8_t**)(sec + 0x108); e; e = *(uint8_t**)e) {
            uint32_t h = 7;
            for (int k = 4; k < 0x204; ++k) h = h * 31 + e[k];
            sprintf(b, " %08x", h);
            s += b;
        }
        s += (*(uint8_t**)(sec + 0x10c) ? ")" : ")-");
    }
    return s;
}
} // namespace

DIFFTEST(misc_ConfigLoad) {
    UiTrace t;
    t.Add(0x557550, (void*)&rt3::Holder_Alloc, (void*)&FakeB45Alloc);
    t.Add(0x414410, (void*)&rt3::Pool_Release, (void*)&RecB45Release);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    char dir[MAX_PATH], path[MAX_PATH];
    GetTempPathA(sizeof dir, dir);
    sprintf(path, "%srt3re_config_test.ini", dir);
    static const char* lines[] = { "[Main]", "  [Video] ; comment", "width = 1024", "name=Big Train", "; only a comment", "", "   \t", "bad line",
                                   "key=", "=value", "x = 1 ; tail", "[Empty]", "[unterminated", "depth\t=\t32" };
    static uint8_t c[0x120];
    for (int it = 0; it < 400; ++it) {
        FILE* f = fopen(path, "w");
        int n = (int)(Rnd() % 12);
        bool sectionFirst = Rnd() % 4 != 0;
        if (sectionFirst) fputs("[First]\n", f);
        for (int k = 0; k < n; ++k) fprintf(f, "%s\n", lines[Rnd() % 14]);
        if (Rnd() % 2) fputs("last=1", f);
        fclose(f);
        bool missing = Rnd() % 10 == 0;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(c, 0, sizeof c);
            logs[s] = t.Run([&] {
                const char* p = missing ? "Z:\no\such.ini" : path;
                int32_t r = s ? rt3::Config_Load(c, 0, p) : ((int32_t(__fastcall*)(uint8_t*, int, const char*))0x557630)(c, 0, p);
                Logf("r %d lines %d err [%s] %s", r, *(int32_t*)(c + 0x114), (char*)c + 0x14, B45Dump(c).c_str());
            });
        }
        CompareLogs(logs[0], logs[1], "config load", it);
    }
    remove(path);
}

namespace rt3 {
int32_t __fastcall EditorDialog_HousesEvent(uint8_t* w, int, int32_t* ev);
void __fastcall EditorDialog_FillHouses(uint8_t* w, int, int32_t keep);
}

namespace {
void __fastcall RecB46Fill(uint8_t* w, int, int32_t keep) { Logf("fill %d", keep); }
} // namespace

DIFFTEST(ui_EditorHousesEvent) {
    SavedGlobals sg({ 0x62b268, 0x6cec78 });
    UiTrace t;
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB18Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB18IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB30Get);
    t.Add(0x434f20, (void*)&rt3::World_SetNamedValue, (void*)&RecB30Set);
    t.Add(0x4cdbd0, (void*)&rt3::EditorDialog_FillHouses, (void*)&RecB46Fill);
    static uint8_t world[8], win[8], ctrl[0x100];
    hook::Global<uint8_t*>(0x6cec78) = world;
    g_b30TypeMode = true;
    static const int32_t ids[] = { 0xa028, 0xa40f, 0xa100, 0xa027, 0xa410, 0x9c43, 0x9c44, 0x9c42, 0x9c45 };
    for (int it = 0; it < 2000; ++it) {
        g_b18Count = (int32_t)(Rnd() % 7) - 1;
        for (int k = 0; k < 6; ++k) {
            g_b30Types[k][0x32] = (uint8_t)(Rnd() % 3);
            sprintf((char*)g_b30Types[k] + 4, "type%d", k);
        }
        int32_t ev[0x10] = {};
        ev[0] = Rnd() % 4 == 0 ? 0xcc : Rnd() % 2 ? 0xca : 0xcb;
        ev[1] = ids[Rnd() % 9];
        ev[2] = (int32_t)(Rnd() % 3);
        *(uint32_t*)(ctrl + 0x82) = Rnd() % 6;
        ev[0xb] = (int32_t)(uintptr_t)ctrl;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::EditorDialog_HousesEvent(win, 0, ev) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t*))0x4cf430)(win, 0, ev);
                Logf("r %d", r);
            });
        }
        CompareLogs(logs[0], logs[1], "editor houses event", it);
    }
    g_b30TypeMode = false;
}

namespace rt3 {
void __fastcall Station_DrawSummary(int32_t, uint32_t id, float x, float y, int32_t, float h, int32_t);
void* __fastcall Obj_Ref3cc(void* o);
float __fastcall Region_ClassSupply(uint8_t* r, int, int32_t cls);
}

namespace {
uint8_t g_b47St[0x500], g_b47Bld[0x40], g_b47Type[0x100], g_b47B[8], g_b47Rec[0x100];
float g_b47Sup[2];
void* __fastcall FakeB47Get(Collection* c, int, uint32_t id) {
    Logf("get %u", id);
    return c == hook::Global<Collection*>(0x6cec20) ? (void*)g_b47St : (void*)g_b47Bld;
}
void* __fastcall FakeB47Ref(void* o) { return g_b47Type; }
void* __fastcall FakeB47Building(void* o) { return g_b47B; }
uint8_t* __fastcall FakeB47Rec(uint8_t* b) { return g_b47Rec; }
float __fastcall FakeB47Sup(uint8_t* r, int, int32_t cls) {
    Logf("supply %x", cls);
    return g_b47Sup[cls & 1];
}
} // namespace

DIFFTEST(ui_StationDrawSummary) {
    SavedGlobals sg({ 0x6cec20, 0x62b26c, 0x6d170c, 0x6d1704 });
    UiTrace t;
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB47Get);
    t.Add(0x40cec0, (void*)&rt3::Obj_Ref3cc, (void*)&FakeB47Ref);
    t.Add(0x47de00, (void*)&rt3::Obj_Building, (void*)&FakeB47Building);
    t.Add(0x53de00, (void*)&rt3::Table_Entry, (void*)&FakeTableEntry);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeIpFormat);
    t.Add(0x47e390, (void*)&rt3::Region_ClassSupply, (void*)&FakeB47Sup);
    t.Add(0x51b700, (void*)&rt3::Num_Format, (void*)&FakeRtNum);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    static void* vt[0x40];
    vt[0x80 / 4] = (void*)&FakeB47Rec;
    *(void***)g_b47B = vt;
    static uint8_t sts[8], blds[8], sprites[8], font[8];
    hook::Global<uint8_t*>(0x6cec20) = sts;
    hook::Global<uint8_t*>(0x62b26c) = blds;
    hook::Global<uint8_t*>(0x6d170c) = sprites;
    hook::Global<uint8_t*>(0x6d1704) = font;
    for (int it = 0; it < 3000; ++it) {
        *(uint32_t*)(g_b47Type + 0xc3) = Rnd() % 4 == 0 ? 0 : 1u << (Rnd() % 8);
        g_b47Rec[0x8c] = (uint8_t)(Rnd() % 3);
        *(uint32_t*)(g_b47St + 4) = Rnd() % 50;
        strcpy((char*)g_b47St + 0x46b, "Station %1");
        g_b47Sup[0] = RndF(-5.0f, 500.0f);
        g_b47Sup[1] = RndF(-5.0f, 500.0f);
        float x = RndF(-1.0f, 2.0f), y = RndF(-1.0f, 2.0f), h = RndF(0.0f, 0.2f);
        uint32_t id = Rnd() % 100;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            logs[s] = t.Run([&] {
                if (s) rt3::Station_DrawSummary(0, id, x, y, 7, h, 9);
                else ((void(__fastcall*)(int32_t, uint32_t, float, float, int32_t, float, int32_t))0x506ac0)(0, id, x, y, 7, h, 9);
            });
        }
        CompareLogs(logs[0], logs[1], "station draw summary", it);
    }
}

namespace rt3 {
void __fastcall Site_Load(uint8_t* o, int, int32_t s, int16_t x, int16_t y);
}

namespace {
std::vector<uint8_t> g_b48Data;
size_t g_b48Pos;
uint8_t g_b48Buf[0x400];
int32_t __fastcall FakeB48Read(int32_t s, void* dst, uint32_t size, int32_t a, int32_t b) {
    Logf("read %d %u %d %d", s, size, a, b);
    for (uint32_t k = 0; k < size; ++k) ((uint8_t*)dst)[k] = g_b48Pos < g_b48Data.size() ? g_b48Data[g_b48Pos++] : 0;
    return 1;
}
void* __cdecl FakeB48Malloc(size_t n) {
    Logf("malloc %u", (unsigned)n);
    return n <= sizeof g_b48Buf ? g_b48Buf : nullptr;
}
} // namespace

DIFFTEST(save_SiteLoad) {
    UiTrace t;
    t.Add(0x531150, (void*)&rt3::StreamRead, (void*)&FakeB48Read);
    t.Add(0x5a125d, nullptr, (void*)&FakeB48Malloc);
    static uint8_t o[0x200];
    for (int it = 0; it < 1500; ++it) {
        g_b48Data.clear();
        for (int k = 0; k < 0x14; ++k) g_b48Data.push_back((uint8_t)Rnd());
        for (int k = 0; k < 0x200; ++k) g_b48Data.push_back((uint8_t)Rnd());
        int16_t n = (int16_t)(Rnd() % 5 == 0 ? -(int)(Rnd() % 3) : Rnd() % 40);
        int16_t x = (int16_t)Rnd(), y = (int16_t)Rnd();
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            g_b48Pos = 0;
            memset(o, 0xcc, sizeof o);
            memset(g_b48Buf, 0, sizeof g_b48Buf);
            // Put the count at the position the record's fixed part ends: find it by replaying the mask.
            size_t pos = 0x14;
            for (int k = 0; k < 0x35 * 3; ++k)
                if (g_b48Data[k >> 3] & 1 << (k & 7)) pos += (k % 3 == 2) ? 4 : 2;
            pos += 12;
            g_b48Data[pos] = (uint8_t)n;
            g_b48Data[pos + 1] = (uint8_t)(n >> 8);
            logs[s] = t.Run([&] {
                if (s) rt3::Site_Load(o, 0, 5, x, y);
                else ((void(__fastcall*)(uint8_t*, int, int32_t, int16_t, int16_t))0x42ae50)(o, 0, 5, x, y);
            });
            uint32_t h = 7;
            for (int k = 0; k < 0xef; ++k) h = h * 31 + o[k];
            for (int k = 0; k < 0xf3 - 0xef; ++k) h = h * 31 + (o[0xef + k] != 0);
            for (int k = 0x103; k < 0x1d7; ++k) h = h * 31 + o[k];
            for (int k = 0; k < 0x100; ++k) h = h * 31 + g_b48Buf[k];
            logs[s] += " " + std::to_string(h) + " " + std::to_string(g_b48Pos);
        }
        CompareLogs(logs[0], logs[1], "site load", it);
    }
}

namespace rt3 {
void Block_AppendImage();
void Block_ExtractImage();
void __fastcall NameTable_Rescan(uint8_t* t);
}

namespace {
void __fastcall RecB49Rescan(uint8_t* t) { Logf("rescan %p", t); }
void CallB49Append(uint32_t fn, uint8_t* block, const char* path) {
    __asm {
        push path
        mov eax, block
        call fn
    }
}
void CallB49Extract(uint32_t fn, const char* path, uint8_t* block, char* out) {
    __asm {
        push out
        mov ecx, path
        mov eax, block
        call fn
    }
}
std::vector<uint8_t> B49ReadFile(const char* p) {
    std::vector<uint8_t> v;
    if (FILE* f = fopen(p, "rb")) {
        int c;
        while ((c = fgetc(f)) != EOF) v.push_back((uint8_t)c);
        fclose(f);
    } else v.push_back(0xee);
    return v;
}
void B49WriteFile(const char* p, const std::vector<uint8_t>& v) {
    FILE* f = fopen(p, "wb");
    fwrite(v.data(), 1, v.size(), f);
    fclose(f);
}
} // namespace

DIFFTEST(save_BlockImageAppendExtract) {
    SavedGlobals sg({ 0x62bed4, 0x62bed8, 0x6d4020 });
    UiTrace t;
    t.Add(0x53c900, (void*)&rt3::NameTable_Rescan, (void*)&RecB49Rescan);
    char dir[MAX_PATH], path[MAX_PATH], jpg[MAX_PATH], tga[MAX_PATH];
    GetTempPathA(sizeof dir, dir);
    sprintf(path, "%srt3b49.gmp", dir);
    sprintf(jpg, "%srt3b49_A.jpg", dir);
    sprintf(tga, "%srt3b49_A.tga", dir);
    static uint8_t block[0x3c2];
    void* firstBuf = nullptr;
    for (int it = 0; it < 40; ++it) {
        bool isJpg = Rnd() % 2;
        std::vector<uint8_t> base(0x3d0 + Rnd() % 300), img(Rnd() % 3000);
        for (auto& b : base) b = (uint8_t)Rnd();
        for (auto& b : img) b = (uint8_t)Rnd();
        bool blockOut = it % 5 == 4;
        std::vector<uint8_t> res[2];
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(block, 0, sizeof block);
            block[9] = (uint8_t)isJpg;
            for (int k = 0x20; k < 0x3c2; ++k) block[k] = (uint8_t)(k * 7);
            B49WriteFile(path, base);
            B49WriteFile(isJpg ? jpg : tga, img);
            B49WriteFile(isJpg ? tga : jpg, std::vector<uint8_t>(3, 1));
            logs[s] = t.Run([&] {
                CallB49Append(s ? (uint32_t)(uintptr_t)&rt3::Block_AppendImage : 0x441f70, block, path);
                Logf("offset %d", *(int32_t*)(block + 0x1c));
            });
            std::vector<uint8_t> after = B49ReadFile(path);
            logs[s] += " " + std::to_string(after.size()) + " " + std::to_string((int)B49ReadFile(jpg).size()) + "/" + std::to_string((int)B49ReadFile(tga).size());
            if (blockOut) CreateDirectoryA(isJpg ? jpg : tga, nullptr);
            char out[0x80] = "unchanged";
            hook::Global<void*>(0x62bed4) = nullptr;
            logs[s] += t.Run([&] {
                CallB49Extract(s ? (uint32_t)(uintptr_t)&rt3::Block_ExtractImage : 0x4420f0, path, block, out);
                Logf("out [%s] %d", strstr(out, "rt3b49") ? strstr(out, "rt3b49") : out, hook::Global<int32_t>(0x62bed8));
            });
            if (blockOut) RemoveDirectoryA(isJpg ? jpg : tga);
            res[s] = after;
            std::vector<uint8_t> x = B49ReadFile(isJpg ? jpg : tga);
            res[s].insert(res[s].end(), x.begin(), x.end());
            if (uint8_t* m = hook::Global<uint8_t*>(0x62bed4)) {
                res[s].insert(res[s].end(), m, m + hook::Global<int32_t>(0x62bed8));
                ((void(__cdecl*)(void*))0x5a1145)(m);
            }
            remove(jpg);
            remove(tga);
        }
        CompareLogs(logs[0], logs[1], "block image append / extract", it);
        EXPECT_EQ(res[0] == res[1], true, "block image files %d", it);
    }
    (void)firstBuf;
    remove(path);
}

namespace rt3 {
void __fastcall BuildingTypes_LoadAll(Collection* t);
void __fastcall BuildingType_Load(uint8_t* o, int, const char* path, int32_t id);
void __fastcall Industries_RebuildPortsWarehouses(Collection* c);
void __fastcall Table_SetPriorities(Collection* t);
const char* __fastcall LocalizeName(const char* name);
}

namespace {
int g_b50Files, g_b50Left;
uint8_t g_b50Recs[8][0x100];
HANDLE WINAPI FakeB50First(const char* pat, WIN32_FIND_DATAA* fd) {
    Logf("first [%s]", pat);
    g_b50Left = g_b50Files;
    if (!g_b50Left) return INVALID_HANDLE_VALUE;
    sprintf(fd->cFileName, "file%d.bca", g_b50Left--);
    return (HANDLE)0x4321;
}
BOOL WINAPI FakeB50Next(HANDLE h, WIN32_FIND_DATAA* fd) {
    if (!g_b50Left) return FALSE;
    sprintf(fd->cFileName, "file%d.bca", g_b50Left--);
    return TRUE;
}
BOOL WINAPI RecB50Close(HANDLE h) {
    Logf("close %p", h);
    return TRUE;
}
int32_t __fastcall FakeB50Add(Collection* t, int, int32_t a, int32_t b, int32_t c, int32_t d) {
    Logf("add %d %d %d %d", a, b, c, d);
    return g_b50Left + 100;
}
void* __fastcall FakeB50Get(Collection* c, int, uint32_t id) {
    Logf("get %u", id);
    return g_b50Recs[id % 8];
}
uint32_t __fastcall FakeB50IdAt(Collection* c, int, uint32_t pos, int list) { return pos * 23 % 0x80; }
void __fastcall RecB50Load(uint8_t* o, int, const char* path, int32_t id) { Logf("load %d [%s] %d", (int)((o - g_b50Recs[0]) / 0x100), path, id); }
void __fastcall RecB50Ports(Collection* c) { Logf("ports"); }
void __fastcall RecB50Prio(Collection* c) { Logf("prio"); }
void __fastcall RecB50Avail(Collection* c) { Logf("avail"); }
const char* __fastcall FakeB50Loc(const char* n) {
    Logf("loc [%s]", n);
    return n;
}
char* __fastcall FakeB50Format(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                               const char* a6, const char* a7, const char* a8) {
    sprintf(dst, "%s|%s|%s", fmt, a1 ? a1 : "-", a2 ? a2 : "-");
    return dst;
}
} // namespace

DIFFTEST(load_BuildingTypesAll) {
    static uint8_t savedNames[0x6f * 0x6e + 8];
    memcpy(savedNames, (void*)0x61dbc2, sizeof savedNames);
    IatSlot i1(0x5c8190, (void*)&FakeB50First), i2(0x5c8194, (void*)&FakeB50Next), i3(0x5c819c, (void*)&RecB50Close);
    UiTrace t;
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB18Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB50IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB50Get);
    t.Add(0x4120b0, (void*)&rt3::BuildingType_Load, (void*)&RecB50Load);
    t.Add(0x412d70, (void*)&rt3::Industries_RebuildPortsWarehouses, (void*)&RecB50Ports);
    t.Add(0x412ab0, (void*)&rt3::Table_SetPriorities, (void*)&RecB50Prio);
    t.Add(0x412c10, nullptr, (void*)&RecB50Avail);
    t.Add(0x51c920, (void*)&rt3::LocalizeName, (void*)&FakeB50Loc);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB11String);
    t.Add(0x518de0, (void*)&rt3::FormatText, (void*)&FakeB50Format);
    static void* vt[4];
    vt[1] = (void*)&FakeB50Add;
    static uint8_t table[0x100];
    *(void***)table = vt;
    for (int it = 0; it < 600; ++it) {
        g_b50Files = (int)(Rnd() % 4);
        g_b18Count = (int32_t)(Rnd() % 9) - 1;
        for (int k = 0; k < 8; ++k) {
            uint8_t* r = g_b50Recs[k];
            r[0x32] = (uint8_t)(Rnd() % 4);
            r[0xba] = (uint8_t)(Rnd() % 4 == 0);
            r[0xbb] = (uint8_t)(Rnd() % 4 == 0);
            for (int f = 0; f < 5; ++f) *(int32_t*)(r + 0x8d + f * 4) = (int32_t)(Rnd() % 1000) - 100;
            sprintf((char*)r + 0x4f, "Type%d", k);
        }
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(table + 4, 0xaa, sizeof table - 4);
            memset((void*)0x61dbc2, 0, sizeof savedNames);
            logs[s] = t.Run([&] {
                if (s) rt3::BuildingTypes_LoadAll((Collection*)table);
                else ((void(__fastcall*)(void*))0x412fb0)(table);
            });
            for (int k = 0; k < 5; ++k) logs[s] += " " + std::to_string(*(int32_t*)(table + 0x8c + k * 4));
            uint32_t h = 7;
            for (size_t k = 0; k < sizeof savedNames; ++k) h = h * 31 + ((uint8_t*)0x61dbc2)[k];
            logs[s] += " " + std::to_string(h);
        }
        CompareLogs(logs[0], logs[1], "building types load all", it);
    }
    memcpy((void*)0x61dbc2, savedNames, sizeof savedNames);
}

namespace rt3 {
void __fastcall City_ResetBuildingMix(uint8_t* c);
uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount);
void __fastcall Collection_Destruct(Collection* c);
Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy,
                                            int32_t list1, int32_t list2, int32_t list3);
}

namespace {
uint32_t __fastcall RecB51Set(void* o, int, const char* name, float a) {
    Logf("set [%s] %08x", name, B(a));
    return 0;
}
void __fastcall RecB51Destruct(Collection* c) { Logf("destruct %p", c); }
Collection* __fastcall RecB51Construct(Collection* c, int, int32_t a, int32_t b, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h) {
    Logf("construct %d %d %d %d %d %d %d", a, b, d, e, f, g, h);
    return c;
}
} // namespace

DIFFTEST(city_ResetBuildingMix) {
    UiTrace t;
    t.Add(0x4206b0, (void*)&rt3::Obj_SetNamedAmount, (void*)&RecB51Set);
    t.Add(0x518bd0, (void*)&rt3::Collection_Destruct, (void*)&RecB51Destruct);
    t.Add(0x518b90, (void*)&rt3::Collection_Construct, (void*)&RecB51Construct);
    t.Add(0x53b080, nullptr, (void*)&RecB11Delete);
    static uint8_t city[0x400], old[0x88];
    for (int it = 0; it < 8; ++it) {
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(city, 0, sizeof city);
            *(int32_t*)(city + 0x23e) = it & 1;
            *(uint8_t**)(city + 0x37f) = it & 2 ? old : nullptr;
            logs[s] = t.Run([&] {
                if (s) rt3::City_ResetBuildingMix(city);
                else ((void(__fastcall*)(uint8_t*))0x420ed0)(city);
                Logf("table %d", (int)(*(void**)(city + 0x37f) != nullptr));
            });
        }
        CompareLogs(logs[0], logs[1], "city reset building mix", it);
    }
}

namespace rt3 {
void __fastcall Train_AutoName(uint8_t* t, int, uint32_t a, uint32_t b);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_BuildingKind(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
}

namespace {
uint8_t g_b52City[4][0x400], g_b52St[8][0x10], g_b52Map[0x2200];
uint8_t g_b52Cities[8], g_b52Sts[8];
int32_t g_b52Count, g_b52Owner[8], g_b52Active[8], g_b52Kind[8], g_b52Rand[4], g_b52RandPos;
double g_b52X[4], g_b52Y[4];
char g_b52Str[4][80];
void* __fastcall FakeB52Get(void* c, int, uint32_t id) {
    Logf("get %d %u", c == g_b52Cities ? 0 : 1, id);
    return c == g_b52Cities ? (void*)g_b52City[id % 4] : (void*)g_b52St[id % 8];
}
int32_t __fastcall FakeB52Count(void*) { return g_b52Count; }
uint32_t __fastcall FakeB52IdAt(void*, int, uint32_t pos, int list) { return pos * 3 + list; }
int32_t __fastcall FakeB52Value(void* o) { return g_b52Owner[((uint8_t*)o - g_b52St[0]) / 0x10]; }
int32_t __fastcall FakeB52Active(void* o) { return g_b52Active[((uint8_t*)o - g_b52St[0]) / 0x10]; }
void* __fastcall FakeB52Kind(void* o) { return g_b52City[g_b52Kind[((uint8_t*)o - g_b52St[0]) / 0x10]]; }
double __fastcall FakeB52CellX(void* o) { return g_b52X[((uint8_t*)o - g_b52City[0]) / 0x400]; }
double __fastcall FakeB52CellY(void* o) { return g_b52Y[((uint8_t*)o - g_b52City[0]) / 0x400]; }
int32_t __fastcall FakeB52Rand(int32_t lo, int32_t hi) {
    Logf("rand %d %d", lo, hi);
    return g_b52Rand[g_b52RandPos++ & 3];
}
const char* __fastcall FakeB52String(int32_t id) {
    Logf("string %x", id);
    return g_b52Str[id & 3];
}
void B52Fill(char* s, size_t n, const char* head) {
    strcpy(s, head);
    while (strlen(s) < n) strcat(s, "x");
}
} // namespace

DIFFTEST(train_AutoName) {
    SavedGlobals sg({ 0x62bae0, 0x6cec20, 0x62c120 });
    UiTrace t;
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB52Get);
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB52Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB52IdAt);
    t.Add(0x47efe0, (void*)&rt3::Obj_BuildingValue, (void*)&FakeB52Value);
    t.Add(0x47fd50, (void*)&rt3::Obj_BuildingStateActive, (void*)&FakeB52Active);
    t.Add(0x480160, (void*)&rt3::Obj_BuildingKind, (void*)&FakeB52Kind);
    t.Add(0x455800, (void*)&rt3::Obj_CellX, (void*)&FakeB52CellX);
    t.Add(0x455810, (void*)&rt3::Obj_CellY, (void*)&FakeB52CellY);
    t.Add(0x518be0, (void*)&rt3::SimRand, (void*)&FakeB52Rand);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB52String);
    hook::Global<uint8_t*>(0x62bae0) = g_b52Cities;
    hook::Global<uint8_t*>(0x6cec20) = g_b52Sts;
    hook::Global<uint8_t*>(0x62c120) = g_b52Map;
    static uint8_t tr[0xe00];
    for (int it = 0; it < 4000; ++it) {
        for (int c = 0; c < 4; ++c) {
            *(float*)(g_b52City[c] + 0x25e) = (float)(Rnd() % 5);
            B52Fill((char*)g_b52City[c] + 0x356, 1 + Rnd() % 30, c & 1 ? "Alpha" : "Bo");
            g_b52X[c] = RndF(-10.0f, 300.0f);
            g_b52Y[c] = RndF(-10.0f, 300.0f);
            B52Fill(g_b52Str[c], Rnd() % 50, Rnd() % 4 ? "%1 " : "");
            g_b52Rand[c] = Rnd() % 101;
        }
        g_b52Count = Rnd() % 9;
        for (int k = 0; k < 8; ++k) {
            *(uint16_t*)g_b52St[k] = (uint16_t)(100 + k);
            g_b52Owner[k] = Rnd() % 3;
            g_b52Active[k] = Rnd() % 2;
            g_b52Kind[k] = Rnd() % 4;
        }
        *(int32_t*)(g_b52Map + 0x2155) = Rnd() % 500 - 5;
        *(int32_t*)(g_b52Map + 0x2159) = Rnd() % 500 - 5;
        uint32_t a = Rnd() % 4, b = Rnd() % 4;
        int32_t owner = Rnd() % 3;
        uint16_t pre = Rnd() % 2 ? 0 : 7;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(tr, 0xcc, sizeof tr);
            *(int32_t*)tr = owner;
            *(uint16_t*)(tr + 0xd57) = pre;
            g_b52RandPos = 0;
            logs[s] = t.Run([&] {
                if (s) rt3::Train_AutoName(tr, 0, a, b);
                else ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))0x426f20)(tr, 0, a, b);
                Logf("name [%.60s] st %u", (char*)tr + 4, *(uint16_t*)(tr + 0xd57));
            });
        }
        CompareLogs(logs[0], logs[1], "train auto name", it);
    }
}

namespace rt3 {
void __fastcall RulesDialog_FillEffectTypes(uint8_t* w, int, int32_t sel);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
}

namespace {
uint8_t g_b53Child[0x100];
uint8_t* __fastcall FakeB53Find(uint8_t* w, int, int32_t id) {
    Logf("find %x", id);
    return g_b53Child;
}
int32_t __fastcall FakeB53Send(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Logf("send %x %x %x %x %x", type, id, a, b, c);
    return 0;
}
} // namespace

DIFFTEST(ui_RulesFillEffectTypes) {
    SavedGlobals sg({ 0x6d07bc });
    UiTrace t;
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB53Find);
    t.Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&FakeB53Send);
    uint8_t* table = (uint8_t*)0x5f3d90;
    const size_t n = 0x385 * 0x81;
    std::vector<uint8_t> saved(table, table + n);
    DWORD old;
    VirtualProtect(table, n, PAGE_READWRITE, &old);
    static uint8_t w[0x100];
    for (int it = 0; it < 60; ++it) {
        for (int i = 0; i < 0x385; ++i) {
            uint8_t* r = table + i * 0x81;
            *(float*)r = (float)(Rnd() % 40) * 0.5f;
            if (Rnd() % 50 == 0) *(uint32_t*)r = 0x7fc00000;
            *(int32_t*)(r + 0x68) = Rnd() % 4 == 0;
            *(int32_t*)(r + 0x64) = Rnd() % 3 != 0;
            *(int32_t*)(r + 0x58) = Rnd() % 2;
        }
        *(int32_t*)(g_b53Child + 0xcc) = Rnd() % 2;
        int32_t sel = Rnd() % 0x390 - 2;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(w, 0, sizeof w);
            logs[s] = t.Run([&] {
                if (s) rt3::RulesDialog_FillEffectTypes(w, 0, sel);
                else ((void(__fastcall*)(uint8_t*, int, int32_t))0x4d4850)(w, 0, sel);
                Logf("effect %d filter %d", *(int32_t*)(w + 0x78), hook::Global<int32_t>(0x6d07bc));
            });
        }
        CompareLogs(logs[0], logs[1], "rules fill effect types", it);
    }
    memcpy(table, saved.data(), n);
    VirtualProtect(table, n, old, &old);
}

namespace rt3 {
int32_t __fastcall Train_UpdateApproachFactor(uint8_t* o, int, uint8_t* obj);
void Train_StepsToNode();
int32_t IsMultiplayer();
int32_t __fastcall Game_EffectiveVersion();
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void* __fastcall Obj_CityRef(void* o);
}

namespace {
int32_t g_b54Steps, g_b54Valid, g_b54Mp, g_b54Ver;
float g_b54Node[2][2], g_b54Pos[3];
uint8_t g_b54Nodes[8], g_b54Sts[8], g_b54NodeRec[2][0x20], g_b54City[0x40];
__declspec(naked) void FakeB54Steps() {
    __asm {
        mov eax, g_b54Steps
        ret 8
    }
}
int32_t __fastcall FakeB54Valid(void* c, int, uint32_t id) {
    Logf("valid %d %u", c == g_b54Nodes, id);
    return (g_b54Valid >> (c == g_b54Nodes)) & 1;
}
void* __fastcall FakeB54Get(void* c, int, uint32_t id) {
    Logf("get %u", id);
    uint8_t* r = g_b54NodeRec[id & 1];
    *(float*)(r + 0xc) = g_b54Node[id & 1][0];
    *(float*)(r + 0x10) = g_b54Node[id & 1][1];
    return r;
}
int32_t FakeB54Mp() { return g_b54Mp; }
int32_t __fastcall FakeB54Ver() { return g_b54Ver; }
void __fastcall FakeB54Pos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    memcpy(x, &g_b54Pos[0], 4);
    memcpy(y, &g_b54Pos[1], 4);
    memcpy(z, &g_b54Pos[2], 4);
}
void* __fastcall FakeB54City(void* o) {
    Logf("city");
    return g_b54City;
}
} // namespace

DIFFTEST(train_UpdateApproachFactor) {
    SavedGlobals sg({ 0x6cec20, 0x6cfca8 });
    UiTrace t;
    t.Add(0x4a78d0, (void*)&rt3::Train_StepsToNode, (void*)&FakeB54Steps);
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeB54Valid);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB54Get);
    t.Add(0x434050, (void*)&rt3::IsMultiplayer, (void*)&FakeB54Mp);
    t.Add(0x482e00, (void*)&rt3::Game_EffectiveVersion, (void*)&FakeB54Ver);
    t.Add(0x52e720, (void*)&rt3::SceneObj_GetPos, (void*)&FakeB54Pos);
    t.Add(0x4a8840, (void*)&rt3::Obj_CityRef, (void*)&FakeB54City);
    hook::Global<uint8_t*>(0x6cec20) = g_b54Sts;
    hook::Global<uint8_t*>(0x6cfca8) = g_b54Nodes;
    static uint8_t o[0x200], obj[0x40];
    for (int it = 0; it < 20000; ++it) {
        uint8_t base[0x200];
        memset(base, 0, sizeof base);
        base[0x8b] = 1;
        *(int32_t*)(base + 0xac) = Rnd() % 10 == 0 ? -1 : 3;
        *(int32_t*)(base + 0x94) = Rnd() % 10 == 0 ? -1 : (int32_t)(Rnd() % 2);
        *(float*)(base + 0x8c) = Rnd() % 6 == 0 ? -1.0f : RndF(0.0f, 4.0f);
        *(float*)(base + 0xcc) = RndF(0.0f, 4.0f);
        *(int32_t*)(base + 0x49) = Rnd() % 5;
        int k = Rnd() % 4;
        *(int32_t*)(base + 0x1ca) = k == 0 ? -1 : k == 1 ? 0 : (int32_t)(Rnd() % 2 + 1);
        *(int32_t*)(base + 0x90) = Rnd() % 8 == 0 ? -(int32_t)(Rnd() % 5) : (int32_t)(Rnd() % 12);
        *(int32_t*)(base + 0xe8) = Rnd() % 20;
        g_b54Steps = Rnd() % 8 == 0 ? -1 : Rnd() % 10 == 0 ? -7 : (int32_t)(Rnd() % 12);
        g_b54Valid = Rnd() % 4;
        if (Rnd() % 2) g_b54Valid = 3;
        g_b54Mp = Rnd() % 2;
        g_b54Ver = 0x60 + Rnd() % 16;
        for (int n = 0; n < 2; ++n)
            for (int c = 0; c < 2; ++c) g_b54Node[n][c] = Rnd() % 6 == 0 ? 0.0f : RndF(-50.0f, 50.0f);
        if (Rnd() % 8 == 0) memcpy(g_b54Node[1], g_b54Node[0], 8);
        for (int c = 0; c < 3; ++c) g_b54Pos[c] = RndF(-60.0f, 60.0f);
        *(float*)(g_b54City + 0x20) = RndF(0.5f, 3.0f);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memcpy(o, base, sizeof o);
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Train_UpdateApproachFactor(o, 0, obj)
                              : ((int32_t(__fastcall*)(uint8_t*, int, uint8_t*))0x4aa590)(o, 0, obj);
                Logf("r %d 8b %d 8c %08x cc %08x 1ca %d", r, o[0x8b], *(uint32_t*)(o + 0x8c), *(uint32_t*)(o + 0xcc),
                     *(int32_t*)(o + 0x1ca));
            });
        }
        CompareLogs(logs[0], logs[1], "train update approach factor", it);
    }
}

namespace rt3 {
int32_t __fastcall Hotkeys_Save(const char* path);
uint8_t* Models_NextInOrder();
}

namespace {
uint8_t g_b55Rec[12][0x110];
int g_b55Count, g_b55Pos;
uint8_t* FakeB55Next() {
    Logf("next");
    if (g_b55Pos < g_b55Count) return g_b55Rec[g_b55Pos++];
    g_b55Pos = 0;
    return nullptr;
}
char g_b55Str[64];
const char* __fastcall FakeB55String(int32_t id) {
    sprintf(g_b55Str, "; header %x\n", id);
    return g_b55Str;
}
std::string B55ReadFile(const char* p) {
    std::string s;
    FILE* f = fopen(p, "rb");
    if (!f) return "<none>";
    char b[512];
    size_t n;
    while ((n = fread(b, 1, sizeof b, f)) > 0) s.append(b, n);
    fclose(f);
    return s;
}
} // namespace

DIFFTEST(misc_HotkeysSave) {
    UiTrace t;
    t.Add(0x45f250, (void*)&rt3::Models_NextInOrder, (void*)&FakeB55Next);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB55String);
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_hotkeys_test.txt");
    for (int it = 0; it < 200; ++it) {
        g_b55Count = Rnd() % 12;
        for (int k = 0; k < g_b55Count; ++k) {
            uint8_t* r = g_b55Rec[k];
            memset(r, 0, sizeof g_b55Rec[k]);
            *(int32_t*)r = Rnd() % 4 == 0;
            int nl = Rnd() % 40, fl = Rnd() % 50;
            for (int c = 0; c < nl; ++c) r[8 + c] = (uint8_t)('a' + Rnd() % 26);
            for (int c = 0; c < fl; ++c) r[0x88 + c] = (uint8_t)('A' + Rnd() % 26);
        }
        bool bad = it % 25 == 7;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            DeleteFileA(path);
            g_b55Pos = 0;
            const char* p = bad ? "Z:/no/such/dir/x.txt" : path;
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Hotkeys_Save(p) : ((int32_t(__fastcall*)(const char*))0x45f550)(p);
                Logf("r %d", r);
            });
            logs[s] += B55ReadFile(path);
        }
        CompareLogs(logs[0], logs[1], "hotkeys save", it);
    }
    DeleteFileA(path);
}

namespace rt3 {
void Models_Shutdown();
int32_t __fastcall Hotkeys_Save(const char* path);
void __fastcall Pool_Free(NodePool* p);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Heap_Destroy2(MaxHeap* h);
Collection* __fastcall BuildingTypes_Construct(Collection* t);
void __fastcall BuildingTypes_LoadAll(Collection* t);
}

namespace {
int32_t __fastcall FakeB56Save(const char* path) {
    Logf("save [%s] pool %d", path, hook::Global<void*>(0x6acd6c) != nullptr);
    return 1;
}
uint8_t g_b56Pool[0x20], g_b56Hash[0x20], g_b56Heap[0x20], g_b56Block[8];
const char* B56Name(void* p) {
    return p == g_b56Pool ? "pool" : p == g_b56Hash ? "hash" : p == g_b56Heap ? "heap" : p == g_b56Block ? "block" : "?";
}
void __cdecl FakeB56Free(void* p) { Logf("free %s", B56Name(p)); }
void __cdecl FakeB56Delete(void* p) { Logf("delete %s", B56Name(p)); }
void __fastcall FakeB56PoolFree(rt3::NodePool* p) { Logf("pool free %s", B56Name(p)); }
void __fastcall FakeB56Hash(rt3::HashMap* m) { Logf("hash destroy %s", B56Name(m)); }
void __fastcall FakeB56Heap(rt3::MaxHeap* h) { Logf("heap destroy %s", B56Name(h)); }
void __fastcall FakeB57Init(rt3::Collection* c, int, int32_t a, int32_t b, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h) {
    Logf("init vt %08x %d %x %d %d %d %d %d", *(uint32_t*)c, a, b, d, e, f, g, h);
}
void __fastcall FakeB57Load(rt3::Collection* c) { Logf("load vt %08x", *(uint32_t*)c); }
} // namespace

DIFFTEST(misc_ModelsShutdown) {
    SavedGlobals sg({ 0x6acd6c, 0x6acd70, 0x6acd74 });
    uint8_t* buckets = (uint8_t*)0x6acd78;
    const size_t n = 0x6ad978 - 0x6acd78;
    std::vector<uint8_t> saved(buckets, buckets + n);
    UiTrace t;
    t.Add(0x45f550, (void*)&rt3::Hotkeys_Save, (void*)&FakeB56Save);
    t.Add(0x5a1145, nullptr, (void*)&FakeB56Free);
    t.Add(0x53b080, nullptr, (void*)&FakeB56Delete);
    t.Add(0x53dcb0, (void*)&rt3::Pool_Free, (void*)&FakeB56PoolFree);
    t.Add(0x53dbe0, (void*)&rt3::Hash_DestroyThunk, (void*)&FakeB56Hash);
    t.Add(0x53d6f0, (void*)&rt3::Heap_Destroy2, (void*)&FakeB56Heap);
    for (int it = 0; it < 16; ++it) {
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<void*>(0x6acd6c) = it & 1 ? g_b56Pool : nullptr;
            *(void**)(g_b56Pool + 4) = it & 2 ? g_b56Block : nullptr;
            hook::Global<void*>(0x6acd70) = it & 4 ? g_b56Hash : nullptr;
            hook::Global<void*>(0x6acd74) = it & 8 ? g_b56Heap : nullptr;
            for (size_t k = 0; k < n; ++k) buckets[k] = (uint8_t)(k * 7 + it);
            logs[s] = t.Run([&] {
                if (s) rt3::Models_Shutdown();
                else ((void(*)())0x461070)();
                uint32_t sum = 0;
                for (size_t k = 0; k < n; ++k) sum = sum * 31 + buckets[k];
                Logf("globals %p %p %p buckets %08x", hook::Global<void*>(0x6acd6c), hook::Global<void*>(0x6acd70),
                     hook::Global<void*>(0x6acd74), sum);
            });
        }
        CompareLogs(logs[0], logs[1], "models shutdown", it);
    }
    memcpy(buckets, saved.data(), n);
}

DIFFTEST(load_BuildingTypesConstruct) {
    UiTrace t;
    t.Add(0x518570, (void*)&rt3::Collection_Init, (void*)&FakeB57Init);
    t.Add(0x412fb0, (void*)&rt3::BuildingTypes_LoadAll, (void*)&FakeB57Load);
    static uint8_t c[0x100];
    std::string logs[2];
    for (int s = 0; s < 2; ++s) {
        memset(c, 0xcc, sizeof c);
        logs[s] = t.Run([&] {
            void* r = s ? (void*)rt3::BuildingTypes_Construct((rt3::Collection*)c)
                        : ((void*(__fastcall*)(void*))0x4131f0)(c);
            Logf("r %d vt %08x", r == c, *(uint32_t*)c);
        });
    }
    CompareLogs(logs[0], logs[1], "building types construct", 0);
}

namespace rt3 {
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                         int32_t serial);
int32_t Global6d4100();
int32_t __stdcall Peer_SendRoom(const char* msg);
void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size);
void* Engine_FirstListEntry();
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
int32_t ClockMs();
int32_t __fastcall Net_SendTo(uint8_t* net, int, const uint8_t* data, uint16_t size, uint8_t type, uint8_t channel, const char* to,
                              uint32_t flags, uint32_t timeout, uint32_t serial, int32_t);
}

namespace {
int32_t g_b58Global;
uint8_t g_b58Pkt[0x2000], g_b58Entry[0x80], g_b58Net[8];
void* g_b58Queue[3];
int32_t FakeB58Global() { return g_b58Global; }
int32_t __stdcall FakeB58Room(const char* msg) {
    Logf("room [%.40s]", msg);
    return 0;
}
void* __fastcall FakeB58Alloc(uint8_t* s, int, int32_t size) {
    Logf("alloc %d", size);
    memset(g_b58Pkt, 0xcd, sizeof g_b58Pkt);
    return g_b58Pkt;
}
void* FakeB58Entry() { return g_b58Entry; }
uint32_t B58Hash(const uint8_t* p, int32_t n) {
    uint32_t h = 2166136261u;
    for (int32_t k = 0; k < n; ++k) h = (h ^ p[k]) * 16777619u;
    return h;
}
void __fastcall FakeB58Insert(void** l, int, void** after, void** node) {
    uint8_t* p = (uint8_t*)node;
    int32_t n = *(int32_t*)(p + 0xc);
    Logf("queue %d %d after %d flags %x type %x size %d data %d hash %08x", l == g_b58Queue, p == g_b58Pkt, after == g_b58Queue[1],
         *(uint32_t*)(p + 0x10), *(uint32_t*)(p + 0x14), n, *(uint8_t**)(p + 8) == p + 0x28, B58Hash(p, 0x28 + (n > 0 ? n : 0)));
}
int32_t FakeB58Clock() { return 12345; }
int32_t __fastcall FakeB58SendTo(void* net, int, const uint8_t* data, uint32_t size, uint32_t type, uint32_t ch, const char* to,
                                 uint32_t flags, uint32_t timeout, int32_t serial, int32_t now) {
    Logf("sendto %d data %d size %u type %x ch %x to %p flags %x timeout %u serial %d now %d lock %d hash %08x", net == g_b58Net,
         data != nullptr, size, type, ch, to, flags, timeout, serial, now, *(volatile LONG*)0x6cd960,
         data ? B58Hash(data, (int32_t)(size & 0xffff)) : 0);
    return 0;
}
} // namespace

DIFFTEST(net_Send) {
    SavedGlobals sg({ 0x6cd960, 0x6d40dc, 0x6d40d0, 0x6cd930, 0x6cd934 });
    UiTrace t;
    t.Add(0x521670, (void*)&rt3::Global6d4100, (void*)&FakeB58Global);
    t.Add(0x469660, (void*)&rt3::Peer_SendRoom, (void*)&FakeB58Room);
    t.Add(0x469800, (void*)&rt3::NetPacket_Alloc, (void*)&FakeB58Alloc);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB58Entry);
    t.Add(0x469690, (void*)&rt3::SList_InsertAfter, (void*)&FakeB58Insert);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeB58Clock);
    t.Add(0x521dc0, (void*)&rt3::Net_SendTo, (void*)&FakeB58SendTo);
    hook::Global<void*>(0x6d40d0) = g_b58Net;
    hook::Global<LONG>(0x6cd960) = 0;
    *(void***)(g_b58Entry + 0x64) = (void**)g_b58Queue;
    g_b58Queue[1] = &g_b58Queue[2];
    static uint8_t s[0x9900], data[0x400];
    for (int it = 0; it < 3000; ++it) {
        memset(s, 0, sizeof s);
        *(int32_t*)(s + 0x985c) = Rnd() % 3 == 0;
        *(uint32_t*)(s + 0x8f38) = Rnd();
        g_b58Global = Rnd() % 2;
        hook::Global<int32_t>(0x6d40dc) = Rnd() % 2;
        uint32_t type = Rnd() % 4 == 0 ? 0x10001 * (Rnd() % 2) + 1 : Rnd() % 0x30;
        int32_t size = Rnd() % 5 == 0 ? 0 : Rnd() % 6 == 0 ? (int32_t)(Rnd() % 0x3f0) : (int32_t)(Rnd() % 0x50);
        int mode = Rnd() % 3;
        for (int k = 0; k < (int)sizeof data; ++k) data[k] = (uint8_t)(mode == 0 ? Rnd() : mode == 1 ? k % 7 : 'a' + k % 3);
        data[sizeof data - 1] = 0;
        const uint8_t* d = Rnd() % 10 == 0 ? nullptr : data;
        if (!d) size = 0;
        uint32_t flags = Rnd() % 3 == 0 ? 0 : Rnd() % 0x40;
        int32_t serial = Rnd() % 2 ? 0 : (int32_t)(Rnd() % 100);
        const char* to = Rnd() % 2 ? nullptr : "Bob";
        std::string logs[2];
        for (int sd = 0; sd < 2; ++sd) {
            hook::Global<int32_t>(0x6cd930) = 0;
            hook::Global<int32_t>(0x6cd934) = 0;
            logs[sd] = t.Run([&] {
                if (sd) rt3::Net_Send(s, 0, to, type, size, d, flags, serial);
                else
                    ((void(__fastcall*)(uint8_t*, int, const char*, uint32_t, int32_t, const uint8_t*, uint32_t, int32_t))0x469d30)(
                        s, 0, to, type, size, d, flags, serial);
                Logf("totals %d %d lock %d", hook::Global<int32_t>(0x6cd930), hook::Global<int32_t>(0x6cd934),
                     *(volatile LONG*)0x6cd960);
            });
        }
        CompareLogs(logs[0], logs[1], "net send", it);
    }
}

namespace rt3 {
void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
const char* __fastcall Options_SaveFolder(void* opt);
int32_t __fastcall Options_Check1bf(void* self);
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall StreamClose(int32_t s);
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset);
}

namespace {
int32_t g_b59Check, g_b59Version, g_b59W, g_b59H;
uint32_t g_b59Seed;
const char* __fastcall FakeB59Folder(void*) { return "maps"; }
int32_t __fastcall FakeB59Check(void*) { return g_b59Check; }
int32_t __fastcall FakeB59Open(const char* path, int32_t mode, int32_t size, int32_t fatal) {
    Logf("open [%s] %d %x %d", path, mode, size, fatal);
    return 4;
}
int32_t __fastcall FakeB59Read(int32_t s, void* dst, uint32_t size, int32_t a, int32_t b) {
    Logf("read %d %x %d %d", s, size, a, b);
    if (size == 4) {
        memcpy(dst, &g_b59Version, 4);
        return 1;
    }
    uint32_t v = g_b59Seed;
    for (uint32_t k = 0; k < size; ++k) {
        v = v * 1103515245u + 12345u;
        ((uint8_t*)dst)[k] = (uint8_t)(v >> 16);
    }
    *(int32_t*)((uint8_t*)dst + 0x18) = 0x1234;
    return 1;
}
int32_t __fastcall FakeB59Skip(int32_t s, int tag, int32_t a, int32_t count) {
    Logf("skip %d %x %d %d", s, tag, a, count);
    return 1;
}
void __fastcall FakeB59Close(int32_t s) { Logf("close %d", s); }
uint8_t* __fastcall FakeB59Jpeg(const char* path, int32_t* w, int32_t* h, int32_t off) {
    Logf("jpeg [%s] %x", path, off);
    *w = g_b59W;
    *h = g_b59H;
    uint8_t* p = ((uint8_t*(__cdecl*)(size_t))0x5a125d)((size_t)(g_b59W * g_b59H * 3 + 1));
    for (int32_t k = 0; k < g_b59W * g_b59H * 3; ++k) p[k] = (uint8_t)(k * 37 + g_b59Seed);
    return p;
}
} // namespace

DIFFTEST(save_BlockLoad) {
    UiTrace t;
    t.Add(0x4839e0, (void*)&rt3::Options_SaveFolder, (void*)&FakeB59Folder);
    t.Add(0x4839b0, (void*)&rt3::Options_Check1bf, (void*)&FakeB59Check);
    t.Add(0x530c80, (void*)&rt3::StreamOpen, (void*)&FakeB59Open);
    t.Add(0x531150, (void*)&rt3::StreamRead, (void*)&FakeB59Read);
    t.Add(0x531360, (void*)&rt3::StreamSkip4, (void*)&FakeB59Skip);
    t.Add(0x530a00, (void*)&rt3::StreamClose, (void*)&FakeB59Close);
    t.Add(0x552e20, (void*)&rt3::Jpeg_Load, (void*)&FakeB59Jpeg);
    static uint8_t block[0x403c2];
    const int32_t versions[] = { 0x3e8, 0x3f0, 0x3f1, 0x3f6, 0x3f7, 0x400 };
    for (int it = 0; it < 40; ++it) {
        g_b59Check = it & 1;
        g_b59Version = versions[(it >> 1) % 6];
        g_b59Seed = Rnd();
        g_b59W = Rnd() % 80;
        g_b59H = Rnd() % 60;
        int32_t tutorial = Rnd() % 2;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(block, 0xee, sizeof block);
            logs[s] = t.Run([&] {
                if (s) rt3::Block_Load("Test Map", block, tutorial);
                else ((void(__fastcall*)(const char*, uint8_t*, int32_t))0x442400)("Test Map", block, tutorial);
                uint32_t h = 2166136261u;
                for (size_t k = 0; k < sizeof block; ++k) h = (h ^ block[k]) * 16777619u;
                Logf("block %08x", h);
            });
        }
        CompareLogs(logs[0], logs[1], "block load", it);
    }
}

namespace rt3 {
void __fastcall Save_UnpackToTemp(const char* path);
void Save_Pack();
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
}

namespace {
void B60Write(const char* p, const std::vector<uint8_t>& v) {
    FILE* f = fopen(p, "wb");
    if (!v.empty()) fwrite(v.data(), 1, v.size(), f);
    fclose(f);
}
std::string B60Read(const char* p) {
    FILE* f = fopen(p, "rb");
    if (!f) return "<none>";
    std::string s;
    char b[4096];
    size_t n;
    while ((n = fread(b, 1, sizeof b, f)) > 0) s.append(b, n);
    fclose(f);
    char h[32];
    uint32_t x = 2166136261u;
    for (unsigned char c : s) x = (x ^ c) * 16777619u;
    sprintf(h, "%u bytes %08x", (unsigned)s.size(), x);
    return h;
}
std::vector<uint8_t> B60Payload(int len) {
    std::vector<uint8_t> v((size_t)len);
    int mode = Rnd() % 3;
    for (int k = 0; k < len; ++k) v[(size_t)k] = (uint8_t)(mode == 0 ? Rnd() : mode == 1 ? k / 5 : (k * k) >> 3);
    return v;
}
void __fastcall FakeB60Progress(int32_t pct) { Logf("progress %d", pct); }
// Calls the save packer at fn (block in EAX, path on the stack).
void B60CallPack(uint32_t fn, uint8_t* block, const char* path) {
    __asm {
        push path
        mov eax, block
        call fn
    }
}
// Runs fn with the current directory set to %TEMP% (the original's name buffer
// only holds 0x30 characters, so the test file names are relative).
struct B60TempDir {
    char old[MAX_PATH];
    B60TempDir() {
        GetCurrentDirectoryA(MAX_PATH, old);
        char t[MAX_PATH];
        GetTempPathA(MAX_PATH, t);
        SetCurrentDirectoryA(t);
    }
    ~B60TempDir() { SetCurrentDirectoryA(old); }
};
} // namespace

DIFFTEST(save_UnpackToTemp) {
    B60TempDir dir;
    UiTrace t;
    for (int it = 0; it < 30; ++it) {
        std::vector<uint8_t> file = B60Payload(0x3ce);
        std::vector<uint8_t> payload = B60Payload(1 + Rnd() % 20000);
        uint8_t* packed = nullptr;
        int32_t packedSize = 0;
        int32_t packer[3] = { 0x91, 0, 0 };
        rt3::Pack_Compress(packer, 0, payload.data(), (int32_t)payload.size(), &packed, &packedSize);
        file.insert(file.end(), packed, packed + packedSize);
        ((void(__cdecl*)(void*))0x5a1145)(packed);
        if (it % 7 == 3) file[0x3ce] ^= 0x55;
        std::vector<uint8_t> tail = B60Payload(Rnd() % 3000);
        file.insert(file.end(), tail.begin(), tail.end());
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            B60Write("rt3re_sv.gm", file);
            DeleteFileA("rt3re_sv.tmp");
            logs[s] = t.Run([&] {
                if (s) rt3::Save_UnpackToTemp("rt3re_sv.gm");
                else ((void(__fastcall*)(const char*))0x4427c0)("rt3re_sv.gm");
            });
            logs[s] += B60Read("rt3re_sv.tmp") + " / " + B60Read("rt3re_sv.gm");
        }
        CompareLogs(logs[0], logs[1], "save unpack to temp", it);
    }
    DeleteFileA("rt3re_sv.gm");
    DeleteFileA("rt3re_sv.tmp");
}

DIFFTEST(save_Pack) {
    B60TempDir dir;
    SavedGlobals sg({ 0xd93988 });
    UiTrace t;
    t.Add(0x441e20, nullptr, (void*)&FakeB60Progress);
    static uint8_t block[0x3c2];
    for (int it = 0; it < 30; ++it) {
        std::vector<uint8_t> file = B60Payload(0x3ce);
        int32_t len = Rnd() % 20000, mid = Rnd() % 500;
        std::vector<uint8_t> body = B60Payload(len + mid + Rnd() % 2000);
        file.insert(file.end(), body.begin(), body.end());
        std::vector<uint8_t> blockInit = B60Payload(0x3c2);
        *(int32_t*)(blockInit.data() + 0x18) = 0x3ce + len;
        *(int32_t*)(blockInit.data() + 0x1c) = 0x3ce + len + mid;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            B60Write("rt3re_pk.gm", file);
            memcpy(block, blockInit.data(), sizeof block);
            logs[s] = t.Run([&] {
                B60CallPack(s ? (uint32_t)(uintptr_t)&rt3::Save_Pack : 0x442900, block, "rt3re_pk.gm");
                Logf("offset %x progress %08x", *(int32_t*)(block + 0x18), hook::Global<uint32_t>(0xd93988));
            });
            logs[s] += B60Read("rt3re_pk.gm");
        }
        CompareLogs(logs[0], logs[1], "save pack", it);
    }
    DeleteFileA("rt3re_pk.gm");
}

namespace rt3 {
void __fastcall Net_Dispatch(uint8_t* s, int, uint8_t* msg, uint32_t arg);
uint8_t* __fastcall NetPacket_Copy(uint8_t* s, int, const uint8_t* m);
int32_t __fastcall Msg_Queue(void*, int, int32_t key, uint32_t* msg, uint8_t* ctx);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
}

namespace {
uint8_t g_b61Msg[0x8000], g_b61Copy[0x100], g_b61Entry[0x10], g_b61S[0x9900];
int32_t g_b61HasHandler, g_b61Queue;
uint32_t B61Hash(const uint8_t* p, int32_t n) {
    uint32_t h = 2166136261u;
    for (int32_t k = 0; k < n; ++k) h = (h ^ p[k]) * 16777619u;
    return h;
}
void __fastcall FakeB61Handler(uint8_t* m, uint32_t arg) {
    int32_t n = *(int32_t*)(m + 0xc);
    const uint8_t* d = *(uint8_t**)(m + 8);
    Logf("handle %s arg %x flags %x size %d data %08x key %d", m == g_b61Msg ? "msg" : m == g_b61Copy ? "copy" : "?", arg,
         (*(uint8_t**)(m + 4))[0xb], n, d && n > 0 && n < 0x10000 ? B61Hash(d, n) : 0, *(int32_t*)(g_b61S + 0x9888));
}
int32_t __fastcall FakeB61Get(void* m, int, uint32_t key) {
    Logf("get %d %x", m == (void*)0x1234, key);
    return g_b61HasHandler ? (int32_t)(uintptr_t)&FakeB61Handler : 0;
}
uint8_t* __fastcall FakeB61Copy(uint8_t* s, int, const uint8_t* m) {
    Logf("copy size %d", *(int32_t*)(m + 0xc));
    memcpy(g_b61Copy, m, 0x28);
    memcpy(g_b61Copy + 0x28, *(uint8_t**)(m + 4), 12);
    *(uint8_t**)(g_b61Copy + 4) = g_b61Copy + 0x28;
    return g_b61Copy;
}
int32_t __fastcall FakeB61Queue(void* s, int, int32_t key, uint32_t* msg, uint8_t* ctx) {
    Logf("queue %d key %d msg %d ctx %d flags %x", s == g_b61S, key, (uint8_t*)msg == g_b61Copy, ctx == g_b61Entry,
         (*(uint8_t**)((uint8_t*)msg + 4))[0xb]);
    return g_b61Queue;
}
void __fastcall FakeB61Free(uint8_t* s, int, uint8_t* p) { Logf("free %d", p == g_b61Copy); }
void* FakeB61Entry() { return g_b61Entry; }
} // namespace

DIFFTEST(net_Dispatch) {
    UiTrace t;
    t.Add(0x53dae0, (void*)&rt3::Hash_Get, (void*)&FakeB61Get);
    t.Add(0x46a310, (void*)&rt3::NetPacket_Copy, (void*)&FakeB61Copy);
    t.Add(0x46ab40, (void*)&rt3::Msg_Queue, (void*)&FakeB61Queue);
    t.Add(0x469890, (void*)&rt3::NetPacket_Free, (void*)&FakeB61Free);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB61Entry);
    *(void**)(g_b61S + 4) = (void*)0x1234;
    for (int it = 0; it < 400; ++it) {
        g_b61HasHandler = Rnd() % 6 != 0;
        g_b61Queue = Rnd() % 2;
        uint8_t flags = (uint8_t)(Rnd() % 4);
        std::vector<uint8_t> payload((size_t)(4 + Rnd() % 3000));
        int mode = Rnd() % 2;
        for (size_t k = 0; k < payload.size(); ++k) payload[k] = (uint8_t)(mode ? Rnd() : k % 13);
        std::vector<uint8_t> wire = payload;
        if (flags & 1) {
            uint8_t* packed = nullptr;
            int32_t n = 0;
            int32_t packer[3] = { 0x91, 0, 0 };
            rt3::Pack_Compress(packer, 0, payload.data(), (int32_t)payload.size(), &packed, &n);
            wire.assign(packed, packed + n);
            ((void(__cdecl*)(void*))0x5a1145)(packed);
            if (Rnd() % 8 == 0) wire[0] ^= 1;
        }
        uint32_t arg = Rnd();
        // Not 0x7f / 0x90 / 0x92 / 0x94 / 0x96: DesyncWatch (loaded with the game)
        // replaces the original's handler lookup call (0x46ac1f) for those kinds;
        // the rewrite follows the exe.
        uint8_t kind = (uint8_t)(Rnd() % 256);
        if ((kind >= 0x90 && kind <= 0x96 && !(kind & 1)) || kind == 0x7f) ++kind;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(g_b61Msg, 0, 0x40);
            uint8_t* hdr = g_b61Msg + 0x28;
            hdr[9] = kind;
            hdr[0xa] = 0xc;
            hdr[0xb] = flags;
            memcpy(g_b61Msg + 0x34, wire.data(), wire.size());
            *(uint8_t**)(g_b61Msg + 4) = hdr;
            *(uint8_t**)(g_b61Msg + 8) = g_b61Msg + 0x34;
            *(int32_t*)(g_b61Msg + 0xc) = (int32_t)wire.size();
            *(int32_t*)(g_b61S + 0x9888) = -1;
            logs[s] = t.Run([&] {
                if (s) rt3::Net_Dispatch(g_b61S, 0, g_b61Msg, arg);
                else ((void(__fastcall*)(uint8_t*, int, uint8_t*, uint32_t))0x46ac00)(g_b61S, 0, g_b61Msg, arg);
                Logf("after data %d size %d key %d", *(uint8_t**)(g_b61Msg + 8) == g_b61Msg + 0x34, *(int32_t*)(g_b61Msg + 0xc),
                     *(int32_t*)(g_b61S + 0x9888));
            });
        }
        CompareLogs(logs[0], logs[1], "net dispatch", it);
    }
}

namespace rt3 {
int32_t Site_CheckPlacement();
int32_t __fastcall Track_RectIsFree(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t allowSpecial);
int32_t __stdcall Site_ChooseShape(uint8_t* obj);
void __fastcall Terrain_SnapshotFlags();
}

namespace {
int32_t g_b62Free, g_b62Shape;
uint8_t g_b62Map[0x2200], g_b62Bae0[0x100], g_b62Types[0x100], g_b62Track[8];
int32_t g_b62Grid[40 * 40];
int32_t __fastcall FakeB62Free(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t sp) {
    Logf("free %d %d %d %d %d %d", t == g_b62Track, x0, x1, y0, y1, sp);
    return g_b62Free;
}
int32_t __stdcall FakeB62Shape(uint8_t* o) {
    Logf("shape");
    return g_b62Shape;
}
void __fastcall FakeB62Snap() { Logf("snapshot"); }
const char* __fastcall FakeB62String(int32_t id) {
    static char s[16];
    sprintf(s, "msg %x", id);
    return s;
}
int32_t B62Call(uint32_t fn, uint8_t* kind, uint32_t a, uint32_t b) {
    int32_t r;
    __asm {
        push b
        push a
        mov eax, kind
        call fn
        mov r, eax
    }
    return r;
}
} // namespace

DIFFTEST(site_CheckPlacement) {
    SavedGlobals sg({ 0x62b304, 0x62b308, 0x62b30c, 0x62b310, 0x62b7a4, 0x62b7a8, 0x62b7ac, 0x62b7b0, 0x62b7b4, 0x62c120, 0x6cfca8,
                      0x62bae0, 0x62b2fc, 0x62b300, 0x62b318, 0x62b31c, 0x62b320, 0x62b324 });
    std::vector<uint8_t> msg((uint8_t*)0x62b270, (uint8_t*)0x62b270 + 0x40);
    UiTrace t;
    t.Add(0x494240, (void*)&rt3::Track_RectIsFree, (void*)&FakeB62Free);
    t.Add(0x415570, (void*)&rt3::Site_ChooseShape, (void*)&FakeB62Shape);
    t.Add(0x414e10, (void*)&rt3::Terrain_SnapshotFlags, (void*)&FakeB62Snap);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB62String);
    hook::Global<void*>(0x62c120) = g_b62Map;
    hook::Global<void*>(0x6cfca8) = g_b62Track;
    hook::Global<void*>(0x62bae0) = g_b62Bae0;
    hook::Global<void*>(0x62b2fc) = g_b62Types;
    static uint8_t type[0x200];
    for (int it = 0; it < 3000; ++it) {
        *(int32_t*)(g_b62Map + 0x2155) = 36 + Rnd() % 4;
        *(int32_t*)(g_b62Map + 0x2159) = 36 + Rnd() % 4;
        *(int32_t*)(g_b62Bae0 + 0x88) = (int32_t)(Rnd() % 3) - 1;
        *(int32_t*)(g_b62Types + 0x9c) = Rnd() % 3 == 0 ? 0 : 1;
        for (int k = 0; k < 40 * 40; ++k) g_b62Grid[k] = Rnd() % 30 == 0;
        hook::Global<int32_t*>(0x62b300) = Rnd() % 5 == 0 ? nullptr : g_b62Grid;
        hook::Global<uint32_t>(0x62b304) = Rnd();
        g_b62Free = Rnd() % 5 != 0;
        g_b62Shape = Rnd() % 3;
        type[0x123] = Rnd() % 3 == 0;
        int32_t x0 = 3 + Rnd() % 30, y0 = 3 + Rnd() % 30;
        int32_t r[5] = { (int32_t)(Rnd() % 4), x0, Rnd() % 12 == 0 ? -1000000 : x0 + (int32_t)(Rnd() % 5), y0, y0 + (int32_t)(Rnd() % 5) };
        if (r[2] > 37) r[2] = 37;
        if (r[4] > 37) r[4] = 37;
        uint32_t a = Rnd(), b = Rnd();
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            for (int k = 0; k < 5; ++k) hook::Global<int32_t>(0x62b7a4 + 4 * k) = r[k];
            for (int k = 0; k < 4; ++k) hook::Global<int32_t>(0x62b318 + 4 * k) = 7;
            *(char*)0x62b270 = 0;
            logs[s] = t.Run([&] {
                int32_t res = B62Call(s ? (uint32_t)(uintptr_t)&rt3::Site_CheckPlacement : 0x416620, type, a, b);
                Logf("r %d rect %d %d %d %d sel %d %d %d %d ab %x %x %x msg [%s]", res, hook::Global<int32_t>(0x62b7a8),
                     hook::Global<int32_t>(0x62b7ac), hook::Global<int32_t>(0x62b7b0), hook::Global<int32_t>(0x62b7b4),
                     hook::Global<int32_t>(0x62b318), hook::Global<int32_t>(0x62b31c), hook::Global<int32_t>(0x62b320),
                     hook::Global<int32_t>(0x62b324), hook::Global<uint32_t>(0x62b308), hook::Global<uint32_t>(0x62b30c),
                     hook::Global<uint32_t>(0x62b310), (char*)0x62b270);
            });
        }
        CompareLogs(logs[0], logs[1], "site check placement", it);
    }
    memcpy((uint8_t*)0x62b270, msg.data(), msg.size());
}

namespace rt3 {
void __fastcall Net_HostStart(uint8_t* s);
int32_t Net_LockQueue();
void Net_UnlockQueue();
const char* Net_WaitingPlayersText();
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
}

namespace {
uint8_t g_b63Nodes[4][0x60], g_b63Players[6][0x200], g_b63Entry[0x40], g_b63Ui[8], g_b63Coll[8], g_b63World[8];
uint8_t* g_b63Queue[1];
int32_t g_b63Waiting, g_b63Count;
int32_t FakeB63Waiting() { return g_b63Waiting; }
int32_t FakeB63Lock() {
    Logf("lock");
    return (int32_t)(uintptr_t)g_b63Queue;
}
void FakeB63Unlock() { Logf("unlock"); }
const char* FakeB63Text() { return "waiting for Bob"; }
int32_t __fastcall FakeB63Tip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data) {
    Logf("tooltip %d [%s] %d %d %d", ui == g_b63Ui, text, prio, sticky, data);
    return 0;
}
int32_t __fastcall FakeB63Count(void*) { return g_b63Count; }
uint32_t __fastcall FakeB63IdAt(void*, int, uint32_t pos, int) { return pos + 10; }
void* __fastcall FakeB63Get(void*, int, uint32_t id) { return g_b63Players[(id - 10) % 6]; }
void* FakeB63Entry() { return g_b63Entry; }
int32_t __fastcall FakeB63Send(void* net, int, const uint8_t* data, uint32_t size, uint32_t type, uint32_t ch, const char* to,
                               uint32_t flags, uint32_t timeout, int32_t serial, int32_t last) {
    uint32_t h = 2166136261u;
    for (uint32_t k = 0; data && k < size; ++k) h = (h ^ data[k]) * 16777619u;
    Logf("send %u %x %x %p %x %u %d %d lock %d hash %08x", size, type, ch, to, flags, timeout, serial, last,
         *(volatile LONG*)0x6cd960, h);
    return 0;
}
} // namespace

DIFFTEST(net_HostStart) {
    SavedGlobals sg({ 0x6cec78, 0x6d40dc, 0x6d401c, 0x6ceb9c, 0x6cd960, 0x6cd930, 0x6cd934, 0x6d40d0 });
    UiTrace t;
    t.Add(0x521670, (void*)&rt3::Global6d4100, (void*)&FakeB63Waiting);
    t.Add(0x521680, (void*)&rt3::Net_LockQueue, (void*)&FakeB63Lock);
    t.Add(0x5216a0, (void*)&rt3::Net_UnlockQueue, (void*)&FakeB63Unlock);
    t.Add(0x46a200, (void*)&rt3::Net_WaitingPlayersText, (void*)&FakeB63Text);
    t.Add(0x538c70, (void*)&rt3::Ui_ShowTooltip, (void*)&FakeB63Tip);
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB63Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB63IdAt);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB63Get);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB63Entry);
    t.Add(0x521dc0, (void*)&rt3::Net_SendTo, (void*)&FakeB63Send);
    hook::Global<void*>(0x6ceb9c) = g_b63Coll;
    hook::Global<LONG>(0x6cd960) = 0;
    strcpy((char*)g_b63Entry + 8, "Host");
    static uint8_t s[0x9000];
    for (int it = 0; it < 800; ++it) {
        memset(s, 0, sizeof s);
        int32_t started0 = Rnd() % 8 == 0;
        hook::Global<void*>(0x6cec78) = Rnd() % 8 == 0 ? nullptr : g_b63World;
        hook::Global<int32_t>(0x6d40dc) = Rnd() % 5 != 0;
        hook::Global<void*>(0x6d401c) = Rnd() % 4 == 0 ? nullptr : g_b63Ui;
        int nodes = Rnd() % 5, flagged = 0;
        g_b63Queue[0] = nodes ? g_b63Nodes[0] : nullptr;
        for (int k = 0; k < 4; ++k) {
            *(uint8_t**)g_b63Nodes[k] = k + 1 < nodes ? g_b63Nodes[k + 1] : nullptr;
            g_b63Nodes[k][0x5c] = (uint8_t)(Rnd() % 16);
            *(int32_t*)(g_b63Nodes[k] + 0x58) = 5;
            if (k < nodes && (g_b63Nodes[k][0x5c] & 8)) ++flagged;
        }
        g_b63Waiting = Rnd() % 3 == 0 ? flagged + 1 : flagged;
        g_b63Count = Rnd() % 7;
        for (int k = 0; k < 6; ++k) {
            memset(g_b63Players[k], 0, 0x200);
            *(int32_t*)(g_b63Players[k] + 4) = Rnd() % 4 != 0;
            *(int32_t*)(g_b63Players[k] + 0x1e1) = Rnd() % 4 != 0;
            sprintf((char*)g_b63Players[k] + 8, "Player%d%s", k, Rnd() % 2 ? "xxxxxxxxxxxxxxxxxxxxxxxx" : "");
        }
        std::string logs[2];
        for (int sd = 0; sd < 2; ++sd) {
            *(int32_t*)(s + 0x8f30) = started0;
            hook::Global<int32_t>(0x6cd930) = 0;
            hook::Global<int32_t>(0x6cd934) = 0;
            uint8_t saved[6][0x40];
            for (int k = 0; k < 6; ++k) memcpy(saved[k], g_b63Players[k] + 8, 0x40);
            logs[sd] = t.Run([&] {
                if (sd) rt3::Net_HostStart(s);
                else ((void(__fastcall*)(uint8_t*))0x46b0c0)(s);
                Logf("started %d totals %d %d nodes %d %d %d %d p0 [%s]", *(int32_t*)(s + 0x8f30), hook::Global<int32_t>(0x6cd930),
                     hook::Global<int32_t>(0x6cd934), *(int32_t*)(g_b63Nodes[0] + 0x58), *(int32_t*)(g_b63Nodes[1] + 0x58),
                     *(int32_t*)(g_b63Nodes[2] + 0x58), *(int32_t*)(g_b63Nodes[3] + 0x58), (char*)g_b63Players[0] + 8);
            });
            for (int k = 0; k < 6; ++k) memcpy(g_b63Players[k] + 8, saved[k], 0x40);
            for (int k = 0; k < 4; ++k) *(int32_t*)(g_b63Nodes[k] + 0x58) = 5;
        }
        CompareLogs(logs[0], logs[1], "net host start", it);
    }
}

namespace rt3 {
uint8_t* __fastcall RulesDialog_Construct(uint8_t* w, int, uint32_t a, int32_t* rule);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
void __fastcall RulesDialog_ShowEffect(uint8_t* w);
void __fastcall RulesDialog_ShowEffectType(uint8_t* w);
}

namespace {
uint8_t g_b64Child[0x100];
void __fastcall FakeB64Load(uint8_t* w, int, float x, float y, const char* name) {
    Logf("load %08x %08x [%s] vt %08x", B(x), B(y), name, *(uint32_t*)w);
}
int32_t __fastcall FakeB64Send(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Logf("send %x %x %x %x %x", type, id, a, b, c);
    return 0;
}
uint8_t* __fastcall FakeB64Find(uint8_t* w, int, int32_t id) {
    Logf("find %x", id);
    return g_b64Child;
}
void __fastcall FakeB64Fill(uint8_t* w, int, int32_t sel) { Logf("fill %d", sel); }
void __fastcall FakeB64Effect(uint8_t* w) { Logf("effect"); }
void __fastcall FakeB64Type(uint8_t* w) { Logf("type %d", *(int32_t*)(w + 0x78)); }
} // namespace

DIFFTEST(ui_RulesDialogConstruct) {
    SavedGlobals sg({ 0x6d07b8, 0x6d07bc, 0x6d3c84 });
    UiTrace t;
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&FakeB64Load);
    t.Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&FakeB64Send);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB64Find);
    t.Add(0x4d4850, (void*)&rt3::RulesDialog_FillEffectTypes, (void*)&FakeB64Fill);
    t.Add(0x4d4560, (void*)&rt3::RulesDialog_ShowEffect, (void*)&FakeB64Effect);
    t.Add(0x4d4d10, (void*)&rt3::RulesDialog_ShowEffectType, (void*)&FakeB64Type);
    static uint8_t w[0x100], rule[0x40];
    for (int it = 0; it < 200; ++it) {
        hook::Global<int32_t>(0x6d3c84) = Rnd() % 6;
        hook::Global<int32_t>(0x6d07bc) = Rnd() % 2;
        memset(rule, 0, sizeof rule);
        int k = Rnd() % 4;
        *(int32_t*)rule = k == 0 ? -2 : k == 1 ? -1 : (int32_t)(Rnd() % 0x385);
        *(int32_t*)(rule + 5) = Rnd() % 2;
        *(int32_t*)(rule + 9) = Rnd() % 700;
        uint32_t a = Rnd();
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(w, 0, sizeof w);
            *(int32_t*)(g_b64Child + 0xcc) = 0;
            logs[s] = t.Run([&] {
                uint8_t* r = s ? rt3::RulesDialog_Construct(w, 0, a, (int32_t*)rule)
                               : ((uint8_t*(__fastcall*)(uint8_t*, int, uint32_t, int32_t*))0x4d57c0)(w, 0, a, (int32_t*)rule);
                Logf("r %d vt %08x g %d 78 %d 7c %d 80 %x 84 %d child %d", r == w, *(uint32_t*)w, hook::Global<uint8_t*>(0x6d07b8) == w,
                     *(int32_t*)(w + 0x78), *(int32_t*)(w + 0x7c), *(uint32_t*)(w + 0x80), *(uint8_t**)(w + 0x84) == rule,
                     *(int32_t*)(g_b64Child + 0xcc));
            });
        }
        CompareLogs(logs[0], logs[1], "rules dialog construct", it);
    }
}

namespace rt3 {
void Emitter_Apply();
void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall Obj_SetPair1ed(uint8_t* o, int, uint32_t v);
}

namespace {
uint8_t g_b65Table[0xc + 4 * 0xa0];
void __fastcall FakeB65Budget(uint8_t* t, int, uint32_t v) { Logf("budget %d", (int32_t)v); }
void __fastcall FakeB65View(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
    Logf("view %d %d %08x %08x %08x %08x", a, (int32_t)b, c, d, e, f);
}
void __fastcall FakeB65Pair(uint8_t* o, int, uint32_t v) { Logf("pair %d", (int32_t)v); }
void B65Call(uint32_t fn, uint8_t* src, uint8_t* dst, uint32_t flags, float scale) {
    __asm {
        push scale
        push flags
        push dst
        mov eax, src
        call fn
    }
}
} // namespace

DIFFTEST(render_EmitterApply) {
    SavedGlobals sg({ 0x6cea5c });
    UiTrace t;
    t.Add(0x554970, (void*)&rt3::Timer_SetBudget, (void*)&FakeB65Budget);
    t.Add(0x5548e0, (void*)&rt3::View_SetParams, (void*)&FakeB65View);
    t.Add(0x554980, (void*)&rt3::Obj_SetPair1ed, (void*)&FakeB65Pair);
    static const char* names[] = { "smoke", "Spark", "steam", "FIRE" };
    *(int32_t*)(g_b65Table + 8) = 4;
    for (int k = 0; k < 4; ++k) {
        uint8_t* e = g_b65Table + 0xc + k * 0xa0;
        for (int b = 4; b < 0xa0; ++b) e[b] = (uint8_t)(k * 31 + b);
        *(uint32_t*)e = rt3::Crc32String(names[k], 1);
    }
    static uint8_t src[0x100], dst[0x200];
    for (int it = 0; it < 3000; ++it) {
        for (int o = 0x84; o < 0xf0; o += 4) *(float*)(src + o) = RndF(-500.0f, 500.0f);
        *(uint32_t*)(src + 0x84) = Rnd();
        *(uint32_t*)(src + 0xc4) = Rnd();
        *(uint32_t*)(src + 0xec) = Rnd();
        strcpy((char*)src + 0x44, Rnd() % 5 == 0 ? "nothing" : names[Rnd() % 4]);
        hook::Global<void*>(0x6cea5c) = Rnd() % 6 == 0 ? nullptr : g_b65Table;
        uint32_t flags = Rnd() & 0x200c1000;
        if (Rnd() % 2) flags |= Rnd();
        float scale = RndF(0.0f, 3.0f);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(dst, 0xab, sizeof dst);
            logs[s] = t.Run([&] {
                B65Call(s ? (uint32_t)(uintptr_t)&rt3::Emitter_Apply : 0x4749d0, src, dst, flags, scale);
                std::string d;
                char b[12];
                for (int o = 0; o < (int)sizeof dst; o += 4) {
                    sprintf(b, "%08x ", *(uint32_t*)(dst + o));
                    d += b;
                }
                g_log += d + "\n";
            });
        }
        CompareLogs(logs[0], logs[1], "emitter apply", it);
    }
}

namespace rt3 {
void __fastcall Texture_SaveJpeg(uint8_t* t, int, void* f, uint32_t withAlpha, int32_t quality);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
}

namespace {
uint32_t* __fastcall FakeB66Pixels(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i) {
    Logf("pixels %p %p %d", w, h, i);
    return nullptr;
}
// The saved file's size and hash, with a raw alpha plane (mark 0xbabb) zeroed:
// the original writes it from past the alpha buffer, i.e. heap contents.
std::string B66ReadMasked(const char* p) {
    FILE* f = fopen(p, "rb");
    if (!f) return "<none>";
    std::string s;
    char b[4096];
    size_t n;
    while ((n = fread(b, 1, sizeof b, f)) > 0) s.append(b, n);
    fclose(f);
    if (s.size() >= 8 && *(const uint16_t*)(s.data() + s.size() - 4) == 0xbabb) {
        uint32_t raw = *(const uint32_t*)(s.data() + s.size() - 8);
        if (raw <= s.size() - 8) memset(&s[s.size() - 8 - raw], 0, raw);
    }
    uint32_t x = 2166136261u;
    for (unsigned char c : s) x = (x ^ c) * 16777619u;
    char h[32];
    sprintf(h, "%u bytes %08x", (unsigned)s.size(), x);
    return h;
}
} // namespace

DIFFTEST(render_TextureSaveJpeg) {
    SavedGlobals sg({ 0x6d4024 });
    UiTrace t;
    t.Add(0x541970, (void*)&rt3::Texture_Pixels32, (void*)&FakeB66Pixels);
    static uint8_t flags[0x114250];
    hook::Global<uint8_t*>(0x6d4024) = flags;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_jpeg_test.bin");
    static uint8_t tex[0x100], img[0x40];
    static uint32_t px[96 * 64];
    *(uint8_t**)(tex + 0xa0) = img;
    for (int it = 0; it < 24; ++it) {
        int32_t w = 1 + Rnd() % 96, h = 1 + Rnd() % 64;
        *(int32_t*)(img + 8) = w;
        *(int32_t*)(img + 0xc) = h;
        *(uint32_t**)(img + 0x26) = px;
        // The alpha always packs: a raw plane is written from past the alpha buffer
        // (see Texture_SaveJpeg), which can run off the heap here.
        int mode = Rnd() % 3;
        for (int k = 0; k < w * h; ++k)
            px[k] = mode == 0 ? (Rnd() & 0xffffff) | 0x80000000u
                    : mode == 1 ? 0xff000000u | (uint32_t)(k * 0x10203)
                                : (uint32_t)((k % w) * 0x1010101);
        flags[0x11424f] = (uint8_t)(Rnd() % 2);
        uint32_t alpha = Rnd() % 3 == 0 ? 0x100 : Rnd() % 2;
        int32_t quality = 10 + Rnd() % 90;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            FILE* f = ((FILE*(__cdecl*)(const char*, const char*))0x5a1887)(path, "wb");
            logs[s] = t.Run([&] {
                if (s) rt3::Texture_SaveJpeg(tex, 0, f, alpha, quality);
                else ((void(__fastcall*)(uint8_t*, int, void*, uint32_t, int32_t))0x5428c0)(tex, 0, f, alpha, quality);
            });
            ((int(__cdecl*)(void*))0x5a1740)(f);
            logs[s] += B66ReadMasked(path);
        }
        CompareLogs(logs[0], logs[1], "texture save jpeg", it);
    }
    DeleteFileA(path);
}

namespace rt3 {
int32_t __fastcall Texture_LoadJpeg(uint8_t* t, int, int32_t id, uint8_t* img);
FILE* __fastcall NameTable_OpenFile(uint8_t* t, int, int32_t id, uint8_t text);
void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img);
}

namespace {
char g_b68Path[MAX_PATH];
void* g_b68File;
FILE* __fastcall FakeB68Open(uint8_t* t, int, int32_t id, uint8_t text) {
    Logf("open %d %d", id, text);
    g_b68File = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(g_b68Path, "rb");
    return (FILE*)g_b68File;
}
void __fastcall FakeB68To4444(uint8_t* tex, int, uint8_t* img) {
    uint32_t h = 2166136261u;
    const uint32_t* p = *(uint32_t**)(img + 0x26);
    int32_t n = *(int32_t*)(img + 8) * *(int32_t*)(img + 0xc);
    for (int32_t k = 0; p && k < n; ++k) h = (h ^ p[k]) * 16777619u;
    Logf("to4444 id %d %dx%d %x %d %d %d %d %d bytes %u same %d big %d pix %08x", *(int32_t*)img, *(int32_t*)(img + 8),
         *(int32_t*)(img + 0xc), *(int32_t*)(img + 0x10), img[0x21], *(int32_t*)(img + 0x14), *(int32_t*)(img + 0x18),
         *(int32_t*)(img + 0x1c), 0, *(uint32_t*)(img + 4), *(uint32_t**)(img + 0x22) == p, img[0x20], h);
}
} // namespace

DIFFTEST(render_TextureLoadJpeg) {
    SavedGlobals sg({ 0x6d4024, 0x6d4020 });
    static uint8_t dev[0x114250];
    hook::Global<uint8_t*>(0x6d4024) = dev;
    GetTempPathA(MAX_PATH, g_b68Path);
    strcat(g_b68Path, "rt3re_jpegload_test.bin");
    static uint8_t tex[0x100], src[0x40], img[0x40];
    static uint32_t px[100 * 80];
    for (int it = 0; it < 24; ++it) {
        int32_t w = 1 + Rnd() % 100, h = 1 + Rnd() % 80;
        int mode = Rnd() % 3;
        for (int k = 0; k < w * h; ++k)
            px[k] = mode == 0 ? Rnd() : mode == 1 ? 0xff000000u | (uint32_t)(k * 0x10203) : (uint32_t)((k % w) * 0x1010101);
        {
            // The image file: a JPEG with (or without) the alpha plane, as the game saves it.
            *(uint8_t**)(tex + 0xa0) = src;
            *(int32_t*)(src + 8) = w;
            *(int32_t*)(src + 0xc) = h;
            *(uint32_t**)(src + 0x26) = px;
            dev[0x11424f] = 0;
            void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(g_b68Path, "wb");
            int kind = Rnd() % 4;
            if (kind == 3) {
                // A raw alpha plane (written by hand: the saver's raw path reads past its buffer).
                for (int k = 0; k < w * h; ++k) px[k] |= 0x80000000u;
                rt3::Texture_SaveJpeg(tex, 0, f, 0, 75);
                typedef size_t(__cdecl * Fw)(const void*, size_t, size_t, void*);
                std::vector<uint8_t> raw((size_t)(w * h));
                for (size_t k = 0; k < raw.size(); ++k) raw[k] = (uint8_t)Rnd();
                int32_t n = w * h;
                uint32_t mark = 0xbabb, end = 0xbabe;
                ((Fw)0x5a276f)(raw.data(), 1, raw.size(), f);
                ((Fw)0x5a276f)(&n, 1, 4, f);
                ((Fw)0x5a276f)(&mark, 1, 2, f);
                ((Fw)0x5a276f)(&end, 1, 2, f);
            } else {
                if (mode == 0)
                    for (int k = 0; k < w * h; ++k) px[k] = (px[k] & 0xffffff) | 0x80000000u;
                rt3::Texture_SaveJpeg(tex, 0, f, kind != 0, 75);
            }
            ((int(__cdecl*)(void*))0x5a1740)(f);
        }
        dev[0x11422f] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(dev + 0x11423b) = Rnd() % 6 == 0 ? 50 : 4096;
        *(int32_t*)(dev + 0x11423f) = 4096;
        tex[5] = (uint8_t)(Rnd() % 2);
        int pre = Rnd() % 5;
        int32_t id = Rnd() % 1000;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            UiTrace t;
            t.Add(0x53be10, (void*)&rt3::NameTable_OpenFile, (void*)&FakeB68Open);
            t.Add(0x541210, (void*)&rt3::Image_To4444, (void*)&FakeB68To4444);
            memset(img, 0, sizeof img);
            if (pre == 1) *(void**)(img + 0x26) = px;
            if (pre == 2) *(void**)(img + 0x2a) = px;
            g_b68File = nullptr;
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Texture_LoadJpeg(tex, 0, id, img)
                              : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, uint8_t*))0x541c30)(tex, 0, id, img);
                Logf("r %d", r);
            });
            if (g_b68File) ((int(__cdecl*)(void*))0x5a1740)(g_b68File);
        }
        CompareLogs(logs[0], logs[1], "texture load jpeg", it);
    }
    DeleteFileA(g_b68Path);
}

namespace rt3 {
void __fastcall EditorDialog_SetTab(uint8_t* w, int, int32_t tab, int32_t city);
int32_t __fastcall RulesDialog_StoreRule(uint8_t*);
void __fastcall Nop4cf900(void*);
void __fastcall EditorDialog_ShowSettings(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a3b(uint8_t*);
int32_t __fastcall Dialog_SetupControl5141(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a40(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a3e(uint8_t*);
void __fastcall CityEditor_Fill(uint8_t*, int, int32_t, int32_t);
void __fastcall TerritoryEditor_Show(uint8_t*, int, int32_t);
void __fastcall RulesDialog_Init(uint8_t*);
void __fastcall NewGameDialog_SetupRows(uint8_t*);
void __fastcall EditorDialog_FillCargoList(uint8_t*);
void __fastcall EditorDialog_ShowEconomy(uint8_t*);
void __fastcall EditorDialog_FillIndustries(uint8_t*, int, int32_t);
void __fastcall EditorDialog_FillHouses(uint8_t*, int, int32_t);
void __fastcall EditorDialog_ShowAchievements(uint8_t*, int, int32_t);
void __fastcall EditorDialog_ShowRegionTypes(uint8_t*);
void __fastcall Window_ShowWorldPicks(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a3f(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a42(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a43(uint8_t*);
int32_t __fastcall Dialog_SetCallback4a41(uint8_t*);
}

namespace {
int32_t g_b67Store, g_b67Count;
int32_t __fastcall FakeB67H0(uint8_t* w) { Logf("store"); return g_b67Store; }
void __fastcall FakeB67H1(void* w) { Logf("h1"); }
void __fastcall FakeB67H2(uint8_t* w) { Logf("h2"); }
int32_t __fastcall FakeB67H3(uint8_t* w) { Logf("h3"); return 0; }
int32_t __fastcall FakeB67H4(uint8_t* w) { Logf("h4"); return 0; }
int32_t __fastcall FakeB67H5(uint8_t* w) { Logf("h5"); return 0; }
int32_t __fastcall FakeB67H6(uint8_t* w) { Logf("h6"); return 0; }
void __fastcall FakeB67H7(uint8_t* w, int, int32_t a, int32_t b) { Logf("h7 %d %d", a, b); }
void __fastcall FakeB67H8(uint8_t* w, int, int32_t a) { Logf("h8 %d", a); }
void __fastcall FakeB67H9(uint8_t* w) { Logf("h9"); }
void __fastcall FakeB67H10(uint8_t* w) { Logf("h10"); }
void __fastcall FakeB67H11(uint8_t* w) { Logf("h11"); }
void __fastcall FakeB67H12(uint8_t* w) { Logf("h12"); }
void __fastcall FakeB67H13(uint8_t* w, int, int32_t a) { Logf("h13 %d", a); }
void __fastcall FakeB67H14(uint8_t* w, int, int32_t a) { Logf("h14 %d", a); }
void __fastcall FakeB67H15(uint8_t* w, int, int32_t a) { Logf("h15 %d", a); }
void __fastcall FakeB67H16(uint8_t* w) { Logf("h16"); }
void __fastcall FakeB67H17(uint8_t* w) { Logf("h17"); }
int32_t __fastcall FakeB67H18(uint8_t* w) { Logf("h18"); return 0; }
int32_t __fastcall FakeB67H19(uint8_t* w) { Logf("h19"); return 0; }
int32_t __fastcall FakeB67H20(uint8_t* w) { Logf("h20"); return 0; }
int32_t __fastcall FakeB67H21(uint8_t* w) { Logf("h21"); return 0; }
int32_t __fastcall FakeB67Send(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Logf("send %x %x %x", type, id, a);
    return 0;
}
int32_t __fastcall FakeB67Valid(void*, int, uint32_t id) { return id == 5; }
int32_t __fastcall FakeB67Count(void*) { return g_b67Count; }
uint32_t __fastcall FakeB67IdAt(void*, int, uint32_t pos, int) { return 9; }
} // namespace

DIFFTEST(ui_EditorSetTab) {
    SavedGlobals sg({ 0x6cff74, 0x6cff80 });
    UiTrace t;
    t.Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&FakeB67Send);
    t.Add(0x517d40, (void*)&rt3::Collection_IsValid, (void*)&FakeB67Valid);
    t.Add(0x517cf0, (void*)&rt3::Collection_Count, (void*)&FakeB67Count);
    t.Add(0x518380, (void*)&rt3::Collection_IdAt, (void*)&FakeB67IdAt);
    t.Add(0x4d9ec0, (void*)&rt3::RulesDialog_StoreRule, (void*)&FakeB67H0);
    t.Add(0x4cf900, (void*)&rt3::Nop4cf900, (void*)&FakeB67H1);
    t.Add(0x4ca790, (void*)&rt3::EditorDialog_ShowSettings, (void*)&FakeB67H2);
    t.Add(0x4d3000, (void*)&rt3::Dialog_SetCallback4a3b, (void*)&FakeB67H3);
    t.Add(0x4d3020, (void*)&rt3::Dialog_SetupControl5141, (void*)&FakeB67H4);
    t.Add(0x4d30a0, (void*)&rt3::Dialog_SetCallback4a40, (void*)&FakeB67H5);
    t.Add(0x4d2fe0, (void*)&rt3::Dialog_SetCallback4a3e, (void*)&FakeB67H6);
    t.Add(0x4cc980, (void*)&rt3::CityEditor_Fill, (void*)&FakeB67H7);
    t.Add(0x4cd070, (void*)&rt3::TerritoryEditor_Show, (void*)&FakeB67H8);
    t.Add(0x4db6f0, (void*)&rt3::RulesDialog_Init, (void*)&FakeB67H9);
    t.Add(0x4cc2d0, (void*)&rt3::NewGameDialog_SetupRows, (void*)&FakeB67H10);
    t.Add(0x4ca540, (void*)&rt3::EditorDialog_FillCargoList, (void*)&FakeB67H11);
    t.Add(0x4cadf0, (void*)&rt3::EditorDialog_ShowEconomy, (void*)&FakeB67H12);
    t.Add(0x4cd680, (void*)&rt3::EditorDialog_FillIndustries, (void*)&FakeB67H13);
    t.Add(0x4cdbd0, (void*)&rt3::EditorDialog_FillHouses, (void*)&FakeB67H14);
    t.Add(0x4cb2b0, (void*)&rt3::EditorDialog_ShowAchievements, (void*)&FakeB67H15);
    t.Add(0x4cf910, (void*)&rt3::EditorDialog_ShowRegionTypes, (void*)&FakeB67H16);
    t.Add(0x4ca910, (void*)&rt3::Window_ShowWorldPicks, (void*)&FakeB67H17);
    t.Add(0x4d3060, (void*)&rt3::Dialog_SetCallback4a3f, (void*)&FakeB67H18);
    t.Add(0x4d4130, (void*)&rt3::Dialog_SetCallback4a42, (void*)&FakeB67H19);
    t.Add(0x4d4110, (void*)&rt3::Dialog_SetCallback4a43, (void*)&FakeB67H20);
    t.Add(0x4d3080, (void*)&rt3::Dialog_SetCallback4a41, (void*)&FakeB67H21);
    static uint8_t w[0x100];
    for (int it = 0; it < 600; ++it) {
        int32_t cur = Rnd() % 3 == 0 ? (Rnd() % 2 ? 9 : 11) : (int32_t)(Rnd() % 22);
        int32_t tab = Rnd() % 4 == 0 ? 1 : (int32_t)(Rnd() % 23) - 1;
        int32_t city = Rnd() % 2 ? -1 : (int32_t)(Rnd() % 8);
        int32_t locked = Rnd() % 10 == 0;
        g_b67Store = Rnd() % 3 != 0;
        g_b67Count = Rnd() % 2;
        uint32_t last = Rnd() % 2 ? 5 : 6;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(w, 0, sizeof w);
            *(int32_t*)(w + 0x78) = cur;
            *(int32_t*)(w + 0x7c) = locked;
            hook::Global<uint32_t>(0x6cff74) = last;
            hook::Global<int32_t>(0x6cff80) = -7;
            logs[s] = t.Run([&] {
                if (s) rt3::EditorDialog_SetTab(w, 0, tab, city);
                else ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ce070)(w, 0, tab, city);
                Logf("tab %d %d", *(int32_t*)(w + 0x78), hook::Global<int32_t>(0x6cff80));
            });
        }
        CompareLogs(logs[0], logs[1], "editor set tab", it);
    }
}

namespace rt3 {
void __fastcall Scene_VisitTerrain(uint8_t* s, int, uint8_t* visitor);
void __fastcall Fade_StartFast();
void __fastcall Scene_VisibleBounds(uint8_t* s, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1, uint8_t* terrain,
                                    int32_t shift);
void __fastcall Render_GetFog(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, uint32_t* e,
                              uint32_t* f, uint32_t* g);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void* __fastcall Terrain_HeightsSlot(uint8_t* r);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Engine_ResetTextureQueue(void* e);
}

namespace {
int32_t g_b69Fog[7], g_b69Rows;
float g_b69Cam[3], g_b69H[40 * 41];
float* g_b69Heights = g_b69H;
uint8_t g_b69Dev[0x40], g_b69CamObj[0x10];
void __fastcall FakeB69Fade() { Logf("fade"); }
void __fastcall FakeB69Bounds(uint8_t* s, int, int32_t* a, int32_t* b, int32_t* c, int32_t* d, uint8_t* t, int32_t sh) {
    Logf("bounds %d", sh);
    *a = 11;
    *b = 12;
    *c = 13;
    *d = 14;
}
void __fastcall FakeB69Fog(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, uint32_t* e, uint32_t* f,
                           uint32_t* g) {
    uint32_t* o[7] = { a, b, c, d, e, f, g };
    for (int k = 0; k < 7; ++k) *o[k] = (uint32_t)g_b69Fog[k];
}
void __fastcall FakeB69Cam(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    Logf("cam %d", o == g_b69CamObj);
    memcpy(x, &g_b69Cam[0], 4);
    memcpy(y, &g_b69Cam[1], 4);
    memcpy(z, &g_b69Cam[2], 4);
}
void* __fastcall FakeB69Slot(uint8_t*) { return &g_b69Heights; }
int32_t __fastcall FakeB69Rows(uint8_t*) { return g_b69Rows; }
int32_t __fastcall FakeB69Project(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    Logf("project %08x %08x %08x", B(x), B(y), B(z));
    *ox = x * 0.5f;
    *oy = y + 1.0f;
    *oz = z - x;
    return (B(x) ^ B(z)) % 3 != 0;
}
void __fastcall FakeB69Reset(void* e) { Logf("reset %d", e == g_b69Dev); }
void __fastcall FakeB69Visit(uint8_t* v, int, uint32_t a, uint32_t b, uint32_t c, int32_t x, int32_t y) {
    Logf("visit %08x %08x %08x %d %d", a, b, c, x, y);
}
} // namespace

DIFFTEST(render_SceneVisitTerrain) {
    SavedGlobals sg({ 0x6d4024 });
    UiTrace t;
    t.Add(0x54f6b0, (void*)&rt3::Fade_StartFast, (void*)&FakeB69Fade);
    t.Add(0x5271b0, (void*)&rt3::Scene_VisibleBounds, (void*)&FakeB69Bounds);
    t.Add(0x532380, (void*)&rt3::Render_GetFog, (void*)&FakeB69Fog);
    t.Add(0x52cd60, (void*)&rt3::Obj_GetXYZ1c, (void*)&FakeB69Cam);
    t.Add(0x533b10, (void*)&rt3::Terrain_HeightsSlot, (void*)&FakeB69Slot);
    t.Add(0x533970, (void*)&rt3::Terrain_Rows, (void*)&FakeB69Rows);
    t.Add(0x527450, (void*)&rt3::Scene_ProjectNormalised, (void*)&FakeB69Project);
    t.Add(0x51f1d0, (void*)&rt3::Engine_ResetTextureQueue, (void*)&FakeB69Reset);
    hook::Global<uint8_t*>(0x6d4024) = g_b69Dev;
    *(uint8_t**)(g_b69Dev + 0x28) = g_b69CamObj;
    static void* vt[8];
    vt[3] = (void*)&FakeB69Visit;
    static uint8_t scene[0x40], visitor[0x40];
    *(void***)visitor = vt;
    for (int k = 0; k < 40 * 41; ++k) g_b69H[k] = RndF(-10.0f, 200.0f);
    for (int it = 0; it < 300; ++it) {
        g_b69Rows = 39;
        g_b69Fog[0] = Rnd() % 10;
        g_b69Fog[1] = Rnd() % 10;
        g_b69Fog[2] = g_b69Fog[0] + Rnd() % 30;
        g_b69Fog[3] = g_b69Fog[1] + Rnd() % 30;
        g_b69Fog[4] = 1 + Rnd() % 3;
        g_b69Fog[5] = 1 + Rnd() % 3;
        g_b69Fog[6] = Rnd() % 3 == 0 ? 1000 + Rnd() % 100 : Rnd() % 25;
        for (int k = 0; k < 3; ++k) g_b69Cam[k] = RndF(-100.0f, 3500.0f);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            logs[s] = t.Run([&] {
                if (s) rt3::Scene_VisitTerrain(scene, 0, visitor);
                else ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x528190)(scene, 0, visitor);
            });
        }
        CompareLogs(logs[0], logs[1], "scene visit terrain", it);
    }
}

namespace rt3 {
int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality);
int32_t __fastcall Texture_LoadImage(uint8_t* t, int, int32_t id, uint8_t* img);
int32_t __fastcall Texture_AddAndLoad(uint8_t* t, int, int32_t id, int32_t format);
int32_t __fastcall Texture_LoadDds(uint8_t* o, int, int32_t id, uint8_t* img);
int32_t __fastcall Texture_LoadTga(uint8_t* tex, int, int32_t id, uint8_t* img);
int32_t __fastcall Texture_AddImage(uint8_t* t, int, int32_t w, int32_t h, int32_t format);
void __fastcall Texture_RemoveImage(uint8_t* t, int, int32_t i);
void __fastcall Texture_CountMemory(uint8_t* t);
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
}

namespace {
int32_t g_b70Ret, g_b70Add;
void __fastcall FakeB70Save(uint8_t* t, int, void* f, uint32_t a, int32_t q) {
    Logf("save %x %d", a, q);
    ((size_t(__cdecl*)(const void*, size_t, size_t, void*))0x5a276f)("jpegdata", 1, 8, f);
}
int32_t __fastcall FakeB70Dds(uint8_t* t, int, int32_t id, uint8_t* img) {
    Logf("dds %d", id);
    return g_b70Ret;
}
int32_t __fastcall FakeB70Tga(uint8_t* t, int, int32_t id, uint8_t* img) {
    Logf("tga %d", id);
    return g_b70Ret;
}
int32_t __fastcall FakeB70Jpeg(uint8_t* t, int, int32_t id, uint8_t* img) {
    Logf("jpeg %d", id);
    return g_b70Ret;
}
int32_t __fastcall FakeB70Add(uint8_t* t, int, int32_t w, int32_t h, int32_t f) {
    Logf("add %d %d %d", w, h, f);
    return g_b70Add;
}
void __fastcall FakeB70Remove(uint8_t* t, int, int32_t i) { Logf("remove %d", i); }
void __fastcall FakeB70Count(uint8_t* t) { Logf("count"); }
void __fastcall FakeB70Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    Logf("send [%s] %x %d %p %x %d", to, type, size, data, flags, serial);
}
} // namespace

DIFFTEST(render_TextureLoadHelpers) {
    UiTrace t;
    t.Add(0x5428c0, (void*)&rt3::Texture_SaveJpeg, (void*)&FakeB70Save);
    t.Add(0x542d50, (void*)&rt3::Texture_LoadDds, (void*)&FakeB70Dds);
    t.Add(0x542030, (void*)&rt3::Texture_LoadTga, (void*)&FakeB70Tga);
    t.Add(0x541c30, (void*)&rt3::Texture_LoadJpeg, (void*)&FakeB70Jpeg);
    t.Add(0x541b10, (void*)&rt3::Texture_AddImage, (void*)&FakeB70Add);
    t.Add(0x541ba0, (void*)&rt3::Texture_RemoveImage, (void*)&FakeB70Remove);
    t.Add(0x540fc0, (void*)&rt3::Texture_CountMemory, (void*)&FakeB70Count);
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB70Send);
    static uint8_t tex[0x100], imgs[4][0x40], sess[0x9000];
    for (int k = 0; k < 4; ++k) *(uint8_t**)(tex + 0xa0 + k * 4) = imgs[k];
    strcpy((char*)sess + 0x8f48, "Host");
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_append_test.bin");
    for (int it = 0; it < 200; ++it) {
        g_b70Ret = Rnd() % 4;
        g_b70Add = Rnd() % 5 == 0 ? -1 : (int32_t)(Rnd() % 4);
        for (int k = 0; k < 4; ++k) *(int32_t*)(imgs[k] + 0x18) = Rnd() % 6;
        int32_t id = Rnd() % 100, fmt = Rnd() % 8;
        int existing = Rnd() % 3;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            DeleteFileA(path);
            if (existing) {
                FILE* f = fopen(path, "wb");
                fwrite("0123456789", 1, (size_t)(existing * 5), f);
                fclose(f);
            }
            tex[7] = 9;
            logs[s] = t.Run([&] {
                int32_t r1 = s ? rt3::Texture_LoadImage(tex, 0, id, imgs[it % 4])
                               : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, uint8_t*))0x542fb0)(tex, 0, id, imgs[it % 4]);
                int32_t r2 = s ? rt3::Texture_AddAndLoad(tex, 0, id, fmt)
                               : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x542ff0)(tex, 0, id, fmt);
                const char* p = it % 9 == 0 ? "Z:/no/such/dir/x.bin" : path;
                int32_t r3 = s ? rt3::Texture_AppendJpeg(tex, 0, p, 1, 80)
                               : ((int32_t(__fastcall*)(uint8_t*, int, const char*, uint32_t, int32_t))0x542f50)(tex, 0, p, 1, 80);
                if (s) rt3::Net_SendToHost(sess, 0, 0x33, it, (const uint8_t*)0x1234);
                else ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t, const uint8_t*))0x407870)(sess, 0, 0x33, it, (const uint8_t*)0x1234);
                Logf("r %d %d %d +7 %d", r1, r2, r3, *(uint32_t*)(tex + 7));
            });
            logs[s] += B60Read(path);
        }
        CompareLogs(logs[0], logs[1], "texture load helpers", it);
    }
    DeleteFileA(path);
}

namespace rt3 {
void Net_SendReady();
int32_t __fastcall Net_Relay1e(const uint8_t* msg);
int32_t __fastcall Net_Relay20(const uint8_t* msg);
int32_t __fastcall Net_Relay22(const uint8_t* msg);
int32_t __fastcall Net_Relay24(const uint8_t* msg);
int32_t __fastcall Net_SendFiled48(const uint8_t* msg);
int32_t __fastcall Net_SendFiled4c(const uint8_t* msg);
uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
struct GameDate;
int32_t __fastcall DateToMinutes(const GameDate* d);
uint32_t __fastcall Obj_Map9050Insert(void* o, int, uint32_t key, int32_t value);
int32_t __fastcall Save_LoadOptions(const char* name);
void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
void __fastcall Input_RemoveHandler(uint8_t* in, int, uint32_t h);
}

namespace {
uint8_t g_b72S[0x9100], g_b72Part[0x20], g_b72Opt[0x100], g_b72Input[8];
int32_t g_b72HasPart, g_b72Flag;
void __fastcall FakeB72Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    uint32_t h = 2166136261u;
    for (int32_t k = 0; data && k < size && k < 64; ++k) h = (h ^ data[k]) * 16777619u;
    Logf("send %d %p %x %d %x %x %d stamp %d", s == g_b72S, to, type, size, h, flags, serial, *(int32_t*)(g_b72S + 0x8f38));
}
uint8_t* __fastcall FakeB72Part(uint8_t* s, int, int32_t c) {
    Logf("part %d %d", s == g_b72S, c);
    return g_b72HasPart ? g_b72Part : nullptr;
}
int32_t __fastcall FakeB72Date(const rt3::GameDate* d) {
    Logf("date %d", (const uint8_t*)d == g_b72Part);
    return 4321;
}
uint32_t __fastcall FakeB72Insert(void* o, int, uint32_t key, int32_t value) {
    const uint8_t* p = (const uint8_t*)(uintptr_t)value;
    Logf("insert %d %08x %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", o == g_b72S, key, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
         p[8], p[9]);
    return 0;
}
void __fastcall FakeB72Block(const char* name, uint8_t* block, int32_t tut) {
    Logf("block [%s] %d", name, tut);
    for (int k = 0; k < 0x40; ++k) block[k] = (uint8_t)(k * 7 + 3);
    block[0x21] = (uint8_t)g_b72Flag;
}
void __fastcall FakeB72Remove(uint8_t* in, int, uint32_t h) { Logf("remove %d %x", in == g_b72Input, h); }
} // namespace

DIFFTEST(net_SmallSenders) {
    SavedGlobals sg({ 0x6cd8d8, 0x6d40dc, 0x6cec7c, 0x6d4018 });
    UiTrace t;
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB72Send);
    t.Add(0x46b6d0, (void*)&rt3::Net_CountMessagePart, (void*)&FakeB72Part);
    t.Add(0x51d3c0, (void*)&rt3::DateToMinutes, (void*)&FakeB72Date);
    t.Add(0x468ca0, (void*)&rt3::Obj_Map9050Insert, (void*)&FakeB72Insert);
    t.Add(0x442400, (void*)&rt3::Block_Load, (void*)&FakeB72Block);
    t.Add(0x54e790, (void*)&rt3::Input_RemoveHandler, (void*)&FakeB72Remove);
    hook::Global<uint8_t*>(0x6cd8d8) = g_b72S;
    hook::Global<uint8_t*>(0x6cec7c) = g_b72Opt;
    hook::Global<uint8_t*>(0x6d4018) = g_b72Input;
    typedef int32_t(__fastcall * MsgFn)(const uint8_t*);
    const uint32_t exe[] = { 0x471070, 0x4710d0, 0x471130, 0x471190, 0x46cbf0, 0x46cc60 };
    const MsgFn ours[] = { &rt3::Net_Relay1e, &rt3::Net_Relay20, &rt3::Net_Relay22, &rt3::Net_Relay24, &rt3::Net_SendFiled48,
                           &rt3::Net_SendFiled4c };
    static uint8_t msg[0x20], payload[0x20];
    for (int it = 0; it < 120; ++it) {
        g_b72HasPart = Rnd() % 2;
        g_b72Flag = Rnd() % 2;
        hook::Global<int32_t>(0x6d40dc) = Rnd() % 2;
        uint8_t init[0x20];
        for (int k = 0; k < 0x20; ++k) init[k] = (uint8_t)Rnd();
        int32_t size = (int32_t)(Rnd() % 0x20);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memcpy(payload, init, sizeof payload);
            memset(g_b72Opt, 0, sizeof g_b72Opt);
            *(uint8_t**)(msg + 8) = payload;
            *(int32_t*)(msg + 0xc) = size;
            *(int32_t*)(g_b72S + 0x8f38) = -5;
            logs[s] = t.Run([&] {
                if (s) rt3::Net_SendReady();
                else ((void (*)())0x4ee4c0)();
                for (int k = 0; k < 6; ++k) {
                    int32_t r = s ? ours[k](msg) : ((MsgFn)exe[k])(msg);
                    Logf("r %d payload %02x%02x%02x%02x", r, payload[6], payload[7], payload[8], payload[9]);
                }
                int32_t r = s ? rt3::Save_LoadOptions("Save One") : ((int32_t(__fastcall*)(const char*))0x4425d0)("Save One");
                std::string o;
                char b[4];
                for (int k = 0x7c; k < 0xd8; ++k) {
                    sprintf(b, "%02x", g_b72Opt[k]);
                    o += b;
                }
                Logf("options %d %s", r, o.c_str());
            });
        }
        CompareLogs(logs[0], logs[1], "net small senders", it);
    }
}

namespace rt3 {
void Net_SendPendingRecords();
void __fastcall Net_DrainQueue(uint8_t* s, int, int32_t);
void Effects_ReapplyAll();
uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep);
void __fastcall CmdQueue_Clear(uint8_t* q);
void __fastcall Net_Dispatch(uint8_t* s, int, uint8_t* msg, uint32_t arg);
uint8_t* __fastcall EmitterLibrary_Get(uint8_t* lib, int, uint32_t key, uint32_t color, const char* name, uint8_t* obj, int32_t spawn);
void Emitter_Apply();
}

namespace {
uint8_t g_b74S[0x9000], g_b74Entry[0x80], g_b74Msgs[6][0x20], g_b74Fx[0x100], g_b74Lib[8];
int32_t g_b74Keys[8], g_b74KeyPos, g_b74Pops, g_b74NoEntry;
uint8_t g_b74Table[0xc + 5 * 0x140];
void __fastcall FakeB74Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    uint32_t h = 2166136261u;
    for (int32_t k = 0; k < size; ++k) h = (h ^ data[k]) * 16777619u;
    Logf("send [%s] %x %d %08x %x %d", to, type, size, h, flags, serial);
}
int32_t __fastcall FakeB74Top(rt3::MaxHeap* h) {
    Logf("top");
    return g_b74Keys[g_b74KeyPos & 7];
}
int32_t __fastcall FakeB74Pop(rt3::MaxHeap* h) {
    ++g_b74KeyPos;
    return (int32_t)(uintptr_t)g_b74Msgs[g_b74Pops++ % 6];
}
uint8_t* __stdcall FakeB74Lookup(uint8_t* m, int32_t keep) { return m + 1; }
void __fastcall FakeB74Dispatch(uint8_t* s, int, uint8_t* m, uint32_t arg) {
    Logf("dispatch %d %d", (int)((m - g_b74Msgs[0]) / 0x20), (int)(arg - (uint32_t)(uintptr_t)m));
}
void __fastcall FakeB74Free(uint8_t* s, int, uint8_t* p) { Logf("free %p", p); }
void __fastcall FakeB74Clear(uint8_t* q) {
    void** l = (void**)q;
    Logf("clear %d %d %d", l[0] ? (int)(((uint8_t*)l[0] - g_b74Msgs[0]) / 0x20) : -1, l[1] ? (int)(((uint8_t*)l[1] - g_b74Msgs[0]) / 0x20) : -1,
         (int)(uintptr_t)l[2]);
}
void* FakeB74Entry() { return g_b74NoEntry ? nullptr : g_b74Entry; }
uint8_t* __fastcall FakeB74Get(uint8_t* lib, int, uint32_t key, uint32_t c, const char* n, uint8_t* o, int32_t sp) {
    Logf("get %d %08x %x %p %p %d", lib == g_b74Lib, key, c, n, o, sp);
    return g_b74Fx;
}
void __cdecl B74LogApply(uint8_t* e, uint8_t* fx, uint32_t flags, float scale) {
    Logf("apply %d %d %x %08x", (int)(e - g_b74Table), fx == g_b74Fx, flags, B(scale));
}
__declspec(naked) void FakeB74Apply() {
    __asm {
        push [esp + 12]
        push [esp + 12]
        push [esp + 12]
        push eax
        call B74LogApply
        add esp, 16
        ret 12
    }
}
} // namespace

DIFFTEST(net_QueueAndEffects) {
    SavedGlobals sg({ 0x6ce9a0, 0x6cd8d8, 0x6cea60, 0x6d4030 });
    std::vector<uint8_t> recs((uint8_t*)0x6cd990, (uint8_t*)0x6cd990 + 0x12 * 64);
    UiTrace t;
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB74Send);
    t.Add(0x53d2e0, (void*)&rt3::Heap_TopKey, (void*)&FakeB74Top);
    t.Add(0x53d540, (void*)&rt3::Heap_PopMax, (void*)&FakeB74Pop);
    t.Add(0x522050, (void*)&rt3::Textures_Lookup, (void*)&FakeB74Lookup);
    t.Add(0x46ac00, (void*)&rt3::Net_Dispatch, (void*)&FakeB74Dispatch);
    t.Add(0x469890, (void*)&rt3::NetPacket_Free, (void*)&FakeB74Free);
    t.Add(0x5221a0, (void*)&rt3::CmdQueue_Clear, (void*)&FakeB74Clear);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB74Entry);
    t.Add(0x556ce0, (void*)&rt3::EmitterLibrary_Get, (void*)&FakeB74Get);
    t.Add(0x4749d0, (void*)&rt3::Emitter_Apply, (void*)&FakeB74Apply);
    hook::Global<uint8_t*>(0x6cd8d8) = g_b74S;
    hook::Global<uint8_t*>(0x6d4030) = g_b74Lib;
    strcpy((char*)g_b74S + 0x8f48, "Host");
    uint8_t* table = g_b74Table;
    hook::Global<uint8_t*>(0x6cea60) = table;
    for (int it = 0; it < 200; ++it) {
        int32_t n = Rnd() % 5 == 0 ? 0 : (int32_t)(Rnd() % 60);
        for (int k = 0; k < 0x12 * 64; ++k) ((uint8_t*)0x6cd990)[k] = (uint8_t)Rnd();
        int32_t count = (int32_t)(Rnd() % 5);
        for (int k = 0; k < 8; ++k) g_b74Keys[k] = Rnd() % 4 == 0 ? 0 : count - k + (Rnd() % 5 == 0 ? 1 : 0);
        *(int32_t*)(table + 8) = (int32_t)(Rnd() % 6);
        for (int k = 0; k < 5; ++k) {
            uint8_t* e = table + 0xc + k * 0x140;
            *(int32_t*)(e + 0xb8) = Rnd() % 3 == 0 ? 0 : 1;
            *(uint32_t*)(e + 0xbc) = Rnd();
            sprintf((char*)e + 4, "fx%d", (int)(Rnd() % 10));
        }
        int32_t noEntry = Rnd() % 6 == 0;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            hook::Global<int32_t>(0x6ce9a0) = n;
            *(int32_t*)(g_b74Entry + 0x70) = count;
            g_b74KeyPos = 0;
            g_b74Pops = 0;
            g_b74NoEntry = noEntry;
            logs[s] = t.Run([&] {
                if (s) rt3::Net_SendPendingRecords();
                else ((void (*)())0x473b40)();
                if (s) rt3::Net_DrainQueue(g_b74S, 0, 7);
                else ((void(__fastcall*)(uint8_t*, int, int32_t))0x46aeb0)(g_b74S, 0, 7);
                if (s) rt3::Effects_ReapplyAll();
                else ((void (*)())0x474c60)();
                Logf("pending %d count %d", hook::Global<int32_t>(0x6ce9a0), *(int32_t*)(g_b74Entry + 0x70));
            });
        }
        CompareLogs(logs[0], logs[1], "net queue and effects", it);
    }
    memcpy((uint8_t*)0x6cd990, recs.data(), recs.size());
}

namespace rt3 {
void __fastcall Net_SendSettings(uint8_t* w);
int32_t __stdcall Net_SendSlot(int32_t slot, int32_t pick);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Obj_Get21(void* o);
int32_t Global6d40e4();
}

namespace {
uint8_t g_b76S[0x9000], g_b76Cursor[8], g_b76World[0x5000], g_b76Vals[0x40], g_b76Opt[0x200], g_b76Player[0x40], g_b76Coll[8];
uint8_t g_b76Nodes[4][0x40];
uint8_t* g_b76Head[1];
void __fastcall FakeB76Cursor(uint8_t* c, int, int32_t i, int32_t f, int32_t n) { Logf("cursor %d %d %d %d", c == g_b76Cursor, i, f, n); }
int32_t __fastcall FakeB76Get21(void* o) { return 0x1234; }
int32_t FakeB76Engine() { return 0x55667788; }
void __fastcall FakeB76Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    uint32_t h = 2166136261u;
    for (int32_t k = 0; k < size; ++k) h = (h ^ data[k]) * 16777619u;
    Logf("send %d [%s] %x %d %08x %x %d", s == g_b76S, to ? to : "(null)", type, size, h, flags, serial);
}
void* __fastcall FakeB76Get(void* c, int, uint32_t id) {
    Logf("get %u", id);
    return g_b76Player;
}
int32_t FakeB76Lock() {
    Logf("lock");
    return (int32_t)(uintptr_t)g_b76Head;
}
void FakeB76Unlock() { Logf("unlock"); }
} // namespace

DIFFTEST(net_SettingsAndSlot) {
    SavedGlobals sg({ 0xccbb20, 0x6cec78, 0x622b0c, 0x62be68, 0x6cec74, 0x6cd8d8, 0x6ce9bc, 0x6ceb9c });
    std::vector<uint8_t> rec((uint8_t*)0x6d1a40, (uint8_t*)0x6d1a40 + 0x4c);
    UiTrace t;
    t.Add(0x53f000, (void*)&rt3::Cursor_Set, (void*)&FakeB76Cursor);
    t.Add(0x4337a0, (void*)&rt3::Obj_Get21, (void*)&FakeB76Get21);
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeB76Engine);
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB76Send);
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB76Get);
    t.Add(0x521680, (void*)&rt3::Net_LockQueue, (void*)&FakeB76Lock);
    t.Add(0x5216a0, (void*)&rt3::Net_UnlockQueue, (void*)&FakeB76Unlock);
    hook::Global<uint8_t*>(0xccbb20) = g_b76Cursor;
    hook::Global<uint8_t*>(0x6cec78) = g_b76World;
    hook::Global<uint8_t*>(0x62be68) = g_b76Vals;
    hook::Global<uint8_t*>(0x6cec74) = g_b76Opt;
    hook::Global<uint8_t*>(0x6cd8d8) = g_b76S;
    hook::Global<uint8_t*>(0x6ceb9c) = g_b76Coll;
    strcpy((char*)g_b76S + 0x8f48, "Host");
    static const char* names[] = { "Ann", "bob", "Cleo", "Dan" };
    static uint8_t w[0x100];
    for (int it = 0; it < 300; ++it) {
        hook::Global<uint8_t>(0x622b0c) = (uint8_t)Rnd();
        for (int k = 0; k < 0x40; ++k) g_b76Vals[k] = (uint8_t)Rnd();
        for (int k = 0x130; k < 0x150; ++k) g_b76Opt[k] = (uint8_t)Rnd();
        g_b76World[0x4c74] = (uint8_t)Rnd();
        hook::Global<int32_t>(0x6ce9bc) = Rnd() % 2;
        int nodes = Rnd() % 5;
        g_b76Head[0] = nodes ? g_b76Nodes[0] : nullptr;
        for (int k = 0; k < 4; ++k) {
            *(uint8_t**)g_b76Nodes[k] = k + 1 < nodes ? g_b76Nodes[k + 1] : nullptr;
            strcpy((char*)g_b76Nodes[k] + 8, names[(k + it) % 4]);
        }
        const char* own = Rnd() % 3 == 0 ? "nobody" : (Rnd() % 2 ? "BOB" : "cleo");
        int32_t slot = (int32_t)(Rnd() % 8), pick = (int32_t)(Rnd() % 6);
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset((uint8_t*)0x6d1a40, 0xcc, 0x4c);
            strcpy((char*)g_b76Player + 8, own);
            memset(w, 0, sizeof w);
            logs[s] = t.Run([&] {
                if (s) rt3::Net_SendSettings(w);
                else ((void(__fastcall*)(uint8_t*))0x50d9d0)(w);
                int32_t r = s ? rt3::Net_SendSlot(slot, pick) : ((int32_t(__stdcall*)(int32_t, int32_t))0x434200)(slot, pick);
                Logf("r %d w %d own [%s]", r, *(int32_t*)(w + 0x83), (char*)g_b76Player + 8);
            });
        }
        CompareLogs(logs[0], logs[1], "net settings and slot", it);
    }
    memcpy((uint8_t*)0x6d1a40, rec.data(), rec.size());
}

namespace rt3 {
void Net_OfferResync();
void __fastcall Lobby_RefreshPlayers(uint8_t* w);
int32_t FirstFreeSlot6ce2e8();
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
int32_t __fastcall GameList_Refresh(uint8_t* w);
void __fastcall Session_Restart(uint8_t* w, int, int32_t host);
}

namespace {
char g_b77Dir[MAX_PATH];
int32_t g_b77Slot, g_b77Players, g_b77List;
uint8_t g_b77S[0x9900], g_b77Opt[0x100], g_b77Child[0x200], g_b77Entry[0x80], g_b77Nodes[5][0x10];
uint8_t* g_b77Head[1];
std::vector<void*> g_b77Files;
const char* __fastcall FakeB77Folder(void* o) { return g_b77Dir; }
int32_t FakeB77Slot() { return g_b77Slot; }
int32_t FakeB77Players() { return g_b77Players; }
int32_t FakeB77Engine() { return 0x77; }
void __fastcall FakeB77Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    if (type == 0x7f) {
        void* f = *(void**)(data + 0x58);
        if (f) g_b77Files.push_back(f);
        Logf("send 7f %d %x %d [%s] crc %08x open %d size %d file %d", size, *(int32_t*)data, *(int32_t*)(data + 4), (const char*)data + 8,
             *(uint32_t*)(data + 0x48), *(int32_t*)(data + 0x4c), *(int32_t*)(data + 0x50), f != nullptr);
    } else {
        Logf("send %x %d %d %x %d", type, size, size ? data[0] : -1, flags, serial);
    }
}
uint8_t* __fastcall FakeB77Find(uint8_t* w, int, int32_t id) {
    Logf("find %x", id);
    return g_b77Child;
}
uint32_t __fastcall FakeB77Sel(uint8_t* w) {
    Logf("selected");
    return 0;
}
int32_t __fastcall FakeB77Event(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Logf("event %x %x %x %x %x", type, id, a, b, c);
    return 0;
}
int32_t __fastcall FakeB77Refresh(uint8_t* w) {
    Logf("refresh");
    return g_b77List;
}
int32_t FakeB77Lock() { return (int32_t)(uintptr_t)g_b77Head; }
void FakeB77Unlock() { Logf("unlock"); }
void __fastcall FakeB77Restart(uint8_t* w, int, int32_t host) { Logf("restart %d", host); }
void* FakeB77Entry() { return g_b77Entry; }
} // namespace

DIFFTEST(net_ResyncOfferAndLobby) {
    SavedGlobals sg({ 0x6d40dc, 0x6cec74, 0x6cd8d8, 0x6d1290, 0x6cd920 });
    std::vector<uint8_t> slots((uint8_t*)0x6ce290, (uint8_t*)0x6ce290 + 10 * 0x5c);
    std::vector<uint8_t> name((uint8_t*)0x6ce630, (uint8_t*)0x6ce630 + 0x40);
    UiTrace t;
    t.Add(0x4839e0, (void*)&rt3::Options_SaveFolder, (void*)&FakeB77Folder);
    t.Add(0x46c330, (void*)&rt3::FirstFreeSlot6ce2e8, (void*)&FakeB77Slot);
    t.Add(0x521670, (void*)&rt3::Global6d4100, (void*)&FakeB77Players);
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeB77Engine);
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB77Send);
    t.Add(0x53f830, (void*)&rt3::Widget_FindChild, (void*)&FakeB77Find);
    t.Add(0x55fba0, (void*)&rt3::TreeView_SelectedData, (void*)&FakeB77Sel);
    t.Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&FakeB77Event);
    t.Add(0x4ed670, (void*)&rt3::GameList_Refresh, (void*)&FakeB77Refresh);
    t.Add(0x521680, (void*)&rt3::Net_LockQueue, (void*)&FakeB77Lock);
    t.Add(0x5216a0, (void*)&rt3::Net_UnlockQueue, (void*)&FakeB77Unlock);
    t.Add(0x4edc40, (void*)&rt3::Session_Restart, (void*)&FakeB77Restart);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB77Entry);
    hook::Global<uint8_t*>(0x6cec74) = g_b77Opt;
    hook::Global<uint8_t*>(0x6cd8d8) = g_b77S;
    GetTempPathA(MAX_PATH, g_b77Dir);
    g_b77Dir[strlen(g_b77Dir) - 1] = 0;
    char file[MAX_PATH];
    sprintf(file, "%s\\rt3re_resync.gm", g_b77Dir);
    static uint8_t w[0x100];
    for (int it = 0; it < 200; ++it) {
        hook::Global<int32_t>(0x6d40dc) = Rnd() % 5 != 0;
        g_b77Players = Rnd() % 4;
        g_b77Slot = Rnd() % 4 == 0 ? 10 : (int32_t)(Rnd() % 10);
        strcpy((char*)0x6ce630, Rnd() % 4 == 0 ? "missing.gm" : "rt3re_resync.gm");
        {
            FILE* f = fopen(file, "wb");
            int n = Rnd() % 3000;
            for (int k = 0; k < n; ++k) fputc(k & 0xff, f);
            fclose(f);
        }
        int32_t browser = Rnd() % 2;
        g_b77List = Rnd() % 3;
        hook::Global<int32_t>(0x6cd920) = Rnd() % 2;
        *(int32_t*)(g_b77Child + 0x1d6) = (int32_t)(Rnd() % 3) - 1;
        *(int32_t*)(g_b77Child + 0x1d2) = Rnd() % 2;
        int nodes = Rnd() % 6;
        g_b77Head[0] = nodes ? g_b77Nodes[0] : nullptr;
        for (int k = 0; k < 5; ++k) {
            *(uint8_t**)g_b77Nodes[k] = k + 1 < nodes ? g_b77Nodes[k + 1] : nullptr;
            *(uint32_t*)(g_b77Nodes[k] + 4) = 0x100 + k;
        }
        g_b77Entry[0x5c] = (uint8_t)Rnd();
        *(int32_t*)(g_b77S + 0x985c) = Rnd() % 2;
        uint8_t guard = Rnd() % 8 == 0;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset((uint8_t*)0x6ce290, 0xee, 10 * 0x5c);
            memset(w, 0, sizeof w);
            *(int32_t*)(w + 0x7c) = browser ? 0xe13 : 0xe14;
            hook::Global<uint8_t>(0x6d1290) = guard;
            logs[s] = t.Run([&] {
                if (s) rt3::Net_OfferResync();
                else ((void (*)())0x46d640)();
                if (s) rt3::Lobby_RefreshPlayers(w);
                else ((void(__fastcall*)(uint8_t*))0x4ee540)(w);
                Logf("guard %d opt6c %d", hook::Global<uint8_t>(0x6d1290), *(int32_t*)(g_b77Opt + 0x6c));
            });
            for (void* f : g_b77Files) ((int(__cdecl*)(void*))0x5a1740)(f);
            g_b77Files.clear();
        }
        CompareLogs(logs[0], logs[1], "net resync offer and lobby", it);
    }
    DeleteFileA(file);
    memcpy((uint8_t*)0x6ce290, slots.data(), slots.size());
    memcpy((uint8_t*)0x6ce630, name.data(), name.size());
}

namespace rt3 {
void __fastcall EditorDialog_Init(uint8_t* w, int, int32_t tab, int32_t city);
}

namespace {
void __fastcall FakeB78Load(uint8_t* w, int, float x, float y, const char* name) { Logf("load %08x %08x [%s]", B(x), B(y), name); }
int32_t __fastcall FakeB78Send(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    if (type == 0x89) {
        const uint32_t* it = (const uint32_t*)(uintptr_t)a;
        Logf("item [%s] [%s] %x %x %x mute %d", (const char*)(uintptr_t)it[0], (const char*)(uintptr_t)it[1], it[2], it[3], it[4],
             *(int32_t*)(w + 0x7c));
    } else if (type == 0x6f) {
        Logf("send 6f %x [%s]", id, (const char*)(uintptr_t)a);
    } else {
        Logf("send %x %x %x %x %x mute %d", type, id, a, b, c, *(int32_t*)(w + 0x7c));
    }
    return 0;
}
const char* __fastcall FakeB78String(int32_t id) {
    static char s[4][16];
    static int k;
    char* p = s[k++ & 3];
    sprintf(p, "str %x", id);
    return p;
}
void __fastcall FakeB78Tab(uint8_t* w, int, int32_t tab, int32_t city) {
    Logf("set tab %d %d tab78 %d mute %d", tab, city, *(int32_t*)(w + 0x78), *(int32_t*)(w + 0x7c));
}
} // namespace

DIFFTEST(ui_EditorInit) {
    SavedGlobals sg({ 0x6d3c84 });
    UiTrace t;
    t.Add(0x53fa50, (void*)&rt3::Window_Load, (void*)&FakeB78Load);
    t.Add(0x540120, (void*)&rt3::Window_SendEvent5, (void*)&FakeB78Send);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB78String);
    t.Add(0x4ce070, (void*)&rt3::EditorDialog_SetTab, (void*)&FakeB78Tab);
    static uint8_t w[0x100];
    for (int it = 0; it < 30; ++it) {
        hook::Global<int32_t>(0x6d3c84) = it % 6;
        int32_t tab = (int32_t)(Rnd() % 20), city = (int32_t)(Rnd() % 5) - 1;
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            memset(w, 0x5a, sizeof w);
            logs[s] = t.Run([&] {
                if (s) rt3::EditorDialog_Init(w, 0, tab, city);
                else ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4cf550)(w, 0, tab, city);
            });
        }
        CompareLogs(logs[0], logs[1], "editor init", it);
    }
}

namespace rt3 {
void __fastcall Net_FeedResync(uint8_t* s);
int32_t __fastcall Net_SendQueueTotals(uint8_t* o, int, int32_t* msgs, int32_t* bytes);
char* __stdcall Net_PlayerNameById(int32_t id);
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
}

namespace {
int32_t g_b79Engine, g_b79Queued, g_b79Ok;
uint8_t g_b79Ui[8];
int32_t FakeB79Engine() { return g_b79Engine; }
int32_t __fastcall FakeB79Totals(uint8_t* o, int, int32_t* m, int32_t* b) {
    *m = g_b79Queued;
    *b = 99;
    return g_b79Ok;
}
char* __stdcall FakeB79Name(int32_t id) {
    static char s[16];
    sprintf(s, "player%d", id);
    return s;
}
void __fastcall FakeB79Status(uint8_t* s, int, const char* text) { Logf("status %d [%s]", s == g_b79Ui, text); }
void __fastcall FakeB79Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    uint32_t h = 2166136261u;
    for (int32_t k = 0; k < size; ++k) h = (h ^ data[k]) * 16777619u;
    Logf("send %x %d %08x %x", type, size, h, flags);
}
const char* __fastcall FakeB79String(int32_t id) { return "%1 to %2: %3%"; }
} // namespace

DIFFTEST(net_FeedResync) {
    SavedGlobals sg({ 0x6d401c, 0x6ce9cc, 0x6cd8d8, 0x6d40d0 });
    std::vector<uint8_t> slots((uint8_t*)0x6ce290, (uint8_t*)0x6ce290 + 10 * 0x5c);
    UiTrace t;
    t.Add(0x5216c0, (void*)&rt3::Global6d40e4, (void*)&FakeB79Engine);
    t.Add(0x522990, (void*)&rt3::Net_SendQueueTotals, (void*)&FakeB79Totals);
    t.Add(0x522580, (void*)&rt3::Net_PlayerNameById, (void*)&FakeB79Name);
    t.Add(0x5387a0, (void*)&rt3::Status_SetMessage, (void*)&FakeB79Status);
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB79Send);
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB79String);
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_feed_test.bin");
    static uint8_t s[0x10];
    for (int it = 0; it < 60; ++it) {
        int32_t len = Rnd() % 4 == 0 ? 0xf00 * (int32_t)(Rnd() % 3) : (int32_t)(Rnd() % 120000);
        {
            FILE* f = fopen(path, "wb");
            for (int k = 0; k < len; ++k) fputc((k * 31 + it) & 0xff, f);
            fclose(f);
        }
        g_b79Engine = Rnd() % 6 == 0 ? 0 : 7;
        g_b79Queued = Rnd() % 4 == 0 ? 30 : (int32_t)(Rnd() % 25);
        g_b79Ok = Rnd() % 5 != 0;
        hook::Global<uint8_t*>(0x6d401c) = Rnd() % 3 == 0 ? nullptr : g_b79Ui;
        int used = 1 + Rnd() % 3;
        int32_t addr = Rnd() % 3 == 0 ? 4 : 0;
        int32_t skip = (int32_t)(Rnd() % 3) * 0xf00;
        std::string logs[2];
        for (int sd = 0; sd < 2; ++sd) {
            std::vector<void*> files;
            memset((uint8_t*)0x6ce290, 0, 10 * 0x5c);
            for (int k = 0; k < used; ++k) {
                uint8_t* rec = (uint8_t*)0x6ce290 + (k * 3) * 0x5c;
                *(int32_t*)rec = k == 2 ? 9 : 7;
                *(int32_t*)(rec + 4) = addr;
                sprintf((char*)rec + 8, "resync%d.gm", k);
                *(uint32_t*)(rec + 0x48) = 0xabc0 + k;
                *(int32_t*)(rec + 0x50) = len ? len : 1;
                *(int32_t*)(rec + 0x54) = 0;
                void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, "rb");
                ((int(__cdecl*)(void*, long, int))0x5a261f)(f, k == 0 ? 0 : skip, 0);
                *(void**)(rec + 0x58) = f;
                files.push_back(f);
            }
            logs[sd] = t.Run([&] {
                if (sd) rt3::Net_FeedResync(s);
                else ((void(__fastcall*)(uint8_t*))0x46d300)(s);
                for (int k = 0; k < used; ++k) {
                    uint8_t* rec = (uint8_t*)0x6ce290 + (k * 3) * 0x5c;
                    Logf("slot %d %x sent %d open %d", k, *(int32_t*)rec, *(int32_t*)(rec + 0x54), *(void**)(rec + 0x58) != nullptr);
                }
                Logf("last %d", hook::Global<uint8_t*>(0x6ce9cc) == s);
            });
            for (int k = 0; k < used; ++k) {
                uint8_t* rec = (uint8_t*)0x6ce290 + (k * 3) * 0x5c;
                if (*(void**)(rec + 0x58)) ((int(__cdecl*)(void*))0x5a1740)(*(void**)(rec + 0x58));
            }
        }
        CompareLogs(logs[0], logs[1], "net feed resync", it);
    }
    DeleteFileA(path);
    memcpy((uint8_t*)0x6ce290, slots.data(), slots.size());
}

namespace rt3 {
int32_t __fastcall Owner_PickBuildingToSell(uint8_t* o, int, int32_t kind);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
void* __fastcall Obj_Ref3cc(void* o);
}

namespace {
uint8_t g_b80Blds[8][0x400], g_b80States[8][0x800], g_b80Type[0x200], g_b80World[0x46c40];
void* g_b80BldVt[0x40];
void* g_b80CollVt[8];
void* g_b80Coll[2] = { g_b80CollVt, nullptr };
int32_t g_b80Mp, g_b80Rand;
uint8_t* __fastcall FakeB80State(uint8_t* b) { return g_b80States[(b - g_b80Blds[0]) / 0x400]; }
void* __fastcall FakeB80Get(void* c, int, uint32_t id) {
    Logf("get %u", id);
    return g_b80Blds[id % 8];
}
double __fastcall FakeB80Rem(void* o, int, int32_t i) { return (double)*(float*)((uint8_t*)o + 0x100 + i * 4); }
double __fastcall FakeB80Out(void* o, int, int32_t i) { return (double)*(float*)((uint8_t*)o + 0x120 + i * 4); }
int32_t __fastcall FakeB80Rand(int32_t lo, int32_t hi) {
    g_b80Rand = g_b80Rand * 1103515245 + 12345;
    int32_t r = lo + (int32_t)((uint32_t)(g_b80Rand >> 8) % (uint32_t)(hi - lo + 1));
    Logf("rand %d %d -> %d", lo, hi, r);
    return r;
}
void* __fastcall FakeB80Ref(void* o) { return g_b80Type; }
int32_t FakeB80Mp() { return g_b80Mp; }
void __fastcall FakeB80Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    Logf("send [%s] %x %d %d %x", to, type, size, *(const int32_t*)data, flags);
}
void __fastcall FakeB80Remove(void* c, int, int32_t id) { Logf("remove %d", id); }
} // namespace

DIFFTEST(game_PickBuildingToSell) {
    SavedGlobals sg({ 0x62b26c, 0x6cec78, 0x6cd8d8 });
    UiTrace t;
    t.Add(0x518140, (void*)&rt3::Collection_Get, (void*)&FakeB80Get);
    t.Add(0x40cac0, (void*)&rt3::Obj_Remaining, (void*)&FakeB80Rem);
    t.Add(0x40ca90, (void*)&rt3::Obj_Float310, (void*)&FakeB80Out);
    t.Add(0x518be0, (void*)&rt3::SimRand, (void*)&FakeB80Rand);
    t.Add(0x40cec0, (void*)&rt3::Obj_Ref3cc, (void*)&FakeB80Ref);
    t.Add(0x434050, (void*)&rt3::IsMultiplayer, (void*)&FakeB80Mp);
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB80Send);
    g_b80BldVt[0x80 / 4] = (void*)&FakeB80State;
    g_b80CollVt[2] = (void*)&FakeB80Remove;
    hook::Global<void*>(0x62b26c) = g_b80Coll;
    hook::Global<uint8_t*>(0x6cec78) = g_b80World;
    static uint8_t sess[0x9000], o[0x400];
    strcpy((char*)sess + 0x8f48, "Host");
    hook::Global<uint8_t*>(0x6cd8d8) = sess;
    for (int it = 0; it < 3000; ++it) {
        int n = Rnd() % 7;
        for (int k = 0; k < 8; ++k) {
            uint8_t* b = g_b80Blds[k];
            memset(b, 0, 0x400);
            *(void***)b = g_b80BldVt;
            *(uint16_t*)(b + 0x270) = (uint16_t)(k + 1 < n ? k + 2 : 0);
            *(uint32_t*)(b + 0x276) = Rnd() % 6 == 0;
            b[0x390] = Rnd() % 8 == 0;
            *(int32_t*)(b + 0x3d5) = (int32_t)(Rnd() % 8);
            *(int32_t*)(b + 0x2a4) = (int32_t)(Rnd() % 4 == 0 ? 0 : 10 + k);
            for (int i = 0; i < 4; ++i) {
                *(float*)(b + 0x104 + i * 4) = RndF(-50.0f, 400.0f);
                *(float*)(b + 0x124 + i * 4) = Rnd() % 5 == 0 ? 0.0f : RndF(0.0f, 300.0f);
            }
            g_b80States[k][0x32] = (uint8_t)(Rnd() % 4);
            *(int32_t*)(g_b80States[k] + 0x790) = Rnd() % 3 == 0 ? 0 : 1;
        }
        *(uint16_t*)(o + 0x383) = (uint16_t)(n ? 1 : 0);
        *(int32_t*)(o + 0x28e) = (int32_t)(Rnd() % 4);
        *(uint16_t*)(g_b80World + 0xd) = (uint16_t)(3 + Rnd() % 6);
        *(int32_t*)(g_b80World + 0x46c3c) = Rnd() % 4 == 0;
        *(int32_t*)(g_b80Type + 0x141) = (int32_t)(Rnd() % 100000);
        g_b80Mp = Rnd() % 2;
        int32_t kind = Rnd() % 2 ? 2 : (int32_t)(Rnd() % 4);
        int32_t seed = (int32_t)Rnd();
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            g_b80Rand = seed;
            logs[s] = t.Run([&] {
                int32_t r = s ? rt3::Owner_PickBuildingToSell(o, 0, kind)
                              : ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x422be0)(o, 0, kind);
                Logf("r %d", r);
            });
        }
        CompareLogs(logs[0], logs[1], "pick building to sell", it);
    }
}

namespace rt3 {
void __fastcall Net_WeeklyCheck(uint8_t* s);
uint8_t* __fastcall Obj_BuildStatusPacket(uint8_t* o, int, int32_t* outSize);
void __stdcall Table9_Append(uint8_t* o, const uint8_t* src);
uint8_t* __fastcall Net_FindFlaggedPlayer();
uint8_t* __stdcall Table9_Find(uint8_t* o, const uint8_t* e);
uint32_t __fastcall Obj_FindNode8f28(void* o, int, int32_t key);
void __stdcall Table9_Prune(uint8_t* o, uint32_t limit);
uint8_t* __stdcall Textures_Find(int32_t key);
void __stdcall Net_UnlockQueueArg(int32_t);
}

namespace {
uint8_t g_b81World[0x20], g_b81Entry[0x10], g_b81Q[0x180 + 2 * 0x1200], g_b81Found[8][9], g_b81Other[8];
uint8_t g_b81Nodes[3][0xa0];
void* g_b81List[1];
int32_t g_b81Players, g_b81Clock, g_b81FindMask, g_b81Hit;
uint8_t g_b81Src[3][9];
const uint8_t* B81Src0() { return g_b81Hit & 1 ? g_b81Src[0] : nullptr; }
const uint8_t* B81Src1() { return g_b81Hit & 2 ? g_b81Src[1] : nullptr; }
const uint8_t* B81Src2() { return g_b81Hit & 4 ? g_b81Src[2] : nullptr; }
void* const g_b81Fns[3] = { (void*)&B81Src0, (void*)&B81Src1, (void*)&B81Src2 };
int32_t __fastcall FakeB81Date(const rt3::GameDate* d) { return 5000; }
int32_t FakeB81Players() { return g_b81Players; }
int32_t FakeB81Clock() { return g_b81Clock += 7; }
uint8_t* __fastcall FakeB81Build(uint8_t* o, int, int32_t* size) {
    *size = 33;
    Logf("build");
    return (uint8_t*)0x1234;
}
void __fastcall FakeB81Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                            int32_t serial) {
    Logf("send %p %x %d %p %x %d", to, type, size, data, flags, serial);
}
void __cdecl FakeB81Delete(void* p) { Logf("delete %p", p); }
void* FakeB81Entry() { return g_b81Entry; }
void __stdcall FakeB81Append(uint8_t* o, const uint8_t* src) { Logf("append %d %d", o == g_b81Entry, (int)(src - g_b81Src[0]) / 9); }
uint8_t* __fastcall FakeB81Flagged() { return g_b81Q; }
uint8_t* __stdcall FakeB81Find(uint8_t* o, const uint8_t* e) {
    int i = (int)((e - (g_b81Q + 0x180 + *(int32_t*)(g_b81Q + 0x17c) * 0x1200)) / 9);
    Logf("find %d", i);
    return (g_b81FindMask >> (i & 7)) & 1 ? g_b81Found[i & 7] : nullptr;
}
uint32_t __fastcall FakeB81Node(void* o, int, int32_t key) { return (uint32_t)(uintptr_t)g_b81Fns[key % 3]; }
void __stdcall FakeB81Prune(uint8_t* o, uint32_t limit) { Logf("prune %d %u", o == g_b81Entry ? 0 : o == g_b81Other ? 1 : 2, limit); }
uint8_t* __stdcall FakeB81Tex(int32_t key) { return g_b81Other; }
void __stdcall FakeB81Unlock(int32_t k) { Logf("unlock %d", k); }
int32_t __fastcall FakeB81SendTo(void* net, int, const uint8_t* data, uint32_t size, uint32_t type, uint32_t ch, const char* to,
                                 uint32_t flags, uint32_t timeout, int32_t serial, int32_t last) {
    Logf("sendto %d %u %x %x [%s] %x lock %d", (int)(data - g_b81Found[0]) / 9, size, type, ch, to, flags, *(volatile LONG*)0x6cd960);
    return 0;
}
} // namespace

DIFFTEST(net_WeeklyCheck) {
    SavedGlobals sg({ 0x6cd908, 0x6cec78, 0x6cd8ec, 0x6d40dc, 0x6cd900, 0x6ae4cc, 0x6cd918, 0x6cd91c, 0x5f2b3c, 0x6cd960, 0x6cd930,
                      0x6cd934 });
    UiTrace t;
    t.Add(0x51d3c0, (void*)&rt3::DateToMinutes, (void*)&FakeB81Date);
    t.Add(0x521670, (void*)&rt3::Global6d4100, (void*)&FakeB81Players);
    t.Add(0x51d890, (void*)&rt3::ClockMs, (void*)&FakeB81Clock);
    t.Add(0x469be0, (void*)&rt3::Obj_BuildStatusPacket, (void*)&FakeB81Build);
    t.Add(0x469d30, (void*)&rt3::Net_Send, (void*)&FakeB81Send);
    t.Add(0x53b080, nullptr, (void*)&FakeB81Delete);
    t.Add(0x521d40, (void*)&rt3::Engine_FirstListEntry, (void*)&FakeB81Entry);
    t.Add(0x468c10, (void*)&rt3::Table9_Append, (void*)&FakeB81Append);
    t.Add(0x522510, (void*)&rt3::Net_FindFlaggedPlayer, (void*)&FakeB81Flagged);
    t.Add(0x468c50, (void*)&rt3::Table9_Find, (void*)&FakeB81Find);
    t.Add(0x4699e0, (void*)&rt3::Obj_FindNode8f28, (void*)&FakeB81Node);
    t.Add(0x468b70, (void*)&rt3::Table9_Prune, (void*)&FakeB81Prune);
    t.Add(0x521d60, (void*)&rt3::Textures_Find, (void*)&FakeB81Tex);
    t.Add(0x5216d0, (void*)&rt3::Net_UnlockQueueArg, (void*)&FakeB81Unlock);
    t.Add(0x521dc0, (void*)&rt3::Net_SendTo, (void*)&FakeB81SendTo);
    static uint8_t s[0x9000];
    *(void***)(s + 0x8f28) = g_b81List;
    strcpy((char*)g_b81Q + 8, "HostName");
    for (int it = 0; it < 2000; ++it) {
        hook::Global<uint8_t*>(0x6cec78) = Rnd() % 10 == 0 ? nullptr : g_b81World;
        g_b81World[0x11] = (uint8_t)(Rnd() % 3 == 0 ? Rnd() : 7 * (Rnd() % 30) + 1);
        int32_t started = Rnd() % 8 != 0;
        hook::Global<int32_t>(0x6cd8ec) = Rnd() % 8 != 0;
        hook::Global<int32_t>(0x6d40dc) = Rnd() % 3 == 0;
        hook::Global<int32_t>(0x6cd900) = Rnd() % 2;
        g_b81Players = Rnd() % 3;
        int nodes = 1 + Rnd() % 3;
        g_b81List[0] = g_b81Nodes[0];
        for (int k = 0; k < 3; ++k) {
            *(uint8_t**)g_b81Nodes[k] = k + 1 < nodes ? g_b81Nodes[k + 1] : nullptr;
            *(void**)(g_b81Nodes[k] + 4) = g_b81Fns[k];
        }
        g_b81Hit = Rnd() % 8;
        *(int32_t*)(g_b81Q + 4) = 42;
        *(int32_t*)(g_b81Q + 0x17c) = Rnd() % 2;
        *(int32_t*)(g_b81Q + 0x178) = Rnd() % 8;
        uint8_t* recs = g_b81Q + 0x180 + *(int32_t*)(g_b81Q + 0x17c) * 0x1200;
        for (int k = 0; k < 8; ++k) {
            g_b81Found[k][0] = (uint8_t)(Rnd() % nodes);
            *(int32_t*)(g_b81Found[k] + 1) = Rnd() % 4 == 0 ? 0 : (int32_t)(Rnd() % 3);
            recs[k * 9] = (uint8_t)k;
            *(int32_t*)(recs + k * 9 + 1) = Rnd() % 4 == 0 ? 0 : (int32_t)(Rnd() % 3);
            *(uint32_t*)(recs + k * 9 + 5) = Rnd();
        }
        g_b81FindMask = Rnd() % 256;
        int32_t s918 = Rnd() % 2, s91c = Rnd() % 2, s5f = Rnd() % 2;
        std::string logs[2];
        for (int sd = 0; sd < 2; ++sd) {
            *(int32_t*)(s + 0x8f30) = started;
            for (int k = 0; k < 3; ++k) *(int32_t*)(g_b81Nodes[k] + 0x98) = 7;
            hook::Global<int32_t>(0x6cd918) = s918;
            hook::Global<int32_t>(0x6cd91c) = s91c;
            hook::Global<int32_t>(0x5f2b3c) = s5f;
            hook::Global<int32_t>(0x6cd930) = 0;
            hook::Global<int32_t>(0x6cd934) = 0;
            g_b81Clock = 100;
            logs[sd] = t.Run([&] {
                if (sd) rt3::Net_WeeklyCheck(s);
                else ((void(__fastcall*)(uint8_t*))0x46b360)(s);
                Logf("908 %x 918 %d 91c %d 5f %d 4cc %d tot %d %d 8f44 %x flags %d %d %d", hook::Global<uint32_t>(0x6cd908),
                     hook::Global<int32_t>(0x6cd918), hook::Global<int32_t>(0x6cd91c), hook::Global<int32_t>(0x5f2b3c),
                     hook::Global<int32_t>(0x6ae4cc), hook::Global<int32_t>(0x6cd930), hook::Global<int32_t>(0x6cd934),
                     *(uint32_t*)(s + 0x8f44), *(int32_t*)(g_b81Nodes[0] + 0x98), *(int32_t*)(g_b81Nodes[1] + 0x98),
                     *(int32_t*)(g_b81Nodes[2] + 0x98));
            });
        }
        CompareLogs(logs[0], logs[1], "net weekly check", it);
    }
}
