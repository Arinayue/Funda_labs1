#ifndef BASE_H
#define BASE_H

#include "status.h"

Status from_base(const char *s, const int base, long *out);
Status add(const long a, const long b, long *out);
unsigned long mag(const long v);
void print_num(const char *title, const long v);

#endif
