#pragma once
#include "NiTimeController.hpp"

struct NiInterpController : NiTimeController
{
	public:
		NiInterpController(Reader* reader) : NiTimeController(reader) {}
};