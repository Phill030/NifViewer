#pragma once
#include "NiObject.hpp"
#include <string>
#include "../../Reader.hpp"
#include "../NifHeader.hpp"

struct NiExtraData : NiObject
{
public:
	std::string name; // Name of this object
	NiExtraData(Reader& reader, const NifHeader& header) {
		name = header.getIndexString(reader);
	}
};