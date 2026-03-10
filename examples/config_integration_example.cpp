// Example of integrating ImGuiConfig into imgui_demo.cpp
// Add this code to demonstrate configuration save/load functionality

#include "ImGuiConfig.hpp"

// Global configuration manager
ImGuiConfigManager config_manager("imgui_app.ini");
ImGuiAppConfig app_config = ImGuiConfigManager::GetDefaultConfig();

// Call this in main() after ImGui initialization
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
        
        printf("Configuration restored from previous session\n");
    } else {
        printf("Using default configuration\n");
    }
}

// Call this when application exits or when "Save Settings" button is pressed
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

// Add to ImGui UI to provide manual save/load buttons
void RenderConfigurationUI() {
    if (ImGui::CollapsingHeader("Configuration Management")) {
        ImGui::Text("Save/load application settings to preserve your preferences");
        ImGui::Spacing();
        
        if (ImGui::Button("💾 Save Current Settings", ImVec2(200, 0))) {
            SaveConfiguration();
            ImGui::OpenPopup("Config Saved");
        }
        
        ImGui::SameLine();
        if (ImGui::Button("📂 Load Settings", ImVec2(200, 0))) {
            if (config_manager.LoadConfig(app_config) == 0) {
                // Apply loaded configuration (same as InitializeConfiguration)
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
        if (ImGui::Button("🔄 Reset to Defaults", ImVec2(200, 0))) {
            app_config = ImGuiConfigManager::GetDefaultConfig();
            SaveConfiguration();
            InitializeConfiguration();
            GenerateSignal();
            ImGui::OpenPopup("Config Reset");
        }
        
        // Popup notifications
        if (ImGui::BeginPopup("Config Saved")) {
            ImGui::Text("✓ Configuration saved successfully!");
            ImGui::EndPopup();
        }
        
        if (ImGui::BeginPopup("Config Loaded")) {
            ImGui::Text("✓ Configuration loaded successfully!");
            ImGui::EndPopup();
        }
        
        if (ImGui::BeginPopup("Config Reset")) {
            ImGui::Text("✓ Configuration reset to defaults!");
            ImGui::EndPopup();
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("Configuration file: imgui_app.ini");
        ImGui::TextDisabled("Settings are auto-saved on exit");
    }
}

// === INTEGRATION STEPS ===

// 1. In main(), after ImGui initialization, add:
//    InitializeConfiguration();

// 2. In main loop, add this UI element where appropriate:
//    RenderConfigurationUI();

// 3. Before application exit (cleanup), add:
//    SaveConfiguration();

// 4. Optional: Auto-save periodically
void AutoSaveConfiguration() {
    static int frame_counter = 0;
    frame_counter++;
    
    // Auto-save every 5 minutes (assuming 60 FPS)
    if (frame_counter % (60 * 60 * 5) == 0) {
        SaveConfiguration();
        printf("Auto-saved configuration\n");
    }
}
