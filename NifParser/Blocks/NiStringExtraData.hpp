#pragma once
#include "NiExtraData.hpp"

struct NiStringExtraData : NiExtraData
{
public:
	string stringData; // The String
	NiStringExtraData(Reader* reader, const NifHeader& header) : NiExtraData(reader, header) {
		stringData = header.getIndexString(reader);
	}
};