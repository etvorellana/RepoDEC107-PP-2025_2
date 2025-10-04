# Roteiro dos testes 

1. gcc -o bin/run_00-0 -Wall -O3 exemplo_00-0.c
2. gcc -o bin/run_00-0_omp -Wall -O3 -fopenmp exemplo_00-0.c
3. gcc -o bin/run_00-1 -Wall -O3 exemplo_00-1.c
4. gcc -o bin/run_00-1_omp -Wall -O3 -fopenmp exemplo_00-1.c
5. gcc -o bin/run_00-2 -Wall -O3 -fopenmp exemplo_00-2.c
    5.1. export OMP_NUM_THREADS=8
    5.2. export OMP_NUM_THREADS=2
    5.3. export OMP_NUM_THREADS=4
6. gcc -o bin/run_00-3 -Wall -O3 -fopenmp exemplo_00-3.c
7. gcc -o bin/run_00-4 -Wall -O3 -fopenmp exemplo_00-4.c
8. gcc -o bin/run_00-5 -Wall -O3 -fopenmp exemplo_00-5.c
    8.1  bin/run_00-5 5
    8.2  bin/run_00-5 20
9. gcc -o bin/run_00-6 -Wall -O3 -fopenmp exemplo_00-6.c
10. gcc -o bin/run_00-7 -Wall -O3 -fopenmp exemplo_00-7.c
