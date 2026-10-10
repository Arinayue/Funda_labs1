#include "array.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 20

int main(int argc, char **argv)
{
    long alo, blo;
    int a, b, i, imin, imax, na, nb, fixed[N], before[N];
    int *A = NULL, *B = NULL, *C = NULL;

    if(argc != 3 || parse_l(argv[1], &alo) || parse_l(argv[2], &blo)
        || alo > blo || alo < -1000000 || blo > 1000000 || blo - alo > RAND_MAX) {
        printf("task09 <a> <b>, a <= b\n");
        return 1;
    }
    a = (int)alo; b = (int)blo;
    srand((unsigned)time(NULL));
    for (i = 0; i < N; i++) before[i] = fixed[i] = rnd(a, b);
    printf("1. диапазон [%d, %d]\n", a, b);
    show("до:", before, N, 0);
    if(swap_minmax(fixed, N, &imin, &imax) != ST_OK) {
        printf("Некорректные данные\n");
        return 1;
    }
    printf("меняем %d (индекс %d) и %d (индекс %d)\n", before[imin], imin, before[imax], imax);
    show("после:", fixed, N, 0);

    na = rnd(10, 10000); nb = rnd(10, 10000);
    A = malloc((size_t)na * sizeof(int));
    B = malloc((size_t)nb * sizeof(int));
    C = malloc((size_t)na * sizeof(int));
    if(!A || !B || !C) { 
        free(A); 
        free(B); 
        free(C); 
        printf("Нет памяти\n"); 
        return 3; 
    }
    for (i = 0; i < na; i++) A[i] = rnd(-1000, 1000);
    for (i = 0; i < nb; i++) B[i] = rnd(-1000, 1000);
    if(build_c(A, na, B, nb, C) != ST_OK) {
        free(A);
        free(B);
        free(C);
        printf("Нет памяти\n");
        return 3;
    }
    printf("2. длины %d и %d\n", na, nb);
    show("A:", A, na, 12);
    show("B:", B, nb, 12);
    show("C:", C, na, 12);
    free(A); free(B); free(C);
    return 0;
}
