
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

gcc -o run_00-12 -Wall -O3 -fopenmp exemplo_00-12.c

*/

int main(){

  int nThread[20];
  int tThreads;
  int j, n = 20;
  int x[100];
  
  nThread[0] = omp_get_thread_num();
  tThreads = omp_get_num_threads();
  
  printf("nThread = %d\n", nThread[0]);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");
  
  #pragma omp parallel
  {
    
    int nT;
    nT = omp_get_thread_num();
    nThread[nT] = nT;
    if(nT == 0) tThreads = omp_get_num_threads();
    //#pragma omp for
    #pragma omp for ordered
    for(j = 0 ;j < 100; j++){
      x[j] = nT;
      #pragma omp ordered
      {
        if (j%10 != 9)
          printf("%d ", x[j]);
        else
          printf("%d\n", x[j]);
      }
    }
    
  }
  printf("tThreads = %d\n", tThreads);
  return 0;
}
