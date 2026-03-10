// SignalProcessing ImGui Demo
// Demonstrates real-time signal processing with interactive visualization
// Requires: ImGui, ImPlot, GLFW, OpenGL

#include "../source/SignalProcessing.h"
#include "../source/SignalRecorder.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <ctime>
#include <string>
#include <unistd.h>
#include <limits.h>
#include "ImGuiConfig.hpp"

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

// HDF5 Recording
bool show_save_dialog = false;
char save_filename[256] = "signal_recording.h5";
char session_info[256] = "Interactive ImGui Session";
bool save_success = false;
bool save_error = false;
char save_message[512] = "";
// HDF5 Loading
bool show_load_dialog = false;
char load_filename[256] = "signal_recording.h5";
bool load_success = false;
bool load_error = false;
char load_message[512] = "";
std::vector<double> loaded_signal;
std::vector<double> loaded_filtered;
std::vector<int> loaded_anomalies;
std::vector<double> loaded_fft_freq;
std::vector<double> loaded_fft_mag;
char loaded_info[512] = "";
bool show_loaded_data = false;

// Configuration management
ImGuiConfigManager config_manager("imgui_app.ini");
ImGuiAppConfig app_config = ImGuiConfigManager::GetDefaultConfig();

// Forward declarations
void GenerateSignal();
void ApplyFilters();

// Initialize configuration from saved file
void InitializeConfiguration() {
    // Try to load existing configuration
    if (config_manager.LoadConfig(app_config) == 0) {
        // Apply loaded configuration to UI variables
        current_signal = app_config.signal_type;
        noise_level = app_config.noise_level;
        frequency = app_config.frequency;
        signal_length = app_config.signal_length;
        sampling_rate = app_config.sampling_rate;
        
        apply_kalman = app_config.apply_kalman != 0;
        apply_median = app_config.apply_median != 0;
        apply_wavelet = app_config.apply_wavelet != 0;
        kalman_process = app_config.kalman_process;
        kalman_measure = app_config.kalman_measure;
        median_window = app_config.median_window;
        
        show_anomalies = app_config.show_anomalies != 0;
        anomaly_method = app_config.anomaly_method;
        anomaly_threshold = app_config.anomaly_threshold;
        
        show_ml_features = app_config.show_ml_features != 0;
        
        strncpy(save_filename, app_config.last_save_path, 255);
        strncpy(load_filename, app_config.last_load_path, 255);
        strncpy(session_info, app_config.session_info, 255);
        
        printf("✓ Configuration restored from previous session\n");
    } else {
        printf("Using default configuration\n");
    }
}

// Save configuration to file
void SaveConfiguration() {
    // Update config structure from UI variables
    app_config.signal_type = current_signal;
    app_config.noise_level = noise_level;
    app_config.frequency = frequency;
    app_config.signal_length = signal_length;
    app_config.sampling_rate = sampling_rate;
    
    app_config.apply_kalman = apply_kalman ? 1 : 0;
    app_config.apply_median = apply_median ? 1 : 0;
    app_config.apply_wavelet = apply_wavelet ? 1 : 0;
    app_config.kalman_process = kalman_process;
    app_config.kalman_measure = kalman_measure;
    app_config.median_window = median_window;
    
    app_config.show_anomalies = show_anomalies ? 1 : 0;
    app_config.anomaly_method = anomaly_method;
    app_config.anomaly_threshold = anomaly_threshold;
    
    app_config.show_ml_features = show_ml_features ? 1 : 0;
    
    strncpy(app_config.last_save_path, save_filename, 255);
    strncpy(app_config.last_load_path, load_filename, 255);
    strncpy(app_config.session_info, session_info, 255);
    
    config_manager.SaveConfig(app_config);
}

// Render configuration UI
void RenderConfigurationUI() {
    if (ImGui::CollapsingHeader("Configuration Management")) {
        ImGui::Text("Save/load application settings");
        ImGui::Spacing();
        
        if (ImGui::Button("Save Settings", ImVec2(180, 0))) {
            SaveConfiguration();
            ImGui::OpenPopup("Config Saved");
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Load Settings", ImVec2(180, 0))) {
            if (config_manager.LoadConfig(app_config) == 0) {
                // Apply loaded configuration
                current_signal = app_config.signal_type;
                noise_level = app_config.noise_level;
                frequency = app_config.frequency;
                signal_length = app_config.signal_length;
                sampling_rate = app_config.sampling_rate;
                
                apply_kalman = app_config.apply_kalman != 0;
                apply_median = app_config.apply_median != 0;
                apply_wavelet = app_config.apply_wavelet != 0;
                kalman_process = app_config.kalman_process;
                kalman_measure = app_config.kalman_measure;
                median_window = app_config.median_window;
                
                show_anomalies = app_config.show_anomalies != 0;
                anomaly_method = app_config.anomaly_method;
                anomaly_threshold = app_config.anomaly_threshold;
                
                show_ml_features = app_config.show_ml_features != 0;
                
                strncpy(save_filename, app_config.last_save_path, 255);
                strncpy(load_filename, app_config.last_load_path, 255);
                strncpy(session_info, app_config.session_info, 255);
                
                GenerateSignal();
                ImGui::OpenPopup("Config Loaded");
            }
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Reset Defaults", ImVec2(180, 0))) {
            app_config = ImGuiConfigManager::GetDefaultConfig();
            SaveConfiguration();
            InitializeConfiguration();
            GenerateSignal();
            ImGui::OpenPopup("Config Reset");
        }
        
        // Popup notifications
        if (ImGui::BeginPopup("Config Saved")) {
            ImGui::Text("Configuration saved!");
            ImGui::EndPopup();
        }
        
        if (ImGui::BeginPopup("Config Loaded")) {
            ImGui::Text("Configuration loaded!");
            ImGui::EndPopup();
        }
        
        if (ImGui::BeginPopup("Config Reset")) {
            ImGui::Text("Configuration reset to defaults!");
            ImGui::EndPopup();
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("Config file: imgui_app.ini");
    }
}

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

// Save signal to HDF5
void SaveSignalToHDF5() {
    try {
        SignalRecorder recorder(save_filename);
        
        // Add session metadata
        time_t now = time(0);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        recorder.addMetadata("Session", "timestamp", timestamp);
        recorder.addMetadata("Session", "info", session_info);
        
        // Signal parameters
        const char* signal_names[] = { "Sine Wave", "ECG Signal", "Turbine Vibration", "White Noise" };
        recorder.addMetadata("Parameters", "signal_type", signal_names[current_signal]);
        
        char param_str[128];
        sprintf(param_str, "%.2f", frequency);
        recorder.addMetadata("Parameters", "frequency_hz", param_str);
        sprintf(param_str, "%.2f", noise_level);
        recorder.addMetadata("Parameters", "noise_level", param_str);
        sprintf(param_str, "%d", signal_length);
        recorder.addMetadata("Parameters", "signal_length", param_str);
        sprintf(param_str, "%.1f", sampling_rate);
        recorder.addMetadata("Parameters", "sampling_rate_hz", param_str);
        
        // Save original signal
        std::vector<float> signal_float(signal_data.begin(), signal_data.end());
        recorder.addFloatVector("Signals/Raw", "original_signal", signal_float, "amplitude");
        
        // Save filtered signal
        std::vector<float> filtered_float(filtered_data.begin(), filtered_data.end());
        recorder.addFloatVector("Signals/Processed", "filtered_signal", filtered_float, "amplitude");
        
        // Save filter information
        if (apply_kalman) {
            recorder.addMetadata("Filters", "kalman", "enabled");
            sprintf(param_str, "%.4f", kalman_process);
            recorder.addMetadata("Filters", "kalman_process_noise", param_str);
            sprintf(param_str, "%.4f", kalman_measure);
            recorder.addMetadata("Filters", "kalman_measure_noise", param_str);
        }
        if (apply_median) {
            recorder.addMetadata("Filters", "median", "enabled");
            sprintf(param_str, "%d", median_window);
            recorder.addMetadata("Filters", "median_window", param_str);
        }
        if (apply_wavelet) {
            recorder.addMetadata("Filters", "wavelet", "enabled");
        }
        
        // Save anomalies if detected
        if (!anomaly_indices.empty()) {
            const char* method_names[] = { "Z-Score", "IQR", "MAD" };
            recorder.addMetadata("Anomalies", "method", method_names[anomaly_method]);
            sprintf(param_str, "%.2f", anomaly_threshold);
            recorder.addMetadata("Anomalies", "threshold", param_str);
            sprintf(param_str, "%d", (int)anomaly_indices.size());
            recorder.addMetadata("Anomalies", "count", param_str);
            
            // Save anomaly indices
            std::vector<float> anomaly_float(anomaly_indices.begin(), anomaly_indices.end());
            recorder.addFloatVector("Anomalies", "indices", anomaly_float, "sample_index");
        }
        
        // Save FFT data
        if (!fft_frequencies.empty()) {
            std::vector<float> fft_freq_float(fft_frequencies.begin(), fft_frequencies.end());
            std::vector<float> fft_mag_float(fft_magnitudes.begin(), fft_magnitudes.end());
            recorder.addFloatVector("Analysis/FFT", "frequencies", fft_freq_float, "Hz");
            recorder.addFloatVector("Analysis/FFT", "magnitudes", fft_mag_float, "amplitude");
        }
        
        // Save ML features if extracted
        if (show_ml_features) {
            std::vector<float> ml_vector;
            ml_vector.push_back((float)ml_features.mean);
            ml_vector.push_back((float)ml_features.std_dev);
            ml_vector.push_back((float)ml_features.variance);
            ml_vector.push_back((float)ml_features.skewness);
            ml_vector.push_back((float)ml_features.kurtosis);
            ml_vector.push_back((float)ml_features.rms);
            ml_vector.push_back((float)ml_features.peak_to_peak);
            ml_vector.push_back((float)ml_features.crest_factor);
            ml_vector.push_back((float)ml_features.dominant_frequency);
            ml_vector.push_back((float)ml_features.spectral_centroid);
            ml_vector.push_back((float)ml_features.spectral_spread);
            ml_vector.push_back((float)ml_features.spectral_entropy);
            ml_vector.push_back((float)ml_features.total_power);
            ml_vector.push_back((float)ml_features.power_low_freq);
            ml_vector.push_back((float)ml_features.power_mid_freq);
            ml_vector.push_back((float)ml_features.power_high_freq);
            ml_vector.push_back((float)ml_features.zero_crossing_rate);
            ml_vector.push_back((float)ml_features.mean_crossing_rate);
            ml_vector.push_back((float)ml_features.energy);
            ml_vector.push_back((float)ml_features.autocorr_peak);
            
            recorder.addFloatVector("Analysis/ML_Features", "feature_vector", ml_vector, "mixed");
            
            // Add feature names as metadata
            recorder.addMetadata("Analysis/ML_Features", "feature_names", 
                "mean,std_dev,variance,skewness,kurtosis,rms,peak_to_peak,crest_factor,"
                "dominant_frequency,spectral_centroid,spectral_spread,spectral_entropy,"
                "total_power,power_low_freq,power_mid_freq,power_high_freq,"
                "zero_crossing_rate,mean_crossing_rate,energy,autocorr_peak");
        }
        
        // Statistics
        sprintf(param_str, "%.6f", sp.GetMean());
        recorder.addMetadata("Statistics", "mean", param_str);
        sprintf(param_str, "%.6f", sp.GetStandardDeviation());
        recorder.addMetadata("Statistics", "std_dev", param_str);
        sprintf(param_str, "%.6f", sp.GetVariance());
        recorder.addMetadata("Statistics", "variance", param_str);
        
        // Get absolute path of saved file
        char abs_path[PATH_MAX];
        if (realpath(save_filename, abs_path) != NULL) {
            save_success = true;
            save_error = false;
            sprintf(save_message, "✓ Successfully saved!\n\nLocation:\n%s\n\nView with:\nh5dump %s", abs_path, save_filename);
        } else {
            // Fallback if realpath fails
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                save_success = true;
                save_error = false;
                sprintf(save_message, "✓ Successfully saved!\n\nLocation:\n%s/%s\n\nView with:\nh5dump %s", cwd, save_filename, save_filename);
            } else {
                save_success = true;
                save_error = false;
                sprintf(save_message, "✓ Successfully saved to %s\nView with: h5dump %s", save_filename, save_filename);
            }
        }
        
    } catch (const std::exception& e) {
        save_success = false;
        save_error = true;
        sprintf(save_message, "✗ Error saving file: %s", e.what());
    }
}

// Load signal from HDF5
void LoadSignalFromHDF5() {
    try {
        H5File file(load_filename, H5F_ACC_RDONLY);
        
        // Clear previous data
        loaded_signal.clear();
        loaded_filtered.clear();
        loaded_anomalies.clear();
        loaded_fft_freq.clear();
        loaded_fft_mag.clear();
        
        // Read metadata
        char info_buffer[512] = "";
        try {
            Group session = file.openGroup("/Session");
            DataSet timestamp_ds = session.openDataSet("timestamp");
            StrType str_type = timestamp_ds.getStrType();
            std::string timestamp_str;
            timestamp_ds.read(timestamp_str, str_type);
            sprintf(info_buffer, "Session: %s", timestamp_str.c_str());
        } catch (...) {
            sprintf(info_buffer, "Loaded from: %s", load_filename);
        }
        strcpy(loaded_info, info_buffer);
        
        // Read original signal
        try {
            DataSet dataset = file.openDataSet("/Signals/Raw/original_signal");
            DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims);
            
            std::vector<float> temp_data(dims[0]);
            dataset.read(temp_data.data(), PredType::NATIVE_FLOAT);
            
            loaded_signal.resize(dims[0]);
            for (size_t i = 0; i < dims[0]; i++) {
                loaded_signal[i] = temp_data[i];
            }
        } catch (...) {
            // Signal not found
        }
        
        // Read filtered signal
        try {
            DataSet dataset = file.openDataSet("/Signals/Processed/filtered_signal");
            DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims);
            
            std::vector<float> temp_data(dims[0]);
            dataset.read(temp_data.data(), PredType::NATIVE_FLOAT);
            
            loaded_filtered.resize(dims[0]);
            for (size_t i = 0; i < dims[0]; i++) {
                loaded_filtered[i] = temp_data[i];
            }
        } catch (...) {
            // Filtered signal not found
        }
        
        // Read anomalies
        try {
            DataSet dataset = file.openDataSet("/Anomalies/indices");
            DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims);
            
            std::vector<float> temp_data(dims[0]);
            dataset.read(temp_data.data(), PredType::NATIVE_FLOAT);
            
            loaded_anomalies.resize(dims[0]);
            for (size_t i = 0; i < dims[0]; i++) {
                loaded_anomalies[i] = (int)temp_data[i];
            }
        } catch (...) {
            // Anomalies not found
        }
        
        // Read FFT data
        try {
            DataSet freq_ds = file.openDataSet("/Analysis/FFT/frequencies");
            DataSet mag_ds = file.openDataSet("/Analysis/FFT/magnitudes");
            
            DataSpace freq_space = freq_ds.getSpace();
            hsize_t dims[1];
            freq_space.getSimpleExtentDims(dims);
            
            std::vector<float> temp_freq(dims[0]);
            std::vector<float> temp_mag(dims[0]);
            freq_ds.read(temp_freq.data(), PredType::NATIVE_FLOAT);
            mag_ds.read(temp_mag.data(), PredType::NATIVE_FLOAT);
            
            loaded_fft_freq.resize(dims[0]);
            loaded_fft_mag.resize(dims[0]);
            for (size_t i = 0; i < dims[0]; i++) {
                loaded_fft_freq[i] = temp_freq[i];
                loaded_fft_mag[i] = temp_mag[i];
            }
        } catch (...) {
            // FFT data not found
        }
        
        file.close();
        
        // Get absolute path
        char abs_path[PATH_MAX];
        if (realpath(load_filename, abs_path) != NULL) {
            load_success = true;
            load_error = false;
            sprintf(load_message, "✓ Successfully loaded!\n\nFile:\n%s\n\n"
                    "Original signal: %d samples\n"
                    "Filtered signal: %d samples\n"
                    "Anomalies: %d points\n"
                    "FFT data: %d bins", 
                    abs_path,
                    (int)loaded_signal.size(),
                    (int)loaded_filtered.size(),
                    (int)loaded_anomalies.size(),
                    (int)loaded_fft_freq.size());
        } else {
            load_success = true;
            load_error = false;
            sprintf(load_message, "✓ Successfully loaded %d samples!", (int)loaded_signal.size());
        }
        
        show_loaded_data = true;
        
    } catch (const FileIException& e) {
        load_success = false;
        load_error = true;
        sprintf(load_message, "✗ Error: Cannot open file '%s'\n\nMake sure the file exists.", load_filename);
    } catch (const Exception& e) {
        load_success = false;
        load_error = true;
        sprintf(load_message, "✗ HDF5 Error: %s", e.getDetailMsg().c_str());
    } catch (const std::exception& e) {
        load_success = false;
        load_error = true;
        sprintf(load_message, "✗ Error loading file: %s", e.what());
    }
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
    InitializeConfiguration();  // După ImGui init

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
        
        // HDF5 Save button
        ImGui::Spacing();
        ImGui::Text("💾 Save to HDF5");
        ImGui::Separator();
        if (ImGui::Button("📁 Save Signal Recording", ImVec2(-1, 0))) {
            show_save_dialog = true;
        }
        
        // HDF5 Load button
        ImGui::Spacing();
        ImGui::Text("📂 Load from HDF5");
        ImGui::Separator();
        if (ImGui::Button("📥 Load Signal Recording", ImVec2(-1, 0))) {
            show_load_dialog = true;
        }
        
        if (show_loaded_data) {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Loaded");
            ImGui::Text("%s", loaded_info);
            if (ImGui::Button("Clear Loaded Data", ImVec2(-1, 0))) {
                show_loaded_data = false;
                loaded_signal.clear();
                loaded_filtered.clear();
                loaded_anomalies.clear();
                loaded_fft_freq.clear();
                loaded_fft_mag.clear();
            }
        }
        
        // Configuration management UI
        ImGui::Spacing();
        RenderConfigurationUI();
        
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
        
        // Loaded signal visualization
        if (show_loaded_data && !loaded_signal.empty()) {
            ImGui::Separator();
            ImGui::Text("📂 Loaded Signal Data");
            
            // Plot loaded signal
            if (ImPlot::BeginPlot("Loaded Signal - Time Domain", ImVec2(-1, 300))) {
                ImPlot::SetupAxes("Sample", "Amplitude");
                
                std::vector<double> x_vals(loaded_signal.size());
                for (size_t i = 0; i < x_vals.size(); i++) x_vals[i] = i;
                
                ImPlot::PlotLine("Loaded Original", x_vals.data(), loaded_signal.data(), loaded_signal.size());
                
                if (!loaded_filtered.empty()) {
                    ImPlot::PlotLine("Loaded Filtered", x_vals.data(), loaded_filtered.data(), loaded_filtered.size());
                }
                
                if (!loaded_anomalies.empty()) {
                    std::vector<double> anomaly_x, anomaly_y;
                    for (int idx : loaded_anomalies) {
                        if (idx < (int)loaded_signal.size()) {
                            anomaly_x.push_back(idx);
                            anomaly_y.push_back(loaded_signal[idx]);
                        }
                    }
                    if (!anomaly_x.empty()) {
                        ImPlot::PlotScatter("Loaded Anomalies", anomaly_x.data(), anomaly_y.data(), anomaly_x.size());
                    }
                }
                
                ImPlot::EndPlot();
            }
            
            // Plot loaded FFT
            if (!loaded_fft_freq.empty()) {
                if (ImPlot::BeginPlot("Loaded FFT Spectrum", ImVec2(-1, 300))) {
                    ImPlot::SetupAxes("Frequency (Hz)", "Magnitude");
                    ImPlot::PlotLine("Loaded FFT", loaded_fft_freq.data(), loaded_fft_mag.data(), loaded_fft_freq.size());
                    ImPlot::EndPlot();
                }
            }
        }
        
        ImGui::EndChild();
        
        ImGui::End();

        // Save dialog
        if (show_save_dialog) {
            ImGui::OpenPopup("Save Signal to HDF5");
        }
        
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        
        if (ImGui::BeginPopupModal("Save Signal to HDF5", &show_save_dialog, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Save current signal data to HDF5 file format");
            ImGui::Separator();
            
            ImGui::Text("Filename:");
            ImGui::InputText("##filename", save_filename, sizeof(save_filename));
            
            ImGui::Text("Session Info:");
            ImGui::InputText("##sessioninfo", session_info, sizeof(session_info));
            
            ImGui::Spacing();
            ImGui::Text("This will save:");
            ImGui::BulletText("Original and filtered signals");
            ImGui::BulletText("Signal parameters and metadata");
            ImGui::BulletText("Filter settings");
            if (!anomaly_indices.empty()) {
                ImGui::BulletText("Detected anomalies (%d points)", (int)anomaly_indices.size());
            }
            if (!fft_frequencies.empty()) {
                ImGui::BulletText("FFT frequency spectrum");
            }
            if (show_ml_features) {
                ImGui::BulletText("ML features (20 features)");
            }
            ImGui::BulletText("Statistical analysis");
            
            ImGui::Spacing();
            
            if (save_success) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", save_message);
            } else if (save_error) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", save_message);
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            
            if (ImGui::Button("Save", ImVec2(120, 0))) {
                SaveSignalToHDF5();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                show_save_dialog = false;
                save_success = false;
                save_error = false;
            }
            
            ImGui::EndPopup();
        }

        // Load dialog
        if (show_load_dialog) {
            ImGui::OpenPopup("Load Signal from HDF5");
        }
        
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        
        if (ImGui::BeginPopupModal("Load Signal from HDF5", &show_load_dialog, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Load previously saved signal data from HDF5 file");
            ImGui::Separator();
            
            ImGui::Text("Filename:");
            ImGui::InputText("##loadfilename", load_filename, sizeof(load_filename));
            
            ImGui::Spacing();
            ImGui::Text("This will load:");
            ImGui::BulletText("Original and filtered signals");
            ImGui::BulletText("Signal metadata");
            ImGui::BulletText("Anomaly detection results (if available)");
            ImGui::BulletText("FFT spectrum data (if available)");
            ImGui::Spacing();
            ImGui::TextWrapped("💡 The loaded signal will be displayed below the current signal plots.");
            
            ImGui::Spacing();
            
            if (load_success) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", load_message);
            } else if (load_error) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", load_message);
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            
            if (ImGui::Button("Load", ImVec2(120, 0))) {
                LoadSignalFromHDF5();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                show_load_dialog = false;
                load_success = false;
                load_error = false;
            }
            
            ImGui::EndPopup();
        }

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
    SaveConfiguration();  // Înainte de cleanup
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
