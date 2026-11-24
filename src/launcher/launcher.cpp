// Starts RT3_mp.exe suspended, loads rt3re.dll into it, then lets it run.
// Usage (from the game folder):
//   rt3re.exe [game args...]          play with the reimplemented functions
//   rt3re.exe --dev-menu             restore the original menu, windowed (-s2)
//   rt3re.exe --test [--filter=name]  run the differential tests (rt3re_test.dll),
//                                     print difftest.log, exit with failure count
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <string>

// Quote one argument using the Windows CRT command-line rules. This also
// preserves spaces/quotes in arguments forwarded to the game or test runner.
static std::string QuoteArg(const char* arg) {
    std::string out = "\"";
    size_t slashes = 0;
    for (const char* p = arg; *p; ++p) {
        if (*p == '\\') { ++slashes; continue; }
        if (*p == '"') out.append(slashes * 2 + 1, '\\');
        else out.append(slashes, '\\');
        slashes = 0;
        out += *p;
    }
    out.append(slashes * 2, '\\');
    return out + '"';
}

int main(int argc, char** argv) {
    bool test = argc > 1 && strcmp(argv[1], "--test") == 0;
    bool devMenu = false;
    for (int i = test ? 2 : 1; i < argc; ++i)
        if (strcmp(argv[i], "--dev-menu") == 0) devMenu = true;
    if (test && devMenu) {
        fprintf(stderr, "--dev-menu is for playing; do not combine it with --test\n");
        return 2;
    }
    char dll[MAX_PATH];
    GetFullPathNameA(test ? "rt3re_test.dll" : "rt3re.dll", MAX_PATH, dll, nullptr);

    std::string cmd = "RT3_mp.exe";
    for (int i = test ? 2 : 1; i < argc; ++i) {
        if (strcmp(argv[i], "--dev-menu") == 0) continue;
        cmd += " " + QuoteArg(argv[i]);
    }
    if (devMenu) {
        SetEnvironmentVariableA("RT3RE_DEV_MENU", "1");
        // The game's existing windowed safe-mode preset retains sound. It
        // must be last so fullscreen launch arguments cannot hide the menu.
        cmd += " -s2";
    }

    STARTUPINFOA si = { sizeof si };
    PROCESS_INFORMATION pi;
    if (!CreateProcessA(nullptr, &cmd[0], nullptr, nullptr, FALSE, CREATE_SUSPENDED,
                        nullptr, nullptr, &si, &pi)) {
        fprintf(stderr, "cannot start RT3_mp.exe (%lu)\n", GetLastError());
        return 1;
    }
    size_t len = strlen(dll) + 1;
    void* remote = VirtualAllocEx(pi.hProcess, nullptr, len, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(pi.hProcess, remote, dll, len, nullptr);
    auto loadLib = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32"), "LoadLibraryA");
    HANDLE t = CreateRemoteThread(pi.hProcess, nullptr, 0, loadLib, remote, 0, nullptr);
    WaitForSingleObject(t, INFINITE);
    DWORD mod = 0;
    GetExitCodeThread(t, &mod);
    CloseHandle(t);
    if (!mod) {
        fprintf(stderr, "%s failed to load\n", dll);
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }
    ResumeThread(pi.hThread);
    DWORD code = 0;
    if (test) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &code);
        FILE* f = fopen("difftest.log", "r");
        char line[512];
        while (f && fgets(line, sizeof line, f)) fputs(line, stdout);
        if (f) fclose(f);
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return (int)code;
}
