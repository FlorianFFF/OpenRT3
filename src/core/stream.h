// File/memory streams (0x5309d0..0x531380): what every Save/Load method reads
// and writes. A stream is a small integer handle into a table of 10 records.
#pragma once
#include <stdint.h>

namespace rt3 {

typedef int32_t Stream; // handle 0..9

enum StreamMode : int32_t {
    kStreamWriteFile   = 0, // straight to a file descriptor
    kStreamReadFile    = 1, // straight from a file descriptor
    kStreamWriteMemory = 2, // growing memory buffer, kept after close (re-openable with mode 3)
    kStreamReadMemory  = 3, // re-open a mode-2 buffer by name for reading
    kStreamWriteBuffered = 4, // growing memory buffer, written to the file on close
    kStreamReadWhole   = 5, // whole file read into memory at open
    kStreamReadChunked = 6, // file read through a 200,000-byte window (becomes 5 if smaller)
};

struct StreamRecord {         // 36 bytes, table at 0xccb880
    int32_t  open;            // 0x00
    int32_t  allocated;       // 0x04: path/buffers owned
    int32_t  fd;              // 0x08
    int32_t  mode;            // 0x0c
    int32_t  capacity;        // 0x10
    int32_t  size;            // 0x14
    int32_t  pos;             // 0x18
    uint8_t* buffer;          // 0x1c
    char*    path;            // 0x20
};
static_assert(sizeof(StreamRecord) == 36, "StreamRecord layout");

void    StreamInit();                                                                   // 0x5309d0
void    __fastcall StreamClose(Stream s);                                               // 0x530a00
int32_t __fastcall StreamTell(Stream s);                                                // 0x530b70
int32_t __fastcall StreamFd(Stream s);                                                  // 0x530b90
void    __fastcall StreamRelease(Stream s);                                             // 0x530ba0
int32_t __fastcall StreamSize(Stream s);                                                // 0x530c10
int32_t __fastcall FileExists(const char* path);                                        // 0x530c20
void    StreamReleaseAll();                                                             // 0x530c50
Stream  __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing); // 0x530c80
void    __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t unused);              // 0x531030
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t unused1, int32_t unused2);    // 0x531150
void    __fastcall StreamWriteU32(Stream s, uint32_t value);                            // 0x531340
int32_t __fastcall StreamSkip4(Stream s, int, int32_t unused, int32_t count);           // 0x531360
int32_t __fastcall StreamReadString(Stream s, char** out);                              // 0x531380

} // namespace rt3
