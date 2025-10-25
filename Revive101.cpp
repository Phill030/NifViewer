#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <commdlg.h>

#include "Revive101.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <format>
#include "KiWadHandler.hpp"
#include <map>
#include <fstream>
#include <minwinbase.h>
#include "NifParser/Nif.hpp"
#include "NifParser/Blocks/NiNode.hpp"
#include <cstring>
#include <gl/GL.h>
#include <memory>
#include <string>

using namespace std;

vector<string> droppedFiles;
void drop_callback(GLFWwindow* window, int count, const char** paths) {
    droppedFiles.clear();
    for (int i = 0; i < count; i++) {
        droppedFiles.emplace_back(paths[i]);
    }
}


void ShowFileDropWindow(KiWadHandler* handler, bool* showFileDialog) {
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
    if (fileChosen) totalHeight += ImGui::GetTextLineHeightWithSpacing() * 3;

    ImVec2 startPos = {
        (winSize.x - textSize.x) * 0.5f,
        (winSize.y - totalHeight) * 0.5f
    };

    ImGui::SetCursorPos(startPos);
    ImGui::BeginGroup();
    ImGui::Text("Drag & Drop a file into the window\nFile selected: %s", fileChosen ? chosenFile.substr(chosenFile.find_last_of("/\\") + 1).c_str() : "None");

    if (fileChosen) {
        if (ImGui::Button("Continue")) {
            handler->processFile(chosenFile);
            *showFileDialog = false;
        }
    }
    ImGui::EndGroup();

    ImGui::End();
}

bool showSaveFileDialog(const string& defaultName, const vector<char>& content) {
    OPENFILENAMEW ofn;
    wchar_t szFile[MAX_PATH] = L"";
    wstring defaultFile(defaultName.begin(), defaultName.end());

    wcscpy_s(szFile, defaultFile.c_str());

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    //ofn.lpstrFilter = L"All Files\0*.*\0";
    //ofn.lpstrDefExt = L"txt";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn)) {
        wstring path = ofn.lpstrFile;
        ofstream file(path, ios::binary);
        if (file.is_open()) {
            file.write(content.data(), content.size());
            file.close();
            return true;
        }
    }
    return false;
}


static vector<char> openedFileData;
static NifFile nif;
static string openedFileName;
static bool showFileWindow = false;

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

    // Render folders
    for (const auto& folderName : folderNames) {
        if (ImGui::TreeNode(folderName.c_str())) {
            DrawFileNode(handler, folders[folderName], baseFlags, prefix + folderName + "/");
            ImGui::TreePop();
        }
    }

    // Render files
    for (const auto& e : files) {
        // Show only the filename
        std::string filename = e.fileName.substr(prefix.size());
        if (ImGui::TreeNodeEx(filename.c_str(), baseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
            if (ImGui::IsItemClicked()) {
                openedFileData = handler.getFileData(e);
				nif = NifFile(&openedFileData);
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

void ShowFilesMenu(KiWadHandler handler) {
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

    ImGui::End();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(720, 480, "Revive101", nullptr, nullptr);
    if (!window) {
        cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetDropCallback(window, drop_callback);


    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main loop
    KiWadHandler handler;
    bool showFileDialog = true;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Dockspace
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());


        // Your custom window
        if (showFileDialog)
            ShowFileDropWindow(&handler, &showFileDialog);
        else {
            ShowFilesMenu(handler);
        }


        // Render everything in another window
        ImGui::Begin("File Viewer: ", &showFileWindow);

        if (!openedFileData.empty() && nif.header.has_value()) {
            ImGui::Text(openedFileName.c_str());
            ImGui::Text("NIF Version: %s", nif.header.value().version.toString().c_str());

            for (auto& obj : *nif.blocks) {
                if (auto node = dynamic_pointer_cast<NiNode>(obj)) {
					ImGui::Text("NiNode: %s", node->name.c_str());
                }
            }
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

