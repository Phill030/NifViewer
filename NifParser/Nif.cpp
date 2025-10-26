#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>
#include <memory>
#include "Nif.hpp"
#include "NifHeader.hpp"
#include "Blocks/NiNode.hpp"
#include "Blocks/NiObject.hpp"
#include "Blocks/NiZBufferProperty.hpp"
#include "Blocks/NiVertexColorProperty.hpp"
#include "Blocks/NiMesh.hpp"
#include "Blocks/NiTexturingProperty.hpp"
#include "Blocks/NiStringExtraData.hpp"
#include "Blocks/NiMaterialProperty.hpp"
#include "Blocks/NiDataStream.hpp"
#include "Blocks/NiSourceTexture.hpp"

using namespace std;

string getReadableText(const std::string& input) {
    string result;
    for (unsigned char c : input) {
        if (!isprint(c))
            break; // stop at first non-printable character
        result += c;
    }
    return result;
}

NifFile::NifFile(vector<char>* data)
    : reader(data), header(NifHeader(&reader)) {
    blocks = vector<shared_ptr<NiObject>>();

    for (uint32_t i = 0; i < header->numBlocks; i++) {
        string blockType = getReadableText(header->blockTypes[header->blockTypeIndex[i]]);
        uint32_t blockSize = header->blockSize[i];

        if (blockType == "NiNode") {
            shared_ptr<NiNode> node = make_shared<NiNode>(&reader, *header);
            blocks->push_back(node);
        }
        else if (blockType == "NiZBufferProperty") {
            shared_ptr<NiZBufferProperty> node = make_shared<NiZBufferProperty>(&reader, *header);
            blocks->push_back(node);
        }
        else if (blockType == "NiVertexColorProperty") {
            shared_ptr<NiVertexColorProperty> node = make_shared<NiVertexColorProperty>(&reader, *header);
            blocks->push_back(node);
        }
        else if (blockType == "NiMesh") {
			shared_ptr<NiMesh> node = make_shared<NiMesh>(&reader, *header);
			blocks->push_back(node);
        }else if (blockType == "NiTexturingProperty") {
            shared_ptr<NiTexturingProperty> node = make_shared<NiTexturingProperty>(&reader, *header);
            blocks->push_back(node);
		}
        else if (blockType == "NiStringExtraData") {
            shared_ptr<NiStringExtraData> node = make_shared<NiStringExtraData>(&reader, *header);
            blocks->push_back(node);
        }
        else if (blockType == "NiMaterialProperty") {
            shared_ptr<NiMaterialProperty> node = make_shared<NiMaterialProperty>(&reader, *header);
            blocks->push_back(node);
        }
        else if (blockType == "NiDataStream") {
            shared_ptr<NiDataStream> node = make_shared<NiDataStream>(&reader, *header);
            blocks->push_back(node);
        }
        else if (blockType == "NiSourceTexture") {
            shared_ptr<NiSourceTexture> node = make_shared<NiSourceTexture>(&reader, *header);
            blocks->push_back(node);
        }
        else {
            printf("Unknown block type: %s\n", blockType.c_str());
        }
    }
}