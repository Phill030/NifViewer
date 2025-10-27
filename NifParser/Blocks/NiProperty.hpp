#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../../Reader.hpp"
#include "../Nif.hpp"
#include "../NifHeader.hpp"
#include "NiObjectNet.hpp"

using namespace std;

// Abstract base class representing all rendering properties.
// Subclasses are attached to NiAVObjects to control their rendering.
struct NiProperty : NiObjectNet
{
	NiProperty(Reader& reader, const NifHeader& header) : NiObjectNet(reader, header) {}
	virtual ~NiProperty() = default;
};