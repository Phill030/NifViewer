#pragma once
#include <cstdint>



struct TexDesc
{
public:
	uint32_t source; // Reference to a NiSourceTexture block
	uint16_t flags;
	uint16_t maxAnisotropy;
	bool hasTextureTransform;

	TexDesc(Reader* reader, const NifHeader& header) {
		source = reader->read<uint32_t>();
		flags = reader->read<uint16_t>();
		maxAnisotropy = reader->read<uint16_t>();
		hasTextureTransform = reader->read<bool>();
	}
};