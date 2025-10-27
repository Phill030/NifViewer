#pragma once
#include "NiProperty.hpp"
#include <cstdint>
#include "../../Reader.hpp"
#include "../NifHeader.hpp"

struct NiSpecularProperty : NiProperty
{
public:
	uint16_t flags;
	NiSpecularProperty(Reader* reader, const NifHeader& header): NiProperty(reader, header) {
		flags = reader->read<uint16_t>();
	}
};