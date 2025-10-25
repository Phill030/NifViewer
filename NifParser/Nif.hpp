#pragma once
#include <vector>
#include <fstream>
#include "../Reader.hpp"
#include <string>
#include <format>
#include "NifHeader.hpp"
#include <optional>
#include "Blocks/NiObject.hpp"
#include <memory>

using namespace std;

struct NifFile
{
private:
	Reader reader;

public:
    optional<NifHeader> header;
    optional<vector<shared_ptr<NiObject>>> blocks;
    
    NifFile(): reader(nullptr) {}

    NifFile(vector<char>* data);
    NifFile(const NifFile&) = default;
    NifFile& operator=(const NifFile&) = default;
};

