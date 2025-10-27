#pragma once
#include <cstdint>
#include <string>
#include <format>
#include <vector>
#include "../Reader.hpp"

using namespace std;

struct Version
{
public:
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
	uint8_t reserved;

	string toString() const {
		return format("{}.{}.{}.{}", major, minor, patch, reserved);
	}
};

struct NifHeader
{
public:
	string header;
	Version version;
	uint8_t endiannes;
	uint32_t userVersion;
	uint32_t numBlocks;
	uint16_t numBlockTypes;
	vector<string> blockTypes;
	vector<uint16_t> blockTypeIndex;
	vector<uint32_t> blockSize;
	uint32_t maxStringLength;
	vector<string> strings;
	vector<uint32_t> groups;

	Version readVersion(Reader& reader) const {
		uint32_t ver = reader.read<uint32_t>();
		Version v;
		v.major = (ver >> 24) & 0xFF;
		v.minor = (ver >> 16) & 0xFF;
		v.patch = (ver >> 8) & 0xFF;
		v.reserved = ver & 0xFF;
		return v;
	}

	string getIndexString(Reader& reader) const {
		int32_t stringIndex = reader.read<int32_t>();
		if (stringIndex == 0xFFFFFFFF) {
			return "";
		}

		return strings[stringIndex];
	}

	NifHeader() = default;
	NifHeader(Reader& reader) {
		header = reader.readUntilNull();
		version = readVersion(reader);
		endiannes = reader.read<uint8_t>();
		userVersion = reader.read<uint32_t>();
		numBlocks = reader.read<uint32_t>();
		numBlockTypes = reader.read<uint16_t>();
		blockTypes.reserve(numBlockTypes);
		blockTypeIndex.reserve(numBlocks);
		blockSize.reserve(numBlocks);
		
		for (int i = 0; i < numBlockTypes; i++) {
			string name = reader.readString();
			blockTypes.push_back(name);
		}
		
		for (int i = 0; i < numBlocks; i++) {
			blockTypeIndex.push_back(reader.read<uint16_t>());
		}

		for (int i = 0; i < numBlocks; i++) {
			blockSize.push_back(reader.read<uint32_t>());
		}

		strings.reserve(reader.read<uint32_t>());
		maxStringLength = reader.read<uint32_t>();
		for (int i = 0; i < strings.capacity(); i++) {
			strings.push_back(reader.readString());
		}

		groups.reserve(reader.read<uint32_t>());
		for (int i = 0; i < groups.capacity(); i++) {
			groups.push_back(reader.read<uint32_t>());
		}
	}
};