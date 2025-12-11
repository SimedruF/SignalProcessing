# Copilot Instructions for SignalProcessing

## Project Overview
C++ header-only style library for real-time signal processing. Single class `SignalProcessing` in `source/SignalProcessing.h` with implementation in `source/SignalProcessing.cpp`. Fixed-size buffer of 1000 samples (`NB_MAX_VALUES`).

## Architecture
- **Core Class**: `SignalProcessing` - manages internal signal buffer, timestamps, and all processing operations
- **Key Structs**: `SegmentStats` (segment analysis), `FrequencySpectrum`/`FrequencyBin` (FFT results), `prob_dist` (distributions)
- **Data Flow**: `AddValue()`/`AddValueWithTimestamp()` → internal buffer → processing methods → output arrays
- **Buffer Wrapping**: Index resets to 0 when reaching `NB_MAX_VALUES` (circular behavior)

## Build Commands

### Windows (PowerShell)
```powershell
# From test/ directory - uses g++ with WINDOWS define
.\build_denoising.bat
.\build_turbine_anomaly.bat
.\build_frequency_analysis.bat
```

### Linux/Bash
```bash
# From test/ directory
./build_stats.sh        # Individual test
./run_all_tests.sh      # Full suite with report
./quick_test.sh         # Fast validation
```

### Compilation Pattern
```bash
g++ -std=c++11 -DWINDOWS -o test_name.exe test_name.cpp ../source/SignalProcessing.cpp -I../source
```
Linux adds `-lm -lrt` for math and real-time libraries.

## Code Conventions

### Adding New Functionality
1. Declare method in `source/SignalProcessing.h` with doxygen-style comments (`@brief`, `@param`, `@return`)
2. Implement in `source/SignalProcessing.cpp`
3. Create `test/test_<feature>.cpp` following existing pattern (simple `main()` with `printf` assertions)
4. Create `test/build_<feature>.sh` and optionally `.bat`

### Test File Pattern
```cpp
#include "../source/SignalProcessing.h"
#include <stdio.h>

int main() {
    SignalProcessing sp;
    // Add test data
    for (int i = 0; i < 10; ++i) sp.AddValue(i + 1);
    // Test and print results
    printf("Mean: %f\n", sp.GetMean());
    return 0;
}
```

### Output Array Convention
Processing methods take pre-allocated output arrays as parameters:
```cpp
double denoised[1000];
sp.KalmanFilter(0.01, 0.1, denoised);

int anomalies[100];
int count = sp.DetectAnomaliesZScore(3.0, anomalies, 100);
```

## Key Patterns

### Platform-Specific Code
Use `#ifdef WINDOWS` / `#elif defined(__linux__)` for platform differences (see timestamp handling in `AddValue()`).

### Frequency Analysis
`FrequencySpectrum` requires manual memory cleanup with `FreeSpectrum()`.

### Anomaly Detection Methods
| Method | Function | Best For |
|--------|----------|----------|
| Z-Score | `DetectAnomaliesZScore()` | Gaussian data, threshold=3.0 |
| IQR | `DetectAnomaliesIQR()` | Robust to outliers, multiplier=1.5 |
| MAD | `DetectAnomaliesMAD()` | Time-varying signals |
| Sudden Change | `DetectSuddenChanges()` | Spikes, impacts |

### Signal Segmentation
For rotating machinery (turbines, motors), use `SegmentByMarkers()` with blade/marker positions and `FindMostAnomalousSegment()` for fault detection.

## Documentation
- `ANOMALY_DETECTION_GUIDE.md` - Comprehensive anomaly detection usage
- `examples/example_complete.cpp` - Full ECG processing workflow demonstrating all features
