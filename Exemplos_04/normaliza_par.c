#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv){
    int rank, p;
    int n = 256; // Tamanho do vetor
    double local_max = 0.0, global_max = 0.0;
    double *vector = NULL;
    double *local_vector = NULL;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int local_n = n / p; // Tamanho do subvetor para cada processo
    
    if (rank == 0) {
        srand(123456789);
        vector = (double *)malloc(n * sizeof(double));
        for (int i = 0; i < n; i++) {
            vector[i] = ((double)rand()/RAND_MAX) * 100.0 - 50.0; // Inicializa o vetor com valores de -50 a 50
        }
        for(int i = 1; i < p; i++) {
            MPI_Send(vector + i*local_n, local_n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        local_vector = vector; // O processo 0 também trabalha com seu próprio segmento
    }else{
        local_vector = (double *)malloc(local_n * sizeof(double));
        MPI_Recv(local_vector, local_n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    local_max = local_vector[0];
    for (int i = 1; i < local_n; i++) {
        if (local_vector[i] > local_max) {
            local_max = local_vector[i];
        }
    }
    
    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    
    if (rank == 0) {
        printf("Valor máximo: %f\n", global_max);
    }
    
    MPI_Bcast(&global_max, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++) {
        local_vector[i] /= global_max; // Normaliza o subvetor
    }

    if (rank == 0) {
        for(int i = 1; i < p; i++) {
            MPI_Recv(vector + i*local_n, local_n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("Vetor normalizado:\n");
        for (int i = 0; i < n; i++) {
            printf("%f ", vector[i]);
            if (i % 10 == 9) printf("\n");
        }
        printf("\n");
        free(vector);
    } else {
        MPI_Send(local_vector, local_n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        free(local_vector);
    }

    MPI_Finalize();
    return 0;
}