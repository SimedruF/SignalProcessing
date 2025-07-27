#include "../source/SignalProcessing.h"
#include <stdio.h>

int main() {
    SignalProcessing sp;
    for (int i = 0; i < 10; ++i) sp.AddValue(i + 1); // 1,2,...,10
    printf("Mean: %f\n", sp.GetMean());
    printf("Variance: %f\n", sp.GetVariance());
    printf("StdDev: %f\n", sp.GetStandardDeviation());
    return 0;
}
