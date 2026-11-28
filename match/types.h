// Types for the byte-matching sources (phase 2), in Visual C++ .NET 2002's
// C++: the game's objects as classes with their known fields at their
// offsets (packed, as the original's records are) and __thiscall members.
// Grows with each matched function; the layouts follow src/ (F<T>(p, off)).
#ifndef RT3_MATCH_TYPES_H
#define RT3_MATCH_TYPES_H

#pragma pack(push, 1)

// The game's id-indexed record collections (src/engine/CollectionCore.cpp).
class Collection {
public:
    void* Get(unsigned id);                    // 0x517da0 Collection_Get
    unsigned IdAt(unsigned pos, int list);     // 0x518380 Collection_IdAt
    void* GetAt(unsigned pos, int list);
    void* RecordAt(unsigned pos);
};

class City {
public:
    char pad0[0x30e];
    float demand30e;
    void SizeClass(int* a, int* b);
};

// A record rated by its +0x296 (Obj_Rating296).
class Record296 {
public:
    char pad0[0x296];
    int value296;
    float Rating();
};

#pragma pack(pop)

#endif
