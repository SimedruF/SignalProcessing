# SignalProcessing

# SignalProcessing

The `SignalProcessing` class provides functions for signal processing, including vector operations, normal distribution calculation, timestamp handling, and index lookup tables.

## Main Features
- Add and manipulate values in the signal vector
- Calculate normal distribution and probabilities
- Retrieve and manage timestamps
- Index lookup table for normalization

## Function Descriptions
See comments in `SignalProcessing.h` for details about each public method.

## How to Build and Test
Tests are located in `test/test.cpp`. To run the tests:

1. Build the project:
   ```bash
   g++ -o test test/test.cpp source/SignalProcessing.cpp -lrt
   ```
   (the `-lrt` option is required for POSIX time functions)

2. Run the executable:
   ```bash
   ./test
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
