#include "func.h"
#include "parse.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    char op;
    Status st = ST_OK;

    if (argc < 2 || parse_flag(argv[1], &op) != ST_OK) {
        printf("task03 -q <eps> a b c | -m n1 n2 | -t <eps> a b c\n");
        return 1;
    }

    switch (op) {
    case 'q': {
        double e, a, b, c;
        Solution s[6];
        int n = 0, i;
        if (argc != 6 || parse_double(argv[2], &e) || parse_double(argv[3], &a) || parse_double(argv[4], &b) || parse_double(argv[5], &c)) {
            printf("Для -q: эпсилон и 3 коэффициента\n");
            return 1;
        }
        st = solve_perms(e, a, b, c, s, &n);
        if (st == ST_OK) {
            for (i = 0; i < n; i++) {
                printf("уравнение: %.6g x^2 + %.6g x + %.6g = 0\n", s[i].a, s[i].b, s[i].c);
                switch (s[i].count) {
                case -1:
                    printf("  бесконечно много корней\n");
                    break;
                case 0:
                    printf("  корней нет\n");
                    break;
                case 1:
                    printf("  x = %.6g\n", s[i].x1);
                    break;
                default:
                    printf("  x1 = %.6g, x2 = %.6g\n", s[i].x1, s[i].x2);
                    break;
                }
            }
        }
        break;
    }
    case 'm': {
        long a, b;
        int yes = 0;
        if (argc != 4 || parse_l(argv[2], &a) || parse_l(argv[3], &b)) {
            printf("Для -m: два целых\n");
            return 1;
        }
        st = multiple(a, b, &yes);
        if (st == ST_OK)
            printf("%ld %sкратно %ld\n", a, yes ? "" : "не ", b);
        break;
    }
    case 't': {
        double e, a, b, c;
        int yes = 0;
        if (argc != 6 || parse_double(argv[2], &e) || parse_double(argv[3], &a) || parse_double(argv[4], &b) || parse_double(argv[5], &c)) {
            printf("Для -t: эпсилон и 3 стороны\n");
            return 1;
        }
        st = right_triangle(e, a, b, c, &yes);
        if (st == ST_OK)
            printf("%s\n", yes ? "прямоугольный" : "не прямоугольный");
        break;
    }
    default:
        printf("Флаг: q, m или t\n");
        return 1;
    }

    if (st == ST_OVERFLOW) {
        printf("Не влезает в диапазон\n");
        return 2;
    }
    if (st != ST_OK) {
        printf("Некорректные данные\n");
        return 1;
    }
    return 0;
}
