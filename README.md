# SignalProcessing

A comprehensive C++ library for real-time signal processing, featuring filtering, statistical analysis, event detection, and peak detection capabilities.

## Main Features
- Add and manipulate values in the signal vector
- Add values with associated timestamps for real-time tracking
- Calculate normal distribution and probabilities
- Retrieve and manage timestamps
- Index lookup table for normalization
- Moving average (windowed smoothing)
- Statistical calculations: mean, variance, standard deviation
- Normalization and scaling (to [0,1] or custom range)
- Exponential smoothing
- Event detection: threshold crossing and zero-crossing with flag status
- Peak detection: simple, threshold-based, prominence-based, and distance-based

## Function Descriptions
See comments in `SignalProcessing.h` for details about each public method.

## How to Build and Test
Each functionality has a dedicated test file in the `test/` folder:

- `test_stats.cpp`: mean, variance, standard deviation
- `test_moving_average.cpp`: moving average
- `test_normalize.cpp`: normalization and scaling
- `test_smoothing.cpp`: exponential smoothing
- `test_event_detection.cpp`: threshold crossing and zero-crossing detection
- `test_timestamp.cpp`: timestamp storage and retrieval
- `test_peak_detection.cpp`: peak detection methods

To build a test, run the corresponding script (from the `test/` folder):

```bash
./build_stats.sh
./build_moving_average.sh
./build_normalize.sh
./build_smoothing.sh
./build_event_detection.sh
./build_timestamp.sh
./build_peak_detection.sh
```

To run a test:
```bash
./test_stats
./test_moving_average
./test_normalize
./test_smoothing
./test_event_detection
./test_timestamp
./test_peak_detection
```

## Examples
A complete working example demonstrating all features is available in the `examples/` folder:

```bash
cd examples
chmod +x build_example.sh
./build_example.sh
./example_complete
```

This example simulates ECG signal processing including:
- Data acquisition with timestamps
- Signal preprocessing (smoothing, scaling)
- Statistical analysis
- R-peak detection and heart rate calculation
- Event detection
- Timestamp validation

## Documentation
- **API_REFERENCE.md** - Complete API documentation with all functions
- **PEAK_DETECTION_GUIDE.md** - Detailed guide for peak detection methods
- **test/README_PEAK_DETECTION.md** - Peak detection test documentation

## Applications
This library is suitable for:
- **Medical monitoring**: ECG analysis, heart rate detection
- **Activity recognition**: Step counting, gesture detection
- **Audio processing**: Beat detection, onset detection
- **Sensor data analysis**: Accelerometer, gyroscope signals
- **General signal analysis**: Any time-series data processing

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## TODO
Possible real-time signal processing operations to implement:
- Filtering (low-pass, high-pass, band-pass, median, moving average) ✓ (moving average implemented)
- Peak detection ✓ (implemented with multiple methods: simple, threshold, prominence, distance)
- Denoising (e.g., Kalman filter, wavelet)
- Transforms (FFT, DCT, STFT for spectral analysis)
- Normalization and scaling ✓ (implemented)
- Statistical calculations (mean, variance, standard deviation, skewness, kurtosis) ✓ (mean, variance, std dev implemented)
- Event detection (thresholding, zero-crossing) ✓ (implemented)
- Smoothing (exponential, Savitzky-Golay) ✓ (exponential smoothing implemented)
- Decimation and interpolation
- Autocorrelation and cross-correlation analysis
- Trend or anomaly detection
