#pragma once
#include "NiExtraData.hpp"
#include <cstdint>
#include "../../Reader.hpp"
#include "../NifParser/NifHeader.hpp"
#include <vector>

using namespace std;

struct NiIntegersExtraData : NiExtraData
{
public:
	uint32_t numIntegers;
	vector<uint32_t> data;

	NiIntegersExtraData(Reader& reader, const NifHeader& header) : NiExtraData(reader, header) {
		numIntegers = reader.read<uint32_t>();
		data.reserve(numIntegers);
		for (uint32_t i = 0; i < numIntegers; i++) {
			data.push_back(reader.read<uint32_t>());
		}
	}
};