#include "series.h"
#include <math.h>
#include <stddef.h>

static int eps_ok(const double eps) { return eps > 0.0 && isfinite(eps); }

Status series_a(const double x, const double eps, double *out)
{
    double sum = 1, term = 1;
    int n;
    if (out == NULL || !eps_ok(eps) || !isfinite(x)) return ST_INVALID_INPUT;
    for (n = 1; n < 1000000; n++) {
        term *= x / n;
        if (fabs(term) < eps) { *out = sum; return ST_OK; }
        sum += term;
    }
    return ST_NO_CONVERGENCE;
}

Status series_b(const double x, const double eps, double *out)
{
    double sum = 1, term = 1, x2 = x * x;
    int n;
    if (out == NULL || !eps_ok(eps) || !isfinite(x)) return ST_INVALID_INPUT;
    for (n = 1; n < 1000000; n++) {
        term *= -x2 / ((2.0 * n - 1) * (2.0 * n));
        if (fabs(term) < eps) { *out = sum; return ST_OK; }
        sum += term;
    }
    return ST_NO_CONVERGENCE;
}

Status series_c(const double x, const double eps, double *out)
{
    double sum = 1, term = 1, x2;
    int n;
    if (out == NULL || !eps_ok(eps) || !isfinite(x) || fabs(x) >= 1) return ST_INVALID_INPUT;
    x2 = x * x;
    for (n = 1; n < 1000000; n++) {
        term *= 27.0 * n * n * n * x2 / ((3.0 * n - 2) * (3.0 * n - 1) * (3.0 * n));
        if (fabs(term) < eps) { *out = sum; return ST_OK; }
        sum += term;
    }
    return ST_NO_CONVERGENCE;
}

Status series_d(const double x, const double eps, double *out)
{
    double x2 = x * x, term = -x2 / 2, sum = 0;
    int n;
    if (out == NULL || !eps_ok(eps) || !isfinite(x) || fabs(x) > 1) return ST_INVALID_INPUT;
    for (n = 1; n < 1000000; n++) {
        sum += term;
        term *= -(2.0 * n + 1) * x2 / (2.0 * n + 2);
        if (fabs(term) < eps) { *out = sum; return ST_OK; }
    }
    return ST_NO_CONVERGENCE;
}
