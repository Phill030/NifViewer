#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "KiWadHandler.hpp"
#include <imgui.h>
#include <commdlg.h>
#include <minwinbase.h>
#include <Windows.h>
#include <cstdint>
#include <fstream>
#include <ios>
#include <optional>
#include "NifParser/Nif.hpp"
#include <map>
#include <format>
#include "NifParser/Blocks/NiDataStream.hpp"
#include "NifParser/Blocks/NiMesh.hpp"
#include <cstring>
#include <cstdio>
#include "NifParser/Blocks/DataStream/DataStreamData.hpp"

static std::vector<std::string> droppedFiles;
void drop_callback(GLFWwindow* window, int count, const char** paths) {
    droppedFiles.clear();
    for (int i = 0; i < count; i++) {
        droppedFiles.emplace_back(paths[i]);
    }
}

struct UI
{
public:
    std::vector<std::uint8_t> openedFileData;
    KiWadHandler handler;
    std::optional<NifFile> nifFile;
    std::string openedFileName;
    bool showFileWindow = false;
    bool showFileDialog = true;

	UI() = default;

    void ShowFileDropWindow() {
        static bool fileChosen = false;
        static std::string chosenFile;

        ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->Pos);
        ImGui::SetNextWindowSize(vp->Size);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoSavedSettings;

        ImGui::Begin("File Drop", (bool*)1, flags);

        if (!droppedFiles.empty()) {
            chosenFile = droppedFiles[0];
            fileChosen = true;
            droppedFiles.clear();
        }

        ImVec2 winSize = ImGui::GetWindowSize();
        ImVec2 textSize = ImGui::CalcTextSize("Drag & Drop a file into the window");

        float totalHeight = textSize.y;
        if (fileChosen)
            totalHeight += ImGui::GetTextLineHeightWithSpacing() * 3;

        ImVec2 startPos = {
            (winSize.x - textSize.x) * 0.5f,
            (winSize.y - totalHeight) * 0.5f
        };

        ImGui::SetCursorPos(startPos);
        ImGui::BeginGroup();
        ImGui::Text("Drag & Drop a file into the window\nFile selected: %s", fileChosen ? chosenFile.substr(chosenFile.find_last_of("/\\") + 1).c_str() : "None");

        if (fileChosen) {
            if (ImGui::Button("Continue")) {
                handler.processFile(chosenFile);
                showFileDialog = false;
            }
        }
        ImGui::EndGroup();

        ImGui::End();
    }

    bool showSaveFileDialog(const string& defaultName, const vector<uint8_t>& content) {
        OPENFILENAMEW ofn;
        wchar_t szFile[MAX_PATH] = L"";
        wstring defaultFile(defaultName.begin(), defaultName.end());

        wcscpy_s(szFile, defaultFile.c_str());

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_OVERWRITEPROMPT;

        if (GetSaveFileNameW(&ofn)) {
            wstring path = ofn.lpstrFile;
            std::ofstream file(path, std::ios::binary);
            if (file.is_open()) {
                file.write(reinterpret_cast<const char*>(content.data()), content.size());
                file.close();
                return true;
            }
        }
        return false;
    }

    void DrawFileNode(KiWadHandler& handler, const vector<KiWadEntry>& entries, ImGuiTreeNodeFlags baseFlags, const string& prefix = "") {
        map<string, vector<KiWadEntry>> folders;
        vector<KiWadEntry> files;

        for (const auto& e : entries) {
            string relative = e.fileName.substr(prefix.size());
            auto slashPos = relative.find('/');

            if (slashPos != string::npos) {
                // Folder entry
                string folder = relative.substr(0, slashPos);
                folders[folder].push_back(e);
            }
            else {
                // File entry
                files.push_back(e);
            }
        }

        vector<string> folderNames;
        folderNames.reserve(folders.size());
        for (auto& [name, _] : folders)
            folderNames.push_back(name);

        // Folders
        for (const auto& folderName : folderNames) {
            if (ImGui::TreeNode(folderName.c_str())) {
                DrawFileNode(handler, folders[folderName], baseFlags, prefix + folderName + "/");
                ImGui::TreePop();
            }
        }

        // Files
        for (const auto& e : files) {
            std::string filename = e.fileName.substr(prefix.size());
            if (ImGui::TreeNodeEx(filename.c_str(), baseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
                if (ImGui::IsItemClicked()) {
                    openedFileData = handler.getFileData(e);
                    nifFile = NifFile(openedFileData);
                    openedFileName = filename;
                    showFileWindow = true;
                }

                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::Button("Save file"))
                        showSaveFileDialog(filename, handler.getFileData(e));
                    ImGui::EndPopup();
                }
            }
        }
    }

    void ShowFilesMenu() {
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse;

        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_DrawLinesFull;

        ImGui::Begin("Files", (bool*)1, flags);

        std::string treeNodeRootLabel = format("{} ({})", handler.getFileName(), handler.getFileCount());
        if (ImGui::TreeNodeEx(treeNodeRootLabel.c_str(), baseFlags | ImGuiTreeNodeFlags_DefaultOpen)) {
            DrawFileNode(handler, handler.getAllEntries(), baseFlags, "");
            ImGui::TreePop();
        }

        if (ImGui::Button("Unload file")) {
            nifFile.reset();
            openedFileData.clear();
            openedFileName.clear();
            showFileWindow = false;
        }

        ImGui::End();
    }

    void renderNifInfo() {
        if (nifFile) {
            ImGui::Begin("File Viewer: ", &showFileWindow);

            ImGui::Text(openedFileName.c_str());
            ImGui::Text("NIF Version: %s\n", nifFile.value().header.version.toString().c_str());

            auto filtered = nifFile.value().getBlocksOfType<NiMesh>();

            for (const auto& mesh : filtered) {
                for (const auto& dataStreamRef : mesh->dataStreams) {
					auto stream = dataStreamRef.stream.getReference(nifFile.value());
                    if (stream == nullptr)
						continue;

                    for (const auto& semanticData : stream->semanticData) {
                        ImGui::Text("Mesh Data Stream Semantic: %s", typeid(semanticData).name());
                    }
                }
            }
            ImGui::End();
        }
    }
};