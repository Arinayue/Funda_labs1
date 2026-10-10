#include "func.h"
#include <math.h>
#include <stdio.h>

static double eq2(const double x) { return x * x - 2; }
static double eq3(const double x) { return x * x * x - x - 2; }
static double eqsin(const double x) { return sin(x); }

int main(void)
{
    int yes = 0, mark[6], i;
    double v = 0, root = 0;
    const char *ks[] = {"1", "45", "10", "297", "99", "0"};
    Status st;

    printf("многоугольник\n");
    st = is_convex(&yes, 1e-9, 4, 0.0, 0.0, 2.0, 0.0, 2.0, 2.0, 0.0, 2.0);
    printf("  квадрат: %s\n", st == ST_OK && yes ? "выпуклый" : "не выпуклый");
    st = is_convex(&yes, 1e-9, 3, 0.0, 0.0, 4.0, 0.0, 1.0, 3.0);
    printf("  треугольник: %s\n", st == ST_OK && yes ? "выпуклый" : "не выпуклый");
    st = is_convex(&yes, 1e-9, 4, 0.0, 0.0, 3.0, 0.0, 1.0, 1.0, 3.0, 2.0);
    printf("  вдавленный: %s\n", st == ST_OK && yes ? "выпуклый" : "не выпуклый");

    printf("многочлен\n");
    st = polynomial(&v, 3, 2, 2.0, -4.0, 2.0);
    if (st == ST_OK) printf("  2x^2-4x+2 в 3 = %.4f\n", v);
    st = polynomial(&v, 2, 3, 1.0, 0.0, 0.0, 1.0);
    if (st == ST_OK) printf("  x^3+1 в 2 = %.4f\n", v);

    printf("капрекар, основание 10\n");
    st = kaprekar_all(mark, 10, 6, ks[0], ks[1], ks[2], ks[3], ks[4], ks[5]);
    if (st == ST_OK)
        for (i = 0; i < 6; i++) printf("  %s: %s\n", ks[i], mark[i] ? "да" : "нет");

    printf("среднее геометрическое\n");
    st = geom_mean(&v, 1e-12, 2, 2.0, 8.0);
    if (st == ST_OK) printf("  2 и 8 -> %.4f\n", v);
    st = geom_mean(&v, 1e-12, 3, 1.0, 2.0, 4.0);
    if (st == ST_OK) printf("  1, 2, 4 -> %.4f\n", v);
    st = geom_mean(&v, 1e-12, 2, -2.0, 8.0);
    if (st != ST_OK) printf("  отрицательное не берём\n");

    printf("степень\n");
    st = fast_pow(&v, 2, 10); if (st == ST_OK) printf("  2^10 = %.0f\n", v);
    st = fast_pow(&v, -2, 5); if (st == ST_OK) printf("  (-2)^5 = %.0f\n", v);
    st = fast_pow(&v, 5, 0); if (st == ST_OK) printf("  5^0 = %.0f\n", v);
    st = fast_pow(&v, 3, -2); if (st == ST_OK) printf("  3^(-2) = %.6f\n", v);
    st = fast_pow(&v, 0, -1); if (st == ST_DIV_ZERO) printf("  0^(-1): деление на ноль\n");

    printf("дихотомия\n");
    st = dichotomy(&root, 1, 2, 1e-10, eq2);
    if (st == ST_OK) printf("  x^2=2 -> %.8f\n", root);
    st = dichotomy(&root, 1, 2, 1e-9, eq3);
    if (st == ST_OK) printf("  x^3-x-2=0 -> %.8f\n", root);
    st = dichotomy(&root, 3, 4, 1e-10, eqsin);
    if (st == ST_OK) printf("  sin(x)=0 на [3,4] -> %.8f\n", root);
    st = dichotomy(&root, 0, 1, 1e-6, eq2);
    if (st == ST_NO_SOLUTION) printf("  x^2=2 на [0,1]: нет корня\n");
    return 0;
}
