#pragma once
#include "DataStreamData.hpp"
#include <cstdint>

struct DataStreamIndex : DataStreamData
{
public:
	uint16_t value;
	explicit DataStreamIndex(uint16_t val) : value(val) {}
};