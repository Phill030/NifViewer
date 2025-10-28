#pragma once
#include "../../Types/MipMap.hpp"
#include "../NifHeader.hpp"
#include "../Reader.hpp"
#include "../Types/Ref.hpp"
#include "NiPalette.hpp"
#include "NiPixelFormat.hpp"
#include <cstdint>
#include <vector>

struct NiPixelData : NiPixelFormat
{
public:
	Ref<NiPalette> palette;
	uint32_t numMipMaps;
	uint32_t bytesPerPixel;
	vector<MipMap> mipMaps;
	uint32_t numPixels;
	uint32_t numFaces;
	vector<uint8_t> pixelData;

	NiPixelData(Reader& reader, const NifHeader& header) : NiPixelFormat(reader), palette(Ref<NiPalette>(reader)) {
		numMipMaps = reader.read<uint32_t>();
		bytesPerPixel = reader.read<uint32_t>();
		mipMaps.reserve(numMipMaps);

		for (uint32_t i = 0; i < numMipMaps; i++) {
			mipMaps.push_back(reader.read<MipMap>());
		}

		numPixels = reader.read<uint32_t>();
		numFaces = reader.read<uint32_t>();

		pixelData = reader.read(numPixels * (numFaces > 0 ? numFaces : 1));
	}
};