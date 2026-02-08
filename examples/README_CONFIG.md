# ConfigDataWriter Integration Guide

## Overview
This guide explains how to integrate the [ConfigDataWriter](https://github.com/SimedruF/ConfigDataWriter.git) library into the SignalProcessing ImGui application to persist user preferences and settings.

## Features
Using ConfigDataWriter in the ImGui application provides:
- ✅ **Automatic session persistence** - Settings are restored when you reopen the app
- ✅ **Manual save/load** - Save and restore configurations with UI buttons
- ✅ **Default reset** - Quick restore to factory defaults
- ✅ **Auto-save** - Periodic background saves to prevent data loss
- ✅ **INI format** - Human-readable configuration files

## What Gets Saved
The configuration system persists all application state including:

### Signal Generation
- Signal type (Sine, ECG, Turbine, Noise)
- Noise level
- Frequency
- Signal length
- Sampling rate

### Filters
- Kalman filter (enabled/disabled + parameters)
- Median filter (enabled/disabled + window size)
- Wavelet denoising (enabled/disabled)

### Anomaly Detection
- Anomaly display toggle
- Detection method (Z-Score, IQR, MAD)
- Threshold values

### UI Preferences
- ML features display toggle
- Last used HDF5 save/load paths
- Session information

## Setup Instructions

### 1. Clone ConfigDataWriter Library
```bash
cd /home/simedruf/Projects/SignalProcessing/SignalProcessing
git clone https://github.com/SimedruF/ConfigDataWriter.git external/ConfigDataWriter
```

### 2. Add to CMakeLists.txt
```cmake
# Add ConfigDataWriter as a subdirectory (if it has CMakeLists.txt)
# OR include directly:
set(CONFIG_WRITER_SOURCES
    external/ConfigDataWriter/source/ConfigDataWriter.cpp
)

# Add to executable
add_executable(imgui_demo
    imgui_demo.cpp
    ../source/SignalProcessing.cpp
    ${CONFIG_WRITER_SOURCES}
    # ... other sources
)

# Add include directories
target_include_directories(imgui_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../source
    ${CMAKE_CURRENT_SOURCE_DIR}/external/ConfigDataWriter/source
    # ... other includes
)
```

### 3. Integration into imgui_demo.cpp
Add the following code to your imgui_demo.cpp:

#### Include Headers
```cpp
#include "ImGuiConfig.hpp"
```

#### Add Global Variables (top of file)
```cpp
// Configuration management
ImGuiConfigManager config_manager("imgui_app.ini");
ImGuiAppConfig app_config = ImGuiConfigManager::GetDefaultConfig();
```

#### In main() function - After ImGui Initialization
```cpp
// Initialize configuration from saved file
InitializeConfiguration();
```

#### In Main Loop - Add UI Element
Add this in your ImGui window where you want the configuration controls:
```cpp
// Configuration save/load UI
RenderConfigurationUI();

// Optional: Auto-save every 5 minutes
AutoSaveConfiguration();
```

#### Before Application Exit - In Cleanup Section
```cpp
// Save configuration before exiting
SaveConfiguration();
```

## Usage Example

### Generated INI File Format
After first save, `imgui_app.ini` will look like:
```ini
; ImGui SignalProcessing Configuration
[SignalGeneration]
signal_type = 1
noise_level = 0.100000
frequency = 1.200000
signal_length = 500
sampling_rate = 100.000000

[Filters]
apply_kalman = 0
apply_median = 1
apply_wavelet = 0
kalman_process = 0.010000
kalman_measure = 0.100000
median_window = 5

[AnomalyDetection]
show_anomalies = 1
anomaly_method = 0
anomaly_threshold = 3.000000

[Features]
show_ml_features = 1

[Paths]
last_save_path = /home/user/data/recording.h5
last_load_path = /home/user/data/recording.h5
session_info = Interactive ImGui Session
```

## Manual Configuration Editing
You can manually edit `imgui_app.ini` with any text editor:
```bash
nano imgui_app.ini
```

Changes will be loaded next time you start the application.

## Advanced Usage

### Using ConfigDataWriter Directly
If you need more complex configuration structures, use ConfigDataWriter directly:

```cpp
#include "ConfigDataWriter.h"

// Save custom binary config
struct MyCustomConfig {
    int value1;
    float value2;
    char data[200];
};

MyCustomConfig cfg = {10, 3.14f, "custom data"};
ConfigDataWriter writer("custom.cfg");
writer.SaveCfgDataBinary(cfg);
writer.LoadCfgDataBinary(cfg);

// Save CSV data
int data[] = {1, 2, 3, 4, 5, 6};
ConfigDataWriter csv_writer("data.csv");
csv_writer.SaveIntDataCSV("Column1,Column2", 16, data, 2, 3);
```

### Multiple Configuration Profiles
```cpp
// Save different profiles
void SaveProfile(const char* profile_name) {
    char filename[256];
    snprintf(filename, sizeof(filename), "profile_%s.ini", profile_name);
    ImGuiConfigManager profile_manager(filename);
    profile_manager.SaveConfig(app_config);
}

void LoadProfile(const char* profile_name) {
    char filename[256];
    snprintf(filename, sizeof(filename), "profile_%s.ini", profile_name);
    ImGuiConfigManager profile_manager(filename);
    if (profile_manager.LoadConfig(app_config) == 0) {
        InitializeConfiguration();
    }
}
```

## Troubleshooting

### Configuration Not Loading
```bash
# Check if file exists
ls -la imgui_app.ini

# Check file permissions
chmod 644 imgui_app.ini

# Verify format by viewing content
cat imgui_app.ini
```

### Reset to Defaults
If configuration becomes corrupted:
```bash
# Delete config file
rm imgui_app.ini

# Application will create new one with defaults
./imgui_demo
```

### Debug Mode
Add debug output in ImGuiConfigManager:
```cpp
printf("Loading key: %s = %s\n", k, value);
```

## Build and Run
```bash
cd examples
./build_and_run_imgui.sh
```

The application will:
1. Load saved configuration from `imgui_app.ini`
2. Apply all settings to UI
3. Auto-save every 5 minutes
4. Save on exit

## Benefits
- **User Experience**: Settings persist across sessions
- **Rapid Prototyping**: Quickly test different parameter combinations
- **Reproducibility**: Save exact parameters for specific experiments
- **Team Sharing**: Share INI files with team members for consistent setups
- **Backup**: Manual saves before trying risky parameter changes

## See Also
- [ConfigDataWriter Repository](https://github.com/SimedruF/ConfigDataWriter.git)
- [ConfigDataWriter Documentation](https://github.com/SimedruF/ConfigDataWriter/blob/main/README.md)
- [ImGuiConfig.hpp](ImGuiConfig.hpp) - Configuration structure and manager
- [config_integration_example.cpp](config_integration_example.cpp) - Complete integration example

## License
ConfigDataWriter is MIT licensed. See repository for details.
