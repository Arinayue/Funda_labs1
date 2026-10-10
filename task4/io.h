#ifndef IO_H
#define IO_H

#include "status.h"
#include <stdio.h>

Status load_file(FILE *in, char **data, size_t *len);
Status save_file(FILE *out, const char *data);

#endif
