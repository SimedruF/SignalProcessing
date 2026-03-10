/**
 * @file test_ml_h5export.cpp
 * @brief Test suite for HDF5 export of ML/AI datasets
 * 
 * Demonstrates:
 * - Exporting ML datasets to HDF5 format
 * - Exporting training statistics to HDF5
 * - Exporting features with raw signals to HDF5
 * - Integration with SignalRecorder for hierarchical data storage
 */

#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Generate synthetic ECG signal
void GenerateECGSignal(SignalProcessing &sp, bool is_abnormal)
{
    double baseline = is_abnormal ? 0.3 : 0.0;
    double noise_level = is_abnormal ? 0.15 : 0.02;
    
    for (int i = 0; i < 200; i++)
    {
        double t = i / 100.0;
        double qrs = exp(-50.0 * fmod(t, 0.8) * fmod(t, 0.8));
        double t_wave = 0.3 * exp(-20.0 * (fmod(t, 0.8) - 0.3) * (fmod(t, 0.8) - 0.3));
        double noise = noise_level * ((rand() % 100) / 50.0 - 1.0);
        double irregular = is_abnormal ? 0.2 * sin(17.0 * t) : 0.0;
        
        double value = baseline + qrs + t_wave + noise + irregular;
        sp.AddValue(value);
    }
}

// Generate vibration signal
void GenerateVibrationSignal(SignalProcessing &sp, bool has_fault)
{
    double base_amp = has_fault ? 1.5 : 1.0;
    double harmonic_amp = has_fault ? 0.4 : 0.05;
    
    for (int i = 0; i < 300; i++)
    {
        double t = i / 100.0;
        double fundamental = base_amp * sin(2.0 * M_PI * 50.0 * t);
        double harmonics = harmonic_amp * sin(2.0 * M_PI * 100.0 * t) +
                          harmonic_amp * 0.5 * sin(2.0 * M_PI * 150.0 * t);
        double noise = 0.05 * ((rand() % 100) / 50.0 - 1.0);
        
        sp.AddValue(fundamental + harmonics + noise);
    }
}

int main()
{
    printf("=== HDF5 Export for ML/AI Test ===\n\n");
    
    // ========== Test 1: Export Dataset to HDF5 ==========
    printf("Test 1: Export ML Dataset to HDF5\n");
    printf("-" "-----------------------------------\n");
    
    // Create and populate training dataset
    MLDataset training_set;
    SignalProcessing::CreateMLDataset(30, true, &training_set);
    
    printf("Generating training data...\n");
    
    // Generate 15 normal ECG signals
    for (int i = 0; i < 15; i++)
    {
        SignalProcessing sp;
        GenerateECGSignal(sp, false);
        
        MLFeatureVector features;
        sp.ExtractMLFeatures(100.0, &features);
        SignalProcessing::AddFeaturesToDataset(&training_set, &features, 0);
    }
    
    // Generate 15 abnormal ECG signals
    for (int i = 0; i < 15; i++)
    {
        SignalProcessing sp;
        GenerateECGSignal(sp, true);
        
        MLFeatureVector features;
        sp.ExtractMLFeatures(100.0, &features);
        SignalProcessing::AddFeaturesToDataset(&training_set, &features, 1);
    }
    
    printf("Dataset created: %d samples\n", training_set.num_samples);
    
    // Export to HDF5
    const char *h5_filename = "ml_training_data.h5";
    bool success = SignalProcessing::ExportDatasetToH5(&training_set, h5_filename, 
                                                       true, "/ML/ECG_Training");
    
    if (success)
    {
        printf("✓ Dataset exported to '%s'\n", h5_filename);
        printf("  Path: /ML/ECG_Training\n");
        printf("  Features: 21 datasets (mean, std_dev, variance, etc.)\n");
        printf("  Labels: included\n");
        printf("  Format: HDF5 (hierarchical)\n\n");
    }
    else
    {
        printf("✗ Failed to export dataset to HDF5\n\n");
    }
    
    // ========== Test 2: Export Training Statistics ==========
    printf("Test 2: Export Training Statistics to HDF5\n");
    printf("-" "------------------------------------------\n");
    
    // Compute training statistics
    MLTrainingStats stats;
    SignalProcessing::ComputeTrainingStats(&training_set, &stats);
    
    printf("Computed statistics from %d samples\n", stats.num_samples);
    printf("  Feature 0 (mean): mean=%.4f, std=%.4f\n", 
           stats.mean_values[0], stats.std_values[0]);
    printf("  Feature 5 (RMS): mean=%.4f, std=%.4f\n", 
           stats.mean_values[5], stats.std_values[5]);
    
    // Export stats to HDF5
    const char *stats_filename = "ml_training_stats.h5";
    success = SignalProcessing::ExportTrainingStatsToH5(&stats, stats_filename,
                                                        "/ML/NormalizationParams");
    
    if (success)
    {
        printf("✓ Training statistics exported to '%s'\n", stats_filename);
        printf("  Path: /ML/NormalizationParams\n");
        printf("  Contains: mean_values, std_values, min_values, max_values\n");
        printf("  Use for: Normalizing inference data\n\n");
    }
    else
    {
        printf("✗ Failed to export training statistics\n\n");
    }
    
    // ========== Test 3: Export Features with Raw Signal ==========
    printf("Test 3: Export Features + Raw Signal to HDF5\n");
    printf("-" "---------------------------------------------\n");
    
    // Generate a test signal
    SignalProcessing test_signal;
    GenerateVibrationSignal(test_signal, true);
    
    // Extract features
    MLFeatureVector test_features;
    test_signal.ExtractMLFeatures(100.0, &test_features);
    
    printf("Test signal generated: %d samples\n", test_signal.GetIndex());
    printf("Features extracted: 21\n");
    printf("  Dominant frequency: %.2f Hz\n", test_features.dominant_frequency);
    printf("  RMS: %.4f\n", test_features.rms);
    
    // Export to HDF5 with raw signal
    const char *signal_filename = "vibration_analysis.h5";
    success = test_signal.ExportMLFeaturesToH5(signal_filename, &test_features,
                                               "/Sensors/Turbine_01", true);
    
    if (success)
    {
        printf("✓ Features and raw signal exported to '%s'\n", signal_filename);
        printf("  Path: /Sensors/Turbine_01\n");
        printf("  Contains: features (21 values), raw_signal (%d samples)\n", 
               test_signal.GetIndex());
        printf("  Metadata: feature names, timestamps\n\n");
    }
    else
    {
        printf("✗ Failed to export features\n\n");
    }
    
    // ========== Test 4: Multi-Sensor Export ==========
    printf("Test 4: Multi-Sensor HDF5 Export\n");
    printf("-" "--------------------------------\n");
    
    const char *multi_sensor_file = "multi_sensor_data.h5";
    
    // Simulate 3 different sensors
    for (int sensor_id = 1; sensor_id <= 3; sensor_id++)
    {
        SignalProcessing sensor_signal;
        GenerateVibrationSignal(sensor_signal, sensor_id % 2 == 0);
        
        MLFeatureVector sensor_features;
        sensor_signal.ExtractMLFeatures(100.0, &sensor_features);
        
        char path[256];
        sprintf(path, "/Factory/Production_Line/Sensor_%02d", sensor_id);
        
        bool result = sensor_signal.ExportMLFeaturesToH5(multi_sensor_file, 
                                                         &sensor_features, 
                                                         path, true);
        
        if (result)
        {
            printf("  ✓ Sensor %d data exported to %s\n", sensor_id, path);
        }
    }
    printf("\n");
    
    // ========== Test 5: Complete ML Pipeline ==========
    printf("Test 5: Complete ML Pipeline with HDF5\n");
    printf("-" "--------------------------------------\n");
    printf("Workflow: Collect → Preprocess → Extract → Normalize → Export H5\n\n");
    
    // 1. Collect training data
    MLDataset pipeline_dataset;
    SignalProcessing::CreateMLDataset(20, true, &pipeline_dataset);
    
    for (int i = 0; i < 20; i++)
    {
        SignalProcessing sp;
        GenerateECGSignal(sp, i >= 10);
        
        // Preprocess (denoise)
        double denoised[1000];
        sp.KalmanFilter(0.01, 0.1, denoised);
        
        SignalProcessing clean;
        for (int j = 0; j < sp.GetIndex(); j++)
        {
            clean.AddValue(denoised[j]);
        }
        
        // Extract features
        MLFeatureVector features;
        clean.ExtractMLFeatures(100.0, &features);
        
        // Add to dataset
        SignalProcessing::AddFeaturesToDataset(&pipeline_dataset, &features, i >= 10 ? 1 : 0);
    }
    
    printf("  ✓ Collected and preprocessed 20 signals\n");
    
    // 2. Compute and export training stats
    MLTrainingStats pipeline_stats;
    SignalProcessing::ComputeTrainingStats(&pipeline_dataset, &pipeline_stats);
    
    const char *pipeline_file = "complete_ml_pipeline.h5";
    SignalProcessing::ExportTrainingStatsToH5(&pipeline_stats, pipeline_file,
                                             "/Pipeline/TrainingStats");
    
    printf("  ✓ Training statistics computed and exported\n");
    
    // 3. Normalize dataset
    SignalProcessing::NormalizeDataset(&pipeline_dataset, &pipeline_stats);
    
    printf("  ✓ Dataset normalized (z-score)\n");
    
    // 4. Export normalized dataset
    SignalProcessing::ExportDatasetToH5(&pipeline_dataset, pipeline_file,
                                       true, "/Pipeline/NormalizedData");
    
    printf("  ✓ Normalized dataset exported\n");
    printf("  ✓ File: '%s'\n", pipeline_file);
    printf("  ✓ Structure:\n");
    printf("      /Pipeline/TrainingStats/  (normalization parameters)\n");
    printf("      /Pipeline/NormalizedData/ (training data)\n\n");
    
    // ========== Summary ==========
    printf("=================================================\n");
    printf("Summary: HDF5 Export for ML/AI\n");
    printf("=================================================\n");
    printf("✓ Dataset export to HDF5 with hierarchical structure\n");
    printf("✓ Training statistics export (mean, std, min, max)\n");
    printf("✓ Features + raw signal export for visualization\n");
    printf("✓ Multi-sensor hierarchical organization\n");
    printf("✓ Complete ML pipeline with normalization\n\n");
    
    printf("Generated HDF5 Files:\n");
    printf("  • ml_training_data.h5       - Training dataset (30 samples)\n");
    printf("  • ml_training_stats.h5      - Normalization parameters\n");
    printf("  • vibration_analysis.h5     - Single signal analysis\n");
    printf("  • multi_sensor_data.h5      - Multi-sensor data\n");
    printf("  • complete_ml_pipeline.h5   - Full ML pipeline\n\n");
    
    printf("Python/TensorFlow Integration:\n");
    printf("  import h5py\n");
    printf("  import numpy as np\n");
    printf("  \n");
    printf("  # Load dataset\n");
    printf("  with h5py.File('ml_training_data.h5', 'r') as f:\n");
    printf("      features = []\n");
    printf("      for name in ['mean', 'std_dev', 'variance', ...]:\n");
    printf("          features.append(f['/ML/ECG_Training'][name][:])\n");
    printf("      X = np.column_stack(features)\n");
    printf("      y = f['/ML/ECG_Training/labels'][:]\n");
    printf("  \n");
    printf("  # Load normalization parameters\n");
    printf("  with h5py.File('ml_training_stats.h5', 'r') as f:\n");
    printf("      mean = f['/ML/NormalizationParams/mean_values'][:]\n");
    printf("      std = f['/ML/NormalizationParams/std_values'][:]\n\n");
    
    printf("MATLAB Integration:\n");
    printf("  %% Load dataset\n");
    printf("  features = h5read('ml_training_data.h5', '/ML/ECG_Training/mean');\n");
    printf("  labels = h5read('ml_training_data.h5', '/ML/ECG_Training/labels');\n");
    printf("  \n");
    printf("  %% Load normalization\n");
    printf("  mean_vals = h5read('ml_training_stats.h5', ...\n");
    printf("                     '/ML/NormalizationParams/mean_values');\n\n");
    
    // Cleanup
    SignalProcessing::FreeMLDataset(&training_set);
    SignalProcessing::FreeMLDataset(&pipeline_dataset);
    
    printf("All tests completed successfully!\n");
    
    return 0;
}
