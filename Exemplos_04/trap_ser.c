#include <stdio.h>

double f(double x);

int main(int argc, char *argv[])
{
    double integral, a = 0.0, b = 1.0, h, x;
    int n = 512, i;
    h = (b - a) / n;
    integral = (f(a) + f(b)) * 0.5;
    x = a;
    for (i = 1; i <= n - 1; i++)
    {
        x += h;
        integral += f(x);
    }
    integral *= h;
    printf("Com n=%d trapezios, a estimativa da\n", n);
    printf("integral de %.2f a %.2f e: %.6f\n", a, b,
           integral);
    return 0;
}
double f(double x)
{
    return x * x + 1.0;
}


