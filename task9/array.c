#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rnd(const int lo, const int hi) { return lo + rand() % (hi - lo + 1); }

Status swap_minmax(int *a, const int n, int *imin, int *imax)
{
    int i, t;
    if(a == NULL || imin == NULL || imax == NULL || n <= 0) return ST_INVALID_INPUT;
    *imin = *imax = 0;
    for(i = 1; i < n; i++) {
        if (a[i] < a[*imin]) *imin = i;
        if (a[i] > a[*imax]) *imax = i;
    }
    t = a[*imin]; a[*imin] = a[*imax]; a[*imax] = t;
    return ST_OK;
}

static int cmp_int(const void *x, const void *y)
{
    int p = *(const int *)x;
    int q = *(const int *)y;
    return (p > q) - (p < q);
}

static int find_closest(const int val, const int *sorted_b, const int nb)
{
    int left = 0, right = nb;
    while(left < right) {
        int mid = left + (right - left) / 2;
        if(sorted_b[mid] < val) left = mid + 1;
        else right = mid;
    }
    if(left == 0) return sorted_b[0];
    if(left == nb) return sorted_b[nb - 1];
    {
        long dl = (long)val - sorted_b[left - 1];
        long dr = (long)sorted_b[left] - val;
        return dl <= dr ? sorted_b[left - 1] : sorted_b[left];
    }
}

Status build_c(const int *a, const int na, const int *b, const int nb, int *c)
{
    int i, *sorted;
    if(a == NULL || b == NULL || c == NULL || na <= 0 || nb <= 0) return ST_INVALID_INPUT;
    sorted = malloc((size_t)nb * sizeof(int));
    if(sorted == NULL) return ST_NO_MEMORY;
    memcpy(sorted, b, (size_t)nb * sizeof(int));
    qsort(sorted, (size_t)nb, sizeof(int), cmp_int);
    for(i = 0; i < na; i++) c[i] = a[i] + find_closest(a[i], sorted, nb);
    free(sorted);
    return ST_OK;
}

void show(const char *title, const int *a, const int n, const int limit)
{
    int i, m;
    if(title == NULL || a == NULL || n < 0) return;
    m = (limit > 0 && n > limit) ? limit : n;
    printf("%s", title);
    for(i = 0; i < m; i++) printf(" %d", a[i]);
    if(m < n) printf(" ... всего %d", n);
    printf("\n");
}
