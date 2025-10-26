#pragma once
#include "NiTexture.hpp"
#include "../../Reader.hpp"
#include "../NifHeader.hpp"
#include <cstdint>
#include <string>

using namespace std;


struct NiSourceTexture : NiTexture
{
public:
	uint8_t useExternal;
	string filePath;
	int32_t unknownLink; // Ref<NiObject>
	FormatPrefs formatPrefs;
	uint8_t isStatic;
	bool directRender;
	bool persistRenderData;

	NiSourceTexture(Reader* reader, const NifHeader& header): NiTexture(reader, header) {
		useExternal = reader->read<uint8_t>();
		filePath = header.getIndexString(reader);
		unknownLink = reader->read<int32_t>();
		formatPrefs = FormatPrefs(reader, header);
		isStatic = reader->read<uint8_t>();
		directRender = reader->read<bool>();
		persistRenderData = reader->read<bool>();
	}
};