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
#include "Types/Color3.hpp"

class Reader {
public:
    explicit Reader(const std::vector<char>* data);

    template<typename T>
    T read();
    
    std::vector<char> read(uint32_t length);
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
inline Color3 Reader::read<Color3>() {
    Color3 c;
    c.r = read<float>();
    c.g = read<float>();
    c.b = read<float>();
	return c;
}