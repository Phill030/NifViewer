#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <commdlg.h>

#include "Revive101.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <vector>
#include <format>
#include <map>
#include <fstream>
#include <minwinbase.h>
#include <string>
#include <optional>
#include "KiWadHandler.hpp"
#include "NifParser/Nif.hpp"
#include "NifParser/Blocks/NiNode.hpp"
#include "UI.hpp"

using namespace std;

// -----------------[ADDED: Simple OpenGL shader helpers]-----------------

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

struct SphereMesh
{
    GLuint vao, vbo, ebo;
    GLsizei indexCount;
};

SphereMesh createSphereMesh(float radius, int stacks, int slices) {
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

    SphereMesh mesh;
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

// -----------------[ADDED: FBO for 3D preview]-----------------
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

// ----------------------------------------------------------------------

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(720, 480, "Revive101", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    glfwSwapInterval(1);

    UI ui;
    glfwSetDropCallback(window, drop_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // [ADDED INIT]
    GLuint sphereShader = createSphereShader();
    SphereMesh sphere = createSphereMesh(1.0f, 32, 32);
    initFBO();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (ui.showFileDialog)
            ui.ShowFileDropWindow();
        else
            ui.ShowFilesMenu();

        // ----------------------------------------------------------------------
        // 3D PREVIEW WINDOW WITH CONTROLS
        // ----------------------------------------------------------------------

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



        // Render everything in another window
        ui.renderNifInfo();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
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
