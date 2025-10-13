#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 256; // Tamanho do vetor
    double max, min;
    double vector[256];
    srand(123456789);
    // Inicializa o vetor com valores de -50 a 50
    for (int i = 0; i < n; i++) {
        vector[i] = ((double)rand()/RAND_MAX) * 100.0 - 50.0;
    }

    // Encontra o valor máximo
    max = min = vector[0];
    for (int i = 1; i < n; i++) {
        if (vector[i] > max) {
            max = vector[i];
        }else if (vector[i] < min){
            min = vector[i];
        }
    }
    
    printf("Valor máximo: %f \t Valor mínimo: %f\n", max, min);

    // Normaliza o vetor
    for (int i = 0; i < n; i++) {
        vector[i] -= min;
        vector[i] /= max - min;
        vector[i] *= 2.0;
        vector[i] -= 1.0;
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


