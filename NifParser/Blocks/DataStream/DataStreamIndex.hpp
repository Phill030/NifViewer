#pragma once
#include "DataStreamData.hpp"
#include <cstdint>
#include <vector>

struct DataStreamIndex : DataStreamData
{
public:
	using value_type = uint16_t;
	vector<uint16_t> values;
};