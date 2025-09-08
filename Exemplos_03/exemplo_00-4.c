
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

gcc -o run_00-4 -Wall -O3 -fopenmp exemplo_00-4.c

*/

int main(){

  int nThread[20];
  int tThreads;
  int i;
  
  nThread[0] = omp_get_thread_num();
  tThreads = omp_get_num_threads();
  
  printf("nThread = %d\n", nThread[0]);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");
  #ifdef _OPENMP
    omp_set_num_threads(6);
  #endif  
  #pragma omp parallel num_threads(10)
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
