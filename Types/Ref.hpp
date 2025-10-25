#pragma once
#include <cstdint>
#include <memory>
#include <type_traits>
#include "../NifParser/Blocks/NiObject.hpp"
#include "../Reader.hpp"
#include "../NifParser/Nif.hpp"

using namespace std;

template <typename T>
class Ref
{
    static_assert(is_base_of_v<NiObject, T>, "T must derive from NiObject");

public:
    int32_t value;

    explicit Ref(Reader* reader) {
        value = reader->read<int32_t>();
    }

    T* getReference(NifFile& file) const {
        if (value == -1)
            return nullptr;

        if (value < 0 || static_cast<size_t>(value) >= file.blocks.value().size())
            return nullptr;

        return dynamic_cast<T*>(file.blocks.value()[value]);
    }

    bool hasReference() const {
        return value != -1;
    }
};
