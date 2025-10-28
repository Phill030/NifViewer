#pragma once
#include "NiGeometry.hpp"

struct NiTriBasedGeom : NiGeometry
{
public:
	NiTriBasedGeom(Reader& reader, const NifHeader& header) : NiGeometry(reader, header) {}
};