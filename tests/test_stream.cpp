// Stream difftests. Original and rewrite share the global stream table, so each
// scenario runs once with the original functions and once with ours, on its own
// files, and the observable results are compared.
#include "difftest.h"
#include "core/stream.h"
#include "game/collection.h"
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using namespace rt3;

namespace {

struct Api {
    Stream  (__fastcall* open)(const char*, int32_t, int32_t, int32_t);
    void    (__fastcall* close)(Stream);
    void    (__fastcall* release)(Stream);
    void    (__fastcall* releaseAll)();
    void    (__fastcall* write)(Stream, const void*, uint32_t, int32_t);
    int32_t (__fastcall* read)(Stream, void*, uint32_t, int32_t, int32_t);
    void    (__fastcall* writeU32)(Stream, uint32_t);
    int32_t (__fastcall* skip4)(Stream, int, int32_t, int32_t);
    int32_t (__fastcall* readString)(Stream, char**);
    int32_t (__fastcall* size)(Stream);
    int32_t (__fastcall* exists)(const char*);
    void    (__fastcall* save)(Collection*, int, Stream, int32_t);
    int32_t (__fastcall* load)(Collection*, int, Stream, int32_t);
};

static const Api kOrig = {
    (Stream(__fastcall*)(const char*, int32_t, int32_t, int32_t))0x530c80,
    (void(__fastcall*)(Stream))0x530a00, (void(__fastcall*)(Stream))0x530ba0, (void(__fastcall*)())0x530c50,
    (void(__fastcall*)(Stream, const void*, uint32_t, int32_t))0x531030,
    (int32_t(__fastcall*)(Stream, void*, uint32_t, int32_t, int32_t))0x531150,
    (void(__fastcall*)(Stream, uint32_t))0x531340, (int32_t(__fastcall*)(Stream, int, int32_t, int32_t))0x531360,
    (int32_t(__fastcall*)(Stream, char**))0x531380, (int32_t(__fastcall*)(Stream))0x530c10,
    (int32_t(__fastcall*)(const char*))0x530c20,
    (void(__fastcall*)(Collection*, int, Stream, int32_t))0x517d90,
    (int32_t(__fastcall*)(Collection*, int, Stream, int32_t))0x518680,
};
static void __fastcall NewReleaseAll() { StreamReleaseAll(); }
static const Api kNew = {
    StreamOpen, StreamClose, StreamRelease, NewReleaseAll, StreamWrite, StreamRead, StreamWriteU32,
    StreamSkip4, StreamReadString, StreamSize, FileExists, Collection_Save, Collection_Load,
};

// Everything a scenario produced, compared between the two runs.
struct Trace {
    std::vector<int64_t> values;
    std::vector<uint8_t> bytes;
    void V(int64_t v) { values.push_back(v); }
    void B(const void* p, size_t n) { bytes.insert(bytes.end(), (const uint8_t*)p, (const uint8_t*)p + n); }
    void Record(Stream s) {
        if (s < 0 || s > 9) { V(-999); return; }
        const StreamRecord& r = ((const StreamRecord*)0xccb880)[s];
        V(r.open); V(r.allocated); V(r.mode); V(r.capacity); V(r.size); V(r.pos);
        V(r.buffer != nullptr); V(r.path != nullptr);
    }
    void File(const std::string& path) {
        FILE* f = fopen(path.c_str(), "rb");
        if (!f) { V(-1); return; }
        std::vector<uint8_t> d(1 << 20);
        size_t n = fread(d.data(), 1, d.size(), f);
        fclose(f);
        V((int64_t)n);
        B(d.data(), n);
    }
};

static std::string g_dir;
static std::string Path(const char* run, const char* name) { return g_dir + run + "_" + name; }

static std::vector<uint8_t> Data(int seed, size_t n) {
    std::vector<uint8_t> d(n);
    srand(seed);
    for (auto& b : d) b = (uint8_t)rand();
    return d;
}

static void Scenario(const Api& a, const char* run, Trace& t) {
    // Mode 0: straight to file.
    std::string p0 = Path(run, "m0.bin");
    Stream s = a.open(p0.c_str(), kStreamWriteFile, 0, 0);
    t.V(s); t.Record(s);
    auto d = Data(1, 5000);
    for (size_t off = 0; off < d.size(); off += 777) a.write(s, d.data() + off, (uint32_t)min((size_t)777, d.size() - off), 0);
    a.writeU32(s, 0xdeadbeef);
    a.close(s); t.Record(s); t.File(p0);

    // Mode 4: buffered, grows past 100000, written on close.
    std::string p4 = Path(run, "m4.bin");
    s = a.open(p4.c_str(), kStreamWriteBuffered, 1000, 0);
    t.V(s); t.Record(s);
    auto d4 = Data(2, 340000);
    for (size_t off = 0; off < d4.size();) {
        size_t n = min((size_t)(1 + rand() % 60000), d4.size() - off);
        a.write(s, d4.data() + off, (uint32_t)n, 0);
        off += n;
        t.Record(s);
    }
    a.close(s); t.Record(s); t.File(p4);

    // Mode 2 then 3: memory buffer, re-opened by name for reading.
    std::string p2 = Path(run, "m2.bin");
    s = a.open(p2.c_str(), kStreamWriteMemory, 64, 0);
    t.V(s);
    auto d2 = Data(3, 9000);
    a.write(s, d2.data(), 9000, 0);
    // a collection, then strings
    Collection c;
    memset(&c, 0, sizeof c);
    Collection_Construct(&c, 0, 1, 24, 4, 3, 1, 1, 0);
    uint8_t rec[24];
    for (int i = 0; i < 40; ++i) {
        memset(rec, i, sizeof rec);
        Collection_Allocate(&c, 0, rec, 24, i * 7 % 13, 0);
        if (i % 5 == 3) Collection_Release(&c, 0, 1 + i / 2);
    }
    a.save(&c, 0, s, 0);
    a.save(&c, 0, s, 30); // padded records
    a.save(&c, 0, s, 10); // truncated records
    const char* strs[] = { "", "x", "hello world", nullptr };
    for (const char** q = strs; *q; ++q) {
        uint8_t len = (uint8_t)(strlen(*q) + 1);
        a.write(s, &len, 1, 0);
        a.write(s, *q, len, 0);
    }
    uint8_t zero = 0;
    a.write(s, &zero, 1, 0);
    std::string big(300, 'q');
    uint8_t hdr[2] = { (uint8_t)(0x80 | (301 >> 8)), (uint8_t)(301 & 0xff) };
    a.write(s, hdr, 2, 0);
    a.write(s, big.c_str(), 301, 0);
    a.writeU32(s, 1234);
    a.close(s); t.Record(s);
    Stream r = a.open(p2.c_str(), kStreamReadMemory, 0, 0);
    t.V(r); t.Record(r);
    std::vector<uint8_t> back(9000);
    t.V(a.read(r, back.data(), 9000, 0, 1));
    t.B(back.data(), back.size());
    for (int k = 0; k < 3; ++k) {
        Collection l;
        memset(&l, 0, sizeof l);
        Collection_Construct(&l, 0, 1, 24, 4, 3, 1, 1, 0);
        t.V(a.load(&l, 0, r, k == 0 ? 0 : (k == 1 ? 30 : 10)));
        t.V(l.count); t.V(l.capacity); t.V(l.sequence);
        for (uint32_t i = 1; i < l.capacity; ++i)
            if (Collection_IsValid(&l, 0, i)) t.B(Collection_Get(&l, 0, i), 24);
        for (int list = 1; list < 3; ++list) t.B(l.nodes[list], l.capacity * 12);
        Collection_Destruct(&l);
    }
    for (int k = 0; k < 6; ++k) {
        char* str = nullptr;
        t.V(a.readString(r, &str));
        t.V(str != nullptr);
        if (str) { t.B(str, strlen(str)); ((void(__cdecl*)(void*))0x5a1145)(str); }
    }
    t.V(a.skip4(r, 0, 0, 1));
    t.V(a.read(r, back.data(), 100, 0, 1)); // past the end
    t.Record(r);
    a.close(r);
    a.release(r);
    t.Record(r);
    Collection_Destruct(&c);

    // Mode 6: chunked read of a big file in random sizes, incl. > 200000.
    std::string p6 = Path(run, "m6.bin");
    {
        auto big6 = Data(4, 650000);
        FILE* f = fopen(p6.c_str(), "wb");
        fwrite(big6.data(), 1, big6.size(), f);
        fclose(f);
    }
    s = a.open(p6.c_str(), kStreamReadChunked, 0, 0);
    t.V(s); t.Record(s);
    std::vector<uint8_t> buf(450000);
    srand(5);
    for (int k = 0; k < 12; ++k) {
        uint32_t n = (k == 4) ? 420000 : (uint32_t)(1 + rand() % 90000);
        t.V(a.read(s, buf.data(), n, 0, 1));
        t.B(buf.data(), n);
        t.Record(s);
    }
    a.close(s);
    a.release(s);

    // Mode 6 on a small file becomes mode 5; mode 1 reads the fd directly.
    std::string p5 = Path(run, "m5.bin");
    {
        FILE* f = fopen(p5.c_str(), "wb");
        fwrite(d.data(), 1, 3000, f);
        fclose(f);
    }
    s = a.open(p5.c_str(), kStreamReadChunked, 0, 0);
    t.Record(s);
    t.V(a.read(s, buf.data(), 5000, 0, 1));
    t.B(buf.data(), 3000);
    a.close(s); t.Record(s);
    s = a.open(p5.c_str(), kStreamReadFile, 0, 0);
    t.V(a.read(s, buf.data(), 1000, 0, 1));
    t.B(buf.data(), 1000);
    a.close(s);

    // Missing file: -1 (no fatal error when not asked).
    t.V(a.open(Path(run, "missing.bin").c_str(), kStreamReadWhole, 0, 0));
    t.V(a.exists(p5.c_str()));
    t.V(a.exists(Path(run, "missing.bin").c_str()));
    a.releaseAll();
    t.V(*(int32_t*)0xccb9ec);
}

} // namespace

DIFFTEST(stream_all) {
    char tmp[MAX_PATH];
    GetTempPathA(MAX_PATH, tmp);
    g_dir = std::string(tmp) + "rt3re_stream_";
    Trace a, b;
    *(int32_t*)0xccb9e8 = 0;
    Scenario(kOrig, "orig", a);
    int32_t skipsA = *(int32_t*)0xccb9e8;
    *(int32_t*)0xccb9e8 = 0;
    Scenario(kNew, "new", b);
    EXPECT_EQ(skipsA, *(int32_t*)0xccb9e8, "skip counter");
    EXPECT_EQ(a.values.size(), b.values.size(), "value count");
    for (size_t i = 0; i < a.values.size() && i < b.values.size(); ++i)
        EXPECT_EQ(a.values[i], b.values[i], "value #%u: %lld vs %lld", (unsigned)i, a.values[i], b.values[i]);
    EXPECT_EQ(a.bytes.size(), b.bytes.size(), "byte count");
    if (a.bytes.size() == b.bytes.size())
        EXPECT_EQ(memcmp(a.bytes.data(), b.bytes.data(), a.bytes.size()), 0, "bytes differ");
    for (const char* n : { "m0.bin", "m4.bin", "m2.bin", "m6.bin", "m5.bin" }) {
        DeleteFileA(Path("orig", n).c_str());
        DeleteFileA(Path("new", n).c_str());
    }
}
