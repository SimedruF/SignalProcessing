# Test Suite Documentation

## Overview
The SignalProcessing library includes comprehensive test scripts to ensure all functionality works correctly after modifications.

## Test Scripts

### 1. `run_all_tests.sh` - Complete Test Suite
**Full regression testing with detailed reporting**

#### Features:
- ✅ Compiles all test files
- ✅ Executes each test with timeout protection (10s)
- ✅ Generates detailed compilation and execution reports
- ✅ Shows success/failure statistics
- ✅ Creates log files for debugging
- ✅ Color-coded output for easy reading
- ✅ Calculates success rates

#### Usage:
```bash
cd test/
chmod +x run_all_tests.sh
./run_all_tests.sh
```

#### Output Example:
```
╔════════════════════════════════════════════════════════════╗
║     SignalProcessing Test Suite Runner                    ║
╚════════════════════════════════════════════════════════════╝

Starting test suite at 2025-10-12 11:15:33

═══════════════════════════════════════════════════════════
  PHASE 1: COMPILATION
═══════════════════════════════════════════════════════════

[1/7] Compiling test_stats.cpp...
  ✓ SUCCESS - Compiled successfully

[2/7] Compiling test_moving_average.cpp...
  ✓ SUCCESS - Compiled successfully
...

═══════════════════════════════════════════════════════════
  PHASE 2: EXECUTION
═══════════════════════════════════════════════════════════

[RUNNING] test_stats...
✓ PASSED - Test executed successfully
...

╔════════════════════════════════════════════════════════════╗
║                    FINAL REPORT                            ║
╚════════════════════════════════════════════════════════════╝

Compilation rate:  100%
Execution rate:    100%
Overall success:   100%

═══════════════════════════════════════════════════════════
  ✓ ALL TESTS PASSED! ✓
═══════════════════════════════════════════════════════════
```

#### Log Files Generated:
- `compile_error_<test>.log` - Compilation errors (if any)
- `run_output_<test>.log` - Test execution output

#### Exit Codes:
- `0` - All tests passed
- `1` - Some tests failed
- `2` - All tests failed

---

### 2. `quick_test.sh` - Fast Validation
**Quick smoke test for rapid iteration**

#### Features:
- ⚡ Fast execution (no detailed output)
- ✅ Compiles and runs all tests
- ✅ Simple pass/fail indication
- ✅ No log files created
- ✅ Perfect for git pre-commit hooks

#### Usage:
```bash
cd test/
chmod +x quick_test.sh
./quick_test.sh
```

#### Output Example:
```
Quick Test Runner
=================

✓ test_stats
✓ test_moving_average
✓ test_normalize
✓ test_smoothing
✓ test_event_detection
✓ test_timestamp
✓ test_peak_detection

Results: 7 passed, 0 failed
All tests OK!
```

#### When to Use:
- Quick validation after small changes
- Pre-commit verification
- Continuous integration pipelines
- Development workflow

---

## Individual Test Files

### test_stats.cpp
Tests statistical functions:
- `GetMean()`
- `GetVariance()`
- `GetStandardDeviation()`

### test_moving_average.cpp
Tests filtering:
- `GetMovingAverage()`
- `GetMovingAverageVector()`

### test_normalize.cpp
Tests preprocessing:
- `NormalizeVector()`
- `ScaleVector()`

### test_smoothing.cpp
Tests smoothing:
- `ExponentialSmoothing()`

### test_event_detection.cpp
Tests event detection:
- `DetectThresholdCrossing()`
- `DetectZeroCrossing()`
- Event flags management

### test_timestamp.cpp
Tests timestamp functionality:
- `AddValueWithTimestamp()`
- Timestamp storage and retrieval

### test_peak_detection.cpp
Tests peak detection (5 comprehensive scenarios):
- `DetectPeaks()` - Simple local maxima
- `DetectPeaksWithThreshold()` - Amplitude filtering
- `DetectPeaksWithProminence()` - Relative height
- `DetectPeaksWithDistance()` - Minimum separation
- Real-world ECG simulation

---

## Building Individual Tests

Each test has its own build script:
```bash
./build_stats.sh
./build_moving_average.sh
./build_normalize.sh
./build_smoothing.sh
./build_event_detection.sh
./build_timestamp.sh
./build_peak_detection.sh
```

Or compile manually:
```bash
g++ -o test_stats test_stats.cpp ../source/SignalProcessing.cpp -lm -lrt
```

---

## Continuous Integration

### Using in CI/CD Pipeline:
```yaml
# .github/workflows/test.yml
name: Test Suite

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Run tests
        run: |
          cd test
          chmod +x run_all_tests.sh
          ./run_all_tests.sh
```

### Git Pre-commit Hook:
```bash
# .git/hooks/pre-commit
#!/bin/bash
cd test
./quick_test.sh
if [ $? -ne 0 ]; then
    echo "Tests failed! Commit aborted."
    exit 1
fi
```

---

## Interpreting Results

### All Tests Pass ✓
```
Compilation rate:  100%
Execution rate:    100%
Overall success:   100%
```
✅ Safe to commit/deploy

### Compilation Failures ✗
```
✗ Failed to compile:
  • test_peak_detection
```
🔧 Check `compile_error_test_peak_detection.log`
- Syntax errors in test or source
- Missing functions in header
- Type mismatches

### Runtime Failures ✗
```
✗ Failed to execute:
  • test_stats (exit code: 139)
```
🐛 Check `run_output_test_stats.log`
- Segmentation fault (null pointer, array overflow)
- Assertion failures
- Logic errors

### Timeouts ⏱
```
✗ Failed to execute:
  • test_peak_detection (timeout)
```
🔄 Possible infinite loop or very slow algorithm

---

## Best Practices

### After Modifying Source Code:
1. Run `./quick_test.sh` for fast feedback
2. If passed, run `./run_all_tests.sh` for full validation
3. Review any log files if failures occur
4. Fix issues and re-run

### Before Committing:
```bash
# Full validation
cd test
./run_all_tests.sh

# If all pass:
cd ..
git add .
git commit -m "Your commit message"
```

### Adding New Tests:
1. Create `test_newfeature.cpp`
2. Create `build_newfeature.sh`
3. Add to `TEST_FILES` array in `run_all_tests.sh`:
   ```bash
   TEST_FILES=(
       ...
       "test_newfeature"
   )
   ```
4. Run `./run_all_tests.sh` to verify

---

## Troubleshooting

### "Permission denied" Error
```bash
chmod +x run_all_tests.sh quick_test.sh
```

### Tests Compile but Fail to Run
- Check if `SignalProcessing.cpp` was modified incorrectly
- Verify function signatures match declarations
- Look for null pointer dereferences

### All Tests Suddenly Fail
- Recent changes likely broke core functionality
- Review latest git commits: `git log -p`
- Revert suspicious changes: `git revert <commit>`

### Memory Leaks
```bash
# Run with valgrind
valgrind --leak-check=full ./test_stats
```

---

## Performance Benchmarking

### Timing Individual Tests:
```bash
time ./test_peak_detection
```

### Full Suite Timing:
The `run_all_tests.sh` script automatically reports execution time:
```
Execution time: 2 seconds
```

---

## Test Coverage

| Feature | Test File | Status |
|---------|-----------|--------|
| Mean, Variance, Std Dev | test_stats.cpp | ✅ |
| Moving Average | test_moving_average.cpp | ✅ |
| Normalization | test_normalize.cpp | ✅ |
| Exponential Smoothing | test_smoothing.cpp | ✅ |
| Event Detection | test_event_detection.cpp | ✅ |
| Timestamps | test_timestamp.cpp | ✅ |
| Peak Detection | test_peak_detection.cpp | ✅ |

**Coverage: 100%** of implemented features

---

## Future Enhancements

- [ ] Unit test framework integration (Google Test)
- [ ] Code coverage reporting (gcov/lcov)
- [ ] Performance regression tests
- [ ] Automated benchmarking
- [ ] Test result database/history
- [ ] HTML report generation

---

## Summary

✅ **run_all_tests.sh** - Comprehensive testing with detailed reports  
⚡ **quick_test.sh** - Fast smoke testing  
📝 **Individual tests** - Granular feature validation  
🔄 **CI/CD ready** - Easy integration with pipelines  

**Use these scripts after every code change to ensure stability!**
