#include "functions.h"
#include "parse.h"
#include <math.h>
#include <stdio.h>
// #include <windows.h> Для русского языка

static int eps_ok(const double eps) { return eps > 0.0 && eps < 1.0 && isfinite(eps); }

int main(int argc, char **argv)
{
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    double eps, v;
    int i;
    struct { const char *name; Status (*fn)(double, double *); } row[] = {
        {"e ряд", e_series}, {"e предел", e_limit}, {"e уравнение", e_equation},
        {"pi ряд", pi_series}, {"pi предел", pi_limit}, {"pi уравнение", pi_equation},
        {"ln2 ряд", ln2_series}, {"ln2 предел", ln2_limit}, {"ln2 уравнение", ln2_equation},
        {"sqrt2 предел", sqrt2_limit}, {"sqrt2 произведение", sqrt2_product}, {"sqrt2 уравнение", sqrt2_equation},
        {"gamma ряд", gamma_series}, {"gamma предел", gamma_limit}, {"gamma уравнение", gamma_equation}
    };

    if(argc != 2 || parse_double(argv[1], &eps) != ST_OK || !eps_ok(eps)) {
        printf("task02 <эпсилон>\n");
        return 1;
    }
    for (i = 0; i < 15; i++) {
        Status st = row[i].fn(eps, &v);
        if(st == ST_OK) printf("%-22s %.10f\n", row[i].name, v);
        else if(st == ST_NO_CONVERGENCE) printf("%-22s %.10f  (не сошлось)\n", row[i].name, v);
        else { 
            printf("%s: ошибка\n", row[i].name); 
            return 1; 
        }
    }
    return 0;
}
