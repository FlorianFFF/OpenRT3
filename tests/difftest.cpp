// Differential tests: run original functions and our reimplementations side by
// side on the same inputs, inside the real RT3_mp.exe process.
//
// Built into rt3re_test.dll. Its DllMain replaces the game's WinMain (0x484440)
// with RunTests, so the exe's own CRT is fully started but no game code has
// run. Results go to difftest.log; the exit code is the failure count.
// Start with:  rt3re.exe --test [filter]   (from the game folder)
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "difftest.h"
#include "hook/hook.h"

int g_fail, g_pass;
FILE* g_out;

static TestCase* g_tests;
TestReg::TestReg(const char* name, void (*fn)()) {
    static TestCase pool[4096];
    static int n;
    TestCase* t = &pool[n++];
    t->name = name; t->fn = fn; t->next = g_tests; g_tests = t;
}

static const char* g_current;

static LONG WINAPI OnCrash(EXCEPTION_POINTERS* ep) {
    void* at = ep->ExceptionRecord->ExceptionAddress;
    HMODULE mod = nullptr;
    char name[MAX_PATH] = "?";
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (const char*)at, &mod);
    if (mod) GetModuleFileNameA(mod, name, MAX_PATH);
    fprintf(g_out, "%-40s CRASH code %08lx at %p (%s+%#x) access %p\n", g_current,
            ep->ExceptionRecord->ExceptionCode, at, strrchr(name, '\\') ? strrchr(name, '\\') + 1 : name,
            (unsigned)((char*)at - (char*)mod), (void*)ep->ExceptionRecord->ExceptionInformation[1]);
    fclose(g_out);
    ExitProcess(1000);
}

// A CRT invalid-parameter error would otherwise end the process with a silent
// fast fail (0xc0000409); name the test instead.
static void OnInvalidParameter(const wchar_t*, const wchar_t*, const wchar_t*, unsigned, uintptr_t) {
    fprintf(g_out, "%-40s CRT INVALID PARAMETER\n", g_current);
    fclose(g_out);
    ExitProcess(1001);
}

// The filter is a comma-separated list of name substrings.
static bool Wanted(const char* name, const char* want) {
    char buf[2048];
    strcpy(buf, want);
    for (char* tok = strtok(buf, ","); tok; tok = strtok(nullptr, ","))
        if (strstr(name, tok)) return true;
    return false;
}

static int __stdcall RunTests(HINSTANCE, HINSTANCE, char* cmdLine, int) {
    g_out = fopen("difftest.log", "w");
    SetUnhandledExceptionFilter(OnCrash);
    _set_invalid_parameter_handler(OnInvalidParameter);
    const char* filter = strstr(cmdLine, "--filter=");
    char want[2048] = "";
    if (filter) sscanf(filter + 9, "%2047s", want);
    for (TestCase* t = g_tests; t; t = t->next) {
        if (want[0] && !Wanted(t->name, want)) continue;
        if (!want[0] && strncmp(t->name, "dump_", 5) == 0) continue; // extraction runs only on request
        int before = g_fail;
        g_current = t->name;
        t->fn();
        fprintf(g_out, "%-40s %s\n", t->name, g_fail == before ? "ok" : "FAIL");
        fflush(g_out);
        if (g_fail == before) ++g_pass;
    }
    fprintf(g_out, "\n%d passed, %d failed\n", g_pass, g_fail);
    fclose(g_out);
    ExitProcess(g_fail);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DWORD old;
        uint8_t* p = (uint8_t*)0x484440;
        VirtualProtect(p, 5, PAGE_EXECUTE_READWRITE, &old);
        p[0] = 0xE9;
        *(int32_t*)(p + 1) = int32_t(uintptr_t(&RunTests) - (0x484440 + 5));
        VirtualProtect(p, 5, old, &old);
    }
    return TRUE;
}
