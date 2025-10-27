#pragma once
#include <cstdint>
#include "../NifParser/NifHeader.hpp"
#include "../Reader.hpp"
#include "../NifParser/Blocks/NiSourceTexture.hpp"
#include "Ref.hpp"
#include "TexCoord.hpp"


enum class TransformMethod : uint32_t
{
	TM_MayaDeprecated = 0, // Center * Rotation * Back * Translate * Scale
	TM_Max = 1, // Center * Scale * Rotation * Translate * Back
	TM_Maya = 2  // Center * Rotation * Back * FromMaya * Translate * Scale
};

struct TextureTransform
{
public:
	TexCoord translation;
	TexCoord scale;
	float rotation;
	TransformMethod method;
	TexCoord center;
	TextureTransform(Reader* reader, const NifHeader& header) {
		translation = reader->read<TexCoord>();
		scale = reader->read<TexCoord>();
		rotation = reader->read<float>();
		method = static_cast<TransformMethod>(reader->read<uint32_t>());
		center = reader->read<TexCoord>();
	}

};

struct TexDesc
{
public:
	Ref<NiSourceTexture> source; // Reference to a NiSourceTexture block
	uint16_t flags;
	uint16_t maxAnisotropy;

	bool hasTextureTransform;
	optional<TextureTransform> textureTransform;

	TexDesc(Reader* reader, const NifHeader& header) : source(Ref<NiSourceTexture>(reader)) {
		flags = reader->read<uint16_t>();
		maxAnisotropy = reader->read<uint16_t>();
		hasTextureTransform = reader->read<bool>();
		if (hasTextureTransform)
			textureTransform = TextureTransform(reader, header);
	}
};