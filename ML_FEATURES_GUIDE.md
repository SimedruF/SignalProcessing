# ML/AI Integration Guide

## Overview

Biblioteca SignalProcessing oferă funcții dedicate pentru **extragerea automată de features (caracteristici)** din semnale preprocesate, pregătite pentru a fi folosite direct ca input în modele de Machine Learning și AI.

## Conceptul "Downstream ML/AI"

**Downstream** înseamnă că semnalele procesate devin **date de intrare** pentru algoritmi ML/AI:

```
Semnal brut → Preprocesare → Extragere Features → Model ML → Predicție
              (SignalProcessing)  (MLFeatureVector)  (TensorFlow, SVM)
```

### Cazuri de utilizare tipice:

#### 1. Monitorizare medicală ECG
```
ECG brut → Kalman filter → 21 features → Neural Network → Clasificare:
                                                          - Normal
                                                          - Aritmie
                                                          - Fibrilație
```

#### 2. Predictive Maintenance pentru turbine
```
Vibrații → Median filter → Features per blade → SVM → Predicție:
                           (RMS, FFT, harmonics)      - OK
                                                      - Uzură
                                                      - Defect iminent
```

## Feature Vector Structure

Structura `MLFeatureVector` conține **21 de features** împărțite în 3 categorii:

### Statistical Features (8)
- `mean` - Media valorilor
- `std_dev` - Deviația standard
- `variance` - Varianța
- `skewness` - Asimetria distribuției
- `kurtosis` - "Spikiness" al distribuției
- `rms` - Root Mean Square (energie)
- `peak_to_peak` - Amplitudine totală
- `crest_factor` - Raport vârf/RMS (detectează spikes)

### Frequency Features (9)
- `dominant_frequency` - Frecvența principală
- `spectral_centroid` - "Centru de greutate" al spectrului
- `spectral_spread` - Lățimea spectrului
- `spectral_entropy` - Complexitatea spectrală
- `total_power` - Puterea totală
- `power_low_freq` - Putere în 0-10 Hz
- `power_mid_freq` - Putere în 10-100 Hz
- `power_high_freq` - Putere în 100+ Hz

### Time Domain Features (4)
- `zero_crossing_rate` - Rata de trecere prin zero
- `mean_crossing_rate` - Rata de trecere prin media semnalului
- `energy` - Energia totală
- `autocorr_peak` - Periodicitate (detectează pattern-uri)

## API Functions

### 1. ExtractMLFeatures()
Extrage toate cele 21 de features din întreg semnalul:

```cpp
SignalProcessing sp;
// ... adaugă date cu AddValue() ...

MLFeatureVector features;
bool success = sp.ExtractMLFeatures(sampling_rate, &features);

// Acum poți folosi features.mean, features.rms, etc.
```

### 2. ExtractMLFeaturesFromSegment()
Extrage features dintr-un segment specific (util pentru analiza pe blade-uri, ferestre temporale):

```cpp
// Extrage features din primele 100 de sample-uri
MLFeatureVector blade1_features;
sp.ExtractMLFeaturesFromSegment(0, 100, sampling_rate, &blade1_features);

// Compară cu al doilea blade
MLFeatureVector blade2_features;
sp.ExtractMLFeaturesFromSegment(100, 100, sampling_rate, &blade2_features);

if (fabs(blade1_features.rms - blade2_features.rms) > 0.1) {
    printf("⚠ Possible imbalance detected!\n");
}
```

### 3. ExportFeaturesToArray()
Exportă features într-un array flat, formatul standard pentru biblioteci ML:

```cpp
MLFeatureVector features;
sp.ExtractMLFeatures(100.0, &features);

double feature_array[21];
int num_features = sp.ExportFeaturesToArray(&features, feature_array);

// feature_array este gata pentru numpy, TensorFlow, PyTorch
// Ordine: Statistical (8) → Frequency (9) → Time (4)
```

### 4. NormalizeMLFeatures()
Normalizează features folosind z-score (esențial pentru rețele neuronale):

```cpp
// Calculează mean și std din setul de antrenament (offline)
double mean_values[21] = { /* compute from training data */ };
double std_values[21] = { /* compute from training data */ };

// Normalizează features noi
sp.NormalizeMLFeatures(&features, mean_values, std_values);

// Acum features sunt normalizate: (x - mean) / std
// Gata pentru input în neural network
```

## Complete Example: ECG Classification

```cpp
#include "SignalProcessing.h"

int main() {
    // 1. Colectează date ECG
    SignalProcessing sp;
    for (int i = 0; i < 500; i++) {
        double ecg_value = read_ecg_sensor();
        sp.AddValue(ecg_value);
    }
    
    // 2. Preprocesare
    double denoised[1000];
    sp.KalmanFilter(0.01, 0.1, denoised);
    
    SignalProcessing sp_clean;
    for (int i = 0; i < sp.GetIndex(); i++) {
        sp_clean.AddValue(denoised[i]);
    }
    
    // 3. Extrage features pentru ML
    MLFeatureVector features;
    sp_clean.ExtractMLFeatures(100.0, &features);
    
    // 4. Exportă pentru model ML
    double input_vector[21];
    sp_clean.ExportFeaturesToArray(&features, input_vector);
    
    // 5. Normalizează
    sp_clean.NormalizeMLFeatures(&features, trained_means, trained_stds);
    
    // 6. Feed to ML model (pseudo-code)
    // int prediction = neural_network.predict(input_vector);
    // if (prediction == ARRHYTHMIA) { alert_doctor(); }
    
    return 0;
}
```

## Integration with Popular ML Frameworks

### Python/NumPy Export Pattern

```cpp
// C++ side: Export features
MLFeatureVector features;
sp.ExtractMLFeatures(100.0, &features);

double feature_array[21];
sp.ExportFeaturesToArray(&features, feature_array);

// Write to file for Python
FILE *fp = fopen("features.txt", "w");
for (int i = 0; i < 21; i++) {
    fprintf(fp, "%.6f\n", feature_array[i]);
}
fclose(fp);
```

```python
# Python side: Load and classify
import numpy as np
from sklearn.svm import SVC

# Load features
features = np.loadtxt('features.txt')

# Use trained model
model = SVC()  # Previously trained
prediction = model.predict(features.reshape(1, -1))

print(f"Classification: {prediction[0]}")  # 0=Normal, 1=Anomaly
```

### Real-time Classification Pattern

```cpp
// Continuous monitoring loop
while (true) {
    // Collect window of data
    for (int i = 0; i < WINDOW_SIZE; i++) {
        sp.AddValue(read_sensor());
    }
    
    // Extract features
    MLFeatureVector features;
    sp.ExtractMLFeatures(SAMPLING_RATE, &features);
    
    // Classify (using embedded ML model)
    int class_id = classify(features);  // Your ML inference code
    
    if (class_id == FAULT_CLASS) {
        trigger_alarm();
    }
    
    sp.ClearVector();  // Reset for next window
}
```

## Training Workflow

### Step 1: Collect Training Data
```cpp
// Collect features from normal operation
SignalProcessing sp;
for (each training sample) {
    sp.ClearVector();
    // Add signal data
    for (int i = 0; i < N; i++) sp.AddValue(training_data[i]);
    
    // Extract features
    MLFeatureVector features;
    sp.ExtractMLFeatures(sampling_rate, &features);
    
    // Save to dataset
    save_to_file(features, label);
}
```

### Step 2: Train Model (Python example)
```python
import numpy as np
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler

# Load all features
X_train = load_feature_dataset()  # Shape: (n_samples, 21)
y_train = load_labels()           # Labels: 0=Normal, 1=Fault

# Compute normalization parameters
scaler = StandardScaler()
X_normalized = scaler.fit_transform(X_train)

# Save mean and std for C++ code
np.savetxt('mean_values.txt', scaler.mean_)
np.savetxt('std_values.txt', scaler.scale_)

# Train model
model = SVC(kernel='rbf')
model.fit(X_normalized, y_train)
```

### Step 3: Deploy in C++
```cpp
// Load normalization params computed in Python
double mean_values[21], std_values[21];
load_params("mean_values.txt", mean_values);
load_params("std_values.txt", std_values);

// Runtime classification
sp.ExtractMLFeatures(sampling_rate, &features);
sp.NormalizeMLFeatures(&features, mean_values, std_values);

// Use deployed model (e.g., ONNX, TensorFlow Lite)
int prediction = model.predict(features);
```

## Feature Importance by Application

### ECG Analysis
**Most important:**
- `rms` - Overall signal energy
- `dominant_frequency` - Heart rate
- `zero_crossing_rate` - QRS detection
- `autocorr_peak` - Regularity of heartbeat

### Turbine Monitoring
**Most important:**
- `rms` - Vibration level
- `dominant_frequency` - Rotation speed
- `power_high_freq` - Bearing wear indicator
- `crest_factor` - Impact detection

### Audio/Speech
**Most important:**
- `spectral_centroid` - Brightness
- `spectral_spread` - Timbre
- `zero_crossing_rate` - Voicing
- `energy` - Loudness

## Performance Notes

- Feature extraction includes FFT → Computationally intensive
- For real-time: Consider extracting features every N samples (windowing)
- Typical extraction time: ~1-5ms for 500 samples on modern CPU
- Memory: Allocate `MLFeatureVector` (168 bytes) + temp FFT buffers

## Testing

```bash
# Build and run test
cd test
./build_ml_features.sh   # Linux
.\build_ml_features.bat  # Windows

./test_ml_features
```

Test demonstrates:
1. ECG feature extraction
2. Export to ML array format
3. Turbine vibration features
4. Per-segment (blade) analysis
5. Feature normalization

## See Also

- `ANOMALY_DETECTION_GUIDE.md` - Anomaly detection methods
- `test/test_ml_features.cpp` - Complete usage examples
- `examples/example_complete.cpp` - ECG processing workflow
