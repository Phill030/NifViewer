#pragma once
#include <glad/glad.h>
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
#include "NifParser/Blocks/DataStream/DataStreamPosition.hpp"
#include <cstring>
#include <type_traits>
#include "NifParser/Blocks/DataStream/DataStreamColor.hpp"
#include "NifParser/Blocks/DataStream/DataStreamNormal.hpp"
#include "NifParser/Blocks/DataStream/DataStreamIndex.hpp"
#include "NifParser/Blocks/DataStream/DataStreamTexcoord.hpp"


GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        cerr << "Shader compile error: " << log << endl;
    }
    return shader;
}

GLuint createSphereShader() {
    const char* vs = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;
        out vec3 Normal;
        out vec3 FragPos;
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = proj * view * vec4(FragPos, 1.0);
        }
    )";

    const char* fs = R"(
        #version 330 core
        in vec3 Normal;
        in vec3 FragPos;
        out vec4 FragColor;
        uniform vec3 lightDir;
        uniform vec3 baseColor;
        void main() {
            float diff = max(dot(normalize(Normal), normalize(-lightDir)), 0.0);
            vec3 color = baseColor * diff + baseColor * 0.1;
            FragColor = vec4(color, 1.0);
        }
    )";

    GLuint vert = compileShader(GL_VERTEX_SHADER, vs);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fs);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}

// -----------------[ADDED: Sphere mesh generation]-----------------
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
};

struct Mesh
{
    GLuint vao, vbo, ebo;
    GLsizei indexCount;
};

Mesh createSphereMesh(float radius, int stacks, int slices) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i) {
        float v = (float)i / stacks;
        float phi = v * glm::pi<float>();
        for (int j = 0; j <= slices; ++j) {
            float u = (float)j / slices;
            float theta = u * glm::two_pi<float>();

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            vertices.push_back({ glm::vec3(radius * x, radius * y, radius * z), glm::vec3(x, y, z) });
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    Mesh mesh;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    mesh.indexCount = indices.size();
    return mesh;
}

GLuint fbo = 0, fboTex = 0, fboDepth = 0;
int fboWidth = 512, fboHeight = 512;

void initFBO() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &fboTex);
    glBindTexture(GL_TEXTURE_2D, fboTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);

    glGenRenderbuffers(1, &fboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fboWidth, fboHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cerr << "FBO incomplete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

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
    GLuint sphereShader = createSphereShader();
    Mesh sphere = createSphereMesh(1.0f, 32, 32);

    UI() {
        initFBO();
    }

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
			ImGui::Text("Meshes in file: %zu", filtered.size());
            for (const auto& mesh : filtered) {
                for (const auto& streamRef : mesh->dataStreams) {
                    auto stream = streamRef.stream.getReference(nifFile.value());
                    if(stream == nullptr) continue;
                    for (const auto& semantic : stream->semanticData) {
                        if (auto index = dynamic_cast<DataStreamIndex*>(semantic.get()))
                            ImGui::Text(" - Index Data Stream: %zu indices", index->values.size());
                        else if (auto normal = dynamic_cast<DataStreamNormal*>(semantic.get()))
                            ImGui::Text(" - Normal Data Stream: %zu normals", normal->values.size());
                        else if (auto color = dynamic_cast<DataStreamColor*>(semantic.get()))
                            ImGui::Text(" - Color Data Stream: %zu colors", color->values.size());
                        else if (auto position = dynamic_cast<DataStreamPosition*>(semantic.get()))
                            ImGui::Text(" - Position Data Stream: %zu positions", position->values.size());
                        else if (auto position = dynamic_cast<DataStreamTexcoord*>(semantic.get()))
							ImGui::Text(" - TexCoord Data Stream: %zu texcoords", position->values.size());


                    }
                }
            }

            ImGui::End();
        }
    }

    void render3DPreview() {
        static float sphereRadius = 1.0f;
        static float rotationSpeed = 1.0f;
        static glm::vec3 sphereColor = { 0.2f, 0.6f, 0.9f };
        static glm::vec3 backgroundColor = { 0.1f, 0.1f, 0.2f };
        static float resolutionScale = 1.0f; // scale relative to ImGui window size

        static int lastFboWidth = fboWidth;
        static int lastFboHeight = fboHeight;

        ImGui::Begin("3D Preview");

        // --- Controls section ---
        ImGui::Text("Sphere Controls");
        ImGui::SliderFloat("Radius", &sphereRadius, 0.1f, 5.0f, "%.2f");
        ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.1f, 5.0f, "%.2fx");
        ImGui::ColorEdit3("Sphere Color", (float*)&sphereColor);
        ImGui::ColorEdit3("Background", (float*)&backgroundColor);
        ImGui::SliderFloat("Resolution Scale", &resolutionScale, 0.5f, 2.0f, "%.1fx");

        ImGui::Separator();
        ImGui::Text("Preview:");

        // Determine available space for the preview texture
        ImVec2 avail = ImGui::GetContentRegionAvail();
        int targetWidth = std::max(64, (int)(avail.x * resolutionScale));
        int targetHeight = std::max(64, (int)(avail.y * resolutionScale));

        // Recreate FBO if the window size or scale changed
        if (targetWidth != lastFboWidth || targetHeight != lastFboHeight) {
            lastFboWidth = targetWidth;
            lastFboHeight = targetHeight;
            fboWidth = targetWidth;
            fboHeight = targetHeight;

            glDeleteTextures(1, &fboTex);
            glDeleteRenderbuffers(1, &fboDepth);
            glDeleteFramebuffers(1, &fbo);
            initFBO();
        }

        // --- Render sphere to resized FBO ---
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, fboWidth, fboHeight);
        glEnable(GL_DEPTH_TEST);
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(sphereShader);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)fboWidth / fboHeight, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0), glm::vec3(0, 1, 0));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * rotationSpeed, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(sphereRadius));

        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sphereShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(sphereShader, "lightDir"), -1.0f, -1.0f, -1.0f);

        // Update fragment color via a uniform (added to shader below)
        GLint colorLoc = glGetUniformLocation(sphereShader, "baseColor");
        if (colorLoc >= 0)
            glUniform3f(colorLoc, sphereColor.r, sphereColor.g, sphereColor.b);

        glBindVertexArray(sphere.vao);
        glDrawElements(GL_TRIANGLES, sphere.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // --- Show FBO texture in ImGui ---
        ImVec2 previewSize(avail.x, avail.y);
        ImGui::Image((ImTextureID)(intptr_t)fboTex, previewSize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
};