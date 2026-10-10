#include "parsing.h"
#include <limits.h>
#include <stdlib.h>

Status parse_l(const char *s, long *out)
{
    char *end;
    long v;
    if (s == NULL || out == NULL || *s == '\0') return ST_INVALID_INPUT;
    v = strtol(s, &end, 10);
    if (end == s || *end != '\0') return ST_INVALID_INPUT;
    if (v == LONG_MAX || v == LONG_MIN) {
        return ST_OVERFLOW;
    }
    *out = v;
    return ST_OK;
}

Status parse_flag(const char *s, char *op)
{
    if(s == NULL || op == NULL || (s[0] != '-' && s[0] != '/') || s[1] == '\0' || s[2] != '\0')
        return ST_INVALID_INPUT;
    *op = s[1];
    return ST_OK;
}
