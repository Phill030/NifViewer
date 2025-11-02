#pragma once
#include "DataStreamData.hpp"
#include "../../Types/TexCoord.hpp"
#include <vector>

struct DataStreamTexcoord : DataStreamData
{
public:
	using value_type = TexCoord;
	vector<TexCoord> values;
};