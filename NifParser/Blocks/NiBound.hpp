#pragma once
#include "../Types/Vector3.hpp"

struct NiBound
{
public:
	Vector3 center; // The sphere's center.
	float radius; // The sphere's radius.

	NiBound() = default;
	NiBound(Reader* reader, const NifHeader& header) {
		center = reader->read<Vector3>();
		radius = reader->read<float>();
	}
};