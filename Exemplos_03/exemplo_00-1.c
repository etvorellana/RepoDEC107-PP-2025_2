
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

gcc -o run_00-1 -Wall -O3 exemplo_00-1.c
gcc -o run_00-1_omp -Wall -O3 -fopenmp exemplo_00-1.c

*/

int main(){

  int nThread[20];
  int tThreads;
  int i;

  nThread[0] = omp_get_thread_num();     // número da thread
  tThreads = omp_get_num_threads();   // quantidade de threads 
  
  printf("nThread = %d\n", nThread[0]);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");
  
  #pragma omp parallel
  {
    int nT;
    nT = omp_get_thread_num();
    nThread[nT] = nT;
    if(nT == 0) tThreads = omp_get_num_threads();
  }

  for(i = 0; i < tThreads; i++){
    printf("nThreads = %d\n", nThread[i]);
  }
  printf("tThreads = %d\n", tThreads);
  return 0;
}
