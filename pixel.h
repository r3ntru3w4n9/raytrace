#pragma once

#include "geometric.h"

// External linkage.
struct Vector;

// A pixel is three integers in the range [0, 255]. Corresponding to (r, g, b)
// @author RenTrueWang
typedef struct Pixel {
    // (r, g, b) of a pixel
    char r, g, b;
} Pixel;

// Convert a pixel to a vector. Scale the range from [0, 255] integer to [0, 1).
// @param px Pixel to transform.
// @return A vector (x, y, z) in the range [0, 1).
// @see Vector
struct Vector Px_2Vec(Pixel px);
