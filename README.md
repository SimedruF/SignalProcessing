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
- **Anomaly Detection**: Z-Score, IQR, Moving Average Deviation, sudden change detection
- **Signal Segmentation**: Analyze signal by markers (e.g., turbine blades, sensor zones)
- **Periodic Anomaly Detection**: Detect anomalies in rotating machinery and periodic signals
- **Frequency Analysis (FFT)**: Fast Fourier Transform for spectral analysis
- **Harmonic Analysis**: Detect and analyze harmonic components, calculate THD
- **Frequency Band Analysis**: Power analysis in specific frequency ranges
- **Frequency-based Anomaly Detection**: Compare frequency spectra to detect abnormal patterns
- **ML/AI Feature Extraction**: Extract 21 comprehensive features for machine learning models (neural networks, SVM, etc.)
- **Decimation and Interpolation**: Downsample, upsample, and resample signals for rate conversion
- **Correlation Analysis**: Autocorrelation for periodicity detection, cross-correlation for signal alignment and time delay estimation
- **Signal Recording (HDF5)**: Save signals and metadata to hierarchical HDF5 files for persistent storage and offline analysis

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
- `test_turbine_anomaly.cpp`: turbine vibration monitoring and blade anomaly detection
- `test_frequency_analysis.cpp`: FFT analysis, harmonic detection, frequency-based anomaly detection
- `test_ml_features.cpp`: ML/AI feature extraction for neural networks and SVM
- `test_decimation.cpp`: decimation, interpolation, and resampling
- `test_correlation.cpp`: autocorrelation and cross-correlation analysis
- `test_signal_recorder.cpp`: HDF5 signal recording and metadata storage

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
./build_turbine_anomaly.sh
./build_frequency_analysis.sh
./build_ml_features.sh
./build_decimation.sh
./build_correlation.sh
./build_signal_recorder.sh
```

Or on Windows:
```cmd
build_denoising.bat
build_turbine_anomaly.bat
build_frequency_analysis.bat
build_ml_features.bat
build_decimation.bat
build_correlation.bat
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

### Console Example (ECG Processing)
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

### 🎮 Interactive ImGui Demo (NEW!)
Real-time visual demonstration with interactive GUI:

```bash
cd examples
./setup_imgui.sh      # Downloads ImGui and ImPlot
cd build
cmake ..
make
./imgui_demo
```

**Features:**
- 🎛️ Live signal generation (ECG, Turbine, Sine, Noise)
- 🔧 Real-time filtering (Kalman, Median, Wavelet)
- ⚠️ Interactive anomaly detection
- 📈 FFT frequency analysis
- 🤖 ML feature extraction (21 features)
- 📊 Interactive plots with zoom/pan

See `examples/README_IMGUI.md` for detailed instructions and screenshots.

## Documentation
- **examples/README_IMGUI.md** - Interactive ImGui demo guide with screenshots
- **ML_FEATURES_GUIDE.md** - Complete guide for ML/AI feature extraction and integration
- **ANOMALY_DETECTION_GUIDE.md** - Comprehensive anomaly detection methods guide
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
- **Rotating machinery**: Turbine blade monitoring, motor bearing analysis, fan imbalance detection
- **Predictive maintenance**: Anomaly detection in manufacturing equipment, early fault detection

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

## Anomaly Detection for Industrial Applications

### Turbine Blade Monitoring Example
Perfect for detecting vibration anomalies in rotating machinery:

```cpp
SignalProcessing sp;
const int NUM_BLADES = 12;
const int SAMPLES_PER_BLADE = 50;
int blade_markers[NUM_BLADES];

// Collect vibration data for each blade
for (int blade = 0; blade < NUM_BLADES; ++blade) {
    blade_markers[blade] = blade * SAMPLES_PER_BLADE;
    for (int sample = 0; sample < SAMPLES_PER_BLADE; ++sample) {
        double vibration = ReadVibrationSensor();
        sp.AddValue(vibration);
    }
}

// Analyze each blade segment
SegmentStats blade_stats[NUM_BLADES];
sp.SegmentByMarkers(blade_markers, NUM_BLADES, blade_stats);

// Find the blade with highest vibration anomaly
int problematic_blade = sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 2);

// Check if maintenance is needed
if (blade_stats[problematic_blade].rms > threshold) {
    printf("Warning: Blade #%d requires inspection!\n", problematic_blade);
    printf("RMS: %.4f, Max: %.4f\n", 
           blade_stats[problematic_blade].rms,
           blade_stats[problematic_blade].max_value);
}
```

### Multiple Anomaly Detection Methods

**1. Z-Score Method** - Statistical deviation from mean:
```cpp
int anomaly_indices[1000];
int count = sp.DetectAnomaliesZScore(3.0, anomaly_indices, 1000);  // 3 sigma threshold
```

**2. IQR Method** - Interquartile range for robust outlier detection:
```cpp
int anomaly_indices[1000];
int count = sp.DetectAnomaliesIQR(1.5, anomaly_indices, 1000);  // 1.5 = outliers, 3.0 = extreme
```

**3. Moving Average Deviation** - Detects deviations from local trends:
```cpp
int anomaly_indices[1000];
int count = sp.DetectAnomaliesMAD(20, 3.0, anomaly_indices, 1000);  // window=20, threshold=3.0
```

**4. Sudden Change Detection** - Catches spikes and rapid variations:
```cpp
int anomaly_indices[1000];
int count = sp.DetectSuddenChanges(2.0, anomaly_indices, 1000);  // threshold change = 2.0
```

**5. Periodic Anomaly Detection** - For rotating machinery:
```cpp
int anomaly_indices[1000];
int period = 360;  // samples per rotation
int count = sp.DetectPeriodicAnomalies(period, 3.0, anomaly_indices, 1000);
```

### Segment Statistics
Access detailed statistics for each segment (blade, zone, etc.):

```cpp
for (int i = 0; i < num_segments; ++i) {
    printf("Segment %d:\n", blade_stats[i].segment_id);
    printf("  Mean: %.4f\n", blade_stats[i].mean);
    printf("  Std Dev: %.4f\n", blade_stats[i].std_dev);
    printf("  RMS: %.4f\n", blade_stats[i].rms);
    printf("  Max: %.4f\n", blade_stats[i].max_value);
    printf("  Peak-to-Peak: %.4f\n", blade_stats[i].peak_to_peak);
    printf("  Anomaly Score: %.4f\n", blade_stats[i].anomaly_score);
}
```

## Frequency Analysis Capabilities

### FFT Analysis
Perform Fast Fourier Transform on signal windows:

```cpp
SignalProcessing sp;
// ... add your vibration data ...

double sampling_rate = 1000.0;  // Hz
FrequencySpectrum spectrum;

// Analyze entire signal
if (sp.FFTAnalysis(sampling_rate, &spectrum)) {
    printf("Dominant frequency: %.2f Hz\n", spectrum.dominant_frequency);
    printf("Total power: %.2f\n", spectrum.total_power);
    printf("Frequency resolution: %.3f Hz\n", spectrum.frequency_resolution);
    
    // Free memory when done
    sp.FreeSpectrum(&spectrum);
}

// Or analyze a specific window
if (sp.FFTAnalysis(start_index, window_size, sampling_rate, &spectrum)) {
    // Process spectrum...
    sp.FreeSpectrum(&spectrum);
}
```

### Frequency Peak Detection
Find dominant frequencies in the spectrum:

```cpp
double peak_frequencies[10];
double peak_magnitudes[10];

int num_peaks = sp.FindFrequencyPeaks(&spectrum, min_magnitude, 
                                      peak_frequencies, peak_magnitudes, 10);

printf("Top frequency components:\n");
for (int i = 0; i < num_peaks; ++i) {
    printf("  %.1f Hz: magnitude %.2f\n", peak_frequencies[i], peak_magnitudes[i]);
}
```

### Harmonic Analysis
Analyze harmonic content and calculate Total Harmonic Distortion (THD):

```cpp
double fundamental = 60.0;  // Hz
double harmonic_magnitudes[10];
int num_harmonics = 5;

double thd = sp.AnalyzeHarmonics(&spectrum, fundamental, 
                                 num_harmonics, harmonic_magnitudes);

printf("Fundamental: %.2f\n", harmonic_magnitudes[0]);
printf("2nd Harmonic: %.2f\n", harmonic_magnitudes[1]);
printf("THD: %.2f%%\n", thd * 100.0);
```

### Frequency Band Power Analysis
Measure power in specific frequency ranges:

```cpp
// Analyze power distribution across bands
double low_freq_power = sp.GetPowerInBand(&spectrum, 10.0, 50.0);
double mid_freq_power = sp.GetPowerInBand(&spectrum, 100.0, 200.0);
double high_freq_power = sp.GetPowerInBand(&spectrum, 500.0, 1000.0);

printf("Low frequency (10-50 Hz): %.2f%%\n", 
       (low_freq_power / spectrum.total_power) * 100.0);
```

### Turbine Blade Frequency Analysis
Compare frequency spectra across blade segments:

```cpp
const int NUM_BLADES = 12;
int blade_markers[NUM_BLADES];
FrequencySpectrum blade_spectra[NUM_BLADES];

// Analyze frequency content for each blade
int num_analyzed = sp.CompareSegmentSpectra(blade_markers, NUM_BLADES, 
                                            sampling_rate, blade_spectra);

// Check for high-frequency anomalies
for (int i = 0; i < num_analyzed; ++i) {
    double high_freq_power = sp.GetPowerInBand(&blade_spectra[i], 500.0, 1000.0);
    
    if (high_freq_power > threshold) {
        printf("⚠ Blade #%d has abnormal high-frequency content\n", i);
        printf("  Dominant freq: %.1f Hz\n", blade_spectra[i].dominant_frequency);
        printf("  High-freq power: %.2f\n", high_freq_power);
    }
    
    sp.FreeSpectrum(&blade_spectra[i]);
}
```

### Frequency-Based Anomaly Detection
Compare current spectrum with baseline to detect changes:

```cpp
FrequencySpectrum baseline_spectrum, current_spectrum;

// Analyze baseline (normal operation)
sp_baseline.FFTAnalysis(sampling_rate, &baseline_spectrum);

// Analyze current signal
sp_current.FFTAnalysis(sampling_rate, &current_spectrum);

// Detect anomalies
double anomaly_score = sp_current.DetectFrequencyAnomalies(
    &current_spectrum, &baseline_spectrum, 1.5  // threshold ratio
);

if (anomaly_score > 0.3) {
    printf("⚠ Frequency anomaly detected! Score: %.4f\n", anomaly_score);
    printf("  New frequency components may indicate:\n");
    printf("  - Bearing wear\n");
    printf("  - Blade damage\n");
    printf("  - Imbalance\n");
    printf("  - Resonance issues\n");
}

sp_baseline.FreeSpectrum(&baseline_spectrum);
sp_current.FreeSpectrum(&current_spectrum);
```

### Use Cases for Frequency Analysis

**Turbine Monitoring**:
- Detect blade pass frequency anomalies
- Identify resonance issues
- Monitor bearing condition
- Track imbalance development

**Bearing Diagnostics**:
- BPFI/BPFO frequency analysis
- Detect wear patterns
- Monitor lubrication issues

**Motor Analysis**:
- Rotor bar defects
- Air gap eccentricity
- Electrical faults

**Vibration Analysis**:
- Identify sources of vibration
- Separate mechanical from electrical issues
- Track degradation trends

## Signal Recording with HDF5

The `SignalRecorder` class provides persistent storage for signal data and metadata using the HDF5 file format. Perfect for:
- **Data logging**: Store sensor readings from multiple channels
- **Offline analysis**: Save signals for later processing and visualization
- **Dataset creation**: Generate training data for machine learning models
- **Audit trails**: Maintain records of measurements with metadata

### Features
- Hierarchical data organization (groups and datasets)
- Metadata storage (device info, location, timestamps)
- Multiple data channels support
- Units and attributes for each dataset
- Efficient storage for large time-series data

### Requirements
Install HDF5 development libraries:

```bash
# Ubuntu/Debian
sudo apt install libhdf5-dev

# Verify installation
find /usr -name "H5Cpp.h"  # Should find /usr/include/hdf5/serial/H5Cpp.h
```

### Basic Usage

```cpp
#include "SignalRecorder.hpp"

int main() {
    // Create recorder and HDF5 file
    SignalRecorder recorder("sensor_data.h5");
    
    // Add metadata
    recorder.addMetadata("Info", "device", "Accelerometer XYZ-100");
    recorder.addMetadata("Info", "location", "Turbine Bearing #3");
    
    // Record vibration data
    std::vector<float> vibration_x = ReadSensorChannel(AXIS_X);
    recorder.addFloatVector("Sensors/Vibration", "axis_x", vibration_x, "m/s²");
    
    // Record temperature
    std::vector<float> temperature = ReadTemperatureSensor();
    recorder.addFloatVector("Sensors/Temperature", "bearing_temp", temperature, "°C");
    
    // Save processed data
    std::vector<float> filtered_signal = ApplyFilter(vibration_x);
    recorder.addFloatVector("Processed/Filtered", "kalman_filtered", filtered_signal, "m/s²");
    
    return 0;  // File automatically closed
}
```

### Hierarchical Organization

Organize data in logical groups:

```
/sensor_data.h5
├── Info/
│   ├── device: "Accelerometer XYZ-100"
│   └── location: "Turbine Bearing #3"
├── Sensors/
│   ├── Vibration/
│   │   ├── axis_x (1000 samples, units: m/s²)
│   │   ├── axis_y (1000 samples, units: m/s²)
│   │   └── axis_z (1000 samples, units: m/s²)
│   └── Temperature/
│       └── bearing_temp (500 samples, units: °C)
└── Processed/
    ├── Filtered/
    │   └── kalman_filtered (1000 samples, units: m/s²)
    └── Features/
        └── rms_values (12 samples, units: m/s²)
```

### Multi-Channel Recording

```cpp
SignalRecorder recorder("multi_channel.h5");

// Record from multiple sensors
const int NUM_CHANNELS = 3;
const char* axes[] = {"axis_x", "axis_y", "axis_z"};

for (int ch = 0; ch < NUM_CHANNELS; ++ch) {
    std::vector<float> data = ReadAccelerometer(ch);
    recorder.addFloatVector("RawData/Accelerometer", axes[ch], data, "m/s²");
}

// Save analysis results
std::vector<float> fft_peaks = ComputeFFTPeaks(data);
recorder.addFloatVector("Analysis/FFT", "dominant_frequencies", fft_peaks, "Hz");
```

### Machine Learning Dataset Creation

```cpp
SignalRecorder dataset("training_data.h5");

// Store features for ML model
dataset.addMetadata("Dataset", "class", "normal_operation");
dataset.addMetadata("Dataset", "timestamp", GetCurrentTimestamp());

// Extract and save features
SignalProcessing sp;
// ... add signal data to sp ...

MLFeatureSet features = sp.ExtractMLFeatures();
std::vector<float> feature_vector(21);
feature_vector[0] = features.mean;
feature_vector[1] = features.std_dev;
feature_vector[2] = features.rms;
// ... fill remaining features ...

dataset.addFloatVector("Features", "statistical", feature_vector, "mixed");

// Save labels
std::vector<float> labels = {0.0};  // 0 = normal, 1 = fault
dataset.addFloatVector("Labels", "class_label", labels, "categorical");
```

### Turbine Monitoring Example

```cpp
SignalRecorder log("turbine_monitoring.h5");

// Session metadata
log.addMetadata("Session", "turbine_id", "TRB-001");
log.addMetadata("Session", "date", "2026-02-08");
log.addMetadata("Session", "operator", "John Doe");

const int NUM_BLADES = 12;
const int SAMPLES_PER_BLADE = 50;

// Record each blade's vibration signature
for (int blade = 0; blade < NUM_BLADES; ++blade) {
    std::vector<float> blade_signal = RecordBladePass(blade);
    
    char path[64], name[32];
    sprintf(path, "Blades/Blade_%02d", blade + 1);
    sprintf(name, "vibration");
    
    log.addFloatVector(path, name, blade_signal, "m/s²");
    
    // Calculate and save blade statistics
    double rms = CalculateRMS(blade_signal);
    std::vector<float> stats = {(float)rms};
    log.addFloatVector(path, "rms", stats, "m/s²");
}
```

### Compilation

Include HDF5 headers and libraries when compiling:

```bash
g++ -std=c++11 \
    -I/usr/include/hdf5/serial \
    -o your_program your_program.cpp \
    -L/usr/lib/x86_64-linux-gnu/hdf5/serial \
    -lhdf5_cpp -lhdf5
```

### Viewing HDF5 Files

Inspect recorded data with HDF5 tools:

```bash
# Install tools
sudo apt install hdf5-tools

# List file contents
h5ls -r sensor_data.h5

# Dump complete file structure
h5dump sensor_data.h5

# View specific dataset
h5dump -d /Sensors/Vibration/axis_x sensor_data.h5
```

### Python Integration

Read HDF5 files with Python for analysis and visualization:

```python
import h5py
import numpy as np
import matplotlib.pyplot as plt

# Open file
with h5py.File('sensor_data.h5', 'r') as f:
    # Read metadata
    device = f['Info/device'][()]
    print(f"Device: {device}")
    
    # Read signal data
    vibration = f['Sensors/Vibration/axis_x'][:]
    units = f['Sensors/Vibration/axis_x'].attrs['units']
    
    # Plot
    plt.plot(vibration)
    plt.ylabel(f'Vibration ({units})')
    plt.xlabel('Sample')
    plt.title(f'Sensor: {device}')
    plt.show()
```

### API Reference

**Constructor**
```cpp
SignalRecorder(const std::string& filename)
```
Creates new HDF5 file (overwrites if exists).

**Methods**
```cpp
void addMetadata(const std::string& groupName, 
                 const std::string& key, 
                 const std::string& value)
```
Add string metadata to a group.

```cpp
void addFloatVector(const std::string& path,
                    const std::string& datasetName,
                    const std::vector<float>& data,
                    const std::string& units = "")
```
Save float vector with optional units attribute. Creates nested groups automatically.

**Note**: File is automatically closed when `SignalRecorder` object is destroyed.

### Use Cases

- **Long-term monitoring**: Record sensor data 24/7 for trend analysis
- **Batch processing**: Save raw data and process later with different algorithms
- **Benchmarking**: Compare performance of different filters and algorithms
- **Calibration**: Store reference signals and baselines
- **Documentation**: Maintain records with timestamps and metadata
- **ML model training**: Generate labeled datasets from sensor recordings

## TODO
Possible real-time signal processing operations to implement:
- Filtering (low-pass, high-pass, band-pass, median, moving average) ✓ (moving average, median filter implemented)
- Peak detection ✓ (implemented with multiple methods: simple, threshold, prominence, distance)
- Denoising (e.g., Kalman filter, wavelet) ✓ (Kalman filter, wavelet, median filter implemented)
- Transforms (FFT, DCT, STFT for spectral analysis) ✓ (FFT with Cooley-Tukey algorithm, windowing, harmonic analysis implemented)
- Normalization and scaling ✓ (implemented)
- Statistical calculations (mean, variance, standard deviation, skewness, kurtosis) ✓ (mean, variance, std dev implemented)
- Event detection (thresholding, zero-crossing) ✓ (implemented)
- Smoothing (exponential, Savitzky-Golay) ✓ (exponential smoothing implemented)
- Decimation and interpolation ✓ (implemented: decimate, linear interpolation, resample)
- Autocorrelation and cross-correlation analysis ✓ (implemented: autocorrelation, cross-correlation, peak finding)
- Trend or anomaly detection ✓ (implemented: Z-Score, IQR, MAD, periodic, sudden change detection, frequency-based)
- Adaptive filters (LMS, RLS) – they adjust their coefficients based on the signal in order to track time-varying changes.
- Notch filters – remove a very narrow frequency component (e.g., 50/60 Hz from the power line).
- Feature extraction – extracting numerical indicators (features) for automatic classification or diagnosis. ✓ (segment-based statistics, frequency spectrum features implemented)
- Downstream ML/AI – using the preprocessed signals as input for neural networks, SVMs, etc.
