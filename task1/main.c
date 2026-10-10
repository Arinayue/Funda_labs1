#include "handlers.h"
#include "parsing.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    long x;
    char op;
    Status st = ST_OK;

    if(argc != 3 || parse_l(argv[1], &x) != ST_OK || parse_flag(argv[2], &op) != ST_OK) {
        printf("task01 <целое> <-h|-p|-s|-e|-a|-f>\n");
        return 1;
    }

    switch (op) {
    case 'h': {
        long buf[100];
        int n = 0, i;
        st = do_h(x, buf, &n);
        if(st == ST_OK) {
            if(n == 0) printf("Кратных нет\n");
            else for (i = 0; i < n; i++) printf("%ld%c", buf[i], i + 1 == n ? '\n' : ' ');
        }
        break;
    }
    case 'p': {
        int kind = 0;
        st = do_p(x, &kind);
        if(st == ST_OK)
            printf("%s\n", kind == 1 ? "Простое" : kind == 2 ? "Составное" : "Ни простое, ни составное");
        break;
    }
    case 's': {
        char text[20];
        int i;
        st = do_s(x, text);
        if(st == ST_OK)
            for(i = 0; text[i]; i++) printf("%c%s", text[i], text[i + 1] ? " " : "\n");
        break;
    }
    case 'e': {
        long long t[11][11];
        int b, e;
        st = do_e(x, t);
        if(st == ST_OK)
            for(b = 1; b <= 10; b++) {
                printf("%d:", b);
                for(e = 1; e <= x; e++) printf(" %llu", t[b][e]);
                printf("\n");
            }
        break;
    }
    case 'a': {
        long long sum;
        st = do_a(x, &sum);
        if(st == ST_OK) printf("%llu\n", sum);
        break;
    }
    case 'f': {
        long f;
        st = do_f(x, &f);
        if(st == ST_OK) printf("%lu\n", f);
        break;
    }
    default:
        printf("Неизвестный флаг\n");
        return 1;
    }

    if(st == ST_OVERFLOW) { 
        printf("Не влезает в диапазон\n"); 
        return 2; 
    }
    if(st != ST_OK) { 
        printf("Некорректное число для этого флага\n"); 
        return 1; 
    }
    return 0;
}
