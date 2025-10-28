#pragma once
#include "NiAvObject.hpp"
#include "../NifHeader.hpp"
#include "../../Types/Ref.hpp"
#include "NiGeometryData.hpp"
#include "NiSkinInstance.hpp"
#include "../../Types/MaterialData.hpp"

struct NiGeometry : NiAvObject
{
public:
	Ref<NiGeometryData> data;
	Ref<NiSkinInstance> skinInstance;
	MaterialData materialData;
	NiGeometry(Reader& reader, const NifHeader& header) : NiAvObject(reader, header),
		data(Ref<NiGeometryData>(reader)),
		skinInstance(Ref<NiSkinInstance>(reader)) {
		materialData = MaterialData(reader, header);
	}
};