#include "../source/SignalProcessing.h"
#include <stdio.h>

int main() {
    SignalProcessing sp;
    for (int i = 0; i < 10; ++i) sp.AddValue(i + 1); // 1,2,...,10
    printf("Moving average (window=3): %f\n", sp.GetMovingAverage(3));
    double out_vector[10];
    sp.GetMovingAverageVector(out_vector, 3);
    printf("Moving average vector (window=3): ");
    for (int i = 0; i < sp.GetIndex(); ++i) printf("%f ", out_vector[i]);
    printf("\n");
    return 0;
}
