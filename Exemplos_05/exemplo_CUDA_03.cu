#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

#define MSIZE 8192
#define BSIZE 32

__host__ int dgemmCUDA(double alpha, double* A, double* B, double beta, double* C);
__host__ void printMatrix(double *A, int n);

int main(int argc, char **argv)
{
    //Alocar as matrizes A, B e C
    double *A = (double* ) malloc(MSIZE*MSIZE*sizeof(double));
    double *B = (double* ) malloc(MSIZE*MSIZE*sizeof(double));
    double *C = (double* ) malloc(MSIZE*MSIZE*sizeof(double));
    // I/O para inicializar as matrizes
    for (int i = 0; i < MSIZE; i++)
    {
        for (int j = 0; j < MSIZE; j++)
        {
            // os elementos de A guardam valores -1
            A[i*MSIZE + j] = -1.0;
            // A matriz B é a identidade
            B[i*MSIZE + j] = 0.0;
            // os elementos de C guardam valores 1
            C[i*MSIZE + j] = 1.0;
        }
        B[i*MSIZE + i] = 1.0;
    }
    double alpha = 2.0, beta = 2.0;

    printf("Matriz A:\n");

    printMatrix(A, MSIZE);

    printf("Matriz B:\n");

    printMatrix(B, MSIZE);

    printf("Matriz C antes da operacao:\n");

    printMatrix(C, MSIZE);

    // Calcular o GEMM
    dgemmCUDA(alpha, A, B, beta, C);

    // I/O para guardar os resultados
    printf("Matriz C apos a operacao:\n");

    printMatrix(C, MSIZE);

    // Libera memória alocada
    free(A);
    free(B);
    free(C);
    
}


__host__ int dgemmCUDA(double alpha, double* A, double* B, double beta, double* C)
{
    int tSize = MSIZE * MSIZE * sizeof(double);
    double *Ad, *Bd, *Cd;
    int M , K, N;
	int lda, ldb, ldc;

    M = K = N = lda = ldb = ldc = MSIZE;

    // Alocar memória para as matrizes no diveice
    if ( cudaSuccess != cudaMalloc((void**)&Ad, tSize))
    {
        printf("Erro alocando A!!\n");
        return 1;
    }
    if ( cudaSuccess != cudaMalloc((void**)&Bd, tSize))
    {
        printf("Erro alocando B!!\n");
        return 1;
    }
    if ( cudaSuccess != cudaMalloc((void**)&Cd, tSize))
    {
        printf("Erro alocando C!!\n");
        return 1;
    }

    // Copiar as matrizes do host para os respectivos espços de memória no divice
    cudaError_t cudaErro;
    cudaErro = cudaMemcpy(Ad, A, tSize, cudaMemcpyHostToDevice);
    if(cudaErro != cudaSuccess)
    {
        printf("Erro copiando A para o device!!\n");
        return 2;
    }
    cudaErro = cudaMemcpy(Bd, B, tSize, cudaMemcpyHostToDevice);
    if(cudaErro != cudaSuccess)
    {
        printf("Erro copiando B para o device!!\n");
        return 2;
    }
    cudaErro = cudaMemcpy(Cd, C, tSize, cudaMemcpyHostToDevice);
    if(cudaErro != cudaSuccess)
    {
        printf("Erro copiando C para o device!!\n");
        return 2;
    }

    // Definir a grade que se deseja utilizar

    cublasHandle_t handle;
    cublasStatus_t cublasErro;
	cublasCreate(&handle);
    /*
    cublasStatus_t cublasDgemm(cublasHandle_t handle,
                           cublasOperation_t transa, cublasOperation_t transb,
                           int m, int n, int k,
                           const double          *alpha,
                           const double          *A, int lda,
                           const double          *B, int ldb,
                           const double          *beta,
                           double          *C, int ldc)
    */
	cublasErro = cublasDgemm(  handle, 
                                CUBLAS_OP_N, CUBLAS_OP_N, 
                                M, N, K, &alpha, Ad, lda, Bd, ldb, &beta, Cd, ldc);
    if ( CUBLAS_STATUS_SUCCESS != cublasErro)
        printf( "Erro no cublasDgemm!\n" );

	cublasDestroy(handle);

    // copiar a matriz resultante do device para o espaço de meória do host
    cudaErro = cudaMemcpy(C, Cd, tSize, cudaMemcpyDeviceToHost);
    if(cudaErro != cudaSuccess)
    {
        printf("Erro copiando C para o host!!\n");
        return 3;
    }

    //Liberar a memória no divice
    cudaFree(Ad);
    cudaFree(Bd);
    cudaFree(Cd);
    return 0;
}

__host__ void printMatrix(double *A, int n)
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
    }else{
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