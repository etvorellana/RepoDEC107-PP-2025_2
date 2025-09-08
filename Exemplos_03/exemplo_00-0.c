
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

gcc -o run_00-0 -Wall -O3 exemplo_00-0.c
gcc -o run_00-0_omp -Wall -O3 -fopenmp exemplo_00-0.c

*/

int main(){

  int nThread, tThreads;

  nThread = omp_get_thread_num();     // número da thread
  tThreads = omp_get_num_threads();   // quantidade de threads 
  
  printf("nThread = %d\n", nThread);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");

  return 0;
}
