#pragma once
#include <cstdint>
#include <vector>

struct MatchGroup
{
public:
    uint16_t numVertices;
    vector<uint16_t> vertexIndices;

    MatchGroup(uint16_t numVertices, vector<uint16_t> vertexIndices)
        : numVertices(numVertices), vertexIndices(vertexIndices) {
    }
};