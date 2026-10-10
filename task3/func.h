#ifndef FUNC_H
#define FUNC_H

#include "status.h"

typedef struct {
    double a, b, c, x1, x2;
    int count;
} Solution;

Status solve_perms(const double eps, const double a, const double b, const double c, Solution *out, int *n);
Status multiple(const long a, const long b, int *yes);
Status right_triangle(const double eps, const double a, const double b, const double c, int *yes);

#endif
