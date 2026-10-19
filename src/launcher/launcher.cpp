// Starts RT3_mp.exe suspended, loads rt3re.dll into it, then lets it run.
// Usage (from the game folder):
//   rt3re.exe [game args...]          play with the reimplemented functions
//   rt3re.exe --test [--filter=name]  run the differential tests (rt3re_test.dll),
//                                     print difftest.log, exit with failure count
#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    bool test = argc > 1 && strcmp(argv[1], "--test") == 0;
    char dll[MAX_PATH];
    GetFullPathNameA(test ? "rt3re_test.dll" : "rt3re.dll", MAX_PATH, dll, nullptr);

    char cmd[2048] = "RT3_mp.exe";
    for (int i = test ? 2 : 1; i < argc; ++i) { strcat(cmd, " "); strcat(cmd, argv[i]); }

    STARTUPINFOA si = { sizeof si };
    PROCESS_INFORMATION pi;
    if (!CreateProcessA(nullptr, cmd, nullptr, nullptr, FALSE, CREATE_SUSPENDED,
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
