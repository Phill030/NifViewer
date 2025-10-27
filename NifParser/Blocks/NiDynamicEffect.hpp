#pragma once
#include "NiAvObject.hpp"

// Abstract base class for dynamic effects such as NiLights or projected texture effects. 
struct NiDynamicEffect : NiAvObject
{
public:
	bool switchState; // If true, then the dynamic effect is applied to affected nodes during rendering.
	uint32_t numAffectedNodes;
	vector<uint32_t> affectedNodes; // If a node appears in this list, then its entire subtree will be affected by the effect.
	
	NiDynamicEffect(Reader& reader, const NifHeader& header) : NiAvObject(reader, header) {
		switchState = reader.read<bool>();
		numAffectedNodes = reader.read<uint32_t>();
		affectedNodes.reserve(numAffectedNodes);
		for (uint32_t i = 0; i < numAffectedNodes; i++) {
			affectedNodes.push_back(reader.read<uint32_t>());
		}
	}

};