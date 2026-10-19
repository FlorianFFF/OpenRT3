#include "pk4.h"
#include <stdio.h>
#include <string.h>

namespace rt3 {

static const uint32_t kPk4Magic = 0x3EB;
static const int kEntrySize = 74;

bool Pk4::Open(const char* path) {
    entries_.clear();
    path_ = path;
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    uint32_t hdr[2];
    bool ok = fread(hdr, 4, 2, f) == 2 && hdr[0] == kPk4Magic;
    if (ok) {
        std::vector<uint8_t> dir(size_t(hdr[1]) * kEntrySize);
        ok = fread(dir.data(), 1, dir.size(), f) == dir.size();
        uint32_t base = 8 + hdr[1] * kEntrySize;
        for (uint32_t i = 0; ok && i < hdr[1]; ++i) {
            const uint8_t* p = &dir[size_t(i) * kEntrySize];
            Pk4Entry e;
            memcpy(&e.hash, p, 4);
            memcpy(&e.size, p + 4, 4);
            memcpy(&e.offset, p + 8, 4);
            e.offset += base;
            e.name.assign((const char*)p + 13, strnlen((const char*)p + 13, kEntrySize - 13));
            entries_.push_back(e);
        }
    }
    fclose(f);
    return ok;
}

const Pk4Entry* Pk4::Find(const char* name) const {
    for (const Pk4Entry& e : entries_)
        if (_stricmp(e.name.c_str(), name) == 0) return &e;
    return nullptr;
}

bool Pk4::Read(const Pk4Entry& e, std::vector<uint8_t>& out) const {
    FILE* f = fopen(path_.c_str(), "rb");
    if (!f) return false;
    out.resize(e.size);
    bool ok = fseek(f, long(e.offset), SEEK_SET) == 0 && fread(out.data(), 1, e.size, f) == e.size;
    fclose(f);
    return ok;
}

} // namespace rt3
