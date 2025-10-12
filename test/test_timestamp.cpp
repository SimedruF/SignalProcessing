#include "../source/SignalProcessing.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
    SignalProcessing sp;
    struct timespec ts;
    
    printf("=== Test 1: Add values with timestamp ===\n");
    
    // Add 5 values with their timestamps
    for (int i = 0; i < 5; ++i) {
        // Get current time
        clock_gettime(CLOCK_REALTIME, &ts);
        
        // Add value with timestamp
        double value = 10.0 + i * 2.5;
        int index = sp.AddValueWithTimestamp(value, ts);
        
        printf("Added value %.2f at index %d with timestamp: %ld.%09ld\n", 
               value, index - 1, ts.tv_sec, ts.tv_nsec);
        
        // Small delay to see time difference
        usleep(100000); // 100ms
    }
    
    printf("\n=== Test 2: Retrieve values with timestamps ===\n");
    
    // Retrieve and display all values with their timestamps
    struct timespec retrieved_ts;
    for (int i = 0; i < sp.GetIndex(); ++i) {
        sp.GetTimespec(&retrieved_ts, i);
        printf("Index %d: timestamp = %ld.%09ld\n", 
               i, retrieved_ts.tv_sec, retrieved_ts.tv_nsec);
    }
    
    printf("\n=== Test 3: Manual timestamp creation ===\n");
    
    // Clear vector and add values with custom timestamps
    sp.ClearVector();
    
    for (int i = 0; i < 3; ++i) {
        struct timespec custom_ts;
        custom_ts.tv_sec = 1700000000 + i * 100;  // Custom epoch time
        custom_ts.tv_nsec = i * 1000000;           // Custom nanoseconds
        
        double value = 100.0 + i * 10.0;
        sp.AddValueWithTimestamp(value, custom_ts);
        
        printf("Added value %.2f with custom timestamp: %ld.%09ld\n", 
               value, custom_ts.tv_sec, custom_ts.tv_nsec);
    }
    
    printf("\n=== Test 4: Verify stored timestamps ===\n");
    
    for (int i = 0; i < sp.GetIndex(); ++i) {
        sp.GetTimespec(&retrieved_ts, i);
        printf("Retrieved timestamp at index %d: %ld.%09ld\n", 
               i, retrieved_ts.tv_sec, retrieved_ts.tv_nsec);
    }
    
    printf("\n=== Test 5: Compare with regular AddValue ===\n");
    
    sp.ClearVector();
    
    // Add with timestamp
    clock_gettime(CLOCK_REALTIME, &ts);
    sp.AddValueWithTimestamp(50.0, ts);
    printf("AddValueWithTimestamp: added 50.0\n");
    
    // Add without timestamp (using regular AddValue)
    sp.AddValue(60.0);
    printf("AddValue (regular): added 60.0\n");
    
    // Check if timestamp was stored only for first value
    struct timespec ts1, ts2;
    sp.GetTimespec(&ts1, 0);
    sp.GetTimespec(&ts2, 1);
    
    printf("Timestamp at index 0: %ld.%09ld\n", ts1.tv_sec, ts1.tv_nsec);
    printf("Timestamp at index 1: %ld.%09ld (from AddValue, may be empty/uninitialized)\n", 
           ts2.tv_sec, ts2.tv_nsec);
    
    printf("\n=== Test Complete ===\n");
    printf("Total values in vector: %d\n", sp.GetIndex());
    
    return 0;
}
