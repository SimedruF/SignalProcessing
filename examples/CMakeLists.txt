cmake_minimum_required(VERSION 3.10)
project(SignalProcessingImGuiDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find HDF5 (C++ bindings)
find_package(HDF5 REQUIRED COMPONENTS CXX)
if(HDF5_FOUND)
    message(STATUS "Found HDF5: ${HDF5_VERSION}")
    message(STATUS "HDF5 include dirs: ${HDF5_INCLUDE_DIRS}")
    message(STATUS "HDF5 libraries: ${HDF5_CXX_LIBRARIES}")
endif()

# Download and build GLFW automatically
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

# Configure GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../source
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/implot
    ${HDF5_INCLUDE_DIRS}
)

# ImGui sources
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# ImPlot sources
set(IMPLOT_SOURCES
    implot/implot.cpp
    implot/implot_items.cpp
)

# SignalProcessing source
set(SIGNALPROCESSING_SOURCES
    ../source/SignalProcessing.cpp
)

# Main executable
add_executable(imgui_demo
    imgui_demo.cpp
    ${IMGUI_SOURCES}
    ${IMPLOT_SOURCES}
    ${SIGNALPROCESSING_SOURCES}
)

# Link libraries
target_link_libraries(imgui_demo
    OpenGL::GL
    glfw
    ${CMAKE_DL_LIBS}
    ${HDF5_CXX_LIBRARIES}
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(imgui_demo PRIVATE WINDOWS)
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(imgui_demo m rt)
endif()
