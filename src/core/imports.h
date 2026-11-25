// The exe's imports (Windows, Winsock, DirectX, Miles Sound System, Bink),
// one name per import-table slot. Each name is a reference to the slot, so a
// call goes through the slot at call time exactly like the exe's own code
// (and a test can fake an import by writing its slot). The type is the
// signature the rewrite uses; Windows types are spelled as plain integers and
// pointers so this header does not need windows.h. "type not established"
// marks an import every call site still casts to its own signature.
//
// Listed per DLL in import-table order (names from RT3.exe's import directory;
// Winsock and DirectSound are imported by ordinal).
#pragma once
#include <stddef.h>
#include <stdint.h>
#include "hook/hook.h"

namespace rt3 {
namespace imp {

// ADVAPI32.dll
static auto& RegQueryValueExA = ::hook::Global<int32_t(__stdcall*)(void*, const char*, void*, void*, void*, void*)>(0x5c8000);
static auto& RegCreateKeyExA = ::hook::Global<void*(__stdcall*)()>(0x5c8008);  // type not established
static auto& RegCloseKey = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c800c);
static auto& RegSetValueExA = ::hook::Global<int32_t(__stdcall*)(void*, const char*, uint32_t, uint32_t, const void*, uint32_t)>(0x5c8014);
// GDI32.dll
static auto& DeleteDC = ::hook::Global<int(__stdcall*)(void*)>(0x5c8030);
static auto& GetTextFaceA = ::hook::Global<int(__stdcall*)(void*, int, char*)>(0x5c8034);
static auto& GetTextMetricsA = ::hook::Global<int(__stdcall*)(void*, int32_t*)>(0x5c8038);
static auto& SelectObject = ::hook::Global<void*(__stdcall*)(void*, void*)>(0x5c803c);
static auto& CreateCompatibleDC = ::hook::Global<void*(__stdcall*)(void*)>(0x5c8040);
static auto& DeleteObject = ::hook::Global<int(__stdcall*)(void*)>(0x5c8058);
static auto& CreateFontA = ::hook::Global<void*(__stdcall*)(int, int, int, int, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, const char*)>(0x5c805c);
// KERNEL32.dll
static auto& SleepEx = ::hook::Global<uint32_t(__stdcall*)(uint32_t, int32_t)>(0x5c8064);
static auto& GetTickCount = ::hook::Global<uint32_t(__stdcall*)()>(0x5c8068);
static auto& GetDriveTypeA = ::hook::Global<uint32_t(__stdcall*)(const char*)>(0x5c806c);
static auto& InterlockedIncrement = ::hook::Global<void*(__stdcall*)()>(0x5c8070);  // type not established
static auto& InterlockedDecrement = ::hook::Global<void*(__stdcall*)()>(0x5c8074);  // type not established
static auto& OutputDebugStringA = ::hook::Global<void(__stdcall*)(const char*)>(0x5c8078);
static auto& GetCurrentDirectoryA = ::hook::Global<uint32_t(__stdcall*)(uint32_t, char*)>(0x5c807c);
static auto& GetComputerNameA = ::hook::Global<int32_t(__stdcall*)(char*, uint32_t*)>(0x5c8080);
static auto& SetThreadPriority = ::hook::Global<int32_t(__stdcall*)(void*, int32_t)>(0x5c8084);
static auto& WinExec = ::hook::Global<uint32_t(__stdcall*)(const char*, uint32_t)>(0x5c8088);
static auto& SetCurrentDirectoryA = ::hook::Global<int32_t(__stdcall*)(const char*)>(0x5c80d8);
static auto& FindFirstFileA = ::hook::Global<void*(__stdcall*)()>(0x5c8190);  // type not established
static auto& FindNextFileA = ::hook::Global<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
static auto& FindClose = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c819c);
static auto& CreateThread = ::hook::Global<void*(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t, void*)>(0x5c81c0);
static auto& WaitForSingleObject = ::hook::Global<uint32_t(__stdcall*)(void*, uint32_t)>(0x5c81c4);
static auto& CloseHandle = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c81c8);
static auto& GetCurrentProcess = ::hook::Global<uint32_t(__stdcall*)()>(0x5c81cc);
static auto& GetPriorityClass = ::hook::Global<uint32_t(__stdcall*)(uint32_t)>(0x5c81d0);
static auto& GetVersionExA = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c81d4);
static auto& GlobalMemoryStatus = ::hook::Global<void(__stdcall*)(void*)>(0x5c81d8);
static auto& QueryPerformanceFrequency = ::hook::Global<int32_t(__stdcall*)(int64_t*)>(0x5c81dc);
static auto& QueryPerformanceCounter = ::hook::Global<int32_t(__stdcall*)(int64_t*)>(0x5c81e0);
static auto& WideCharToMultiByte = ::hook::Global<int32_t(__stdcall*)(uint32_t, uint32_t, const wchar_t*, int32_t, char*, int32_t, const char*, int32_t*)>(0x5c81e4);
static auto& MultiByteToWideChar = ::hook::Global<int32_t(__stdcall*)(uint32_t, uint32_t, const char*, int32_t, wchar_t*, int32_t)>(0x5c81e8);
static auto& InitializeCriticalSection = ::hook::Global<void(__stdcall*)(void*)>(0x5c81ec);
static auto& Sleep = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c81f0);
static auto& DeleteCriticalSection = ::hook::Global<void(__stdcall*)(void*)>(0x5c81f4);
static auto& LeaveCriticalSection = ::hook::Global<void(__stdcall*)(void*)>(0x5c81f8);
static auto& EnterCriticalSection = ::hook::Global<void(__stdcall*)(void*)>(0x5c81fc);
static auto& GetLocalTime = ::hook::Global<void(__stdcall*)(void*)>(0x5c8200);
static auto& lstrcpynA = ::hook::Global<char*(__stdcall*)(char*, const void*, int32_t)>(0x5c8204);
static auto& GlobalAlloc = ::hook::Global<void*(__stdcall*)(uint32_t, size_t)>(0x5c8208);
static auto& GlobalLock = ::hook::Global<void*(__stdcall*)(void*)>(0x5c820c);
static auto& lstrcpyA = ::hook::Global<char*(__stdcall*)(void*, const char*)>(0x5c8210);
static auto& GlobalUnlock = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c8214);
static auto& GetUserDefaultLangID = ::hook::Global<uint16_t(__stdcall*)()>(0x5c8218);
// USER32.dll
static auto& MessageBoxA = ::hook::Global<void*(__stdcall*)()>(0x5c8220);  // type not established
static auto& SetClipboardData = ::hook::Global<void*(__stdcall*)(uint32_t, void*)>(0x5c8224);
static auto& DestroyMenu = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c8228);
static auto& SetMenu = ::hook::Global<int32_t(__stdcall*)(void*, void*)>(0x5c822c);
static auto& EnableMenuItem = ::hook::Global<void*(__stdcall*)()>(0x5c8230);  // type not established
static auto& CheckMenuItem = ::hook::Global<void*(__stdcall*)()>(0x5c8234);  // type not established
static auto& GetMenuStringA = ::hook::Global<void*(__stdcall*)()>(0x5c8238);  // type not established
static auto& SetFocus = ::hook::Global<void*(__stdcall*)()>(0x5c823c);  // type not established
static auto& VkKeyScanA = ::hook::Global<int16_t(__stdcall*)(char)>(0x5c8240);
static auto& CharNextExA = ::hook::Global<void*(__stdcall*)()>(0x5c8244);  // type not established
static auto& EnumDisplayDevicesA = ::hook::Global<int32_t(__stdcall*)(const char*, uint32_t, void*, uint32_t)>(0x5c8250);
static auto& CharPrevExA = ::hook::Global<const char*(__stdcall*)(uint16_t, const char*, const char*, uint32_t)>(0x5c8254);
static auto& EmptyClipboard = ::hook::Global<int32_t(__stdcall*)()>(0x5c8258);
static auto& OpenClipboard = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c825c);
static auto& GetClipboardData = ::hook::Global<void*(__stdcall*)(uint32_t)>(0x5c8260);
static auto& SetCursorPos = ::hook::Global<int32_t(__stdcall*)(int32_t, int32_t)>(0x5c8264);
static auto& ScreenToClient = ::hook::Global<void*(__stdcall*)()>(0x5c8268);  // type not established
static auto& ToAscii = ::hook::Global<int32_t(__stdcall*)(uint32_t, uint32_t, const uint8_t*, uint16_t*, uint32_t)>(0x5c826c);
static auto& GetCursorPos = ::hook::Global<int32_t(__stdcall*)(int32_t*)>(0x5c8270);
static auto& GetSystemMetrics = ::hook::Global<int32_t(__stdcall*)(int32_t)>(0x5c8274);
static auto& GetDoubleClickTime = ::hook::Global<uint32_t(__stdcall*)()>(0x5c8278);
static auto& keybd_event = ::hook::Global<void(__stdcall*)(uint8_t, uint8_t, uint32_t, uint32_t)>(0x5c827c);
static auto& GetKeyboardState = ::hook::Global<int32_t(__stdcall*)(uint8_t*)>(0x5c8280);
static auto& GetForegroundWindow = ::hook::Global<void*(__stdcall*)()>(0x5c8284);  // type not established
static auto& PeekMessageA = ::hook::Global<int32_t(__stdcall*)(void*, void*, uint32_t, uint32_t, uint32_t)>(0x5c8288);
static auto& DispatchMessageA = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c828c);
static auto& TranslateMessage = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c8290);
static auto& DefWindowProcA = ::hook::Global<int32_t(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c8294);
static auto& SetCursor = ::hook::Global<void*(__stdcall*)(void*)>(0x5c8298);
static auto& LoadCursorFromFileA = ::hook::Global<void*(__stdcall*)(const char*)>(0x5c829c);
static auto& LoadCursorA = ::hook::Global<void*(__stdcall*)(void*, uint32_t)>(0x5c82a0);
static auto& ShowWindow = ::hook::Global<int32_t(__stdcall*)(void*, int32_t)>(0x5c82a4);
static auto& ReleaseCapture = ::hook::Global<int32_t(__stdcall*)()>(0x5c82a8);
static auto& SetCapture = ::hook::Global<void*(__stdcall*)(void*)>(0x5c82ac);
static auto& GetWindowRect = ::hook::Global<int32_t(__stdcall*)(void*, int32_t*)>(0x5c82b0);
static auto& AdjustWindowRect = ::hook::Global<int32_t(__stdcall*)(int32_t*, uint32_t, int32_t)>(0x5c82b4);
static auto& CreateWindowExA = ::hook::Global<void*(__stdcall*)()>(0x5c82b8);  // type not established
static auto& SetWindowLongA = ::hook::Global<void*(__stdcall*)()>(0x5c82bc);  // type not established
static auto& PostMessageA = ::hook::Global<void*(__stdcall*)()>(0x5c82c0);  // type not established
static auto& DestroyWindow = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c82c4);
static auto& UnregisterClassA = ::hook::Global<void*(__stdcall*)()>(0x5c82c8);  // type not established
static auto& LoadIconA = ::hook::Global<void*(__stdcall*)()>(0x5c82cc);  // type not established
static auto& ShowCursor = ::hook::Global<int32_t(__stdcall*)(int32_t)>(0x5c82d4);
static auto& CloseClipboard = ::hook::Global<int32_t(__stdcall*)()>(0x5c82d8);
// WS2_32.dll
static auto& bind = ::hook::Global<int(__stdcall*)(int32_t, const void*, int)>(0x5c82f0);
static auto& gethostbyname = ::hook::Global<uint8_t*(__stdcall*)(const char*)>(0x5c82f4);
static auto& gethostname = ::hook::Global<int32_t(__stdcall*)(char*, int32_t)>(0x5c82f8);
static auto& closesocket = ::hook::Global<int32_t(__stdcall*)(int32_t)>(0x5c82fc);
static auto& sendto = ::hook::Global<int32_t(__stdcall*)(int32_t, const char*, int32_t, int32_t, const void*, int32_t)>(0x5c8300);
static auto& htons = ::hook::Global<uint16_t(__stdcall*)(uint16_t)>(0x5c8304);
static auto& htonl = ::hook::Global<uint32_t(__stdcall*)(uint32_t)>(0x5c8308);
static auto& setsockopt = ::hook::Global<int32_t(__stdcall*)(int32_t, int32_t, int32_t, const void*, int32_t)>(0x5c830c);
static auto& socket = ::hook::Global<int32_t(__stdcall*)(int32_t, int32_t, int32_t)>(0x5c8310);
static auto& WSACleanup = ::hook::Global<int32_t(__stdcall*)()>(0x5c8314);
static auto& WSAStartup = ::hook::Global<int32_t(__stdcall*)(uint32_t, void*)>(0x5c8318);
static auto& recvfrom = ::hook::Global<int(__stdcall*)(int32_t, char*, int, int, void*, int32_t*)>(0x5c831c);
static auto& inet_ntoa = ::hook::Global<const char*(__stdcall*)(uint32_t)>(0x5c8320);
static auto& WSAGetLastError = ::hook::Global<int(__stdcall*)()>(0x5c8324);
// binkw32.dll
static auto& BinkSetVolume = ::hook::Global<int32_t(__stdcall*)(void*, uint32_t, int32_t)>(0x5c8358);
// Passed (not called) to BinkSetSoundSystem: Bink's Miles sound-system opener.
static auto& BinkOpenMiles = ::hook::Global<void*(__stdcall*)(void*)>(0x5c835c);
static auto& BinkSetSoundSystem = ::hook::Global<int32_t(__stdcall*)(void*, uint32_t)>(0x5c8360);
static auto& BinkSetIOSize = ::hook::Global<int32_t(__stdcall*)(uint32_t)>(0x5c8364);
static auto& BinkOpen = ::hook::Global<uint32_t*(__stdcall*)(const char*, uint32_t)>(0x5c8368);
static auto& BinkGoto = ::hook::Global<void(__stdcall*)(uint8_t*, uint32_t, uint32_t)>(0x5c836c);
static auto& BinkDoFrame = ::hook::Global<int32_t(__stdcall*)(uint8_t*)>(0x5c8370);
static auto& BinkNextFrame = ::hook::Global<int32_t(__stdcall*)(uint8_t*)>(0x5c8374);
static auto& BinkWait = ::hook::Global<int32_t(__stdcall*)(uint8_t*)>(0x5c8378);
static auto& BinkGetKeyFrame = ::hook::Global<uint32_t(__stdcall*)(uint8_t*, uint32_t, uint32_t)>(0x5c837c);
static auto& BinkClose = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8380);
static auto& BinkCopyToBuffer = ::hook::Global<int32_t(__stdcall*)(void*, void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c8384);
static auto& BinkGetError = ::hook::Global<const char*(__stdcall*)()>(0x5c8388);
// comdlg32.dll
static auto& GetOpenFileNameA = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c8390);
// d3d8.dll
static auto& Direct3DCreate8 = ::hook::Global<void*(__stdcall*)(uint32_t)>(0x5c8398);
// mss32.dll
static auto& AIL_start_timer = ::hook::Global<void(__stdcall*)(int32_t)>(0x5c83a0);
static auto& AIL_register_3D_EOS_callback = ::hook::Global<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a4);
static auto& AIL_register_EOS_callback = ::hook::Global<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8);
static auto& AIL_shutdown = ::hook::Global<void(__stdcall*)()>(0x5c83ac);
static auto& AIL_init_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c83b0);
static auto& AIL_startup = ::hook::Global<int32_t(__stdcall*)()>(0x5c83b4);
static auto& AIL_stop_timer = ::hook::Global<void(__stdcall*)(int32_t)>(0x5c83b8);
static auto& AIL_close_stream = ::hook::Global<void(__stdcall*)(void*)>(0x5c83bc);
static auto& AIL_stream_status = ::hook::Global<int32_t(__stdcall*)(void*)>(0x5c83c0);
static auto& AIL_register_stream_callback = ::hook::Global<void(__stdcall*)(void*, void*)>(0x5c83c4);
static auto& AIL_set_timer_frequency = ::hook::Global<void(__stdcall*)(int32_t, uint32_t)>(0x5c83c8);
static auto& AIL_start_stream = ::hook::Global<void(__stdcall*)(void*)>(0x5c83cc);
static auto& AIL_open_stream = ::hook::Global<void*(__stdcall*)(void*, const char*, uint32_t)>(0x5c83d0);
static auto& AIL_WAV_info = ::hook::Global<int32_t(__stdcall*)(const void*, void*)>(0x5c83d4);
static auto& AIL_set_3D_distance_factor = ::hook::Global<void(__stdcall*)(uint32_t, uint32_t)>(0x5c83d8);
static auto& AIL_3D_room_type = ::hook::Global<int32_t(__stdcall*)(uint32_t)>(0x5c83dc);
static auto& AIL_3D_speaker_type = ::hook::Global<int32_t(__stdcall*)(uint32_t)>(0x5c83e0);
static auto& AIL_unlock = ::hook::Global<void(__stdcall*)()>(0x5c83e4);
static auto& AIL_open_3D_provider = ::hook::Global<int32_t(__stdcall*)(uint32_t)>(0x5c83e8);
static auto& AIL_set_3D_speaker_type = ::hook::Global<void(__stdcall*)(uint32_t, int32_t)>(0x5c83ec);
static auto& AIL_set_3D_position = ::hook::Global<void(__stdcall*)(uint32_t, float, float, float)>(0x5c83f0);
static auto& AIL_register_timer = ::hook::Global<int32_t(__stdcall*)(uint32_t)>(0x5c83f4);
static auto& AIL_set_3D_orientation = ::hook::Global<void(__stdcall*)(uint32_t, float, float, float, float, float, float)>(0x5c83f8);
static auto& AIL_set_stream_volume_levels = ::hook::Global<void(__stdcall*)(void*, float, float)>(0x5c83fc);
static auto& AIL_set_timer_user = ::hook::Global<void(__stdcall*)(int32_t, uint32_t)>(0x5c8400);
static auto& AIL_close_3D_provider = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8404);
static auto& AIL_allocate_3D_sample_handle = ::hook::Global<uint32_t(__stdcall*)(uint32_t)>(0x5c8408);
static auto& AIL_resume_3D_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c840c);
static auto& AIL_stop_3D_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8410);
static auto& AIL_end_3D_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8414);
static auto& AIL_close_3D_listener = ::hook::Global<void(__stdcall*)(void*)>(0x5c8418);
static auto& AIL_release_3D_sample_handle = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c841c);
static auto& AIL_set_3D_sample_playback_rate = ::hook::Global<void(__stdcall*)(uint32_t, int32_t)>(0x5c8420);
static auto& AIL_set_3D_sample_effects_level = ::hook::Global<void(__stdcall*)(uint32_t, float)>(0x5c8424);
static auto& AIL_set_3D_sample_distances = ::hook::Global<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8428);
static auto& AIL_set_3D_sample_volume = ::hook::Global<void(__stdcall*)(uint32_t, float)>(0x5c842c);
static auto& AIL_start_3D_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8430);
static auto& AIL_set_3D_sample_occlusion = ::hook::Global<void(__stdcall*)(uint32_t, uint32_t)>(0x5c8434);
static auto& AIL_set_3D_sample_loop_count = ::hook::Global<void(__stdcall*)(uint32_t, int32_t)>(0x5c8438);
static auto& AIL_set_3D_sample_file = ::hook::Global<void(__stdcall*)(uint32_t, void*)>(0x5c843c);
static auto& AIL_open_digital_driver = ::hook::Global<void*(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c8440);
static auto& AIL_set_preference = ::hook::Global<int32_t(__stdcall*)(uint32_t, int32_t)>(0x5c8444);
static auto& AIL_allocate_sample_handle = ::hook::Global<uint32_t(__stdcall*)(uint32_t)>(0x5c8448);
static auto& AIL_set_sample_playback_rate = ::hook::Global<void(__stdcall*)(uint32_t, int32_t)>(0x5c844c);
static auto& AIL_release_timer_handle = ::hook::Global<void(__stdcall*)(int32_t)>(0x5c8450);
static auto& AIL_enumerate_3D_providers = ::hook::Global<int32_t(__stdcall*)(uint32_t*, void*, uint32_t*)>(0x5c8454);
static auto& AIL_set_sample_volume_pan = ::hook::Global<void(__stdcall*)(uint32_t, float, float)>(0x5c8458);
static auto& AIL_set_sample_loop_count = ::hook::Global<void(__stdcall*)(uint32_t, int32_t)>(0x5c845c);
static auto& AIL_open_3D_listener = ::hook::Global<uint32_t(__stdcall*)(uint32_t)>(0x5c8460);
static auto& AIL_pause_stream = ::hook::Global<void(__stdcall*)(void*, int32_t)>(0x5c8464);
static auto& AIL_resume_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8468);
static auto& AIL_stop_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c846c);
static auto& AIL_set_sample_volume_levels = ::hook::Global<void(__stdcall*)(uint32_t, float, float)>(0x5c8470);
static auto& AIL_end_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8474);
static auto& AIL_start_sample = ::hook::Global<void(__stdcall*)(uint32_t)>(0x5c8478);
static auto& AIL_set_redist_directory = ::hook::Global<void(__stdcall*)(const char*)>(0x5c847c);
static auto& AIL_set_sample_file = ::hook::Global<void(__stdcall*)(uint32_t, void*, int32_t)>(0x5c8480);
static auto& AIL_lock = ::hook::Global<void(__stdcall*)()>(0x5c8484);
// ole32.dll
static auto& CoCreateInstance = ::hook::Global<void*(__stdcall*)()>(0x5c848c);  // type not established
static auto& CoUninitialize = ::hook::Global<void*(__stdcall*)()>(0x5c8490);  // type not established
static auto& CoInitializeEx = ::hook::Global<int32_t(__stdcall*)(void*, uint32_t)>(0x5c8494);

} // namespace imp
} // namespace rt3
