#include "convert.h"
#include <stdlib.h>
#include <string.h>

static int dig(const char c)
{
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if(c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

static Status mul_add(char **d, size_t *len, size_t *cap, const int base, const int digit)
{
    size_t i;
    int carry = digit;
    if(d == NULL || *d == NULL || len == NULL || cap == NULL || base < 2) return ST_INVALID_INPUT;
    for(i = 0; i < *len; i++) {
        int cur = ((*d)[i] - '0') * base + carry;
        (*d)[i] = (char)('0' + cur % 10);
        carry = cur / 10;
    }
    while(carry) {
        if(*len + 1 >= *cap) {
            char *g = realloc(*d, *cap * 2);
            if(!g) return ST_NO_MEMORY;
            *cap *= 2;
            *d = g;
        }
        (*d)[(*len)++] = (char)('0' + carry % 10);
        carry /= 10;
    }
    return ST_OK;
}

Status describe(const char *tok, char **clean, int *base, char **decimal)
{
    int maxd = 0, i;
    size_t start = 0, n, len = 1, cap = 8;
    char *norm, *dec;
    Status st;

    if(tok == NULL || clean == NULL || base == NULL || decimal == NULL || tok[0] == '\0')
        return ST_INVALID_INPUT;

    for(i = 0; tok[i]; i++) {
        int d = dig(tok[i]);
        if(d < 0) return ST_INVALID_INPUT;
        if(d > maxd) maxd = d;
    }
    while(tok[start] == '0' && tok[start + 1]) start++;
    n = strlen(tok + start);
    norm = malloc(n + 1);
    dec = malloc(cap);
    if(!norm || !dec) { 
        free(norm); 
        free(dec); 
        return ST_NO_MEMORY; 
    }
    for(i = 0; i < (int)n; i++) {
        char c = tok[start + i];
        norm[i] = (c >= 'a' && c <= 'z') ? (char)(c - 'a' + 'A') : c;
    }
    norm[n] = 0;
    *base = maxd + 1;
    if (*base < 2) *base = 2;
    dec[0] = '0';
    for(i = 0; norm[i]; i++) {
        st = mul_add(&dec, &len, &cap, *base, dig(norm[i]));
        if(st != ST_OK) { 
            free(norm); 
            free(dec); return st; 
        }
    }
    while(len > 1 && dec[len - 1] == '0') len--;
    {
        char *text = malloc(len + 1);
        size_t k;
        if(!text) { 
            free(norm); 
            free(dec); 
            return ST_NO_MEMORY; 
        }
        for(k = 0; k < len; k++) text[k] = dec[len - 1 - k];
        text[len] = 0;
        free(dec);
        *clean = norm;
        *decimal = text;
    }
    return ST_OK;
}
