#pragma once
#include "../NifParser/Blocks/NiSourceTexture.hpp"
#include "../NifParser/NifHeader.hpp"
#include "../Reader.hpp"
#include "Ref.hpp"
#include "TexCoord.hpp"
#include <cstdint>

#define ExcludeVersion(ver) if (header.version.toString() != ver)

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
	TextureTransform(Reader& reader, const NifHeader& header) {
		translation = reader.read<TexCoord>();
		scale = reader.read<TexCoord>();
		rotation = reader.read<float>();
		method = static_cast<TransformMethod>(reader.read<uint32_t>());
		center = reader.read<TexCoord>();
	}

};

struct TexDesc
{
public:
	Ref<NiSourceTexture> source; // Reference to a NiSourceTexture block
	uint16_t flags;

	// NOT IN EVERY VERSION 
	optional<uint16_t> maxAnisotropy;

	bool hasTextureTransform;
	optional<TextureTransform> textureTransform;

	TexDesc(Reader& reader, const NifHeader& header) : source(Ref<NiSourceTexture>(reader)) {
		flags = reader.read<uint16_t>();

		ExcludeVersion("20.3.0.9")
			maxAnisotropy = reader.read<uint16_t>();

		hasTextureTransform = reader.read<bool>();
		if (hasTextureTransform)
			textureTransform = TextureTransform(reader, header);
	}
};