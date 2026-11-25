// IJG libjpeg (release 6b: JPEG_LIB_VERSION 62) statically linked into RT3.exe,
// one name per function (jpeg_read_header -> jpeg::read_header). The game's
// copy was compiled __fastcall. Used for .jpg textures, fonts and screenshots.
//
// Moving off the exe later replaces these definitions; the call sites stay.
// The structs (jpeg_decompress_struct 0x19f bytes, jpeg_compress_struct 0x15b,
// jpeg_error_mgr 0x84) are kept as byte buffers by the callers.
#pragma once
#include <stdint.h>

namespace rt3 {
namespace jpeg {

#define RT3_JPEG(ret, name, params, addr) static ret(__fastcall* const name) params = (ret(__fastcall*) params)addr

RT3_JPEG(void*, std_error, (void* err), 0x5b3b30);

// Decompression.
RT3_JPEG(void, CreateDecompress, (void* cinfo, int32_t version, int32_t structSize), 0x5b3230);
RT3_JPEG(void, stdio_src, (void* cinfo, void* file), 0x5b3980);
RT3_JPEG(int, read_header, (void* cinfo, uint8_t requireImage), 0x5b3610);
RT3_JPEG(int, start_decompress, (void* cinfo), 0x5b37e0);
RT3_JPEG(uint32_t, read_scanlines, (void* cinfo, uint8_t** rows, uint32_t maxLines), 0x5b3740);
RT3_JPEG(int, finish_decompress, (void* cinfo), 0x5b3550);
RT3_JPEG(void, destroy_decompress, (void* cinfo), 0x5b3300);

// Compression.
RT3_JPEG(void, CreateCompress, (void* cinfo, int32_t version, int32_t structSize), 0x5b3b80);
RT3_JPEG(void, stdio_dest, (void* cinfo, void* file), 0x5b48f0);
RT3_JPEG(void, set_defaults, (void* cinfo), 0x5b4710);
RT3_JPEG(void, set_quality, (void* cinfo, int32_t quality, int32_t forceBaseline), 0x5b4290);
RT3_JPEG(void, start_compress, (void* cinfo, uint8_t writeAllTables), 0x5b3db0);
RT3_JPEG(uint32_t, write_scanlines, (void* cinfo, uint8_t** rows, uint32_t numLines), 0x5b3e20);
RT3_JPEG(void, finish_compress, (void* cinfo), 0x5b3cc0);
RT3_JPEG(void, destroy_compress, (void* cinfo), 0x5b3c40);

#undef RT3_JPEG

} // namespace jpeg
} // namespace rt3
