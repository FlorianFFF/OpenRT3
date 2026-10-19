#include <windows.h>
#include <stdio.h>
#include "hook/hook.h"
#include "core/log.h"

// Logs the first few hardware exceptions (first chance) with module + offset,
// so a crash in a rewritten function can be traced to it.
static LONG CALLBACK LogException(EXCEPTION_POINTERS* ep) {
    static int logged;
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    if (code != EXCEPTION_ACCESS_VIOLATION && code != EXCEPTION_ILLEGAL_INSTRUCTION &&
        code != EXCEPTION_INT_DIVIDE_BY_ZERO && code != EXCEPTION_STACK_OVERFLOW)
        return EXCEPTION_CONTINUE_SEARCH;
    if (logged++ >= 8) return EXCEPTION_CONTINUE_SEARCH;
    void* at = ep->ExceptionRecord->ExceptionAddress;
    HMODULE mod = nullptr;
    char name[MAX_PATH] = "?";
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (const char*)at, &mod);
    if (mod) GetModuleFileNameA(mod, name, MAX_PATH);
    const char* base = strrchr(name, '\\') ? strrchr(name, '\\') + 1 : name;
    DWORD* esp = (DWORD*)ep->ContextRecord->Esp;
    rt3::Log("exception %08lx at %p (%s+%#x) addr %p esp[0..3] %08lx %08lx %08lx %08lx", code, at, base,
             (unsigned)((char*)at - (char*)mod), (void*)ep->ExceptionRecord->ExceptionInformation[1],
             esp[0], esp[1], esp[2], esp[3]);
    return EXCEPTION_CONTINUE_SEARCH;
}

namespace harness { void Install(); }

static void Startup(HMODULE self) {
    AddVectoredExceptionHandler(1, LogException);
    char ini[MAX_PATH];
    GetModuleFileNameA(self, ini, MAX_PATH);
    char* slash = strrchr(ini, '\\');
    strcpy(slash ? slash + 1 : ini, "rt3re.ini");
    rt3::LogOpen("rt3re.log");
    int n = hook::InstallAll(ini);
    rt3::Log("rt3re: %d functions replaced", n);
    harness::Install();
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(inst);
        Startup(inst);
    }
    return TRUE;
}
