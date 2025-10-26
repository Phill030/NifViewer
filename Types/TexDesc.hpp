#pragma once
#include <cstdint>
#include "../NifParser/NifHeader.hpp"
#include "../Reader.hpp"
#include "../NifParser/Blocks/NiSourceTexture.hpp"
#include "Ref.hpp"

struct TexDesc
{
public:
	Ref<NiSourceTexture> source; // Reference to a NiSourceTexture block
	uint16_t flags;
	uint16_t maxAnisotropy;
	bool hasTextureTransform;

	TexDesc(Reader* reader, const NifHeader& header) : source(Ref<NiSourceTexture>(reader)) {
		flags = reader->read<uint16_t>();
		maxAnisotropy = reader->read<uint16_t>();
		hasTextureTransform = reader->read<bool>();
	}
};