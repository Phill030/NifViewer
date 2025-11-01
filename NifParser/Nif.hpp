#pragma once
#include <vector>
#include "../Reader.hpp"
#include "NifHeader.hpp"
#include "Blocks/NiObject.hpp"
#include <memory>
#include <type_traits>
#include "Blocks/NiNode.hpp"

using namespace std;

struct NifFile
{
private:
	Reader reader;

public:
    NifHeader header;
    vector<shared_ptr<NiObject>> blocks;
    
    explicit NifFile(const vector<uint8_t>& data);
    NifFile(const NifFile&) = delete;
    NifFile& operator=(const NifFile&) = delete;
    NifFile& operator=(NifFile&&) = default;
    NifFile(NifFile&&) = default;

    template<typename T>
    vector<T*> getBlocksOfType() {
        static_assert(std::is_base_of_v<NiObject, T>, "T must derive from NiObject");

        std::vector<T*> result;

        for (const auto& obj : blocks) {
            if (auto* casted = dynamic_cast<T*>(obj.get())) {
                result.push_back(casted);
            }
        }

        return result;
    }

    vector<shared_ptr<NiNode>> getRootNodes() {
        vector<shared_ptr<NiNode>> roots;

        for (const auto& block : blocks) {
            if (auto node = dynamic_pointer_cast<NiNode>(block)) {
                roots.push_back(node);
            }
		}

		return roots;
    }
};

// NifFile is now fully defined
template <typename T>
T* Ref<T>::getReference(const NifFile& file) const {
    if (value == -1)
        return nullptr;

    if (value < 0 || static_cast<size_t>(value) >= file.blocks.size())
        return nullptr;

    return dynamic_cast<T*>(file.blocks[value].get());
}