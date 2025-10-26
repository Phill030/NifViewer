#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../../Reader.hpp"
#include "../Nif.hpp"
#include "../NifHeader.hpp"
#include "NiObject.hpp"
#include "NiExtraData.hpp"
#include "../../Types/Ref.hpp"

using namespace std;

struct NiObjectNet : NiObject
{
public:
	string name;
	vector<Ref<NiExtraData>> extraData;
	int32_t controller; // TODO: Make Ref<NiTimeController>

	NiObjectNet(Reader* reader, const NifHeader& header) {
		name = header.getIndexString(reader);

		extraData.reserve(reader->read<uint32_t>());
		for (int j = 0; j < extraData.capacity(); j++) {
			extraData.push_back(Ref<NiExtraData>(reader));
		}

		controller = reader->read<int32_t>();
	}
};