#pragma once
#include "TexDesc.hpp"
#include <cstdint>

struct ShaderTexDesc
{
public:
	bool hasMap;
	TexDesc map;
	uint32_t mapId;

	ShaderTexDesc(Reader* reader, const NifHeader& header)
		: hasMap(reader->read<bool>()),
		map(reader, header),
		mapId(reader->read<uint32_t>()) {
	}
};
