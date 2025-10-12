#!/bin/bash
g++ -o test_peak_detection test_peak_detection.cpp ../source/SignalProcessing.cpp -lm -lrt
echo "Build complete: test_peak_detection"
