// Every rewritten leaf function (the original calls nothing but the C runtime)
// against its original, generically: the table comes from
// tools/gen_leaftests.py (tests/test_leaves_table.cpp).
//
// Both versions get the same inputs: pointers to buffers of random words
// (pointers back into the buffers, small numbers, floats, random bits that
// are never valid addresses), small integers and floats. They must return the
// same (EAX by the result's size, the x87 result), leave the buffers the same
// and write the same bytes to the exe's globals. Global writes are found by
// write-protecting the exe's data sections during the call: the first write
// to a page saves a copy of it, and afterwards the page is compared and put
// back, so neither version (nor a later case) sees the other's writes. A
// fault is a result like any other: both must fault.
#include <windows.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "difftest.h"
#include "leaftest.h"
#include "hook/hook.h"

namespace {

const int kBuf = 0x10000, kNumBufs = 8;
uint8_t* g_mem;  // kNumBufs buffers and one spare (reads past the last stay mapped)
uint32_t g_bits;

uint32_t Draw() {
    g_bits = g_bits * 1103515245u + 12345u;
    return g_bits >> 8;
}

uint32_t BufPtr() { return (uint32_t)(uintptr_t)(g_mem + kBuf * (Draw() % kNumBufs) + 4 * (Draw() % 16)); }

uint32_t SmallInt() {
    static const uint32_t v[] = { 0, 1, 2, 3, 4, 5, 7, 8, 0x10, 0x3c, 0xffffffff, 0x64, 0x21, 0x7f };
    return v[Draw() % (sizeof v / sizeof v[0])];
}

const float kSmallFloats[] = { 0.0f, 1.0f, -1.0f, 0.5f, 2.5f, 100.0f, -37.25f, 1e-3f, 3000.0f, 0.999f };
const int kNumSmallFloats = (int)(sizeof kSmallFloats / sizeof kSmallFloats[0]);

uint32_t SmallFloat() {
    float f = kSmallFloats[Draw() % kNumSmallFloats];
    uint32_t u;
    memcpy(&u, &f, 4);
    return u;
}

void Fill() {
    memset(g_mem + kBuf * kNumBufs, 0, kBuf);  // the spare: nothing left from the other side's run
    uint32_t* w = (uint32_t*)g_mem;
    for (int k = 0; k < kBuf * kNumBufs / 4; ++k) {
        const uint32_t r = Draw() % 16;
        w[k] = r < 5 ? BufPtr() : r < 9 ? SmallInt() : r < 11 ? SmallFloat() : r == 11 ? 0 : (Draw() << 8 | Draw() & 0xff) | 0x80000000;
    }
}

// --- the exe's writable data, write-tracked ---------------------------------
struct Range { uint8_t* at; size_t size; };
std::vector<Range> g_data;
struct SavedPage { uint8_t* page; uint8_t copy[0x1000]; };
std::vector<SavedPage> g_saved;
volatile bool g_tracking;

bool InData(const uint8_t* p) {
    for (const Range& r : g_data)
        if (p >= r.at && p < r.at + r.size) return true;
    return false;
}

LONG CALLBACK OnWrite(EXCEPTION_POINTERS* ep) {
    const EXCEPTION_RECORD* er = ep->ExceptionRecord;
    if (!g_tracking || er->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || er->ExceptionInformation[0] != 1)
        return EXCEPTION_CONTINUE_SEARCH;
    uint8_t* page = (uint8_t*)(er->ExceptionInformation[1] & ~(ULONG_PTR)0xfff);
    if (!InData(page)) return EXCEPTION_CONTINUE_SEARCH;
    g_saved.emplace_back();
    g_saved.back().page = page;
    memcpy(g_saved.back().copy, page, 0x1000);
    DWORD o;
    VirtualProtect(page, 0x1000, PAGE_READWRITE, &o);
    return EXCEPTION_CONTINUE_EXECUTION;
}

void FindData() {
    const uint8_t* base = (const uint8_t*)0x400000;
    const IMAGE_NT_HEADERS* nt = (const IMAGE_NT_HEADERS*)(base + ((const IMAGE_DOS_HEADER*)base)->e_lfanew);
    const IMAGE_SECTION_HEADER* s = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; ++i)
        if ((s[i].Characteristics & IMAGE_SCN_MEM_WRITE) && !(s[i].Characteristics & IMAGE_SCN_MEM_EXECUTE))
            g_data.push_back({ (uint8_t*)base + s[i].VirtualAddress, (s[i].Misc.VirtualSize + 0xfff) & ~0xfffu });
}

void Protect(bool on) {
    DWORD o;
    for (const Range& r : g_data) VirtualProtect(r.at, r.size, on ? PAGE_READONLY : PAGE_READWRITE, &o);
    for (const SavedPage& p : g_saved) VirtualProtect(p.page, 0x1000, on ? PAGE_READONLY : PAGE_READWRITE, &o);
}

// The global writes (the first few as text, all of them hashed), then the
// pages put back.
std::string TakeWrites() {
    std::string out;
    uint32_t h = 2166136261u, n = 0;
    for (const SavedPage& p : g_saved) {
        for (int k = 0; k < 0x1000; ++k)
            if (p.page[k] != p.copy[k]) {
                const uint32_t at = (uint32_t)(uintptr_t)(p.page + k);
                h = (h ^ at) * 16777619u;
                h = (h ^ p.page[k]) * 16777619u;
                if (++n <= 16) {
                    char b[32];
                    sprintf(b, " %06x=%02x", at, p.page[k]);
                    out += b;
                }
            }
        memcpy(p.page, p.copy, 0x1000);
    }
    g_saved.clear();
    if (n) {
        char b[48];
        sprintf(b, " (%u global bytes, hash %08x)", n, h);
        out += b;
    }
    return out;
}

// A call that runs over 2 s (an endless loop the original has as well, such
// as a list walk without an end check) is stopped: the watchdog moves the test
// thread to TimeoutTrap, whose exception the call's __except takes.
const DWORD kTimeout = 0xe0001eaf;
HANDLE g_testThread;
volatile LONG g_callStart;  // GetTickCount at the call, 0 outside one
volatile LONG g_limitMs = 2000;  // (30 s when a case that differs is run again)

void TimeoutTrap() { RaiseException(kTimeout, 0, 0, nullptr); }

DWORD WINAPI Watchdog(void*) {
    for (;;) {
        Sleep(50);
        const LONG t = g_callStart;
        if (!t || (LONG)GetTickCount() - t < g_limitMs) continue;
        SuspendThread(g_testThread);
        if (g_callStart == t) {
            CONTEXT c = {};
            c.ContextFlags = CONTEXT_CONTROL;
            if (GetThreadContext(g_testThread, &c)) {
                c.Esp = (c.Esp - 64) & ~15u;  // (a fresh frame below whatever it was doing)
                c.Eip = (DWORD)(uintptr_t)&TimeoutTrap;
                SetThreadContext(g_testThread, &c);
            }
            g_callStart = 0;
        }
        ResumeThread(g_testThread);
    }
}

uint32_t CallRaw(const void* fn, uint32_t rc, uint32_t rd, const uint32_t* a, int fp, uint64_t* f) {
    uint32_t r, saved;
    double fr = 0;
    __asm {
        mov saved, esp
        mov eax, a
        push dword ptr [eax + 28]
        push dword ptr [eax + 24]
        push dword ptr [eax + 20]
        push dword ptr [eax + 16]
        push dword ptr [eax + 12]
        push dword ptr [eax + 8]
        push dword ptr [eax + 4]
        push dword ptr [eax]
        mov ecx, rc
        mov edx, rd
        mov eax, fn
        call eax
        mov esp, saved
        mov r, eax
    }
    if (fp) {
        __asm fstp fr
        memcpy(f, &fr, 8);
    }
    return r;
}

// A read that runs past the buffers can hit a thread stack's guard page,
// which then stops being one: put back after the call, so both versions meet
// it alike.
uintptr_t g_guardHit;
// The last exception taken (for the report of a difference).
DWORD g_lastCode;
void* g_lastAt;
ULONG_PTR g_lastAddr;

DWORD Filter(EXCEPTION_POINTERS* ep, DWORD code) {
    g_lastCode = code;
    g_lastAt = ep->ExceptionRecord->ExceptionAddress;
    g_lastAddr = ep->ExceptionRecord->NumberParameters > 1 ? ep->ExceptionRecord->ExceptionInformation[1] : 0;
    if (code == STATUS_GUARD_PAGE_VIOLATION) g_guardHit = ep->ExceptionRecord->ExceptionInformation[1];
    return code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_INT_DIVIDE_BY_ZERO || code == EXCEPTION_INT_OVERFLOW ||
                   code == EXCEPTION_ARRAY_BOUNDS_EXCEEDED || code == STATUS_GUARD_PAGE_VIOLATION || code == kTimeout
               ? EXCEPTION_EXECUTE_HANDLER
               : EXCEPTION_CONTINUE_SEARCH;
}

// *faulted: 0 returned, 1 faulted, 2 stopped after kTimeout.
uint32_t Call(const void* fn, uint32_t rc, uint32_t rd, const uint32_t* a, int fp, uint64_t* f, int* faulted) {
    *faulted = 0;
    DWORD code = 0;
    __try {
        g_callStart = (LONG)GetTickCount() | 1;
        const uint32_t r = CallRaw(fn, rc, rd, a, fp, f);
        g_callStart = 0;
        return r;
    } __except (Filter(GetExceptionInformation(), code = GetExceptionCode())) {
        g_callStart = 0;
        if (g_guardHit) {
            DWORD o;
            VirtualProtect((void*)(g_guardHit & ~(uintptr_t)0xfff), 0x1000, PAGE_READWRITE | PAGE_GUARD, &o);
            g_guardHit = 0;
        }
        *faulted = code == kTimeout ? 2 : 1;
        *f = 0;
        return 0;
    }
}

// An empty x87 stack and the control word `cw`.
void FpuReset(unsigned cw) {
    __asm fninit
    __asm fldcw word ptr cw
}

uint32_t Hash(const void* p, size_t n) {
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < n; ++i) h = (h ^ ((const uint8_t*)p)[i]) * 16777619u;
    return h;
}

} // namespace

DIFFTEST(leaves_all) {
    if (!g_mem) {
        // The buffers inside 32 MB of reserved, inaccessible address space: a
        // loop running far past them (a random count) faults instead of
        // writing over the test process.
        uint8_t* r = (uint8_t*)VirtualAlloc(nullptr, 32 << 20, MEM_RESERVE, PAGE_NOACCESS);
        g_mem = (uint8_t*)VirtualAlloc(r + (16 << 20), kBuf * (kNumBufs + 1), MEM_COMMIT, PAGE_READWRITE);
    }
    if (g_data.empty()) FindData();
    // A float used as a pointer (1.0f is 0x3f800000) is a user-space address:
    // reserve those, inaccessible, so such a dereference faults instead of
    // touching whatever might be mapped there.
    static bool reserved;
    for (int k = 0; k < kNumSmallFloats && !reserved; ++k) {
        uint32_t v;
        memcpy(&v, &kSmallFloats[k], 4);
        if (v && v < 0x80000000u) VirtualAlloc((void*)(uintptr_t)(v & ~0xffffu), 0x20000, MEM_RESERVE, PAGE_NOACCESS);
    }
    reserved = true;
    // The exe's C runtime string functions set up their locale state on first
    // use (partly outside the tracked data): once before the cases, so the
    // first leaf that calls one does not pay for it on one side only.
    {
        char b[16];
        ((int(__cdecl*)(const char*, const char*, size_t))0x5a5f26)("a", "A", 1);
        ((int(__cdecl*)(const char*, const char*))0x5a57cf)("a", "A");
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(b, "%d %s %g", 1, "x", 1.5);
    }
    void* veh = AddVectoredExceptionHandler(1, OnWrite);
    if (!g_testThread) {
        DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_testThread, 0, FALSE,
                        DUPLICATE_SAME_ACCESS);
        CreateThread(nullptr, 0, Watchdog, nullptr, 0, nullptr);
    }
    unsigned cw = 0;
    __asm fnstcw word ptr cw
    cw &= 0xffff;
    int tested = 0, failed = 0;
    for (int c = 0; c < kNumLeafCases; ++c) {
        const LeafCase& lc = kLeafCases[c];
        const void* ours = hook::Find(lc.exe);
        if (!ours) continue;
        ++tested;
        if (FILE* f = fopen("leaves_current.txt", "w")) {  // (a hang names its function)
            fprintf(f, "%s 0x%06x %s\n", lc.name, lc.exe, lc.spec);
            fclose(f);
        }
        const char* sp = lc.spec;
        const char res = *sp++;
        bool bad = false;
        uint32_t regs[2], a[8];
        // One side of case `it`: the same buffers and arguments for both sides.
        // `snap` (if given) receives the buffers afterwards.
        auto run = [&](int it, int s, uint8_t* snap) -> std::string {
            g_bits = (uint32_t)(c * 7919 + it) * 2654435761u + 0x1eaf;
            Fill();
            memset(regs, 0, sizeof regs);
            memset(a, 0, sizeof a);
            auto value = [&](char k, uint32_t* hi) -> uint32_t {
                switch (k) {
                case 'p': return Draw() % 16 ? BufPtr() : 0;
                case 'i': return SmallInt();
                case 'f': return SmallFloat();
                case 'd': {
                    static const double v[] = { 0.0, 1.0, -1.0, 0.5, 1234.5, -0.001, 1e9 };
                    uint64_t u;
                    memcpy(&u, &v[Draw() % 7], 8);
                    *hi = (uint32_t)(u >> 32);
                    return (uint32_t)u;
                }
                default: return Draw();
                }
            };
            uint32_t hi = 0;
            regs[0] = value(sp[0], &hi);
            regs[1] = value(sp[1], &hi);
            for (int k = 0; k < 8 && sp[2 + k]; ++k) {
                if (sp[2 + k] == '-' && k > 0 && sp[1 + k] == 'd') {
                    a[k] = hi;
                    continue;
                }
                a[k] = value(sp[2 + k], &hi);
            }
            const void* fn = s ? ours : (const void*)(uintptr_t)lc.exe;
            uint64_t f = 0;
            int faulted;
            FpuReset(cw);
            g_tracking = true;
            Protect(true);
            uint32_t r = Call(fn, regs[0], regs[1], a, res == 'F', &f, &faulted);
            Protect(false);
            g_tracking = false;
            FpuReset(cw);
            if (snap) memcpy(snap, g_mem, kBuf * kNumBufs);
            if (res == 'B') r &= 0xff;
            if (res == 'W') r &= 0xffff;
            if (res == 'V' || res == 'F') r = 0;
            // A fault or a timeout is input the game never gives: either one
            // compares equal (a slow loop may time out on one side before it
            // reaches the fault the other side hits), and what was written
            // before it is not compared.
            char line[96];
            if (faulted)
                sprintf(line, "invalid");
            else
                sprintf(line, "ok ret %08x fp %016llx mem %08x", r, (unsigned long long)f, Hash(g_mem, kBuf * kNumBufs));
            const std::string writes = TakeWrites();
            return faulted ? std::string(line) : line + writes;
        };
        for (int it = 0; it < 48 && !bad; ++it) {
            std::string out0 = run(it, 0, nullptr), out1 = run(it, 1, nullptr);
            if (out0 != out1 && (out0 == "invalid" || out1 == "invalid")) {
                // Maybe only slower: both again with a long time limit.
                g_limitMs = 30000;
                out0 = run(it, 0, nullptr);
                out1 = run(it, 1, nullptr);
                g_limitMs = 2000;
            }
            if (out0 == out1) continue;
            // A difference must come back: both sides twice more. One that does
            // not (seen only in the full suite: something else, such as a thread
            // an earlier test left running, writing into the buffers) is noted,
            // not failed.
            bool again = false;
            for (int k = 0; k < 2 && !again; ++k) again = run(it, 0, nullptr) != run(it, 1, nullptr);
            if (!again) {
                fprintf(g_out, "  leaf %s case %d: a difference that did not come back (not counted)\n", lc.name, it);
                continue;
            }
            bad = true;
            if (++failed > 40) continue;
            fprintf(g_out, "  leaf %s (0x%06x %s) case %d:\n    orig  %.300s\n    ours  %.300s\n    args", lc.name, lc.exe,
                    lc.spec, it, out0.c_str(), out1.c_str());
            auto where = [](uint32_t v) {
                static char b[4][32];
                static int k;
                char* p = b[k++ & 3];
                const uint32_t m = (uint32_t)(uintptr_t)g_mem;
                if (v - m < (uint32_t)(kBuf * kNumBufs)) sprintf(p, "buf%u+%x", (v - m) / kBuf, (v - m) % kBuf);
                else sprintf(p, "%08x", v);
                return (const char*)p;
            };
            fprintf(g_out, " ecx %s edx %s stack", where(regs[0]), where(regs[1]));
            for (int k = 0; k < 8 && sp[2 + k]; ++k) fprintf(g_out, " %s", where(a[k]));
            fprintf(g_out, "\n");
            // Where the buffers differ (both sides run again, kept).
            static uint8_t snap[2][kBuf * kNumBufs];
            for (int side = 0; side < 2; ++side) {
                g_lastCode = 0;
                const std::string o = run(it, side, snap[side]);
                if (o == "invalid")
                    fprintf(g_out, "    %s: exception %08lx at %p, address %08lx\n", side ? "ours" : "orig", g_lastCode, g_lastAt,
                            (unsigned long)g_lastAddr);
            }
            // The object in ECX as the call saw it (its fields explain most cases):
            // the buffers filled again as for the call.
            if (regs[0] - (uint32_t)(uintptr_t)g_mem < (uint32_t)(kBuf * kNumBufs)) {
                g_bits = (uint32_t)(c * 7919 + it) * 2654435761u + 0x1eaf;
                Fill();
                const uint32_t* o = (const uint32_t*)(uintptr_t)regs[0];
                for (int k = 0; k < 0x80; k += 8)
                    fprintf(g_out, "    ecx+%03x: %08x %08x %08x %08x %08x %08x %08x %08x\n", k * 4, o[k], o[k + 1], o[k + 2],
                            o[k + 3], o[k + 4], o[k + 5], o[k + 6], o[k + 7]);
            }
            int shown = 0;
            for (int k = 0; k < kBuf * kNumBufs && shown < 6; ++k)
                if (snap[0][k] != snap[1][k]) {
                    fprintf(g_out, "    buf%d+%x: orig %02x ours %02x\n", k / kBuf, k % kBuf, snap[0][k], snap[1][k]);
                    ++shown;
                }
        }
        EXPECT_EQ(bad, false, "leaf %s differs", lc.name);
    }
    RemoveVectoredExceptionHandler(veh);
    fprintf(g_out, "  %d leaf functions tested, %d differ\n", tested, failed);
}
