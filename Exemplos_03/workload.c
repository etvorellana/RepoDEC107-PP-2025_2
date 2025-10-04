/* workload.c
 *
 * Exemplo de uso de DGEMM (CBLAS) para multiplicação de matrizes n×n.
 * A função workLoad(n) aloca A, B, C; executa C = A*B via DGEMM e
 * retorna a estimativa de FLOPs (2*n^3) como int (com saturação).
 *
 * Compilação (OpenBLAS):
 *   gcc workload.c -O3 -march=native -lopenblas -o workload
 *
 * Alternativas:
 *   # Referência BLAS + CBLAS:
 *   gcc workload.c -O3 -lblas -lcblas -o workload
 *
 * Pacotes Ubuntu (sugestão):
 *   sudo apt-get update
 *   sudo apt-get install -y build-essential libopenblas-dev
 *   # Alternativa à OpenBLAS (não simultânea):
 *   # sudo apt-get install -y libblas-dev libcblas-dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cblas.h>


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


int main(int argc, char** argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 1024;
    double flops_est = workLoad(n);
    if (flops_est < 0) return 1;
    printf("DGEMM n=%d -> FLOPs estimados: %lf", n, flops_est);
    printf(" (~2*n^3 + n^2)\n");
    printf("DGEMM n=%d -> GFLOPs estimados: %lf\n", n, flops_est/1e9);
    
    
    return 0;
}

