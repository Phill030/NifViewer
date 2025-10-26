#pragma once
#include "../../Reader.hpp"
#include "../../Types/Ref.hpp"
#include "../NifHeader.hpp"
#include "NiObject.hpp"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

enum class DataStreamUsage : uint32_t
{
    VERTEX_INDEX = 0, // Vertex index usage
    VERTEX = 1, // Vertex data usage
    SHADER_CONSTANT = 2, // Shader constant usage
    USER = 3  // User-defined usage
};

enum class DataStreamAccess : uint32_t
{
    CPU_READ = 0x00000001, // CPU read access
    CPU_WRITE_STATIC = 0x00000002, // CPU write (static)
    CPU_WRITE_MUTABLE = 0x00000004, // CPU write (mutable)
    CPU_WRITE_VOLATILE = 0x00000008, // CPU write (volatile)
    GPU_READ = 0x00000010, // GPU read access
    GPU_WRITE = 0x00000020, // GPU write access
    CPU_WRITE_STATIC_INITIALIZED = 0x00000040 // CPU write static (initialized)
};

enum class CloningBehavior : uint32_t
{
    CLONING_SHARE = 0, // Share this object pointer with the newly cloned scene
    CLONING_COPY = 1, // Create an exact duplicate of this object for the cloned scene
    CLONING_BLANK_COPY = 2  // Create a copy for the cloned stream, leaving data to be written later
};

enum class ComponentFormat : uint32_t
{
    F_UNKNOWN = 0x00000000,

    F_INT8_1 = 0x00010101,
    F_INT8_2 = 0x00020102,
    F_INT8_3 = 0x00030103,
    F_INT8_4 = 0x00040104,

    F_UINT8_1 = 0x00010105,
    F_UINT8_2 = 0x00020106,
    F_UINT8_3 = 0x00030107,
    F_UINT8_4 = 0x00040108,

    F_NORMINT8_1 = 0x00010109,
    F_NORMINT8_2 = 0x0002010A,
    F_NORMINT8_3 = 0x0003010B,
    F_NORMINT8_4 = 0x0004010C,

    F_NORMUINT8_1 = 0x0001010D,
    F_NORMUINT8_2 = 0x0002010E,
    F_NORMUINT8_3 = 0x0003010F,
    F_NORMUINT8_4 = 0x00040110,

    F_INT16_1 = 0x00010211,
    F_INT16_2 = 0x00020212,
    F_INT16_3 = 0x00030213,
    F_INT16_4 = 0x00040214,

    F_UINT16_1 = 0x00010215,
    F_UINT16_2 = 0x00020216,
    F_UINT16_3 = 0x00030217,
    F_UINT16_4 = 0x00040218,

    F_NORMINT16_1 = 0x00010219,
    F_NORMINT16_2 = 0x0002021A,
    F_NORMINT16_3 = 0x0003021B,
    F_NORMINT16_4 = 0x0004021C,

    F_NORMUINT16_1 = 0x0001021D,
    F_NORMUINT16_2 = 0x0002021E,
    F_NORMUINT16_3 = 0x0003021F,
    F_NORMUINT16_4 = 0x00040220,

    F_INT32_1 = 0x00010421,
    F_INT32_2 = 0x00020422,
    F_INT32_3 = 0x00030423,
    F_INT32_4 = 0x00040424,

    F_UINT32_1 = 0x00010425,
    F_UINT32_2 = 0x00020426,
    F_UINT32_3 = 0x00030427,
    F_UINT32_4 = 0x00040428,

    F_NORMINT32_1 = 0x00010429,
    F_NORMINT32_2 = 0x0002042A,
    F_NORMINT32_3 = 0x0003042B,
    F_NORMINT32_4 = 0x0004042C,

    F_NORMUINT32_1 = 0x0001042D,
    F_NORMUINT32_2 = 0x0002042E,
    F_NORMUINT32_3 = 0x0003042F,
    F_NORMUINT32_4 = 0x00040430,

    F_FLOAT16_1 = 0x00010231,
    F_FLOAT16_2 = 0x00020232,
    F_FLOAT16_3 = 0x00030233,
    F_FLOAT16_4 = 0x00040234,

    F_FLOAT32_1 = 0x00010435,
    F_FLOAT32_2 = 0x00020436,
    F_FLOAT32_3 = 0x00030437,
    F_FLOAT32_4 = 0x00040438,

    F_UINT_10_10_10_L1 = 0x00010439,
    F_NORMINT_10_10_10_L1 = 0x0001043A,
    F_NORMINT_11_11_10 = 0x0001043B,
    F_NORMUINT8_4_BGRA = 0x0004013C,
    F_NORMINT_10_10_10_2 = 0x0001043D,
    F_UINT_10_10_10_2 = 0x0001043E
};

struct SemanticData
{
public:
    /// <summary>
    /// Type of data (POSITION, POSITION_BP, INDEX, NORMAL, NORMAL_BP,
    /// TEXCOORD, BLENDINDICES, BLENDWEIGHT, BONE_PALETTE, COLOR, DISPLAYLIST,
    /// MORPH_POSITION, BINORMAL_BP, TANGENT_BP).
    /// </summary>
    string name;
    /// <summary>
    /// An extra index of the data. For example, if there are 3 uv maps, then the corresponding TEXCOORD data components would have indices 0, 1, and 2, respectively.
    /// </summary>
    uint32_t index;

	SemanticData() = default;
    SemanticData(Reader* reader, const NifHeader& header) {
        name = header.getIndexString(reader);
        index = reader->read<uint32_t>();
    }
};

struct Region
{
public:
    uint32_t startIndex;
	uint32_t numIndices;

    Region() = default;
    Region(Reader* reader, const NifHeader& header) {
        startIndex = reader->read<uint32_t>();
        numIndices = reader->read<uint32_t>();
	}
};

struct NiDataStream : NiObject
{
public:
    DataStreamUsage usage;
    DataStreamAccess access;
    uint32_t numBytes;
	CloningBehavior cloningBehavior;
    uint32_t numRegions;
	vector<Region> regions;
    uint32_t numComponents;
	vector<ComponentFormat> componentFormats;
    vector<uint8_t> data;
    bool streamable;

    NiDataStream(Reader* reader, NifHeader& header) {
		usage = static_cast<DataStreamUsage>(reader->read<uint32_t>());
        access = static_cast<DataStreamAccess>(reader->read<uint32_t>());
		numBytes = reader->read<uint32_t>();
		data.reserve(numBytes); // ??
		cloningBehavior = static_cast<CloningBehavior>(reader->read<uint32_t>());
		numRegions = reader->read<uint32_t>();
        regions.reserve(numRegions);
        for (int i = 0; i < numRegions; i++) {
            regions.push_back(Region(reader, header));
        }
		numComponents = reader->read<uint32_t>();
        componentFormats.reserve(numComponents);
        for (int i = 0; i < numComponents; i++) {
            componentFormats.push_back(static_cast<ComponentFormat>(reader->read<uint32_t>()));
		}
        for (int i = 0; i < numBytes; i++) {
            data.push_back(reader->read<uint8_t>());
        }
		streamable = reader->read<bool>();
    }
};

struct DataStreamRef
{
public:
	Ref<NiDataStream> stream; // Reference to a NiDataStream
    bool isPerInstance; // Sets whether this stream data is per-instance data for use in hardware instancing.
    uint16_t numSubmeshes; // The number of submesh-to-region mappings that this data stream has.
	vector<uint16_t> submeshToRegionMap;
    uint32_t numComponents;
    vector<SemanticData> componentSemantics;

    DataStreamRef(Reader* reader, const NifHeader& header) : stream(Ref<NiDataStream>(reader)) {
        isPerInstance = reader->read<bool>();

        numSubmeshes = reader->read<uint16_t>();
		submeshToRegionMap.reserve(numSubmeshes);
		for (int i = 0; i < numSubmeshes; i++) {
			submeshToRegionMap.push_back(reader->read<uint16_t>());
		}

        numComponents = reader->read<uint32_t>();
		componentSemantics.reserve(numComponents);
        for (int i = 0; i < numComponents; i++) {
            componentSemantics.push_back(SemanticData(reader, header));
        }
    }
};

