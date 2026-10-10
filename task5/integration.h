#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "status.h"

Status integrate(double (*f)(double), const double a, const double b, const double eps, double *out);
double fa(const double x);
double fb(const double x);
double fc(const double x);
double fd(const double x);

#endif
