#pragma once
#include "NiGeometryData.hpp"
#include <cstdint>

struct NiTriBasedGeomData : NiGeometryData
{
public:
	uint16_t numTriangles;

	NiTriBasedGeomData(Reader& reader, const NifHeader& header) : NiGeometryData(reader, header) {
		numTriangles = reader.read<uint16_t>();
	}
};