#ifndef TEXT_H
#define TEXT_H

#include "status.h"
#include <stddef.h>

Status transform(const char *in, const size_t n, const char op, char **out);

#endif
