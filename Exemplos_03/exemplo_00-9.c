
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <cblas.h>

//Compilação Condicional
#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
  #define omp_get_num_threads() 1 
#endif

double workLoad(int n);
/* Como compilar
gcc exemplo_00-8.c -Wall -O3 -fopenmp -lblas -o bin/run_00-8
 

*/

int main(){

  int nThread[20];
  double work4thread[20];
  int tThreads;
  int load[10] = {2*256, 2*4196, 2*1024, 2*128, 2*1024, 2*2048, 2*128, 2*128, 2*512, 2*512};
  double thTime[10];
  int x[100];
  
  nThread[0] = omp_get_thread_num();
  tThreads = omp_get_num_threads();
  printf("Região sequencial, thread master\n");
  printf("nThread = %d\n", nThread[0]);
  printf("tThreads = %d\n", tThreads);
  printf("---------------------------\n");


  // Inicio da medida de tempo
  double start = omp_get_wtime();
  
  #pragma omp parallel num_threads(5)  
  {
    int nT;
    double thWork;
    nT = omp_get_thread_num();
    if(nT == 0) tThreads = omp_get_num_threads();
    thTime[nT] = start;
    nThread[nT] = nT;
    thWork = (load[2*nT])/1e9; // em GFLOPs
    #pragma omp for schedule(static, 5)
    for(int j = 0 ;j < 100; j++){
      x[j] = nT;
      thWork += (load[(2*nT)]);
      thWork += (load[(2*nT)+1]);
    }
    thWork += workLoad(load[(2*(nT+1))]);
    thTime[nT] = omp_get_wtime() - thTime[nT];
    work4thread[nT] = thWork;
  }

  double tMin, tMax;
  double wMax, wMin;
  int nT;
  wMin = wMax = work4thread[0];
  int t_1, t_n;
  int w_1, w_n;
  t_1 = t_n = 0;
  w_1 = w_n = 0;
  tMin = tMax = thTime[0];

  nT = 0;
  printf("nThreads = %d,  ", nThread[0]);
  printf("load = %d, %d, ", load[2*nT], load[(2*nT)+1]);
  printf("thTime = %.12lf, ", thTime[nT]);
  printf("eork4thread = %.2lf\n", work4thread[nT]);
  for(int nT = 1; nT < tThreads; nT++){
    printf("nThreads = %d, ", nThread[nT]);
    printf("load = %d, %d,  ", load[(2*nT)], load[(2*nT)+1]);
    printf("thTime = %.12lf, ", thTime[nT]);
    printf("work4thread = %.2lf\n", work4thread[nT]);
    if (tMin > thTime[nT]){
      tMin = thTime[nT];
      t_1 = nT;
    }else if (tMax < thTime[nT]){
      tMax = thTime[nT];
      t_n = nT;
    }
    if (wMin > work4thread[nT]){
      wMin = work4thread[nT];
      w_1 = nT;
    }else if (wMax < work4thread[nT]){
      wMax = work4thread[nT];
      w_n = nT;
    }
  }
  printf("tMin = %.12lf (th %d), tMax = %.12lf (th %d)\n", tMin, t_1, tMax, t_n);
  printf("FBC = %.2lf\n", (tMax - tMin)/tMax);

  printf("wMin = %.2lf (th %d), wMax = %.2lf (th %d)\n", wMin, w_1, wMax, w_n);

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
double workLoad(int n) {
    if (n <= 0) return 0;

    // Alocação das matrizes (row-major)
    double *A = (double*)aligned_alloc(64, (size_t)n * n * sizeof(double));
    double *B = (double*)aligned_alloc(64, (size_t)n * n * sizeof(double));
    double *C = (double*)aligned_alloc(64, (size_t)n * n * sizeof(double));
    if (!A || !B || !C) {
        fprintf(stderr, "Falha ao alocar matrizes.\n");
        free(A); free(B); free(C);
        return -1;
    }

    // Inicialização simples e determinística
    for (int i = 0; i < n*n; ++i) {
        A[i] = (double)((i % 97) - 48) / 97.0;  // valores pequenos
        B[i] = (double)(((i*7) % 89) - 44) / 89.0;
        C[i] = 0.0;
    }

    // DGEMM: C = alpha*A*B + beta*C
    const double alpha = 1.0;
    const double beta  = -1.0;

    // Usando layout RowMajor para conveniência
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans, CblasNoTrans,
        n,      // m
        n,      // n
        n,      // k
        alpha,
        A, n,   // A, lda
        B, n,   // B, ldb
        beta,
        C, n    // C, ldc
    );

    // Estimativa de FLOPs para beta=0: 2*n^3
    // (se beta!=0, acrescente +n^2)
    double flops = 2.0 * (double)n * (double)n * (double)n + (beta != 0.0 ? (double)n * (double)n : 0.0);
    //int flops_int = saturate_to_int_double(flops);

    free(A);
    free(B);
    free(C);

    return flops;
}