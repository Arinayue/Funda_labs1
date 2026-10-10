#include "parse.h"
#include <stdlib.h>
#include <string.h>

Status parse_flag(const char *s, char *op, int *has_n)
{
    if(s == NULL || op == NULL || has_n == NULL || (s[0] != '-' && s[0] != '/')) return ST_INVALID_INPUT;
    if(s[1] == 'n') {
        if(s[2] == '\0' || s[3] != '\0') return ST_INVALID_INPUT;
        *has_n = 1; *op = s[2];
    } else {
        if(s[1] == '\0' || s[2] != '\0') return ST_INVALID_INPUT;
        *has_n = 0; *op = s[1];
    }
    return (*op == 'd' || *op == 'i' || *op == 's' || *op == 'a') ? ST_OK : ST_INVALID_INPUT;
}

Status out_name(const char *in, char **out)
{
    const char *base = in, *p;
    size_t dir, len;
    char *r;
    if(in == NULL || out == NULL || in[0] == '\0') return ST_INVALID_INPUT;
    for(p = in; *p; p++)
        if(*p == '/' || *p == '\\') base = p + 1;
    if(base[0] == '\0') return ST_INVALID_INPUT;
    dir = (size_t)(base - in);
    len = strlen(base);
    r = malloc(dir + len + 5);
    if(r == NULL) return ST_NO_MEMORY;
    memcpy(r, in, dir);
    memcpy(r + dir, "out_", 4);
    memcpy(r + dir + 4, base, len + 1);
    *out = r;
    return ST_OK;
}
