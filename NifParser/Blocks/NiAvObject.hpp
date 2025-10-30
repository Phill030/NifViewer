#pragma once
#include <cstdint>
#include "../../Types/Vector3.hpp"
#include "../../Reader.hpp"
#include "NiObjectNet.hpp"
#include <vector>
#include "../NifHeader.hpp"
#include "../../Types/Matrix.hpp"


using namespace std;

struct NiAvObject : NiObjectNet
{
public:
	uint16_t flags;
	Vector3 translation;
	Matrix<3,3> rotation;
	float scale;
	uint32_t numProperties;
	vector<uint32_t> properties;
	int32_t collisionObject;

	NiAvObject(Reader& reader, const NifHeader& header) : NiObjectNet(reader, header) {
		flags = reader.read<uint16_t>();
		translation = reader.read<Vector3>();
		rotation = reader.readMatrix<3,3>();
		scale = reader.read<float>();
		readProperties(reader);
		collisionObject = reader.read<int32_t>();
	}

private:
	void readProperties(Reader& reader) {
		numProperties = reader.read<uint32_t>();

		properties.reserve(numProperties);
		for (int j = 0; j < numProperties; j++) {
			properties.push_back(reader.read<uint32_t>());
		}
	}
};