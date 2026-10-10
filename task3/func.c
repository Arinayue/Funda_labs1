#include "func.h"
#include <math.h>
#include <stddef.h>

static int eps_ok(const double eps) { return eps > 0.0 && isfinite(eps); }
static int nearly_equal(const double a, const double b, const double eps) { return fabs(a - b) <= eps; }

static Status solve_one(const double eps, const double a, const double b, const double c, Solution *s)
{
    double d;
    if(s == NULL) return ST_INVALID_INPUT;
    s->a = a; s->b = b; s->c = c; s->x1 = s->x2 = 0;
    if (nearly_equal(a, 0, eps)) {
        if (nearly_equal(b, 0, eps)) s->count = nearly_equal(c, 0, eps) ? -1 : 0;
        else { s->x1 = -c / b; s->count = 1; }
        return ST_OK;
    }
    d = b * b - 4 * a * c;
    if (d < -eps) s->count = 0;
    else if (fabs(d) <= eps) { 
        s->x1 = -b / (2 * a); 
        s->count = 1; 
    }
    else {
        double sq = sqrt(d);
        s->x1 = (-b - sq) / (2 * a);
        s->x2 = (-b + sq) / (2 * a);
        s->count = 2;
    }
    return ST_OK;
}

Status solve_perms(const double eps, const double a, const double b, const double c, Solution *out, int *n)
{
    double p[6][3] = {{a, b, c}, {a, c, b}, {b, a, c}, {b, c, a}, {c, a, b}, {c, b, a}};
    int i, cnt = 0;
    if (out == NULL || n == NULL || !eps_ok(eps) || eps > 1.0) return ST_INVALID_INPUT;
    for (i = 0; i < 6; i++) {
        int j, uniq = 1;
        for (j = 0; j < i; j++)
            if (nearly_equal(p[i][0], p[j][0], eps) && nearly_equal(p[i][1], p[j][1], eps) && nearly_equal(p[i][2], p[j][2], eps))
                uniq = 0;
        if (!uniq) continue;
        solve_one(eps, p[i][0], p[i][1], p[i][2], &out[cnt]);
        cnt++;
    }
    *n = cnt;
    return ST_OK;
}

Status multiple(const long a, const long b, int *yes)
{
    if (yes == NULL || a == 0 || b == 0) return ST_INVALID_INPUT;
    if (b == 1 || b == -1) { 
        *yes = 1; 
        return ST_OK; 
    }
    *yes = a % b == 0;
    return ST_OK;
}

Status right_triangle(const double eps, const double a, const double b, const double c, int *yes)
{
    if (yes == NULL || !eps_ok(eps) || eps > 1.0 || a <= eps || b <= eps || c <= eps) return ST_INVALID_INPUT;
    *yes = nearly_equal(a * a + b * b, c * c, eps)
        || nearly_equal(a * a + c * c, b * b, eps)
        || nearly_equal(b * b + c * c, a * a, eps);
    return ST_OK;
}
