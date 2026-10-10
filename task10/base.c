#include "base.h"
#include <limits.h>
#include <stdio.h>

static int dig(const char c)
{
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return -1;
}

unsigned long mag(const long v)
{
    return v < 0 ? (unsigned long)(-(v + 1)) + 1ul : (unsigned long)v;
}

Status from_base(const char *s, const int base, long *out)
{
    int neg = 0;
    unsigned long m = 0;
    if(s == NULL || out == NULL || *s == '\0' || base < 2 || base > 36) return ST_INVALID_INPUT;
    if(*s == '+' || *s == '-') { 
        neg = *s == '-'; 
        s++; 
        if(!*s) return ST_INVALID_INPUT; 
    }
    for(; *s; s++) {
        int d = dig(*s);
        if (d < 0 || d >= base) return ST_INVALID_INPUT;
        if (m > (ULONG_MAX - (unsigned)d) / (unsigned)base) return ST_OVERFLOW;
        m = m * (unsigned)base + (unsigned)d;
    }
    if(!neg) {
        if (m > (unsigned long)LONG_MAX) return ST_OVERFLOW;
        *out = (long)m;
    } else if(m == (unsigned long)LONG_MAX + 1ul) *out = LONG_MIN;
    else if(m > (unsigned long)LONG_MAX) return ST_OVERFLOW;
    else *out = -(long)m;
    return ST_OK;
}

Status add(const long a, const long b, long *out)
{
    if(out == NULL) return ST_INVALID_INPUT;
    if((b > 0 && a > LONG_MAX - b) || (b < 0 && a < LONG_MIN - b)) return ST_OVERFLOW;
    *out = a + b;
    return ST_OK;
}

static void print_base(const char *title, const long v, const int base)
{
    unsigned long m;
    char t[80];
    if(title == NULL || base < 2 || base > 36) return;
    m = mag(v);
    int n = 0;
    if(!m) t[n++] = '0';
    while(m) { t[n++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[m % (unsigned)base]; m /= (unsigned)base; }
    printf("%s_%d: %s", title, base, v < 0 ? "-" : "");
    while(n--) putchar(t[n]);
    putchar('\n');
}

void print_num(const char *title, const long v)
{
    int bases[] = {9, 18, 27, 36}, i;
    if(title == NULL) return;
    printf("%s: %ld\n", title, v);
    for(i = 0; i < 4; i++) print_base(title, v, bases[i]);
}
