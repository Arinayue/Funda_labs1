#ifndef IO_H
#define IO_H

#include "status.h"
#include <stdio.h>

Status next_word(FILE *fp, char **word, int *got);
Status write_row(FILE *out, const char *clean, const int base, const char *decimal);

#endif
