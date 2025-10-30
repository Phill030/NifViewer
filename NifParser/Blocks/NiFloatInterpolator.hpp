#pragma once
#include "NiKeyBasedInterpolator.hpp"
#include "../../Types/Ref.hpp"
#include "NiFloatData.hpp"

struct NiFloatInterpolator : NiKeyBasedInterpolator
{
public:
	float value; // Pose value if lacking NiFloatData.
	Ref<NiFloatData> data;
	NiFloatInterpolator(Reader& reader, const NifHeader& header) : NiKeyBasedInterpolator(),
		value(reader.read<float>()),
		data(Ref<NiFloatData>(reader)) {}
};