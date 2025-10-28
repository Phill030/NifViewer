#pragma once
#include <cstdint>

struct Triangle
{
public:
	uint16_t v1, v2, v3;
	Triangle() : v1(0), v2(0), v3(0) {}
	Triangle(uint16_t v1, uint16_t v2, uint16_t v3) : v1(v1), v2(v2), v3(v3) {}
};