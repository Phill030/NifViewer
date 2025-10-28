#pragma once
#include "NiObject.hpp"
#include <cstdint>
#include "../../Reader.hpp"
#include <vector>

using namespace std;

/// Describes the pixel component used by the NiPixelData object to store a texture.
enum class PixelComponent : uint32_t
{
    PX_COMP_RED = 0,          ///< Red color component.
    PX_COMP_GREEN = 1,        ///< Green color component.
    PX_COMP_BLUE = 2,         ///< Blue color component.
    PX_COMP_ALPHA = 3,        ///< Alpha (transparency) component.
    PX_COMP_COMPRESSED = 4,   ///< Compressed texture component.
    PX_COMP_OFFSET_U = 5,     ///< Texture coordinate U offset.
    PX_COMP_OFFSET_V = 6,     ///< Texture coordinate V offset.
    PX_COMP_OFFSET_W = 7,     ///< Texture coordinate W offset.
    PX_COMP_OFFSET_Q = 8,     ///< Texture coordinate Q offset.
    PX_COMP_LUMA = 9,         ///< Luminance component.
    PX_COMP_HEIGHT = 10,      ///< Height or elevation component.
    PX_COMP_VECTOR_X = 11,    ///< X component of a vector (e.g. normal map).
    PX_COMP_VECTOR_Y = 12,    ///< Y component of a vector.
    PX_COMP_VECTOR_Z = 13,    ///< Z component of a vector.
    PX_COMP_PADDING = 14,     ///< Padding component (unused data).
    PX_COMP_INTENSITY = 15,   ///< Intensity (brightness) component.
    PX_COMP_INDEX = 16,       ///< Palette index component.
    PX_COMP_DEPTH = 17,       ///< Depth component (used for depth maps).
    PX_COMP_STENCIL = 18,     ///< Stencil component.
    PX_COMP_EMPTY = 19        ///< Empty or unused component slot.
};

/// Describes how each pixel should be accessed on NiPixelFormat.
enum class PixelRepresentation : uint32_t
{
    PX_REP_NORM_INT = 0, ///< Normalized integer representation.
    PX_REP_HALF = 1, ///< 16-bit half-precision floating point.
    PX_REP_FLOAT = 2, ///< 32-bit floating point.
    PX_REP_INDEX = 3, ///< Indexed color representation.
    PX_REP_COMPRESSED = 4, ///< Compressed pixel data.
    PX_REP_UNKNOWN = 5, ///< Unknown or undefined representation.
    PX_REP_INT = 6  ///< Unnormalized integer representation.
};

struct PixelFormatComponent
{
public:
    PixelComponent type;
    PixelRepresentation convention;
    uint8_t bitsPerChannel;
    bool isSigned;
    PixelFormatComponent(Reader& reader) {
		type = static_cast<PixelComponent>(reader.read<uint32_t>());
		convention = static_cast<PixelRepresentation>(reader.read<uint32_t>());
		bitsPerChannel = reader.read<uint8_t>();
		isSigned = reader.read<bool>();
    }
};




/// Describes the pixel format used by NiPixelData to store a texture.
enum class PixelFormat : uint32_t
{
    PX_FMT_RGB = 0,           ///< 24-bit RGB. 8 bits per red, blue, and green component.
    PX_FMT_RGBA = 1,          ///< 32-bit RGB with alpha. 8 bits per red, blue, green, and alpha component.
    PX_FMT_PAL = 2,           ///< 8-bit palette index.
    PX_FMT_PALA = 3,          ///< 8-bit palette index with alpha.
    PX_FMT_DXT1 = 4,          ///< DXT1 compressed texture.
    PX_FMT_DXT3 = 5,          ///< DXT3 compressed texture.
    PX_FMT_DXT5 = 6,          ///< DXT5 compressed texture.
    PX_FMT_RGB24NONINT = 7,   ///< (Deprecated) 24-bit noninterleaved texture (old PS2 format).
    PX_FMT_BUMP = 8,          ///< Uncompressed dU/dV gradient bump map.
    PX_FMT_BUMPLUMA = 9,      ///< Uncompressed dU/dV gradient bump map with luma channel representing shininess.
    PX_FMT_RENDERSPEC = 10,   ///< Renderer-specific format not covered by other formats.
    PX_FMT_1CH = 11,          ///< Format with 1 component.
    PX_FMT_2CH = 12,          ///< Format with 2 components.
    PX_FMT_3CH = 13,          ///< Format with 3 components.
    PX_FMT_4CH = 14,          ///< Format with 4 components.
    PX_FMT_DEPTH_STENCIL = 15,///< Depth/stencil surface format.
    PX_FMT_UNKNOWN = 16       ///< Unknown or undefined format.
};

/// Describes whether pixels have been tiled from their standard row-major format
/// to a format optimized for a particular platform.
enum class PixelTiling : uint32_t
{
    PX_TILE_NONE = 0,        ///< Standard row-major format (no tiling).
    PX_TILE_XENON = 1,       ///< Xbox 360 (Xenon) optimized tiling.
    PX_TILE_WII = 2,         ///< Nintendo Wii optimized tiling.
    PX_TILE_NV_SWIZZLED = 3  ///< NVIDIA swizzled format.
};

struct NiPixelFormat : NiObject
{
public:
    PixelFormat pixelFormat; // The format of the pixels in this internally stored image.
    uint8_t bitsPerPixel; // Bits per pixel, 0 (Compressed), 8, 24 or 32.
    uint32_t rendererHint;
    uint32_t extraData;
    uint8_t flags;
    PixelTiling tiling;
    bool sRGBSpace;
    vector<PixelFormatComponent> channels; // Hardcoded to 4 ?
    NiPixelFormat(Reader& reader) {
		pixelFormat = static_cast<PixelFormat>(reader.read<uint32_t>());
		bitsPerPixel = reader.read<uint8_t>();
		rendererHint = reader.read<uint32_t>();
		extraData = reader.read<uint32_t>();
		flags = reader.read<uint8_t>();
		tiling = static_cast<PixelTiling>(reader.read<uint32_t>());
		sRGBSpace = reader.read<bool>();

		channels.reserve(4);
        for (uint32_t i = 0; i < 4; i++) {
            channels.push_back(PixelFormatComponent(reader));
        }
    }
};