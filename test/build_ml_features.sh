#!/bin/bash
# Build script for test_ml_features

echo "Building test_ml_features..."
g++ -o test_ml_features test_ml_features.cpp ../source/SignalProcessing.cpp -lm -lrt

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run with: ./test_ml_features"
else
    echo "Build failed!"
    exit 1
fi
