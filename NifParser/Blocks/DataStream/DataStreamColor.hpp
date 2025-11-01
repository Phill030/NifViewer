#pragma once
#include "DataStreamData.hpp"
#include "../../Types/Color3.hpp"

struct DataStreamColor : DataStreamData, Color3
{
	using Color3::Color3;
};