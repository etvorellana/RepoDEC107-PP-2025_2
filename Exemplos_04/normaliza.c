#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 256; // Tamanho do vetor
    double max = 0.0;
    double vector[256];
    srand(123456789);
    // Inicializa o vetor com valores de -50 a 50
    for (int i = 0; i < n; i++) {
        vector[i] = ((double)rand()/RAND_MAX) * 100.0 - 50.0;
    }

    // Encontra o valor máximo
    max = vector[0];
    for (int i = 1; i < n; i++) {
        if (vector[i] > max) {
            max = vector[i];
        }
    }
    printf("Valor máximo: %f\n", max);

    // Normaliza o vetor
    for (int i = 0; i < n; i++) {
        vector[i] /= max;
    }

    // Imprime o vetor normalizado
    printf("Vetor normalizado:\n");
    for (int i = 0; i < n; i++) {
        printf("%f ", vector[i]);
        if (i % 10 == 9) printf("\n");
    }
    printf("\n");

    return 0;
}