#pragma once
#include "DataStreamData.hpp"
#include "../../Types/TexCoord.hpp"
struct DataStreamTexcoord : DataStreamData, TexCoord
{
    using TexCoord::TexCoord;
};