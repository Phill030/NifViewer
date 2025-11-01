#pragma once
#include <cstdint>

struct Color4
{
public:
	uint8_t r,g,b,a;
	Color4() : r(0), g(0), b(0), a(0) {}
	Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
};