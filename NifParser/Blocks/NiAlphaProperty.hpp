#pragma once
#include "NiProperty.hpp"
#include <cstdint>
#include "../../Reader.hpp"
#include "../NifHeader.hpp"

using namespace std;

struct NiAlphaProperty : NiProperty
{
public:
	uint16_t flags;
	uint8_t threshold;
	NiAlphaProperty(Reader* reader, const NifHeader& header): NiProperty(reader, header) {
		flags = reader->read<uint16_t>();
		threshold = reader->read<uint8_t>();
	}
};