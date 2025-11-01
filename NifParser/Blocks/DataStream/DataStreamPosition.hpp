#pragma once
#include "DataStreamData.hpp"
#include "../../Types/Vector3.hpp"
struct DataStreamPosition : DataStreamData, Vector3
{
    using Vector3::Vector3; // inherit constructors
};
