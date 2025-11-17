#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>   // Interface C para BLAS

#define MSIZE 8192

int dgemmBLAS(double alpha, double* A, double* B, double beta, double* C);
void printMatrix(double *A, int n);

int main(int argc, char **argv)
{
    // Alocar as matrizes A, B e C
    double *A = (double* ) malloc(MSIZE*MSIZE*sizeof(double));
    double *B = (double* ) malloc(MSIZE*MSIZE*sizeof(double));
    double *C = (double* ) malloc(MSIZE*MSIZE*sizeof(double));

    if (!A || !B || !C) {
        printf("Erro ao alocar memória para as matrizes.\n");
        return 1;
    }

    // Inicializar as matrizes
    for (int i = 0; i < MSIZE; i++)
    {
        for (int j = 0; j < MSIZE; j++)
        {
            // os elementos de A guardam valores -1
            A[i*MSIZE + j] = -1.0;
            // A matriz B é inicialmente zero
            B[i*MSIZE + j] = 0.0;
            // os elementos de C guardam valores 1
            C[i*MSIZE + j] = 1.0;
        }
        // Definir B como identidade
        B[i*MSIZE + i] = 1.0;
    }

    double alpha = 2.0, beta = 2.0;
    
    printf("Matriz A:\n");
    printMatrix(A, MSIZE);

    printf("Matriz B:\n");
    printMatrix(B, MSIZE);

    printf("Matriz C antes da operacao:\n");
    printMatrix(C, MSIZE);
    

    // Calcular o GEMM usando BLAS
    dgemmBLAS(alpha, A, B, beta, C);

    

    printf("Matriz C apos a operacao:\n");
    printMatrix(C, MSIZE);
    
    // Liberar memória alocada
    free(A);
    free(B);
    free(C);

    return 0;
}

int dgemmBLAS(double alpha, double* A, double* B, double beta, double* C)
{
    /*
      CBLAS GEMM (double):

      cblas_dgemm(
          CblasRowMajor,    // Layout das matrizes em memória
          CblasNoTrans,     // transA = 'N'
          CblasNoTrans,     // transB = 'N'
          M, N, K,
          alpha,
          A, lda,
          B, ldb,
          beta,
          C, ldc
      );

      Aqui, todas as matrizes são MSIZE x MSIZE, em row-major, então:
      M = N = K = MSIZE
      lda = ldb = ldc = MSIZE
    */

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                MSIZE, MSIZE, MSIZE,
                alpha,
                A, MSIZE,
                B, MSIZE,
                beta,
                C, MSIZE);

    return 0;
}

void printMatrix(double *A, int n)
{
    int i, j;
    double *ptr;
    size_t lda;

    if (A == NULL)
    {
        printf("Matriz nula\n");
        return;
    }
    ptr = A;
    lda = n;

    if(n <= 8)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                printf("%.1lf ", ptr[i * lda + j]);
            printf("\n");
        }
    } else {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
                printf("%.1lf ", ptr[i * lda + j]);
            printf(" ... ");
            for (j = n - 4; j < n; j++)
                printf("%.1lf ", ptr[i * lda + j]);
            printf("\n");
        }
        printf(" ... \n");
        for(i = n - 4; i < n; i++)
        {
            for (j = 0; j < 4; j++)
                printf("%.1lf ", ptr[i * lda + j]);
            printf(" ... ");
            for (j = n - 4; j < n; j++)
                printf("%.1lf ", ptr[i * lda + j]);
            printf("\n");
        }
    }
}
