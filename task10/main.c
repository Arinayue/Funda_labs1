#include "base.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>

static int read_line(char *buf, const int cap)
{
    int i = 0, c;
    if(buf == NULL || cap <= 1) return 0;
    c = getchar();
    if(c == EOF) return 0;
    while(c != EOF && c != '\n' && i < cap - 1) {
        buf[i++] = (char)c;
        c = getchar();
    }
    buf[i] = 0;
    return 1;
}

int main(void)
{
    char line[512];
    long base = 0, sum = 0, mx = 0, v;
    int cnt = 0, have = 0;

    printf("Основание 2..36: ");
    if(!read_line(line, (int)sizeof line)) return 1;
    line[strcspn(line, "\r\n")] = 0;
    if(parse_l(line, &base) || base < 2 || base > 36) {
        printf("Основание от 2 до 36\n");
        return 1;
    }
    printf("Числа, в конце Stop:\n");
    while(read_line(line, (int)sizeof line)) {
        Status st;
        line[strcspn(line, "\r\n")] = 0;
        if(!line[0]) continue;
        if(strcmp(line, "Stop") == 0) break;
        st = from_base(line, (int)base, &v);
        if(st == ST_OVERFLOW) { 
            printf("Не влезло: %s\n", line); 
            continue; 
        }
        if(st != ST_OK) { 
            printf("Не число: %s\n", line); 
            continue; 
        }
        if(add(sum, v, &sum) != ST_OK) { 
            printf("Сумма переполнилась\n"); 
            return 2; 
        }
        if(!have || mag(v) > mag(mx)) { 
            mx = v; 
            have = 1; 
        }
        cnt++;
    }
    if(!cnt) { 
        printf("Чисел не введено\n"); 
        return 0; 
    }
    print_num("Максимум", mx);
    print_num("Сумма", sum);
    return 0;
}
