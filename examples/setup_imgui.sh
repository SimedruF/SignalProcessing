#!/bin/bash

echo "Setting up ImGui Demo for SignalProcessing"
echo "==========================================="

# Check if git is installed
if ! command -v git &> /dev/null; then
    echo "Error: git is not installed. Please install git first."
    exit 1
fi

# Clone ImGui if not present
if [ ! -d "imgui" ]; then
    echo "Cloning ImGui..."
    git clone --depth 1 https://github.com/ocornut/imgui.git
    echo "✓ ImGui cloned successfully"
else
    echo "✓ ImGui already exists"
fi

# Clone ImPlot if not present
if [ ! -d "implot" ]; then
    echo "Cloning ImPlot..."
    git clone --depth 1 https://github.com/epezent/implot.git
    echo "✓ ImPlot cloned successfully"
else
    echo "✓ ImPlot already exists"
fi

# Check for GLFW
echo ""
echo "Checking dependencies..."
echo "You need to install GLFW3 and OpenGL:"
echo ""
echo "Ubuntu/Debian:"
echo "  sudo apt-get install libglfw3-dev libgl1-mesa-dev"
echo ""
echo "Fedora:"
echo "  sudo dnf install glfw-devel mesa-libGL-devel"
echo ""
echo "macOS (with Homebrew):"
echo "  brew install glfw"
echo ""
echo "Windows (vcpkg):"
echo "  vcpkg install glfw3:x64-windows"
echo ""

# Create build directory
mkdir -p build

echo ""
echo "Setup complete!"
echo ""
echo "To build:"
echo "  cd build"
echo "  cmake .."
echo "  make"
echo "  ./imgui_demo"
echo ""
