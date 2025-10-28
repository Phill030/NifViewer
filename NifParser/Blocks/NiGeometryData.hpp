#pragma once
#include "NiObject.hpp"
#include <cstdint>
#include <vector>
#include "../../Types/Vector3.hpp"
#include "../../Types/TexCoord.hpp"
#include "../../Types/Ref.hpp"
#include "AbstractAdditionalGeometryData.hpp"

enum class VectorFlags : uint16_t
{
    VF_UV_1 = 0x0001,
    VF_UV_2 = 0x0002,
    VF_UV_4 = 0x0004,
    VF_UV_8 = 0x0008,
    VF_UV_16 = 0x0010,
    VF_UV_32 = 0x0020,
    VF_Unk64 = 0x0040,
    VF_Unk128 = 0x0080,
    VF_Unk256 = 0x0100,
    VF_Unk512 = 0x0200,
    VF_Unk1024 = 0x0400,
    VF_Unk2048 = 0x0800,
    VF_Has_Tangents = 0x1000,
    VF_Unk8192 = 0x2000,
    VF_Unk16384 = 0x4000,
    VF_Unk32768 = 0x8000,
};

// Enable bitwise operations for VectorFlags
inline VectorFlags operator|(VectorFlags a, VectorFlags b) {
    return static_cast<VectorFlags>(
        static_cast<uint16_t>(a) | static_cast<uint16_t>(b)
        );
}

inline VectorFlags operator&(VectorFlags a, VectorFlags b) {
    return static_cast<VectorFlags>(
        static_cast<uint16_t>(a) & static_cast<uint16_t>(b)
        );
}

inline VectorFlags& operator|=(VectorFlags& a, VectorFlags b) {
    a = a | b;
    return a;
}

// TODO: make own file for this --^

// Used by NiGeometryData to control the volatility of the mesh.
// Consistency Type is masked to only the upper 4 bits(0xF000).Dirty mask is the lower 12 (0x0FFF) but only used at runtime.
enum class ConsistencyType : uint16_t
{
    CT_MUTABLE = 0x0000,
    CT_STATIC = 0x4000,
    CT_VOLATILE = 0x8000
};

struct NiGeometryData : NiObject
{
public:
	int32_t groupId;
	uint16_t numVertices;
	uint8_t keepFlags;
	uint8_t compressFlags;

	bool hasVertices;
	vector<Vector3> vertices;

    VectorFlags vectorFlags;

	bool hasNormals;
	vector<Vector3> normals;

    Vector3 center;
    float radius;
    
    bool hasVertexColors;
    // vector3<???> vertexColors    ???

	vector<TexCoord> uvSets;
	ConsistencyType consistencyFlags;

	int32_t additionalData; // Ref<AbstractAdditionalGeometryData>
    NiGeometryData(Reader& reader, const NifHeader& header) : NiObject() {
		groupId = reader.read<int32_t>();
		numVertices = reader.read<uint16_t>();
		keepFlags = reader.read<uint8_t>();
		compressFlags = reader.read<uint8_t>();

		hasVertices = reader.read<bool>();
        if (hasVertices) {
            vertices.reserve(numVertices);
            for (int i = 0; i < numVertices; i++) {
                vertices.push_back(reader.read<Vector3>());
            }
        }

		vectorFlags = static_cast<VectorFlags>(reader.read<uint16_t>());

        hasNormals = reader.read<bool>();
        if (hasNormals) {
            normals.reserve(numVertices);
            for (int i = 0; i < numVertices; i++) {
                normals.push_back(reader.read<Vector3>());
            }
        }

        center = reader.read<Vector3>();
        radius = reader.read<float>();


		hasVertexColors = reader.read<bool>();
        // vector of vertex colors?

        for (size_t i = 0; i < numVertices; i++) {
			uvSets.push_back(reader.read<TexCoord>());
        }

		consistencyFlags = static_cast<ConsistencyType>(reader.read<uint16_t>());
		additionalData = reader.read<int32_t>();
    }
};