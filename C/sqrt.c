#include <stdio.h>

#define Z_CONJECTURE 1.0

double Sqrt(double x)
{
    double z = Z_CONJECTURE;
    double last = 0.0;
    for (int i = 0; i < 10; i++)
    {
        z -= (z * z - x) / (2 * z);
        printf("%f\n", z);
    }
    return z;
}

void main()
{
    printf("%f\n", Sqrt(7));
}