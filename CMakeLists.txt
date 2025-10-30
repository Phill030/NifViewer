cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_TOOLCHAIN_FILE "C:/VCPKG/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project ("Revive101")

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ImGui setup
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

# zLib
add_subdirectory(external/zlib)

# GLM
add_subdirectory(external/glm)

add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link ImGui with required libraries
target_link_libraries(imgui PUBLIC 
    OpenGL::GL
    glfw
)

# Add source to this project's executable.
add_executable (Revive101 
    "Revive101.cpp"
    "Revive101.h"
    "KiWadHandler.cpp"
    "KiWadHandler.hpp"
    "Reader.cpp"
    "Reader.hpp"
    "NifParser/Nif.cpp"
    "NifParser/Nif.hpp"
    "UI.hpp"
    "glad.c"
 )

 target_include_directories(Revive101 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Revive101 PROPERTY CXX_STANDARD 20)
endif()

# Link ImGui, zLib & NifLib to executable
target_link_libraries(Revive101 PRIVATE imgui)
target_link_libraries(Revive101 PRIVATE zlibstatic)
target_link_libraries(Revive101 PRIVATE glm)



# TODO: Add tests and install targets if needed.