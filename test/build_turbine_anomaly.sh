#!/bin/bash

# Build script for test_turbine_anomaly

echo "Building test_turbine_anomaly..."

g++ -std=c++11 -o test_turbine_anomaly test_turbine_anomaly.cpp ../source/SignalProcessing.cpp -I../source -lm

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo "Run with: ./test_turbine_anomaly"
else
    echo "✗ Build failed!"
    exit 1
fi
