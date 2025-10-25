#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../../Reader.hpp"
#include "../Nif.hpp"
#include "../NifHeader.hpp"
#include "NiObject.hpp"

using namespace std;

struct NiObjectNet : NiObject
{
public:
	string name;
	vector<int32_t> extraData;
	int32_t controller;

	NiObjectNet(Reader* reader, const NifHeader& header) {
		name = header.getIndexString(reader);

		extraData.reserve(reader->read<uint32_t>());
		for (int j = 0; j < extraData.capacity(); j++) {
			extraData.push_back(reader->read<int32_t>());
		}

		controller = reader->read<int32_t>();
	}
};