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
- **Denoising**: Kalman filter, wavelet denoising, median filter
- **Noise estimation**: Automatic noise level detection using MAD (Median Absolute Deviation)

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
- `test_denoising.cpp`: Kalman filter, wavelet denoising, median filter, and noise estimation

To build a test, run the corresponding script (from the `test/` folder):

```bash
./build_stats.sh
./build_moving_average.sh
./build_normalize.sh
./build_smoothing.sh
./build_event_detection.sh
./build_timestamp.sh
./build_peak_detection.sh
./build_denoising.sh
```

Or on Windows:
```cmd
build_denoising.bat
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
./test_denoising
```

### Run All Tests Automatically:
For comprehensive testing after code modifications:

```bash
cd test
chmod +x run_all_tests.sh
./run_all_tests.sh
```

This will:
- Compile all tests
- Execute each test
- Generate detailed report with success rates
- Create log files for debugging

For quick validation:
```bash
cd test
chmod +x quick_test.sh
./quick_test.sh
```

See `test/TEST_SUITE_README.md` for detailed testing documentation.

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
- **Medical monitoring**: ECG analysis, heart rate detection, noise reduction in biomedical signals
- **Activity recognition**: Step counting, gesture detection
- **Audio processing**: Beat detection, onset detection, audio denoising
- **Sensor data analysis**: Accelerometer, gyroscope signals, removing sensor noise
- **General signal analysis**: Any time-series data processing with noise removal capabilities
- **Industrial monitoring**: Vibration analysis, quality control with noise filtering

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Denoising Capabilities

### Kalman Filter
Simple 1D Kalman filter for smooth signal tracking:
```cpp
SignalProcessing sp;
// Add noisy data...
double denoised[1000];
sp.KalmanFilter(0.01, 0.1, denoised);  // process_noise, measurement_noise
```

### Wavelet Denoising
Haar wavelet transform with soft thresholding:
```cpp
double noise_level = sp.EstimateNoiseLevel();  // Automatic noise estimation
double denoised[1000];
sp.WaveletDenoise(noise_level * 2.0, denoised, 2);  // threshold, output, levels
```

### Median Filter
Effective for impulse noise (salt-and-pepper):
```cpp
double denoised[1000];
sp.MedianFilter(5, denoised);  // window_size (odd number)
```

### Noise Estimation
Automatic noise level detection using MAD:
```cpp
double noise_std = sp.EstimateNoiseLevel();
```

## TODO
Possible real-time signal processing operations to implement:
- Filtering (low-pass, high-pass, band-pass, median, moving average) ✓ (moving average, median filter implemented)
- Peak detection ✓ (implemented with multiple methods: simple, threshold, prominence, distance)
- Denoising (e.g., Kalman filter, wavelet) ✓ (Kalman filter, wavelet, median filter implemented)
- Transforms (FFT, DCT, STFT for spectral analysis)
- Normalization and scaling ✓ (implemented)
- Statistical calculations (mean, variance, standard deviation, skewness, kurtosis) ✓ (mean, variance, std dev implemented)
- Event detection (thresholding, zero-crossing) ✓ (implemented)
- Smoothing (exponential, Savitzky-Golay) ✓ (exponential smoothing implemented)
- Decimation and interpolation
- Autocorrelation and cross-correlation analysis
- Trend or anomaly detection
- Adaptive filters (LMS, RLS) – they adjust their coefficients based on the signal in order to track time-varying changes.
- Notch filters – remove a very narrow frequency component (e.g., 50/60 Hz from the power line).
- Feature extraction – extracting numerical indicators (features) for automatic classification or diagnosis. 
- Downstream ML/AI – using the preprocessed signals as input for neural networks, SVMs, etc.
