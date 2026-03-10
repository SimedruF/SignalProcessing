#!/bin/bash
# Build script for ML HDF5 export test (Linux)

echo "Building test_ml_h5export..."

# Note: Requires HDF5 C++ library installed
# On Ubuntu/Debian: sudo apt-get install libhdf5-dev libhdf5-cpp-103
# On Fedora/RHEL: sudo dnf install hdf5-devel

g++ -std=c++11 -DUSE_HDF5 -o test_ml_h5export test_ml_h5export.cpp ../source/SignalProcessing.cpp -I../source -lhdf5_cpp -lhdf5 -lm -lrt

if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./test_ml_h5export"
    chmod +x test_ml_h5export
    echo ""
    echo "Note: Make sure HDF5 libraries are installed:"
    echo "  Ubuntu/Debian: sudo apt-get install libhdf5-dev"
    echo "  Fedora/RHEL: sudo dnf install hdf5-devel"
else
    echo "Build failed!"
    echo ""
    echo "Make sure HDF5 C++ library is installed."
    echo "You may need to specify include/lib paths manually:"
    echo "  -I/usr/include/hdf5/serial -L/usr/lib/x86_64-linux-gnu/hdf5/serial"
    exit 1
fi
