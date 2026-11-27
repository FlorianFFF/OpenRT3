# tools/gen_test.py spec: the last functions no run reaches - dialog layout
# helpers (the dividend dialog's arrows, the scenario setup's type rows, the
# lobby's portrait picker and name field), a lobby worker's deleting
# destructor and device-level renderer / texture / mouse code - each with
# every function it calls faked (python tools/gen_test.py b508 tests/gen_specs/b508_spec.py).
cases = [
    (0x4c4c70, "CompanyDetail_AddArrowButtons", "Vp-ffii"),
    (0x4cbea0, "Setup_AddTypeRow", "Vp-cffff"),
    (0x4ecdd0, "Setup_AddPortraitPicker", "Vp-ffii"),
    (0x4eccd0, "NameDialog_SetName", "Vp-q"),
    (0x4ed7e0, "WorkerA_Delete", "p-c"),
    (0x543ae0, "Mouse_Acquire", "Vp-"),
    (0x5410f0, "Texture_FreePixels", "Vp-"),
    (0x55dd80, "Display_DestroyRenderer", "Vp-"),
    (0x546390, "Render_CreateDevice", "VR-iq"),  # R: a renderer whose D3D object (+0x25db) is a pool object
]
ptrs = [0x6d0818, 0x62be10, 0x62b268, 0x62bae0, 0x6cec74, 0x6d4024, 0x6d401c, 0x6d4020]
nonnull = [0x6cec74, 0x6d4024, 0x6d401c]
ints = [0x6226d0]
# IDirect3D8::CreateDevice (vtable +0x3c) is a COM stdcall method: seven stack
# arguments, ECX not one of them (the pool's vtable fakes are thiscall).
extra_fakes = {"CreateDevice": (28, 7, 0, 2)}
values = {"R": ["(uint32_t)(uintptr_t)(g_genBig + 0x20000)"]}
setup = """
            *(uint32_t*)(g_genBig + 0x20000 + 0x25db) = (uint32_t)(uintptr_t)g_genObj[GenDraw() % 4];
            g_genVtbl[0x3c / 4] = (void*)&GenFakeb508_xCreateDevice;
            strcpy((char*)g_genBuf[2], GenDraw() % 2 ? "Player Name" : "");
"""
