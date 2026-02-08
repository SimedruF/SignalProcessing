#!/bin/bash
echo "Building test_decimation..."
g++ -std=c++11 -o test_decimation test_decimation.cpp ../source/SignalProcessing.cpp -I../source -lm -lrt

if [ $? -eq 0 ]; then
    echo "Build successful! Running test..."
    echo ""
    ./test_decimation
else
    echo "Build failed!"
    exit 1
fi
