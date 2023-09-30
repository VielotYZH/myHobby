#include <stdio.h>
#include <math.h>

double CalculateProbability(int n, double p, int k);

void main(void)
{
    printf("%f", CalculateProbability(35, 0.1, 10));
}

/*
 * This function is used to calculate the probability
 * that X is less than or equal to k
 * in a binomial distribution*/
double CalculateProbability(int n, double p, int k)
{
    double result = 0.0;

    double A;
    double B;
    for (int i = 0; i <= k; i++)
    {
        A = 1;
        for (int j = i + 1; j <= n; j++)
        {
            A *= j;
        }

        B = 1;
        for (int j = 1; j <= n - i; j++)
        {
            B *= j;
        }

        result += A / B * pow(1 - p, n - i) * pow(p, i);
    }

    return result;
}