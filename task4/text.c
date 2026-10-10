#include "text.h"
#include <stdio.h>
#include <stdlib.h>

static int latin(const int c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
static int digit(const int c) { return c >= '0' && c <= '9'; }

static Status add_byte(char **buf, size_t *n, size_t *cap, const char c)
{
    if(*n + 1 >= *cap) {
        char *g = realloc(*buf, *cap ? *cap * 2 : 64);
        if (!g) return ST_NO_MEMORY;
        *cap = *cap ? *cap * 2 : 64;
        *buf = g;
    }
    (*buf)[(*n)++] = c;
    return ST_OK;
}

Status transform(const char *in, const size_t n, const char op, char **out)
{
    char *buf = NULL;
    size_t len = 0, cap = 0, i;
    int cnt = 0, open = 0;
    Status st;

    if(out == NULL || (n > 0 && in == NULL)) return ST_INVALID_INPUT;
    if(op != 'd' && op != 'i' && op != 's' && op != 'a') return ST_INVALID_INPUT;

    for(i = 0; i < n; i++) {
        unsigned char c = (unsigned char)in[i];
        if(op == 'd') {
            if(!digit(c) && (st = add_byte(&buf, &len, &cap, (char)c)) != ST_OK) { 
                free(buf); 
                return st; 
            }
            continue;
        }
        if(op == 'a') {
            if(digit(c)) {
                if((st = add_byte(&buf, &len, &cap, (char)c)) != ST_OK) { 
                    free(buf); 
                    return st; 
                }
            } else {
                char hex[8];
                int k, m = sprintf(hex, "%X", c);
                for (k = 0; k < m; k++)
                    if ((st = add_byte(&buf, &len, &cap, hex[k])) != ST_OK) { 
                        free(buf); 
                        return st; 
                    }
            }
            continue;
        }
        if(c == '\r') {
            if (i + 1 < n && in[i + 1] == '\n') i++;
            c = '\n';
        }
        if(c == '\n') {
            char num[32];
            int k, m = sprintf(num, "%d\n", cnt);
            for (k = 0; k < m; k++)
                if ((st = add_byte(&buf, &len, &cap, num[k])) != ST_OK) { 
                    free(buf); 
                    return st; 
                }
            cnt = 0; open = 0; continue;
        }
        open = 1;
        if(op == 'i') cnt += latin(c);
        else if(!latin(c) && !digit(c) && c != ' ') cnt++;
    }
    if((op == 'i' || op == 's') && open) {
        char num[32];
        int k, m = sprintf(num, "%d\n", cnt);
        for(k = 0; k < m; k++)
            if((st = add_byte(&buf, &len, &cap, num[k])) != ST_OK) { 
                free(buf); 
                return st;
            }
    }
    if((st = add_byte(&buf, &len, &cap, '\0')) != ST_OK) { 
        free(buf);
        return st;
    }
    *out = buf;
    return ST_OK;
}
