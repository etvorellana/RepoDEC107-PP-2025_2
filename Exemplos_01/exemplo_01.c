#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    printf("Como avaliar o desempenho: métricas de desempenho!\n");
    unsigned int contador = 0;
    double val = 1.0;
    // simula carga de trabalho
    unsigned int step = UINT_MAX / 10;
    int iter = 5;
    if (argc > 1)
        iter = atoi(argv[1]);
    printf("Número de iterações: %d\n", iter);
    for( int i = 0; i < 10; i++)
    {
        printf("[]");
        for(contador = 0; contador < step ; contador++)
        {
            for(int j = 0; j < iter; j++)
            {
                val *= 2.0;
                val /= 2.0;
            }
        }
        fflush(stdout);
    }
    printf("\n");
    return 0;
}

/** 
 * Como compilar os testes
 * Sem otimização (-O0)
 * $ gcc -o bin/main_00 exemplo_01.c
 * Com otimização
 * $ gcc -o bin/main_01 -O1 exemplo_01.c
 * $ gcc -o bin/main_02 -O2 exemplo_01.c
 * $ gcc -o bin/main_03 -O3 exemplo_01.c
 * 
 * Como fazer os testes
 * $ time ./bin/main_00
 * $ time ./bin/main_01
 * $ time ./bin/main_02
 * $ time ./bin/main_03
*/