#ifndef HANDLERS_H
#define HANDLERS_H

#include "status.h"

Status do_h(const long x, long *buf, int *n);
Status do_p(const long x, int *kind);
Status do_s(const long x, char *out);
Status do_e(const long x, long long t[11][11]);
Status do_a(const long n, long long *out);
Status do_f(const long n, long *out);

#endif
