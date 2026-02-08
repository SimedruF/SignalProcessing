#!/bin/bash

echo "Building multi-channel HDF5 test..."

g++ -std=c++11 \
    -I../source \
    -I/usr/include/hdf5/serial \
    -o test_multi_channel_h5 \
    test_multi_channel_h5.cpp \
    ../source/SignalProcessing.cpp \
    -L/usr/lib/x86_64-linux-gnu/hdf5/serial \
    -lhdf5_cpp -lhdf5 -lm -lrt

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo ""
    echo "Running test..."
    echo "================="
    ./test_multi_channel_h5
    
    if [ $? -eq 0 ]; then
        echo ""
        echo "✓ Test passed!"
        
        # Show file structure
        if command -v h5ls &> /dev/null; then
            echo ""
            echo "HDF5 File Structure:"
            echo "===================="
            h5ls -r multi_channel_sync.h5
        fi
    else
        echo "✗ Test failed!"
        exit 1
    fi
else
    echo "✗ Build failed!"
    exit 1
fi
