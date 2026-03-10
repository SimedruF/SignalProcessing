#include "SignalRecorder.hpp"
#include <cmath>

int main() {
    SignalRecorder recorder("test_recording.h5");

    // Metadate
    recorder.addMetadata("Metadata", "date", "2025-10-17");
    recorder.addMetadata("Metadata", "operator", "Florin");
    recorder.addMetadata("Metadata", "sampling_rate", "10000");

    // Simulare semnale
    std::vector<float> vibration, rotation;
    int n = 1000;
    for (int i = 0; i < n; ++i) {
        vibration.push_back(0.02f * sin(2 * M_PI * 120 * i / 10000.0f));
        rotation.push_back(1500.0f + 20.0f * sin(2 * M_PI * 2 * i / 10000.0f));
    }

    recorder.addFloatVector("Sensors/Vibrations", "ch1", vibration, "m/s^2");
    recorder.addFloatVector("Sensors/RotationSpeed", "data", rotation, "RPM");

    return 0;
}
