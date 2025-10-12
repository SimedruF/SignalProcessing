#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <math.h>

int main() {
    SignalProcessing sp;
    
    // Test 1: Threshold crossing detection
    printf("=== Test 1: Threshold Crossing Detection ===\n");
    // Add a sine wave signal that crosses threshold 0.5
    for (int i = 0; i < 20; ++i) {
        double value = sin(i * 0.5);
        sp.AddValue(value);
    }
    
    int events[100];
    int event_count = sp.DetectThresholdCrossing(0.5, 0, events); // Detect both directions
    
    printf("Threshold crossings detected (threshold=0.5): %d\n", event_count);
    printf("Crossing indices: ");
    for (int i = 0; i < event_count; ++i) {
        printf("%d ", events[i]);
    }
    printf("\n");
    printf("Threshold crossing flag: %s\n", sp.GetThresholdCrossingFlag() ? "TRUE" : "FALSE");
    
    // Test 2: Zero crossing detection
    printf("\n=== Test 2: Zero Crossing Detection ===\n");
    sp.ClearVector();
    
    // Add signal that crosses zero
    for (int i = 0; i < 20; ++i) {
        double value = sin(i * 0.3) * 2.0 - 0.5;
        sp.AddValue(value);
    }
    
    event_count = sp.DetectZeroCrossing(0, events); // Detect both directions
    
    printf("Zero crossings detected: %d\n", event_count);
    printf("Crossing indices: ");
    for (int i = 0; i < event_count; ++i) {
        printf("%d ", events[i]);
    }
    printf("\n");
    printf("Zero crossing flag: %s\n", sp.GetZeroCrossingFlag() ? "TRUE" : "FALSE");
    
    // Test 3: Rising edge only detection
    printf("\n=== Test 3: Rising Edge Detection (threshold=0) ===\n");
    sp.ClearVector();
    
    // Add sawtooth signal
    for (int i = 0; i < 10; ++i) {
        sp.AddValue(-2.0 + i * 0.5);
    }
    
    event_count = sp.DetectThresholdCrossing(0.0, 1, events); // Rising edge only
    
    printf("Rising edge crossings detected: %d\n", event_count);
    printf("Crossing indices: ");
    for (int i = 0; i < event_count; ++i) {
        printf("%d ", events[i]);
    }
    printf("\n");
    
    // Test 4: Falling edge only detection
    printf("\n=== Test 4: Falling Edge Detection (threshold=1.0) ===\n");
    sp.ClearVector();
    
    // Add signal with peaks
    for (int i = 0; i < 20; ++i) {
        double value = 2.0 * sin(i * 0.5);
        sp.AddValue(value);
    }
    
    event_count = sp.DetectThresholdCrossing(1.0, -1, events); // Falling edge only
    
    printf("Falling edge crossings detected: %d\n", event_count);
    printf("Crossing indices: ");
    for (int i = 0; i < event_count; ++i) {
        printf("%d ", events[i]);
    }
    printf("\n");
    
    // Test 5: Clear flags
    printf("\n=== Test 5: Clear Event Flags ===\n");
    printf("Before clear - Threshold flag: %s, Zero crossing flag: %s\n", 
           sp.GetThresholdCrossingFlag() ? "TRUE" : "FALSE",
           sp.GetZeroCrossingFlag() ? "TRUE" : "FALSE");
    
    sp.ClearEventFlags();
    
    printf("After clear - Threshold flag: %s, Zero crossing flag: %s\n", 
           sp.GetThresholdCrossingFlag() ? "TRUE" : "FALSE",
           sp.GetZeroCrossingFlag() ? "TRUE" : "FALSE");
    
    return 0;
}
