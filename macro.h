#pragma once

#define swap(T, X, Y) \
    do {              \
        T tmp = X;    \
        X = Y;        \
        Y = tmp;      \
    } while (0)

#define forever for (;;)

#define genfloat(SEED) (double)rand_r(SEED) / RAND_MAX
