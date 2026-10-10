#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "status.h"

Status e_series(const double eps, double *out);
Status e_limit(const double eps, double *out);
Status e_equation(const double eps, double *out);
Status pi_series(const double eps, double *out);
Status pi_limit(const double eps, double *out);
Status pi_equation(const double eps, double *out);
Status ln2_series(const double eps, double *out);
Status ln2_limit(const double eps, double *out);
Status ln2_equation(const double eps, double *out);
Status sqrt2_limit(const double eps, double *out);
Status sqrt2_product(const double eps, double *out);
Status sqrt2_equation(const double eps, double *out);
Status gamma_series(const double eps, double *out);
Status gamma_limit(const double eps, double *out);
Status gamma_equation(const double eps, double *out);

#endif
