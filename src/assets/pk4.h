// PK4 archive reader. Format: docs/formats.md#pk4.
#pragma once
#include <stdint.h>
#include <string>
#include <vector>

namespace rt3 {

struct Pk4Entry {
    std::string name;
    uint32_t    hash;
    uint32_t    size;
    uint32_t    offset; // absolute file offset
};

class Pk4 {
public:
    bool Open(const char* path);
    const std::vector<Pk4Entry>& Entries() const { return entries_; }
    const Pk4Entry* Find(const char* name) const; // case-insensitive
    bool Read(const Pk4Entry& e, std::vector<uint8_t>& out) const;

private:
    std::string path_;
    std::vector<Pk4Entry> entries_;
};

} // namespace rt3
