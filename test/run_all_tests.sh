#!/bin/bash

###############################################################################
# SignalProcessing - Test Suite Runner
# Compiles and runs all test files, generates a comprehensive report
###############################################################################

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
COMPILED_OK=0
COMPILED_FAIL=0
RUN_OK=0
RUN_FAIL=0

# Arrays to store results
declare -a COMPILE_SUCCESS=()
declare -a COMPILE_FAILED=()
declare -a RUN_SUCCESS=()
declare -a RUN_FAILED=()

# Start time
START_TIME=$(date +%s)

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║     SignalProcessing Test Suite Runner                    ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${BLUE}Starting test suite at $(date '+%Y-%m-%d %H:%M:%S')${NC}"
echo ""

# Navigate to test directory
cd "$(dirname "$0")"
SCRIPT_DIR=$(pwd)

# Clean old executables
echo -e "${YELLOW}[CLEANUP]${NC} Removing old executables..."
rm -f test_stats test_moving_average test_normalize test_smoothing \
      test_event_detection test_timestamp test_peak_detection test 2>/dev/null
echo ""

# Define test files (without .cpp extension)
TEST_FILES=(
    "test_stats"
    "test_moving_average"
    "test_normalize"
    "test_smoothing"
    "test_event_detection"
    "test_timestamp"
    "test_peak_detection"
)

echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  PHASE 1: COMPILATION${NC}"
echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Compile each test
for test in "${TEST_FILES[@]}"; do
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}[${TOTAL_TESTS}/${#TEST_FILES[@]}]${NC} Compiling ${test}.cpp..."
    
    # Check if source file exists
    if [ ! -f "${test}.cpp" ]; then
        echo -e "  ${RED}✗ SKIP${NC} - Source file not found"
        COMPILE_FAILED+=("${test} (source not found)")
        COMPILED_FAIL=$((COMPILED_FAIL + 1))
        echo ""
        continue
    fi
    
    # Compile
    g++ -o "${test}" "${test}.cpp" ../source/SignalProcessing.cpp -lm -lrt 2> compile_error_${test}.log
    
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ SUCCESS${NC} - Compiled successfully"
        COMPILE_SUCCESS+=("${test}")
        COMPILED_OK=$((COMPILED_OK + 1))
        rm -f compile_error_${test}.log
    else
        echo -e "  ${RED}✗ FAILED${NC} - Compilation error"
        echo -e "  ${YELLOW}See compile_error_${test}.log for details${NC}"
        COMPILE_FAILED+=("${test}")
        COMPILED_FAIL=$((COMPILED_FAIL + 1))
    fi
    echo ""
done

echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  PHASE 2: EXECUTION${NC}"
echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Run each successfully compiled test
for test in "${COMPILE_SUCCESS[@]}"; do
    echo -e "${BLUE}[RUNNING]${NC} ${test}..."
    echo -e "${YELLOW}─────────────────────────────────────────────────────────${NC}"
    
    # Run test and capture output
    timeout 10s ./${test} > run_output_${test}.log 2>&1
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo -e "${GREEN}✓ PASSED${NC} - Test executed successfully"
        RUN_SUCCESS+=("${test}")
        RUN_OK=$((RUN_OK + 1))
        
        # Show first few lines of output
        echo -e "${CYAN}Output preview:${NC}"
        head -n 5 run_output_${test}.log | sed 's/^/  /'
        echo "  ..."
        
    elif [ $EXIT_CODE -eq 124 ]; then
        echo -e "${RED}✗ TIMEOUT${NC} - Test exceeded 10 seconds"
        RUN_FAILED+=("${test} (timeout)")
        RUN_FAIL=$((RUN_FAIL + 1))
    else
        echo -e "${RED}✗ FAILED${NC} - Test exited with code ${EXIT_CODE}"
        RUN_FAILED+=("${test} (exit code: ${EXIT_CODE})")
        RUN_FAIL=$((RUN_FAIL + 1))
        
        # Show error output
        echo -e "${YELLOW}Error output:${NC}"
        tail -n 10 run_output_${test}.log | sed 's/^/  /'
    fi
    
    echo ""
done

# Calculate execution time
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

# Generate final report
echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                    FINAL REPORT                            ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""

echo -e "${BLUE}Execution time:${NC} ${DURATION} seconds"
echo ""

echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
echo -e "${CYAN}  COMPILATION RESULTS${NC}"
echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
echo -e "Total tests:       ${TOTAL_TESTS}"
echo -e "${GREEN}Compiled OK:       ${COMPILED_OK}${NC}"
echo -e "${RED}Compilation FAILED: ${COMPILED_FAIL}${NC}"
echo ""

if [ ${#COMPILE_SUCCESS[@]} -gt 0 ]; then
    echo -e "${GREEN}✓ Successfully compiled:${NC}"
    for test in "${COMPILE_SUCCESS[@]}"; do
        echo -e "  • ${test}"
    done
    echo ""
fi

if [ ${#COMPILE_FAILED[@]} -gt 0 ]; then
    echo -e "${RED}✗ Failed to compile:${NC}"
    for test in "${COMPILE_FAILED[@]}"; do
        echo -e "  • ${test}"
    done
    echo ""
fi

echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
echo -e "${CYAN}  EXECUTION RESULTS${NC}"
echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
echo -e "Tests executed:    ${COMPILED_OK}"
echo -e "${GREEN}Passed:            ${RUN_OK}${NC}"
echo -e "${RED}Failed:            ${RUN_FAIL}${NC}"
echo ""

if [ ${#RUN_SUCCESS[@]} -gt 0 ]; then
    echo -e "${GREEN}✓ Successfully executed:${NC}"
    for test in "${RUN_SUCCESS[@]}"; do
        echo -e "  • ${test}"
    done
    echo ""
fi

if [ ${#RUN_FAILED[@]} -gt 0 ]; then
    echo -e "${RED}✗ Failed to execute:${NC}"
    for test in "${RUN_FAILED[@]}"; do
        echo -e "  • ${test}"
    done
    echo ""
fi

# Calculate success rate
if [ ${TOTAL_TESTS} -gt 0 ]; then
    COMPILE_RATE=$((COMPILED_OK * 100 / TOTAL_TESTS))
    echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
    echo -e "${CYAN}  SUCCESS RATE${NC}"
    echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
    echo -e "Compilation rate:  ${COMPILE_RATE}%"
    
    if [ ${COMPILED_OK} -gt 0 ]; then
        RUN_RATE=$((RUN_OK * 100 / COMPILED_OK))
        echo -e "Execution rate:    ${RUN_RATE}%"
        OVERALL_RATE=$((RUN_OK * 100 / TOTAL_TESTS))
        echo -e "Overall success:   ${OVERALL_RATE}%"
    fi
    echo ""
fi

# Log files information
echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
echo -e "${CYAN}  LOG FILES${NC}"
echo -e "${CYAN}─────────────────────────────────────────────────────────${NC}"
echo -e "Compilation errors: ${YELLOW}compile_error_*.log${NC}"
echo -e "Execution outputs:  ${YELLOW}run_output_*.log${NC}"
echo ""

# Final status
echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
if [ ${RUN_FAIL} -eq 0 ] && [ ${COMPILED_FAIL} -eq 0 ]; then
    echo -e "${GREEN}  ✓ ALL TESTS PASSED! ✓${NC}"
    EXIT_STATUS=0
elif [ ${RUN_OK} -gt 0 ]; then
    echo -e "${YELLOW}  ⚠ SOME TESTS FAILED ⚠${NC}"
    EXIT_STATUS=1
else
    echo -e "${RED}  ✗ ALL TESTS FAILED ✗${NC}"
    EXIT_STATUS=2
fi
echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Optional: Clean up log files from successful tests
read -p "Clean up log files from successful tests? [y/N] " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    for test in "${RUN_SUCCESS[@]}"; do
        rm -f run_output_${test}.log
    done
    echo -e "${GREEN}✓ Cleaned up log files${NC}"
fi

echo ""
echo -e "${BLUE}Test suite completed at $(date '+%Y-%m-%d %H:%M:%S')${NC}"
echo ""

exit ${EXIT_STATUS}
