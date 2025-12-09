# Anomaly Detection Guide

## Overview

This library provides comprehensive anomaly detection capabilities specifically designed for industrial applications such as turbine monitoring, rotating machinery analysis, and predictive maintenance.

## Table of Contents
1. [Core Concepts](#core-concepts)
2. [Detection Methods](#detection-methods)
3. [Signal Segmentation](#signal-segmentation)
4. [Practical Examples](#practical-examples)
5. [Best Practices](#best-practices)

---

## Core Concepts

### What is an Anomaly?
An anomaly is a data point or pattern that significantly deviates from the expected behavior of a signal. In industrial monitoring:
- **Turbine blades**: Excessive vibration indicating damage or imbalance
- **Motors**: Unusual frequency components suggesting bearing wear
- **Sensors**: Spikes or drift indicating malfunction

### When to Use Each Method

| Method | Best For | Typical Threshold |
|--------|----------|-------------------|
| Z-Score | Gaussian-distributed data, general anomalies | 3.0 (99.7%) |
| IQR | Non-Gaussian data, robust to outliers | 1.5 (outliers), 3.0 (extreme) |
| MAD | Local deviations, time-varying signals | 3.0 - 5.0 |
| Sudden Change | Spikes, rapid transitions | Domain-specific |
| Periodic | Rotating machinery, cyclical processes | 2.0 - 4.0 |

---

## Detection Methods

### 1. Z-Score Method (Statistical Threshold)

**Description**: Detects values that deviate from the mean by a specified number of standard deviations.

**Formula**: `z = (x - μ) / σ`

**When to use**:
- Signal follows approximately normal distribution
- Looking for global outliers
- Simple, interpretable results

**Example**:
```cpp
SignalProcessing sp;
// ... add your data ...

int anomaly_indices[1000];
int num_anomalies = sp.DetectAnomaliesZScore(3.0, anomaly_indices, 1000);

printf("Found %d anomalies using Z-Score method\n", num_anomalies);
for (int i = 0; i < num_anomalies; ++i) {
    printf("Anomaly at index: %d, value: %.4f\n", 
           anomaly_indices[i], 
           sp.GetValue(anomaly_indices[i]));
}
```

**Interpretation**:
- `threshold = 2.0`: ~95% confidence (5% false positives)
- `threshold = 3.0`: ~99.7% confidence (0.3% false positives)
- `threshold = 4.0`: ~99.99% confidence (very strict)

---

### 2. IQR Method (Interquartile Range)

**Description**: Uses quartiles to detect outliers, more robust to extreme values than Z-score.

**Formula**: 
- `IQR = Q3 - Q1`
- `Lower bound = Q1 - k * IQR`
- `Upper bound = Q3 + k * IQR`

**When to use**:
- Data has outliers that would skew mean/std dev
- Non-Gaussian distributions
- More conservative detection needed

**Example**:
```cpp
SignalProcessing sp;
// ... add your data ...

int anomaly_indices[1000];
// Use 1.5 for standard outliers, 3.0 for extreme outliers
int num_anomalies = sp.DetectAnomaliesIQR(1.5, anomaly_indices, 1000);

printf("Found %d outliers using IQR method\n", num_anomalies);
```

**Interpretation**:
- `multiplier = 1.5`: Tukey's fences (standard outliers)
- `multiplier = 3.0`: Extreme outliers only
- More robust to non-normal distributions

---

### 3. Moving Average Deviation (MAD)

**Description**: Compares each point to a local moving average, detecting deviations from recent trends.

**When to use**:
- Signal has time-varying mean
- Want to detect local anomalies
- Trending data
- Real-time monitoring

**Example**:
```cpp
SignalProcessing sp;
// ... add your data ...

int anomaly_indices[1000];
int window_size = 20;      // Look at 20 previous samples
double threshold_factor = 3.0;  // 3 standard deviations

int num_anomalies = sp.DetectAnomaliesMAD(window_size, threshold_factor, 
                                          anomaly_indices, 1000);

printf("Found %d local anomalies\n", num_anomalies);
```

**Parameter Selection**:
- **window_size**: 
  - Smaller (5-20): Detects rapid changes
  - Larger (50-100): Detects sustained deviations
- **threshold_factor**:
  - 2.0-3.0: Sensitive detection
  - 3.0-5.0: Conservative detection

---

### 4. Sudden Change Detection

**Description**: Detects rapid transitions or spikes in the signal.

**When to use**:
- Catching sensor spikes
- Detecting impacts or shocks
- Monitoring for abrupt transitions
- Quality control in manufacturing

**Example**:
```cpp
SignalProcessing sp;
// ... add your data ...

int anomaly_indices[1000];
double threshold = 2.0;  // Minimum change magnitude

int num_spikes = sp.DetectSuddenChanges(threshold, anomaly_indices, 1000);

printf("Found %d sudden changes\n", num_spikes);
```

---

### 5. Periodic Anomaly Detection

**Description**: For rotating machinery - detects deviations from expected periodic pattern.

**When to use**:
- **Turbines**: Blade-to-blade comparison
- **Motors**: Revolution-to-revolution analysis
- **Pumps**: Stroke-to-stroke monitoring
- Any cyclical process

**Example**:
```cpp
SignalProcessing sp;
// ... add rotation data ...

int anomaly_indices[1000];
int samples_per_rotation = 360;
double tolerance = 3.0;  // Tolerance in standard deviations

int num_anomalies = sp.DetectPeriodicAnomalies(samples_per_rotation, 
                                               tolerance, 
                                               anomaly_indices, 1000);

printf("Found %d periodic anomalies\n", num_anomalies);
```

---

## Signal Segmentation

### Turbine Blade Monitoring

**Scenario**: Monitor a turbine with 12 blades, detect which blade(s) have anomalous vibration.

```cpp
SignalProcessing sp;
const int NUM_BLADES = 12;
const int SAMPLES_PER_BLADE = 50;
int blade_markers[NUM_BLADES];

// Step 1: Collect data
for (int blade = 0; blade < NUM_BLADES; ++blade) {
    blade_markers[blade] = blade * SAMPLES_PER_BLADE;
    
    for (int sample = 0; sample < SAMPLES_PER_BLADE; ++sample) {
        double vibration = ReadVibrationSensor();
        sp.AddValue(vibration);
    }
}

// Step 2: Analyze each blade
SegmentStats blade_stats[NUM_BLADES];
int num_analyzed = sp.SegmentByMarkers(blade_markers, NUM_BLADES, blade_stats);

// Step 3: Print analysis
printf("Blade Analysis:\n");
printf("Blade | Mean   | Std Dev | RMS    | Max    | Status\n");
printf("------|--------|---------|--------|--------|-----------\n");

double global_mean_rms = 0.0;
for (int i = 0; i < num_analyzed; ++i) {
    global_mean_rms += blade_stats[i].rms;
}
global_mean_rms /= num_analyzed;

for (int i = 0; i < num_analyzed; ++i) {
    printf("%5d | %6.3f | %7.3f | %6.3f | %6.3f | ",
           i,
           blade_stats[i].mean,
           blade_stats[i].std_dev,
           blade_stats[i].rms,
           blade_stats[i].max_value);
    
    if (blade_stats[i].rms > global_mean_rms * 2.0) {
        printf("CRITICAL\n");
    } else if (blade_stats[i].rms > global_mean_rms * 1.5) {
        printf("WARNING\n");
    } else {
        printf("NORMAL\n");
    }
}

// Step 4: Find most anomalous blade
int problematic_blade = sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 2);
printf("\nMost anomalous blade: #%d\n", problematic_blade);
printf("RMS: %.4f (%.1f%% above average)\n", 
       blade_stats[problematic_blade].rms,
       (blade_stats[problematic_blade].rms / global_mean_rms - 1.0) * 100.0);
```

### SegmentStats Structure

Each segment provides comprehensive statistics:

```cpp
typedef struct SegmentStats {
    int start_index;        // Starting index of segment
    int end_index;          // Ending index of segment
    int segment_id;         // Segment identifier (e.g., blade number)
    double mean;            // Mean value in segment
    double std_dev;         // Standard deviation in segment
    double max_value;       // Maximum value in segment
    double min_value;       // Minimum value in segment
    double rms;             // Root mean square
    double peak_to_peak;    // Peak-to-peak amplitude
    double anomaly_score;   // Computed anomaly score
    int num_points;         // Number of data points in segment
} SegmentStats;
```

### Anomaly Score Methods

`FindMostAnomalousSegment()` supports multiple scoring methods:

```cpp
// Method 0: Z-Score based on mean deviation
int blade_zscore = sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 0);

// Method 1: Based on standard deviation (variability)
int blade_stddev = sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 1);

// Method 2: Based on RMS (recommended for vibration)
int blade_rms = sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 2);

// Method 3: Based on maximum absolute value
int blade_max = sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 3);
```

---

## Practical Examples

### Example 1: Real-Time Turbine Monitoring

```cpp
#include "SignalProcessing.h"

class TurbineMonitor {
private:
    SignalProcessing sp;
    const int NUM_BLADES = 12;
    const int SAMPLES_PER_BLADE = 50;
    int blade_markers[12];
    double warning_threshold;
    double critical_threshold;
    
public:
    TurbineMonitor(double warn_thresh = 1.5, double crit_thresh = 2.0) 
        : warning_threshold(warn_thresh), critical_threshold(crit_thresh) {
        for (int i = 0; i < NUM_BLADES; ++i) {
            blade_markers[i] = i * SAMPLES_PER_BLADE;
        }
    }
    
    void CollectRotationData() {
        sp.ClearVector();
        for (int blade = 0; blade < NUM_BLADES; ++blade) {
            for (int sample = 0; sample < SAMPLES_PER_BLADE; ++sample) {
                double vibration = ReadSensor();
                sp.AddValue(vibration);
            }
        }
    }
    
    void AnalyzeAndAlert() {
        SegmentStats stats[12];
        sp.SegmentByMarkers(blade_markers, NUM_BLADES, stats);
        
        // Calculate baseline
        double avg_rms = 0.0;
        for (int i = 0; i < NUM_BLADES; ++i) {
            avg_rms += stats[i].rms;
        }
        avg_rms /= NUM_BLADES;
        
        // Check each blade
        for (int i = 0; i < NUM_BLADES; ++i) {
            double ratio = stats[i].rms / avg_rms;
            
            if (ratio > critical_threshold) {
                TriggerCriticalAlert(i, stats[i]);
            } else if (ratio > warning_threshold) {
                TriggerWarningAlert(i, stats[i]);
            }
        }
    }
    
    void TriggerCriticalAlert(int blade, const SegmentStats& stats) {
        printf("🔴 CRITICAL: Blade #%d\n", blade);
        printf("   RMS: %.4f, Max: %.4f\n", stats.rms, stats.max_value);
        printf("   IMMEDIATE ACTION REQUIRED\n");
        // Log to database, send SMS/email, trigger shutdown, etc.
    }
    
    void TriggerWarningAlert(int blade, const SegmentStats& stats) {
        printf("🟡 WARNING: Blade #%d\n", blade);
        printf("   RMS: %.4f, Max: %.4f\n", stats.rms, stats.max_value);
        printf("   Schedule inspection\n");
        // Log to database, notify maintenance team
    }
};

int main() {
    TurbineMonitor monitor(1.5, 2.0);
    
    while (true) {
        monitor.CollectRotationData();
        monitor.AnalyzeAndAlert();
        Sleep(1000);  // Check every second
    }
}
```

### Example 2: Multi-Method Anomaly Detection

Combine multiple methods for robust detection:

```cpp
void ComprehensiveAnomalyCheck(SignalProcessing& sp) {
    int anomalies_zscore[1000];
    int anomalies_iqr[1000];
    int anomalies_mad[1000];
    
    // Method 1: Z-Score
    int count_z = sp.DetectAnomaliesZScore(3.0, anomalies_zscore, 1000);
    
    // Method 2: IQR
    int count_iqr = sp.DetectAnomaliesIQR(1.5, anomalies_iqr, 1000);
    
    // Method 3: MAD
    int count_mad = sp.DetectAnomaliesMAD(20, 3.0, anomalies_mad, 1000);
    
    // Find consensus (detected by multiple methods)
    printf("Detection Results:\n");
    printf("  Z-Score: %d anomalies\n", count_z);
    printf("  IQR:     %d anomalies\n", count_iqr);
    printf("  MAD:     %d anomalies\n", count_mad);
    
    // High-confidence anomalies (detected by at least 2 methods)
    // Implementation left as exercise
}
```

### Example 3: Trending and Degradation Tracking

```cpp
void TrackDegradation() {
    const int NUM_ROTATIONS = 100;
    const int NUM_BLADES = 12;
    const int SAMPLES_PER_BLADE = 50;
    
    int blade_markers[12];
    for (int i = 0; i < NUM_BLADES; ++i) {
        blade_markers[i] = i * SAMPLES_PER_BLADE;
    }
    
    double blade7_history[NUM_ROTATIONS];
    
    for (int rotation = 0; rotation < NUM_ROTATIONS; ++rotation) {
        SignalProcessing sp;
        
        // Collect one rotation
        for (int blade = 0; blade < NUM_BLADES; ++blade) {
            for (int sample = 0; sample < SAMPLES_PER_BLADE; ++sample) {
                sp.AddValue(ReadSensor());
            }
        }
        
        // Analyze
        SegmentStats stats[12];
        sp.SegmentByMarkers(blade_markers, NUM_BLADES, stats);
        
        // Track blade #7 over time
        blade7_history[rotation] = stats[7].rms;
        
        // Check for upward trend
        if (rotation >= 10) {
            double recent_avg = 0.0;
            for (int i = rotation - 10; i < rotation; ++i) {
                recent_avg += blade7_history[i];
            }
            recent_avg /= 10.0;
            
            if (blade7_history[rotation] > recent_avg * 1.2) {
                printf("⚠ Blade #7 showing degradation trend at rotation %d\n", 
                       rotation);
            }
        }
    }
}
```

---

## Best Practices

### 1. Choosing Detection Thresholds

**Start Conservative, Then Tune**:
1. Begin with strict thresholds (e.g., Z-Score = 4.0)
2. Collect baseline data during normal operation
3. Gradually reduce threshold until false positive rate is acceptable
4. Document chosen thresholds and rationale

### 2. Combining Methods

**Use multiple methods for critical applications**:
- Z-Score for global outliers
- MAD for local deviations
- Sudden Change for impacts
- Periodic for consistency checks

**Example decision logic**:
```cpp
bool IsCriticalAnomaly(int index) {
    bool zscore_detected = /* check zscore list */;
    bool mad_detected = /* check mad list */;
    bool sudden_detected = /* check sudden change list */;
    
    // Critical if detected by 2+ methods
    return (zscore_detected + mad_detected + sudden_detected) >= 2;
}
```

### 3. Data Preprocessing

**Always preprocess before anomaly detection**:

```cpp
// Step 1: Remove obvious noise
double denoised[1000];
sp.MedianFilter(5, denoised);

// Step 2: (Optional) Apply to new SP instance
SignalProcessing sp_clean;
sp_clean.InitVector(denoised, sp.GetIndex());

// Step 3: Detect anomalies on clean signal
int anomalies[1000];
sp_clean.DetectAnomaliesZScore(3.0, anomalies, 1000);
```

### 4. Validation and Testing

**Test with known scenarios**:
```cpp
void TestDetection() {
    SignalProcessing sp;
    
    // Generate normal data
    for (int i = 0; i < 100; ++i) {
        sp.AddValue(sin(i * 0.1) + small_noise());
    }
    
    // Insert known anomaly
    sp.AddValue(10.0);  // Large spike
    
    // Verify detection
    int anomalies[100];
    int count = sp.DetectAnomaliesZScore(3.0, anomalies, 100);
    
    assert(count > 0);  // Should detect the spike
    assert(anomalies[count-1] == 100);  // Should be at index 100
}
```

### 5. Performance Optimization

**For real-time systems**:
- Use fixed-size buffers (already implemented in library)
- Choose appropriate window sizes for MAD (smaller = faster)
- Consider detection frequency vs. sampling rate
- Cache segment statistics if analyzing repeatedly

### 6. Logging and Alerts

**Implement comprehensive logging**:
```cpp
void LogAnomaly(int index, double value, const char* method) {
    struct timespec ts = sp.GetTimestamp(index);
    
    fprintf(log_file, "%ld.%09ld,%d,%.6f,%s\n", 
            ts.tv_sec, ts.tv_nsec, index, value, method);
    
    // Also log to database for trend analysis
    LogToDatabase(ts, index, value, method);
}
```

### 7. Maintenance Recommendations

**Based on anomaly severity**:

| Severity | Criteria | Action |
|----------|----------|--------|
| NORMAL | RMS < 1.2× average | Continue operation |
| ELEVATED | RMS 1.2-1.5× average | Increase monitoring frequency |
| WARNING | RMS 1.5-2.0× average | Schedule maintenance |
| CRITICAL | RMS > 2.0× average | Immediate shutdown |

---

## Troubleshooting

### False Positives

**Problem**: Too many false alarms

**Solutions**:
1. Increase detection threshold
2. Use IQR instead of Z-Score (more robust)
3. Increase MAD window size
4. Apply denoising before detection
5. Require detection by multiple methods

### Missed Detections

**Problem**: Known anomalies not detected

**Solutions**:
1. Decrease detection threshold
2. Try different detection methods
3. Check if signal is properly preprocessed
4. Verify segment markers are correct
5. Review data quality and sampling rate

### Performance Issues

**Problem**: Detection too slow for real-time

**Solutions**:
1. Reduce window sizes
2. Detect less frequently (every N rotations)
3. Use simpler methods (Z-Score, Sudden Change)
4. Profile code to find bottlenecks

---

## Summary

This anomaly detection system provides:
- ✅ Multiple detection algorithms for different scenarios
- ✅ Segment-based analysis for component-level monitoring
- ✅ Robust statistics (mean, std dev, RMS, peak-to-peak)
- ✅ Real-time capability with fixed memory usage
- ✅ Industrial-grade reliability

Choose the right method for your application and always validate with real data!
