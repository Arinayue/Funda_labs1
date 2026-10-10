#ifndef PARSE_H
#define PARSE_H

#include "status.h"

Status parse_l(const char *s, long *out);
Status parse_double(const char *s, double *out);
Status parse_flag(const char *s, char *op);

#endif
