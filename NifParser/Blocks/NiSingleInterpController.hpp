#pragma once
#include "NiInterpController.hpp"
#include "../../Types/Ref.hpp"
#include "NiInterpolator.hpp"

struct NiSingleInterpController : NiInterpController
{
public:
	Ref<NiInterpolator> interpolator;
	NiSingleInterpController(Reader& reader) : NiInterpController(reader),
		interpolator(Ref<NiInterpolator>(reader)) {}
};