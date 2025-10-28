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

NifFile::NifFile(const std::vector<char>& data): reader(data), header(reader) {
    blocks.reserve(header.numBlocks);

    for (uint32_t i = 0; i < header.numBlocks; i++) {
        string blockType = getReadableText(header.blockTypes[header.blockTypeIndex[i]]);
        uint32_t blockSize = header.blockSize[i];

		printf("Current index: %d, blockType: %s\n", i, blockType.c_str());
        if (blockType == "NiNode") {
            shared_ptr<NiNode> node = make_shared<NiNode>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiZBufferProperty") {
            shared_ptr<NiZBufferProperty> node = make_shared<NiZBufferProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiVertexColorProperty") {
            shared_ptr<NiVertexColorProperty> node = make_shared<NiVertexColorProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiMesh") {
			shared_ptr<NiMesh> node = make_shared<NiMesh>(reader, header);
			blocks.push_back(node);
        }
        else if (blockType == "NiTexturingProperty") {
            shared_ptr<NiTexturingProperty> node = make_shared<NiTexturingProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiStringExtraData") {
            shared_ptr<NiStringExtraData> node = make_shared<NiStringExtraData>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiMaterialProperty") {
            shared_ptr<NiMaterialProperty> node = make_shared<NiMaterialProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiDataStream") {
            shared_ptr<NiDataStream> node = make_shared<NiDataStream>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiSourceTexture") {
            shared_ptr<NiSourceTexture> node = make_shared<NiSourceTexture>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiAlphaProperty") {
            shared_ptr<NiAlphaProperty> node = make_shared<NiAlphaProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiSpecularProperty") {
            shared_ptr<NiSpecularProperty> node = make_shared<NiSpecularProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiStencilProperty") {
            shared_ptr<NiStencilProperty> node = make_shared<NiStencilProperty>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiTextureTransformController") {
            shared_ptr<NiTextureTransformController> node = make_shared<NiTextureTransformController>(reader);
            blocks.push_back(node);
        }
        else if (blockType == "NiFloatInterpolator") {
            shared_ptr<NiFloatInterpolator> node = make_shared<NiFloatInterpolator>(reader);
            blocks.push_back(node);
        }
        else if (blockType == "NiFloatData") {
            shared_ptr<NiFloatData> node = make_shared<NiFloatData>(reader);
            blocks.push_back(node);
        }
        else if (blockType == "NiMorphWeightsController") {
            shared_ptr<NiMorphWeightsController> node = make_shared<NiMorphWeightsController>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiBillboardNode") {
            shared_ptr<NiBillboardNode> node = make_shared<NiBillboardNode>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiBooleanExtraData") {
            shared_ptr<NiBooleanExtraData> node = make_shared<NiBooleanExtraData>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiIntegerExtraData") {
            shared_ptr<NiIntegerExtraData> node = make_shared<NiIntegerExtraData>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiIntegersExtraData") {
            shared_ptr<NiIntegersExtraData> node = make_shared<NiIntegersExtraData>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiTriShape") {
            shared_ptr<NiTriShape> node = make_shared<NiTriShape>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiTriShapeData") {
            shared_ptr<NiTriShapeData> node = make_shared<NiTriShapeData>(reader, header);
            blocks.push_back(node);
        }
        else if (blockType == "NiPixelData") {
            shared_ptr<NiPixelData> node = make_shared<NiPixelData>(reader, header);
            blocks.push_back(node);
        }
        else {
            printf("Unknown block type: %s\n", blockType.c_str());
			reader.read(blockSize);
        }
    }
}