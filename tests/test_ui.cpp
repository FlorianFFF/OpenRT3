// Difftests for UI builders and draw callbacks: the leaf calls they make (text,
// glyphs, lines, widget constructors, window events, strings, allocations) are
// redirected to recorders, and the original's call log is compared with the
// rewrite's. Allocations come from a bump arena that restarts for each side, so
// pointers to new objects and strings match when the calls do.
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
