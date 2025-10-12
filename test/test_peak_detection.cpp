#include <stdio.h>
#include <math.h>
#include "../source/SignalProcessing.h"

void print_peaks(const char* test_name, int* peaks, int count, SignalProcessing* sp)
{
    printf("\n=== %s ===\n", test_name);
    printf("Found %d peaks:\n", count);
    for (int i = 0; i < count; ++i)
    {
        printf("  Peak %d: index=%d, value=%.3f\n", i+1, peaks[i], sp->GetPeakValue(peaks[i]));
    }
}

void test_simple_peaks()
{
    printf("\n*** Test 1: Simple Peak Detection ***\n");
    printf("Signal: [1, 3, 2, 5, 4, 2, 6, 3, 1]\n");
    
    SignalProcessing sp;
    double values[] = {1.0, 3.0, 2.0, 5.0, 4.0, 2.0, 6.0, 3.0, 1.0};
    int size = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < size; ++i)
        sp.AddValue(values[i]);
    
    int peaks[100];
    int count = sp.DetectPeaks(peaks, 100);
    
    print_peaks("Simple Local Maxima", peaks, count, &sp);
    printf("Expected peaks at indices 1 (value=3), 3 (value=5), 6 (value=6)\n");
}

void test_peaks_with_threshold()
{
    printf("\n*** Test 2: Peak Detection with Threshold ***\n");
    printf("Signal: sine wave with amplitude 5\n");
    
    SignalProcessing sp;
    
    // Generate sine wave
    for (int i = 0; i < 50; ++i)
    {
        double value = 5.0 * sin(2.0 * M_PI * i / 10.0);
        sp.AddValue(value);
    }
    
    // Detect all peaks
    int peaks_all[100];
    int count_all = sp.DetectPeaks(peaks_all, 100);
    print_peaks("All Peaks (no threshold)", peaks_all, count_all, &sp);
    
    // Detect only peaks above threshold 3.0
    int peaks_threshold[100];
    int count_threshold = sp.DetectPeaksWithThreshold(3.0, peaks_threshold, 100);
    print_peaks("Peaks above 3.0", peaks_threshold, count_threshold, &sp);
}

void test_peaks_with_prominence()
{
    printf("\n*** Test 3: Peak Detection with Prominence ***\n");
    printf("Signal: Mix of large and small peaks\n");
    
    SignalProcessing sp;
    
    // Generate signal with peaks of different prominence
    // Large peak, small peak, large peak, small peak
    double values[] = {0, 1, 0.5, 1.2, 0.8, 1.5, 1.0, 5.0, 2.0, 1.5, 2.2, 1.8, 2.5, 1.0, 0};
    int size = sizeof(values) / sizeof(values[0]);
    
    printf("Signal values: ");
    for (int i = 0; i < size; ++i)
    {
        sp.AddValue(values[i]);
        printf("%.1f ", values[i]);
    }
    printf("\n");
    
    // Detect all peaks
    int peaks_all[100];
    int count_all = sp.DetectPeaks(peaks_all, 100);
    print_peaks("All Peaks", peaks_all, count_all, &sp);
    
    // Detect only prominent peaks (prominence >= 1.5)
    int peaks_prominent[100];
    int count_prominent = sp.DetectPeaksWithProminence(1.5, peaks_prominent, 100);
    print_peaks("Prominent Peaks (prominence >= 1.5)", peaks_prominent, count_prominent, &sp);
}

void test_peaks_with_distance()
{
    printf("\n*** Test 4: Peak Detection with Minimum Distance ***\n");
    printf("Signal: Multiple close peaks\n");
    
    SignalProcessing sp;
    
    // Generate signal with closely spaced peaks
    double values[] = {0, 5, 3, 4, 2, 6, 4, 5, 3, 7, 2, 4, 1, 8, 5, 3, 0};
    int size = sizeof(values) / sizeof(values[0]);
    
    printf("Signal values: ");
    for (int i = 0; i < size; ++i)
    {
        sp.AddValue(values[i]);
        printf("%.0f ", values[i]);
    }
    printf("\n");
    
    // Detect all peaks
    int peaks_all[100];
    int count_all = sp.DetectPeaks(peaks_all, 100);
    print_peaks("All Peaks", peaks_all, count_all, &sp);
    
    // Detect peaks with minimum distance of 5 samples
    int peaks_distance[100];
    int count_distance = sp.DetectPeaksWithDistance(5, peaks_distance, 100);
    print_peaks("Peaks with min distance = 5", peaks_distance, count_distance, &sp);
}

void test_real_world_ecg()
{
    printf("\n*** Test 5: Real-World Application - ECG-like Signal ***\n");
    printf("Simulating heartbeat peaks (R-peaks)\n");
    
    SignalProcessing sp;
    
    // Simulate ECG-like signal with periodic peaks (heartbeats)
    // Normal heart rate: ~60 bpm = 1 beat per second
    // Sampling at 100 Hz, so peaks every ~100 samples
    
    for (int i = 0; i < 500; ++i)
    {
        double value = 0.5 * sin(2.0 * M_PI * i / 100.0); // Baseline variation
        
        // Add sharp peaks (R-peaks) every 100 samples
        if (i % 100 >= 48 && i % 100 <= 52)
        {
            // Gaussian-like peak
            double peak_pos = (i % 100) - 50;
            value += 5.0 * exp(-peak_pos * peak_pos / 4.0);
        }
        
        // Add some noise
        value += 0.1 * (rand() % 100 - 50) / 100.0;
        
        sp.AddValue(value);
    }
    
    // Detect R-peaks with threshold
    int peaks_threshold[100];
    int count_threshold = sp.DetectPeaksWithThreshold(3.0, peaks_threshold, 100);
    print_peaks("R-peaks (threshold method)", peaks_threshold, count_threshold, &sp);
    
    // Detect R-peaks with distance constraint (minimum 50 samples between peaks)
    int peaks_distance[100];
    int count_distance = sp.DetectPeaksWithDistance(50, peaks_distance, 100);
    print_peaks("R-peaks (distance method)", peaks_distance, count_distance, &sp);
    
    // Calculate heart rate
    if (count_distance >= 2)
    {
        double avg_interval = 0;
        for (int i = 1; i < count_distance; ++i)
        {
            avg_interval += (peaks_distance[i] - peaks_distance[i-1]);
        }
        avg_interval /= (count_distance - 1);
        
        // If sampling at 100 Hz, interval in samples / 100 = time in seconds
        double heart_rate = 60.0 / (avg_interval / 100.0); // BPM
        printf("\nEstimated Heart Rate: %.1f BPM\n", heart_rate);
    }
}

int main()
{
    printf("=====================================\n");
    printf("   Peak Detection Test Suite\n");
    printf("=====================================\n");
    
    test_simple_peaks();
    test_peaks_with_threshold();
    test_peaks_with_prominence();
    test_peaks_with_distance();
    test_real_world_ecg();
    
    printf("\n=====================================\n");
    printf("   All Tests Completed!\n");
    printf("=====================================\n");
    
    return 0;
}
