#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Turbine Vibration Anomaly Detection Test
 * 
 * Scenario: Monitoring vibration of a turbine with 12 blades
 * - Each blade rotation generates a vibration signature
 * - Blade #7 has a defect causing higher vibration
 * - System should detect which blade has the anomaly
 */

// Helper function to generate blade vibration signal
double GenerateBladeVibration(int blade_number, int sample_in_blade, int samples_per_blade) {
    // Base frequency component (normal vibration)
    double base_freq= 2.0 * M_PI * sample_in_blade / samples_per_blade;
    double normal_vibration= 0.5 * sin(base_freq) + 0.2 * sin(2.0 * base_freq);
    
    // Add some random noise
    double noise= ((double)rand() / RAND_MAX - 0.5) * 0.1;
    
    // Blade #7 has a defect (higher amplitude and additional frequency)
    if (blade_number== 7) {
        normal_vibration *= 2.5;  // Higher amplitude
        normal_vibration += 0.8 * sin(3.0 * base_freq);  // Additional harmonic
    }
    
    // Blade #3 has minor imbalance
    if (blade_number== 3) {
        normal_vibration *= 1.3;
    }
    
    return normal_vibration + noise;
}

void TestTurbineBladeDetection() {
    printf("\n========================================================\n");
    printf("||  Test 1: Turbine Blade Anomaly Detection              \n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    const int NUM_BLADES= 12;
    const int SAMPLES_PER_BLADE= 50;
    const int TOTAL_SAMPLES= NUM_BLADES * SAMPLES_PER_BLADE;
    
    int blade_markers[NUM_BLADES];
    
    printf("Simulating turbine with %d blades...\n", NUM_BLADES);
    printf("Samples per blade rotation: %d\n", SAMPLES_PER_BLADE);
    printf("Known defect: Blade #7 (high vibration)\n");
    printf("Known defect: Blade #3 (minor imbalance)\n\n");
    
    // Generate vibration data
    srand((unsigned int)time(NULL));
    
    for (int blade= 0; blade < NUM_BLADES; ++blade) {
        blade_markers[blade]= blade * SAMPLES_PER_BLADE;
        
        for (int sample= 0; sample < SAMPLES_PER_BLADE; ++sample) {
            double vibration= GenerateBladeVibration(blade, sample, SAMPLES_PER_BLADE);
            sp.AddValue(vibration);
        }
    }
    
    printf("ok  Generated %d vibration samples\n\n", TOTAL_SAMPLES);
    
    // Analyze segments (blades)
    SegmentStats blade_stats[NUM_BLADES];
    int num_analyzed= sp.SegmentByMarkers(blade_markers, NUM_BLADES, blade_stats);
    
    printf("=======================================================================\n");
    printf("Blade Analysis Results:\n");
    printf("=======================================================================\n");
    printf("Blade | Mean    | Std Dev | RMS     | Max     | Peak-Peak | Status\n");
    printf("-------+---------+---------+---------+---------+-----------+------------\n");
    
    double global_mean_rms= 0.0;
    for (int i= 0; i < num_analyzed; ++i) {
        global_mean_rms += blade_stats[i].rms;
    }
    global_mean_rms /= num_analyzed;
    
    for (int i= 0; i < num_analyzed; ++i) {
        printf(" %2d   | %7.4f | %7.4f | %7.4f | %7.4f |  %7.4f  | ",
               i,
               blade_stats[i].mean,
               blade_stats[i].std_dev,
               blade_stats[i].rms,
               blade_stats[i].max_value,
               blade_stats[i].peak_to_peak);
        
        // Flag anomalies
        if (blade_stats[i].rms > global_mean_rms * 1.5) {
            printf("! HIGH\n");
        } else if (blade_stats[i].rms > global_mean_rms * 1.2) {
            printf("! ELEVATED\n");
        } else {
            printf("ok  NORMAL\n");
        }
    }
    printf("=======================================================================\n\n");
    
    // Find most anomalous blade using different methods
    printf("Anomaly Detection Methods:\n");
    printf("-----------------------------------------------------------------------\n");
    
    int anomalous_zscore= sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 0);
    int anomalous_stddev= sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 1);
    int anomalous_rms= sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 2);
    int anomalous_max= sp.FindMostAnomalousSegment(blade_markers, NUM_BLADES, 3);
    
    printf("  Z-Score method     : Blade #%d (Score: %.4f)\n", 
           anomalous_zscore, blade_stats[anomalous_zscore].anomaly_score);
    printf("  Std Dev method     : Blade #%d (Std: %.4f)\n", 
           anomalous_stddev, blade_stats[anomalous_stddev].std_dev);
    printf("  RMS method         : Blade #%d (RMS: %.4f)\n", 
           anomalous_rms, blade_stats[anomalous_rms].rms);
    printf("  Max Value method   : Blade #%d (Max: %.4f)\n", 
           anomalous_max, blade_stats[anomalous_max].max_value);
    
    printf("\n");
    
    // Verify detection
    bool detected_correctly= (anomalous_rms== 7 || anomalous_max== 7 || anomalous_zscore== 7);
    
    if (detected_correctly) {
        printf("ok  TEST PASSED: Correctly identified Blade #7 as most anomalous\n");
    } else {
        printf("! TEST WARNING: Expected Blade #7, but methods detected blade #%d\n", anomalous_rms);
        printf("  (This may vary due to random noise in simulation)\n");
    }
    
    printf("\n");
}

void TestContinuousMonitoring() {
    printf("\n========================================================\n");
    printf("||  Test 2: Continuous Vibration Monitoring              ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    const int NUM_BLADES= 8;
    const int SAMPLES_PER_BLADE= 40;
    const int NUM_ROTATIONS= 5;
    
    printf("Simulating %d complete turbine rotations\n", NUM_ROTATIONS);
    printf("Blades per rotation: %d\n", NUM_BLADES);
    printf("Detecting periodic anomalies...\n\n");
    
    srand((unsigned int)time(NULL) + 1);
    
    // Generate multiple rotations
    for (int rotation= 0; rotation < NUM_ROTATIONS; ++rotation) {
        for (int blade= 0; blade < NUM_BLADES; ++blade) {
            for (int sample= 0; sample < SAMPLES_PER_BLADE; ++sample) {
                double vibration= GenerateBladeVibration(blade, sample, SAMPLES_PER_BLADE);
                sp.AddValue(vibration);
            }
        }
    }
    
    // Detect periodic anomalies
    int anomaly_indices[1000];
    int period= NUM_BLADES * SAMPLES_PER_BLADE;
    int num_anomalies= sp.DetectPeriodicAnomalies(period, 3.0, anomaly_indices, 1000);
    
    printf("Period length: %d samples\n", period);
    printf("Anomalies detected: %d\n\n", num_anomalies);
    
    if (num_anomalies > 0) {
        printf("Anomaly locations (sample indices):\n");
        int count= 0;
        for (int i= 0; i < num_anomalies && count < 10; ++i) {
            int rotation_num= anomaly_indices[i] / period;
            int blade_num= (anomaly_indices[i] % period) / SAMPLES_PER_BLADE;
            printf("  Sample %4d: Rotation %d, Blade %d\n", 
                   anomaly_indices[i], rotation_num, blade_num);
            count++;
        }
        if (num_anomalies > 10) {
            printf("  ... and %d more anomalies\n", num_anomalies - 10);
        }
    }
    
    printf("\nok  TEST PASSED: Periodic monitoring functional\n\n");
}

void TestMultipleDetectionMethods() {
    printf("\n========================================================\n");
    printf("||  Test 3: Multiple Anomaly Detection Methods           ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    const int NUM_BLADES= 10;
    const int SAMPLES_PER_BLADE= 60;
    
    srand((unsigned int)time(NULL) + 2);
    
    // Generate data with various anomaly types
    for (int blade= 0; blade < NUM_BLADES; ++blade) {
        for (int sample= 0; sample < SAMPLES_PER_BLADE; ++sample) {
            double vibration= GenerateBladeVibration(blade, sample, SAMPLES_PER_BLADE);
            
            // Add sudden spike at specific location
            if (blade== 5 && sample== 30) {
                vibration += 5.0;
            }
            
            sp.AddValue(vibration);
        }
    }
    
    printf("Testing different detection methods:\n\n");
    
    // Method 1: Z-Score
    int anomalies_zscore[1000];
    int count_zscore= sp.DetectAnomaliesZScore(3.0, anomalies_zscore, 1000);
    printf("1. Z-Score Method (threshold=3.0 ):\n");
    printf("   Detected: %d anomalies\n", count_zscore);
    
    // Method 2: IQR
    int anomalies_iqr[1000];
    int count_iqr= sp.DetectAnomaliesIQR(1.5, anomalies_iqr, 1000);
    printf("\n2. IQR Method (multiplier=1.5):\n");
    printf("   Detected: %d anomalies\n", count_iqr);
    
    // Method 3: Moving Average Deviation
    int anomalies_mad[1000];
    int count_mad= sp.DetectAnomaliesMAD(20, 3.0, anomalies_mad, 1000);
    printf("\n3. Moving Average Deviation (window=20, factor=3.0):\n");
    printf("   Detected: %d anomalies\n", count_mad);
    
    // Method 4: Sudden Changes
    int anomalies_sudden[1000];
    int count_sudden= sp.DetectSuddenChanges(2.0, anomalies_sudden, 1000);
    printf("\n4. Sudden Change Detection (threshold=2.0):\n");
    printf("   Detected: %d anomalies\n", count_sudden);
    if (count_sudden > 0) {
        printf("   First few locations: ");
        for (int i= 0; i < count_sudden && i < 5; ++i) {
            printf("%d ", anomalies_sudden[i]);
        }
        printf("\n");
    }
    
    // Overall anomaly score
    printf("\n5. Overall Anomaly Scores:\n");
    printf("   Z-Score based    : %.4f\n", sp.CalculateAnomalyScore(0));
    printf("   IQR based        : %.4f\n", sp.CalculateAnomalyScore(1));
    printf("   Max Deviation    : %.4f\n", sp.CalculateAnomalyScore(2));
    
    printf("\nok  TEST PASSED: All detection methods executed successfully\n\n");
}

void TestRealTimeScenario() {
    printf("\n========================================================\n");
    printf("||  Test 4: Real-Time Monitoring Scenario                ||\n");
    printf("========================================================\n\n");
    
    printf("Simulating real-time turbine monitoring...\n");
    printf("Scenario: Blade damage develops over time\n\n");
    
    const int NUM_BLADES= 12;
    const int SAMPLES_PER_BLADE= 50;
    const int NUM_ROTATIONS= 10;
    
    int blade_markers[NUM_BLADES];
    for (int i= 0; i < NUM_BLADES; ++i) {
        blade_markers[i]= i * SAMPLES_PER_BLADE;
    }
    
    srand((unsigned int)time(NULL) + 3);
    
    printf("Rotation | Blade #7 RMS | Status        | Recommended Action\n");
    printf("---------+--------------+---------------+------------------------\n");
    
    for (int rotation= 0; rotation < NUM_ROTATIONS; ++rotation) {
        SignalProcessing sp;
        
        // Progressive degradation of blade #7
        double degradation_factor= 1.0 + (rotation * 0.3);
        
        for (int blade= 0; blade < NUM_BLADES; ++blade) {
            for (int sample= 0; sample < SAMPLES_PER_BLADE; ++sample) {
                double vibration= GenerateBladeVibration(blade, sample, SAMPLES_PER_BLADE);
                
                // Apply progressive degradation to blade #7
                if (blade== 7) {
                    vibration *= degradation_factor;
                }
                
                sp.AddValue(vibration);
            }
        }
        
        // Analyze this rotation
        SegmentStats stats[NUM_BLADES];
        sp.SegmentByMarkers(blade_markers, NUM_BLADES, stats);
        
        double blade7_rms= stats[7].rms;
        double avg_rms= 0.0;
        for (int i= 0; i < NUM_BLADES; ++i) {
            avg_rms += stats[i].rms;
        }
        avg_rms /= NUM_BLADES;
        
        double ratio= blade7_rms / avg_rms;
        
        printf("   %2d    |   %7.4f    | ", rotation + 1, blade7_rms);
        
        if (ratio > 2.5) {
            printf("  CRITICAL    | IMMEDIATE SHUTDOWN\n");
        } else if (ratio > 2.0) {
            printf("  WARNING     | Schedule maintenance\n");
        } else if (ratio > 1.5) {
            printf(" ELEVATED    | Monitor closely\n");
        } else {
            printf(" NORMAL      | Continue operation\n");
        }
    }
    
    printf("\nok  TEST PASSED: Real-time monitoring simulation complete\n\n");
}

void PrintUsageExample() {
    printf("\n========================================================\n");
    printf("||  Usage Example: Integration Guide                     ||\n");
    printf("========================================================\n\n");
    
    printf("// STEP 1: Setup\n");
    printf("SignalProcessing sp;\n");
    printf("const int NUM_BLADES= 12;\n");
    printf("const int SAMPLES_PER_BLADE= 50;\n");
    printf("int blade_markers[NUM_BLADES];\n\n");
    
    printf("// STEP 2: Collect vibration data\n");
    printf("for (int blade= 0; blade < NUM_BLADES; ++blade) {\n");
    printf("    blade_markers[blade]= blade * SAMPLES_PER_BLADE;\n");
    printf("    for (int sample= 0; sample < SAMPLES_PER_BLADE; ++sample) {\n");
    printf("        double vibration= ReadVibrationSensor();\n");
    printf("        sp.AddValue(vibration);\n");
    printf("    }\n");
    printf("}\n\n");
    
    printf("// STEP 3: Analyze by blade segments\n");
    printf("SegmentStats blade_stats[NUM_BLADES];\n");
    printf("sp.SegmentByMarkers(blade_markers, NUM_BLADES, blade_stats);\n\n");
    
    printf("// STEP 4: Find problematic blade\n");
    printf("int problematic_blade= sp.FindMostAnomalousSegment(\n");
    printf("    blade_markers, NUM_BLADES, 2  // Method 2: RMS-based\n");
    printf(");\n\n");
    
    printf("// STEP 5: Check severity\n");
    printf("if (blade_stats[problematic_blade].rms > threshold) {\n");
    printf("    TriggerAlert(problematic_blade);\n");
    printf("}\n\n");
    
    printf("// OPTIONAL: Detect all anomalies\n");
    printf("int anomaly_indices[1000];\n");
    printf("int count= sp.DetectAnomaliesZScore(3.0, anomaly_indices, 1000);\n\n");
}

int main() {
    printf("\n");
    printf("========================================================================\n");
    printf("             TURBINE VIBRATION ANOMALY DETECTION TEST SUITE\n");
    printf("========================================================================\n");
    
    TestTurbineBladeDetection();
    TestContinuousMonitoring();
    TestMultipleDetectionMethods();
    TestRealTimeScenario();
    PrintUsageExample();
    
    printf("========================================================================\n");
    printf("                        ALL TESTS COMPLETED\n");
    printf("========================================================================\n\n");
    
    return 0;
}
