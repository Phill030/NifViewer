#pragma once
#include "NiProperty.hpp"
#include <cstdint>
#include <vector>
#include "../Types/ShaderTexDesc.hpp"
#include "../NifHeader.hpp"
#include "../../Reader.hpp"
#include "../../Types/TexDesc.hpp"

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
	bool hasDetailTexture;
	bool hasGlossTexture;
	bool hasBumpMapTexture;
	bool hasNormalTexture;
	bool hasParallaxTexture;
	bool hasDecal0Texture;
	uint32_t numShaderTextures;
	vector<ShaderTexDesc> shaderTextures;

	NiTexturingProperty(Reader* reader, const NifHeader& header)
		: NiProperty(reader, header), flags(reader->read<uint16_t>()),
		numTextures(reader->read<uint32_t>()), hasBaseTexture(reader->read<bool>()),
		baseTexture(TexDesc(reader, header)) {
		hasDarkTexture = reader->read<bool>();
		hasDetailTexture = reader->read<bool>();
		hasGlossTexture = reader->read<bool>();
		hasBumpMapTexture = reader->read<bool>();
		hasNormalTexture = reader->read<bool>();
		hasParallaxTexture = reader->read<bool>();
		hasDecal0Texture = reader->read<bool>();

		numShaderTextures = reader->read<uint32_t>();
		shaderTextures.reserve(numShaderTextures);
		for (uint32_t i = 0; i < numShaderTextures; i++) {
			shaderTextures.push_back(ShaderTexDesc(reader, header));
		}
	}
};