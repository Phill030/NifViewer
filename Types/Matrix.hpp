#pragma once
#include <vector>
#include <cstdint>


template <std::size_t N, std::size_t M, typename T = float>
struct Matrix 
{
public:
	T m[N][M];
	Matrix() {
		for (int i = 0; i < N; i++)
			for(int j = 0; j < M; j++)
				m[i][j] = T(0);
	}

	string toString() const {
		string result = "";
		for (int i = 0; i < N; ++i) {
			result += "[ ";
			for (int j = 0; j < M; ++j) {
				result += std::to_string(m[i][j]) + " ";
			}
			result += "]\n";
		}
		return result;
	}
};