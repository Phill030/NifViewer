#pragma once
#include "NiTriBasedGeom.hpp"

struct NiTriShape : NiTriBasedGeom
{
public:
	NiTriShape(Reader& reader, const NifHeader& header) : NiTriBasedGeom(reader, header) {}
};