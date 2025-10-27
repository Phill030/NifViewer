#pragma once
#include <vector>
#include <cstdint>

using namespace std;

struct Matrix22
{
public:
	float m[2][2];
	Matrix22() {
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				m[i][j] = 0.0f;
	}
};