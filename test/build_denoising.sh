#!/bin/bash

# Build script for test_denoising

echo "Building test_denoising..."

g++ -std=c++11 -o test_denoising test_denoising.cpp ../source/SignalProcessing.cpp -I../source -lm

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo "Run with: ./test_denoising"
else
    echo "✗ Build failed!"
    exit 1
fi
