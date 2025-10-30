#pragma once
#include "Blocks/NiAlphaProperty.hpp"
#include "Blocks/NiBillboardNode.hpp"
#include "Blocks/NiBooleanExtraData.hpp"
#include "Blocks/NiDataStream.hpp"
#include "Blocks/NiFloatData.hpp"
#include "Blocks/NiFloatInterpolator.hpp"
#include "Blocks/NiIntegerExtraData.hpp"
#include "Blocks/NiIntegersExtraData.hpp"
#include "Blocks/NiMaterialProperty.hpp"
#include "Blocks/NiMesh.hpp"
#include "Blocks/NiMorphWeightsController.hpp"
#include "Blocks/NiNode.hpp"
#include "Blocks/NiObject.hpp"
#include "Blocks/NiPixelData.hpp"
#include "Blocks/NiSourceTexture.hpp"
#include "Blocks/NiSpecularProperty.hpp"
#include "Blocks/NiStencilProperty.hpp"
#include "Blocks/NiStringExtraData.hpp"
#include "Blocks/NiTextureTransformController.hpp"
#include "Blocks/NiTexturingProperty.hpp"
#include "Blocks/NiTriShape.hpp"
#include "Blocks/NiTriShapeData.hpp"
#include "Blocks/NiVertexColorProperty.hpp"
#include "Blocks/NiZBufferProperty.hpp"
#include "Nif.hpp"
#include "NifHeader.hpp"
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>


using namespace std;

string getReadableText(const std::string& input) {
    string result;
    for (unsigned char c : input) {
        if (!isprint(c))
            break;
        result += c;
    }
    return result;
}

NifFile::NifFile(const std::vector<uint8_t>& data): reader(data), header(reader) {
    const unordered_map<string, function<shared_ptr<NiObject>(Reader&, NifHeader&)>> factories = {
            {"NiNode", [](Reader& r, NifHeader& h) { return make_shared<NiNode>(r, h); }},
            {"NiZBufferProperty", [](Reader& r, NifHeader& h) { return make_shared<NiZBufferProperty>(r, h); }},
            {"NiVertexColorProperty", [](Reader& r, NifHeader& h) { return make_shared<NiVertexColorProperty>(r, h); }},
            {"NiMesh", [](Reader& r, NifHeader& h) { return make_shared<NiMesh>(r, h); }},
            {"NiTexturingProperty", [](Reader& r, NifHeader& h) { return make_shared<NiTexturingProperty>(r, h); }},
            {"NiStringExtraData", [](Reader& r, NifHeader& h) { return make_shared<NiStringExtraData>(r, h); }},
            {"NiMaterialProperty", [](Reader& r, NifHeader& h) { return make_shared<NiMaterialProperty>(r, h); }},
            {"NiDataStream", [](Reader& r, NifHeader& h) { return make_shared<NiDataStream>(r, h); }},
            {"NiSourceTexture", [](Reader& r, NifHeader& h) { return make_shared<NiSourceTexture>(r, h); }},
            {"NiAlphaProperty", [](Reader& r, NifHeader& h) { return make_shared<NiAlphaProperty>(r, h); }},
            {"NiSpecularProperty", [](Reader& r, NifHeader& h) { return make_shared<NiSpecularProperty>(r, h); }},
            {"NiStencilProperty", [](Reader& r, NifHeader& h) { return make_shared<NiStencilProperty>(r, h); }},
            {"NiTextureTransformController", [](Reader& r, NifHeader& h) { return make_shared<NiTextureTransformController>(r, h); }},
            {"NiFloatInterpolator", [](Reader& r, NifHeader& h) { return make_shared<NiFloatInterpolator>(r, h); }},
            {"NiFloatData", [](Reader& r, NifHeader& h) { return make_shared<NiFloatData>(r, h); }},
            {"NiMorphWeightsController", [](Reader& r, NifHeader& h) { return make_shared<NiMorphWeightsController>(r, h); }},
            {"NiBillboardNode", [](Reader& r, NifHeader& h) { return make_shared<NiBillboardNode>(r, h); }},
            {"NiBooleanExtraData", [](Reader& r, NifHeader& h) { return make_shared<NiBooleanExtraData>(r, h); }},
            {"NiIntegerExtraData", [](Reader& r, NifHeader& h) { return make_shared<NiIntegerExtraData>(r, h); }},
            {"NiIntegersExtraData", [](Reader& r, NifHeader& h) { return make_shared<NiIntegersExtraData>(r, h); }},
            {"NiTriShape", [](Reader& r, NifHeader& h) { return make_shared<NiTriShape>(r, h); }},
            {"NiTriShapeData", [](Reader& r, NifHeader& h) { return make_shared<NiTriShapeData>(r, h); }},
            {"NiPixelData", [](Reader& r, NifHeader& h) { return make_shared<NiPixelData>(r, h); }},
    };

    for (uint32_t i = 0; i < header.numBlocks; ++i) {
        string blockType = getReadableText(header.blockTypes[header.blockTypeIndex[i]]);
        uint32_t blockSize = header.blockSize[i];

        printf("Current index: %u, blockType: %s\n", i, blockType.c_str());

        auto it = factories.find(blockType);
        if (it != factories.end()) {
            blocks.push_back(it->second(reader, header));
        }
        else {
            printf("Unknown block type: %s\n", blockType.c_str());
            reader.read(blockSize);
        }
    }
}