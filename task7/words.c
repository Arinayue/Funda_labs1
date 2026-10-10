#include "words.h"
#include <stdlib.h>
#include <string.h>

static void lower_latin(char *s)
{
    for (; *s; s++) if (*s >= 'A' && *s <= 'Z') *s += (char)('a' - 'A');
}

static Status codes_of(const char *word, const int base, char **out)
{
    const unsigned char *p;
    size_t n = 0, cap = 32;
    char *b;
    if(word == NULL || out == NULL || base < 2 || base > 16) return ST_INVALID_INPUT;
    b = malloc(cap);
    if(!b) return ST_NO_MEMORY;
    for(p = (const unsigned char *)word; *p; p++) {
        char tmp[8];
        int k = 0, v = *p;
        if(!v) tmp[k++] = '0';
        while(v) { tmp[k++] = "0123456789ABCDEF"[v % base]; v /= base; }
        while(k--) {
            if (n + 1 >= cap) {
                char *g = realloc(b, cap *= 2);
                if (!g) { 
                    free(b); 
                    return ST_NO_MEMORY; 
                }
                b = g;
            }
            b[n++] = tmp[k];
        }
    }
    b[n] = 0;
    *out = b;
    return ST_OK;
}

Status change_word(const char *word, const int index, char **out)
{
    char *copy;
    int as_codes;
    if(word == NULL || out == NULL || index < 1) return ST_INVALID_INPUT;
    copy = malloc(strlen(word) + 1);
    as_codes = index % 10 == 0 || (index % 5 == 0);
    if(!copy) return ST_NO_MEMORY;
    strcpy(copy, word);
    if(index % 10 == 0 || (index % 2 == 0 && index % 5 != 0)) lower_latin(copy);
    if(as_codes) {
        Status st = codes_of(copy, index % 10 == 0 ? 4 : 8, out);
        free(copy);
        return st;
    }
    *out = copy;
    return ST_OK;
}

Status join_words(char *const *words, const int n, char **text)
{
    size_t len = 1, i;
    char *b;
    if(text == NULL || n < 0 || (n > 0 && words == NULL)) return ST_INVALID_INPUT;
    for(i = 0; i < (size_t)n; i++) {
        if(words[i] == NULL) return ST_INVALID_INPUT;
        len += strlen(words[i]) + 1;
    }
    b = malloc(len + 1);
    if(!b) return ST_NO_MEMORY;
    b[0] = 0;
    for(i = 0; i < (size_t)n; i++) {
        if(i) strcat(b, " ");
        strcat(b, words[i]);
    }
    strcat(b, "\n");
    *text = b;
    return ST_OK;
}

Status weave(char *const *a, const int na, char *const *b, const int nb, char ***out, int *nout)
{
    int i = 0, j = 0, k = 0;
    char **r;
    if(out == NULL || nout == NULL || na < 0 || nb < 0 || (na > 0 && a == NULL) || (nb > 0 && b == NULL))
        return ST_INVALID_INPUT;
    if(na + nb == 0) { 
        *out = NULL; 
        *nout = 0; 
        return ST_OK; 
    }
    r = malloc((size_t)(na + nb) * sizeof(char *));
    if(!r) return ST_NO_MEMORY;
    while(i < na || j < nb) {
        if(i < na) r[k++] = a[i++];
        if(j < nb) r[k++] = b[j++];
    }
    *out = r;
    *nout = k;
    return ST_OK;
}
