/*
 * Test file for Decimation and Interpolation functions
 * Tests downsampling, upsampling, and resampling
 */

#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void test_decimation()
{
    printf("=== Test 1: Decimation (Downsampling) ===\n");
    
    SignalProcessing sp;
    
    // Generate 1000 Hz sine wave (100 samples at 1000 Hz = 0.1 seconds)
    int num_samples = 100;
    double frequency = 10.0;  // 10 Hz sine wave
    double sampling_rate = 1000.0;
    
    for (int i = 0; i < num_samples; i++)
    {
        double t = i / sampling_rate;
        double value = sin(2.0 * M_PI * frequency * t);
        sp.AddValue(value);
    }
    
    printf("Original signal: %d samples at %.0f Hz\n", sp.GetSize(), sampling_rate);
    
    // Decimate by factor 4 (1000 Hz -> 250 Hz)
    double decimated[1000];
    int decimated_count = sp.Decimate(4, decimated, true);
    
    printf("Decimated signal: %d samples (factor=4, with anti-aliasing)\n", decimated_count);
    printf("Expected: ~25 samples (100/4)\n");
    printf("First 5 decimated values: %.4f, %.4f, %.4f, %.4f, %.4f\n",
           decimated[0], decimated[1], decimated[2], decimated[3], decimated[4]);
    
    // Test without anti-aliasing
    double decimated_no_aa[1000];
    int count_no_aa = sp.Decimate(4, decimated_no_aa, false);
    
    printf("Decimated without anti-aliasing: %d samples\n", count_no_aa);
    printf("\n");
}

void test_interpolation()
{
    printf("=== Test 2: Linear Interpolation (Upsampling) ===\n");
    
    SignalProcessing sp;
    
    // Generate simple signal
    for (int i = 0; i < 10; i++)
    {
        sp.AddValue(i * 10.0);  // 0, 10, 20, 30, ..., 90
    }
    
    printf("Original signal: %d samples\n", sp.GetSize());
    printf("Values: ");
    for (int i = 0; i < sp.GetSize(); i++)
    {
        printf("%.0f ", sp.GetValue(i));
    }
    printf("\n");
    
    // Interpolate by factor 4
    double interpolated[1000];
    int interp_count = sp.InterpolateLinear(4, interpolated);
    
    printf("Interpolated signal: %d samples (factor=4)\n", interp_count);
    printf("Expected: ~37 samples (10*4 - 3 + 1)\n");
    printf("First 10 interpolated values: ");
    for (int i = 0; i < 10 && i < interp_count; i++)
    {
        printf("%.1f ", interpolated[i]);
    }
    printf("\n\n");
}

void test_resampling()
{
    printf("=== Test 3: Resampling (Rate Conversion) ===\n");
    
    SignalProcessing sp;
    
    // Generate 44.1 kHz audio-like signal (100 samples)
    double original_rate = 44100.0;
    double target_rate = 48000.0;
    
    for (int i = 0; i < 100; i++)
    {
        double t = i / original_rate;
        double value = sin(2.0 * M_PI * 440.0 * t);  // 440 Hz tone (A4)
        sp.AddValue(value);
    }
    
    printf("Original: %d samples at %.0f Hz\n", sp.GetSize(), original_rate);
    
    // Resample to 48 kHz
    double resampled[1000];
    int resampled_count = sp.Resample(original_rate, target_rate, resampled);
    
    double expected_ratio = target_rate / original_rate;
    printf("Resampled: %d samples at %.0f Hz\n", resampled_count, target_rate);
    printf("Rate ratio: %.4f (%.0f Hz -> %.0f Hz)\n", 
           expected_ratio, original_rate, target_rate);
    printf("First 5 resampled values: %.4f, %.4f, %.4f, %.4f, %.4f\n",
           resampled[0], resampled[1], resampled[2], resampled[3], resampled[4]);
    
    // Test downsampling
    double downsampled[1000];
    int down_count = sp.Resample(original_rate, 22050.0, downsampled);
    
    printf("\nDownsampled to 22.05 kHz: %d samples\n", down_count);
    printf("\n");
}

void test_practical_audio()
{
    printf("=== Test 4: Practical Audio Resampling ===\n");
    
    SignalProcessing sp;
    
    // Simulate 8 kHz telephony signal (common in VoIP)
    double telephony_rate = 8000.0;
    int num_samples = 80;  // 10ms of audio
    
    for (int i = 0; i < num_samples; i++)
    {
        // Mix of frequencies
        double t = i / telephony_rate;
        double value = sin(2.0 * M_PI * 300.0 * t) +   // Bass
                      0.5 * sin(2.0 * M_PI * 800.0 * t); // Mid
        sp.AddValue(value);
    }
    
    printf("Telephony signal: %d samples at %.0f Hz (10ms)\n", 
           sp.GetSize(), telephony_rate);
    
    // Convert to CD quality (44.1 kHz)
    double cd_quality[1000];
    int cd_count = sp.Resample(telephony_rate, 44100.0, cd_quality);
    
    printf("Converted to CD quality: %d samples at 44100 Hz\n", cd_count);
    printf("Upsampling ratio: %.2fx\n", 44100.0 / telephony_rate);
    
    // Convert back down
    SignalProcessing sp_cd;
    for (int i = 0; i < cd_count && i < NB_MAX_VALUES; i++)
    {
        sp_cd.AddValue(cd_quality[i]);
    }
    
    double telephony_back[1000];
    int tel_count = sp_cd.Resample(44100.0, telephony_rate, telephony_back);
    
    printf("Converted back to telephony: %d samples at %.0f Hz\n", 
           tel_count, telephony_rate);
    printf("Should be close to original %d samples\n", num_samples);
    printf("\n");
}

void test_decimation_frequencies()
{
    printf("=== Test 5: Decimation Effect on Frequencies ===\n");
    
    SignalProcessing sp;
    
    // Generate signal with multiple frequencies
    int num_samples = 200;
    double sampling_rate = 1000.0;
    
    for (int i = 0; i < num_samples; i++)
    {
        double t = i / sampling_rate;
        double value = sin(2.0 * M_PI * 5.0 * t) +    // 5 Hz - should survive
                      sin(2.0 * M_PI * 50.0 * t) +     // 50 Hz - might alias
                      sin(2.0 * M_PI * 100.0 * t);     // 100 Hz - will alias
        sp.AddValue(value);
    }
    
    printf("Original: %d samples, %.0f Hz sampling rate\n", 
           sp.GetSize(), sampling_rate);
    printf("Contains: 5 Hz, 50 Hz, 100 Hz components\n");
    
    // Decimate by 8 (1000 Hz -> 125 Hz)
    // Nyquist frequency becomes 62.5 Hz
    double decimated_aa[1000];
    int count_aa = sp.Decimate(8, decimated_aa, true);
    
    double decimated_no_aa[1000];
    int count_no_aa = sp.Decimate(8, decimated_no_aa, false);
    
    printf("\nDecimated by 8 (new rate: 125 Hz, Nyquist: 62.5 Hz)\n");
    printf("  With anti-aliasing: %d samples\n", count_aa);
    printf("  Without anti-aliasing: %d samples\n", count_no_aa);
    printf("  100 Hz component should be removed by anti-aliasing\n");
    printf("\n");
}

int main()
{
    printf("========================================\n");
    printf("Decimation and Interpolation Test Suite\n");
    printf("========================================\n\n");
    
    test_decimation();
    test_interpolation();
    test_resampling();
    test_practical_audio();
    test_decimation_frequencies();
    
    printf("========================================\n");
    printf("All tests completed successfully!\n");
    printf("========================================\n");
    
    return 0;
}
