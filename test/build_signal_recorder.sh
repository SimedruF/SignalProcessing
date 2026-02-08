#!/bin/bash

echo "Building SignalRecorder test..."

g++ -std=c++11 \
    -I../source \
    -I/usr/include/hdf5/serial \
    -o test_signal_recorder \
    test_signal_recorder.cpp \
    -L/usr/lib/x86_64-linux-gnu/hdf5/serial \
    -lhdf5_cpp -lhdf5 -lm

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo ""
    echo "Running test..."
    echo "================="
    ./test_signal_recorder
    
    if [ $? -eq 0 ]; then
        echo ""
        echo "✓ Test passed!"
        
        # Optional: show HDF5 file structure
        if command -v h5dump &> /dev/null; then
            echo ""
            echo "HDF5 File Structure:"
            echo "===================="
            h5ls -r test_output.h5
        fi
    else
        echo "✗ Test failed!"
        exit 1
    fi
else
    echo "✗ Build failed!"
    exit 1
fi
