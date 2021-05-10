#pragma once

#include <stdbool.h>

#include "pixel.h"

// External linkage.
struct Pixel;

// A pair consists of two numbers. (x, y) respectively.
// @author RenTrueWang
typedef struct Pair {
    // (x, y) of the pair.
    double x, y;
} Pair;

// PairOrdered creates a pair that's ordered. (pair.x < pair.y)
// @param x The first number.
// @param y The second number.
// @return The pair generated such that pair.x < pair.y
Pair Pair_ordered(double x, double y);

// Checks if a pair is ordered.
// @param pair The pair is checked.
// @return True if the pair is ordered.
bool Pair_is_ordered(Pair pair);

// Random pair in the range [0, 1].
// @param seed The seed pointer for the random number generator.
// @return A random pair.
Pair Pair_rand_r(unsigned* seed);

// Random pair that with norm <= radius.
// @param radius The radius to which the norm of the result pair is smaller
// @param seed The seed pointer for the random number generator.
// @return A random pair in a ball.
Pair Pair_rand_disk(double radius, unsigned* seed);

// Creates a pair that contains both of the pairs.
// @param a The first pair.
// @param b The second pair.
// @return A pair that is big enough to contain both a and b.
Pair Pair_wraps(Pair a, Pair b);

// Vector is composed of three numbers. (x, y, z) respectively.
// @author RenTrueWang
typedef struct Vector {
    // (x, y, z) are the locations in 3D space.
    double x, y, z;
} Vector;

// vec[dim]
// @param vec The vector to use.
// @param dim The dimension of the vector to access. dim is one of {0, 1, 2}.
// @return The value at the dimension.
double Vec_dim(Vector vec, int dim);

// a == b
// @param a First vector.
// @param b Second vector.
// @return Return if a is equal to b
bool Vec_eq(Vector a, Vector b);

// a + b
// @param a First vector.
// @param b Second vector.
// @return The value of a + b
Vector Vec_add(Vector a, Vector b);

// a - b
// @param a First vector.
// @param b Second vector.
// @return The value of a - b
Vector Vec_sub(Vector a, Vector b);

// a * b
// @param a First vector.
// @param b Second vector.
// @return The value of a * b
Vector Vec_mul(Vector a, Vector b);

// a / b
// @param a First vector.
// @param b Second vector. Cannot contain 0.
// @return The value of a / b
Vector Vec_div(Vector a, Vector b);

// vec + s
// @param vec Vector.
// @param s Scalar.
// @return The value of vec + s after broadcast operation.
Vector Vec_add_s(Vector vec, double s);

// vec - s
// @param vec Vector.
// @param s Scalar.
// @return The value of vec - s after broadcast operation.
Vector Vec_sub_s(Vector vec, double s);

// vec - s
// @param vec Vector.
// @param s Scalar.
// @return The value of vec - s after broadcast operation.
Vector Vec_mul_s(Vector vec, double s);

// vec / s
// @param vec Vector.
// @param s Scalar. Cannot be 0.
// @return The value of vec / s after broadcast operation.
Vector Vec_div_s(Vector vec, double s);

// a += b
// @param a Vector to modify.
// @param b Vector to use.
void Vec_iadd(Vector* a, Vector b);

// a -= b
// @param a Vector to modify.
// @param b Vector to use.
void Vec_isub(Vector* a, Vector b);

// a *= b
// @param a Vector to modify.
// @param b Vector to use.
void Vec_imul(Vector* a, Vector b);

// a /= b
// @param a Vector to modify.
// @param b Vector to use. Cannot contain 0.
void Vec_idiv(Vector* a, Vector b);

// vec += s
// @param vec Vector to modify.
// @param s Scalar to use.
void Vec_iadd_s(Vector* vec, double s);

// vec -= s
// @param vec Vector to modify.
// @param s Scalar to use.
void Vec_isub_s(Vector* vec, double s);

// vec *= s
// @param vec Vector to modify.
// @param s Scalar to use.
void Vec_imul_s(Vector* vec, double s);

// vec /= s
// @param vec Vector to modify.
// @param s Scalar to use. Cannot be 0.
void Vec_idiv_s(Vector* vec, double s);

// cross(a, b)
// @param a First vector.
// @param b Second vector.
// @return The value of a cross b.
Vector Vec_cross(Vector a, Vector b);

// dot(a, b)
// @param a First vector.
// @param b Second vector.
// @return The value of a dot b.
double Vec_dot(Vector a, Vector b);

// dot(vec, vec)
// @param vec Vector.
// @return The L2 norm of the vector.
double Vec_l2(Vector vec);

// len(vec)
// @param vec Vector.
// @return The length of the vector.
double Vec_len(Vector vec);

// vec/len(vec)
// @param vec Vector.
// @return The direction of the vector.
Vector Vec_unit(Vector vec);

// sqrt(vec)
// @param vec Vector.
// @return The sqrt of the vector.
Vector Vec_sqrt(Vector vec);

// abs(vec)
// @param vec Vector.
// @return The absolute value of the vector.
Vector Vec_abs(Vector vec);

// isnan(vec)
// @param vec Vector.
// @return True if the vector contains NaN.
bool Vec_nan(Vector vec);

// all(vec)
// @param vec Vector.
// @return True if the vector contains no 0. Else False.
bool Vec_all(Vector vec);

// any(vec)
// @param vec Vector.
// @return False if all elements are 0. Else True.
bool Vec_any(Vector vec);

// (n, n, n)
// @param n Number.
// @return A vector (n, n, n)
Vector Vec_from(double n);

// (0, 0, 0)
// @return A vector (0, 0, 0)
Vector Vec_o(void);

// (1, 0, 0)
// @return A vector (1, 0, 0)
Vector Vec_i(void);

// (0, 1, 0)
// @return A vector (0, 1, 0)
Vector Vec_j(void);

// (0, 0, 1)
// @return A vector (0, 0, 1)
Vector Vec_k(void);

// Random vector in the range [0, 1].
// @param seed The seed pointer for the random number generator.
// @return A random vector.
Vector Vec_rand_r(unsigned* seed);

// Random vector that with norm <= radius.
// @param radius The radius to which the norm of the result vector is smaller
// @param seed The seed pointer for the random number generator.
// @return A random vector in a ball.
Vector Vec_rand_ball(double radius, unsigned* seed);

// Converts a vector to a pixel. Scales the range from [0, 1) to [0, 255] integer.
// @param vec Vector to transform.
// @return A pixel (r, g, b) in the range [0, 255].
// @see Pixel
struct Pixel Vec_2Px(Vector vec);

// A box is a 3D box that holds some volumes.
// @author RenTrueWang
typedef struct Box {
    // A box is three pairs of (low, high).
    Pair x, y, z;
} Box;

// Creates a box based on x, y, z pairs.
// @param x1 The first x.
// @param x2 The second x.
// @param y1 The first y.
// @param y2 The second y.
// @param z1 The first z.
// @param z2 The second z.
// @return A box that has the boundaries as the parameters.
Box Box_make(double x1, double x2, double y1, double y2, double z1, double z2);

// Determines whether the box intersects with the ray.
// @param box The box for the ray to hit.
// @param source The source of the ray.
// @param ray The direction of the ray.
// @return True if the ray intersects with the box.
bool Box_is_through(Box box, Vector source, Vector towards);

// The center of the box.
// @param box The box to use.
// @return The 3D dimensional position for the center of the box.
Vector Box_center(Box box);

// Creates a box that contains both of the boxes.
// @param a The first box.
// @param b The second box.
// @return A box that is big enough to contain both a and b.
Box Box_wraps(Box a, Box b);
