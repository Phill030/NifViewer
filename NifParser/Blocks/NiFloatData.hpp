#pragma once
#include "../../Reader.hpp"
#include "NiObject.hpp"
#include "../../Types/KeyGroup.hpp"
#include <cstdint>
#include "../NifHeader.hpp"
#include "../../Types/Key.hpp"

struct NiFloatData : NiObject
{
public:
	KeyGroup<float> data;
	NiFloatData(Reader* reader) : NiObject() {
		data.numKeys = reader->read<uint32_t>();
		data.interpolation = static_cast<KeyType>(reader->read<uint32_t>());

		data.keys.reserve(data.numKeys);
		for (uint32_t i = 0; i < data.numKeys; i++) {
			Key<float> key;
			key.time = reader->read<float>();
			key.value = reader->read<float>();
			key.forward = reader->read<float>();
			key.backward = reader->read<float>();
			data.keys.push_back(key);
		}
	}
};