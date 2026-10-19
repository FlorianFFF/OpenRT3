// Stream table (0x5309d0..0x531380). QUIRKs of the original are kept.
#include "stream.h"
#include "crt.h"
#include "hook/hook.h"
#include <string.h>
#include <stdio.h>
#include <intrin.h>

namespace rt3 {

const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);

static StreamRecord* const g_streams = reinterpret_cast<StreamRecord*>(0xccb880);
static const int kStreams = 10;
static int32_t& g_streamsReady = hook::Global<int32_t>(0xccb9ec);
static int32_t& g_skipCount = hook::Global<int32_t>(0xccb9e8);

static const int kOpenRead = 0x8000;           // _O_RDONLY | _O_BINARY
static const int kOpenWrite = 0x8301;          // _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY
static const int kChunk = 200000;

static inline void FatalError(const char* msg) { ((void(__fastcall*)(const char*))0x51e980)(msg); }

static void FatalCannotOpen(const char* path) {
    char msg[0x180];
    FormatText(msg, GetString(0xf1b), path, 0, 0, 0, 0, 0, 0, 0);
    FatalError(msg);
}

void StreamInit() {
    if (g_streamsReady == 0) {
        memset(g_streams, 0, sizeof(StreamRecord) * kStreams);
        g_streamsReady = 1;
    }
}
RT3_REPLACE(0x5309d0, StreamInit);

void __fastcall StreamClose(Stream s) {
    StreamRecord& r = g_streams[s];
    switch (r.mode) {
    case kStreamWriteFile:
    case kStreamReadFile:
        Close(r.fd);
        r.allocated = 0;
        break;
    case kStreamWriteBuffered: {
        int fd = Open(r.path, kOpenWrite, 0x80);
        if (fd == -1) {
            r.open = 0;
            r.allocated = 0;
            Close(-1);
            return; // QUIRK: path and buffer are leaked
        }
        Write(fd, r.buffer, r.size);
        Close(fd);
        Free(r.buffer); // QUIRK: pointer not cleared
        r.allocated = 0;
        break;
    }
    case kStreamWriteMemory:
        if (r.size < r.capacity) { // shrink to fit; stays allocated for mode-3 re-open
            uint8_t* b = (uint8_t*)Malloc(r.size);
            memcpy(b, r.buffer, r.size);
            Free(r.buffer);
            r.buffer = b;
            r.capacity = r.size;
        }
        break;
    case kStreamReadMemory:
        break;
    case kStreamReadWhole:
        FreeAndNull((void**)&r.buffer);
        r.capacity = 0;
        r.allocated = 0;
        break;
    case kStreamReadChunked:
        FreeAndNull((void**)&r.buffer);
        r.capacity = 0;
        r.allocated = 0;
        Close(r.fd);
        break;
    }
    if (r.allocated == 0) FreeAndNull((void**)&r.path);
    r.open = 0;
}
RT3_REPLACE(0x530a00, StreamClose);

int32_t __fastcall StreamTell(Stream s) { return Tell(g_streams[s].fd); }
RT3_REPLACE(0x530b70, StreamTell);

int32_t __fastcall StreamFd(Stream s) { return g_streams[s].fd; }
RT3_REPLACE(0x530b90, StreamFd);

void __fastcall StreamRelease(Stream s) {
    StreamRecord& r = g_streams[s];
    if (r.open) StreamClose(s);
    if (r.allocated) {
        Free(r.path);
        if (r.buffer) Free(r.buffer);
        r.allocated = 0;
        r.path = nullptr;
        r.buffer = nullptr;
    }
}
RT3_REPLACE(0x530ba0, StreamRelease);

int32_t __fastcall StreamSize(Stream s) { return g_streams[s].size; }
RT3_REPLACE(0x530c10, StreamSize);

int32_t __fastcall FileExists(const char* path) {
    int fd = Open(path, kOpenRead, 0);
    if (fd == -1) return 0;
    Close(fd);
    return 1;
}
RT3_REPLACE(0x530c20, FileExists);

void StreamReleaseAll() {
    if (g_streamsReady == 0) return;
    for (int i = 0; i < kStreams; ++i) StreamRelease(i);
    g_streamsReady = 0;
}
RT3_REPLACE(0x530c50, StreamReleaseAll);

Stream __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing) {
    StreamInit();
    int k = -1;
    if (mode == kStreamReadMemory || mode == kStreamWriteMemory) {
        for (int i = 0; i < kStreams; ++i) {
            StreamRecord& r = g_streams[i];
            if (r.allocated && (r.mode == kStreamReadMemory || r.mode == kStreamWriteMemory) &&
                Stricmp(r.path, path) == 0) {
                if (mode != kStreamWriteMemory) { // re-open the kept buffer for reading
                    r.open = 1;
                    r.pos = 0;
                    return i;
                }
                StreamRelease(i); // rewrite: reuse this slot
                k = i;
                break;
            }
        }
    }
    if (k < 0) {
        if (mode == kStreamReadMemory) FatalCannotOpen(path); // no such buffer
        for (int i = 0; i < kStreams; ++i)
            if (!g_streams[i].allocated) { k = i; break; }
        if (k < 0) return 0; // QUIRK: 0 is also a valid handle
    }

    StreamRecord& r = g_streams[k];
    r.size = 0;
    r.pos = 0;
    r.mode = mode;
    r.open = 1;
    r.allocated = 1;
    size_t n = strlen(path) + 1;
    r.path = (char*)Malloc(n);
    r.capacity = 0;
    r.buffer = nullptr;
    memcpy(r.path, path, n);

    if (r.mode == kStreamReadFile || r.mode == kStreamReadWhole || r.mode == kStreamReadChunked) {
        r.fd = Open(path, kOpenRead, 0x100);
        if (r.fd == -1) {
            if (fatalIfMissing) FatalCannotOpen(path);
            FreeAndNull((void**)&r.path);
            r.open = 0;
            r.allocated = 0;
            return -1;
        }
        if (r.mode == kStreamReadWhole) {
            r.capacity = r.size = FileLength(r.fd);
            r.buffer = (uint8_t*)Malloc(r.capacity);
            Read(r.fd, r.buffer, r.capacity);
            Close(r.fd);
        } else if (r.mode == kStreamReadChunked) {
            if (FileLength(r.fd) < kChunk) {
                r.capacity = r.size = FileLength(r.fd);
                r.mode = kStreamReadWhole;
            } else {
                r.capacity = r.size = kChunk;
            }
            r.buffer = (uint8_t*)Malloc(r.capacity);
            Read(r.fd, r.buffer, r.capacity);
            if (r.mode == kStreamReadWhole) Close(r.fd);
        }
        return k;
    }
    if (r.mode == kStreamWriteFile) {
        r.fd = Open(path, kOpenWrite, 0x80);
        if (r.fd != -1) return k;
        r.allocated = 0;
        r.open = 0;
        Close(-1);
        return -1;
    }
    // Memory modes: check the file can be created (QUIRK: this truncates it).
    Stream probe = StreamOpen(path, kStreamWriteFile, kChunk, 1);
    if (probe == -1) return -1; // QUIRK: slot k stays taken
    StreamClose(probe);
    r.buffer = (uint8_t*)Malloc(bufferSize);
    r.capacity = bufferSize;
    return k;
}
RT3_REPLACE(0x530c80, StreamOpen);

// Save tracing for the determinism harness: when a trace file is set, every
// write logs its caller, size and CRC, so two runs' saves can be compared
// write by write (which subsystem diverged first).
FILE* g_writeTrace;
int g_traceMaskPointer; // set around writes whose first 4 bytes are a heap pointer
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);

void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t) {
    if (g_writeTrace) {
        uint32_t skip = (g_traceMaskPointer && size >= 4) ? 4 : 0;
        fprintf(g_writeTrace, "%08x %u %08x\n", (unsigned)(uintptr_t)_ReturnAddress(), size,
                Crc32((const uint8_t*)data + skip, size - skip));
    }
    StreamRecord& r = g_streams[s];
    if (r.mode == kStreamWriteMemory || r.mode == kStreamWriteBuffered) {
        while (r.capacity < (int32_t)(r.size + size)) {
            int32_t grow = r.capacity / 3 > 99999 ? r.capacity / 3 : 100000;
            uint8_t* b = (uint8_t*)Malloc(r.capacity + grow);
            memcpy(b, r.buffer, r.size);
            Free(r.buffer);
            r.buffer = b;
            r.capacity += grow;
        }
        memcpy(r.buffer + r.pos, data, size);
        r.pos += size;
        if (r.pos > r.size) r.size = r.pos;
    } else if (r.mode == kStreamWriteFile) {
        Write(r.fd, data, size);
    }
}
RT3_REPLACE(0x531030, StreamWrite);

int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t) {
    StreamRecord& r = g_streams[s];
    uint8_t* out = (uint8_t*)dst;
    if (r.mode == kStreamReadFile) return Read(r.fd, dst, size);
    if (r.mode != kStreamReadChunked) {
        int32_t n = r.size - r.pos;
        if ((int32_t)size < n) n = size;
        memcpy(out, r.buffer + r.pos, n);
        r.pos += n;
        return n;
    }
    int32_t avail = kChunk - r.pos; // QUIRK: ignores the real file size; past the end reads zeros
    if (avail < (int32_t)size) {
        memcpy(out, r.buffer + r.pos, avail);
        memset(r.buffer, 0, kChunk);
        Read(r.fd, r.buffer, kChunk);
        int32_t rest = size - avail;
        r.pos = 0;
        if (rest > kChunk) {
            int32_t chunks = (rest - (kChunk + 1)) / kChunk + 1;
            rest -= chunks * kChunk;
            do {
                memcpy(out + avail, r.buffer, kChunk);
                memset(r.buffer, 0, kChunk);
                avail += kChunk;
                Read(r.fd, r.buffer, kChunk);
            } while (--chunks);
        }
        memcpy(out + avail, r.buffer, rest);
        r.pos += rest;
        return size;
    }
    memcpy(out, r.buffer + r.pos, size);
    r.pos += size;
    return size;
}
RT3_REPLACE(0x531150, StreamRead);

void __fastcall StreamWriteU32(Stream s, uint32_t value) { StreamWrite(s, &value, 4, 0); }
RT3_REPLACE(0x531340, StreamWriteU32);

// Reads 4 bytes and drops them; counts the skip (0xccb9e8) when asked.
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count) {
    if (count) g_skipCount++;
    uint32_t junk;
    return StreamRead(s, &junk, 4, 0, 1);
}
RT3_REPLACE(0x531360, StreamSkip4);

// Length-prefixed string: 1 byte length, or 2 bytes (high bit set) for 128..32767.
// The stored length includes the terminator. *out = NULL for length 0.
int32_t __fastcall StreamReadString(Stream s, char** out) {
    if (*out) Free(*out);
    *out = nullptr;
    uint8_t len;
    int32_t total = StreamRead(s, &len, 1, 0, 1);
    if (len == 0) return total;
    uint32_t size;
    if (len < 0x80) {
        size = len;
    } else {
        uint8_t low;
        size = (uint32_t)(len & 0x7f) << 8;
        total += StreamRead(s, &low, 1, 0, 1);
        size |= low;
    }
    *out = (char*)Malloc(size);
    return StreamRead(s, *out, size, 0, 1) + total;
}
RT3_REPLACE(0x531380, StreamReadString);

} // namespace rt3
