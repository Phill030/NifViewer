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
    NifHeader header;
    vector<shared_ptr<NiObject>> blocks;
    
    explicit NifFile(const std::vector<char>& data);
    NifFile(const NifFile&) = delete;
    NifFile& operator=(const NifFile&) = delete;
    NifFile& operator=(NifFile&&) = default;
    NifFile(NifFile&&) = default;
};

