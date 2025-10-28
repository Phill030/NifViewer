#pragma once
#include "KiWadHandler.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstddef>
#include "Types/Vector3.hpp"
#include "Types/Matrix33.hpp"
#include "Types/Matrix22.hpp"
#include "Types/Color3.hpp"
#include "Types/TexCoord.hpp"
#include "Types/Triangle.hpp"
#include "Types/MatchGroup.hpp"
#include "Types/ByteColor4.hpp"
#include "Types/MipMap.hpp"

class Reader {
public:
    explicit Reader(const vector<char>& data) : data(&data), pos(0) {}

    template<typename T>
    T read();

    std::vector<uint8_t> read(uint32_t length);
    std::string readString(size_t length);
    std::string readString();
    std::string readUntilNull();
    std::vector<char> readKiWadEntry(KiWadEntry entry);

    void seek(size_t newPos);
    size_t tell() const;

private:
    const std::vector<char>* data;
    size_t pos;
};

template<typename T>
T Reader::read() {
    if (pos + sizeof(T) > data->size())
        throw std::out_of_range("Read past end of buffer");

    T value;
    std::memcpy(&value, data->data() + pos, sizeof(T));
    pos += sizeof(T);
    return value;
}

template<>
inline Vector3 Reader::read<Vector3>() {
    Vector3 v;
    v.x = read<float>();
    v.y = read<float>();
    v.z = read<float>();
    return v;
}

template<>
inline Matrix33 Reader::read<Matrix33>() {
    Matrix33 m;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            m.m[r][c] = read<float>();
        }
    }
    return m;
}

template<>
inline Matrix22 Reader::read<Matrix22>() {
    Matrix22 m;
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 2; c++) {
            m.m[r][c] = read<float>();
        }
    }
    return m;
}

template<>
inline Color3 Reader::read<Color3>() {
    Color3 c;
    c.r = read<float>();
    c.g = read<float>();
    c.b = read<float>();
	return c;
}

template<>
inline TexCoord Reader::read<TexCoord>() {
    TexCoord t;
    t.u = read<float>();
    t.v = read<float>();
    return t;
}

template<>
inline Triangle Reader::read<Triangle>() {
    Triangle t;
    t.v1 = read<uint16_t>();
    t.v2 = read<uint16_t>();
    t.v3 = read<uint16_t>();
    return t;
}

template<>
inline MatchGroup Reader::read<MatchGroup>() {
	uint16_t numVertices = read<uint16_t>();
	vector<uint16_t> vertexIndices;
	vertexIndices.reserve(numVertices);
	for (uint16_t i = 0; i < numVertices; i++) {
		vertexIndices.push_back(read<uint16_t>());
	}

	MatchGroup m(numVertices, vertexIndices);

    return m;
}

template<>
inline ByteColor4 Reader::read<ByteColor4>() {
    ByteColor4 b;
	b.r = read<uint8_t>();
	b.g = read<uint8_t>();
	b.b = read<uint8_t>();
    b.a = read<uint8_t>();
    return b;
}

template<>
inline MipMap Reader::read<MipMap>() {
    MipMap m;
    m.width = read<uint32_t>();
    m.height = read<uint32_t>();
    m.offset = read<uint32_t>();
    return m;
}