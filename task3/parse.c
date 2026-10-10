#include "parse.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>

Status parse_l(const char *s, long *out)
{
    char *end;
    long v;
    if (s == NULL || out == NULL || *s == '\0') return ST_INVALID_INPUT;
    v = strtol(s, &end, 10);
    if (end == s || *end != '\0') return ST_INVALID_INPUT;
    if (v == LONG_MAX || v == LONG_MIN) return ST_OVERFLOW;
    *out = v;
    return ST_OK;
}

Status parse_double(const char *s, double *out)
{
    char *end;
    double v;
    if (s == NULL || out == NULL || *s == '\0') return ST_INVALID_INPUT;
    v = strtod(s, &end);
    if (end == s || *end != '\0') return ST_INVALID_INPUT;
    if (v == HUGE_VAL || v == -HUGE_VAL || isnan(v) || isinf(v)) return ST_OVERFLOW;
    *out = v;
    return ST_OK;
}

Status parse_flag(const char *s, char *op)
{
    if (s == NULL || op == NULL || (s[0] != '-' && s[0] != '/') || s[1] == '\0' || s[2] != '\0')
        return ST_INVALID_INPUT;
    if (s[1] != 'q' && s[1] != 'm' && s[1] != 't') return ST_INVALID_INPUT;
    *op = s[1];
    return ST_OK;
}
