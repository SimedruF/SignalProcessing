# SignalProcessing ImGui Interactive Demo

Aplicație interactivă cu interfață grafică pentru demonstrarea funcționalităților clasei `SignalProcessing`.

![Demo Preview](screenshot.png)

## Features

### 🎛️ Signal Generation
- **Multiple Signal Types**:
  - Sine Wave - Undă sinusoidală simplă
  - ECG Signal - Semnal cardiac simplificat (P-QRS-T)
  - Turbine Vibration - Vibrații cu armonici
  - White Noise - Zgomot aleator
- **Parameters**:
  - Frecvență ajustabilă (0.1 - 10 Hz)
  - Nivel de zgomot (0-100%)
  - Lungime semnal (100-1000 sample-uri)

### 🔧 Filters (Real-time)
- **Kalman Filter**
  - Process noise ajustabil
  - Measurement noise ajustabil
- **Median Filter**
  - Window size: 3, 5, 7, 9, 11
- **Wavelet Denoising**
  - Haar wavelet decomposition

### ⚠️ Anomaly Detection
- **Z-Score Method** - Detectare bazată pe deviații standard
- **IQR Method** - Interquartile Range (robust la outliers)
- **MAD Method** - Moving Average Deviation
- Threshold ajustabil
- Vizualizare anomalii pe grafic

### 📈 Analysis Tools
- **FFT (Fast Fourier Transform)**
  - Spectru de frecvență în timp real
  - Identificare frecvențe dominante
- **ML Feature Extraction**
  - 21 features pentru Machine Learning
  - Statistical features (8)
  - Frequency features (9)
  - Time domain features (4)

### 📊 Visualization
- **Time Domain Plot**
  - Semnal original vs. filtrat
  - Overlay anomalii
  - Zoom și pan interactiv
- **Frequency Spectrum**
  - Magnitudine vs. frecvență
  - Identificare armonici

## Prerequisites

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libglfw3-dev libgl1-mesa-dev
```

### macOS
```bash
brew install cmake glfw
```

### Windows
Instalează [vcpkg](https://github.com/microsoft/vcpkg):
```cmd
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install glfw3:x64-windows
vcpkg integrate install
```

## Build Instructions

### Step 1: Setup (download ImGui and ImPlot)

**Linux/macOS:**
```bash
cd examples
chmod +x setup_imgui.sh
./setup_imgui.sh
```

**Windows:**
```cmd
cd examples
setup_imgui.bat
```

### Step 2: Build

**Linux/macOS:**
```bash
cd build
cmake ..
make
./imgui_demo
```

**Windows (with vcpkg):**
```cmd
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .
.\Debug\imgui_demo.exe
```

**Windows (Visual Studio):**
- Open `examples/CMakeLists.txt` in Visual Studio
- Build and run (F5)

## Usage Guide

### Basic Workflow

1. **Generate Signal**
   - Select signal type from dropdown
   - Adjust frequency and noise level
   - Click "Regenerate Signal"

2. **Apply Filters**
   - Enable desired filters (Kalman/Median/Wavelet)
   - Tune filter parameters
   - Click "Apply Filters"
   - Compare original vs. filtered in plot

3. **Detect Anomalies**
   - Check "Show Anomalies"
   - Select detection method
   - Adjust threshold
   - Click "Detect Anomalies"
   - Anomalies appear as red dots on plot

4. **Analyze Spectrum**
   - Click "Compute FFT"
   - View frequency spectrum below time plot
   - Identify dominant frequencies

5. **Extract ML Features**
   - Check "Show ML Features"
   - View all 21 features in panel
   - Perfect for training ML models

### Example Scenarios

#### ECG Analysis
```
1. Set Signal Type: "ECG Signal"
2. Frequency: 1.2 Hz (72 BPM)
3. Noise Level: 0.15
4. Apply Kalman Filter (0.01, 0.1)
5. Compute FFT → See heart rate frequency
6. Extract ML Features → Get RMS, dominant freq, etc.
```

#### Turbine Monitoring
```
1. Set Signal Type: "Turbine Vibration"
2. Add some noise (0.2)
3. Apply Median Filter (window=5)
4. Enable "Show Anomalies" (Z-Score, threshold=3.0)
5. Compute FFT → See 50 Hz fundamental + harmonics
6. Extract ML Features → Check high-freq power for bearing wear
```

#### Filter Comparison
```
1. Generate noisy sine wave
2. Enable Kalman Filter → observe smoothing
3. Disable Kalman, enable Median Filter → compare
4. Enable Wavelet → see different behavior
```

## Code Structure

```cpp
// Main components:

// 1. Signal generation
void GenerateSignal() {
    // Creates signal based on type
}

// 2. Filter application
void ApplyFilters() {
    // Applies Kalman/Median/Wavelet
}

// 3. Anomaly detection
void DetectAnomalies() {
    // Uses SignalProcessing methods
}

// 4. FFT computation
void ComputeFFT() {
    // FrequencySpectrum analysis
}

// 5. ML feature extraction
void ExtractMLFeatures() {
    // MLFeatureVector with 21 features
}
```

## Customization

### Adding New Signal Types

Edit `GenerateSignal()` in `imgui_demo.cpp`:

```cpp
case SIGNAL_CUSTOM:
    value = your_custom_signal_function(t);
    break;
```

### Adding New Visualizations

Use ImPlot API:

```cpp
if (ImPlot::BeginPlot("My Plot")) {
    ImPlot::PlotLine("Data", x_data, y_data, count);
    ImPlot::EndPlot();
}
```

### Adding New Filters

```cpp
bool apply_custom_filter = false;

// In UI:
if (ImGui::Checkbox("Custom Filter", &apply_custom_filter)) {
    ApplyFilters();
}

// In ApplyFilters():
if (apply_custom_filter) {
    // Your filter logic
}
```

## Keyboard Shortcuts

- `F` - Fit plot to data
- `Right-click + drag` - Pan plot
- `Scroll wheel` - Zoom plot
- `Double-click` - Reset zoom

## Performance

- Real-time for signals up to 1000 samples
- FFT computation: ~1-5ms
- ML feature extraction: ~2-10ms (includes FFT)
- 60 FPS rendering

## Troubleshooting

### Build Errors

**"GLFW not found"**
```bash
# Make sure GLFW is installed
# Linux: sudo apt-get install libglfw3-dev
# macOS: brew install glfw
# Windows: vcpkg install glfw3:x64-windows
```

**"ImGui headers not found"**
```bash
# Run setup script first
./setup_imgui.sh  # or setup_imgui.bat on Windows
```

### Runtime Issues

**Window doesn't open**
- Check OpenGL drivers are installed
- Try running with `--version` to check GLFW installation

**Plots not showing**
- Make sure ImPlot was cloned correctly
- Check console for errors

## Screenshots

### Main Interface
Shows signal generation, filtering, and real-time plotting.

### Anomaly Detection
Red markers showing detected anomalies in ECG signal.

### FFT Analysis
Frequency spectrum showing harmonics in turbine vibration.

### ML Features
All 21 features displayed for machine learning applications.

## See Also

- [SignalProcessing Library](../README.md)
- [ML Features Guide](../ML_FEATURES_GUIDE.md)
- [Anomaly Detection Guide](../ANOMALY_DETECTION_GUIDE.md)
- [ImGui Documentation](https://github.com/ocornut/imgui)
- [ImPlot Documentation](https://github.com/epezent/implot)

## License

MIT License - Same as SignalProcessing library
