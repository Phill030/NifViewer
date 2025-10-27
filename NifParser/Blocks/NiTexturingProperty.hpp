#pragma once
#include "NiProperty.hpp"
#include <cstdint>
#include <vector>
#include "../../Types/ShaderTexDesc.hpp"
#include "../NifHeader.hpp"
#include "../../Reader.hpp"
#include "../../Types/TexDesc.hpp"
#include <optional>

using namespace std;

// Describes how a fragment shader should be configured for a given piece of geometry. 
struct NiTexturingProperty : NiProperty
{
public:
	uint16_t flags;
	uint32_t numTextures;
	bool hasBaseTexture;
	TexDesc baseTexture;

	bool hasDarkTexture;
	optional<TexDesc> darkTexture; //TODO: TexDesc

	bool hasDetailTexture;
	optional<TexDesc> detailTexture; //TODO: TexDesc

	bool hasGlossTexture;
	optional<TexDesc> glossTexture; //TODO: TexDesc

	bool hasGlowTexture;
	optional<TexDesc> glowTexture; //TODO: TexDesc

	bool hasBumpMapTexture;
	optional<TexDesc> bumpMapTexture; //TODO: TexDesc

	bool hasNormalTexture;
	optional<TexDesc> normalTexture; //TODO: TexDesc

	bool hasParallaxTexture;
	optional<TexDesc> parallaxTexture; //TODO: TexDesc

	bool hasDecal0Texture;
	optional<TexDesc> decal0Texture; //TODO: TexDesc

	uint32_t numShaderTextures;
	vector<ShaderTexDesc> shaderTextures;

	NiTexturingProperty(Reader* reader, const NifHeader& header): NiProperty(reader, header),
		flags(reader->read<uint16_t>()),
		numTextures(reader->read<uint32_t>()),
		hasBaseTexture(reader->read<bool>()),
		baseTexture(TexDesc(reader, header)) {

		hasDarkTexture = reader->read<bool>();
		if(hasDarkTexture)
			darkTexture = TexDesc(reader, header); // TODO: THIS IS NOT A Ref<TexDesc> BUT A TexDesc

		hasDetailTexture = reader->read<bool>();
		if (hasDetailTexture)
			detailTexture = TexDesc(reader, header);

		hasGlossTexture = reader->read<bool>();
		if (hasGlossTexture)
			glossTexture = TexDesc(reader, header);

		hasGlowTexture = reader->read<bool>();
		if (hasGlowTexture)
			glowTexture = TexDesc(reader, header);

		hasBumpMapTexture = reader->read<bool>();
		if (hasBumpMapTexture)
			bumpMapTexture = TexDesc(reader, header);

		hasNormalTexture = reader->read<bool>();
		if (hasNormalTexture)
			normalTexture = TexDesc(reader, header);

		hasParallaxTexture = reader->read<bool>();
		if (hasParallaxTexture)
			parallaxTexture = TexDesc(reader, header);

		hasDecal0Texture = reader->read<bool>();
		if (hasDecal0Texture)
			decal0Texture = TexDesc(reader, header);

		numShaderTextures = reader->read<uint32_t>();
		shaderTextures.reserve(numShaderTextures);
		for (uint32_t i = 0; i < numShaderTextures; i++) {
			shaderTextures.push_back(ShaderTexDesc(reader, header));
		}
	}
};