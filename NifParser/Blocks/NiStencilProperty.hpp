#pragma once
#include "NiProperty.hpp"
#include <cstdint>
#include "../../Reader.hpp"
#include "../NifHeader.hpp"

using namespace std;

/*Property flags:
Bit 0: Stencil Enable
Bits 1-3: Fail Action
Bits 4-6: Z Fail Action
Bits 7-9: Pass Action
Bits 10-11: Draw Mode
Bits 12-14: Stencil Function
*/
struct NiStencilProperty : NiProperty
{
public:
	uint16_t flags;
	uint32_t stencilRef;
	uint32_t stencilMask; // A bit mask. The default is 0xffffffff.
	NiStencilProperty(Reader* reader, const NifHeader& header): NiProperty(reader, header) {
		flags = reader->read<uint16_t>();
		stencilRef = reader->read<uint32_t>();
		stencilMask = reader->read<uint32_t>();
	}
};