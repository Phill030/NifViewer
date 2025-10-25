#pragma once
#include <cstdint>
#include "NiProperty.hpp"
#include "../NifHeader.hpp"
#include "../../Reader.hpp"

enum LightningMode : uint32_t
{
	// Only the emissive component of the lighting equation is used. No dynamic lights are considered in the lighting process
	Emissive = 0,
	// The emissive, ambient, and diffuse components of the lighting equation are all used.
	EmissiveAmbientDiffuse = 1
};

enum VertexMode : uint16_t
{
	// Emissive, ambient, and diffuse colors are all specified by the niMaterialProperty. In other words, vertex colors aren't used.
	SourceIgnore = 0,
	// Emissive colors are specified by the source vertex colors. Ambient and Diffuse are specified by the niMaterialProperty.
	SourceEmissive = 1,
	// Ambient and Diffuse colors are specified by the source vertex colors. Emissive is specified by the niMaterialProperty (Default).
	SourceAmbientDiffuse = 2
};


//Based on https://cs.uesp.net/w/index.php?title=Category:NifSE_(NiVertexColorProperty)
// Property of vertex colors.
// This object is referred to by the root object of the NIF file whenever some NiTriShapeData object has vertex colors with non-default settings; 
// if not present, vertex colors have vertex_mode=2 and lighting_mode=1. 
struct NiVertexColorProperty : NiProperty
{
private: 
	uint16_t flags;

public:
	NiVertexColorProperty(Reader* reader, const NifHeader& header) : NiProperty(reader, header) {
		flags = reader->read<uint16_t>();
	}

	VertexMode getVertexMode() const {
		return static_cast<VertexMode>(flags & 0x0003);
	}

	LightningMode getLightningMode() const {
		return static_cast<LightningMode>((flags >> 2) & 0x0003);
	}
};