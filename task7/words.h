#ifndef WORDS_H
#define WORDS_H

#include "status.h"

Status change_word(const char *word, const int index, char **out);
Status join_words(char *const *words, const int n, char **text);
Status weave(char *const *a, const int na, char *const *b, const int nb, char ***out, int *nout);

#endif
