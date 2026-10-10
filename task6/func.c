#include "func.h"
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

static int eps_ok(const double eps) { return eps > 0.0 && isfinite(eps); }

Status is_convex(int *yes, const double eps, const int n, ...)
{
    double *x, *y, turn = 0;
    va_list ap;
    int i, sign = 0, turned = 0;
    if(!yes || n < 3 || !eps_ok(eps)) return ST_INVALID_INPUT;
    x = malloc((size_t)n * sizeof(double));
    y = malloc((size_t)n * sizeof(double));
    if(!x || !y) { 
        free(x); 
        free(y); 
        return ST_NO_MEMORY; 
    }
    va_start(ap, n);
    for(i = 0; i < n; i++) { 
        x[i] = va_arg(ap, double); 
        y[i] = va_arg(ap, double);
        if(!isfinite(x[i]) || !isfinite(y[i])) {
            va_end(ap);
            free(x);
            free(y);
            return ST_INVALID_INPUT;
        }
    }
    va_end(ap);
    *yes = 1;
    for(i = 0; i < n; i++) {
        int j = (i + 1) % n, k = (i + 2) % n;
        double dx = x[j] - x[i], dy = y[j] - y[i];
        double ex = x[k] - x[j], ey = y[k] - y[j];
        double z = dx * ey - dy * ex;
        if(hypot(dx, dy) <= eps || hypot(ex, ey) <= eps) { 
            free(x); 
            free(y); 
            return ST_INVALID_INPUT; 
        }
        if(fabs(z) <= eps) continue;
        if(!turned) {
            sign = z > 0 ? 1 : -1; 
            turned = 1; 
        }
        else if((z > 0) != (sign > 0)) { 
            *yes = 0; 
            break; 
        }
        turn += atan2(z, dx * ex + dy * ey);
    }
    if(!turned || fabs(fabs(turn) - 2.0 * acos(-1.0)) > 1e-6) *yes = 0;
    free(x); 
    free(y);
    return ST_OK;
}

Status polynomial(double *out, const double x, const int deg, ...)
{
    va_list ap;
    double v;
    int i;
    if(!out || deg < 0 || !isfinite(x)) return ST_INVALID_INPUT;
    va_start(ap, deg);
    v = va_arg(ap, double);
    if(!isfinite(v)) { 
        va_end(ap); 
        return ST_INVALID_INPUT; 
    }
    for(i = 0; i < deg; i++) {
        double c = va_arg(ap, double);
        if(!isfinite(c)) { 
            va_end(ap); 
            return ST_INVALID_INPUT; 
        }
        v = v * x + c;
    }
    va_end(ap);
    if(!isfinite(v)) return ST_OVERFLOW;
    *out = v;
    return ST_OK;
}

static int dig(const char c)
{
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if(c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

static Status kaprekar_one(const char *s, const int base, int *yes)
{
    unsigned long v = 0, sq, place = 1;
    const char *p;
    if(!yes || !s || !*s || base < 2 || base > 36) return ST_INVALID_INPUT;
    for(p = s; *p; p++) {
        int d = dig(*p);
        if(d < 0 || d >= base) return ST_INVALID_INPUT;
        if(v > (ULONG_MAX - (unsigned)d) / (unsigned)base) return ST_OVERFLOW;
        v = v * (unsigned)base + (unsigned)d;
    }
    if(v && v > ULONG_MAX / v) return ST_OVERFLOW;
    sq = v * v;
    *yes = 0;
    if(v == 0) { 
        *yes = 1; 
        return ST_OK; 
    }
    place = (unsigned)base;
    for(;;) {
        unsigned long right = sq % place, left = sq / place;
        if(right && left <= ULONG_MAX - right && left + right == v) { 
            *yes = 1; 
            break; 
        }
        if(place > sq || place > ULONG_MAX / (unsigned)base) break;
        place *= (unsigned)base;
    }
    return ST_OK;
}

Status kaprekar_all(int *yes, const int base, const int n, ...)
{
    va_list ap;
    int i;
    if(!yes || n <= 0 || base < 2 || base > 36) return ST_INVALID_INPUT;
    va_start(ap, n);
    for(i = 0; i < n; i++) {
        Status st = kaprekar_one(va_arg(ap, const char *), base, &yes[i]);
        if(st != ST_OK) { 
            va_end(ap); 
            return st; 
        }
    }
    va_end(ap);
    return ST_OK;
}

Status geom_mean(double *out, const double eps, const int n, ...)
{
    va_list ap;
    double s = 0;
    int i;
    if(!out || n <= 0 || !eps_ok(eps)) return ST_INVALID_INPUT;
    va_start(ap, n);
    for(i = 0; i < n; i++) {
        double v = va_arg(ap, double);
        if(!isfinite(v) || !(v > eps)) { 
            va_end(ap); 
            return ST_INVALID_INPUT; 
        }
        s += log(v);
    }
    va_end(ap);
    *out = exp(s / n);
    if(!isfinite(*out)) return ST_OVERFLOW;
    return ST_OK;
}

static Status pow_pos(const double b, const unsigned e, double *out)
{
    double h;
    Status st;
    if(e == 0) { 
        *out = 1; 
        return ST_OK; 
    }
    if(e % 2 == 0) {
        st = pow_pos(b, e / 2, &h);
        if(st != ST_OK) return st;
        *out = h * h;
    } else {
        st = pow_pos(b, e - 1, &h);
        if(st != ST_OK) return st;
        *out = b * h;
    }
    return isfinite(*out) ? ST_OK : ST_OVERFLOW;
}

Status fast_pow(double *out, const double b, const int e)
{
    unsigned u;
    double m;
    Status st;
    int neg = e < 0;
    if(!out || !isfinite(b)) return ST_INVALID_INPUT;
    if(neg && b == 0) return ST_DIV_ZERO;
    u = e == INT_MIN ? 2147483648u : (unsigned)(e < 0 ? -e : e);
    st = pow_pos(b, u, &m);
    if(st != ST_OK) return st;
    if(neg) {
        if(m == 0) return ST_DIV_ZERO;
        m = 1 / m;
    }
    if(!isfinite(m)) return ST_OVERFLOW;
    *out = m;
    return ST_OK;
}

Status dichotomy(double *root, double a, double b, const double eps, double (*f)(double))
{
    double fa, fb;
    int i;
    if(!root || !f || !eps_ok(eps) || !isfinite(a) || !isfinite(b)) return ST_INVALID_INPUT;
    if(a > b) { 
        double t = a; 
        a = b; 
        b = t; 
    }
    fa = f(a); fb = f(b);
    if(!isfinite(fa) || !isfinite(fb)) return ST_INVALID_INPUT;
    if(fabs(fa) <= eps) { 
        *root = a; 
        return ST_OK; 
    }
    if(fabs(fb) <= eps) { 
        *root = b; 
        return ST_OK; 
    }
    if(fa * fb > 0) return ST_NO_SOLUTION;
    for(i = 0; i < 200 && b - a > eps; i++) {
        double m = (a + b) / 2, fm = f(m);
        if(!isfinite(fm)) return ST_INVALID_INPUT;
        if(fabs(fm) <= eps) { 
            *root = m; 
            return ST_OK; 
        }
        if(fa * fm < 0) { 
            b = m; 
            fb = fm; 
        }
        else { 
            a = m; 
            fa = fm; }

    }
    *root = (a + b) / 2;
    return ST_OK;
}
