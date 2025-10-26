#pragma once
#include <cstdint>

struct Color3
{
public:
	float r;
	float g;
	float b;
	Color3() : r(0), g(0), b(0) {}
	Color3(float red, float green, float blue) : r(red), g(green), b(blue) {}
};
