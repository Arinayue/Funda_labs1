#include "integration.h"
#include <math.h>
#include <stddef.h>

static int eps_ok(const double eps) { return eps > 0.0 && isfinite(eps); }

double fa(const double x) { return fabs(x) < 1e-15 ? 1 : log(1 + x) / x; }
double fb(const double x) { return exp(-x * x / 2); }
double fc(const double x) { return log(1 / (1 - x)); }
double fd(const double x) { return fabs(x) < 1e-15 ? 1 : pow(x, x); }

Status integrate(double (*f)(double), const double a, const double b, const double eps, double *out)
{
    int n = 2, step;
    double prev = 0, cur = 0;
    if(f == NULL || out == NULL || !eps_ok(eps) || !isfinite(a) || !isfinite(b) || a >= b) return ST_INVALID_INPUT;
    for(step = 0; step < 22; step++) {
        double h = (b - a) / n, fa = f(a), fb = f(b), s;
        int i;
        if(!isfinite(fa) || !isfinite(fb)) return ST_INVALID_INPUT;
        s = fa + fb;
        for (i = 1; i < n; i++) {
            double fx = f(a + i * h);
            if(!isfinite(fx)) return ST_INVALID_INPUT;
            s += (i % 2 ? 4 : 2) * fx;
        }
        cur = s * h / 3;
        if (step > 0 && fabs(cur - prev) < eps) { *out = cur; return ST_OK; }
        prev = cur;
        n *= 2;
    }
    *out = cur;
    return ST_NO_CONVERGENCE;
}
