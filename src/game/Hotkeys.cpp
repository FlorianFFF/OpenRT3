// Hotkeys.cpp - RT3.exe 0x45e000-0x462000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/imports.h"
#include "game/calendar.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "hook/keepregs.h"
#include "core/containers.h"

namespace rt3 {

// Rewritten functions called directly.
const char* __fastcall LocalizeName(const char* name);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

  // industries/stations/houses

int32_t Global6d07b4Set2();
void __fastcall SceneObj_SetUvTransform(uint8_t* o, int, float u, float v, float su, float sv);
uint8_t* __fastcall NamedObj_Construct(uint8_t* o);
void __fastcall NamedObj_Destructor(uint8_t* o);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);

// A collection of scene objects loaded from a save between the frame tags
// `t0` / `t1` / `t2`: the records get the vtable of a freshly constructed
// object of their class, then load their own extra data (vtbl +0x40).
template <typename Ctor, typename Dtor>
static int32_t LoadSceneObjects(Collection* t, Stream s, int32_t recordSize, int32_t t0, int32_t t1, int32_t t2, Ctor ctor, Dtor dtor) {
    int32_t n = StreamSkip4(s, t0, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, t1, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        uint8_t tmp[0x400];
        ctor(tmp);
        F<uint32_t>(r, 0) = F<uint32_t>(tmp, 0);
        n += ((int32_t(__thiscall*)(uint8_t*, Stream))(*(void***)tmp)[0x40 / 4])(r, s);
        dtor(tmp);
    }
    return StreamSkip4(s, t2, 0, 0) + n;
}

struct Collection;
uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall Marker_Init(uint8_t* o, int, uint32_t id, uint32_t x, uint32_t y, const char* label, int32_t bodied);
int32_t __stdcall Map_FogEnd(int32_t x, int32_t y);
void __fastcall StripExtension(char* s);
int __fastcall ToUpper(char c);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
char* __fastcall StrCopy(char* dst, const char* src);

uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data);

int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
uint8_t* __fastcall Emitters_NextFile(const char* prefix, int32_t restart);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall String_Assign(char** s, const char* src);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
int32_t __fastcall StreamReadString(int32_t s, char** out);
void __fastcall FreeAndNull2(void** p);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
void __fastcall SceneObj_Register(uint8_t* o);
int32_t __fastcall NamedObj_Reset(uint8_t* o);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t unused1, int32_t unused2);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t unused, int32_t count);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall Clear_d939a0(void*, int, int32_t, int32_t);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void __fastcall Obj_DropToGround(uint8_t* o, int, int32_t lift);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
int32_t IsMultiplayer();
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
int32_t __fastcall World_LookupNamed2(void* w, int, const char* name);
void __fastcall World_UpdateSpeedScale(uint8_t* o);
void __fastcall Table_RefreshNamedFlags(Collection* t);
uint32_t __fastcall LookupSingleFlag(int32_t key, int32_t mask);
uint32_t __fastcall Models_Move(uint32_t key, uint32_t sub);

// The default key bindings registered by 0x45f9e0, in order: modifiers
// (ECX / EDX), key, sub-key, kind, handler, name string, then the per-
// language alternatives (key, sub) for modes 2, 1, 3 and 4.
struct HotkeyDefault {
    int32_t a, b;
    uint32_t key, sub, kind, handler, name;
    uint32_t alt[8];
};

static const HotkeyDefault kHotkeyDefaults[] = {
    { 0, 0, 0x70, 0, 1, 0x441900, 0xd48, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x71, 0, 1, 0x4419c0, 0xd49, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x72, 0, 1, 0x441a00, 0xd4a, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x73, 0, 1, 0x441940, 0xd4b, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x74, 0, 1, 0x441980, 0xd4c, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x75, 0, 1, 0x482b00, 0xd4d, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x76, 0, 1, 0x4408b0, 0xd4e, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x77, 0, 1, 0x441ac0, 0xd4f, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x76, 1, 1, 0x4408d0, 0xd50, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x77, 1, 1, 0x441af0, 0xd51, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x78, 0, 1, 0x483680, 0xd52, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x7a, 0, 1, 0x4839a0, 0xd53, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x7b, 0, 1, 0x483990, 0xd54, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x91, 0, 1, 0x440c40, 0xd55, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xc0, 0, 1, 0x4828b0, 0xd56, { 0xff, 0x0, 0x0, 0x0, 0xdc, 0x0, 0xff, 0x0 } },
    { 0, 0, 0x20, 0, 1, 0x441690, 0xd57, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x2c, 0, 2, 0x482b20, 0xd58, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x2c, 4, 2, 0x482bb0, 0xd59, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x4d, 0, 1, 0x441400, 0xd5a, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x4d, 1, 1, 0x441460, 0xd5b, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x4e, 0, 1, 0x441490, 0xd5c, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x44, 0, 1, 0x4414d0, 0xd5d, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x46, 0, 1, 0x441500, 0xd5e, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x43, 0, 1, 0x4405a0, 0xd5f, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x43, 1, 1, 0x4405d0, 0xd60, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x54, 0, 1, 0x440560, 0xd61, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x54, 1, 1, 0x441040, 0xd62, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x53, 0, 1, 0x440580, 0xd63, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x53, 1, 1, 0x4410d0, 0xd64, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x22, 0, 1, 0x440e00, 0xd65, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x21, 0, 1, 0x440f20, 0xd66, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x4c, 4, 1, 0x440660, 0xd67, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x54, 4, 1, 0x4406c0, 0xd68, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x53, 4, 1, 0x440690, 0xd69, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0xdb, 0, 1, 0x440c80, 0xd6a, { 0x24, 0x0, 0x24, 0x0, 0x24, 0x0, 0x24, 0x0 } },
    { 0, 0, 0xdd, 0, 1, 0x440d40, 0xd6b, { 0x23, 0x0, 0x23, 0x0, 0x23, 0x0, 0x23, 0x0 } },
    { 0, 0, 0x4b, 0, 1, 0x440530, 0xd6c, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0xbb, 0, 1, 0x441a40, 0xd6d, { 0xbb, 0x1, 0xdd, 0x0, 0xdd, 0x0, 0xdd, 0x0 } },
    { 0, 0, 0xbd, 0, 1, 0x441a80, 0xd6e, { 0xbb, 0x0, 0xbf, 0x0, 0xbf, 0x0, 0xbf, 0x0 } },
    { 0, 0, 0x13, 0, 1, 0x440880, 0xd6f, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x9, 0, 1, 0x441260, 0xd70, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x9, 1, 1, 0x4412a0, 0xd71, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x48, 0, 1, 0x441b20, 0xd72, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x47, 0, 1, 0x441160, 0xd73, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x47, 1, 1, 0x4411a0, 0xf01, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x4e, 1, 1, 0x441240, 0xd74, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x45, 1, 1, 0x441b50, 0xd75, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x57, 0, 1, 0x441870, 0xd76, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x43, 4, 1, 0x441810, 0xd77, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x42, 4, 1, 0x4418a0, 0xd78, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x48, 1, 1, 0x4411e0, 0xd79, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0xbf, 0, 1, 0x42a780, 0xd7a, { 0x0, 0x0, 0xdc, 0x0, 0xff, 0x0, 0xff, 0x0 } },
    { 0, 0, 0x5a, 4, 1, 0x441200, 0xd7b, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0xbe, 0, 1, 0x441340, 0xd7c, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x4e, 4, 1, 0x468b00, 0xd7d, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x4c, 0, 1, 0x440700, 0xd7e, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x49, 0, 1, 0x440720, 0xd7f, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x42, 0, 1, 0x440740, 0xd80, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x440760, 0xd81, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x440780, 0xd82, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x4407a0, 0xd83, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x4407c0, 0xd84, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x4407e0, 0xd85, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x440800, 0xd86, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x440820, 0xd87, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0xff, 0, 1, 0x440840, 0xd88, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x41, 0, 1, 0x440860, 0xd89, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x26, 0, 1, 0x43d2a0, 0xd8a, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x28, 0, 1, 0x43d2b0, 0xd8b, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x25, 0, 1, 0x43d2c0, 0xd8c, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x27, 0, 1, 0x43d2d0, 0xd8d, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x26, 1, 1, 0x43d2e0, 0xd8e, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x28, 1, 1, 0x43d2f0, 0xd8f, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x25, 1, 1, 0x43d300, 0xd90, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 1, 0x27, 1, 1, 0x43d310, 0xd91, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x31, 0, 1, 0x440ba0, 0xd92, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x31, 1, 1, 0x440b00, 0xd93, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x32, 0, 1, 0x440bb0, 0xd94, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x32, 1, 1, 0x440b10, 0xd95, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x33, 0, 1, 0x440bc0, 0xd96, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x33, 1, 1, 0x440b20, 0xd97, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x34, 0, 1, 0x440bd0, 0xd98, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x34, 1, 1, 0x440b30, 0xd99, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x35, 0, 1, 0x440be0, 0xd9a, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x35, 1, 1, 0x440b40, 0xd9b, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x36, 0, 1, 0x440bf0, 0xd9c, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x36, 1, 1, 0x440b50, 0xd9d, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x37, 0, 1, 0x440c00, 0xd9e, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x37, 1, 1, 0x440b60, 0xd9f, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x38, 0, 1, 0x440c10, 0xda0, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x38, 1, 1, 0x440b70, 0xda1, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x39, 0, 1, 0x440c20, 0xda2, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x39, 1, 1, 0x440b80, 0xda3, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x30, 0, 1, 0x440c30, 0xda4, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 0, 0, 0x30, 1, 1, 0x440b90, 0xda5, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x31, 4, 1, 0x42a660, 0xda6, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x32, 4, 1, 0x42a6c0, 0xda7, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x33, 4, 1, 0x42a720, 0xda8, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x34, 4, 1, 0x482c30, 0xda9, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x35, 4, 1, 0x482c50, 0xdaa, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x36, 4, 1, 0x482c70, 0xdab, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x71, 1, 1, 0x482a20, 0xdac, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x72, 1, 1, 0x482a10, 0xdad, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x71, 4, 1, 0x482af0, 0xdae, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x72, 4, 1, 0x482ae0, 0xdaf, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x6f, 0, 1, 0x42a7c0, 0xdb0, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x57, 1, 1, 0x4412e0, 0xdb1, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x4c, 1, 1, 0x42a810, 0xdb2, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x47, 4, 1, 0x441360, 0xdb3, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x50, 4, 1, 0x4413b0, 0xdb4, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x6b, 0, 1, 0x441cb0, 0xdb5, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x6d, 0, 1, 0x441d20, 0xdb6, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    { 1, 0, 0x6a, 0, 1, 0x4413e0, 0xdb7, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
};

void Models_Shutdown();
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void Hotkeys_RegisterDefaults();
int32_t __fastcall Hotkeys_LoadFile(const char* path);
void __fastcall SceneObj_Teardown(uint8_t* o);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Car_SetCrashed(uint8_t* c, int, int32_t on);
void __fastcall Car_SetBrokenDown(uint8_t* c, int, int32_t on);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);
void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall World_UpdateSpeedScale(uint8_t* o);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Hotkeys_Init(int32_t load);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall WorldModel_Load(uint8_t* o, int, int32_t s);
void __fastcall NamedObj_Save(uint8_t* o, int, Stream s);
uint8_t* __fastcall Holder_Init(uint8_t* h, int, uint32_t a, uint32_t b);
void __fastcall Set_6257e0(uint32_t a, uint32_t b);
int32_t __fastcall Config_Load(uint8_t* c, int, const char* path);
void __fastcall Str_TrimLeft(char* s);
void __fastcall Str_TrimRight(char* s);
void __fastcall Holder_Destruct(uint8_t* h);
void __cdecl Text_SplitPlusImpl(char* dst, char* src, char* dst2);
int32_t __cdecl NamedValue_FindImpl(const char* name);
int32_t __cdecl TeamName_KindImpl(char* name);
int32_t __cdecl Models_AddCopyImpl(int32_t slot, int32_t key, const char* name);
void __fastcall PtrList_Destroy(PtrList* l);
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);
int32_t __fastcall Camera_FogAlpha(uint8_t* c, int, int32_t x, int32_t y);
int32_t __fastcall HasString(int32_t id);
struct HashMap;
struct MaxHeap;
int32_t __fastcall Heap_PopMax(MaxHeap* h);
struct HeapNode;
HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value);
void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
void __fastcall Heap_Destroy2(MaxHeap* h);
struct NodePool;
void __fastcall Pool_Free(NodePool* p);
void __fastcall Hash_DestroyThunk(HashMap* m);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y, int32_t freeStanding);
RT3_NOINLINE void __fastcall SceneObj_Teardown(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_ShowPreview(uint8_t* o);
RT3_NOINLINE void __fastcall Car_LoadModel(uint8_t* c, int, const char* name, const char* tex, float x, float z, int32_t reset);
RT3_NOINLINE void __fastcall Vehicle_Teardown(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Car_Load(uint8_t* c, int, int32_t s, char* texSet, const char* name, uint8_t* type);
void __fastcall Model_ShowOnlyPart(uint8_t* o, int, int32_t sel);
RT3_NOINLINE uint32_t __fastcall Effects_SpawnForModel(uint8_t* parent, int, const char* prefix, uint8_t** out, int32_t count, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __stdcall VectorConstruct(uint8_t* base, int32_t stride, int32_t count, void(__thiscall* fn)(void*));
void Record_DisplayName();
RT3_NOINLINE void __cdecl Text_SplitPlusImpl(char* dst, char* src, char* dst2);
void Text_SplitPlus();
RT3_NOINLINE int32_t __cdecl NamedValue_FindImpl(const char* name);
void NamedValue_Find();
RT3_NOINLINE int32_t __cdecl TeamName_KindImpl(char* name);
void TeamName_Kind();
RT3_NOINLINE uint32_t __fastcall RepackFlags(uint32_t f);
RT3_NOINLINE uint32_t __fastcall LookupSingleFlag(int32_t key, int32_t mask);
void Models_FindByName();
void Models_Unlink();
RT3_NOINLINE int32_t __cdecl Models_AddCopyImpl(int32_t slot, int32_t key, const char* name);
void Models_AddCopy();
int32_t __fastcall Records_Dispatch(uint8_t* r);
RT3_NOINLINE uint8_t* Models_NextInOrder();
int32_t __fastcall Buckets_Find(int32_t key, int32_t* value, uint32_t* flags);
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out);
RT3_NOINLINE int32_t __fastcall Models_ValueByName(const char* name);
RT3_NOINLINE int32_t __fastcall Hotkeys_LoadFile(const char* path);
RT3_NOINLINE int32_t __fastcall Hotkeys_Save(const char* path);
RT3_NOINLINE uint32_t __fastcall Models_Add(int32_t a, int32_t b, uint32_t key, uint32_t sub, uint32_t v, int32_t value, const char* name, int32_t id, uint32_t key2, uint32_t sub2, uint32_t key1, uint32_t sub1, uint32_t key3, uint32_t sub3, uint32_t key4, uint32_t sub4);
RT3_NOINLINE uint32_t __fastcall Models_Move(uint32_t key, uint32_t sub);
RT3_NOINLINE void Hotkeys_RegisterDefaults();
void Models_Shutdown();
RT3_NOINLINE void __fastcall Hotkeys_Init(int32_t load);
RT3_NOINLINE uint8_t* __fastcall NamedObj4611e0_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall NamedObj4611e0_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall Thunk461220(uint8_t* o);
void __fastcall Marker_Init(uint8_t* o, int, uint32_t id, uint32_t x, uint32_t y, const char* label, int32_t bodied);
int32_t __fastcall Thunk461310(uint8_t* p0, int p1, int32_t p2);
void __fastcall Thunk461320(uint8_t* p0, int p1, Stream p2);
void __fastcall Obj_SelectWithMessage(uint8_t* o);
void Tooltip_Show1b2();
void __fastcall Nop4613c0(void*);
int32_t __fastcall Scenery_Tooltip(uint8_t* o, int, char* out, int32_t n, uint32_t* color, float* scale, int32_t a5);
int32_t __fastcall Obj_FogEnd2(void* o);
int32_t __fastcall Obj_Visible461500(uint8_t* o);
RT3_NOINLINE Collection* __fastcall Collection5ce044_Construct(Collection* c);
void __fastcall Collection5ce044_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall NamedObjs4611e0_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Collection_SaveWithRecords5209b(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
void __fastcall SceneObjects_Remove(Collection* c, int, uint32_t id);
void __fastcall Objs_DropAllInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Names_ApplyRegionVariants(Collection* c, int, char* mapName);
RT3_NOINLINE void __fastcall Collection5ce068_Destruct(Collection* c);
RT3_NOINLINE float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
uint32_t __fastcall Table_AddWith62ba88Refs(Collection* t, int, uint8_t* rec, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
RT3_NOINLINE void* __fastcall Obj_Ref72Plus45(void* o);
RT3_NOINLINE uint32_t __fastcall Obj_RefName72(void* o);
RT3_NOINLINE uint32_t __fastcall EngineTypes_PickForYear(Collection* c);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
RT3_NOINLINE void __fastcall EngineTypes_Load(Collection* t);
RT3_NOINLINE uint8_t* __fastcall Effect_Spawn(const char* name, uint8_t* parent, uint8_t* def, uint32_t flags, float scale);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);


// Save of a collection whose records save themselves (virtual slot 17), between three tags.
static void SaveWithRecords(Collection* t, Stream s, int32_t recordSize, uint32_t tag) {
    StreamWriteU32(s, tag);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, tag + 1);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, tag + 2);
}

// Model parts: 32 entries of {flag byte, object*} at +0x276 (5 bytes each).
static inline uint8_t* PartObj(uint8_t* o, int32_t i) { return F<uint8_t*>(o, 0x277 + i * 5); }

static inline uint8_t PartFlag(uint8_t* o, int32_t i) { return o[0x276 + i * 5]; }

static inline void Show(uint8_t* p) { ((void(__thiscall*)(void*))(*(void***)p)[0xc / 4])(p); }

static inline void Hide(uint8_t* p) { ((void(__thiscall*)(void*))(*(void***)p)[0x10 / 4])(p); }

// Fog end at the object's cell (Obj_CellX / Obj_CellY truncated).
static int32_t ObjFogEnd(void* o) {
    int32_t cellX, cellY;
    __asm {
        mov ecx, o
        call Obj_CellY
        call Rt3_Ftol
        mov cellY, eax
        mov ecx, o
        call Obj_CellX
        call Rt3_Ftol
        mov cellX, eax
    }
    return Map_FogEnd(cellX, cellY);
}

static inline uint8_t* World() { return g_world; }

static inline uint8_t* Snd() { return g_sound; }

static inline float FromBits(uint32_t u) { return *(float*)&u; }

static inline float SnapInt(float v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return (float)r;
}

static inline bool InCellRect(uint8_t* o, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    return Obj_CellX(o) >= (float)x0 && Obj_CellX(o) <= (float)x1 && Obj_CellY(o) >= (float)y0 &&
           Obj_CellY(o) <= (float)y1;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// 0x45ede0 body. The model in the hash buckets 0x6acd78..0x6ad96c (12 bytes each)
// whose name hash (+0x90) matches name (first 127 characters, case-insensitive).
static uint8_t* __cdecl Models_FindByNameImpl(const char* name) {
    char buf[0x100];
    strncpy(buf, name, 0x7f);
    buf[0x7f] = 0;
    uint32_t h = Crc32String(buf, 1);
    for (uint32_t b = 0x6acd78; b <= 0x6ad96c; b += 0xc)
        for (uint8_t* n = *(uint8_t**)b; n; n = F<uint8_t*>(n, 0))
            if (h == F<uint32_t>(n, 0x90)) return n;
    return nullptr;
}


// 0x45ee40 body. Unlinks a model from its hash bucket (index +0xc into the
// { head, tail, count } buckets at 0x6acd78) and, with recycle == 1, returns it to
// the model pool 0x6acd6c (or frees it when outside the pool). A model not in its
// bucket is left alone.
static void __cdecl Models_UnlinkImpl(uint8_t* m, int32_t recycle) {
    uint8_t* b = (uint8_t*)0x6acd78 + F<int32_t>(m, 0xc) * 0xc;
    uint8_t* head = F<uint8_t*>(b, 0);
    if (m == head) {
        uint8_t* next = F<uint8_t*>(head, 0);
        F<uint8_t*>(b, 0) = next;
        if (head == F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = next;
        --F<int32_t>(b, 8);
    } else {
        if (!head) return;
        uint8_t* prev = head;
        for (;;) {
            uint8_t* cur = F<uint8_t*>(prev, 0);
            if (cur == m) break;
            prev = cur;
            if (!prev) return;
        }
        F<uint8_t*>(prev, 0) = F<uint8_t*>(m, 0);
        F<uint8_t*>(m, 0) = nullptr;
        --F<int32_t>(b, 8);
        if (m == F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = prev;
    }
    if (recycle != 1) return;
    uint8_t* pool = g_6acd6c;
    if (F<int32_t>(pool, 0xc) && m >= F<uint8_t*>(pool, 4) && m < F<uint8_t*>(pool, 8)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        F<uint8_t*>(m, 0) = F<uint8_t*>(list, 0);
        F<uint8_t*>(list, 0) = m;
        ++F<int32_t>(list, 8);
        if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = m;
    } else crt::operator_delete(m);
}

// 0x45ea90 body. The display name of record rec into out: the qualifier words of
// its flags +0x94 (4: string 0xdef, 2: 0xdf0, 1: 0xdf1) followed by its type's name
// from the table 0x5f0a34 (by value +0xc; localized string or fallback). Empty for
// type 0xff; only the qualifiers for an unknown type.
static void __cdecl Record_DisplayNameImpl(char* out, uint8_t* rec) {
    out[0] = 0;
    if (F<int32_t>(rec, 0xc) == 0xff) return;
    if (F<uint8_t>(rec, 0x94) & 4) strcat(out, GetString(0xdef));
    if (F<uint8_t>(rec, 0x94) & 2) strcat(out, GetString(0xdf0));
    if (F<uint8_t>(rec, 0x94) & 1) strcat(out, GetString(0xdf1));
    for (uint32_t i = 0; i < 0x5b; ++i) {
        uint8_t* e = (uint8_t*)0x5f0a34 + i * 0x10;
        if (F<int32_t>(rec, 0xc) != F<int32_t>(e, 0)) continue;
        int32_t id = F<int32_t>(e, 4);
        strcat(out, id && (uint8_t)HasString(id) ? GetString(F<int32_t>(e, 4)) : F<const char*>(e, 8));
        return;
    }
}


// 0x45e390. Vehicle o: tears down its scene parts - stops its sound (+0x346),
// clears crash / breakdown, frees each car record's buffers (+0x316, 0xc0
// bytes: +0x4 count of +0xa0 entries, +0x98, +0x9c) and the models of its
// part list (+0xc -> +0x75), then the scene object itself and its buffers.
RT3_NOINLINE void __fastcall Vehicle_Teardown(uint8_t* o) {
    if (F<uint32_t>(o, 0x346)) {
        if (Sound_LookupSample(g_sound, 0, F<uint32_t>(o, 0x346), 1))
            Sound_Stop(g_sound, 0, F<uint32_t>(o, 0x346), 1);
        F<uint32_t>(o, 0x346) = 0;
    }
    F<uint32_t>(o, 0x336) = 0;
    Car_SetCrashed(o, 0, 0);
    Car_SetBrokenDown(o, 0, 0);
    for (int32_t i = F<int32_t>(o, 0x31a) - 1; i >= 0; --i) {
        uint32_t r = (uint32_t)i * 0xc0;
        for (int32_t k = 0; k < F<int32_t>(F<uint8_t*>(o, 0x316) + r, 4); ++k) crt::free(F<void*>(F<uint8_t*>(o, 0x316) + r, 0xa0 + k * 4));
        crt::free(F<void*>(F<uint8_t*>(o, 0x316) + r, 0x98));
        crt::free(F<void*>(F<uint8_t*>(o, 0x316) + r, 0x9c));
        uint8_t* s = F<uint8_t*>(F<uint8_t*>(o, 0x316) + r, 0xc);
        if (!F<PtrList*>(s, 0x75)) continue;
        PtrList_Rewind(F<PtrList*>(s, 0x75));
        for (uint8_t* v = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x75)); v; v = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x75))) {
            SceneObj_Teardown(v);
            NamedObj_Destructor(v);
            crt::operator_delete(v);
        }
        if (PtrList* l = F<PtrList*>(s, 0x75)) {
            PtrList_Destroy(l);
            crt::operator_delete(l);
        }
        F<void*>(s, 0x75) = nullptr;
    }
    SceneObj_Teardown(o);
    F<uint32_t>(F<uint8_t*>(o, 0x316), 0xc) = 0;
    if (F<void*>(o, 0x316)) crt::free(F<void*>(o, 0x316));
    F<void*>(o, 0x316) = nullptr;
    FreeAndNull2((void**)(o + 0x32e));
    FreeAndNull2((void**)(o + 0x332));
}
RT3_REPLACE(0x45e390, Vehicle_Teardown);

// 0x45e530. Loads car c from stream s: its model and texture set names
// (the texture set falls back to the type's +0xa1, clearing *texSet, unless
// a texture set named `name` is in the pack table +0x429b8), reloads the
// model and its emitters, then (saves from 0x2586) each part's and its
// attachments' state (vtable +0x48) after checking their count, or else
// (older saves, or a count mismatch) a position and angles up to a 0x61ab
// marker. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall Car_Load(uint8_t* c, int, int32_t s, char* texSet, const char* name, uint8_t* type) {
    F<uint32_t>(c, 0x316) = 0;
    F<uint32_t>(c, 0x31a) = 0;
    F<uint32_t>(c, 0x32e) = 0;
    F<uint32_t>(c, 0x332) = 0;
    F<uint32_t>(c, 0x336) = 0;
    NamedObj_Reset(c);
    int32_t total = StreamSkip4(s, 0x61a9, 0, 0);
    total += StreamSkip4(s, 0x61aa, 0, 0);
    char* model = nullptr;
    char* tex = nullptr;
    total += StreamReadString(s, &model);
    total += StreamReadString(s, &tex);
    if (*texSet) {
        bool found = false;
        for (int32_t i = 0; i < Collection_Count(F<Collection*>(g_pack, 0x429b8)); ++i) {
            Collection* t = F<Collection*>(g_pack, 0x429b8);
            if (!crt::stricmp((const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0)), name)) {
                found = true;
                break;
            }
        }
        if (!found) {
            String_Assign(&tex, (const char*)type + 0xa1);
            *texSet = 0;
        }
    }
    Car_LoadModel(c, 0, model, tex, 1.0f, 1.0f, 1);
    for (uint32_t k = 0x10; k < 0x30; k += 4)
        if (uint8_t* e = F<uint8_t*>(F<uint8_t*>(c, 0x316), k)) Clear_d939a0(hook::As<void*>(g_models), 0, (int32_t)(uintptr_t)e, s);
    FreeAndNull2((void**)&model);
    FreeAndNull2((void**)&tex);
    bool old = F<int32_t>(g_world, 0x46c34) <= 0x2585;
    if (!old) {
        total += StreamSkip4(s, 0x61b2, 0, 0);
        int32_t count = 0, saved;
        for (int32_t pass = 0; pass < 2 && !old; ++pass) {
            for (int32_t m = 0; m < F<int32_t>(c, 0x31a); ++m) {
                uint8_t* o = F<uint8_t*>(F<uint8_t*>(c, 0x316) + m * 0xc0, 0xc);
                if (!pass)
                    ++count;
                else
                    total += ((int32_t(__thiscall*)(uint8_t*, int32_t))(*(void***)o)[0x12])(o, s);
                if (!F<PtrList*>(o, 0x75)) continue;
                PtrList_Rewind(F<PtrList*>(o, 0x75));
                for (uint8_t* a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)); a; a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75))) {
                    if (!pass)
                        ++count;
                    else
                        ((int32_t(__thiscall*)(uint8_t*, int32_t))(*(void***)a)[0x12])(a, s);
                }
            }
            if (!pass) {
                StreamRead(s, &saved, 4, 0, 1);
                if (saved != count) old = true;
            }
        }
        if (!old) total += StreamSkip4(s, 0x61ab, 0, 0);
    }
    if (old) {
        float x, y, z;
        uint32_t a, b, cc, mark;
        StreamRead(s, &x, 4, 0, 1);
        if (SnapInt(x) != (float)0x61ab) {
            StreamRead(s, &y, 4, 0, 1);
            StreamRead(s, &z, 4, 0, 1);
            StreamRead(s, &a, 4, 0, 1);
            StreamRead(s, &b, 4, 0, 1);
            StreamRead(s, &cc, 4, 0, 1);
            do StreamRead(s, &mark, 4, 0, 1);
            while (mark != 0x61ab);
            Obj_MoveTo(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 0, x, y, z);
            SceneObj_SetAngles(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 0, a, b, cc);
        }
    }
    uint8_t* r0 = F<uint8_t*>(c, 0x316);
    F<uint32_t>(r0, 0x34) = 0;
    F<uint32_t>(r0, 0x38) = 0;
    F<uint32_t>(r0, 0x3c) = 0;
    F<uint32_t>(r0, 0x40) = 0;
    return total;
}
RT3_REPLACE(0x45e530, Car_Load);

// 0x45e8c0. Hides all parts and shows part `sel` (texture reset when flagged).
void __fastcall Model_ShowOnlyPart(uint8_t* o, int, int32_t sel) {
    for (int32_t i = 0; i < 0x20; ++i)
        if (uint8_t* p = PartObj(o, i)) Hide(p);
    if (sel == -1 || sel < 0 || sel >= 0x20) return;
    uint8_t* p = PartObj(o, sel);
    if (!p) return;
    Show(p);
    if (PartFlag(o, sel)) SceneObj_SetUvTransform(PartObj(o, sel), 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x45e8c0, Model_ShowOnlyPart);

// 0x45e940. Spawns on `parent` an emitter for each emitter model file of
// `prefix` (Emitters_NextFile; none for stadiums), at most `count`, storing
// them in `out` (null-terminated while room is left). ParticleCar drops flag
// 0x10, the world's +0x46c3c adds 0x10000, Nuclear ones get +0xb0 bit 2.
// Returns what the original leaves in EAX.
RT3_NOINLINE uint32_t __fastcall Effects_SpawnForModel(uint8_t* parent, int, const char* prefix, uint8_t** out, int32_t count, uint32_t flags, float scale) {
    typedef const char*(__cdecl * StrstrFn)(const char*, const char*);
    if (const char* p = ((const char*)crt::strstr(prefix, (const char*)0x5c8f14))) return (uint32_t)(uintptr_t)p;
    if (((const char*)crt::strstr(prefix, (const char*)0x5cb7f8))) flags &= 0xffffffef;
    uint8_t* rec = Emitters_NextFile(prefix, 1);
    while (rec && count) {
        if (F<int32_t>(g_world, 0x46c3c)) flags |= 0x10000;
        uint8_t* e = Effect_Spawn((const char*)rec + 4, parent, F<uint8_t*>(rec, 0), flags, scale);
        if (e) {
            --count;
            if (((const char*)crt::strstr(prefix, (const char*)0x5cb7f0))) F<uint32_t>(e, 0xb0) |= 2;
            if (out) *out++ = e;
        }
        rec = Emitters_NextFile(prefix, 0);
    }
    if (!out) return (uint32_t)(uintptr_t)rec;
    if (count) *out = nullptr;
    return (uint32_t)count;
}
RT3_REPLACE(0x45e940, Effects_SpawnForModel);

// 0x45ea20. Plays interface sound id (2..64, table 0x5f0828 {volume, name}) at pan,
// unless muted (0xccba14).
RT3_NOINLINE void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan) {
    if (id <= 1 || g_ccba14 || id <= 0 || id >= 0x41) return;
    const uint32_t* e = (const uint32_t*)(0x5f0828 + id * 8);
    Sound_Play2D(Snd(), 0, (const char*)e[1], FromBits(e[0]), pan, 0, 0, nullptr);
}
RT3_REPLACE(0x45ea20, Ui_PlaySound);

// 0x45ea60. MSVC "vector constructor iterator": calls fn(elem) for count elements.
void __stdcall VectorConstruct(uint8_t* base, int32_t stride, int32_t count, void(__thiscall* fn)(void*)) {
    for (int32_t i = count - 1; i >= 0; --i, base += stride) fn(base);
}
RT3_REPLACE(0x45ea60, VectorConstruct);

// 0x45ea90. Output in EAX, record on the stack.
__declspec(naked) void Record_DisplayName() {
    __asm {
        push [esp + 4]
        push eax
        call Record_DisplayNameImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x45ea90, Record_DisplayName);

// 0x45ec00 body. Splits "a+b" (unless it starts with '+' or is string 0xdf2):
// dst gets b and dst2 a; otherwise dst gets the whole text and dst2 is emptied.
RT3_NOINLINE void __cdecl Text_SplitPlusImpl(char* dst, char* src, char* dst2) {
    char* p;
    if (*src != '+' && crt::stricmp(src, GetString(0xdf2)) && (p = crt::strchr(src, '+')) != nullptr) {
        strcpy(dst, p + 1);
        *p = 0;
        strcpy(dst2, src);
        return;
    }
    strcpy(dst, src);
    *dst2 = 0;
}

// 0x45ec00. Destination in EAX, source in ESI, second destination in EBX.
__declspec(naked) void Text_SplitPlus() {
    __asm {
        push ebx
        push esi
        push eax
        call Text_SplitPlusImpl
        add esp, 12
        ret
    }
}
RT3_REPLACE(0x45ec00, Text_SplitPlus);

// 0x45ec70 body. The value for a named entry of the table at 0x5f0a34 (0x5b
// entries of { value, string id, fallback name, - }), matched case-insensitively
// against the localized string or the fallback; 0xff for "", -1 when unknown.
RT3_NOINLINE int32_t __cdecl NamedValue_FindImpl(const char* name) {
    if (!*name) return 0xff;
    for (uint32_t i = 0; i < 0x5b; ++i) {
        uint8_t* e = (uint8_t*)0x5f0a34 + i * 0x10;
        int32_t id = F<int32_t>(e, 4);
        const char* s = id && (uint8_t)HasString(id) ? GetString(F<int32_t>(e, 4)) : F<const char*>(e, 8);
        if (!crt::stricmp(name, s)) return F<int32_t>(e, 0);
    }
    return -1;
}

// 0x45ec70. Name in EBX.
__declspec(naked) void NamedValue_Find() {
    __asm {
        push ebx
        call NamedValue_FindImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x45ec70, NamedValue_Find);

// 0x45ecd0 body. Classifies a scenario team name (uppercased in place first):
// 1 for string 0xdf3 or names starting like 0x5cbc64 / 0xdf3 (3 characters),
// else 4 for 0xdf4, 2 for 0xdf5, 0 otherwise.
RT3_NOINLINE int32_t __cdecl TeamName_KindImpl(char* name) {
    crt::strlwr(name);
    typedef int(__cdecl * StrnicmpFn)(const char*, const char*, size_t);
    if (!crt::stricmp(name, GetString(0xdf3))) return 1;
    if (!crt::strnicmp(name, (const char*)0x5cbc64, 3) && !crt::strnicmp(name, GetString(0xdf3), 3)) return 1;
    if (!crt::stricmp(name, GetString(0xdf4))) return 4;
    if (!crt::stricmp(name, GetString(0xdf5))) return 2;
    return 0;
}

// 0x45ecd0. Name in ESI.
__declspec(naked) void TeamName_Kind() {
    __asm {
        push esi
        call TeamName_KindImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x45ecd0, TeamName_Kind);

// 0x45ed70. Repacks flags: bit 0 = any of bits 0-1, bit 1 = bit 5, bit 2 = bit 2.
RT3_NOINLINE uint32_t __fastcall RepackFlags(uint32_t f) { return ((f & 3) ? 1u : 0u) | ((f >> 4) & 2) | (f & 4); }
RT3_REPLACE(0x45ed70, RepackFlags);

// 0x45ed90. Value of entry `key` in the table 0x5f0a30 ({value, key, ..}
// x 91) if `mask` has at most one bit set; 0 otherwise.
RT3_NOINLINE uint32_t __fastcall LookupSingleFlag(int32_t key, int32_t mask) {
    uint32_t i = 0;
    for (; i < 0x5b; ++i)
        if (((const int32_t*)0x5f0a34)[i * 4] == key) break;
    if (i >= 0x5b) return 0;
    int32_t bits = 0;
    for (int32_t m = mask; m; m >>= 1)
        if (m & 1) ++bits;
    if (bits > 1) return 0;
    return ((const uint32_t*)0x5f0a30)[i * 4];
}
RT3_REPLACE(0x45ed90, LookupSingleFlag);

// 0x45ede0. Name in EAX.
__declspec(naked) void Models_FindByName() {
    __asm {
        push eax
        call Models_FindByNameImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x45ede0, Models_FindByName);

// 0x45ee40. Model in EAX, recycle flag in EDI.
__declspec(naked) void Models_Unlink() {
    __asm {
        push edi
        push eax
        call Models_UnlinkImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x45ee40, Models_Unlink);

// 0x45ef00 body. Puts a copy of the model called name (taken from the model pool
// 0x6acd6c) at the head of bucket slot under key (+0x94); the original record is
// unlinked and recycled, and an older record of the slot with the same key goes
// to the free list (bucket 0xff). Needs slot to be 0xff or a known flag for key.
// 1 when done.
RT3_NOINLINE int32_t __cdecl Models_AddCopyImpl(int32_t slot, int32_t key, const char* name) {
    if (slot != 0xff && !LookupSingleFlag(slot, key)) return 0;
    uint8_t* src = Models_FindByNameImpl(name);
    if (!src) return 0;
    uint8_t* pool = g_6acd6c;
    uint8_t* m;
    if (F<int32_t>(pool, 0xc)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        m = F<uint8_t*>(list, 0);
        if (m) {
            F<uint8_t*>(list, 0) = F<uint8_t*>(m, 0);
            if (m == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = F<uint8_t*>(m, 0);
            --F<int32_t>(list, 8);
        }
    } else m = (uint8_t*)crt::operator_new(F<uint32_t>(pool, 0x10));
    F<int32_t>(m, 0xc) = slot;
    F<int32_t>(m, 0x94) = key;
    F<int32_t>(m, 0x98) = F<int32_t>(src, 0x98);
    F<int32_t>(m, 0xa0) = F<int32_t>(src, 0xa0);
    F<int32_t>(m, 0x9c) = F<int32_t>(src, 0x9c);
    F<int32_t>(m, 0x90) = F<int32_t>(src, 0x90);
    F<int32_t>(m, 4) = F<int32_t>(src, 4);
    F<int32_t>(m, 8) = F<int32_t>(src, 8);
    strcpy((char*)m + 0x10, (const char*)src + 0x10);
    Models_UnlinkImpl(src, 1);
    uint8_t* b = (uint8_t*)0x6acd78 + slot * 0xc;
    if (slot != 0xff) {
        for (uint8_t* o = F<uint8_t*>(b, 0); o; o = F<uint8_t*>(o, 0)) {
            if (F<int32_t>(o, 0x94) != key) continue;
            Models_UnlinkImpl(o, 0);
            F<int32_t>(o, 0xc) = 0xff;
            F<uint8_t*>(o, 0) = g_6ad96c;
            ++g_6ad974;
            g_6ad96c = o;
            if (!g_6ad970) g_6ad970 = o;
            break;
        }
    }
    F<uint8_t*>(m, 0) = F<uint8_t*>(b, 0);
    F<uint8_t*>(b, 0) = m;
    ++F<int32_t>(b, 8);
    if (!F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = m;
    return 1;
}

// 0x45ef00. Slot in ECX, key in EDX, name on the stack; leaves ECX = key.
__declspec(naked) void Models_AddCopy() {
    __asm {
        push edx
        push [esp + 8]
        push edx
        push ecx
        call Models_AddCopyImpl
        add esp, 12
        pop ecx
        ret 4
    }
}
RT3_REPLACE(0x45ef00, Models_AddCopy);

// 0x45f140. Offers record r (kind +0 3 or 4, model id +8) to the handlers chained in
// its model's hash bucket (0x6acd78): the first handler whose kind mask (+0x98)
// matches, whose variant (+0x94) equals r's flag variant (+0x20; ids 0x21..0x24 and
// 0x2c..0x2e fold variant 1 into 0) and that is allowed by the options (+4 needs
// option +0x28f, +8 needs 0x6d3b4c) is called (+0xa0, thiscall on r). 1 otherwise.
int32_t __fastcall Records_Dispatch(uint8_t* r) {
    uint32_t mask = (F<int32_t>(r, 0) == 4 ? 2 : 0) | (F<int32_t>(r, 0) == 3 ? 1 : 0);
    if (!mask) return 1;
    int32_t id = F<int32_t>(r, 8);
    if (id == 0xff) return 1;
    uint32_t variant = 1;
    for (uint8_t* n = *(uint8_t**)(0x6acd78 + id * 0xc); n; n = F<uint8_t*>(n, 0)) {
        if (!(F<uint32_t>(n, 0x98) & mask)) continue;
        uint32_t v = F<uint32_t>(r, 0x20);
        variant = ((v & 3) != 0 ? 1u : 0u) | (v >> 4 & 2) | (v & 4);
        if (((id >= 0x21 && id <= 0x24) || (id >= 0x2c && id <= 0x2e)) && variant == 1) variant = 0;
        if (variant != F<uint32_t>(n, 0x94)) continue;
        if (!F<int32_t>(g_app, 0x28f) && F<int32_t>(n, 4)) continue;
        if (g_6d3b4c && !F<int32_t>(n, 8)) continue;
        void* fn = F<void*>(n, 0xa0);
        if (!fn) return 1;
        return ((int32_t(__fastcall*)(uint8_t*))fn)(r);
    }
    return 1;
}
RT3_REPLACE(0x45f140, Records_Dispatch);

// 0x45f250. The next model in list order (by priority +0x9c, smallest first; hidden
// ones only with option +0x28f): its display name, file name and two values in
// the static record 0x6ad978 (returned); null at the end, which also resets the
// walk (0x5f0fe0, heap 0x6acd74).
RT3_NOINLINE uint8_t* Models_NextInOrder() {
    if (g_5f0fe0 == 1) {
        g_5f0fe0 = 0;
        uint32_t* h = (uint32_t*)crt::operator_new(0x1c);
        if (h) h[0] = h[1] = h[2] = h[3] = 0;
        hook::As<void*>(g_6acd74) = h;
        Heap_Init((MaxHeap*)h, 0, 0x400);
        for (uint32_t b = 0x6acd78; b <= 0x6ad96c; b += 0xc)
            for (uint8_t* m = *(uint8_t**)b; m; m = F<uint8_t*>(m, 0))
                if (F<int32_t>(g_app, 0x28f) || !F<int32_t>(m, 4))
                    Heap_Push(g_6acd74, 0, 0x7ffffffe - F<int32_t>(m, 0x9c), (int32_t)(uintptr_t)m);
    }
    uint8_t* m = (uint8_t*)(uintptr_t)Heap_PopMax(g_6acd74);
    if (m) {
        Record_DisplayNameImpl((char*)0x6ad980, m);
        strcpy((char*)0x6ada00, (const char*)m + 0x10);
        g_6ad978 = F<int32_t>(m, 4);
        g_6ad97c = F<int32_t>(m, 8);
        return (uint8_t*)0x6ad978;
    }
    if (MaxHeap* h = g_6acd74) {
        Heap_Destroy2(h);
        crt::operator_delete(h);
    }
    hook::As<void*>(g_6acd74) = nullptr;
    g_5f0fe0 = 1;
    return nullptr;
}
RT3_REPLACE(0x45f250, Models_NextInOrder);

// 0x45f370. Finds the entry with key +0xa0 in the hash buckets 0x6acd78..0x6ad960
// (stride 12): outputs its +0xc and repacked flags +0x94. 1 if found.
int32_t __fastcall Buckets_Find(int32_t key, int32_t* value, uint32_t* flags) {
    for (uint8_t** b = (uint8_t**)0x6acd78; b <= (uint8_t**)0x6ad960; b += 3) {
        for (uint8_t* e = *b; e; e = *(uint8_t**)e) {
            if (F<int32_t>(e, 0xa0) != key) continue;
            *value = F<int32_t>(e, 0xc);
            uint32_t f = F<uint32_t>(e, 0x94);
            *flags = ((((f & 2) << 3) | (f & 1)) << 1) | (f & 4);
            return 1;
        }
    }
    return 0;
}
RT3_REPLACE(0x45f370, Buckets_Find);

// 0x45f3d0. The display name of the model called name into out; 0 when unknown.
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out) {
    uint8_t* m = Models_FindByNameImpl(name);
    if (!m) return 0;
    Record_DisplayNameImpl(out, m);
    return 1;
}
RT3_REPLACE(0x45f3d0, Models_DisplayName);

// 0x45f400. The value (+0xc) of the model called name, or 0.
RT3_NOINLINE int32_t __fastcall Models_ValueByName(const char* name) {
    uint8_t* m = Models_FindByNameImpl(name);
    return m ? F<int32_t>(m, 0xc) : 0;
}
RT3_REPLACE(0x45f400, Models_ValueByName);

// 0x45f420. Loads the key bindings file `path` (comment characters 0x21 /
// 0x7e, then 0x28 / 0x29 when that fails): every "name = keys" entry of
// every section is split at '+' into the command and the key, both
// trimmed; a known command (0x45ec70) gets the key with its modifiers
// (0x45ecd0) bound to the entry name. Returns the load result.
RT3_NOINLINE int32_t __fastcall Hotkeys_LoadFile(const char* path) {
    char command[0x80], mods[0x80];
    uint8_t holder[0x118];
    Holder_Init(holder, 0, 0x64, 0x12c);
    Set_6257e0(0x21, 0x7e);
    int32_t r = Config_Load(holder, 0, path);
    if (!r) {
        Set_6257e0(0x28, 0x29);
        r = Config_Load(holder, 0, path);
    }
    if (r == 1) {
        for (uint8_t* sec = *F<uint8_t**>(holder, 8); sec; sec = F<uint8_t*>(sec, 0))
            for (uint8_t* e = F<uint8_t*>(sec, 0x108); e; e = F<uint8_t*>(e, 0)) {
                Text_SplitPlusImpl(command, (char*)e + 0x104, mods);
                Str_TrimLeft(command);
                Str_TrimRight(command);
                Str_TrimLeft(mods);
                Str_TrimRight(mods);
                const int32_t id = NamedValue_FindImpl(command);
                if (id == -1) continue;
                Models_AddCopyImpl(id, TeamName_KindImpl(mods), (const char*)e + 4);
            }
        r = 1;
    }
    Holder_Destruct(holder);
    return r;
}
RT3_REPLACE(0x45f420, Hotkeys_LoadFile);

// 0x45f550. Writes the hotkey file path: the comment header (strings 0xdf6..0xdfc),
// "!Hotkeys~", then "file = name" for every shown model in list order, the file
// name padded to 32 columns. 0 when the file cannot be created.
RT3_NOINLINE int32_t __fastcall Hotkeys_Save(const char* path) {
    void* f = crt::fopen(path, (const char*)0x5cbc80);
    if (!f) return 0;
    int(__cdecl* const print)(void*, const char*, ...) = (int(__cdecl*)(void*, const char*, ...))0x5a2bd2;
    for (int32_t id = 0xdf6; id <= 0xdfc; ++id) print(f, GetString(id));
    print(f, (const char*)0x5cbc74);
    for (uint8_t* r = Models_NextInOrder(); r; r = Models_NextInOrder()) {
        char line[0x100];
        memset(line, 0, sizeof line);
        strcpy(line, (const char*)r + 0x88);
        int32_t n = (int32_t)strlen(line);
        if (n < 0x20) memset(line + n, ' ', 0x20 - n);
        if (F<int32_t>(r, 0) == 0) print(f, (const char*)0x5cbc68, line, (const char*)r + 8);
    }
    crt::fclose(f);
    return 1;
}
RT3_REPLACE(0x45f550, Hotkeys_Save);

// 0x45f6d0 (a in ECX, b in EDX). Adds a model entry (0xa4 bytes, from the pool
// 0x6acd6c or the heap) to hash bucket `key` (0x6acd78, { head, tail, count }):
// a +4, b +8, key +0xc, name +0x10 (127 characters), its case-blind hash +0x90
// (registered in 0x6acd70 with `value`), sub +0x94, v +0x98, id +0x9c (-1:
// the next of 0x6acd68), value +0xa0. Modes 1..4 (0x6d3c84) may use their own
// (key, sub) pair. Unless key is 0xff, the pair must be enabled (0 otherwise)
// and an existing entry for it moves to bucket 0xff first. Returns 1.
RT3_NOINLINE uint32_t __fastcall Models_Add(int32_t a, int32_t b, uint32_t key, uint32_t sub, uint32_t v, int32_t value, const char* name, int32_t id, uint32_t key2,
                               uint32_t sub2, uint32_t key1, uint32_t sub1, uint32_t key3, uint32_t sub3, uint32_t key4, uint32_t sub4) {
    int32_t mode = g_language;
    uint32_t k = 0, s = 0;
    if (mode == 2) k = key2, s = sub2;
    else if (mode == 1) k = key1, s = sub1;
    else if (mode == 3) k = key3, s = sub3;
    else if (mode == 4) k = key4, s = sub4;
    if (!k) k = key, s = sub;
    uint8_t* buckets = (uint8_t*)0x6acd78;
    if (k != 0xff) {
        if (!LookupSingleFlag((int32_t)k, (int32_t)s)) return 0;
        for (uint8_t* n = F<uint8_t*>(buckets, k * 0xc); n; n = F<uint8_t*>(n, 0))
            if (F<uint32_t>(n, 0x94) == s) {
                Models_Move(k, s);
                break;
            }
    }
    uint8_t* pool = g_6acd6c;
    uint8_t* m;
    if (F<int32_t>(pool, 0xc)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        m = F<uint8_t*>(list, 0);
        if (m) {
            F<uint8_t*>(list, 0) = F<uint8_t*>(m, 0);
            if (m == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = F<uint8_t*>(m, 0);
            --F<int32_t>(list, 8);
        }
    } else
        m = (uint8_t*)crt::operator_new(F<uint32_t>(pool, 0x10));
    memset(m, 0, 0xa4);
    F<int32_t>(m, 8) = b;
    F<uint32_t>(m, 0x94) = s;
    F<int32_t>(m, 4) = a;
    F<uint32_t>(m, 0xc) = k;
    F<uint32_t>(m, 0x98) = v;
    F<int32_t>(m, 0xa0) = value;
    F<int32_t>(m, 0x9c) = id;
    if (id == -1) F<int32_t>(m, 0x9c) = g_6acd68++;
    crt::strncpy((char*)m + 0x10, name, 0x7f);
    m[0x8f] = 0;
    uint8_t* bucket = buckets + k * 0xc;
    F<uint8_t*>(m, 0) = F<uint8_t*>(bucket, 0);
    ++F<int32_t>(bucket, 8);
    F<uint8_t*>(bucket, 0) = m;
    if (!F<uint8_t*>(bucket, 4)) F<uint8_t*>(bucket, 4) = m;
    uint32_t h = Crc32String((const char*)m + 0x10, 1);
    F<uint32_t>(m, 0x90) = h;
    if (Hash_Contains(g_6acd70, 0, h) == 1) Hash_Remove(g_6acd70, 0, h);
    Hash_Insert(g_6acd70, 0, h, value);
    return 1;
}
RT3_REPLACE(0x45f6d0, Models_Add);

// 0x45f8a0 (key in ECX, sub in EDX). Moves the model entry for (key, sub)
// to bucket 0xff: unlinks it (back into the pool, or freed) and adds a copy
// with the same name, v, value and id. 0 when there is none.
RT3_NOINLINE uint32_t __fastcall Models_Move(uint32_t key, uint32_t sub) {
    uint8_t* m = F<uint8_t*>((uint8_t*)0x6acd78, key * 0xc);
    for (; m; m = F<uint8_t*>(m, 0))
        if (F<uint32_t>(m, 0x94) == sub) break;
    if (!m) return 0;
    uint8_t copy[0xa4];
    memcpy(copy, m, sizeof copy);
    uint8_t* b = (uint8_t*)0x6acd78 + F<int32_t>(m, 0xc) * 0xc;
    uint8_t* head = F<uint8_t*>(b, 0);
    bool unlinked = false;
    if (m == head) {
        if (head) {
            F<uint8_t*>(b, 0) = F<uint8_t*>(head, 0);
            if (head == F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = F<uint8_t*>(head, 0);
            --F<int32_t>(b, 8);
        }
        unlinked = true;
    } else {
        for (uint8_t* prev = head; prev; prev = F<uint8_t*>(prev, 0))
            if (F<uint8_t*>(prev, 0) == m) {
                F<uint8_t*>(prev, 0) = F<uint8_t*>(m, 0);
                F<uint8_t*>(m, 0) = nullptr;
                --F<int32_t>(b, 8);
                if (m == F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = prev;
                unlinked = true;
                break;
            }
    }
    if (unlinked) {
        uint8_t* pool = g_6acd6c;
        if (F<int32_t>(pool, 0xc) && m >= F<uint8_t*>(pool, 4) && m < F<uint8_t*>(pool, 8)) {
            uint8_t* list = F<uint8_t*>(pool, 0);
            F<uint8_t*>(m, 0) = F<uint8_t*>(list, 0);
            ++F<int32_t>(list, 8);
            F<uint8_t*>(list, 0) = m;
            if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = m;
        } else
            crt::operator_delete(m);
    }
    return Models_Add(F<int32_t>(copy, 4), F<int32_t>(copy, 8), 0xff, 0, F<uint32_t>(copy, 0x98), F<int32_t>(copy, 0xa0), (const char*)copy + 0x10,
                      F<int32_t>(copy, 0x9c), 0, 0, 0, 0, 0, 0, 0, 0);
}
RT3_REPLACE(0x45f8a0, Models_Move);

// 0x45f9e0. Registers the default key bindings (Models_Add, id -1).
RT3_NOINLINE void Hotkeys_RegisterDefaults() {
    for (const HotkeyDefault& h : kHotkeyDefaults) {
        const char* name = GetString((int32_t)h.name);
        Models_Add(h.a, h.b, h.key, h.sub, h.kind, (int32_t)h.handler, name, -1, h.alt[0], h.alt[1], h.alt[2], h.alt[3], h.alt[4], h.alt[5], h.alt[6], h.alt[7]);
    }
}
RT3_REPLACE(0x45f9e0, Hotkeys_RegisterDefaults);

// 0x461070. Shuts the model tables down: with a model pool 0x6acd6c the hotkeys
// are saved first (0x5cbc84), then the pool (its block +4), the name map 0x6acd70,
// the buckets 0x6acd78..0x6ad977 and the list-order heap 0x6acd74 are released.
RT3_NOINLINE void Models_Shutdown() {
    if (g_6acd6c) {
        Hotkeys_Save((const char*)0x5cbc84);
        if (uint8_t* pool = g_6acd6c) {
            if (void* b = F<void*>(pool, 4)) crt::free(b);
            pool = g_6acd6c;
            if (pool) {
                Pool_Free((NodePool*)pool);
                crt::operator_delete(pool);
            }
            g_6acd6c = nullptr;
        }
    }
    if (HashMap* m = g_6acd70) {
        Hash_DestroyThunk(m);
        crt::operator_delete(m);
        g_6acd70 = nullptr;
    }
    for (uint32_t b = 0x6acd78; b <= 0x6ad96c; b += 0xc) memset((void*)b, 0, 0xc);
    if (MaxHeap* h = g_6acd74) {
        Heap_Destroy2(h);
        crt::operator_delete(h);
        g_6acd74 = nullptr;
    }
}
RT3_REPLACE(0x461070, Models_Shutdown);

// 0x461120. (Re)creates the key binding tables - the 0x400-entry pool
// 0x6acd6c (0xa4-byte entries) and the 0x65-bucket hash 0x6acd70 - and
// registers the defaults; load == 1 then reads hotkeys\hotkeys.txt.
RT3_NOINLINE void __fastcall Hotkeys_Init(int32_t load) {
    Models_Shutdown();
    g_6acd68 = 0;
    uint8_t* p = (uint8_t*)crt::operator_new(0x1c);
    if (p) memset(p, 0, 0x1c);
    g_6acd6c = p;
    void* buf = crt::malloc(0x29000);
    memset(buf, 0, 0x29000);
    Pool_Init(hook::As<NodePool*>(g_6acd6c), 0, buf, 0xa4, 0x400, 0);
    uint8_t* h = (uint8_t*)crt::operator_new(0x14);
    if (h) memset(h, 0, 0x14);
    hook::As<uint8_t*>(g_6acd70) = h;
    Hash_Init((HashMap*)h, 0, 0x65, 0x400);
    Hotkeys_RegisterDefaults();
    if (load == 1) Hotkeys_LoadFile((const char*)0x5cbc84);
}
RT3_REPLACE(0x461120, Hotkeys_Init);

RT3_NOINLINE uint8_t* __fastcall NamedObj4611e0_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cbc98;
    F<uint32_t>(o, 0x2a2) = 0x787878;
    o[0x23e] = 0;
    return o;
}
RT3_REPLACE(0x4611e0, NamedObj4611e0_Construct);

RT3_NOINLINE void __fastcall NamedObj4611e0_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5cbc98;
    NamedObj_Destructor(o);
}
RT3_REPLACE(0x461210, NamedObj4611e0_Destructor);

// 0x461220 (stub). Forwards to SceneObj_Teardown (0x455d20).
RT3_NOINLINE void __fastcall Thunk461220(uint8_t* o) { SceneObj_Teardown(o); }
RT3_REPLACE(0x461220, Thunk461220);

// 0x461230. Sets up marker o for owner id at cell (x, y) with label (none:
// empty); a bodied marker uses model "Marker11" when the pack has
// "Marker11_Body", else "Marker01". Registers it with the scene.
RT3_NOINLINE void __fastcall Marker_Init(uint8_t* o, int, uint32_t id, uint32_t x, uint32_t y, const char* label, int32_t bodied) {
    F<uint32_t>(o, 0x23a) = id;
    memset(o + 0x23f, 0, 0x18 * 4 + 3);
    if (label) strcpy((char*)o + 0x2a2, label);
    else o[0x2a2] = 0;
    o[0x23e] = (uint8_t)bodied;
    char name[9];
    const uint8_t* src = (const uint8_t*)0x5cbd0c;
    if (bodied && NameTable_ContainsName(g_pack, 0, (const char*)0x5cbd24, 0, nullptr)) src = (const uint8_t*)0x5cbd18;
    memcpy(name, src, 9);
    WorldModel_Init(o, 0, name, nullptr, nullptr, x, y, 0);
    SceneObj_Register(o);
}
RT3_REPLACE(0x461230, Marker_Init);

// 0x461310 (vtable stub).
// Forwards to WorldModel_Load (0x455fc0).
int32_t __fastcall Thunk461310(uint8_t* p0, int p1, int32_t p2) { return WorldModel_Load(p0, p1, p2); }
RT3_REPLACE(0x461310, Thunk461310);

// 0x461320 (vtable stub).
// Forwards to NamedObj_Save (0x4559d0).
void __fastcall Thunk461320(uint8_t* p0, int p1, Stream p2) { NamedObj_Save(p0, p1, p2); }
RT3_REPLACE(0x461320, Thunk461320);

// 0x461330. Object o: previewed, then shows message 0x2a0 (type 9) with its
// name (+0x2a2).
void __fastcall Obj_SelectWithMessage(uint8_t* o) {
    SceneObj_ShowPreview(o);
    Dialog_RunMessage(GetString(0x2a0), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)(o + 0x2a2), 0x28, 0, nullptr, nullptr, 0, 0,
                      nullptr, 0);
}
RT3_REPLACE(0x461330, Obj_SelectWithMessage);

// 0x461380 (callback). Shows string 0x1b2 as a tooltip.
// QUIRK: the string is used as the sprintf format.
void Tooltip_Show1b2() {
    char text[0x130];
    crt::sprintf(text, GetString(0x1b2));
    Ui_ShowTooltip(g_windowManager, 0, text, 3, 0, -1);
}
RT3_REPLACE(0x461380, Tooltip_Show1b2);

// 0x4613c0 (vtable stub).
void __fastcall Nop4613c0(void*) { __asm nop }
RT3_REPLACE(0x4613c0, Nop4613c0);

 // namespace

// 0x4613d0 (vtable). Tooltip of scenery object o: when fogged (camera fog
// alpha at its cell) its text (+0x2a2), fog-tinted colour 0xffffaf and
// scale -1.5 (0.5 with +0x23e); else the scene object's.
// QUIRK: the text is used as the sprintf format.
int32_t __fastcall Scenery_Tooltip(uint8_t* o, int, char* out, int32_t n, uint32_t* color, float* scale, int32_t a5) {
    if (g_62be84) return 0;
    const int32_t y = Ftol(Obj_CellY(o));
    const int32_t x = Ftol(Obj_CellX(o));
    const int32_t a = Camera_FogAlpha(g_62be68, 0, x, y);
    if (!a) return SceneObj_DebugTooltip(o, 0, out, n, (int32_t)(uintptr_t)color, (int32_t)(uintptr_t)scale, a5);
    *color = 0xffffffaf;
    if ((uint32_t)a < 0xff) *color = (uint32_t)a << 24 | 0xffffaf;
    char text[0x130];
    crt::sprintf(text, (const char*)(o + 0x2a2));
    *(uint32_t*)scale = 0xbfc00000;
    if (o[0x23e]) *(uint32_t*)scale = 0x3f000000;
    StrCopyN(out, text, (size_t)(n - 1));
    return 1;
}
RT3_REPLACE(0x4613d0, Scenery_Tooltip);

// 0x4614d0. Same as 0x41f6b0 (another class).
int32_t __fastcall Obj_FogEnd2(void* o) { return ObjFogEnd(o); }
RT3_REPLACE(0x4614d0, Obj_FogEnd2);

int32_t __fastcall Obj_Visible461500(uint8_t* o) {
    if (!Global6d07b4Set2() && !o[0x23e]) return 1;
    return o[0x42];
}
RT3_REPLACE(0x461500, Obj_Visible461500);

RT3_NOINLINE Collection* __fastcall Collection5ce044_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce044;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x461530, Collection5ce044_Construct);

void __fastcall Collection5ce044_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce044;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x461560, Collection5ce044_Destruct);

// 0x461580. Loads the 0x4611e0 objects (frame 0x5209 / 0x520a / 0x520b).
RT3_NOINLINE int32_t __fastcall NamedObjs4611e0_Load(Collection* t, int, Stream s, int32_t recordSize) {
    return LoadSceneObjects(t, s, recordSize, 0x5209, 0x520a, 0x520b, [](uint8_t* o) { NamedObj4611e0_Construct(o); },
                            [](uint8_t* o) { NamedObj4611e0_Destructor(o); });
}
RT3_REPLACE(0x461580, NamedObjs4611e0_Load);

RT3_NOINLINE void __fastcall Collection_SaveWithRecords5209b(Collection* t, int, Stream s, int32_t recordSize) { SaveWithRecords(t, s, recordSize, 0x5209); }
RT3_REPLACE(0x461650, Collection_SaveWithRecords5209b);

// 0x4616d0. Adds a marker at cell (x, y) with label and body flag to table t
// (0x2cb-byte records) and returns its id.
RT3_NOINLINE uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied) {
    uint8_t tmp[0x2cc];
    NamedObj4611e0_Construct(tmp);
    uint32_t id = Collection_Allocate(t, 0, tmp, 0x2cb, 0, 0);
    Marker_Init((uint8_t*)Collection_Get(t, 0, id), 0, id, x, y, label, bodied);
    NamedObj4611e0_Destructor(tmp);
    return id;
}
RT3_REPLACE(0x4616d0, Markers_Add);

// 0x461740. Removes scene object `id` of collection c: tears it down and
// releases its slot.
void __fastcall SceneObjects_Remove(Collection* c, int, uint32_t id) {
    Thunk461220((uint8_t*)Collection_Get(c, 0, id));
    Collection_Release(c, 0, id);
}
RT3_REPLACE(0x461740, SceneObjects_Remove);

// 0x461770. Drops every object of the collection inside the cell rectangle to the
// ground.
void __fastcall Objs_DropAllInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (InCellRect(o, x0, y0, x1, y1)) Obj_DropToGround(o, 0, 0);
    }
}
RT3_REPLACE(0x461770, Objs_DropAllInRect);

// 0x461830. Renames the collection's elements (+0x2a2) to their variant for the
// current region set (0x6d3c84 -> column 2..6 of table 0x5f0fe8, 0x1c-byte rows
// {map key, name, variants...}) when the row's key matches the map name (its
// extension stripped; the tutorials count as "Italy").
RT3_NOINLINE void __fastcall Names_ApplyRegionVariants(Collection* c, int, char* mapName) {
    char key[0x1fc];
    StripExtension(mapName);
    StrCopy(key, mapName);
    if (!crt::stricmp(key, (const char*)0x5ce05c) || !crt::stricmp(key, (const char*)0x5ce050))
        memcpy(key, (const void*)0x5cc9d8, 6);
    int32_t col;
    switch (g_language) {
    case 2: col = 2; break;
    case 1: col = 3; break;
    case 3: col = 4; break;
    case 4: col = 5; break;
    case 0: col = 6; break;
    default: return;
    }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        for (const char* const* row = (const char* const*)0x5f0fe8; (uint32_t)row < 0x5f2ad0; row += 7) {
            if ((char)ToUpper(row[0][0]) != (char)ToUpper(key[0])) continue;
            if (crt::stricmp(row[0], key)) continue;
            if (crt::stricmp(row[1], (const char*)e + 0x2a2)) continue;
            StrCopyN((char*)e + 0x2a2, row[col], 0x27);
        }
    }
}
RT3_REPLACE(0x461830, Names_ApplyRegionVariants);

RT3_NOINLINE void __fastcall Collection5ce068_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce068;
    Collection_Destruct(c);
}
RT3_REPLACE(0x4619b0, Collection5ce068_Destruct);

// 0x4619c0. o's base value (+4) scaled by the effects 0x46 + kind (+0x10) and 0x45,
// and 0x160 + id (+0) when that is an effect, for company / player.
RT3_NOINLINE float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player) {
    float base = F<float>(o, 4);
    float m1 = Effects_Multiplier(World(), 0, F<int32_t>(o, 0x10) + 0x46, company, player, 0);
    float m2 = Effects_Multiplier(World(), 0, 0x45, company, player, 0);
    __asm {
        fld m2
        fmul m1
        fmul base
        fstp base
    }
    int32_t id = F<int32_t>(o, 0) + 0x160;
    if (id >= 0x160 && id <= 0x1c4)
        base = (float)((double)Effects_Multiplier(World(), 0, id, company, player, 0) * base);
    return base;
}
RT3_REPLACE(0x4619c0, Obj_EffectScale);

// 0x461a50. Adds record rec (fields a / b / c) to table t after resolving
// its names in table 0x62ba88 (+0x82): the main one (+0x34, id to +0x72,
// fatal string 0x1b5 when unknown) and the optional second one (+0x53, id to
// +0x76 and flag +0x7a, fatal string 0x1b6). The new record holds its id
// (+0) and the main entry links back to it (+0x126). QUIRK: when the main
// name is unknown the back link goes to the last entry scanned (or, for an
// empty table, through an unset local; here a null one).
uint32_t __fastcall Table_AddWith62ba88Refs(Collection* t, int, uint8_t* rec, uint32_t a, uint32_t b, uint32_t c) {
    uint8_t* main = nullptr;
    char frame[0x138];
    char* msg = frame + 0xc;
    bool found = false;
    for (int32_t i = 0; i < Collection_Count(g_carTypes); ++i) {
        Collection* refs = g_carTypes;
        main = (uint8_t*)Collection_Get(refs, 0, Collection_IdAt(refs, 0, (uint32_t)i, 0));
        if (crt::stricmp((const char*)rec + 0x34, (const char*)main + 0x82)) continue;
        F<uint32_t>(rec, 0x72) = Collection_IdAt(g_carTypes, 0, (uint32_t)i, 0);
        found = true;
        break;
    }
    if (!found) {
        FormatText(msg, GetString(0x1b5), (const char*)rec + 0x34, (const char*)rec + 0x34, 0, 0, 0, 0, 0, 0);
        Fatal_Exit(msg);
    }
    const char* second = (const char*)rec + 0x53;
    F<uint32_t>(rec, 0x76) = 0;
    rec[0x7a] = 0;
    if (strlen(second)) {
        found = false;
        for (int32_t i = 0; i < Collection_Count(g_carTypes); ++i) {
            Collection* refs = g_carTypes;
            uint8_t* e = (uint8_t*)Collection_Get(refs, 0, Collection_IdAt(refs, 0, (uint32_t)i, 0));
            if (crt::stricmp(second, (const char*)e + 0x82)) continue;
            F<uint32_t>(rec, 0x76) = Collection_IdAt(g_carTypes, 0, (uint32_t)i, 0);
            rec[0x7a] = 1;
            found = true;
            break;
        }
        if (!found) {
            FormatText(msg, GetString(0x1b6), second, (const char*)rec + 0x34, 0, 0, 0, 0, 0, 0);
            Fatal_Exit(msg);
        }
    }
    const uint32_t id = Collection_Allocate(t, 0, rec, a, b, c);
    F<uint32_t>(Collection_Get(t, 0, id), 0) = id;
    F<uint32_t>(main, 0x126) = id;
    return id;
}
RT3_REPLACE(0x461a50, Table_AddWith62ba88Refs);

// 0x461c00. ID of the record whose name at +0x34 or +0x53 equals `name`, or 0.
RT3_NOINLINE uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (crt::stricmp((const char*)r + 0x34, name) == 0) return Collection_IdAt(t, 0, i, 0);
        if (crt::stricmp((const char*)r + 0x53, name) == 0) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x461c00, Table_FindByAltName);

// 0x461c80. Address of +0x45 in the 0x62ba88 record referenced at +0x72.
RT3_NOINLINE void* __fastcall Obj_Ref72Plus45(void* o) {
    uint32_t id = F<uint32_t>(o, 0x72);
    if (!id) return nullptr;
    uint8_t* r = (uint8_t*)Collection_Get(g_carTypes, 0, id);
    return r ? r + 0x45 : nullptr;
}
RT3_REPLACE(0x461c80, Obj_Ref72Plus45);

// 0x461ca0. 0x51c920 on the name (+0x45) of the 0x62ba88 record +0x72; 0 if none.
RT3_NOINLINE uint32_t __fastcall Obj_RefName72(void* o) {
    uint32_t id = F<uint32_t>(o, 0x72);
    if (!id) return 0;
    uint8_t* r = (uint8_t*)Collection_Get(g_carTypes, 0, id);
    if (!r) return 0;
    return ((uint32_t(__fastcall*)(void*, int))&LocalizeName)(r + 0x45, 0);
}
RT3_REPLACE(0x461ca0, Obj_RefName72);

// 0x461cd0. The engine type closest to the current year: scored by the years from
// its introduction (+500 if not yet introduced), +50 if retired, +2000 if not
// available; kind 2 skipped. 1 if there are none.
RT3_NOINLINE uint32_t __fastcall EngineTypes_PickForYear(Collection* c) {
    uint32_t bestId = 1;
    int32_t best = 9999999;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x10) == 2) continue;
        auto ct = [&]() -> uint8_t* {
            return F<uint32_t>(e, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(e, 0x72))
                                        : nullptr;
        };
        int32_t intro = F<int32_t>(ct(), 0x105);
        int32_t year = F<uint16_t>(g_world, 0xd);
        int32_t score = abs(year - intro);
        if (intro > year) score += 0x1f4;
        if (F<int32_t>(ct(), 0x109) < (int32_t)F<uint16_t>(g_world, 0xd)) score += 0x32;
        if (!CarType_Available(ct(), 0, 0, 0)) score += 0x7d0;
        if (score < best) {
            best = score;
            bestId = Collection_IdAt(c, 0, i, 0);
        }
    }
    return bestId;
}
RT3_REPLACE(0x461cd0, EngineTypes_PickForYear);

// 0x461e00. Table t (0x6ada84): for each record whose kind flags +0x30..+0x32
// match the world's +0x4c97..+0x4c99, sets the named flag of its type (+0x45
// of record +0x72 in 0x62ba88); every record then takes that flag's value
// (+0x7b). Guarded against re-entry (0x6ada88); ends with the speed scale
// update.
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t) {
    if (!g_world || g_6ada88) return;
    g_6ada88 = 1;
    auto typeName = [](uint8_t* r) -> const char* {
        if (!F<uint32_t>(r, 0x72)) return nullptr;
        uint8_t* ty = (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(r, 0x72));
        return ty ? (const char*)ty + 0x45 : nullptr;
    };
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        uint8_t* w = g_world;
        if ((r[0x30] && w[0x4c97]) || (r[0x31] && w[0x4c98]) || (r[0x32] && w[0x4c99])) World_SetNamedFlag(g_world, 0, typeName(r), 1);
        F<int32_t>(r, 0x7b) = World_LookupNamed2(g_world, 0, typeName(r));
    }
    --g_6ada88;
    World_UpdateSpeedScale(g_world);
}
RT3_REPLACE(0x461e00, Table_RefreshNamedFlags);

// 0x461f10. Engine type table t: loads Data/EngineTypes/*.lco (0xad-byte
// record: name +0x34, stats +4..+0x33, version-0x7d5+ fields +0x83 < 9 and
// +0x87 < 10), the eight base engines (0x5ce07c..0x5ce0e4) in a second pass
// after the others, and skipped in multiplayer. Each engine's cargo (table
// 0x62ba88: the first of kind 3 whose name prefixes the engine's, copied to
// +0x53; then the first named exactly as the engine) adds its weight +0xfd
// to +0x24 and +0x7f. Ids up to 100 get the 0x1b3 / 0x1b4 named-flag names
// (0x616f7a / 0x619ae0, 0x6e-byte entries). Then refreshes the named flags.
// QUIRK: an engine named after a cargo of kind 3 gets its weight twice.
// QUIRK: the prefix test skips the cargo name's last letter.
RT3_NOINLINE void __fastcall EngineTypes_Load(Collection* t) {
    auto findFirst = ((void*(__stdcall*)(const char*, void*))imp::FindFirstFileA);
    auto findNext = imp::FindNextFileA;
    auto findClose = imp::FindClose;
    auto fread = [](void* p, size_t n, void* f) { crt::fread(p, 1, n, f); };
    static const uint32_t base[] = { 0x5ce0e4, 0x5ce0d8, 0x5ce0c8, 0x5ce0b8, 0x5ce0a8, 0x5ce098, 0x5ce088, 0x5ce07c };
    char fd[0x140];
    const char* file = fd + 0x2c;
    for (int32_t pass = 0; pass < 2; ++pass) {
        void* h = findFirst((const char*)0x5ce0ec, fd);
        if (h == (void*)-1) continue;
        do {
            int32_t special = 0;
            for (uint32_t b : base)
                if (!crt::stricmp(file, (const char*)b)) {
                    special = 1;
                    break;
                }
            if (special != pass) continue;
            char path[0x68];
            FormatText(path, (const char*)0x5c9870, file, 0, 0, 0, 0, 0, 0, 0);
            void* f = crt::fopen(path, (const char*)0x5c9678);
            if (!f) continue;
            uint8_t r[0xad];
            memset(r, 0, sizeof r);
            int32_t version;
            float weight;
            uint8_t skip[0x1f];
            fread(&version, 4, f);
            fread(r + 0x34, 0x1e, f);
            fread(r + 0x20, 4, f);
            fread(r + 0x24, 4, f);
            fread(r + 0x28, 4, f);
            fread(r + 0x2c, 4, f);
            fread(skip, 1, f);
            fread(r + 0x1c, 4, f);
            fread(r + 0x14, 4, f);
            fread(r + 0x18, 4, f);
            fread(r + 4, 4, f);
            fread(r + 0x10, 4, f);
            fread(&weight, 4, f);
            F<int32_t>(r, 0xc) = Ftol((double)weight + D(0x5c85b8));
            fread(r + 8, 4, f);
            fread(r + 0x30, 1, f);
            fread(r + 0x31, 1, f);
            fread(r + 0x32, 1, f);
            fread(r + 0x33, 1, f);
            if (version < 0x7d5) {
                fread(skip, 0x1f, f);
                fread(skip, 0x1f, f);
                F<int32_t>(r, 0x83) = 0;
                F<int32_t>(r, 0x87) = 0;
            } else {
                fread(r + 0x83, 4, f);
                fread(r + 0x87, 4, f);
                if (F<int32_t>(r, 0x83) < 0 || F<int32_t>(r, 0x83) >= 9) F<int32_t>(r, 0x83) = 0;
                if (F<int32_t>(r, 0x87) < 0 || F<int32_t>(r, 0x87) >= 10) F<int32_t>(r, 0x87) = 0;
            }
            r[0x53] = 0;
            for (int32_t i = 0; i < Collection_Count(g_carTypes); ++i) {
                Collection* c = g_carTypes;
                uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
                if (F<int32_t>(e, 0x41) != 3) continue;
                const char* n = (const char*)e + 0x82;
                // QUIRK: the length is one short (SUB/DEC after the strlen loop), so the
                // cargo's last letter is not compared (an empty name wraps to "all").
                if (crt::strnicmp(n, (const char*)r + 0x34, strlen(n) - 1)) continue;
                F<float>(r, 0x7f) = (float)((double)F<float>(r, 0x7f) + F<float>(e, 0xfd));
                F<float>(r, 0x24) = (float)((double)F<float>(r, 0x24) + F<float>(e, 0xfd));
                strcpy((char*)r + 0x53, n);
                break;
            }
            for (int32_t i = 0; i < Collection_Count(g_carTypes); ++i) {
                Collection* c = g_carTypes;
                uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
                if (crt::stricmp((const char*)r + 0x34, (const char*)e + 0x82)) continue;
                F<float>(r, 0x7f) = (float)((double)F<float>(r, 0x7f) + F<float>(e, 0xfd));
                F<float>(r, 0x24) = (float)((double)F<float>(r, 0x24) + F<float>(e, 0xfd));
                break;
            }
            if (!g_world || !IsMultiplayer() || !special) {
                uint32_t id = ((uint32_t(__fastcall*)(Collection*, int, void*, int32_t, int32_t, int32_t))(*(void***)t)[1])(t, 0, r, 0, 0, 0);
                Collection_Get(t, 0, id);
                if (id <= 100) {
                    char text[0x110];
                    for (int32_t k = 0; k < 2; ++k) {
                        const char* name = nullptr;
                        if (uint32_t c = F<uint32_t>(r, 0x72))
                            if (uint8_t* e = (uint8_t*)Collection_Get(g_carTypes, 0, c)) name = (const char*)e + 0x45;
                        FormatText(text, GetString(k ? 0x1b4 : 0x1b3), name, 0, 0, 0, 0, 0, 0, 0);
                        uint8_t* slot = (uint8_t*)(uintptr_t)((k ? 0x619ae0 : 0x616f7a) + id * 0x6e);
                        StrCopyN((char*)slot, text, 0x46);
                        F<uint32_t>(slot, 0x47) = id;
                    }
                }
            }
            crt::fclose(f);
        } while (findNext(h, fd));
        findClose(h);
    }
    Table_RefreshNamedFlags(t);
}
RT3_REPLACE(0x461f10, EngineTypes_Load);

}  // namespace rt3
