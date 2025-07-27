# SignalProcessing

# SignalProcessing

The `SignalProcessing` class provides functions for signal processing, including vector operations, normal distribution calculation, timestamp handling, and index lookup tables.

## Main Features
- Add and manipulate values in the signal vector
- Calculate normal distribution and probabilities
- Retrieve and manage timestamps
- Index lookup table for normalization
- Moving average (windowed smoothing)
- Statistical calculations: mean, variance, standard deviation
- Normalization and scaling (to [0,1] or custom range)
- Exponential smoothing

## Function Descriptions
See comments in `SignalProcessing.h` for details about each public method.

## How to Build and Test
Each functionality has a dedicated test file in the `test/` folder:

- `test_stats.cpp`: mean, variance, standard deviation
- `test_moving_average.cpp`: moving average
- `test_normalize.cpp`: normalization and scaling
- `test_smoothing.cpp`: exponential smoothing

To build a test, run the corresponding script (from the `test/` folder):

```bash
./build_stats.sh
./build_moving_average.sh
./build_normalize.sh
./build_smoothing.sh
```

To run a test:
```bash
./test_stats
./test_moving_average
./test_normalize
./test_smoothing
```

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## TODO
Possible real-time signal processing operations to implement:
- Filtering (low-pass, high-pass, band-pass, median, moving average)
- Peak detection
- Denoising (e.g., Kalman filter, wavelet)
- Transforms (FFT, DCT, STFT for spectral analysis)
- Normalization and scaling
- Statistical calculations (mean, variance, standard deviation, skewness, kurtosis)
- Event detection (thresholding, zero-crossing)
- Smoothing (exponential, Savitzky-Golay)
- Decimation and interpolation
- Autocorrelation and cross-correlation analysis
- Trend or anomaly detection
