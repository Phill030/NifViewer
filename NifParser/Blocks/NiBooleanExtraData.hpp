#pragma once
#include "NiExtraData.hpp"
#include <cstdint>

using namespace std;
struct NiBooleanExtraData : NiExtraData
{
public:
	uint8_t booleanData; // The boolean extra data value.
	NiBooleanExtraData(Reader& reader, const NifHeader& header) : NiExtraData(reader, header) {
		booleanData = reader.read<uint8_t>();
	}
};