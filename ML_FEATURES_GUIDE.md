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

## Downstream ML/AI Integration

### Overview

Biblioteca oferă acum **integrare completă** pentru preprocessing batch, training statistics și export către framework-uri externe ML/AI.

### Dataset Management

#### Crearea unui Dataset

```cpp
MLDataset training_set;
SignalProcessing::CreateMLDataset(100, true, &training_set);
// capacity=100 samples, with_labels=true pentru supervised learning
```

#### Adăugarea de Sample-uri

```cpp
// Extrage features din semnal
MLFeatureVector features;
sp.ExtractMLFeatures(100.0, &features);

// Adaugă la dataset cu label
SignalProcessing::AddFeaturesToDataset(&training_set, &features, label);
```

#### Eliberarea Memoriei

```cpp
SignalProcessing::FreeMLDataset(&training_set);
```

### Rolling Window Processing

Pentru time series și streaming data:

```cpp
SignalProcessing sp;
// ... adaugă date ...

MLDataset windows;
SignalProcessing::CreateMLDataset(50, false, &windows);

int num_windows = sp.ExtractMLFeaturesRollingWindow(
    100,    // window_size
    50,     // step_size (50% overlap)
    100.0,  // sampling_rate
    &windows
);

// Acum ai features pentru fiecare fereastră
```

### Training Statistics & Normalization

#### Calculează Statistici din Training Set

```cpp
MLTrainingStats stats;
SignalProcessing::ComputeTrainingStats(&training_set, &stats);

// stats conține acum mean, std, min, max pentru fiecare feature
// Salvează aceste statistici pentru a normaliza datele noi!
```

#### Normalizează Dataset (Z-Score)

```cpp
// Normalizează toate sample-urile din dataset
SignalProcessing::NormalizeDataset(&training_set, &stats);
```

#### Normalizează Features Noi (Inference)

```cpp
// Pentru predicție pe date noi
MLFeatureVector new_features;
sp.ExtractMLFeatures(100.0, &new_features);

// Aplică aceleași statistici de training
SignalProcessing::NormalizeMLFeatures(&new_features, 
                                      stats.mean_values, 
                                      stats.std_values);
```

### CSV Export pentru Python/MATLAB

```cpp
// Exportă dataset ca CSV
SignalProcessing::ExportDatasetToCSV(&training_set, 
                                     "training_data.csv", 
                                     true);  // include labels
```

**Python Integration:**
```python
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC

# Load data
df = pd.read_csv('training_data.csv')
X = df.iloc[:, :-1].values  # All features (21)
y = df.iloc[:, -1].values   # Labels

# Train model
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
model = SVC(kernel='rbf')
model.fit(X_train, y_train)
accuracy = model.score(X_test, y_test)
```

### Batch Processing

Pentru procesarea simultană a mai multor semnale:

```cpp
// Array de semnale din mai mulți senzori
double *signals[10];
int sizes[10];

// ... populează signals și sizes ...

MLDataset batch;
SignalProcessing::CreateMLDataset(10, false, &batch);

int processed = SignalProcessing::BatchExtractFeatures(
    signals, sizes, 10, 100.0, &batch
);

// Acum ai features pentru toate cele 10 semnale
```

### Complete Workflow: ECG Classification

```cpp
// 1. Colectează training data
MLDataset training_set;
SignalProcessing::CreateMLDataset(100, true, &training_set);

for (int i = 0; i < num_patients; i++) {
    SignalProcessing sp;
    // ... adaugă date ECG ...
    
    // Preprocesare
    double denoised[1000];
    sp.KalmanFilter(0.01, 0.1, denoised);
    
    SignalProcessing clean;
    for (int j = 0; j < sp.GetIndex(); j++) {
        clean.AddValue(denoised[j]);
    }
    
    // Extrage features
    MLFeatureVector features;
    clean.ExtractMLFeatures(100.0, &features);
    
    // Adaugă la dataset
    int label = patient_has_arrhythmia[i] ? 1 : 0;
    SignalProcessing::AddFeaturesToDataset(&training_set, &features, label);
}

// 2. Calculează training statistics
MLTrainingStats stats;
SignalProcessing::ComputeTrainingStats(&training_set, &stats);

// 3. Normalizează dataset
SignalProcessing::NormalizeDataset(&training_set, &stats);

// 4. Exportă pentru training extern (CSV sau HDF5)
SignalProcessing::ExportDatasetToCSV(&training_set, "ecg_train.csv", true);

// Opțional: Export HDF5 (dacă ai biblioteca instalată și -DUSE_HDF5)
SignalProcessing::ExportDatasetToH5(&training_set, "ecg_train.h5", 
                                     true, "/Medical/ECG");
SignalProcessing::ExportTrainingStatsToH5(&stats, "ecg_params.h5",
                                          "/Medical/Normalization");

// 5. Training în Python (TensorFlow/Keras)
// ... antrenează modelul folosind CSV-ul exportat ...

// 6. Inference pe date noi
SignalProcessing new_patient;
// ... adaugă date ECG noi ...

MLFeatureVector test_features;
new_patient.ExtractMLFeatures(100.0, &test_features);

// Normalizează folosind ACELAȘI stats din training
SignalProcessing::NormalizeMLFeatures(&test_features, 
                                      stats.mean_values, 
                                      stats.std_values);

// Export pentru predicție
double feature_array[21];
SignalProcessing::ExportFeaturesToArray(&test_features, feature_array);

// feature_array gata pentru model.predict() în Python sau inferență C++
```

### Integration Testing

```bash
# Build și run test complet
cd test
./build_ml_downstream.sh   # Linux
.\build_ml_downstream.bat  # Windows

./test_ml_downstream
```

Test demonstrează:
1. Dataset creation și management
2. Batch feature extraction (ECG classification)
3. Rolling window processing (time series)
4. Training statistics computation
5. Dataset normalization (z-score)
6. CSV export pentru Python/MATLAB
7. Batch processing pentru inference
8. Complete workflow: signal → preprocessing → features → normalization → model input

## See Also

- `ANOMALY_DETECTION_GUIDE.md` - Anomaly detection methods
- `HDF5_EXPORT_GUIDE.md` - HDF5 export for ML/AI datasets (optional feature)
- `test/test_ml_features.cpp` - Feature extraction examples
- `test/test_ml_downstream.cpp` - Complete downstream ML/AI integration (datasets, batch processing, CSV export)
- `test/test_ml_h5export.cpp` - HDF5 export examples (requires HDF5 library)
- `examples/example_complete.cpp` - ECG processing workflow

## Framework Integration Resources

### TensorFlow / Keras (Python)
```python
# Import CSV și train neural network
import tensorflow as tf
from tensorflow import keras
import pandas as pd

df = pd.read_csv('training_data.csv')
X = df.iloc[:, :-1].values
y = df.iloc[:, -1].values

model = keras.Sequential([
    keras.layers.Dense(64, activation='relu', input_shape=(21,)),
    keras.layers.Dropout(0.3),
    keras.layers.Dense(32, activation='relu'),
    keras.layers.Dense(1, activation='sigmoid')
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
model.fit(X, y, epochs=50, batch_size=16, validation_split=0.2)
```

### PyTorch (Python)
```python
import torch
import torch.nn as nn
import pandas as pd

df = pd.read_csv('training_data.csv')
X = torch.FloatTensor(df.iloc[:, :-1].values)
y = torch.FloatTensor(df.iloc[:, -1].values)

class ECGClassifier(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(21, 64)
        self.fc2 = nn.Linear(64, 32)
        self.fc3 = nn.Linear(32, 1)
        
    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        return torch.sigmoid(self.fc3(x))
```

### scikit-learn SVM (Python)
```python
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split
import pandas as pd

df = pd.read_csv('training_data.csv')
X = df.iloc[:, :-1].values
y = df.iloc[:, -1].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

svm = SVC(kernel='rbf', C=1.0, gamma='scale')
svm.fit(X_train, y_train)
accuracy = svm.score(X_test, y_test)
print(f"SVM Accuracy: {accuracy:.2%}")
```

### MATLAB Machine Learning Toolbox
```matlab
% Import CSV
data = readtable('training_data.csv');
X = table2array(data(:, 1:21));
y = table2array(data(:, 22));

% Train SVM
svmModel = fitcsvm(X, y, 'KernelFunction', 'rbf', 'Standardize', true);

% Cross-validation
cvModel = crossval(svmModel);
accuracy = 1 - kfoldLoss(cvModel);
fprintf('SVM Accuracy: %.2f%%\n', accuracy * 100);
```
