#include "io.h"
#include "parse.h"
#include "words.h"
#include <stdio.h>
#include <stdlib.h>

static void close_files(FILE *a, FILE *b, FILE *o)
{
    if (a) fclose(a);
    if (b) fclose(b);
    if (o) fclose(o);
}

int main(int argc, char **argv)
{
    char op;
    FILE *a = NULL, *b = NULL, *o = NULL;
    char **w1 = NULL, **w2 = NULL, **mixed = NULL, **changed = NULL, *text = NULL;
    int n1 = 0, n2 = 0, nm = 0, i;
    Status st = ST_OK;

    if(argc < 2 || parse_flag(argv[1], &op) != ST_OK || !((op == 'r' && argc == 5) || (op == 'a' && argc == 4))) {
        printf("task07 -r ф1 ф2 выход | -a вход выход\n");
        return 1;
    }

    if(op == 'r') {
        a = fopen(argv[2], "rb");
        b = fopen(argv[3], "rb");
        o = fopen(argv[4], "wb");
        if (!a || !b || !o) {
            printf("Не открылся файл\n");
            close_files(a, b, o);
            return 4;
        }
        st = load_words(a, &w1, &n1);
        if (st == ST_OK) st = load_words(b, &w2, &n2);
        if (st == ST_OK) st = weave(w1, n1, w2, n2, &mixed, &nm);
        if (st == ST_OK) st = join_words(mixed, nm, &text);
    } else {
        a = fopen(argv[2], "rb");
        o = fopen(argv[3], "wb");
        if(!a || !o) {
            printf("Не открылся файл\n");
            close_files(a, NULL, o);
            return 4;
        }
        st = load_words(a, &w1, &n1);
        if(st == ST_OK) {
            changed = calloc((size_t)n1, sizeof(char *));
            if(!changed) st = ST_NO_MEMORY;
        }
        for(i = 0; st == ST_OK && i < n1; i++)
            st = change_word(w1[i], i + 1, &changed[i]);
        if(st == ST_OK) st = join_words(changed, n1, &text);
    }

    if(st == ST_OK) st = save_text(o, text);
    close_files(a, b, o);
    free_words(w1, n1);
    free_words(w2, n2);
    free(mixed);
    if(changed) free_words(changed, n1);
    free(text);

    if (st == ST_OK) { 
        printf("Готово\n"); 
        return 0; 
    }
    printf(st == ST_NO_MEMORY ? "Нет памяти\n" : "Ошибка файла\n");
    return st == ST_NO_MEMORY ? 3 : 4;
}
