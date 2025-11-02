#pragma once
#include "DataStreamData.hpp"
#include "../../Types/Vector3.hpp"

struct DataStreamNormal : DataStreamData
{
public:
	using value_type = Vector3;
	vector<Vector3> values;
};