#include "../source/SignalProcessing.h"
#include <stdio.h>

int main() {
    SignalProcessing sp;
    for (int i = 0; i < 10; ++i) sp.AddValue(i + 1); // 1,2,...,10
    sp.NormalizeVector();
    printf("Normalized vector: ");
    for (int i = 0; i < sp.GetIndex(); ++i) printf("%f ", sp.GetLastValue());
    printf("\n");
    sp.ScaleVector(-1.0, 1.0);
    printf("Scaled vector [-1,1]: ");
    for (int i = 0; i < sp.GetIndex(); ++i) printf("%f ", sp.GetLastValue());
    printf("\n");
    return 0;
}
