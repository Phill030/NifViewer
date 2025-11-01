#pragma once
#include "DataStreamData.hpp"
#include "../../Types/Vector3.hpp"
struct DataStreamNormal : DataStreamData, Vector3
{
    using Vector3::Vector3;
};