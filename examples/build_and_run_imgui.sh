#!/bin/bash

# Build and Run ImGui Demo for SignalProcessing
# This script builds the ImGui demo and launches it automatically

echo "================================================"
echo "  SignalProcessing ImGui Demo - Build & Run"
echo "================================================"
echo ""

# Check if setup has been run
if [ ! -d "imgui" ] || [ ! -d "implot" ]; then
    echo "⚠️  ImGui/ImPlot not found!"
    echo "Running setup script first..."
    echo ""
    chmod +x setup_imgui.sh
    ./setup_imgui.sh
    if [ $? -ne 0 ]; then
        echo "❌ Setup failed!"
        exit 1
    fi
    echo ""
fi

# Check for HDF5 development libraries
if ! dpkg -l | grep -q libhdf5-dev; then
    echo "⚠️  HDF5 development libraries not found!"
    echo "Installing libhdf5-dev..."
    echo ""
    sudo apt install -y libhdf5-dev
    if [ $? -ne 0 ]; then
        echo "❌ Failed to install HDF5 libraries!"
        echo "Please run: sudo apt install libhdf5-dev"
        exit 1
    fi
    echo ""
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir build
fi

cd build

echo "🔨 Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed!"
    exit 1
fi

echo ""
echo "🔨 Building..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo ""
echo "✅ Build successful!"
echo ""
echo "================================================"
echo "  Launching ImGui Demo..."
echo "================================================"
echo ""
echo "💡 Tips:"
echo "  - Generate different signal types"
echo "  - Apply filters in real-time"
echo "  - Detect anomalies"
echo "  - Compute FFT spectrum"
echo "  - Extract ML features"
echo "  - Save to HDF5 file"
echo ""
echo "Press Ctrl+C or close window to exit"
echo ""

# Run the demo
./imgui_demo

echo ""
echo "Demo closed. Goodbye! 👋"
