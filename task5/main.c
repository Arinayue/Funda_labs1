#include "integration.h"
#include "parse.h"
#include "series.h"
#include <math.h>
#include <stdio.h>

static int eps_ok(const double eps) { return eps > 0.0 && isfinite(eps); }

static double fc_smooth(const double t)
{
    if (t <= 0.0) return 0.0;
    return -4.0 * t * log(t);
}

static void show(const char *name, const Status st, const double v)
{
    switch (st) {
    case ST_OK:
        printf("%s %.10f\n", name, v);
        break;
    case ST_INVALID_INPUT:
        printf("%s некорректные данные\n", name);
        break;
    case ST_OVERFLOW:
        printf("%s переполнение\n", name);
        break;
    case ST_NO_CONVERGENCE:
        printf("%s не сошёлся\n", name);
        break;
    case ST_DIV_ZERO:
        printf("%s деление на ноль\n", name);
        break;
    default:
        printf("%s ошибка\n", name);
        break;
    }
}

int main(int argc, char **argv)
{
    double eps, x, v;
    Status st;

    if (argc != 3) {
        printf("task05 <эпсилон> <x>\n");
        return 1;
    }

    st = parse_double(argv[1], &eps);
    if (st == ST_OK && !eps_ok(eps)) st = ST_INVALID_INPUT;
    if (st == ST_OK) st = parse_double(argv[2], &x);
    switch (st) {
    case ST_OK:
        break;
    case ST_OVERFLOW:
        printf("Число не влезло\n");
        return 2;
    default:
        printf("Некорректный ввод\n");
        return 1;
    }

    printf("ряды\n");
    st = series_a(x, eps, &v); show("  a:", st, v);
    st = series_b(x, eps, &v); show("  b:", st, v);
    st = series_c(x, eps, &v); show("  c:", st, v);
    st = series_d(x, eps, &v); show("  d:", st, v);
    printf("интегралы\n");
    st = integrate(fa, 0, 1, eps, &v); show("  a:", st, v);
    st = integrate(fb, 0, 1, eps, &v); show("  b:", st, v);
    st = integrate(fc_smooth, 0, 1, eps, &v); show("  c:", st, v);
    st = integrate(fd, 0, 1, eps, &v); show("  d:", st, v);
    return 0;
}