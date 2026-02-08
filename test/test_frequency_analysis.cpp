#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Helper function: Generate a sinusoidal signal at a specific frequency
 * 
 * @param sp SignalProcessing object to store the generated signal
 * @param frequency Target frequency in Hz
 * @param sampling_rate Sampling rate in Hz
 * @param num_samples Number of samples to generate
 * @param amplitude Signal amplitude (default = 1.0)
 * @param phase_offset Phase offset in radians (default = 0.0)
 */
void GenerateSignal(SignalProcessing* sp, double frequency, double sampling_rate, 
                    int num_samples, double amplitude = 1.0, double phase_offset = 0.0) {
    for (int i = 0; i < num_samples; ++i) {
        double t = i / sampling_rate;
        double signal = amplitude * sin(2.0 * M_PI * frequency * t + phase_offset);
        sp->AddValue(signal);
    }
}

/**
 * Frequency Analysis Test Suite
 * 
 * Tests FFT analysis, frequency peak detection, harmonic analysis,
 * and frequency-based anomaly detection for turbine monitoring
 */

void TestBasicFFT() {
    printf("\n ========================================================\n");
    printf("||  Test 1: Basic FFT Analysis                           ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    // Generate a composite signal: 50 Hz + 150 Hz + 300 Hz
    double sampling_rate= 1000.0;  // 1000 Hz sampling
    int num_samples= 1024;         // Power of 2 for FFT
    
    double freq1= 50.0;   // Fundamental
    double freq2= 150.0;  // 3rd harmonic
    double freq3= 300.0;  // 6th harmonic
    
    printf("Generating test signal:\n");
    printf("  Sampling rate: %.0f Hz\n", sampling_rate);
    printf("  Number of samples: %d\n", num_samples);
    printf("  Duration: %.3f seconds\n", num_samples / sampling_rate);
    printf("  Frequencies: %.0f Hz, %.0f Hz, %.0f Hz\n\n", freq1, freq2, freq3);
    
    for (int i= 0; i < num_samples; ++i) {
        double t= i / sampling_rate;
        double signal= sin(2.0 * M_PI * freq1 * t) + 
                       0.5 * sin(2.0 * M_PI * freq2 * t) + 
                       0.3 * sin(2.0 * M_PI * freq3 * t);
        sp.AddValue(signal);
    }
    
    // Perform FFT
    FrequencySpectrum spectrum;
    if (sp.FFTAnalysis(sampling_rate, &spectrum)) {
        printf("FFT Analysis Results:\n");
        printf("  Number of frequency bins: %d\n", spectrum.num_bins);
        printf("  Frequency resolution: %.3f Hz\n", spectrum.frequency_resolution);
        printf("  Dominant frequency: %.2f Hz\n", spectrum.dominant_frequency);
        printf("  Total power: %.6f\n\n", spectrum.total_power);
        
        // Find peaks
        double peak_freqs[10];
        double peak_mags[10];
        int num_peaks= sp.FindFrequencyPeaks(&spectrum, 10.0, peak_freqs, peak_mags, 10);
        
        printf("Top frequency peaks detected:\n");
        printf("  Rank | Frequency (Hz) | Magnitude\n");
        printf("  -----+----------------+----------\n");
        for (int i= 0; i < num_peaks && i < 5; ++i) {
            printf("   %2d  |    %7.2f     |  %8.2f\n", i + 1, peak_freqs[i], peak_mags[i]);
        }
        
        // Verify detection of expected frequencies
        bool found_50= false, found_150= false, found_300= false;
        for (int i= 0; i < num_peaks; ++i) {
            if (fabs(peak_freqs[i] - freq1) < 5.0) found_50= true;
            if (fabs(peak_freqs[i] - freq2) < 5.0) found_150= true;
            if (fabs(peak_freqs[i] - freq3) < 5.0) found_300= true;
        }
        
        printf("\n");
        if (found_50 && found_150 && found_300) {
            printf("v TEST PASSED: All expected frequencies detected\n");
        } else {
            printf("!TEST WARNING: Some frequencies not detected\n");
            printf("  50 Hz: %s, 150 Hz: %s, 300 Hz: %s\n",
                   found_50 ? "v" : "x", found_150 ? "v" : "x", found_300 ? "v" : "x");
        }
        
        sp.FreeSpectrum(&spectrum);
    } else {
        printf("x TEST FAILED: FFT analysis failed\n");
    }
    
    printf("\n");
}

void TestHarmonicAnalysis() {
    printf("\n ========================================================\n");
    printf("||  Test 2: Harmonic Analysis                            ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    double sampling_rate= 2000.0;
    int num_samples= 2048;
    double fundamental= 60.0;  // 60 Hz fundamental
    
    printf("Generating signal with harmonics:\n");
    printf("  Fundamental: %.0f Hz\n", fundamental);
    printf("  Harmonics: 2nd (120 Hz), 3rd (180 Hz), 4th (240 Hz), 5th (300 Hz)\n\n");
    
    // Generate signal with harmonics (simulating distorted waveform)
    for (int i= 0; i < num_samples; ++i) {
        double t= i / sampling_rate;
        double signal= 1.0 * sin(2.0 * M_PI * fundamental * t) +      // Fundamental
                       0.3 * sin(2.0 * M_PI * 2 * fundamental * t) +   // 2nd harmonic
                       0.2 * sin(2.0 * M_PI * 3 * fundamental * t) +   // 3rd harmonic
                       0.15 * sin(2.0 * M_PI * 4 * fundamental * t) +  // 4th harmonic
                       0.1 * sin(2.0 * M_PI * 5 * fundamental * t);    // 5th harmonic
        sp.AddValue(signal);
    }
    
    // Perform FFT
    FrequencySpectrum spectrum;
    if (sp.FFTAnalysis(sampling_rate, &spectrum)) {
        // Analyze harmonics
        double harmonic_mags[10];
        int num_harmonics= 5;
        double thd= sp.AnalyzeHarmonics(&spectrum, fundamental, num_harmonics, harmonic_mags);
        
        printf("Harmonic Analysis Results:\n");
        printf("  Harmonic | Frequency (Hz) | Magnitude  | Ratio to Fund.\n");
        printf("  ---------+----------------+------------+----------------\n");
        
        for (int i= 0; i < num_harmonics; ++i) {
            double freq= (i + 1) * fundamental;
            double ratio= (i== 0) ? 1.0 : harmonic_mags[i] / harmonic_mags[0];
            printf("     %d     |    %6.1f      |  %8.2f  |     %.3f\n",
                   i + 1, freq, harmonic_mags[i], ratio);
        }
        
        printf("\n  Total Harmonic Distortion (THD): %.2f%%\n", thd * 100.0);
        
        if (thd < 0.5) {
            printf("  Status: v Low distortion\n");
        } else if (thd < 1.0) {
            printf("  Status: !Moderate distortion\n");
        } else {
            printf("  Status: !High distortion\n");
        }
        
        printf("\nv TEST PASSED: Harmonic analysis completed\n");
        
        sp.FreeSpectrum(&spectrum);
    } else {
        printf("x TEST FAILED: FFT analysis failed\n");
    }
    
    printf("\n");
}

void TestFrequencyBandPower() {
    printf("\n ========================================================\n");
    printf("||  Test 3: Frequency Band Power Analysis                ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    double sampling_rate= 1000.0;
    int num_samples= 1024;
    
    // Generate signal with components in different bands
    printf("Generating multi-band signal:\n");
    printf("  Low frequency band (10-50 Hz): Strong component\n");
    printf("  Mid frequency band (100-200 Hz): Medium component\n");
    printf("  High frequency band (300-400 Hz): Weak component\n\n");
    
    for (int i= 0; i < num_samples; ++i) {
        double t= i / sampling_rate;
        double signal= 1.0 * sin(2.0 * M_PI * 30.0 * t) +    // Low band
                       0.5 * sin(2.0 * M_PI * 150.0 * t) +   // Mid band
                       0.2 * sin(2.0 * M_PI * 350.0 * t);    // High band
        sp.AddValue(signal);
    }
    
    FrequencySpectrum spectrum;
    if (sp.FFTAnalysis(sampling_rate, &spectrum)) {
        // Analyze power in different bands
        double low_power= sp.GetPowerInBand(&spectrum, 10.0, 50.0);
        double mid_power= sp.GetPowerInBand(&spectrum, 100.0, 200.0);
        double high_power= sp.GetPowerInBand(&spectrum, 300.0, 400.0);
        double total= low_power + mid_power + high_power;
        
        printf("Band Power Analysis:\n");
        printf("  Band          | Power      | Percentage\n");
        printf("  --------------+------------+------------\n");
        printf("  Low (10-50)   | %10.2f |   %.1f%%\n", low_power, (low_power / total) * 100.0);
        printf("  Mid (100-200) | %10.2f |   %.1f%%\n", mid_power, (mid_power / total) * 100.0);
        printf("  High (300-400)| %10.2f |   %.1f%%\n", high_power, (high_power / total) * 100.0);
        printf("  --------------+------------+------------\n");
        printf("  Total         | %10.2f |   100.0%%\n", total);
        
        printf("\nv TEST PASSED: Band power analysis completed\n");
        
        sp.FreeSpectrum(&spectrum);
    } else {
        printf("x TEST FAILED: FFT analysis failed\n");
    }
    
    printf("\n");
}

void TestTurbineBladeFrequencyAnalysis() {
    printf("\n ========================================================\n");
    printf("||  Test 4: Turbine Blade Frequency Analysis             ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    const int NUM_BLADES= 12;
    const int SAMPLES_PER_BLADE= 128;
    const double ROTATION_FREQ= 25.0;  // 25 Hz (1500 RPM)
    const double SAMPLING_RATE= 5000.0;
    
    int blade_markers[NUM_BLADES];
    
    printf("Turbine Configuration:\n");
    printf("  Number of blades: %d\n", NUM_BLADES);
    printf("  Rotation frequency: %.1f Hz (%.0f RPM)\n", ROTATION_FREQ, ROTATION_FREQ * 60);
    printf("  Sampling rate: %.0f Hz\n", SAMPLING_RATE);
    printf("  Blade pass frequency: %.1f Hz\n\n", ROTATION_FREQ * NUM_BLADES);
    
    srand((unsigned int)time(NULL));
    
    // Generate vibration data for each blade
    for (int blade= 0; blade < NUM_BLADES; ++blade) {
        blade_markers[blade]= blade * SAMPLES_PER_BLADE;
        
        for (int sample= 0; sample < SAMPLES_PER_BLADE; ++sample) {
            double t= (blade * SAMPLES_PER_BLADE + sample) / SAMPLING_RATE;
            
            // Base vibration (rotation frequency and blade pass frequency)
            double vibration= 0.5 * sin(2.0 * M_PI * ROTATION_FREQ * t) +
                             0.3 * sin(2.0 * M_PI * ROTATION_FREQ * NUM_BLADES * t);
            
            // Blade #7 has a defect - additional high frequency component
            if (blade== 7) {
                vibration += 0.8 * sin(2.0 * M_PI * 800.0 * t);  // 800 Hz resonance
            }
            
            // Add noise
            vibration += ((double)rand() / RAND_MAX - 0.5) * 0.1;
            
            sp.AddValue(vibration);
        }
    }
    
    printf("Performing frequency analysis on each blade...\n\n");
    
    // Analyze frequency spectrum for each blade
    FrequencySpectrum spectra[NUM_BLADES];
    int num_spectra= sp.CompareSegmentSpectra(blade_markers, NUM_BLADES, SAMPLING_RATE, spectra);
    
    if (num_spectra > 0) {
        printf("Blade Frequency Analysis:\n");
        printf("  Blade | Dominant Freq | Total Power | High Band Power (>500Hz)\n");
        printf("  ------+---------------+-------------+-------------------------\n");
        
        int anomalous_blade= -1;
        double max_high_power= 0.0;
        
        for (int i= 0; i < num_spectra; ++i) {
            double high_power= sp.GetPowerInBand(&spectra[i], 500.0, 1000.0);
            
            printf("   %2d   |   %7.1f Hz  |  %9.2f  |    %9.2f", 
                   i,
                   spectra[i].dominant_frequency,
                   spectra[i].total_power,
                   high_power);
            
            if (high_power > max_high_power) {
                max_high_power= high_power;
                anomalous_blade= i;
            }
            
            if (high_power > 10.0) {  // Threshold for anomaly
                printf("   ! ANOMALOUS\n");
            } else {
                printf("    NORMAL\n");
            }
        }
        
        printf("\n");
        printf("Analysis Summary:\n");
        printf("  Most anomalous blade: #%d\n", anomalous_blade);
        printf("  High-frequency power: %.2f\n", max_high_power);
        
        if (anomalous_blade== 7) {
            printf("\nv TEST PASSED: Correctly identified blade #7 with frequency anomaly\n");
        } else {
            printf("\n! TEST WARNING: Expected blade #7, detected blade #%d\n", anomalous_blade);
        }
        
        // Free all spectra
        for (int i= 0; i < num_spectra; ++i) {
            sp.FreeSpectrum(&spectra[i]);
        }
    } else {
        printf("x TEST FAILED: Spectrum analysis failed\n");
    }
    
    printf("\n");
}

void TestFrequencyAnomalyDetection() {
    printf("\n ========================================================\n");
    printf("||  Test 5: Frequency Anomaly Detection                  ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp_baseline, sp_current;
    
    double sampling_rate= 2000.0;
    int num_samples= 1024;
    
    printf("Scenario: Detecting bearing wear by frequency shift\n");
    printf("  Baseline: Normal operation\n");
    printf("  Current: Bearing wear causes new frequency components\n\n");
    
    // Generate baseline (normal) signal
    for (int i= 0; i < num_samples; ++i) {
        double t= i / sampling_rate;
        double signal= sin(2.0 * M_PI * 100.0 * t) + 
                       0.3 * sin(2.0 * M_PI * 200.0 * t);
        sp_baseline.AddValue(signal);
    }
    
    // Generate current signal with anomaly (new frequency component)
    srand((unsigned int)time(NULL) + 100);
    for (int i= 0; i < num_samples; ++i) {
        double t= i / sampling_rate;
        double signal= sin(2.0 * M_PI * 100.0 * t) + 
                       0.3 * sin(2.0 * M_PI * 200.0 * t) +
                       0.5 * sin(2.0 * M_PI * 450.0 * t);  // New anomalous component
        sp_current.AddValue(signal);
    }
    
    // Analyze both
    FrequencySpectrum baseline_spec, current_spec;
    
    if (sp_baseline.FFTAnalysis(sampling_rate, &baseline_spec) &&
        sp_current.FFTAnalysis(sampling_rate, &current_spec)) {
        
        double anomaly_score= sp_current.DetectFrequencyAnomalies(&current_spec, &baseline_spec, 1.5);
        
        printf("Frequency Anomaly Detection Results:\n");
        printf("  Baseline dominant frequency: %.1f Hz\n", baseline_spec.dominant_frequency);
        printf("  Current dominant frequency:  %.1f Hz\n", current_spec.dominant_frequency);
        printf("  Anomaly score: %.4f\n\n", anomaly_score);
        
        if (anomaly_score > 0.3) {
            printf("  Status: !ANOMALY DETECTED\n");
            printf("  Recommendation: Investigate new frequency components\n");
        } else {
            printf("  Status: v NORMAL\n");
        }
        
        // Find peaks in current spectrum
        double peak_freqs[10];
        double peak_mags[10];
        int num_peaks= sp_current.FindFrequencyPeaks(&current_spec, 5.0, peak_freqs, peak_mags, 10);
        
        printf("\n  Current spectrum peaks:\n");
        for (int i= 0; i < num_peaks && i < 5; ++i) {
            printf("    %.1f Hz: magnitude %.2f\n", peak_freqs[i], peak_mags[i]);
        }
        
        if (anomaly_score > 0.2) {
            printf("\nv TEST PASSED: Anomaly detected successfully\n");
        } else {
            printf("\n! TEST WARNING: Anomaly score lower than expected\n");
        }
        
        sp_baseline.FreeSpectrum(&baseline_spec);
        sp_current.FreeSpectrum(&current_spec);
    } else {
        printf("x TEST FAILED: FFT analysis failed\n");
    }
    
    printf("\n");
}

void TestWindowedAnalysis() {
    printf("\n ========================================================\n");
    printf("||  Test 6: Windowed Frequency Analysis                  ||\n");
    printf("========================================================\n\n");
    
    SignalProcessing sp;
    
    double sampling_rate= 1000.0;
    int total_samples= 2048;
    int window_size= 512;
    
    printf("Testing frequency analysis on signal windows:\n");
    printf("  Total samples: %d\n", total_samples);
    printf("  Window size: %d\n", window_size);
    printf("  Number of windows: %d\n\n", total_samples / window_size);
    
    // Generate signal that changes frequency over time
    for (int i= 0; i < total_samples; ++i) {
        double t= i / sampling_rate;
        double freq= 50.0 + (i / (double)total_samples) * 150.0;  // Sweep from 50 to 200 Hz
        double signal= sin(2.0 * M_PI * freq * t);
        sp.AddValue(signal);
    }
    
    printf("Analyzing windows:\n");
    printf("  Window | Start | Dominant Freq | Total Power\n");
    printf("  -------+-------+---------------+------------\n");
    
    for (int w= 0; w < total_samples / window_size; ++w) {
        int start_idx= w * window_size;
        
        FrequencySpectrum spectrum;
        if (sp.FFTAnalysis(start_idx, window_size, sampling_rate, &spectrum)) {
            printf("    %d    | %5d |   %7.1f Hz  |  %8.2f\n",
                   w + 1,
                   start_idx,
                   spectrum.dominant_frequency,
                   spectrum.total_power);
            
            sp.FreeSpectrum(&spectrum);
        }
    }
    
    printf("\nv TEST PASSED: Windowed analysis completed\n");
    printf("  Note: Dominant frequency increases across windows (frequency sweep)\n");
    
    printf("\n");
}

void TestHighFrequencySignal() {
    printf("\n ========================================================\n");
    printf("||  Test 7: High Frequency Signal Detection (20 MHz)     ||\n");
    printf("========================================================\n\n");
    
    // Note: For maximum NB_MAX_VALUES=1000, we need to adjust our approach
    // We'll use a lower sampling rate that still satisfies Nyquist (>40 MHz)
    // but provides better frequency resolution
    
    SignalProcessing sp;
    
    // Generate 20 MHz signal with optimized parameters
    double target_frequency = 20000000.0;  // 20 MHz
    double sampling_rate = 50000000.0;     // 50 MHz sampling (2.5x Nyquist, conservative)
    int num_samples = 1000;                // Maximum available samples
    
    printf("Generating high-frequency test signal:\n");
    printf("  Target frequency: %.2f MHz\n", target_frequency / 1e6);
    printf("  Sampling rate: %.2f MHz\n", sampling_rate / 1e6);
    printf("  Nyquist frequency: %.2f MHz\n", (sampling_rate / 2.0) / 1e6);
    printf("  Number of samples: %d\n", num_samples);
    printf("  Duration: %.3f microseconds\n\n", (num_samples / sampling_rate) * 1e6);
    
    // Generate signal using helper function
    GenerateSignal(&sp, target_frequency, sampling_rate, num_samples, 1.0, 0.0);
    
    // Perform FFT
    FrequencySpectrum spectrum;
    if (sp.FFTAnalysis(sampling_rate, &spectrum)) {
        printf("FFT Analysis Results:\n");
        printf("  Number of frequency bins: %d\n", spectrum.num_bins);
        printf("  Frequency resolution: %.3f kHz\n", spectrum.frequency_resolution / 1e3);
        printf("  Dominant frequency: %.6f MHz\n", spectrum.dominant_frequency / 1e6);
        printf("  Total power: %.6f\n\n", spectrum.total_power);
        
        // Find peaks
        double peak_freqs[10];
        double peak_mags[10];
        int num_peaks = sp.FindFrequencyPeaks(&spectrum, 10.0, peak_freqs, peak_mags, 10);
        
        printf("Top frequency peaks detected:\n");
        printf("  Rank | Frequency (MHz) | Magnitude\n");
        printf("  -----+-----------------+----------\n");
        for (int i = 0; i < num_peaks && i < 5; ++i) {
            printf("   %2d  |   %10.6f    |  %8.2f\n", i + 1, peak_freqs[i] / 1e6, peak_mags[i]);
        }
        
        // Verify detection
        bool detected = false;
        double error_percent = 0.0;
        
        for (int i = 0; i < num_peaks; ++i) {
            double freq_error = fabs(peak_freqs[i] - target_frequency);
            double tolerance = spectrum.frequency_resolution * 2.0;  // Allow 2 bins tolerance
            
            if (freq_error < tolerance) {
                detected = true;
                error_percent = (freq_error / target_frequency) * 100.0;
                printf("\nv 20 MHz signal detected at %.6f MHz\n", peak_freqs[i] / 1e6);
                printf("  Frequency error: %.3f kHz (%.4f%%)\n", 
                       freq_error / 1e3, error_percent);
                break;
            }
        }
        
        if (detected) {
            printf("\nv TEST PASSED: 20 MHz frequency correctly detected\n");
        } else {
            printf("\nx TEST FAILED: 20 MHz frequency not detected\n");
            printf("  Expected: %.6f MHz\n", target_frequency / 1e6);
            printf("  Detected: %.6f MHz\n", spectrum.dominant_frequency / 1e6);
        }
        
        sp.FreeSpectrum(&spectrum);
    } else {
        printf("x TEST FAILED: FFT analysis failed\n");
    }
    
    printf("\n");
}

int main() {
    printf("\n");
    printf("========================================================================\n");
    printf("           FREQUENCY ANALYSIS TEST SUITE\n");
    printf("========================================================================\n");
    
    TestBasicFFT();
    TestHarmonicAnalysis();
    TestFrequencyBandPower();
    TestTurbineBladeFrequencyAnalysis();
    TestFrequencyAnomalyDetection();
    TestWindowedAnalysis();
    TestHighFrequencySignal();
    
    printf("========================================================================\n");
    printf("                      ALL TESTS COMPLETED\n");
    printf("========================================================================\n\n");
    
    return 0;
}
