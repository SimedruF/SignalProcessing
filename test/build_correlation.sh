#!/bin/bash
echo "Building test_correlation..."
g++ -std=c++11 -o test_correlation test_correlation.cpp ../source/SignalProcessing.cpp -I../source -lm -lrt

if [ $? -eq 0 ]; then
    echo "Build successful! Running test..."
    echo ""
    ./test_correlation
else
    echo "Build failed!"
    exit 1
fi
