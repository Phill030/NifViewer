#pragma once
#include "NiInterpController.hpp"
#include <cstdint>
#include <vector>
#include "../../Types/Ref.hpp"
#include "NiInterpolator.hpp"

struct NiMorphWeightsController : NiInterpController
{
public:
	uint32_t count;
	uint32_t numInterpolators;
	vector<Ref<NiInterpolator>> interpolators;
	uint32_t numTargets;
	vector<string> targetNames;

	NiMorphWeightsController(Reader& reader, const NifHeader& header) : NiInterpController(reader) {
		count = reader.read<uint32_t>();
		numInterpolators = reader.read<uint32_t>();
		interpolators.reserve(numInterpolators);
		for (uint32_t i = 0; i < numInterpolators; i++) {
			interpolators.push_back(Ref<NiInterpolator>(reader));
		}
		numTargets = reader.read<uint32_t>();
		targetNames.reserve(numTargets);
		for (uint32_t i = 0; i < numTargets; i++) {
			string name = header.getIndexString(reader);
			targetNames.push_back(name);
		}
	}
};