#include "handlers.h"
#include <limits.h>
#include <stddef.h>

Status do_h(const long x, long *buf, int *n)
{
    long step, i;
    if(buf == NULL || n == NULL) return ST_INVALID_INPUT;
    *n = 0;
    if(x == 0) return ST_INVALID_INPUT;
    if(x == LONG_MIN) return ST_OK;
    step = x < 0 ? -x : x;
    if(step > 100) return ST_OK;
    for(i = step; i <= 100; i += step) buf[(*n)++] = i;
    return ST_OK;
}

Status do_p(const long x, int *kind)
{
    long d;
    if(kind == NULL) return ST_INVALID_INPUT;
    if(x <= 1) { 
        *kind = 0; 
        return ST_OK; 
    }
    if(x % 2 == 0) { 
        *kind = x == 2 ? 1 : 2;
         return ST_OK; 
        }
    for (d = 3; d <= x / d; d += 2)
        if (x % d == 0) { 
            *kind = 2; 
            return ST_OK; 
        }
    *kind = 1;
    return ST_OK;
}

Status do_s(const long x, char *out)
{
    unsigned long v;
    char tmp[20];
    int n = 0, i = 0;
    const char *hex = "0123456789ABCDEF";
    if(out == NULL) return ST_INVALID_INPUT;
    if(x == LONG_MIN) v = (unsigned long)LONG_MAX + 1ul;
    else v = (unsigned long)(x < 0 ? -x : x);
    if(v == 0) tmp[n++] = '0';
    while(v > 0) { 
        tmp[n++] = hex[v % 16]; 
        v /= 16; 
    }
    if(x < 0) out[i++] = '-';
    while(n > 0) out[i++] = tmp[--n];
    out[i] = '\0';
    return ST_OK;
}

Status do_e(const long x, long long t[11][11])
{
    int b, e;
    if(t == NULL || x < 1 || x > 10) return ST_INVALID_INPUT;
    for(b = 1; b <= 10; b++) {
        long long p = 1;
        for (e = 1; e <= x; e++) t[b][e] = (p *= b);
    }
    return ST_OK;
}

Status do_a(const long n, long long *out)
{
    if(out == NULL || n < 1) return ST_INVALID_INPUT;
    *out = (long long)n * ((long long)n + 1) / 2;
    return ST_OK;
}

Status do_f(const long n, long *out)
{
    long i;
    long r = 1;
    if(out == NULL || n < 0) return ST_INVALID_INPUT;
    if(n > 12) return ST_OVERFLOW;
    for(i = 2; i <= n; i++) r *= i;
    *out = r;
    return ST_OK;
}
