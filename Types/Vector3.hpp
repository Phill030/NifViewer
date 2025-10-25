#pragma once
#include <vector>
#include <cstdint>
#include <format>

using namespace std;

struct Vector3
{
public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator*(float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 crossProduct(const Vector3& other) const {
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	float dotProduct(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 normalize() const {
		float len = length();
		if (len == 0) return Vector3(0, 0, 0);
		return Vector3(x / len, y / len, z / len);
	}

	string toString() const {
		return format("({}, {}, {})", x, y, z);
	}
};
static_assert(sizeof(Vector3) == sizeof(float) * 3, "Vector3 must be tightly packed");
