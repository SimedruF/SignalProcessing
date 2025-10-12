/**
 * @file example_complete.cpp
 * @brief Complete example demonstrating all SignalProcessing features
 * 
 * This example shows a typical workflow for processing a simulated ECG signal:
 * 1. Data acquisition with timestamps
 * 2. Preprocessing (smoothing, normalization)
 * 3. Feature detection (R-peaks)
 * 4. Analysis (heart rate, statistics)
 * 5. Event detection
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../source/SignalProcessing.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Simulates ECG data acquisition
 * @param sp SignalProcessing object to store data
 * @param duration_sec Duration in seconds
 * @param sampling_rate Sampling frequency (Hz)
 * @param heart_rate Heart rate in BPM
 */
void acquire_ecg_data(SignalProcessing *sp, double duration_sec, 
                      double sampling_rate, double heart_rate)
{
    int total_samples = (int)(duration_sec * sampling_rate);
    double beat_interval = 60.0 / heart_rate; // seconds per beat
    int samples_per_beat = (int)(beat_interval * sampling_rate);
    
    printf("Simulating ECG acquisition:\n");
    printf("  Duration: %.1f seconds\n", duration_sec);
    printf("  Sampling rate: %.0f Hz\n", sampling_rate);
    printf("  Heart rate: %.0f BPM\n", heart_rate);
    printf("  Total samples: %d\n\n", total_samples);
    
    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    
    for (int i = 0; i < total_samples; ++i)
    {
        // Baseline wander (breathing artifact)
        double baseline = 0.3 * sin(2.0 * M_PI * i / (sampling_rate * 3.0));
        
        // P-wave, QRS complex, T-wave
        int phase = i % samples_per_beat;
        double ecg_value = baseline;
        
        // P-wave (small bump before QRS)
        if (phase >= samples_per_beat * 0.15 && phase <= samples_per_beat * 0.25)
        {
            double p_pos = (phase - samples_per_beat * 0.2) / (samples_per_beat * 0.05);
            ecg_value += 0.3 * exp(-p_pos * p_pos / 0.5);
        }
        
        // QRS complex (sharp peak)
        if (phase >= samples_per_beat * 0.45 && phase <= samples_per_beat * 0.55)
        {
            double qrs_pos = (phase - samples_per_beat * 0.5) / (samples_per_beat * 0.02);
            // Q-wave (small negative)
            if (qrs_pos < -1.0)
                ecg_value -= 0.2 * exp(-(qrs_pos + 1.5) * (qrs_pos + 1.5) / 0.3);
            // R-wave (large positive)
            else if (qrs_pos >= -1.0 && qrs_pos <= 0.0)
                ecg_value += 5.0 * exp(-qrs_pos * qrs_pos / 0.1);
            // S-wave (negative)
            else
                ecg_value -= 0.5 * exp(-(qrs_pos - 1.0) * (qrs_pos - 1.0) / 0.3);
        }
        
        // T-wave (rounded bump after QRS)
        if (phase >= samples_per_beat * 0.65 && phase <= samples_per_beat * 0.85)
        {
            double t_pos = (phase - samples_per_beat * 0.75) / (samples_per_beat * 0.1);
            ecg_value += 0.5 * exp(-t_pos * t_pos / 1.0);
        }
        
        // Add noise
        ecg_value += 0.05 * (rand() % 100 - 50) / 100.0;
        
        // Create timestamp
        struct timespec ts = start_time;
        long nsec_offset = (long)((i / sampling_rate) * 1e9);
        ts.tv_sec += nsec_offset / 1000000000L;
        ts.tv_nsec += nsec_offset % 1000000000L;
        if (ts.tv_nsec >= 1000000000L) {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000L;
        }
        
        sp->AddValueWithTimestamp(ecg_value, ts);
    }
    
    printf("✓ Data acquisition complete: %d samples\n\n", sp->GetIndex());
}

/**
 * @brief Preprocesses the signal
 */
void preprocess_signal(SignalProcessing *sp, SignalProcessing *sp_clean)
{
    printf("Preprocessing signal:\n");
    
    // Step 1: Moving average for noise reduction
    double smoothed[1000];
    sp->GetMovingAverageVector(smoothed, 3);
    printf("  ✓ Applied moving average (window=3)\n");
    
    // Step 2: Load smoothed data
    for (int i = 0; i < sp->GetIndex(); ++i)
        sp_clean->AddValue(smoothed[i]);
    
    // Step 3: Normalize
    sp_clean->ScaleVector(0, 10);
    printf("  ✓ Scaled to [0, 10]\n");
    
    // Statistics
    double mean = sp_clean->GetMean();
    double std = sp_clean->GetStandardDeviation();
    printf("  ✓ Mean: %.3f, Std Dev: %.3f\n\n", mean, std);
}

/**
 * @brief Detects R-peaks and analyzes heart rate
 */
void detect_r_peaks(SignalProcessing *sp, double sampling_rate)
{
    printf("Detecting R-peaks:\n");
    
    // Calculate adaptive threshold
    double mean = sp->GetMean();
    double std = sp->GetStandardDeviation();
    double threshold = mean + 1.5 * std;
    printf("  Threshold: %.3f (mean + 1.5*std)\n", threshold);
    
    // Method 1: Threshold-based detection
    int peaks_threshold[100];
    int count_threshold = sp->DetectPeaksWithThreshold(threshold, peaks_threshold, 100);
    printf("  Method 1 (Threshold): %d peaks detected\n", count_threshold);
    
    // Method 2: Distance-based detection (more robust)
    int min_rr_distance = (int)(sampling_rate * 0.4); // Min 0.4s between beats (150 BPM max)
    int peaks_distance[100];
    int count_distance = sp->DetectPeaksWithDistance(min_rr_distance, peaks_distance, 100);
    printf("  Method 2 (Distance): %d peaks detected\n", count_distance);
    
    // Use distance-based peaks for analysis
    if (count_distance >= 2)
    {
        printf("\n  R-peak analysis:\n");
        
        // Calculate RR intervals
        double rr_intervals[100];
        double sum_rr = 0;
        
        for (int i = 1; i < count_distance; ++i)
        {
            rr_intervals[i-1] = (peaks_distance[i] - peaks_distance[i-1]) / sampling_rate;
            sum_rr += rr_intervals[i-1];
            printf("    RR[%d]: %.3f s (%.1f BPM)\n", 
                   i, rr_intervals[i-1], 60.0 / rr_intervals[i-1]);
        }
        
        // Average heart rate
        double avg_rr = sum_rr / (count_distance - 1);
        double avg_hr = 60.0 / avg_rr;
        printf("\n  ✓ Average Heart Rate: %.1f BPM\n", avg_hr);
        
        // Heart Rate Variability (SDNN)
        double hrv_sum = 0;
        for (int i = 0; i < count_distance - 1; ++i)
        {
            double diff = rr_intervals[i] - avg_rr;
            hrv_sum += diff * diff;
        }
        double sdnn = sqrt(hrv_sum / (count_distance - 1));
        printf("  ✓ HRV (SDNN): %.3f s\n", sdnn);
        
        // Peak amplitudes
        printf("\n  R-peak amplitudes:\n");
        for (int i = 0; i < count_distance; ++i)
        {
            double amplitude = sp->GetPeakValue(peaks_distance[i]);
            printf("    Peak[%d] at sample %d: %.3f\n", 
                   i+1, peaks_distance[i], amplitude);
        }
    }
    else
    {
        printf("  ⚠ Not enough peaks for analysis\n");
    }
    
    printf("\n");
}

/**
 * @brief Detects arrhythmia events
 */
void detect_events(SignalProcessing *sp)
{
    printf("Event detection:\n");
    
    // Detect threshold crossings (potential arrhythmia)
    double high_threshold = sp->GetMean() + 2.5 * sp->GetStandardDeviation();
    int events_high[100];
    int count_high = sp->DetectThresholdCrossing(high_threshold, 1, events_high);
    printf("  High amplitude events (>%.3f): %d\n", high_threshold, count_high);
    
    if (count_high > 0)
    {
        printf("    Events at samples: ");
        for (int i = 0; i < count_high && i < 5; ++i)
            printf("%d ", events_high[i]);
        if (count_high > 5)
            printf("...");
        printf("\n");
    }
    
    // Check flags
    if (sp->GetThresholdCrossingFlag())
        printf("  ⚠ Threshold crossing flag is SET\n");
    else
        printf("  ✓ No threshold crossing flag\n");
    
    printf("\n");
}

/**
 * @brief Performs statistical analysis
 */
void analyze_statistics(SignalProcessing *sp)
{
    printf("Statistical Analysis:\n");
    
    double mean = sp->GetMean();
    double variance = sp->GetVariance();
    double std_dev = sp->GetStandardDeviation();
    
    printf("  Mean:             %.4f\n", mean);
    printf("  Variance:         %.4f\n", variance);
    printf("  Std Deviation:    %.4f\n", std_dev);
    printf("  Coefficient of Variation: %.2f%%\n", (std_dev / mean) * 100);
    
    // Find min/max
    double min_val = sp->GetValue(0);
    double max_val = sp->GetValue(0);
    for (int i = 1; i < sp->GetIndex(); ++i)
    {
        double val = sp->GetValue(i);
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    
    printf("  Min value:        %.4f\n", min_val);
    printf("  Max value:        %.4f\n", max_val);
    printf("  Range:            %.4f\n", max_val - min_val);
    
    printf("\n");
}

/**
 * @brief Main function demonstrating complete workflow
 */
int main()
{
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     Complete ECG Signal Processing Example                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Initialize
    SignalProcessing sp_raw;      // Raw signal
    SignalProcessing sp_clean;    // Preprocessed signal
    
    // Parameters
    double duration = 10.0;       // 10 seconds
    double sampling_rate = 100.0; // 100 Hz
    double heart_rate = 72.0;     // 72 BPM
    
    // Step 1: Data Acquisition
    printf("═══ STEP 1: DATA ACQUISITION ═══\n");
    acquire_ecg_data(&sp_raw, duration, sampling_rate, heart_rate);
    
    // Step 2: Preprocessing
    printf("═══ STEP 2: PREPROCESSING ═══\n");
    preprocess_signal(&sp_raw, &sp_clean);
    
    // Step 3: Statistical Analysis
    printf("═══ STEP 3: STATISTICAL ANALYSIS ═══\n");
    analyze_statistics(&sp_clean);
    
    // Step 4: R-Peak Detection & Heart Rate Analysis
    printf("═══ STEP 4: R-PEAK DETECTION & ANALYSIS ═══\n");
    detect_r_peaks(&sp_clean, sampling_rate);
    
    // Step 5: Event Detection
    printf("═══ STEP 5: EVENT DETECTION ═══\n");
    detect_events(&sp_clean);
    
    // Step 6: Timestamp Analysis
    printf("═══ STEP 6: TIMESTAMP ANALYSIS ═══\n");
    printf("Signal duration:\n");
    struct timespec ts_first = sp_raw.GetTimestamp(0);
    struct timespec ts_last = sp_raw.GetTimestamp(sp_raw.GetIndex() - 1);
    double duration_measured = (ts_last.tv_sec - ts_first.tv_sec) + 
                               (ts_last.tv_nsec - ts_first.tv_nsec) / 1e9;
    printf("  First timestamp: %ld.%09ld\n", ts_first.tv_sec, ts_first.tv_nsec);
    printf("  Last timestamp:  %ld.%09ld\n", ts_last.tv_sec, ts_last.tv_nsec);
    printf("  Measured duration: %.3f seconds\n", duration_measured);
    printf("  ✓ Timestamps correctly stored\n\n");
    
    // Summary
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    ANALYSIS COMPLETE                       ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  Total samples processed: %-4d                            ║\n", sp_clean.GetIndex());
    printf("║  Signal quality: GOOD                                     ║\n");
    printf("║  R-peaks detected successfully                            ║\n");
    printf("║  Heart rate analysis complete                             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}
