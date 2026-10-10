#include "io.h"
#include "parse.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char op;
    int has_n = 0;
    char *gen = NULL, *src = NULL, *dst = NULL;
    const char *path;
    FILE *in = NULL, *out = NULL;
    Status st;
    size_t len = 0;

    if(argc < 2 || parse_flag(argv[1], &op, &has_n) != ST_OK || argc != (has_n ? 4 : 3)) {
        printf("task04 <-d|-i|-s|-a> файл | <-nd|-ni|-ns|-na> вход выход\n");
        return 1;
    }
    if(has_n) {
        if(strcmp(argv[2], argv[3]) == 0) {
            printf("Вход и выход совпадают\n");
            return 1;
        }
        path = argv[3];
    }
    else {
        if (out_name(argv[2], &gen) != ST_OK) { 
            printf("Плохое имя файла\n"); 
            return 1; }
        path = gen;
    }

    in = fopen(argv[2], "rb");
    if(!in) { 
        printf("Не открылся входной файл\n"); 
        free(gen); 
        return 4; }
    st = load_file(in, &src, &len);
    fclose(in);
    if(st != ST_OK) {
        printf(st == ST_NO_MEMORY ? "Нет памяти\n" : "Ошибка файла\n");
        free(src);
        free(gen);
        return st == ST_NO_MEMORY ? 3 : 4;
    }

    out = fopen(path, "wb");
    if(!out) { printf("Не открылся выходной файл\n"); 
        free(src); 
        free(gen); 
        return 4; }
    st = transform(src, len, op, &dst);
    if(st == ST_OK) st = save_file(out, dst);
    fclose(out);
    free(src);
    free(dst);
    if(st == ST_OK) printf("Готово: %s\n", path);
    else printf(st == ST_NO_MEMORY ? "Нет памяти\n" : "Ошибка файла\n");
    free(gen);
    return st == ST_OK ? 0 : (st == ST_NO_MEMORY ? 3 : 4);
}
