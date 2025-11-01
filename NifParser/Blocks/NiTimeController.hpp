#pragma once
#include "../Reader.hpp"
#include "../Nif.hpp"
#include "NiObject.hpp"
#include <cstdint>
#include <memory>

struct NiTimeController : NiObject
{
public:
	std::unique_ptr<Ref<NiTimeController>> nextController; // Index of the next controller.
	uint16_t flags;
	float frequency;
	float phase;
	float startTime;
	float stopTime;
	int32_t target; // Controller target (object index of the first controllable ancestor of this object).

	NiTimeController(Reader& reader)
		: NiObject()
		, nextController(std::make_unique<Ref<NiTimeController>>(reader))
		, flags(reader.read<uint16_t>())
		, frequency(reader.read<float>())
		, phase(reader.read<float>())
		, startTime(reader.read<float>())
		, stopTime(reader.read<float>())
		, target(reader.read<int32_t>()) {
	}
};