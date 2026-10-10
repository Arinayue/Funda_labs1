#include "io.h"
#include <stdlib.h>

static int sp(const int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'; }

void free_words(char **words, const int n)
{
    int i;
    if(words == NULL) return;
    for(i = 0; i < n; i++) free(words[i]);
    free(words);
}

static Status next_word(FILE *fp, char **word, int *got)
{
    int c;
    size_t n = 0, cap = 32;
    char *b;
    if(fp == NULL || word == NULL || got == NULL) return ST_INVALID_INPUT;
    *got = 0; *word = NULL;
    do { c = fgetc(fp); if(c == EOF) return ferror(fp) ? ST_FILE : ST_OK; } while(sp(c));
    b = malloc(cap);
    if(!b) return ST_NO_MEMORY;
    while(c != EOF && !sp(c)) {
        if(n + 1 >= cap) {
            char *g = realloc(b, cap *= 2);
            if (!g) { 
                free(b); 
                return ST_NO_MEMORY; 
            }
            b = g;
        }
        b[n++] = (char)c;
        c = fgetc(fp);
    }
    if (ferror(fp)) { 
        free(b); 
        return ST_FILE; 
    }
    b[n] = 0;
    *word = b; *got = 1;
    return ST_OK;
}

Status load_words(FILE *fp, char ***words, int *count)
{
    char **list = NULL;
    int n = 0;
    if(fp == NULL || words == NULL || count == NULL) return ST_INVALID_INPUT;
    *words = NULL; *count = 0;
    for (;;) {
        char *w = NULL, **g;
        int got = 0;
        Status st = next_word(fp, &w, &got);
        if(st != ST_OK) { 
            free_words(list, n); 
            return st; 
        }
        if(!got) break;
        g = realloc(list, (size_t)(n + 1) * sizeof(char *));
        if (!g) { 
            free(w); 
            free_words(list, n); 
            return ST_NO_MEMORY; 
        }
        list = g;
        list[n++] = w;
    }
    *words = list;
    *count = n;
    return ST_OK;
}

Status save_text(FILE *out, const char *text)
{
    if(out == NULL || text == NULL) return ST_INVALID_INPUT;
    return fputs(text, out) < 0 ? ST_FILE : ST_OK;
}
