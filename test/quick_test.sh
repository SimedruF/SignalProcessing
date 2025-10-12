#!/bin/bash

###############################################################################
# Quick Test Runner - Fast compilation and execution check
###############################################################################

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

cd "$(dirname "$0")"

echo "Quick Test Runner"
echo "================="
echo ""

PASSED=0
FAILED=0

for test in test_stats test_moving_average test_normalize test_smoothing \
            test_event_detection test_timestamp test_peak_detection; do
    
    # Compile
    g++ -o "${test}" "${test}.cpp" ../source/SignalProcessing.cpp -lm -lrt 2>/dev/null
    
    if [ $? -eq 0 ]; then
        # Run
        timeout 5s ./${test} > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓${NC} ${test}"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} ${test} (runtime error)"
            FAILED=$((FAILED + 1))
        fi
    else
        echo -e "${RED}✗${NC} ${test} (compilation error)"
        FAILED=$((FAILED + 1))
    fi
done

echo ""
echo "Results: ${GREEN}${PASSED} passed${NC}, ${RED}${FAILED} failed${NC}"

if [ ${FAILED} -eq 0 ]; then
    echo -e "${GREEN}All tests OK!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
