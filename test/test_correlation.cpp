/*
 * Test file for Autocorrelation and Cross-correlation functions
 * Tests periodicity detection, time delay estimation, and peak finding
 */

#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void test_autocorrelation_sine()
{
    printf("=== Test 1: Autocorrelation of Sine Wave (Periodicity Detection) ===\n");
    
    SignalProcessing sp;
    
    // Generate 10 Hz sine wave at 100 Hz sampling rate
    double frequency = 10.0;
    double sampling_rate = 100.0;
    int num_samples = 100;  // 1 second
    
    for (int i = 0; i < num_samples; i++)
    {
        double t = i / sampling_rate;
        double value = sin(2.0 * M_PI * frequency * t);
        sp.AddValue(value);
    }
    
    printf("Signal: 10 Hz sine wave, 100 samples at 100 Hz sampling\n");
    printf("Expected period: %.1f samples (100 Hz / 10 Hz)\n", sampling_rate / frequency);
    
    // Compute autocorrelation
    double autocorr[100];
    int max_lag = 50;
    int corr_count = sp.Autocorrelation(max_lag, autocorr, true);
    
    printf("Autocorrelation computed for %d lags (normalized)\n", corr_count);
    
    // Find first peak (excluding lag=0)
    double peak_value = 0.0;
    int peak_lag = sp.FindCorrelationPeak(autocorr, corr_count, &peak_value);
    
    printf("First peak at lag %d with value %.4f\n", peak_lag, peak_value);
    printf("Detected period: %d samples\n", peak_lag);
    printf("Detected frequency: %.2f Hz\n", sampling_rate / peak_lag);
    
    // Show first few correlation values
    printf("\nFirst 15 autocorrelation values:\n");
    for (int i = 0; i < 15 && i < corr_count; i++)
    {
        printf("  lag %2d: %7.4f", i, autocorr[i]);
        if (i % 3 == 2) printf("\n");
    }
    printf("\n\n");
}

void test_autocorrelation_ecg()
{
    printf("=== Test 2: Autocorrelation for ECG Heart Rate Detection ===\n");
    
    SignalProcessing sp;
    
    // Simulate ECG signal with 75 BPM (1.25 Hz)
    double heart_rate = 1.25;  // Hz (75 beats per minute)
    double sampling_rate = 100.0;  // 100 Hz
    int num_samples = 200;  // 2 seconds
    
    for (int i = 0; i < num_samples; i++)
    {
        double t = i / sampling_rate;
        // Simplified ECG with sharp R-peaks
        double ecg = 0.0;
        double period = 1.0 / heart_rate;
        double phase = fmod(t, period) / period;
        
        if (phase < 0.1)
        {
            // R-peak (QRS complex)
            ecg = 5.0 * sin(M_PI * phase / 0.1);
        }
        else if (phase < 0.3)
        {
            // T-wave
            ecg = 1.0 * sin(M_PI * (phase - 0.1) / 0.2);
        }
        
        // Add some noise
        ecg += 0.1 * (rand() % 100 - 50) / 50.0;
        
        sp.AddValue(ecg);
    }
    
    printf("Simulated ECG: 75 BPM (1.25 Hz), 200 samples at 100 Hz\n");
    printf("Expected R-R interval: %.1f samples\n", sampling_rate / heart_rate);
    
    // Compute autocorrelation
    double autocorr[100];
    int max_lag = 100;
    int corr_count = sp.Autocorrelation(max_lag, autocorr, true);
    
    // Find R-R interval peak
    double peak_value = 0.0;
    int rr_interval = sp.FindCorrelationPeak(autocorr, corr_count, &peak_value);
    
    printf("Detected R-R interval: %d samples (%.4f correlation)\n", 
           rr_interval, peak_value);
    printf("Detected heart rate: %.2f BPM\n", 
           (sampling_rate / rr_interval) * 60.0);
    printf("\n");
}

void test_cross_correlation_delay()
{
    printf("=== Test 3: Cross-correlation for Time Delay Estimation ===\n");
    
    SignalProcessing sp1;
    
    // Generate original signal
    int num_samples = 100;
    double signal2[200];
    int delay = 15;  // samples
    
    for (int i = 0; i < num_samples; i++)
    {
        double value = sin(2.0 * M_PI * 0.05 * i) + 
                      0.5 * sin(2.0 * M_PI * 0.1 * i);
        sp1.AddValue(value);
    }
    
    // Create delayed copy with noise
    for (int i = 0; i < num_samples + delay; i++)
    {
        if (i < delay)
        {
            signal2[i] = 0.1 * (rand() % 100 - 50) / 50.0;  // Noise before signal
        }
        else
        {
            signal2[i] = sp1.GetValue(i - delay) + 0.05 * (rand() % 100 - 50) / 50.0;
        }
    }
    
    printf("Signal 1: %d samples\n", sp1.GetSize());
    printf("Signal 2: %d samples (delayed by %d samples + noise)\n", 
           num_samples + delay, delay);
    
    // Compute cross-correlation
    double crosscorr[200];
    int max_lag = 50;
    int corr_count = sp1.CrossCorrelation(signal2, num_samples + delay, 
                                          max_lag, crosscorr, true);
    
    printf("Cross-correlation computed: %d values (±%d lags)\n", 
           corr_count, max_lag);
    
    // Find peak to detect delay
    double peak_value = 0.0;
    int peak_index = sp1.FindCorrelationPeak(crosscorr, corr_count, &peak_value);
    
    // Convert index to lag (centered at max_lag)
    int detected_lag = peak_index - max_lag;
    
    printf("Peak at index %d (lag %d) with value %.4f\n", 
           peak_index, detected_lag, peak_value);
    printf("Expected delay: %d samples\n", delay);
    printf("Detected delay: %d samples\n", detected_lag);
    printf("\n");
}

void test_cross_correlation_similarity()
{
    printf("=== Test 4: Cross-correlation for Signal Similarity ===\n");
    
    SignalProcessing sp1;
    
    // Generate reference signal
    int num_samples = 50;
    for (int i = 0; i < num_samples; i++)
    {
        double value = sin(2.0 * M_PI * 0.1 * i);
        sp1.AddValue(value);
    }
    
    // Test signal 1: Similar (same frequency)
    double similar_signal[50];
    for (int i = 0; i < num_samples; i++)
    {
        similar_signal[i] = sin(2.0 * M_PI * 0.1 * i + 0.5);  // Phase shifted
    }
    
    // Test signal 2: Different (different frequency)
    double different_signal[50];
    for (int i = 0; i < num_samples; i++)
    {
        different_signal[i] = sin(2.0 * M_PI * 0.3 * i);  // 3x frequency
    }
    
    // Test signal 3: Random noise
    double noise_signal[50];
    for (int i = 0; i < num_samples; i++)
    {
        noise_signal[i] = (rand() % 100 - 50) / 50.0;
    }
    
    printf("Reference: 0.1 Hz sine wave\n\n");
    
    // Cross-correlate with similar signal
    double corr1[100];
    int count1 = sp1.CrossCorrelation(similar_signal, num_samples, 
                                      25, corr1, true);
    double peak1 = 0.0;
    sp1.FindCorrelationPeak(corr1, count1, &peak1);
    
    printf("Similar signal (same freq, phase shift):\n");
    printf("  Max correlation: %.4f\n", peak1);
    
    // Cross-correlate with different signal
    double corr2[100];
    int count2 = sp1.CrossCorrelation(different_signal, num_samples, 
                                      25, corr2, true);
    double peak2 = 0.0;
    sp1.FindCorrelationPeak(corr2, count2, &peak2);
    
    printf("Different signal (3x frequency):\n");
    printf("  Max correlation: %.4f\n", peak2);
    
    // Cross-correlate with noise
    double corr3[100];
    int count3 = sp1.CrossCorrelation(noise_signal, num_samples, 
                                      25, corr3, true);
    double peak3 = 0.0;
    sp1.FindCorrelationPeak(corr3, count3, &peak3);
    
    printf("Random noise:\n");
    printf("  Max correlation: %.4f\n", peak3);
    
    printf("\nExpected: Similar > Different > Noise\n\n");
}

void test_autocorrelation_composite()
{
    printf("=== Test 5: Autocorrelation of Composite Signal ===\n");
    
    SignalProcessing sp;
    
    // Generate signal with two frequency components
    double f1 = 5.0;   // 5 Hz
    double f2 = 12.0;  // 12 Hz
    double sampling_rate = 100.0;
    int num_samples = 200;
    
    for (int i = 0; i < num_samples; i++)
    {
        double t = i / sampling_rate;
        double value = sin(2.0 * M_PI * f1 * t) + 
                      0.7 * sin(2.0 * M_PI * f2 * t);
        sp.AddValue(value);
    }
    
    printf("Composite signal: 5 Hz + 12 Hz at 100 Hz sampling\n");
    printf("Expected periods: %.1f samples (5 Hz), %.1f samples (12 Hz)\n",
           sampling_rate / f1, sampling_rate / f2);
    
    // Compute autocorrelation
    double autocorr[100];
    int max_lag = 80;
    int corr_count = sp.Autocorrelation(max_lag, autocorr, true);
    
    // Find multiple peaks
    printf("\nPeaks in autocorrelation:\n");
    for (int i = 1; i < corr_count; i++)
    {
        // Simple peak detection: value higher than neighbors
        if (i > 0 && i < corr_count - 1)
        {
            if (autocorr[i] > autocorr[i-1] && 
                autocorr[i] > autocorr[i+1] && 
                autocorr[i] > 0.5)  // Threshold
            {
                printf("  Lag %2d: %.4f (period detected)\n", i, autocorr[i]);
            }
        }
    }
    
    printf("\n");
}

void test_normalized_vs_unnormalized()
{
    printf("=== Test 6: Normalized vs Unnormalized Correlation ===\n");
    
    SignalProcessing sp;
    
    // Generate simple signal
    for (int i = 0; i < 50; i++)
    {
        sp.AddValue(sin(2.0 * M_PI * 0.1 * i));
    }
    
    // Normalized autocorrelation
    double autocorr_norm[50];
    sp.Autocorrelation(20, autocorr_norm, true);
    
    // Unnormalized autocorrelation
    double autocorr_unnorm[50];
    sp.Autocorrelation(20, autocorr_unnorm, false);
    
    printf("Normalized autocorrelation (first 10 lags):\n");
    for (int i = 0; i < 10; i++)
    {
        printf("  lag %d: %7.4f", i, autocorr_norm[i]);
        if (i % 3 == 2) printf("\n");
    }
    if (10 % 3 != 0) printf("\n");
    
    printf("\nUnnormalized autocorrelation (first 10 lags):\n");
    for (int i = 0; i < 10; i++)
    {
        printf("  lag %d: %9.4f", i, autocorr_unnorm[i]);
        if (i % 3 == 2) printf("\n");
    }
    if (10 % 3 != 0) printf("\n");
    
    printf("\nNormalized values are in range [-1, 1]\n");
    printf("Unnormalized values show actual covariance\n\n");
}

int main()
{
    printf("=============================================\n");
    printf("Autocorrelation and Cross-correlation Tests\n");
    printf("=============================================\n\n");
    
    test_autocorrelation_sine();
    test_autocorrelation_ecg();
    test_cross_correlation_delay();
    test_cross_correlation_similarity();
    test_autocorrelation_composite();
    test_normalized_vs_unnormalized();
    
    printf("=============================================\n");
    printf("All tests completed successfully!\n");
    printf("=============================================\n");
    
    return 0;
}
