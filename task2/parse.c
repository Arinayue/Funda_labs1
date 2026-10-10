#include "parse.h"
#include <math.h>
#include <stdlib.h>

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
