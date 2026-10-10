#include "io.h"
#include <stdlib.h>

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

Status load_file(FILE *in, char **data, size_t *len)
{
    char *buf = NULL;
    size_t n = 0, cap = 0;
    int c;
    if(in == NULL || data == NULL || len == NULL) return ST_INVALID_INPUT;
    while((c = fgetc(in)) != EOF) {
        Status st = add_byte(&buf, &n, &cap, (char)c);
        if(st != ST_OK) { 
            free(buf); 
            return st; 
        }
    }
    if(ferror(in)) { 
        free(buf); 
        return ST_FILE; 
    }
    if(add_byte(&buf, &n, &cap, '\0') != ST_OK) { 
        free(buf); 
        return ST_NO_MEMORY; 
    }
    *data = buf;
    *len = n - 1;
    return ST_OK;
}

Status save_file(FILE *out, const char *data)
{
    if(out == NULL || data == NULL) return ST_INVALID_INPUT;
    if(fputs(data, out) < 0) return ST_FILE;
    return ST_OK;
}
