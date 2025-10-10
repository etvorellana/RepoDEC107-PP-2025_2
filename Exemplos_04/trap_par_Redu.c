#include <stdio.h>
#include <mpi.h>

double f(double);
double Trap(double, double, int, double);

int main(int argc, char **argv){
    double integral, a, b, h, x;
    int n, i;
    double local_a, local_b, total;
    int local_n, rank, p;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    /*
    Vamos ler os limites do intervalo (a e b) e 
    o número de subintervalos (n) do arquivo de entrada
    */
    FILE *fp;
    if (rank == 0){
        fp = fopen("input.txt", "r");
        if (fp == NULL){
            printf("Erro ao abrir o arquivo de entrada.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(fp, "%lf %lf %d", &a, &b, &n);
        fclose(fp);
    }
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    h = (b - a) / n;
    local_n = n / p;
    local_a = a + rank * local_n * h;
    local_b = local_a + local_n * h;
    integral = Trap(local_a, local_b, local_n, h);
    MPI_Reduce(&integral, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0){
        printf("Com n=%d trapezios, a estimativa da\n", n);
        printf("integral de %.2f a %.2f e: %.6f\n", a, b,total);
        printf("O valor exato desta integral é 4/3 = %.6f\n", 4.0 / 3.0);
    }
    MPI_Finalize();
    return 0;
}

double f(double x)
{
    return x * x + 1.0;
}
double Trap(double local_a, double local_b, int local_n,
            double h)
{
    double integral, x;
    int i;
    integral = (f(local_a) + f(local_b)) * 0.5;
    x = local_a;
    for (i = 1; i <= local_n - 1; i++)
    {
        x += h;
        integral += f(x);
    }
    integral *= h;
    return integral;
}



