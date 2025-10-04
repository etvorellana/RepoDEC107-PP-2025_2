#include <stdio.h>
#include <stdlib.h>

//Compilação Condicional
#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
  #define omp_get_num_threads() 1 
#endif

/* Como compilar

gcc -o run_06 -Wall -O3 -fopenmp exemplo_06.c

*/

int main(){

    double x[100];

    srand(1234567890);

    for(int i=0; i < 100; i++){
        x[i] = ((double) rand())/RAND_MAX;
    }

    double max = x[0];
    double soma1 = 0.0;
    double soma2 = 0.0;

    for(int i=0; i < 100; i++){
        if (max < x[i])
            max = x[i];
    }
    printf("%.6lf\n", max);

    for(int i=1; i < 100; i++){
      if (x[i] < max/2)
        soma1 += x[i];
      else
        soma2 += x[i];
    }

    printf("%.6lf\n", soma1);
    printf("%.6lf\n", soma2);

    return 0;

}