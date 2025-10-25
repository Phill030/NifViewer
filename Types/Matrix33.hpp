#pragma once
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

struct Matrix33
{
	public:
	float m[3][3];
	Matrix33() {
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				m[i][j] = 0.0f;
	}

	string toString() const {
		string result = "";
		for (int i = 0; i < 3; ++i) {
			result += "[ ";
			for (int j = 0; j < 3; ++j) {
				result += std::to_string(m[i][j]) + " ";
			}
			result += "]\n";
		}
		return result;
	}
};