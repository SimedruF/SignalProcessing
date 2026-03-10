#include "../source/SignalProcessing.h"
#include "../source/SignalRecorder.hpp"
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    printf("=== Multi-Channel Synchronized Recording Example ===\n\n");
    
    // Simulation parameters
    const int NUM_SAMPLES = 1000;
    const double SAMPLING_RATE = 100.0; // Hz
    const double dt = 1.0 / SAMPLING_RATE;
    
    // Create recorder
    SignalRecorder recorder("multi_channel_sync.h5");
    printf("✓ Created HDF5 file: multi_channel_sync.h5\n");
    
    // Add session metadata
    time_t now = time(0);
    char timestamp_str[64];
    strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    recorder.addMetadata("Session", "timestamp", timestamp_str);
    recorder.addMetadata("Session", "device", "Multi-Axis Accelerometer + Gyroscope");
    recorder.addMetadata("Session", "location", "Test Bench");
    
    char param_str[128];
    sprintf(param_str, "%.1f", SAMPLING_RATE);
    recorder.addMetadata("Parameters", "sampling_rate_hz", param_str);
    sprintf(param_str, "%d", NUM_SAMPLES);
    recorder.addMetadata("Parameters", "num_samples", param_str);
    
    printf("✓ Added session metadata\n");
    
    // ===================================================================
    // METHOD 1: Manual - Save each channel separately with timestamps
    // ===================================================================
    printf("\n--- Method 1: Manual channel-by-channel ---\n");
    
    std::vector<double> timestamps;
    std::vector<float> accel_x, accel_y, accel_z;
    std::vector<float> gyro_x, gyro_y, gyro_z;
    
    // Generate synchronized data
    for (int i = 0; i < NUM_SAMPLES; i++) {
        double t = i * dt;
        timestamps.push_back(t);
        
        // Simulate accelerometer (vibration + gravity)
        accel_x.push_back(9.81 * sin(2 * M_PI * 5 * t) + 0.1 * (rand() % 100) / 100.0);
        accel_y.push_back(9.81 * cos(2 * M_PI * 5 * t) + 0.1 * (rand() % 100) / 100.0);
        accel_z.push_back(9.81 + 0.5 * sin(2 * M_PI * 2 * t));
        
        // Simulate gyroscope (rotation)
        gyro_x.push_back(0.5 * sin(2 * M_PI * 3 * t));
        gyro_y.push_back(0.3 * cos(2 * M_PI * 4 * t));
        gyro_z.push_back(0.2 * sin(2 * M_PI * 1 * t));
    }
    
    printf("✓ Generated %d synchronized samples\n", NUM_SAMPLES);
    
    // Save timestamps (common for all channels)
    recorder.addDoubleVector("Sensors/IMU", "timestamps", timestamps, "seconds");
    printf("✓ Saved timestamps\n");
    
    // Save accelerometer channels
    recorder.addFloatVector("Sensors/IMU/Accelerometer", "x_axis", accel_x, "m/s²");
    recorder.addFloatVector("Sensors/IMU/Accelerometer", "y_axis", accel_y, "m/s²");
    recorder.addFloatVector("Sensors/IMU/Accelerometer", "z_axis", accel_z, "m/s²");
    printf("✓ Saved 3-axis accelerometer data\n");
    
    // Save gyroscope channels
    recorder.addFloatVector("Sensors/IMU/Gyroscope", "x_axis", gyro_x, "rad/s");
    recorder.addFloatVector("Sensors/IMU/Gyroscope", "y_axis", gyro_y, "rad/s");
    recorder.addFloatVector("Sensors/IMU/Gyroscope", "z_axis", gyro_z, "rad/s");
    printf("✓ Saved 3-axis gyroscope data\n");
    
    // ===================================================================
    // METHOD 2: Using addSynchronizedChannels helper
    // ===================================================================
    printf("\n--- Method 2: Using helper function ---\n");
    
    // Prepare channels
    std::vector<std::pair<std::string, std::vector<float>>> channels;
    channels.push_back({"temperature", std::vector<float>()});
    channels.push_back({"pressure", std::vector<float>()});
    channels.push_back({"humidity", std::vector<float>()});
    
    std::vector<double> env_timestamps;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        double t = i * dt;
        env_timestamps.push_back(t);
        
        // Environmental sensors
        channels[0].second.push_back(25.0 + 2.0 * sin(2 * M_PI * 0.1 * t)); // Temperature °C
        channels[1].second.push_back(1013.0 + 10.0 * sin(2 * M_PI * 0.05 * t)); // Pressure hPa
        channels[2].second.push_back(60.0 + 5.0 * sin(2 * M_PI * 0.03 * t)); // Humidity %
    }
    
    recorder.addSynchronizedChannels("Sensors/Environment", env_timestamps, channels);
    printf("✓ Saved 3 synchronized environmental channels\n");
    
    // ===================================================================
    // METHOD 3: Using SignalProcessing timestamps
    // ===================================================================
    printf("\n--- Method 3: Using SignalProcessing class ---\n");
    
    SignalProcessing sp1, sp2, sp3;
    
    // Add values with timestamps (simulating real-time acquisition)
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    double start_time = ts.tv_sec + ts.tv_nsec / 1e9;
    
    for (int i = 0; i < 100; i++) {
        double value1 = sin(2 * M_PI * i / 10.0);
        double value2 = cos(2 * M_PI * i / 10.0);
        double value3 = sin(2 * M_PI * i / 5.0);
        
        // Get current timestamp
        clock_gettime(CLOCK_REALTIME, &ts);
        
        sp1.AddValueWithTimestamp(value1, ts);
        sp2.AddValueWithTimestamp(value2, ts);
        sp3.AddValueWithTimestamp(value3, ts);
    }
    
    // Extract data and timestamps
    std::vector<double> sp_timestamps;
    std::vector<float> sp_data1, sp_data2, sp_data3;
    
    for (int i = 0; i < sp1.GetIndex(); i++) {
        struct timespec ts_i = sp1.GetTimestamp(i);
        double timestamp_sec = ts_i.tv_sec + ts_i.tv_nsec / 1e9;
        sp_timestamps.push_back(timestamp_sec - start_time); // Relative to start
        sp_data1.push_back(sp1.GetValue(i));
        sp_data2.push_back(sp2.GetValue(i));
        sp_data3.push_back(sp3.GetValue(i));
    }
    
    recorder.addDoubleVector("Realtime/Channels", "timestamps", sp_timestamps, "seconds");
    recorder.addFloatVector("Realtime/Channels", "channel_1", sp_data1, "V");
    recorder.addFloatVector("Realtime/Channels", "channel_2", sp_data2, "V");
    recorder.addFloatVector("Realtime/Channels", "channel_3", sp_data3, "V");
    printf("✓ Saved 3 channels with real timestamps from SignalProcessing\n");
    
    printf("\n=== Recording Complete! ===\n");
    printf("File structure:\n");
    printf("/multi_channel_sync.h5\n");
    printf("├── Session/ (metadata)\n");
    printf("├── Parameters/ (sampling info)\n");
    printf("├── Sensors/\n");
    printf("│   ├── IMU/\n");
    printf("│   │   ├── timestamps [%d] (seconds)\n", NUM_SAMPLES);
    printf("│   │   ├── Accelerometer/\n");
    printf("│   │   │   ├── x_axis [%d] (m/s²)\n", NUM_SAMPLES);
    printf("│   │   │   ├── y_axis [%d] (m/s²)\n", NUM_SAMPLES);
    printf("│   │   │   └── z_axis [%d] (m/s²)\n", NUM_SAMPLES);
    printf("│   │   └── Gyroscope/\n");
    printf("│   │       ├── x_axis [%d] (rad/s)\n", NUM_SAMPLES);
    printf("│   │       ├── y_axis [%d] (rad/s)\n", NUM_SAMPLES);
    printf("│   │       └── z_axis [%d] (rad/s)\n", NUM_SAMPLES);
    printf("│   └── Environment/\n");
    printf("│       ├── timestamps [%d] (seconds)\n", NUM_SAMPLES);
    printf("│       ├── temperature [%d]\n", NUM_SAMPLES);
    printf("│       ├── pressure [%d]\n", NUM_SAMPLES);
    printf("│       └── humidity [%d]\n", NUM_SAMPLES);
    printf("└── Realtime/\n");
    printf("    └── Channels/\n");
    printf("        ├── timestamps [100] (seconds)\n");
    printf("        ├── channel_1 [100] (V)\n");
    printf("        ├── channel_2 [100] (V)\n");
    printf("        └── channel_3 [100] (V)\n");
    
    printf("\nView with: h5dump multi_channel_sync.h5\n");
    printf("Or: h5ls -r multi_channel_sync.h5\n");
    
    return 0;
}
