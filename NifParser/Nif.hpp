#pragma once
#include <vector>
#include "../Reader.hpp"
#include "NifHeader.hpp"
#include "Blocks/NiObject.hpp"
#include <memory>
#include <type_traits>

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

    template<typename T>
    vector<T*> getBlocksOfType() {
        static_assert(std::is_base_of_v<NiObject, T>, "t must derive from niobject");

        std::vector<T*> result;

        for (const auto& obj : blocks) {
            if (auto* casted = dynamic_cast<T*>(obj.get())) {
                result.push_back(casted);
            }
        }

        return result;
    }
};

