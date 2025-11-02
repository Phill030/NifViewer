#pragma once
#include "DataStreamData.hpp"
#include "../../Types/Color4.hpp"
#include <vector>

struct DataStreamColor : DataStreamData
{
public:
	using value_type = Color4;
	vector<Color4> values;
};