
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

gcc -o run_00-5 -Wall -O3 -fopenmp exemplo_00-5.c

*/

int main( int argc, char** argv ){

  int nThread[20];
  int tThreads;
  int i, n;
  
  nThread[0] = omp_get_thread_num();
  tThreads = omp_get_num_threads();
  
  printf("nThread = %d\n", nThread[0]);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");
  // Testando a clausula if
  // Argumento da linha de comando
	if(argc > 1)
    n = atoi(argv[1]);
	else
		n = 10;
  
  #pragma omp parallel if(n > 10) num_threads(10)
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
