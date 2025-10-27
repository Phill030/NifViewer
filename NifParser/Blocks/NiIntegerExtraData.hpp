#pragma once
#include "NiExtraData.hpp"
#include <cstdint>

using namespace std;

struct NiIntegerExtraData : NiExtraData
{
public:
	uint32_t integerData;
	NiIntegerExtraData(Reader& reader, const NifHeader& header) : NiExtraData(reader, header) {
		integerData = reader.read<uint32_t>();
	}
};