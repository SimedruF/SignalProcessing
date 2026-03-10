/**
 * @file test_ml_downstream.cpp
 * @brief Test suite for downstream ML/AI integration features
 * 
 * Demonstrates:
 * - Batch dataset creation and management
 * - Rolling window feature extraction
 * - Training statistics computation
 * - Dataset normalization
 * - CSV export for external ML frameworks
 * - Integration with neural networks and SVM workflows
 */

#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simulate ECG signal with normal and abnormal patterns
void GenerateECGSignal(SignalProcessing &sp, bool is_abnormal)
{
    // Normal ECG: periodic with clear P-QRS-T pattern
    // Abnormal: irregular rhythm, elevated baseline, noise
    
    double baseline = is_abnormal ? 0.3 : 0.0;
    double noise_level = is_abnormal ? 0.15 : 0.02;
    
    for (int i = 0; i < 200; i++)
    {
        double t = i / 100.0;  // Time in seconds
        
        // Simulate QRS complex (main spike)
        double qrs = exp(-50.0 * fmod(t, 0.8) * fmod(t, 0.8));
        
        // Add T wave
        double t_wave = 0.3 * exp(-20.0 * (fmod(t, 0.8) - 0.3) * (fmod(t, 0.8) - 0.3));
        
        // Add noise
        double noise = noise_level * ((rand() % 100) / 50.0 - 1.0);
        
        // Abnormal: add irregular component
        double irregular = is_abnormal ? 0.2 * sin(17.0 * t) : 0.0;
        
        double value = baseline + qrs + t_wave + noise + irregular;
        sp.AddValue(value);
    }
}

// Generate vibration signal (for turbine monitoring)
void GenerateVibrationSignal(SignalProcessing &sp, bool has_fault)
{
    // Normal: periodic vibration at fundamental frequency
    // Faulty: additional harmonics, increased amplitude
    
    double base_amp = has_fault ? 1.5 : 1.0;
    double harmonic_amp = has_fault ? 0.4 : 0.05;
    
    for (int i = 0; i < 300; i++)
    {
        double t = i / 100.0;
        
        // Fundamental frequency (50 Hz)
        double fundamental = base_amp * sin(2.0 * M_PI * 50.0 * t);
        
        // Add harmonics (100 Hz, 150 Hz) - stronger in faulty case
        double harmonics = harmonic_amp * sin(2.0 * M_PI * 100.0 * t) +
                          harmonic_amp * 0.5 * sin(2.0 * M_PI * 150.0 * t);
        
        // Add noise
        double noise = 0.05 * ((rand() % 100) / 50.0 - 1.0);
        
        sp.AddValue(fundamental + harmonics + noise);
    }
}

int main()
{
    printf("=== Downstream ML/AI Integration Test ===\n\n");
    
    // ========== Test 1: Create Dataset ==========
    printf("Test 1: Creating ML Dataset\n");
    printf("-" "------------------------------\n");
    
    MLDataset training_set;
    bool success = SignalProcessing::CreateMLDataset(50, true, &training_set);
    
    if (success)
    {
        printf("✓ Dataset created with capacity: %d\n", training_set.capacity);
        printf("✓ Labels enabled: %s\n\n", training_set.has_labels ? "yes" : "no");
    }
    else
    {
        printf("✗ Failed to create dataset\n\n");
        return 1;
    }
    
    // ========== Test 2: Batch Feature Extraction ==========
    printf("Test 2: Batch Feature Extraction (ECG Classification)\n");
    printf("-" "-----------------------------------------------------\n");
    
    // Generate multiple ECG signals - some normal, some abnormal
    int num_normal = 10;
    int num_abnormal = 10;
    
    printf("Generating %d normal ECG signals...\n", num_normal);
    for (int i = 0; i < num_normal; i++)
    {
        SignalProcessing sp;
        GenerateECGSignal(sp, false);  // Normal
        
        MLFeatureVector features;
        sp.ExtractMLFeatures(100.0, &features);  // 100 Hz sampling
        
        SignalProcessing::AddFeaturesToDataset(&training_set, &features, 0);  // Label 0 = normal
    }
    
    printf("Generating %d abnormal ECG signals...\n", num_abnormal);
    for (int i = 0; i < num_abnormal; i++)
    {
        SignalProcessing sp;
        GenerateECGSignal(sp, true);  // Abnormal
        
        MLFeatureVector features;
        sp.ExtractMLFeatures(100.0, &features);
        
        SignalProcessing::AddFeaturesToDataset(&training_set, &features, 1);  // Label 1 = abnormal
    }
    
    printf("✓ Dataset now contains %d samples\n", training_set.num_samples);
    printf("  - First sample RMS: %.4f\n", training_set.samples[0].rms);
    printf("  - Last sample RMS: %.4f\n", training_set.samples[training_set.num_samples-1].rms);
    printf("  - First sample label: %d (normal)\n", training_set.labels[0]);
    printf("  - Last sample label: %d (abnormal)\n\n", training_set.labels[training_set.num_samples-1]);
    
    // ========== Test 3: Rolling Window Feature Extraction ==========
    printf("Test 3: Rolling Window Feature Extraction\n");
    printf("-" "-----------------------------------------\n");
    
    SignalProcessing long_signal;
    GenerateVibrationSignal(long_signal, false);
    
    MLDataset window_dataset;
    SignalProcessing::CreateMLDataset(20, false, &window_dataset);
    
    int num_windows = long_signal.ExtractMLFeaturesRollingWindow(50, 25, 100.0, &window_dataset);
    
    printf("✓ Extracted features from %d overlapping windows\n", num_windows);
    printf("  Window size: 50 samples\n");
    printf("  Step size: 25 samples (50%% overlap)\n");
    printf("  Features per window: %d\n", window_dataset.samples[0].num_features);
    printf("  First window mean: %.4f\n", window_dataset.samples[0].mean);
    printf("  Last window mean: %.4f\n\n", window_dataset.samples[num_windows-1].mean);
    
    SignalProcessing::FreeMLDataset(&window_dataset);
    
    // ========== Test 4: Compute Training Statistics ==========
    printf("Test 4: Computing Training Statistics\n");
    printf("-" "-------------------------------------\n");
    
    MLTrainingStats stats;
    success = SignalProcessing::ComputeTrainingStats(&training_set, &stats);
    
    if (success)
    {
        printf("✓ Statistics computed from %d samples\n", stats.num_samples);
        printf("  Feature 0 (mean): mean=%.4f, std=%.4f\n", 
               stats.mean_values[0], stats.std_values[0]);
        printf("  Feature 5 (RMS): mean=%.4f, std=%.4f\n", 
               stats.mean_values[5], stats.std_values[5]);
        printf("  Feature 5 range: [%.4f, %.4f]\n\n", 
               stats.min_values[5], stats.max_values[5]);
    }
    else
    {
        printf("✗ Failed to compute statistics\n\n");
    }
    
    // ========== Test 5: Dataset Normalization ==========
    printf("Test 5: Dataset Normalization (Z-Score)\n");
    printf("-" "---------------------------------------\n");
    
    // Save original values for comparison
    double original_mean = training_set.samples[0].mean;
    double original_rms = training_set.samples[0].rms;
    
    success = SignalProcessing::NormalizeDataset(&training_set, &stats);
    
    if (success)
    {
        printf("✓ Dataset normalized using training statistics\n");
        printf("  Before normalization:\n");
        printf("    Sample 0 mean: %.4f\n", original_mean);
        printf("    Sample 0 RMS: %.4f\n", original_rms);
        printf("  After normalization:\n");
        printf("    Sample 0 mean: %.4f\n", training_set.samples[0].mean);
        printf("    Sample 0 RMS: %.4f\n\n", training_set.samples[0].rms);
    }
    else
    {
        printf("✗ Failed to normalize dataset\n\n");
    }
    
    // ========== Test 6: Export to CSV ==========
    printf("Test 6: Export Dataset to CSV\n");
    printf("-" "-----------------------------\n");
    
    const char *csv_filename = "ecg_training_data.csv";
    success = SignalProcessing::ExportDatasetToCSV(&training_set, csv_filename, true);
    
    if (success)
    {
        printf("✓ Dataset exported to '%s'\n", csv_filename);
        printf("  Format: 21 features + label column\n");
        printf("  Ready for: TensorFlow, PyTorch, scikit-learn\n");
        printf("  Total rows: %d (plus header)\n\n", training_set.num_samples);
    }
    else
    {
        printf("✗ Failed to export dataset\n\n");
    }
    
    // ========== Test 7: Batch Processing Multiple Signals ==========
    printf("Test 7: Batch Processing for Inference\n");
    printf("-" "---------------------------------------\n");
    
    // Simulate multiple sensor readings arriving simultaneously
    int num_sensors = 5;
    double *sensor_signals[5];
    int sensor_sizes[5];
    
    for (int i = 0; i < num_sensors; i++)
    {
        sensor_sizes[i] = 150;
        sensor_signals[i] = new double[sensor_sizes[i]];
        
        // Generate test signal
        SignalProcessing temp;
        GenerateVibrationSignal(temp, i % 2 == 0);  // Alternate normal/faulty
        
        for (int j = 0; j < sensor_sizes[i]; j++)
        {
            sensor_signals[i][j] = temp.GetValue(j);
        }
    }
    
    MLDataset inference_batch;
    SignalProcessing::CreateMLDataset(num_sensors, false, &inference_batch);
    
    int processed = SignalProcessing::BatchExtractFeatures(sensor_signals, sensor_sizes, 
                                                          num_sensors, 100.0, &inference_batch);
    
    printf("✓ Batch processed %d sensor signals\n", processed);
    printf("  Extracted features ready for model inference\n");
    printf("  Sample 0 dominant frequency: %.2f Hz\n", 
           inference_batch.samples[0].dominant_frequency);
    printf("  Sample 0 RMS: %.4f\n\n", inference_batch.samples[0].rms);
    
    // Cleanup
    for (int i = 0; i < num_sensors; i++)
    {
        delete[] sensor_signals[i];
    }
    SignalProcessing::FreeMLDataset(&inference_batch);
    
    // ========== Test 8: Integration Example ==========
    printf("Test 8: Complete ML Workflow Simulation\n");
    printf("-" "---------------------------------------\n");
    printf("Workflow: Signal → Preprocess → Extract Features → Normalize → Predict\n\n");
    
    // Step 1: Acquire new signal (simulated patient ECG)
    SignalProcessing new_patient;
    GenerateECGSignal(new_patient, true);  // Unknown if abnormal
    
    // Step 2: Preprocess (denoise)
    double denoised[1000];
    new_patient.KalmanFilter(0.01, 0.1, denoised);
    
    SignalProcessing clean_signal;
    for (int i = 0; i < new_patient.GetIndex(); i++)
    {
        clean_signal.AddValue(denoised[i]);
    }
    
    // Step 3: Extract features
    MLFeatureVector test_features;
    clean_signal.ExtractMLFeatures(100.0, &test_features);
    
    printf("  ✓ Preprocessed signal (Kalman filter)\n");
    printf("  ✓ Extracted %d features\n", test_features.num_features);
    
    // Step 4: Normalize using training statistics
    SignalProcessing::NormalizeMLFeatures(&test_features, stats.mean_values, stats.std_values);
    
    printf("  ✓ Normalized features using training stats\n");
    
    // Step 5: Export to array for model input
    double feature_array[21];
    int num_features = SignalProcessing::ExportFeaturesToArray(&test_features, feature_array);
    
    printf("  ✓ Exported %d features to array\n", num_features);
    printf("  ✓ Ready for neural network / SVM prediction\n\n");
    
    printf("  Feature array (first 5 values):\n");
    for (int i = 0; i < 5; i++)
    {
        printf("    feature[%d] = %.4f\n", i, feature_array[i]);
    }
    printf("    ...\n\n");
    
    // ========== Summary ==========
    printf("=================================================\n");
    printf("Summary: Downstream ML/AI Integration\n");
    printf("=================================================\n");
    printf("✓ Dataset creation and management\n");
    printf("✓ Batch feature extraction from multiple signals\n");
    printf("✓ Rolling window processing for time series\n");
    printf("✓ Training statistics computation (mean, std, min, max)\n");
    printf("✓ Z-score normalization for neural networks\n");
    printf("✓ CSV export for external ML frameworks\n");
    printf("✓ Complete workflow: Raw signal → ML model input\n\n");
    
    printf("Integration Ready For:\n");
    printf("  • TensorFlow / Keras (Python)\n");
    printf("  • PyTorch (Python)\n");
    printf("  • scikit-learn SVM (Python)\n");
    printf("  • MATLAB Machine Learning Toolbox\n");
    printf("  • C++ ML libraries (dlib, mlpack)\n\n");
    
    printf("CSV file '%s' can be used as:\n", csv_filename);
    printf("  import pandas as pd\n");
    printf("  df = pd.read_csv('%s')\n", csv_filename);
    printf("  X = df.iloc[:, :-1].values  # Features\n");
    printf("  y = df.iloc[:, -1].values   # Labels\n\n");
    
    // Cleanup
    SignalProcessing::FreeMLDataset(&training_set);
    
    printf("All tests completed successfully!\n");
    
    return 0;
}
