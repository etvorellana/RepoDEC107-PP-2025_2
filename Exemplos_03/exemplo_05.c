
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

//Compilação Condicional
#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
  #define omp_get_num_threads() 1 
#endif

void carga(int N);
int alpha();
int beta(int a);
int gama(int a);
int delta(int a);
int epsilon(int b, int c);
int zeta(int d, int e);

/* Como compilar

gcc -o run_05 -Wall -O3 -fopenmp exemplo_05.c

*/

int main(){

  int nThread[20];
  int tThreads;
  int delayTime[10] = {2, 4, 8, 32, 32, 2048, 128, 128, 512, 512};
  double thTime[10];
  int x[100];
  int a, b, c, d, e, f;
  
  nThread[0] = omp_get_thread_num();
  tThreads = omp_get_num_threads();
  
  printf("nThread = %d\n", nThread[0]);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");

  double start = omp_get_wtime();
  for (int i = 0; i < 10; i++){
    thTime[i] = start; 
  }
  #pragma omp parallel num_threads(4)
  {
    int nT;
    nT = omp_get_thread_num();
    carga(delayTime[(2*(nT+1))+1]);
    nThread[nT] = nT;
    if(nT == 0) tThreads = omp_get_num_threads();
    #pragma omp for
    for(int j = 0 ;j < 100; j++){
      x[j] = nT;
      carga(delayTime[(2*(nT+1))+1]);
    }
    carga(delayTime[(2*(nT+1))+1]);
    thTime[nT] = omp_get_wtime() - thTime[nT];
   
    #pragma omp single
        a = alpha();

    #pragma omp sections
    {
      #pragma omp section
      {
        b = beta(a);
      }
      #pragma omp section
      {
        c = gama(a);
      }
    }
    #pragma omp sections
    {
      #pragma omp section
      {
        d = delta(a);
      }
      #pragma omp section
      {
        e = epsilon(b, c);
      }
    }
  }
  f = zeta(d, e);
  printf("a= %d, b= %d, c= %d, d= %d, e= %d, f= %d\n", a, b, c, d, e, f);

  double tMin, tMax;
  tMin = tMax = thTime[0];
  printf("nThreads = %d, ", nThread[0]);
  printf("delayTime = %d, ", delayTime[3]);
  printf("thTime = %.12lf\n", thTime[0]);
  for(int i = 1; i < tThreads; i++){
    printf("nThreads = %d, ", nThread[i]);
    printf("delayTime = %d, ", delayTime[(2*(i+1))+1]);
    printf("thTime = %.12lf\n", thTime[i]);
    if (tMin > thTime[i]){
      tMin = thTime[i];
    }else if (tMax < thTime[i]){
      tMax = thTime[i];
    }
  }
  printf("tMin = %.12lf, tMax = %.12lf\n", tMin, tMax);
  printf("FBC = %.2lf\n", (tMax - tMin)/tMax);

  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 10; j++){
      printf("%d ", x[i*10 + j]);
    }
    printf("\n");
  }
  printf("tThreads = %d\n", tThreads);
  return 0;
}

// Função de coplexidade N^3
void carga(int N) 
{ 
  double *A;
  double *B;
  double *C;
  A = (double *)malloc(N*N*sizeof(double));
  B = (double *)malloc(N*N*sizeof(double));
  C = (double *)malloc(N*N*sizeof(double));
  
  for(int i = 0; i < N*N; i++){
    A[i] = 1.0;
    B[i] = -1.0;
    C[i] = 0.0; 
  }

  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      for(int k = 0; k < N; k++){
        C[i*N + j] += A[i*N + k]*B[k*N + j];
      }
    }
  }

  free(A);
  free(B);
  free(C);
  return;
} 

int alpha(){
  carga(256);
  return 1024; 
}

int beta(int a){
  carga(a);
  carga(256);
  return(512);
}

int gama(int a){
  carga(a);
  carga(512);
  return(2048);
}

int delta(int a){
  carga(a);
  carga(1024);
  return(64);
}

int epsilon(int b, int c){
  carga(b);
  carga(c);
  carga(256);
  return(2048);
}

int zeta(int d, int e){
  carga(d);
  carga(e);
  carga(512);
  return(64);
}