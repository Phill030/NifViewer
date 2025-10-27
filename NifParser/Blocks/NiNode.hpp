#pragma once
#include <cstdint>
#include <vector>
#include "../../Reader.hpp"
#include "NiAvObject.hpp"
#include "../NifHeader.hpp"
#include "../../Types/Ref.hpp";
#include "NiDynamicEffect.hpp"

using namespace std;

struct NiNode : NiAvObject
{
public:
	uint32_t numChildren;
	vector<Ref<NiAvObject>> children;
	uint32_t numEffects;
	vector<Ref<NiDynamicEffect>> effects;

	NiNode(Reader& reader, const NifHeader& header) : NiAvObject(reader, header)  {
		numChildren = reader.read<uint32_t>();
		children.reserve(numChildren);
		for (int j = 0; j < numChildren; j++) {
			children.push_back(Ref<NiAvObject>(reader));
		}

		numEffects = reader.read<uint32_t>();
		effects.reserve(numEffects);
		for (int j = 0; j < numEffects; j++) {
			effects.push_back(Ref<NiDynamicEffect>(reader));
		}
	}
};
