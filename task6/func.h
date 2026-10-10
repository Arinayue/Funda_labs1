#ifndef FUNC_H
#define FUNC_H

#include "status.h"

Status is_convex(int *yes, const double eps, const int n, ...);
Status polynomial(double *out, const double x, const int deg, ...);
Status kaprekar_all(int *yes, const int base, const int n, ...);
Status geom_mean(double *out, const double eps, const int n, ...);
Status fast_pow(double *out, const double b, const int e);
Status dichotomy(double *root, double a, double b, const double eps, double (*f)(double));

#endif
