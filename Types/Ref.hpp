#pragma once
#include "../NifParser/Blocks/NiObject.hpp"
#include "../NifParser/Nif.hpp"
#include "../Reader.hpp"
#include <cstdint>
#include <memory>
#include <type_traits>

using namespace std;

template <typename T>
class Ref
{
public:
    int32_t value;

    explicit Ref(Reader& reader) {
        value = reader.read<int32_t>();
    }


    //T* getReference(NifFile& file) const {
    //    static_assert(is_base_of_v<NiObject, T>, "T must derive from NiObject");

    //    if (value == -1)
    //        return nullptr;

    //    if (value < 0 || static_cast<size_t>(value) >= file.blocks.size())
    //        return nullptr;

    //    return dynamic_cast<T*>(file.blocks[value].get());
    //}



    bool hasReference() const {
        return value != -1;
    }
};
