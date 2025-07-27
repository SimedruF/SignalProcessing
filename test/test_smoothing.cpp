#include "../source/SignalProcessing.h"
#include <stdio.h>

int main() {
    SignalProcessing sp;
    for (int i = 0; i < 10; ++i) sp.AddValue(i + 1); // 1,2,...,10
    double out_vector[10];
    sp.ExponentialSmoothing(0.5, out_vector);
    printf("Exponential smoothing (alpha=0.5): ");
    for (int i = 0; i < sp.GetIndex(); ++i) printf("%f ", out_vector[i]);
    printf("\n");
    return 0;
}
