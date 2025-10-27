#pragma once
#include "NiSingleInterpController.hpp"

struct NiFloatInterpController : NiSingleInterpController
{
public:
	NiFloatInterpController(Reader* reader) : NiSingleInterpController(reader) {}
};