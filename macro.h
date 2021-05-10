#pragma once

// Swapping based on type.
// @param T The type of X, Y
// @param X The first value to swap.
// @param Y The second value to swap.
#define swap(T, X, Y) \
    do {              \
        T tmp = X;    \
        X = Y;        \
        Y = tmp;      \
    } while (0)

// An endless loop. Usage: forever {}
#define forever for (;;)

// Generates a floating point in the range [0, 1].
// @param SEED The pointer-to-state used in the random number generator.
#define genfloat(SEED) (double)rand_r(SEED) / RAND_MAX
