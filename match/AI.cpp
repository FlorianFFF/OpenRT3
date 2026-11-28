// Byte-matching sources (phase 2) for the start of the exe (src/game/AI.cpp).
// Written for Visual C++ .NET 2002 (C++98, real __thiscall members); scored
// by tools/matchbuild.py against the exe. Behaviour is the same as the
// rewrite's functions named in each comment.
#include "types.h"

// 0x401000 City::SizeClass
// (Obj_SizeClass) Size class of the demand +0x30e: <= 5 -> 0, <= 13 -> 1,
// else 2, written to both outputs.
void City::SizeClass(int* a, int* b) {
    if (demand30e <= 5.0f) {
        *b = 0;
        *a = 0;
    } else if (demand30e <= 13.0f) {
        *b = 1;
        *a = 1;
    } else {
        *b = 2;
        *a = 2;
    }
}

// 0x401070 Record296::Rating
// (Obj_Rating296) Rating by +0x296: 9.4 / 8.4 / 7.4 / 6.4 from 85 / 70 / 50.
float Record296::Rating() {
    int v = value296;
    if (v >= 85) return 9.4f;
    if (v >= 70) return 8.4f;
    if (v >= 50) return 7.4f;
    return 6.4f;
}

// 0x4010b0 Collection::GetAt
// (Collection_GetAt) The record at position pos of list `list`.
void* Collection::GetAt(unsigned pos, int list) { return Get(IdAt(pos, list)); }

// 0x4010d0 0x406bf0 Collection::RecordAt
// (Collection_RecordAt / Collection_RecordAt2) The record at position pos of list 0.
void* Collection::RecordAt(unsigned pos) { return Get(IdAt(pos, 0)); }
