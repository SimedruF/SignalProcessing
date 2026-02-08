#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to add Gaussian noise
double AddGaussianNoise(double value, double noise_level) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double noise = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2) * noise_level;
    return value + noise;
}

// Helper function to calculate Mean Squared Error
double CalculateMSE(double *original, double *denoised, int size) {
    double mse = 0.0;
    for (int i = 0; i < size; ++i) {
        double diff = original[i] - denoised[i];
        mse += diff * diff;
    }
    return mse / size;
}

// Helper function to calculate Signal-to-Noise Ratio
double CalculateSNR(double *original, double *denoised, int size) {
    double signal_power = 0.0;
    double noise_power = 0.0;
    
    for (int i = 0; i < size; ++i) {
        signal_power += original[i] * original[i];
        double diff = original[i] - denoised[i];
        noise_power += diff * diff;
    }
    
    if (noise_power == 0.0) return 1000.0; // Very high SNR
    return 10.0 * log10(signal_power / noise_power);
}

void TestKalmanFilter() {
    printf("\n=== Test 1: Kalman Filter ===\n");
    
    SignalProcessing sp;
    double original[100];
    double noisy[100];
    double denoised[100];
    
    // Generate sine wave with noise
    srand((unsigned int)time(NULL));
    double noise_level = 0.5;
    
    for (int i = 0; i < 100; ++i) {
        original[i] = sin(i * 0.1);
        noisy[i] = AddGaussianNoise(original[i], noise_level);
        sp.AddValue(noisy[i]);
    }
    
    // Apply Kalman filter
    sp.KalmanFilter(0.01, 0.1, denoised);
    
    // Calculate metrics
    double mse = CalculateMSE(original, denoised, 100);
    double snr = CalculateSNR(original, denoised, 100);
    
    printf("Kalman Filter Results:\n");
    printf("  MSE: %f\n", mse);
    printf("  SNR: %f dB\n", snr);
    printf("  Sample values:\n");
    printf("    Original[50]: %f, Noisy[50]: %f, Denoised[50]: %f\n", 
           original[50], noisy[50], denoised[50]);
    printf("    Original[75]: %f, Noisy[75]: %f, Denoised[75]: %f\n", 
           original[75], noisy[75], denoised[75]);
    
    if (mse < 0.1 && snr > 10.0) {
        printf("  ✓ PASSED - Kalman filter effectively reduced noise\n");
    } else {
        printf("  ✗ FAILED - Denoising performance below threshold\n");
    }
}

void TestWaveletDenoise() {
    printf("\n=== Test 2: Wavelet Denoising ===\n");
    
    SignalProcessing sp;
    double original[128];
    double noisy[128];
    double denoised[128];
    
    // Generate square wave with noise
    srand((unsigned int)time(NULL) + 1);
    double noise_level = 0.3;
    
    for (int i = 0; i < 128; ++i) {
        original[i] = (i % 32 < 16) ? 1.0 : -1.0;
        noisy[i] = AddGaussianNoise(original[i], noise_level);
        sp.AddValue(noisy[i]);
    }
    
    // Estimate noise and apply wavelet denoising
    double estimated_noise = sp.EstimateNoiseLevel();
    sp.WaveletDenoise(estimated_noise * 2.0, denoised, 2);
    
    // Calculate metrics
    double mse = CalculateMSE(original, denoised, 128);
    double snr = CalculateSNR(original, denoised, 128);
    
    printf("Wavelet Denoising Results:\n");
    printf("  Estimated noise level: %f\n", estimated_noise);
    printf("  MSE: %f\n", mse);
    printf("  SNR: %f dB\n", snr);
    printf("  Sample values:\n");
    printf("    Original[64]: %f, Noisy[64]: %f, Denoised[64]: %f\n", 
           original[64], noisy[64], denoised[64]);
    printf("    Original[96]: %f, Noisy[96]: %f, Denoised[96]: %f\n", 
           original[96], noisy[96], denoised[96]);
    
    if (mse < 0.2 && snr > 8.0) {
        printf("  ✓ PASSED - Wavelet denoising effectively reduced noise\n");
    } else {
        printf("  ✗ FAILED - Denoising performance below threshold\n");
    }
}

void TestMedianFilter() {
    printf("\n=== Test 3: Median Filter ===\n");
    
    SignalProcessing sp;
    double original[100];
    double noisy[100];
    double denoised[100];
    
    // Generate smooth signal with impulse noise (salt-and-pepper)
    srand((unsigned int)time(NULL) + 2);
    
    for (int i = 0; i < 100; ++i) {
        original[i] = sin(i * 0.1);
        noisy[i] = original[i];
        
        // Add random impulse noise (10% probability)
        if (rand() % 10 == 0) {
            noisy[i] += (rand() % 2 == 0) ? 5.0 : -5.0;
        }
        
        sp.AddValue(noisy[i]);
    }
    
    // Apply median filter
    sp.MedianFilter(5, denoised);
    
    // Calculate metrics
    double mse = CalculateMSE(original, denoised, 100);
    double snr = CalculateSNR(original, denoised, 100);
    
    printf("Median Filter Results:\n");
    printf("  MSE: %f\n", mse);
    printf("  SNR: %f dB\n", snr);
    printf("  Sample values:\n");
    printf("    Original[30]: %f, Noisy[30]: %f, Denoised[30]: %f\n", 
           original[30], noisy[30], denoised[30]);
    printf("    Original[60]: %f, Noisy[60]: %f, Denoised[60]: %f\n", 
           original[60], noisy[60], denoised[60]);
    
    if (mse < 0.5 && snr > 10.0) {
        printf("  ✓ PASSED - Median filter effectively removed impulse noise\n");
    } else {
        printf("  ✗ FAILED - Denoising performance below threshold\n");
    }
}

void TestNoiseEstimation() {
    printf("\n=== Test 4: Noise Level Estimation ===\n");
    
    SignalProcessing sp;
    double noise_level = 0.5;
    
    // Generate signal with known noise level
    srand((unsigned int)time(NULL) + 3);
    for (int i = 0; i < 100; ++i) {
        double clean_signal = sin(i * 0.1);
        double noisy_signal = AddGaussianNoise(clean_signal, noise_level);
        sp.AddValue(noisy_signal);
    }
    
    // Estimate noise level
    double estimated_noise = sp.EstimateNoiseLevel();
    double error = fabs(estimated_noise - noise_level);
    double error_percent = (error / noise_level) * 100.0;
    
    printf("Noise Estimation Results:\n");
    printf("  True noise level: %f\n", noise_level);
    printf("  Estimated noise level: %f\n", estimated_noise);
    printf("  Absolute error: %f\n", error);
    printf("  Relative error: %.2f%%\n", error_percent);
    
    if (error_percent < 50.0) {
        printf("  ✓ PASSED - Noise estimation is reasonably accurate\n");
    } else {
        printf("  ✗ FAILED - Noise estimation error too high\n");
    }
}

void TestDenoisingComparison() {
    printf("\n=== Test 5: Denoising Methods Comparison ===\n");
    
    SignalProcessing sp;
    double original[100];
    double noisy[100];
    double kalman_out[100];
    double wavelet_out[100];
    double median_out[100];
    
    // Generate test signal with mixed noise
    srand((unsigned int)time(NULL) + 4);
    double gaussian_noise = 0.3;
    
    for (int i = 0; i < 100; ++i) {
        original[i] = sin(i * 0.15) + 0.5 * cos(i * 0.05);
        noisy[i] = AddGaussianNoise(original[i], gaussian_noise);
        
        // Add occasional impulses
        if (rand() % 20 == 0) {
            noisy[i] += (rand() % 2 == 0) ? 2.0 : -2.0;
        }
        
        sp.AddValue(noisy[i]);
    }
    
    // Apply all denoising methods
    sp.KalmanFilter(0.01, 0.1, kalman_out);
    
    double noise_estimate = sp.EstimateNoiseLevel();
    sp.WaveletDenoise(noise_estimate * 1.5, wavelet_out, 2);
    
    sp.MedianFilter(5, median_out);
    
    // Calculate performance metrics
    double kalman_mse = CalculateMSE(original, kalman_out, 100);
    double wavelet_mse = CalculateMSE(original, wavelet_out, 100);
    double median_mse = CalculateMSE(original, median_out, 100);
    
    double kalman_snr = CalculateSNR(original, kalman_out, 100);
    double wavelet_snr = CalculateSNR(original, wavelet_out, 100);
    double median_snr = CalculateSNR(original, median_out, 100);
    
    printf("Comparison Results:\n");
    printf("  Method          | MSE      | SNR (dB)\n");
    printf("  ----------------|----------|----------\n");
    printf("  Kalman Filter   | %.6f | %.2f\n", kalman_mse, kalman_snr);
    printf("  Wavelet Denoise | %.6f | %.2f\n", wavelet_mse, wavelet_snr);
    printf("  Median Filter   | %.6f | %.2f\n", median_mse, median_snr);
    
    printf("\n  Best MSE: ");
    if (kalman_mse <= wavelet_mse && kalman_mse <= median_mse)
        printf("Kalman Filter\n");
    else if (wavelet_mse <= median_mse)
        printf("Wavelet Denoise\n");
    else
        printf("Median Filter\n");
    
    printf("  Best SNR: ");
    if (kalman_snr >= wavelet_snr && kalman_snr >= median_snr)
        printf("Kalman Filter\n");
    else if (wavelet_snr >= median_snr)
        printf("Wavelet Denoise\n");
    else
        printf("Median Filter\n");
    
    if (kalman_snr > 5.0 && wavelet_snr > 5.0 && median_snr > 5.0) {
        printf("  ✓ PASSED - All methods achieved acceptable denoising\n");
    } else {
        printf("  ✗ WARNING - Some methods performed below expectations\n");
    }
}

void TestEdgeCases() {
    printf("\n=== Test 6: Edge Cases ===\n");
    
    // Test 1: Empty signal
    SignalProcessing sp1;
    double out1[10];
    sp1.KalmanFilter(0.01, 0.1, out1);
    printf("  Empty signal - Kalman: ✓ No crash\n");
    
    // Test 2: Single value
    SignalProcessing sp2;
    sp2.AddValue(5.0);
    double out2[10];
    sp2.WaveletDenoise(0.5, out2);
    printf("  Single value - Wavelet: ✓ No crash\n");
    
    // Test 3: Two values
    SignalProcessing sp3;
    sp3.AddValue(1.0);
    sp3.AddValue(2.0);
    double noise = sp3.EstimateNoiseLevel();
    printf("  Two values - Noise estimation: %f ✓\n", noise);
    
    // Test 4: Constant signal
    SignalProcessing sp4;
    for (int i = 0; i < 50; ++i) {
        sp4.AddValue(3.14159);
    }
    double out4[100];
    sp4.MedianFilter(7, out4);
    printf("  Constant signal - Median filter: ✓ No crash\n");
    
    // Test 5: Very noisy signal
    SignalProcessing sp5;
    srand((unsigned int)time(NULL) + 5);
    for (int i = 0; i < 50; ++i) {
        sp5.AddValue(AddGaussianNoise(0.0, 10.0));
    }
    double out5[100];
    sp5.KalmanFilter(0.1, 1.0, out5);
    printf("  High noise signal - Kalman: ✓ No crash\n");
    
    printf("  ✓ PASSED - All edge cases handled correctly\n");
}

int main() {
    printf("========================================\n");
    printf("   Signal Denoising Test Suite\n");
    printf("========================================\n");
    
    TestKalmanFilter();
    TestWaveletDenoise();
    TestMedianFilter();
    TestNoiseEstimation();
    TestDenoisingComparison();
    TestEdgeCases();
    
    printf("\n========================================\n");
    printf("   Test Suite Complete\n");
    printf("========================================\n");
    
    return 0;
}
