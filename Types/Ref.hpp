#pragma once
#include "../NifParser/Blocks/NiObject.hpp"
#include "../Reader.hpp"
#include <cstdint>
#include <memory>
#include <type_traits>

// Forward declaration
struct NifFile;

template <typename T>
struct Ref
{
public:
    static_assert(std::is_base_of_v<NiObject, T>, "T must derive from NiObject");
    int32_t value;

    explicit Ref(Reader& reader) {
        value = reader.read<int32_t>();
    }

    // Declare but don't define
    T* getReference(const NifFile& file) const;

    bool hasReference() const { return value != -1; }
};