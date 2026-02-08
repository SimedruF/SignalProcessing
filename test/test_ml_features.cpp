#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     ML/AI Feature Extraction Test                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // === TEST 1: ECG-like signal ===
    printf("═══ TEST 1: ECG Signal Feature Extraction ═══\n");
    SignalProcessing sp_ecg;
    double sampling_rate = 100.0;  // 100 Hz
    int num_samples = 500;
    
    // Simulate ECG signal: QRS complex + noise
    for (int i = 0; i < num_samples; i++) {
        double t = (double)i / sampling_rate;
        double ecg = 0.0;
        
        // P-wave + QRS + T-wave (simplified ECG)
        double phase = fmod(t * 1.2 * 2 * M_PI, 2 * M_PI);  // 1.2 Hz heart rate
        
        if (phase < M_PI / 4) {
            ecg = 0.3 * sin(phase * 4);  // P-wave
        } else if (phase > M_PI / 2 && phase < M_PI * 0.7) {
            ecg = 1.5 * sin((phase - M_PI/2) * 10);  // QRS complex
        } else if (phase > M_PI && phase < M_PI * 1.5) {
            ecg = 0.4 * sin((phase - M_PI) * 2);  // T-wave
        }
        
        // Add small noise
        ecg += ((double)rand() / RAND_MAX - 0.5) * 0.1;
        
        sp_ecg.AddValue(ecg);
    }
    
    MLFeatureVector ecg_features;
    bool success = sp_ecg.ExtractMLFeatures(sampling_rate, &ecg_features);
    
    if (success) {
        printf("\n✓ Feature extraction successful!\n\n");
        
        printf("Statistical Features:\n");
        printf("  Mean:              %.4f\n", ecg_features.mean);
        printf("  Std Deviation:     %.4f\n", ecg_features.std_dev);
        printf("  Variance:          %.4f\n", ecg_features.variance);
        printf("  Skewness:          %.4f\n", ecg_features.skewness);
        printf("  Kurtosis:          %.4f\n", ecg_features.kurtosis);
        printf("  RMS:               %.4f\n", ecg_features.rms);
        printf("  Peak-to-Peak:      %.4f\n", ecg_features.peak_to_peak);
        printf("  Crest Factor:      %.4f\n\n", ecg_features.crest_factor);
        
        printf("Frequency Features:\n");
        printf("  Dominant Freq:     %.2f Hz\n", ecg_features.dominant_frequency);
        printf("  Spectral Centroid: %.2f Hz\n", ecg_features.spectral_centroid);
        printf("  Spectral Spread:   %.2f Hz\n", ecg_features.spectral_spread);
        printf("  Spectral Entropy:  %.4f\n", ecg_features.spectral_entropy);
        printf("  Total Power:       %.2f\n", ecg_features.total_power);
        printf("  Power (0-10 Hz):   %.2f\n", ecg_features.power_low_freq);
        printf("  Power (10-100 Hz): %.2f\n", ecg_features.power_mid_freq);
        printf("  Power (100+ Hz):   %.2f\n\n", ecg_features.power_high_freq);
        
        printf("Time Domain Features:\n");
        printf("  Zero-Cross Rate:   %.4f\n", ecg_features.zero_crossing_rate);
        printf("  Energy:            %.2f\n", ecg_features.energy);
        printf("  Autocorr Peak:     %.4f s\n", ecg_features.autocorr_peak);
        printf("  Total Features:    %d\n\n", ecg_features.num_features);
    }
    
    // === TEST 2: Export to array (for ML libraries) ===
    printf("═══ TEST 2: Export to ML-Ready Array ═══\n");
    double feature_array[21];
    int num_exported = sp_ecg.ExportFeaturesToArray(&ecg_features, feature_array);
    
    printf("Exported %d features to flat array:\n", num_exported);
    printf("[ ");
    for (int i = 0; i < num_exported; i++) {
        printf("%.3f ", feature_array[i]);
        if ((i + 1) % 7 == 0 && i < num_exported - 1) {
            printf("\n  ");
        }
    }
    printf("]\n\n");
    
    // === TEST 3: Turbine vibration features ===
    printf("═══ TEST 3: Turbine Vibration Features ═══\n");
    SignalProcessing sp_turbine;
    
    // Simulate turbine with imbalance (low freq) + bearing wear (high freq)
    for (int i = 0; i < 300; i++) {
        double t = (double)i / 1000.0;  // 1 kHz sampling
        double vibration = 0.0;
        
        // Normal rotation: 50 Hz (3000 RPM)
        vibration += 1.0 * sin(2 * M_PI * 50 * t);
        
        // Imbalance: 1x rotation frequency
        vibration += 0.3 * sin(2 * M_PI * 50 * t + M_PI/4);
        
        // Bearing wear: high frequency components
        vibration += 0.2 * sin(2 * M_PI * 500 * t);
        vibration += 0.15 * sin(2 * M_PI * 750 * t);
        
        // Random noise
        vibration += ((double)rand() / RAND_MAX - 0.5) * 0.1;
        
        sp_turbine.AddValue(vibration);
    }
    
    MLFeatureVector turbine_features;
    success = sp_turbine.ExtractMLFeatures(1000.0, &turbine_features);
    
    if (success) {
        printf("✓ Turbine features extracted\n\n");
        printf("Key indicators:\n");
        printf("  RMS (overall):     %.4f\n", turbine_features.rms);
        printf("  Dominant Freq:     %.2f Hz (rotation speed)\n", turbine_features.dominant_frequency);
        printf("  Low Freq Power:    %.2f (imbalance)\n", turbine_features.power_low_freq);
        printf("  High Freq Power:   %.2f (bearing wear)\n", turbine_features.power_high_freq);
        printf("  Crest Factor:      %.4f (spikes)\n\n", turbine_features.crest_factor);
    }
    
    // === TEST 4: Segment-based feature extraction ===
    printf("═══ TEST 4: Segment-Based Features (Per Blade) ═══\n");
    
    // Extract features for first 100 samples (first blade pass)
    MLFeatureVector blade1_features;
    success = sp_turbine.ExtractMLFeaturesFromSegment(0, 100, 1000.0, &blade1_features);
    
    if (success) {
        printf("Blade 1 RMS: %.4f\n", blade1_features.rms);
    }
    
    // Extract features for samples 100-200 (second blade)
    MLFeatureVector blade2_features;
    success = sp_turbine.ExtractMLFeaturesFromSegment(100, 100, 1000.0, &blade2_features);
    
    if (success) {
        printf("Blade 2 RMS: %.4f\n", blade2_features.rms);
    }
    
    // Compare blades
    double rms_diff = fabs(blade1_features.rms - blade2_features.rms);
    printf("\nRMS Difference: %.4f ", rms_diff);
    if (rms_diff > 0.1) {
        printf("⚠ Possible imbalance detected!\n");
    } else {
        printf("✓ Balanced\n");
    }
    
    // === TEST 5: Feature normalization ===
    printf("\n═══ TEST 5: Feature Normalization (for Neural Networks) ═══\n");
    
    // Mock training dataset statistics (normally computed from training data)
    double mean_values[21] = {
        0.1, 0.5, 0.25, 0.0, 3.0, 0.7, 2.0, 1.5,  // Statistical
        50.0, 100.0, 50.0, 2.0, 100.0, 20.0, 60.0, 20.0,  // Frequency
        0.3, 150.0, 0.02  // Time domain
    };
    
    double std_values[21] = {
        0.05, 0.2, 0.1, 0.5, 1.0, 0.3, 1.0, 0.5,  // Statistical
        10.0, 50.0, 25.0, 0.5, 50.0, 10.0, 30.0, 10.0,  // Frequency
        0.1, 50.0, 0.01  // Time domain
    };
    
    // Create a copy for comparison
    MLFeatureVector original_features = ecg_features;
    
    printf("Before normalization:\n");
    printf("  Mean: %.4f, RMS: %.4f, Dominant Freq: %.2f Hz\n",
           original_features.mean, original_features.rms, original_features.dominant_frequency);
    
    sp_ecg.NormalizeMLFeatures(&ecg_features, mean_values, std_values);
    
    printf("After normalization (z-score):\n");
    printf("  Mean: %.4f, RMS: %.4f, Dominant Freq: %.4f\n",
           ecg_features.mean, ecg_features.rms, ecg_features.dominant_frequency);
    printf("✓ Features ready for neural network input\n\n");
    
    // === SUMMARY ===
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Use Case: Feed these features to:                        ║\n");
    printf("║  • Neural Networks (TensorFlow, PyTorch)                  ║\n");
    printf("║  • SVM classifiers (scikit-learn)                         ║\n");
    printf("║  • Random Forest / Decision Trees                         ║\n");
    printf("║  • Any ML model expecting numerical feature vectors       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}
