#ifndef SERIES_H
#define SERIES_H

#include "status.h"

Status series_a(const double x, const double eps, double *out);
Status series_b(const double x, const double eps, double *out);
Status series_c(const double x, const double eps, double *out);
Status series_d(const double x, const double eps, double *out);

#endif
