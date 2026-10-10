#ifndef PARSE_H
#define PARSE_H

#include "status.h"

Status parse_flag(const char *s, char *op, int *has_n);
Status out_name(const char *in, char **out);

#endif
