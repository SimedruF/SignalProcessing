#include "../source/SignalRecorder.hpp"
#include <stdio.h>
#include <vector>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    printf("=== SignalRecorder Test ===\n\n");

    // Create a recorder
    SignalRecorder recorder("test_output.h5");
    printf("✓ Created HDF5 file: test_output.h5\n");

    // Add metadata
    recorder.addMetadata("Info", "device", "Vibration Sensor XYZ-100");
    recorder.addMetadata("Info", "location", "Turbine Bearing #3");
    printf("✓ Added metadata\n");

    // Prepare test data - simulated vibration signal
    std::vector<float> vibrationData;
    for (int i = 0; i < 1000; i++) {
        float value = 2.5f * sin(2.0f * M_PI * i / 50.0f) + 0.3f * (rand() % 100) / 100.0f;
        vibrationData.push_back(value);
    }
    printf("✓ Generated 1000 samples of vibration data\n");

    // Save data with path structure
    recorder.addFloatVector("Sensors/Vibrations", "raw_signal", vibrationData, "m/s²");
    printf("✓ Saved data to /Sensors/Vibrations/raw_signal with units\n");

    // Add another dataset - temperature
    std::vector<float> temperatureData;
    for (int i = 0; i < 500; i++) {
        temperatureData.push_back(75.0f + 2.0f * sin(i / 100.0f));
    }
    recorder.addFloatVector("Sensors/Temperature", "bearing_temp", temperatureData, "°C");
    printf("✓ Saved temperature data to /Sensors/Temperature/bearing_temp\n");

    // Add processed data
    std::vector<float> processedData;
    for (int i = 0; i < vibrationData.size(); i++) {
        processedData.push_back(vibrationData[i] * 1.5f); // Simple scaling
    }
    recorder.addFloatVector("Processed/Scaled", "scaled_signal", processedData, "m/s²");
    printf("✓ Saved processed data to /Processed/Scaled/scaled_signal\n");

    printf("\n=== Test Completed Successfully ===\n");
    printf("Output file: test_output.h5\n");
    printf("You can inspect it with: h5dump test_output.h5\n");

    return 0;
}
