#include "parse.h"

Status parse_flag(const char *s, char *op)
{
    if (!s || !op || (s[0] != '-' && s[0] != '/') || !s[1] || s[2]) return ST_INVALID_INPUT;
    if (s[1] != 'r' && s[1] != 'a') return ST_INVALID_INPUT;
    *op = s[1];
    return ST_OK;
}
