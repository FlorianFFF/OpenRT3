#include "crt.h"
#include "hook/hook.h"

namespace rt3 {

void __fastcall FreeAndNull(void** p) {
    if (*p) {
        Free(*p);
        *p = nullptr;
    }
}
RT3_REPLACE(0x53afd0, FreeAndNull);

} // namespace rt3
