// Music playback / sound timer / window child order / widget subtypes / edit text in blocks 0x4e..0x55 (batch 111). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Widget_SetVtbl(uint8_t* w);
void __fastcall Music_Close(uint8_t* m);
int32_t __fastcall Music_IsPlaying(uint8_t* m);
int32_t __fastcall Music_IsPaused(uint8_t* m);
void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
void __fastcall Sound_ReleaseTimer(uint8_t* s);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
void __fastcall Widget_Attach(uint8_t* w, int, uint8_t* parent);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline void MemMove(void* d, const void* s, size_t n) { ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(d, s, n); }

// 0x4ea730. Title screen destructor: closes the menu music (0x6cfd34).
void __fastcall TitleScreen_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1194;
    hook::Global<int32_t>(0x6d10b0) = 0;
    if (uint8_t* m = hook::Global<uint8_t*>(0x6cfd34)) {
        Music_Close(m);
        ExeDelete(m);
    }
    hook::Global<void*>(0x6cfd34) = nullptr;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4ea730, TitleScreen_Destruct);

// 0x531520. Stops the game music (0xccba00) when it is playing or paused.
void __fastcall Sound_StopMusic(uint8_t* s, int, uint32_t notify) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return;
    uint8_t* m = hook::Global<uint8_t*>(0xccba00);
    if (Music_IsPlaying(m) == 1 || Music_IsPaused(hook::Global<uint8_t*>(0xccba00)) == 1)
        Music_Stop(hook::Global<uint8_t*>(0xccba00), 0, notify);
}
RT3_REPLACE(0x531520, Sound_StopMusic);

// 0x532260. (Re)starts the music fade timer (+0x4c, mode +0x50: 0 fades in from
// silence) via the Miles timer API.
void __fastcall Sound_StartFadeTimer(uint8_t* s, int, int32_t mode) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return;
    if (F<int32_t>(s, 0x4c) != -1) {
        if (mode == F<int32_t>(s, 0x50)) return;
        hook::Global<int32_t>(0x624d58) = 1;
        Sound_ReleaseTimer(s);
    }
    int32_t t = Import<int32_t(__stdcall*)(uint32_t)>(0x5c83f4)(0x531fd0);
    F<int32_t>(s, 0x4c) = t;
    if (t == -1) return;
    F<int32_t>(s, 0x50) = mode;
    hook::Global<uint32_t>(0x624d60) = 0xbf800000;
    uint32_t user = 1;
    if (!mode) {
        Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, 0.0f);
        t = F<int32_t>(s, 0x4c);
        user = 2;
    }
    Import<void(__stdcall*)(int32_t, uint32_t)>(0x5c8400)(t, user);
    Import<void(__stdcall*)(int32_t, uint32_t)>(0x5c83c8)(F<int32_t>(s, 0x4c), 3);
    hook::Global<int32_t>(0x624d58) = 0;
    Import<void(__stdcall*)(int32_t)>(0x5c83a0)(F<int32_t>(s, 0x4c));
}
RT3_REPLACE(0x532260, Sound_StartFadeTimer);

// 0x53f9c0. Adds a child widget: ordered by layer (+8), or right after the child
// with id `afterId`; doubly linked through +0x8a / +0x8e (head +0x70, tail +0x74).
RT3_NOINLINE void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId) {
    Widget_Attach(w, 0, win);
    F<uint8_t*>(w, 0x8a) = nullptr;
    uint8_t* c = F<uint8_t*>(win, 0x70);
    if (c) {
        int8_t layer = (int8_t)w[8];
        do {
            if ((int8_t)c[8] > layer) break;
            int32_t id = F<uint16_t>(c, 4);
            F<uint8_t*>(w, 0x8a) = c;
            c = F<uint8_t*>(c, 0x8e);
            if (afterId && afterId == id) break;
        } while (c);
    }
    if (c == F<uint8_t*>(win, 0x70)) F<uint8_t*>(win, 0x70) = w;
    if (!c) F<uint8_t*>(win, 0x74) = w;
    F<uint8_t*>(w, 0x8e) = c;
    if (c) F<uint8_t*>(c, 0x8a) = w;
    if (uint8_t* prev = F<uint8_t*>(w, 0x8a)) F<uint8_t*>(prev, 0x8e) = w;
}
RT3_REPLACE(0x53f9c0, Window_AddChild);

// 0x557e50. Opens and starts a music stream (file +4, copied from `file` when
// given) at the stored volume (+0x104 > 0 only); `onEnd` becomes the end callback.
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd) {
    if (!Sound_Is3D(hook::Global<uint8_t*>(0x6d402c))) return 0;
    if (!(*(const float*)0x5c8598 < F<float>(m, 0x104))) return 0;
    char* name = (char*)m + 4;
    if (!file) {
        if (!*name) return 0;
    } else {
        for (size_t k = 0; (name[k] = file[k]) != 0; ++k) {}
    }
    Music_Stop(m, 0, 0);
    void* s = Import<void*(__stdcall*)(void*, const char*, uint32_t)>(0x5c83d0)(digital, name, 0x80000);
    F<void*>(m, 0) = s;
    if (!s) return 0;
    Music_SetVolume(m, 0, F<float>(m, 0x104));
    for (int i = 0; i < 10; ++i)
        if (!((void**)0xd973b8)[i]) {
            ((void**)0xd973b8)[i] = F<void*>(m, 0);
            break;
        }
    Import<void(__stdcall*)(void*)>(0x5c83cc)(F<void*>(m, 0));
    auto reg = Import<void(__stdcall*)(void*, uint32_t)>(0x5c83c4);
    if (onEnd) {
        hook::Global<void*>(0xd973e0) = onEnd;
        reg(F<void*>(m, 0), 0x557d40);
    } else {
        reg(F<void*>(m, 0), 0);
    }
    return 1;
}
RT3_REPLACE(0x557e50, Music_Play);

// 0x558530. Sends a 48-byte message {a, b, c, d, e, 0...} to the widget (vtbl +4).
RT3_NOINLINE int32_t __fastcall Widget_Send(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    uint32_t msg[12] = { a, b, c, d, e };
    return ((int32_t(__thiscall*)(void*, void*))(*(void***)w)[1])(w, msg);
}
RT3_REPLACE(0x558530, Widget_Send);

// 0x559340. Scrollbar widget defaults (type 4).
void __fastcall Scrollbar_Init(uint8_t* w) {
    F<uint16_t>(w, 6) = 4;
    F<uint16_t>(w, 0xae) = 0;
    F<int32_t>(w, 0xb0) = 0;
    F<int32_t>(w, 0x10d) = 0;
    F<int32_t>(w, 0xb4) = 0;
    F<int32_t>(w, 0xb8) = -1;
    F<int32_t>(w, 0xf8) = 0;
    F<int32_t>(w, 0xfc) = 0;
    F<int32_t>(w, 0xec) = 0;
    F<int32_t>(w, 0xf0) = 0;
    F<int32_t>(w, 0xf4) = 0;
    F<uint32_t>(w, 0x100) = 0x3f800000;
    F<uint32_t>(w, 0x104) = 0x3f800000;
    F<uint16_t>(w, 0xbc) = 0;
    F<uint16_t>(w, 0xbe) = 0;
    F<uint16_t>(w, 0xc0) = 0;
    F<uint16_t>(w, 0xc2) = 0;
    F<uint32_t>(w, 0x108) = 0x3f800000;
    F<int32_t>(w, 0xa2) = 0;
    F<int32_t>(w, 0xa6) = 0;
    F<int32_t>(w, 0xaa) = 0;
    w[0x10c] = 0xff;
}
RT3_REPLACE(0x559340, Scrollbar_Init);

void __fastcall Widget_SetClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    F<uint32_t>(w, 0x33) |= 0x40000;
    F<uint32_t>(w, 0xd8) = a;
    F<uint32_t>(w, 0xdc) = b;
    F<uint32_t>(w, 0xe0) = c;
    F<uint32_t>(w, 0xe4) = d;
}
RT3_REPLACE(0x559f50, Widget_SetClip);

// 0x55a560. List box defaults (type 0x40).
void __fastcall ListBox_Init(uint8_t* w) {
    static const uint16_t zero[] = { 0x101, 0x105, 0x109, 0x10d, 0x111, 0x185, 0x141, 0x145, 0x149, 0x14d, 0x151, 0x155,
                                     0x189, 0x18d, 0x191, 0x195, 0x199, 0x159, 0x15d, 0x161, 0x165, 0x169, 0x16d, 0x115,
                                     0x119, 0x19d, 0x1a1, 0x1a5, 0x1ad, 0x1b1, 0x121, 0x125, 0x11d, 0x1b5 };
    F<uint16_t>(w, 6) = 0x40;
    for (uint16_t off : zero) F<int32_t>(w, off) = 0;
}
RT3_REPLACE(0x55a560, ListBox_Init);
void __fastcall ListBox_SetSelection(uint8_t* w, int, uint32_t v) {
    if (v != F<uint32_t>(w, 0x101)) F<uint32_t>(w, 0x101) = v;
}
RT3_REPLACE(0x55a640, ListBox_SetSelection);

// 0x55b2d0. Edit box defaults (type 2).
void __fastcall EditBox_Init(uint8_t* w) {
    static const uint8_t zero[] = { 0xba, 0xa2, 0xa6, 0xaa, 0xae, 0xb2, 0xb6, 0xe4, 0xd8, 0xdc, 0xe0, 0xe8, 0xd4 };
    F<uint16_t>(w, 6) = 2;
    for (uint8_t off : zero) F<int32_t>(w, off) = 0;
    F<uint16_t>(w, 0xbe) = 0;
}
RT3_REPLACE(0x55b2d0, EditBox_Init);

// 0x55b6b0. Deletes the character at pos (a whole [..] tag in tag mode, flag
// 0x4000000) and moves the cursor (+0xd8) there.
void __fastcall EditBox_DeleteAt(uint8_t* w, int, char* s, int32_t pos) {
    int32_t len = 0;
    while (s[len]) ++len;
    if (pos < 0 || pos >= len) return;
    int32_t from = pos, to = pos;
    if (F<uint32_t>(w, 0x33) & 0x4000000) {
        char c = s[to];
        if (c == ']' && to > 0)
            while (s[from] != '[') {
                if (--from <= 0) break;
            }
        if (c == '[' && to < len - 1)
            while (s[to] != ']') {
                if (++to >= len - 1) break;
            }
    }
    if (to < len - 1) MemMove(s + from, s + to + 1, len - to - 1);
    s[from - to + len - 1] = 0;
    F<int32_t>(w, 0xd8) = from;
}
RT3_REPLACE(0x55b6b0, EditBox_DeleteAt);

// 0x55bd80. Rebuilds the display text (+0xba, 5000 bytes) from `text` with the
// cursor character inserted at pos ('|', or 0x0f / ' ' for custom fonts +0xe4).
void __fastcall EditBox_InsertCursor(uint8_t* w, int, const char* text, int32_t pos) {
    char* buf = (char*)Malloc(0x1388);
    const char* old = F<const char*>(w, 0xba);
    for (size_t k = 0; (buf[k] = old[k]) != 0; ++k) {}
    Free(F<void*>(w, 0xba));
    F<char*>(w, 0xba) = buf;
    if (pos) MemMove(buf, text, pos);
    char cursor;
    if (F<int32_t>(w, 0xe4)) cursor = F<int32_t>(F<uint8_t*>(w, 0x63), 0x10) ? 0x0f : 0x20;
    else cursor = 0x7c;
    F<char*>(w, 0xba)[pos] = cursor;
    const char* rest = text + pos;
    size_t n = 0;
    while (rest[n]) ++n;
    MemMove(F<char*>(w, 0xba) + pos + 1, rest, n + 1);
}
RT3_REPLACE(0x55bd80, EditBox_InsertCursor);

} // namespace rt3
