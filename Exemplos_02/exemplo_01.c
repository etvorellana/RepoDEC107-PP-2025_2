#include <stdio.h>
#include <omp.h>

int main() {
    double x[]= {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

    for(int i = 0; i < 8; i++) {
        printf("%f\n", x[i]);
    }
    int k;
    //for (int i =0; i < 4; i++)
    #pragma omp  parallel for num_threads(4)
    for (int i = 0; i < 8; i++) {
        //int i = omp_get_thread_num();
        k = omp_get_num_threads();
        x[i] += 1;
    }

    printf("k = %d\n", k);

    printf("______________________\n");
    for(int i = 0; i < 8; i++) {
        printf("%f\n", x[i]);
    }

    return 0;
}