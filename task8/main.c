#include "convert.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *in = NULL, *out = NULL;
    Status st = ST_OK;

    if(argc != 3) { 
        printf("task08 <вход> <выход>\n"); 
        return 1; 
    }
    if(strcmp(argv[1], argv[2]) == 0) {
        printf("Вход и выход совпадают\n");
        return 1;
    }
    in = fopen(argv[1], "rb");
    if(!in) {
        printf("Не открылся входной файл\n"); 
        return 4; 
    }
    out = fopen(argv[2], "wb");
    if(!out) { 
        printf("Не открылся выходной файл\n"); 
        fclose(in); return 4; 
    }

    for (;;) {
        char *tok = NULL, *clean = NULL, *dec = NULL;
        int got = 0, base = 0;
        st = next_word(in, &tok, &got);
        if(st != ST_OK || !got) { 
            free(tok); 
            break; 
        }
        st = describe(tok, &clean, &base, &dec);
        free(tok);
        if(st != ST_OK) break;
        st = write_row(out, clean, base, dec);
        free(clean);
        free(dec);
        if(st != ST_OK) break;
    }
    fclose(in);
    fclose(out);
    if(st == ST_OK) { 
        printf("Готово: %s\n", argv[2]); 
        return 0; 
    }
    printf(st == ST_INVALID_INPUT ? "В файле не число\n" : st == ST_NO_MEMORY ? "Нет памяти\n" : "Ошибка файла\n");
    return st == ST_NO_MEMORY ? 3 : 1;
}
