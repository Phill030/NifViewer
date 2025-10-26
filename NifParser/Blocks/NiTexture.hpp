#pragma once
#include "NiObjectNet.hpp"
#include <cstdint>
#include "../../Reader.hpp"
#include "../NifParser/NifHeader.hpp"

using namespace std;

// Describes the color depth in an NiTexture. 
enum class PixelLayout : uint32_t
{
    PX_LAY_PALETTIZED_8 = 0,   // Texture is in 8-bit palettized format.
    PX_LAY_HIGH_COLOR_16 = 1,  // Texture is in 16-bit high color format.
    PX_LAY_TRUE_COLOR_32 = 2,  // Texture is in 32-bit true color format.
    PX_LAY_COMPRESSED = 3,     // Texture is compressed.
    PX_LAY_BUMPMAP = 4,        // Texture is a grayscale bump map.
    PX_LAY_PALETTIZED_4 = 5,   // Texture is in 4-bit palettized format.
    PX_LAY_DEFAULT = 6,        // Use default setting.
    PX_LAY_SINGLE_COLOR_8 = 7, // Single color (8-bit).
    PX_LAY_SINGLE_COLOR_16 = 8,// Single color (16-bit).
    PX_LAY_SINGLE_COLOR_32 = 9,// Single color (32-bit).
    PX_LAY_DOUBLE_COLOR_32 = 10,// Double color (32-bit).
    PX_LAY_DOUBLE_COLOR_64 = 11,// Double color (64-bit).
    PX_LAY_FLOAT_COLOR_32 = 12,// Float color (32-bit).
    PX_LAY_FLOAT_COLOR_64 = 13,// Float color (64-bit).
    PX_LAY_FLOAT_COLOR_128 = 14,// Float color (128-bit).
    PX_LAY_SINGLE_COLOR_4 = 15,// Single color (4-bit).
    PX_LAY_DEPTH_24_X8 = 16    // Depth 24-bit + 8-bit extra channel.
};

// Describes how mipmaps are handled in an NiTexture. 
enum class MipMapFormat : uint32_t
{
    MIP_FMT_NO = 0,       // Texture does not use mip maps.
    MIP_FMT_YES = 1,      // Texture uses mip maps.
    MIP_FMT_DEFAULT = 2   // Use default setting.
};

// Describes how transparency (alpha) is handled in an NiTexture.
enum class AlphaFormat : uint32_t
{
    ALPHA_NONE = 0,     // No alpha.
    ALPHA_BINARY = 1,   // 1-bit alpha.
    ALPHA_SMOOTH = 2,   // Interpolated 4- or 8-bit alpha.
    ALPHA_DEFAULT = 3   // Use default setting.
};

// NiTexture::FormatPrefs. These preferences are a request to the renderer to use a format the most closely matches the settings and may be ignored. 
struct FormatPrefs
{
	PixelLayout pixelLayout; // Requests the way the image will be stored.
    MipMapFormat useMipMaps; // Requests if mipmaps are used or not.
    AlphaFormat alphaFormat; // Requests no alpha, 1-bit alpha, or

    FormatPrefs() = default;
    FormatPrefs(Reader* reader, const NifHeader& header) {
        pixelLayout = static_cast<PixelLayout>(reader->read<uint32_t>());
        useMipMaps = static_cast<MipMapFormat>(reader->read<uint32_t>());
        alphaFormat = static_cast<AlphaFormat>(reader->read<uint32_t>());
	}
};

struct NiTexture : NiObjectNet
{
    // Forwarding constructor so derived classes can initialize the NiObjectNet base.
    NiTexture(Reader* reader, const NifHeader& header)
        : NiObjectNet(reader, header) {
    }
};