#!/bin/bash
g++ -o example_complete example_complete.cpp ../source/SignalProcessing.cpp -lm -lrt
echo "Build complete: example_complete"
