#pragma once
#include <cstdint>

struct MipMap
{
public:
	uint32_t width, height, offset;

	MipMap() : width(0), height(0), offset(0) {}
	MipMap(uint32_t w, uint32_t h, uint32_t o) : width(w), height(h), offset(o) {}
};