#pragma once
#include "NiProperty.hpp"
#include "../../Types/Color3.hpp"
#include "../NifHeader.hpp"
#include "../../Reader.hpp"

// Describes the surface properties of an object e.g. translucency, ambient color, diffuse color, emissive color, and specular color. 
struct NiMaterialProperty : NiProperty
{
public:
	Color3 ambientColor;
	Color3 diffuseColor;
	Color3 specularColor;
	Color3 emissiveColor;
	float glossiness;
	float alpha;

	NiMaterialProperty(Reader& reader, const NifHeader& header) : NiProperty(reader, header) {
		ambientColor = reader.read<Color3>();
		diffuseColor = reader.read<Color3>();
		specularColor = reader.read<Color3>();
		emissiveColor = reader.read<Color3>();
		glossiness = reader.read<float>();
		alpha = reader.read<float>();

	}

	virtual ~NiMaterialProperty() = default;
};
