# HDF5 Export for ML/AI - Setup Guide

## Overview

SignalProcessing oferă suport opțional pentru export HDF5 prin biblioteca SignalRecorder.hpp. Acest format este ideal pentru:
- **Stocarea hierarhică** a dataset-urilor ML
- **Interoperabilitate** cu Python (h5py, TensorFlow, PyTorch)
- **Compresie eficientă** pentru dataset-uri mari
- **Metadata** asociate cu semnale și features

## Features

### 1. Export ML Datasets
```cpp
MLDataset training_set;
// ... populează dataset ...

SignalProcessing::ExportDatasetToH5(&training_set, "training.h5", 
                                     true, "/ML/Training");
```

**Structură HDF5:**
```
/ML/Training/
  ├── mean              (double array, N samples)
  ├── std_dev           (double array, N samples)
  ├── variance          (double array, N samples)
  ├── ... (toate cele 21 features)
  ├── labels            (double array, N samples)
  └── attributes: num_samples, num_features, has_labels
```

### 2. Export Training Statistics
```cpp
MLTrainingStats stats;
SignalProcessing::ComputeTrainingStats(&training_set, &stats);

SignalProcessing::ExportTrainingStatsToH5(&stats, "model_params.h5",
                                           "/Normalization");
```

Salvează parametrii de normalizare (mean, std, min, max) pentru toate features.

### 3. Export Features + Raw Signal
```cpp
SignalProcessing sp;
// ... adaugă date ...

MLFeatureVector features;
sp.ExtractMLFeatures(100.0, &features);

sp.ExportMLFeaturesToH5("signal_analysis.h5", &features,
                        "/Sensors/Sensor_01", true);
```

Salvează atât features-urile cât și semnalul brut pentru debugging și vizualizare.

## Installation

### Windows

#### Option 1: Pre-built Binaries (Recommended)
1. Download HDF5 for Windows from: https://www.hdfgroup.org/downloads/hdf5/
2. Install (e.g., `C:\Program Files\HDF5\`)
3. Add to system PATH:
   - `C:\Program Files\HDF5\bin`
   - `C:\Program Files\HDF5\lib`

#### Option 2: Build from Source
```powershell
# Clone HDF5
git clone https://github.com/HDFGroup/hdf5.git
cd hdf5

# Build with CMake
mkdir build
cd build
cmake .. -DHDF5_BUILD_CPP_LIB=ON -DBUILD_TESTING=OFF
cmake --build . --config Release
cmake --install .
```

#### Compilation
```cmd
g++ -std=c++11 -DWINDOWS -DUSE_HDF5 -o my_app.exe my_app.cpp SignalProcessing.cpp ^
    -I../source -I"C:\Program Files\HDF5\include" ^
    -L"C:\Program Files\HDF5\lib" -lhdf5_cpp -lhdf5
```

### Linux

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libhdf5-dev libhdf5-cpp-103
```

#### Fedora/RHEL/CentOS
```bash
sudo dnf install hdf5-devel
```

#### Arch Linux
```bash
sudo pacman -S hdf5
```

#### Compilation
```bash
g++ -std=c++11 -DUSE_HDF5 -o my_app my_app.cpp SignalProcessing.cpp \
    -I../source -lhdf5_cpp -lhdf5 -lm -lrt
```

### macOS
```bash
brew install hdf5

# Compilation
g++ -std=c++11 -DUSE_HDF5 -o my_app my_app.cpp SignalProcessing.cpp \
    -I../source -I/usr/local/include -L/usr/local/lib \
    -lhdf5_cpp -lhdf5
```

## Using Without HDF5

Dacă **NU** ai HDF5 instalat, biblioteca funcționează în continuare perfect:
- Toate funcțiile HDF5 returnează `false`
- Funcționalitatea CSV export rămâne disponibilă
- Nu este necesară nicio modificare în cod

## Python Integration

### Loading HDF5 Datasets

```python
import h5py
import numpy as np

# Load training dataset
with h5py.File('training.h5', 'r') as f:
    # Citește toate features-urile
    feature_names = ['mean', 'std_dev', 'variance', 'skewness', 'kurtosis', 
                     'rms', 'peak_to_peak', 'crest_factor',
                     'dominant_freq', 'spectral_centroid', 'spectral_spread', 
                     'spectral_entropy', 'total_power', 'power_low_freq', 
                     'power_mid_freq', 'power_high_freq',
                     'zero_crossing_rate', 'mean_crossing_rate', 
                     'energy', 'autocorr_peak']
    
    features = []
    for name in feature_names:
        features.append(f['/ML/Training'][name][:])
    
    X = np.column_stack(features)  # Shape: (N, 21)
    y = f['/ML/Training/labels'][:]  # Shape: (N,)
    
    # Metadata
    num_samples = f['/ML/Training'].attrs['num_samples']
    print(f"Loaded {num_samples} samples")

# Load normalization parameters
with h5py.File('model_params.h5', 'r') as f:
    mean_vals = f['/Normalization/mean_values'][:]
    std_vals = f['/Normalization/std_values'][:]
    
    # Apply normalization
    X_normalized = (X - mean_vals) / std_vals
```

### TensorFlow Integration

```python
import tensorflow as tf
import h5py

# Create TensorFlow dataset from HDF5
def load_h5_dataset(filename, path='/ML/Training'):
    with h5py.File(filename, 'r') as f:
        # Load data
        features = []
        for name in feature_names:
            features.append(f[path][name][:])
        X = np.column_stack(features)
        y = f[path + '/labels'][:]
    
    # Create TF dataset
    dataset = tf.data.Dataset.from_tensor_slices((X, y))
    dataset = dataset.shuffle(1000).batch(32).prefetch(tf.data.AUTOTUNE)
    return dataset

# Use in training
train_dataset = load_h5_dataset('training.h5')
model.fit(train_dataset, epochs=50)
```

### PyTorch Integration

```python
import torch
from torch.utils.data import Dataset, DataLoader
import h5py

class H5Dataset(Dataset):
    def __init__(self, filename, path='/ML/Training'):
        with h5py.File(filename, 'r') as f:
            features = []
            for name in feature_names:
                features.append(f[path][name][:])
            self.X = torch.FloatTensor(np.column_stack(features))
            self.y = torch.FloatTensor(f[path + '/labels'][:])
    
    def __len__(self):
        return len(self.y)
    
    def __getitem__(self, idx):
        return self.X[idx], self.y[idx]

# Use in training
dataset = H5Dataset('training.h5')
dataloader = DataLoader(dataset, batch_size=32, shuffle=True)

for features, labels in dataloader:
    outputs = model(features)
    loss = criterion(outputs, labels)
    # ...
```

## MATLAB Integration

```matlab
% Load dataset
filename = 'training.h5';
path = '/ML/Training';

% Read features
mean_vals = h5read(filename, [path '/mean']);
std_vals = h5read(filename, [path '/std_dev']);
rms_vals = h5read(filename, [path '/rms']);
% ... (toate features)

% Combine into matrix
X = [mean_vals, std_vals, rms_vals, ...];

% Read labels
y = h5read(filename, [path '/labels']);

% Train model
svm_model = fitcsvm(X, y, 'KernelFunction', 'rbf');

% Load normalization parameters
mean_norm = h5read('model_params.h5', '/Normalization/mean_values');
std_norm = h5read('model_params.h5', '/Normalization/std_values');

% Apply to new data
X_new_normalized = (X_new - mean_norm') ./ std_norm';
```

## Example: Complete Workflow

```cpp
#include "SignalProcessing.h"
#include <stdio.h>

int main() {
    // 1. Create and populate dataset
    MLDataset training_set;
    SignalProcessing::CreateMLDataset(100, true, &training_set);
    
    // ... collect signals and extract features ...
    
    // 2. Export dataset to HDF5
    bool success = SignalProcessing::ExportDatasetToH5(
        &training_set, 
        "ecg_training.h5",
        true,  // include labels
        "/Medical/ECG/Training"
    );
    
    if (success) {
        printf("Dataset exported to HDF5\n");
    } else {
        printf("HDF5 not available, use CSV export instead\n");
        SignalProcessing::ExportDatasetToCSV(&training_set, "ecg_training.csv", true);
    }
    
    // 3. Compute and export training statistics
    MLTrainingStats stats;
    SignalProcessing::ComputeTrainingStats(&training_set, &stats);
    SignalProcessing::ExportTrainingStatsToH5(&stats, "ecg_params.h5",
                                              "/Medical/ECG/Normalization");
    
    // 4. Now train model in Python using the HDF5 files
    
    SignalProcessing::FreeMLDataset(&training_set);
    return 0;
}
```

## Troubleshooting

### Error: "H5Cpp.h: No such file or directory"
- HDF5 library not installed or not in include path
- Solution: Install HDF5 or specify include path with `-I` flag

### Linker Error: "undefined reference to H5::..."
- HDF5 libraries not linked
- Solution: Add `-lhdf5_cpp -lhdf5` to compilation command

### Functions Return False
- Compiled without `-DUSE_HDF5` flag
- HDF5 library not available at runtime
- Check that HDF5 DLLs/shared libraries are in PATH/LD_LIBRARY_PATH

### "Symbol not found: _H5..." (macOS)
- Library path issue
- Solution: `export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH`

## Advantages of HDF5 over CSV

| Feature | HDF5 | CSV |
|---------|------|-----|
| Hierarchical structure | ✓ | ✗ |
| Metadata support | ✓ | ✗ |
| Compression | ✓ | ✗ |
| Partial reading | ✓ | ✗ |
| Binary format | ✓ | ✗ |
| File size | Smaller | Larger |
| Human readable | ✗ | ✓ |
| Python integration | Excellent | Good |
| TensorFlow native | ✓ | Requires parsing |

## See Also

- [ML_FEATURES_GUIDE.md](ML_FEATURES_GUIDE.md) - ML/AI feature extraction
- [test/test_ml_h5export.cpp](test/test_ml_h5export.cpp) - Complete usage examples
- [HDF5 Official Documentation](https://www.hdfgroup.org/solutions/hdf5/)
