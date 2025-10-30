#pragma once
#include <vector>
#include "../Reader.hpp"
#include "NifHeader.hpp"
#include "Blocks/NiObject.hpp"
#include <memory>
#include <type_traits>
#include "Blocks/NiNode.hpp"
#include "../Types/Ref.hpp"

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

    template<typename T>
    T* getReference(const Ref<T>& ref) const {
        static_assert(std::is_base_of_v<NiObject, T>, "T must derive from NiObject");

        if (ref.value == -1)
            return nullptr;

        if (ref.value < 0 || static_cast<size_t>(ref.value) >= blocks.size())
            return nullptr;

        NiObject* obj = blocks[ref.value].get();
		return dynamic_cast<T*>(obj);
    }
};

