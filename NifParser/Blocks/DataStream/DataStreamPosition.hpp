#pragma once
#include "DataStreamData.hpp"
#include "../../Types/Vector3.hpp"
#include <vector>

struct DataStreamPosition : DataStreamData
{
public: 
	using value_type = Vector3;
	vector<Vector3> values;
};
