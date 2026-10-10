#include "functions.h"
#include <math.h>
#include <stddef.h>

#define MAX_ITER 2000000
#define MAX_GAMMA_M 3000

static int eps_ok(const double eps)
{
    return eps > 0.0 && eps < 1.0 && isfinite(eps);
}

Status e_series(const double eps, double *out)
{
    double sum = 1.0;
    double term = 1.0;
    int n;

    if(out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for(n = 1; n < 1000; n++) {
        term /= n;
        sum += term;
        if(term < eps) {
            *out = sum;
            return ST_OK;
        }
    }

    *out = sum;
    return ST_NO_CONVERGENCE;
}

Status e_limit(const double eps, double *out)
{
    double n;

    if(out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    n = 2.0 / eps;
    if(n > 1e7) n = 1e7;
    if(n < 1.0) n = 1.0;

    *out = pow(1.0 + 1.0 / n, n);
    return 2.0 / n <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status e_equation(const double eps, double *out)
{
    double a = 2.0;
    double b = 3.0;
    int i;

    if(out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for(i = 0; i < 200 && b - a > eps; i++) {
        double mid = (a + b) / 2.0;
        if(log(mid) < 1.0) a = mid;
        else b = mid;
    }

    *out = (a + b) / 2.0;
    return b - a <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status pi_series(const double eps, double *out)
{
    double sum = 0.0;
    double sign = 1.0;
    long n;

    if(out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for(n = 0; n < MAX_ITER; n++) {
        double term = sign / (2.0 * n + 1.0);
        double next_term = 1.0 / (2.0 * n + 3.0);

        sum += term;
        if(4.0 * next_term <= eps) {
            *out = 4.0 * sum;
            return ST_OK;
        }
        sign = -sign;
    }

    *out = 4.0 * sum;
    return ST_NO_CONVERGENCE;
}

Status pi_limit(const double eps, double *out)
{
    double approximation = 4.0;
    double requested;
    long need;
    long n;

    if(out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    requested = 8.0 / eps;
    if(requested > MAX_ITER) need = MAX_ITER;
    else {
        need = (long)ceil(requested);
        if (need < 1) need = 1;
    }

    for (n = 1; n <= need; n++) {
        double numerator = 4.0 * n * (n + 1.0);
        double denominator = (2.0 * n + 1.0) * (2.0 * n + 1.0);
        approximation *= numerator / denominator;
    }

    *out = approximation;
    return 8.0 / need <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status pi_equation(const double eps, double *out)
{
    double a = 2.0;
    double b = 4.0;
    int i;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (i = 0; i < 200 && b - a > eps; i++) {
        double mid = (a + b) / 2.0;
        if (sin(mid) > 0.0) a = mid;
        else b = mid;
    }

    *out = (a + b) / 2.0;
    return b - a <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status ln2_series(const double eps, double *out)
{
    double sum = 0.0;
    double sign = 1.0;
    long n;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (n = 1; n <= MAX_ITER; n++) {
        double term = sign / n;
        double next_term = 1.0 / (n + 1.0);

        sum += term;
        if (next_term <= eps) {
            *out = sum;
            return ST_OK;
        }
        sign = -sign;
    }

    *out = sum;
    return ST_NO_CONVERGENCE;
}

Status ln2_limit(const double eps, double *out)
{
    double n;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    n = 1.0 / eps;
    if (n > 1e7) n = 1e7;
    if (n < 1.0) n = 1.0;

    *out = n * (pow(2.0, 1.0 / n) - 1.0);
    return 1.0 / n <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status ln2_equation(const double eps, double *out)
{
    double a = 0.0;
    double b = 1.0;
    int i;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (i = 0; i < 200 && b - a > eps; i++) {
        double mid = (a + b) / 2.0;
        if (exp(mid) < 2.0) a = mid;
        else b = mid;
    }

    *out = (a + b) / 2.0;
    return b - a <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status sqrt2_limit(const double eps, double *out)
{
    double x = -0.5;
    int i;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (i = 0; i < 100; i++) {
        x = x - x * x / 2.0 + 1.0;
        if (fabs(x * x - 2.0) < eps) {
            *out = x;
            return ST_OK;
        }
    }

    *out = x;
    return ST_NO_CONVERGENCE;
}

Status sqrt2_product(const double eps, double *out)
{
    double product = 1.0;
    double power = 0.25;
    int i;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (i = 0; i < 40; i++) {
        double term = pow(2.0, power);
        product *= term;
        power /= 2.0;

        if (fabs(product * (term - 1.0)) <= eps) {
            *out = product;
            return ST_OK;
        }
    }

    *out = product;
    return ST_NO_CONVERGENCE;
}

Status sqrt2_equation(const double eps, double *out)
{
    double a = 1.0;
    double b = 2.0;
    int i;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (i = 0; i < 200 && b - a > eps; i++) {
        double mid = (a + b) / 2.0;
        if (mid * mid < 2.0) a = mid;
        else b = mid;
    }

    *out = (a + b) / 2.0;
    return b - a <= eps ? ST_OK : ST_NO_CONVERGENCE;
}

Status gamma_series(const double eps, double *out)
{
    double sum = 0.0;
    double pi = acos(-1.0);
    int max_m;
    int m;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    if (eps <= 4.0 / (MAX_GAMMA_M * (double)MAX_GAMMA_M)) {
        max_m = MAX_GAMMA_M;
    } else {
        max_m = (int)ceil(sqrt(4.0 / eps));
        if (max_m < 10) max_m = 10;
    }

    for (m = 1; m <= max_m; m++) {
        int lo = m * m < 2 ? 2 : m * m;
        int hi = (m + 1) * (m + 1) - 1;
        int k;
        double inverse_square = 1.0 / (m * (double)m);

        for (k = lo; k <= hi; k++)
            sum += inverse_square - 1.0 / k;
    }

    sum += 2.0 / (max_m + 1.0);
    *out = -(pi * pi) / 6.0 + sum;

    if (max_m < MAX_GAMMA_M ||
        1.0 / (max_m * (double)max_m) <= eps)
        return ST_OK;
    return ST_NO_CONVERGENCE;
}

Status gamma_limit(const double eps, double *out)
{
    double harmonic = 0.0;
    long n;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (n = 1; n <= MAX_ITER; n++) {
        double approximation;

        harmonic += 1.0 / n;
        approximation = harmonic - log((double)n);
        *out = approximation;

        if (1.0 / (2.0 * n) <= eps / 2.0)
            return ST_OK;
    }

    return ST_NO_CONVERGENCE;
}

static int is_prime(const int n)
{
    int i;

    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;

    for (i = 3; i * i <= n; i += 2)
        if (n % i == 0) return 0;

    return 1;
}

Status gamma_equation(const double eps, double *out)
{
    double product = 1.0;
    double previous = 0.0;
    double current = 0.0;
    int t;

    if (out == NULL || !eps_ok(eps)) return ST_INVALID_INPUT;

    for (t = 2; t <= 100000; t++) {
        if (is_prime(t)) product *= (t - 1.0) / t;

        previous = current;
        current = -log(log((double)t) * product);
    }

    *out = current;
    if (fabs(current - previous) < eps)
        return ST_OK;
    return ST_NO_CONVERGENCE;
}
