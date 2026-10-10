#ifndef PARSING_H
#define PARSING_H

#include "status.h"

Status parse_l(const char *s, long *out);
Status parse_flag(const char *s, char *op);

#endif
