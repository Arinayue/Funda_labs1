#ifndef STATUS_H
#define STATUS_H

typedef enum {
    ST_OK = 0,
    ST_INVALID_INPUT,
    ST_OVERFLOW,
    ST_NO_MEMORY,
    ST_FILE,
    ST_NO_CONVERGENCE,
    ST_DIV_ZERO,
    ST_NO_SOLUTION
} Status;

#endif
