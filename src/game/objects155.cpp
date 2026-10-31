// Text tag encoding / decoding in block 0x42 (batch 159). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

int __fastcall ToUpper(char c);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all);
const char* __fastcall GetString(int32_t id);

#define SPRINTF ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)

// 0x42d820. Converts the [tags] of text in place (max length maxLen): with `encode`,
// names from the tag table (0x5f3d9c, 901 x 0x81 bytes) become their number
// (10000 + index) and the six ~~name] markers (strings 0x620b6c) become ~~2000x];
// otherwise the reverse. Quirk: a line break inside a tag restarts the scan one
// character after the tag's start.
void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode) {
    if (!*text) return;
    int32_t found = 0;
    char* p = text;
    for (;;) {
        bool replaced = false;
        while (*p == '[') {
            ++p;
            char* tag = p;
            char c;
            while ((c = *p) != 0 && c != ']' && c != '~' && c != '\n' && c != '\r') ++p;
            if (c == '\n' || c == '\r') {
                p = tag + 1;
                if (tag[1] == '[') continue;
                break;
            }
            found = 1;
            char first = (char)ToUpper(*tag);
            char term = *p;
            *p = 0;
            char buf[0x124];
            strcpy(buf, tag);
            if (encode) {
                int32_t idx = 0;
                for (const char* e = (const char*)0x5f3d9c; (uint32_t)e < 0x6103a1; e += 0x81, ++idx)
                    if ((char)ToUpper(*e) == first && !Stricmp(tag, e)) SPRINTF(buf, (const char*)0x5c9cd0, idx + 10000);
            } else {
                int32_t v = ((int(__cdecl*)(const char*))0x5a1ea5)(tag) - 10000;
                if (v >= 0 && v < 0x385) SPRINTF(buf, (const char*)0x5f3d9c + v * 0x81);
            }
            *p = term;
            int32_t start = (int32_t)(tag - text);
            int32_t blen = (int32_t)strlen(buf);
            char big[0x7c0];
            strcpy(big, text);
            strcpy(big + start, buf);
            int32_t end = blen + start;
            strcpy(big + end, p);
            int32_t n = (int32_t)strlen(big);
            if (n <= maxLen) n = maxLen;
            StrCopyN(text, big, n);
            p = text + end;
            replaced = true;
            break;
        }
        if (!replaced) ++p;
        if (!*p) break;
    }
    if (!found) return;
    for (int32_t i = 0; i < 6; ++i) {
        char num[0x134], name[0x124];
        SPRINTF(num, (const char*)0x5c9cdc, i + 20000);
        SPRINTF(name, (const char*)0x5c9cd4, GetString(((const int32_t*)0x620b6c)[i]));
        if (encode) Str_Replace(text, maxLen, name, num, 1);
        else Str_Replace(text, maxLen, num, name, 1);
    }
}
RT3_REPLACE(0x42d820, Text_ConvertTags);

} // namespace rt3
