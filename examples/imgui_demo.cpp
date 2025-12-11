// SignalProcessing ImGui Demo
// Demonstrates real-time signal processing with interactive visualization
// Requires: ImGui, ImPlot, GLFW, OpenGL

#include "../source/SignalProcessing.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global state
SignalProcessing sp;
std::vector<double> signal_data;
std::vector<double> filtered_data;
std::vector<double> fft_frequencies;
std::vector<double> fft_magnitudes;
int signal_length = 500;
double sampling_rate = 100.0;

// UI state
enum SignalType { SIGNAL_SINE, SIGNAL_ECG, SIGNAL_TURBINE, SIGNAL_NOISE };
int current_signal = SIGNAL_ECG;
float noise_level = 0.1f;
float frequency = 1.2f;

// Filter state
bool apply_kalman = false;
bool apply_median = false;
bool apply_wavelet = false;
float kalman_process = 0.01f;
float kalman_measure = 0.1f;
int median_window = 5;

// Anomaly detection
bool show_anomalies = false;
int anomaly_method = 0; // 0=ZScore, 1=IQR, 2=MAD
float anomaly_threshold = 3.0f;
std::vector<int> anomaly_indices;

// ML Features
bool show_ml_features = false;
MLFeatureVector ml_features;

// Generate signal based on type
void GenerateSignal() {
    sp.ClearVector();
    signal_data.clear();
    
    for (int i = 0; i < signal_length; i++) {
        double t = (double)i / sampling_rate;
        double value = 0.0;
        
        switch (current_signal) {
            case SIGNAL_SINE:
                value = sin(2 * M_PI * frequency * t);
                break;
                
            case SIGNAL_ECG: {
                // Simplified ECG
                double phase = fmod(t * frequency * 2 * M_PI, 2 * M_PI);
                if (phase < M_PI / 4) {
                    value = 0.3 * sin(phase * 4); // P-wave
                } else if (phase > M_PI / 2 && phase < M_PI * 0.7) {
                    value = 1.5 * sin((phase - M_PI/2) * 10); // QRS
                } else if (phase > M_PI && phase < M_PI * 1.5) {
                    value = 0.4 * sin((phase - M_PI) * 2); // T-wave
                }
                break;
            }
            
            case SIGNAL_TURBINE:
                // Turbine with harmonics
                value = 1.0 * sin(2 * M_PI * 50 * t);           // Fundamental
                value += 0.3 * sin(2 * M_PI * 50 * t + M_PI/4); // Imbalance
                value += 0.2 * sin(2 * M_PI * 500 * t);         // Bearing
                break;
                
            case SIGNAL_NOISE:
                value = ((double)rand() / RAND_MAX - 0.5) * 2.0;
                break;
        }
        
        // Add noise
        value += ((double)rand() / RAND_MAX - 0.5) * noise_level;
        
        signal_data.push_back(value);
        sp.AddValue(value);
    }
}

// Apply filters
void ApplyFilters() {
    filtered_data = signal_data;
    
    if (apply_kalman) {
        double* temp = new double[signal_length];
        sp.KalmanFilter(kalman_process, kalman_measure, temp);
        for (int i = 0; i < sp.GetIndex(); i++) {
            filtered_data[i] = temp[i];
        }
        delete[] temp;
    }
    
    if (apply_median) {
        SignalProcessing sp_temp;
        for (size_t i = 0; i < filtered_data.size(); i++) {
            sp_temp.AddValue(filtered_data[i]);
        }
        double* temp = new double[signal_length];
        sp_temp.MedianFilter(median_window, temp);
        for (int i = 0; i < sp_temp.GetIndex(); i++) {
            filtered_data[i] = temp[i];
        }
        delete[] temp;
    }
    
    if (apply_wavelet) {
        SignalProcessing sp_temp;
        for (size_t i = 0; i < filtered_data.size(); i++) {
            sp_temp.AddValue(filtered_data[i]);
        }
        double* temp = new double[signal_length];
        sp_temp.WaveletDenoise(0.5, temp, 1);
        for (int i = 0; i < sp_temp.GetIndex(); i++) {
            filtered_data[i] = temp[i];
        }
        delete[] temp;
    }
}

// Detect anomalies
void DetectAnomalies() {
    anomaly_indices.clear();
    
    SignalProcessing sp_check;
    for (size_t i = 0; i < filtered_data.size(); i++) {
        sp_check.AddValue(filtered_data[i]);
    }
    
    int temp_anomalies[1000];
    int count = 0;
    
    switch (anomaly_method) {
        case 0:
            count = sp_check.DetectAnomaliesZScore(anomaly_threshold, temp_anomalies, 1000);
            break;
        case 1:
            count = sp_check.DetectAnomaliesIQR(anomaly_threshold, temp_anomalies, 1000);
            break;
        case 2:
            count = sp_check.DetectAnomaliesMAD(20, anomaly_threshold, temp_anomalies, 1000);
            break;
    }
    
    for (int i = 0; i < count; i++) {
        anomaly_indices.push_back(temp_anomalies[i]);
    }
}

// Compute FFT
void ComputeFFT() {
    fft_frequencies.clear();
    fft_magnitudes.clear();
    
    SignalProcessing sp_fft;
    for (size_t i = 0; i < filtered_data.size(); i++) {
        sp_fft.AddValue(filtered_data[i]);
    }
    
    FrequencySpectrum spectrum;
    if (sp_fft.FFTAnalysis(sampling_rate, &spectrum)) {
        for (int i = 0; i < spectrum.num_bins && i < 100; i++) {
            fft_frequencies.push_back(spectrum.bins[i].frequency);
            fft_magnitudes.push_back(spectrum.bins[i].magnitude);
        }
        sp_fft.FreeSpectrum(&spectrum);
    }
}

// Extract ML features
void ExtractMLFeatures() {
    SignalProcessing sp_ml;
    for (size_t i = 0; i < filtered_data.size(); i++) {
        sp_ml.AddValue(filtered_data[i]);
    }
    sp_ml.ExtractMLFeatures(sampling_rate, &ml_features);
}

int main(int, char**) {
    // Initialize GLFW
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window
    GLFWwindow* window = glfwCreateWindow(1600, 900, "SignalProcessing Interactive Demo", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Generate initial signal
    GenerateSignal();
    ApplyFilters();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("SignalProcessing Demo", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Left panel - Controls
        ImGui::BeginChild("Controls", ImVec2(400, 0), true);
        
        ImGui::Text("📊 Signal Generator");
        ImGui::Separator();
        
        const char* signal_types[] = { "Sine Wave", "ECG Signal", "Turbine Vibration", "White Noise" };
        if (ImGui::Combo("Signal Type", &current_signal, signal_types, 4)) {
            GenerateSignal();
            ApplyFilters();
        }
        
        ImGui::SliderFloat("Frequency (Hz)", &frequency, 0.1f, 10.0f);
        ImGui::SliderFloat("Noise Level", &noise_level, 0.0f, 1.0f);
        ImGui::SliderInt("Signal Length", &signal_length, 100, 1000);
        
        if (ImGui::Button("🔄 Regenerate Signal", ImVec2(-1, 0))) {
            GenerateSignal();
            ApplyFilters();
        }
        
        ImGui::Spacing();
        ImGui::Text("🔧 Filters");
        ImGui::Separator();
        
        if (ImGui::Checkbox("Kalman Filter", &apply_kalman)) {
            ApplyFilters();
        }
        if (apply_kalman) {
            ImGui::SliderFloat("Process Noise", &kalman_process, 0.001f, 0.1f, "%.3f");
            ImGui::SliderFloat("Measure Noise", &kalman_measure, 0.01f, 1.0f, "%.3f");
        }
        
        if (ImGui::Checkbox("Median Filter", &apply_median)) {
            ApplyFilters();
        }
        if (apply_median) {
            if (ImGui::SliderInt("Window Size", &median_window, 3, 11)) {
                if (median_window % 2 == 0) median_window++; // Keep odd
            }
        }
        
        if (ImGui::Checkbox("Wavelet Denoise", &apply_wavelet)) {
            ApplyFilters();
        }
        
        if (ImGui::Button("🔧 Apply Filters", ImVec2(-1, 0))) {
            ApplyFilters();
        }
        
        ImGui::Spacing();
        ImGui::Text("⚠ Anomaly Detection");
        ImGui::Separator();
        
        ImGui::Checkbox("Show Anomalies", &show_anomalies);
        const char* anomaly_methods[] = { "Z-Score", "IQR", "MAD" };
        ImGui::Combo("Method", &anomaly_method, anomaly_methods, 3);
        ImGui::SliderFloat("Threshold", &anomaly_threshold, 1.0f, 5.0f);
        
        if (ImGui::Button("🔍 Detect Anomalies", ImVec2(-1, 0))) {
            DetectAnomalies();
        }
        
        if (!anomaly_indices.empty()) {
            ImGui::Text("Found %d anomalies", (int)anomaly_indices.size());
        }
        
        ImGui::Spacing();
        ImGui::Text("📈 Analysis");
        ImGui::Separator();
        
        if (ImGui::Button("⚡ Compute FFT", ImVec2(-1, 0))) {
            ComputeFFT();
        }
        
        if (ImGui::Checkbox("Show ML Features", &show_ml_features)) {
            if (show_ml_features) {
                ExtractMLFeatures();
            }
        }
        
        // Statistics
        ImGui::Spacing();
        ImGui::Text("📊 Statistics");
        ImGui::Separator();
        ImGui::Text("Mean: %.4f", sp.GetMean());
        ImGui::Text("Std Dev: %.4f", sp.GetStandardDeviation());
        ImGui::Text("RMS: %.4f", ml_features.rms);
        
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Right panel - Plots
        ImGui::BeginChild("Plots");
        
        // Time domain plot
        if (ImPlot::BeginPlot("Signal - Time Domain", ImVec2(-1, 300))) {
            ImPlot::SetupAxes("Sample", "Amplitude");
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, signal_length, ImGuiCond_Always);
            
            // Plot original signal
            std::vector<double> x_vals(signal_data.size());
            for (size_t i = 0; i < x_vals.size(); i++) x_vals[i] = i;
            
            ImPlot::PlotLine("Original", x_vals.data(), signal_data.data(), signal_data.size());
            
            // Plot filtered signal
            if (apply_kalman || apply_median || apply_wavelet) {
                ImPlot::PlotLine("Filtered", x_vals.data(), filtered_data.data(), filtered_data.size());
            }
            
            // Plot anomalies
            if (show_anomalies && !anomaly_indices.empty()) {
                std::vector<double> anomaly_x, anomaly_y;
                for (int idx : anomaly_indices) {
                    if (idx < (int)filtered_data.size()) {
                        anomaly_x.push_back(idx);
                        anomaly_y.push_back(filtered_data[idx]);
                    }
                }
                if (!anomaly_x.empty()) {
                    ImPlot::PlotScatter("Anomalies", anomaly_x.data(), anomaly_y.data(), anomaly_x.size());
                }
            }
            
            ImPlot::EndPlot();
        }
        
        // FFT plot
        if (!fft_frequencies.empty()) {
            if (ImPlot::BeginPlot("Frequency Spectrum", ImVec2(-1, 300))) {
                ImPlot::SetupAxes("Frequency (Hz)", "Magnitude");
                ImPlot::PlotLine("FFT", fft_frequencies.data(), fft_magnitudes.data(), fft_frequencies.size());
                ImPlot::EndPlot();
            }
        }
        
        // ML Features panel
        if (show_ml_features) {
            ImGui::BeginChild("MLFeatures", ImVec2(-1, 250), true);
            ImGui::Text("🤖 ML Features (21 total)");
            ImGui::Separator();
            
            ImGui::Columns(3, "MLColumns");
            
            ImGui::Text("Statistical");
            ImGui::Separator();
            ImGui::Text("Mean: %.4f", ml_features.mean);
            ImGui::Text("Std Dev: %.4f", ml_features.std_dev);
            ImGui::Text("Variance: %.4f", ml_features.variance);
            ImGui::Text("Skewness: %.4f", ml_features.skewness);
            ImGui::Text("Kurtosis: %.4f", ml_features.kurtosis);
            ImGui::Text("RMS: %.4f", ml_features.rms);
            ImGui::Text("Peak-Peak: %.4f", ml_features.peak_to_peak);
            ImGui::Text("Crest: %.4f", ml_features.crest_factor);
            
            ImGui::NextColumn();
            
            ImGui::Text("Frequency");
            ImGui::Separator();
            ImGui::Text("Dom Freq: %.2f Hz", ml_features.dominant_frequency);
            ImGui::Text("Centroid: %.2f Hz", ml_features.spectral_centroid);
            ImGui::Text("Spread: %.2f Hz", ml_features.spectral_spread);
            ImGui::Text("Entropy: %.4f", ml_features.spectral_entropy);
            ImGui::Text("Power: %.2f", ml_features.total_power);
            ImGui::Text("Low: %.2f", ml_features.power_low_freq);
            ImGui::Text("Mid: %.2f", ml_features.power_mid_freq);
            ImGui::Text("High: %.2f", ml_features.power_high_freq);
            
            ImGui::NextColumn();
            
            ImGui::Text("Time Domain");
            ImGui::Separator();
            ImGui::Text("Zero Cross: %.4f", ml_features.zero_crossing_rate);
            ImGui::Text("Mean Cross: %.4f", ml_features.mean_crossing_rate);
            ImGui::Text("Energy: %.2f", ml_features.energy);
            ImGui::Text("Autocorr: %.4f s", ml_features.autocorr_peak);
            
            ImGui::Columns(1);
            ImGui::EndChild();
        }
        
        ImGui::EndChild();
        
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
