#include "io.h"
#include <stdlib.h>

static int sp(const int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'; }

Status next_word(FILE *fp, char **word, int *got)
{
    int c;
    size_t n = 0, cap = 32;
    char *b;
    if(fp == NULL || word == NULL || got == NULL) return ST_INVALID_INPUT;
    *got = 0; *word = NULL;
    do { c = fgetc(fp); if (c == EOF) return ferror(fp) ? ST_FILE : ST_OK; } while (sp(c));
    b = malloc(cap);
    if(!b) return ST_NO_MEMORY;
    while(c != EOF && !sp(c)) {
        if (n + 1 >= cap) {
            char *g = realloc(b, cap *= 2);
            if(!g) { 
                free(b); 
                return ST_NO_MEMORY; 
            }
            b = g;
        }
        b[n++] = (char)c;
        c = fgetc(fp);
    }
    if(ferror(fp)) { 
        free(b); 
        return ST_FILE; 
    }
    b[n] = 0; *word = b; *got = 1;
    return ST_OK;
}

Status write_row(FILE *out, const char *clean, const int base, const char *decimal)
{
    if(out == NULL || clean == NULL || decimal == NULL || base < 2 || base > 36) return ST_INVALID_INPUT;
    if(fprintf(out, "%s %d %s\n", clean, base, decimal) < 0) return ST_FILE;
    return ST_OK;
}
