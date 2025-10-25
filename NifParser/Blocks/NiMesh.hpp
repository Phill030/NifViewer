#pragma once
#include "NiAvObject.hpp"
#include "NiBound.hpp"
#include "NiDataStream.hpp"
#include <cstdint>
#include <vector>
#include "../NifHeader.hpp"
#include "../../Reader.hpp"
#include "../../Types/MaterialData.hpp"

enum class MeshPrimitiveType : uint32_t
{
    Triangles= 0, // Triangle primitive type
    TripStrips= 1, // Triangle strip primitive type
    Lines = 2, // Lines primitive type
    LineStrips= 3, // Line strip primitive type
    Quads = 4, // Quadrilateral primitive type
    PrimitivePoints = 5  // Point primitive type
};

struct NiMesh : NiAvObject
{
public:
	MaterialData materialData;
	MeshPrimitiveType primitiveType; // The primitive type of the mesh, such as triangles or lines.
    uint16_t numSubmeshes; // The number of submeshes contained in this mesh.
	bool instancingEnabled; // Sets whether hardware instancing is being used.
    NiBound bound; // The combined bounding volume of all submeshes.
    uint32_t numDataStreams;
    vector<DataStreamRef> dataStreams;
    uint32_t numModifiers;
    int32_t modifiers;

    NiMesh(Reader* reader, const NifHeader& header) : NiAvObject(reader, header) {
		materialData = MaterialData(reader, header);
        primitiveType = static_cast<MeshPrimitiveType>(reader->read<uint32_t>());
		numSubmeshes = reader->read<uint16_t>();
        instancingEnabled = reader->read<bool>();
		bound = NiBound(reader, header);
        numDataStreams = reader->read<uint32_t>();
		dataStreams.reserve(numDataStreams);
        for (int i = 0; i < numDataStreams; i++) {
            dataStreams.push_back(DataStreamRef(reader, header));
		}
		numModifiers = reader->read<uint32_t>();
		modifiers = reader->read<int32_t>();
    }
};