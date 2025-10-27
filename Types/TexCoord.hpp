#pragma once
#include <cstdint>

// Texture coordinates (u,v). As in OpenGL; image origin is in the lower left corner. 
struct TexCoord
{
public:
	float u; // First coordinate
	float v; // Second coordinate
	TexCoord() : u(0.0f), v(0.0f) {}
	TexCoord(float uCoord, float vCoord) : u(uCoord), v(vCoord) {}
};