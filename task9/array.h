#ifndef ARRAY_H
#define ARRAY_H

#include "status.h"

int rnd(const int lo, const int hi);
Status swap_minmax(int *a, const int n, int *imin, int *imax);
Status build_c(const int *a, const int na, const int *b, const int nb, int *c);
void show(const char *title, const int *a, const int n, const int limit);

#endif
