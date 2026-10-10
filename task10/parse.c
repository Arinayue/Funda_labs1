#include "parse.h"
#include <limits.h>
#include <stdlib.h>

Status parse_l(const char *s, long *out)
{
    char *end;
    long v;
    if(s == NULL || out == NULL || *s == '\0') return ST_INVALID_INPUT;
    v = strtol(s, &end, 10);
    if(end == s || *end != '\0') return ST_INVALID_INPUT;
    if(v == LONG_MAX || v == LONG_MIN) return ST_OVERFLOW;
    *out = v;
    return ST_OK;
}
