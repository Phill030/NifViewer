#pragma once
#include "KiWadHandler.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstddef>
#include "Types/Vector3.hpp"
#include "Types/Color3.hpp"
#include "Types/TexCoord.hpp"
#include "Types/Triangle.hpp"
#include "Types/MatchGroup.hpp"
#include "Types/Color4.hpp"
#include "Types/MipMap.hpp"
#include "Types/Matrix.hpp"

using namespace std;

class Reader {
public:
    explicit Reader(const vector<uint8_t>& data) : data(&data), pos(0) {}

    template<typename T>
    T read();

	template<std::size_t N, std::size_t M, typename T = float>
	Matrix<N, M, T> readMatrix();

    std::vector<uint8_t> read(uint32_t length);
    std::string readString(size_t length);
    std::string readString();
    std::string readUntilNull();
    std::vector<uint8_t> readKiWadEntry(KiWadEntry entry);

    void seek(size_t newPos);
    size_t tell() const;

private:
    const std::vector<uint8_t>* data;
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


template<std::size_t N, std::size_t M, typename T>
Matrix<N, M, T> Reader::readMatrix() {
    Matrix<N, M, T> mat;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            mat.m[i][j] = read<T>();
        }
    }
    return mat;
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
inline Color4 Reader::read<Color4>() {
    Color4 b;
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