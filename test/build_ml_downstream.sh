#!/bin/bash
# Build script for ML Downstream integration test (Linux)

echo "Building test_ml_downstream..."

g++ -std=c++11 -o test_ml_downstream test_ml_downstream.cpp ../source/SignalProcessing.cpp -I../source -lm -lrt

if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./test_ml_downstream"
    chmod +x test_ml_downstream
else
    echo "Build failed!"
    exit 1
fi
