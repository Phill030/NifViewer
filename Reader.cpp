#include "KiWadHandler.hpp"
#include "Reader.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

string Reader::readString(size_t length) {
    if (pos + length > data->size())
        throw out_of_range("Read past end of buffer");

    string s(reinterpret_cast<const char*>(&(*data)[pos]), length);
    pos += length;
    return s;
}

string Reader::readString() {
	uint32_t length = read<uint32_t>();
	return readString(length);
}

string Reader::readUntilNull() {
    size_t start = pos;
    while (pos < data->size() && (*data)[pos] != 0x0A) {
        pos++;
    }
    if (pos >= data->size())
        throw out_of_range("Null terminator not found");
    string s(reinterpret_cast<const char*>(&(*data)[start]), pos - start);
    pos++; 
    return s;
}

vector<uint8_t> Reader::read(uint32_t length) {
    if (pos + length > data->size())
        throw out_of_range("Read past end of buffer");
    vector<uint8_t> buf(data->data() + pos, data->data() + pos + length);
    pos += length;
    return buf;
}

vector<uint8_t> Reader::readKiWadEntry(KiWadEntry entry) {
    if (!data || data->size() == 0)
        throw std::runtime_error("No data buffer");

    uint32_t len = entry.zipped ? entry.zipSize : entry.size;
    if (entry.offset > data->size() || entry.offset + len > data->size())
        throw std::out_of_range("Entry outside buffer");

    return std::vector<uint8_t>(data->data() + entry.offset, data->data() + entry.offset + len);
}

void Reader::seek(size_t newPos) { pos = newPos; }
size_t Reader::tell() const { return pos; }