#ifndef IO_H
#define IO_H

#include "status.h"
#include <stdio.h>

Status load_words(FILE *fp, char ***words, int *count);
Status save_text(FILE *out, const char *text);
void free_words(char **words, const int n);

#endif
