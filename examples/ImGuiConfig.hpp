#ifndef IMGUI_CONFIG_HPP
#define IMGUI_CONFIG_HPP

#include <string.h>
#include <stdio.h>

// Structure for ImGui application configuration
struct ImGuiAppConfig {
    // Signal generation parameters
    int signal_type;           // 0=Sine, 1=ECG, 2=Turbine, 3=Noise
    float noise_level;
    float frequency;
    int signal_length;
    double sampling_rate;
    
    // Filter parameters
    int apply_kalman;          // 0/1 boolean
    int apply_median;
    int apply_wavelet;
    float kalman_process;
    float kalman_measure;
    int median_window;
    
    // Anomaly detection
    int show_anomalies;
    int anomaly_method;        // 0=ZScore, 1=IQR, 2=MAD
    float anomaly_threshold;
    
    // ML Features
    int show_ml_features;
    
    // Last used paths
    char last_save_path[256];
    char last_load_path[256];
    char session_info[256];
};

// Helper class to manage ImGui configuration with ConfigDataWriter integration
class ImGuiConfigManager {
private:
    const char* config_filename;
    
public:
    ImGuiConfigManager(const char* filename = "imgui_app_config.ini") 
        : config_filename(filename) {}
    
    // Save configuration to INI file
    int SaveConfig(const ImGuiAppConfig& config) {
        FILE* file = fopen(config_filename, "w");
        if (!file) {
            fprintf(stderr, "Failed to open config file for writing: %s\n", config_filename);
            return -1;
        }
        
        fprintf(file, "; ImGui SignalProcessing Configuration\n");
        fprintf(file, "[SignalGeneration]\n");
        fprintf(file, "signal_type = %d\n", config.signal_type);
        fprintf(file, "noise_level = %f\n", config.noise_level);
        fprintf(file, "frequency = %f\n", config.frequency);
        fprintf(file, "signal_length = %d\n", config.signal_length);
        fprintf(file, "sampling_rate = %f\n", config.sampling_rate);
        
        fprintf(file, "\n[Filters]\n");
        fprintf(file, "apply_kalman = %d\n", config.apply_kalman);
        fprintf(file, "apply_median = %d\n", config.apply_median);
        fprintf(file, "apply_wavelet = %d\n", config.apply_wavelet);
        fprintf(file, "kalman_process = %f\n", config.kalman_process);
        fprintf(file, "kalman_measure = %f\n", config.kalman_measure);
        fprintf(file, "median_window = %d\n", config.median_window);
        
        fprintf(file, "\n[AnomalyDetection]\n");
        fprintf(file, "show_anomalies = %d\n", config.show_anomalies);
        fprintf(file, "anomaly_method = %d\n", config.anomaly_method);
        fprintf(file, "anomaly_threshold = %f\n", config.anomaly_threshold);
        
        fprintf(file, "\n[Features]\n");
        fprintf(file, "show_ml_features = %d\n", config.show_ml_features);
        
        fprintf(file, "\n[Paths]\n");
        fprintf(file, "last_save_path = %s\n", config.last_save_path);
        fprintf(file, "last_load_path = %s\n", config.last_load_path);
        fprintf(file, "session_info = %s\n", config.session_info);
        
        fclose(file);
        printf("✓ Configuration saved to %s\n", config_filename);
        return 0;
    }
    
    // Load configuration from INI file
    int LoadConfig(ImGuiAppConfig& config) {
        FILE* file = fopen(config_filename, "r");
        if (!file) {
            fprintf(stderr, "Config file not found: %s (using defaults)\n", config_filename);
            return -1;
        }
        
        char line[512];
        char current_section[64] = "";
        
        while (fgets(line, sizeof(line), file)) {
            // Skip comments and empty lines
            if (line[0] == ';' || line[0] == '#' || line[0] == '\n') continue;
            
            // Parse section headers
            if (line[0] == '[') {
                sscanf(line, "[%63[^]]]", current_section);
                continue;
            }
            
            // Parse key-value pairs
            char key[128], value[256];
            if (sscanf(line, "%127[^=] = %255[^\n]", key, value) == 2) {
                // Trim whitespace from key
                char* k = key;
                while (*k == ' ') k++;
                char* end = k + strlen(k) - 1;
                while (end > k && (*end == ' ' || *end == '\t')) end--;
                *(end + 1) = '\0';
                
                // Parse values based on section and key
                if (strcmp(current_section, "SignalGeneration") == 0) {
                    if (strcmp(k, "signal_type") == 0) config.signal_type = atoi(value);
                    else if (strcmp(k, "noise_level") == 0) config.noise_level = atof(value);
                    else if (strcmp(k, "frequency") == 0) config.frequency = atof(value);
                    else if (strcmp(k, "signal_length") == 0) config.signal_length = atoi(value);
                    else if (strcmp(k, "sampling_rate") == 0) config.sampling_rate = atof(value);
                }
                else if (strcmp(current_section, "Filters") == 0) {
                    if (strcmp(k, "apply_kalman") == 0) config.apply_kalman = atoi(value);
                    else if (strcmp(k, "apply_median") == 0) config.apply_median = atoi(value);
                    else if (strcmp(k, "apply_wavelet") == 0) config.apply_wavelet = atoi(value);
                    else if (strcmp(k, "kalman_process") == 0) config.kalman_process = atof(value);
                    else if (strcmp(k, "kalman_measure") == 0) config.kalman_measure = atof(value);
                    else if (strcmp(k, "median_window") == 0) config.median_window = atoi(value);
                }
                else if (strcmp(current_section, "AnomalyDetection") == 0) {
                    if (strcmp(k, "show_anomalies") == 0) config.show_anomalies = atoi(value);
                    else if (strcmp(k, "anomaly_method") == 0) config.anomaly_method = atoi(value);
                    else if (strcmp(k, "anomaly_threshold") == 0) config.anomaly_threshold = atof(value);
                }
                else if (strcmp(current_section, "Features") == 0) {
                    if (strcmp(k, "show_ml_features") == 0) config.show_ml_features = atoi(value);
                }
                else if (strcmp(current_section, "Paths") == 0) {
                    if (strcmp(k, "last_save_path") == 0) strncpy(config.last_save_path, value, 255);
                    else if (strcmp(k, "last_load_path") == 0) strncpy(config.last_load_path, value, 255);
                    else if (strcmp(k, "session_info") == 0) strncpy(config.session_info, value, 255);
                }
            }
        }
        
        fclose(file);
        printf("✓ Configuration loaded from %s\n", config_filename);
        return 0;
    }
    
    // Get default configuration
    static ImGuiAppConfig GetDefaultConfig() {
        ImGuiAppConfig config;
        config.signal_type = 1;           // ECG
        config.noise_level = 0.1f;
        config.frequency = 1.2f;
        config.signal_length = 500;
        config.sampling_rate = 100.0;
        
        config.apply_kalman = 0;
        config.apply_median = 0;
        config.apply_wavelet = 0;
        config.kalman_process = 0.01f;
        config.kalman_measure = 0.1f;
        config.median_window = 5;
        
        config.show_anomalies = 0;
        config.anomaly_method = 0;
        config.anomaly_threshold = 3.0f;
        
        config.show_ml_features = 0;
        
        strcpy(config.last_save_path, "signal_recording.h5");
        strcpy(config.last_load_path, "signal_recording.h5");
        strcpy(config.session_info, "Interactive ImGui Session");
        
        return config;
    }
};

#endif // IMGUI_CONFIG_HPP
