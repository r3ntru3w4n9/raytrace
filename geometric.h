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
Pair PairOrdered(double x, double y);

// Check if a pair is ordered.
// @param pair The pair is checked.
// @return True if the pair is ordered.
bool IsOrdered(Pair pair);

// Random pair in the range [0, 1].
// @param seed The seed pointer for the random number generator.
// @return A random pair.
Pair PairRand(unsigned* seed);

// Random pair that with norm <= radius.
// @param radius The radius to which the norm of the result pair is smaller
// @param seed The seed pointer for the random number generator.
// @return A random pair in a ball.
Pair PairRandDisk(double radius, unsigned* seed);

// Create a pair that contains both of the pairs.
// @param a The first pair.
// @param b The second pair.
// @return A pair that is big enough to contain both a and b.
Pair PairWraps(Pair a, Pair b);

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
double VecDim(Vector vec, int dim);

// a == b
// @param a First vector.
// @param b Second vector.
// @return Return if a is equal to b
bool VecEq(Vector a, Vector b);

// a + b
// @param a First vector.
// @param b Second vector.
// @return The value of a + b
Vector VecAdd(Vector a, Vector b);

// a - b
// @param a First vector.
// @param b Second vector.
// @return The value of a - b
Vector VecSub(Vector a, Vector b);

// a * b
// @param a First vector.
// @param b Second vector.
// @return The value of a * b
Vector VecMul(Vector a, Vector b);

// a / b
// @param a First vector.
// @param b Second vector. Cannot contain 0.
// @return The value of a / b
Vector VecDiv(Vector a, Vector b);

// vec + s
// @param vec Vector.
// @param s Scalar.
// @return The value of vec + s after broadcast operation.
Vector VecAddS(Vector vec, double s);

// vec - s
// @param vec Vector.
// @param s Scalar.
// @return The value of vec - s after broadcast operation.
Vector VecSubS(Vector vec, double s);

// vec - s
// @param vec Vector.
// @param s Scalar.
// @return The value of vec - s after broadcast operation.
Vector VecMulS(Vector vec, double s);

// vec / s
// @param vec Vector.
// @param s Scalar. Cannot be 0.
// @return The value of vec / s after broadcast operation.
Vector VecDivS(Vector vec, double s);

// a += b
// @param a Vector to modify.
// @param b Vector to use.
void VecIAdd(Vector* a, Vector b);

// a -= b
// @param a Vector to modify.
// @param b Vector to use.
void VecISub(Vector* a, Vector b);

// a *= b
// @param a Vector to modify.
// @param b Vector to use.
void VecIMul(Vector* a, Vector b);

// a /= b
// @param a Vector to modify.
// @param b Vector to use. Cannot contain 0.
void VecIDiv(Vector* a, Vector b);

// vec += s
// @param vec Vector to modify.
// @param s Scalar to use.
void VecIAddS(Vector* vec, double s);

// vec -= s
// @param vec Vector to modify.
// @param s Scalar to use.
void VecISubS(Vector* vec, double s);

// vec *= s
// @param vec Vector to modify.
// @param s Scalar to use.
void VecIMulS(Vector* vec, double s);

// vec /= s
// @param vec Vector to modify.
// @param s Scalar to use. Cannot be 0.
void VecIDivS(Vector* vec, double s);

// cross(a, b)
// @param a First vector.
// @param b Second vector.
// @return The value of a cross b.
Vector VecCross(Vector a, Vector b);

// dot(a, b)
// @param a First vector.
// @param b Second vector.
// @return The value of a dot b.
double VecDot(Vector a, Vector b);

// dot(vec, vec)
// @param vec Vector.
// @return The L2 norm of the vector.
double VecL2(Vector vec);

// len(vec)
// @param vec Vector.
// @return The length of the vector.
double VecLen(Vector vec);

// vec/len(vec)
// @param vec Vector.
// @return The direction of the vector.
Vector VecUnit(Vector vec);

// sqrt(vec)
// @param vec Vector.
// @return The sqrt of the vector.
Vector VecSqrt(Vector vec);

// abs(vec)
// @param vec Vector.
// @return The absolute value of the vector.
Vector VecAbs(Vector vec);

// isnan(vec)
// @param vec Vector.
// @return True if the vector contains NaN.
bool VecNaN(Vector vec);

// all(vec)
// @param vec Vector.
// @return True if the vector contains no 0. Else False.
bool VecAll(Vector vec);

// any(vec)
// @param vec Vector.
// @return False if all elements are 0. Else True.
bool VecAny(Vector vec);

// (n, n, n)
// @param n Number.
// @return A vector (n, n, n)
Vector VecUniform(double n);

// (0, 0, 0)
// @return A vector (0, 0, 0)
Vector VecO(void);

// (1, 0, 0)
// @return A vector (1, 0, 0)
Vector VecI(void);

// (0, 1, 0)
// @return A vector (0, 1, 0)
Vector VecJ(void);

// (0, 0, 1)
// @return A vector (0, 0, 1)
Vector VecK(void);

// Random vector in the range [0, 1].
// @param seed The seed pointer for the random number generator.
// @return A random vector.
Vector VecRand(unsigned* seed);

// Random vector that with norm <= radius.
// @param radius The radius to which the norm of the result vector is smaller
// @param seed The seed pointer for the random number generator.
// @return A random vector in a ball.
Vector VecRandBall(double radius, unsigned* seed);

// Convert a vector to a pixel. Scale the range from [0, 1) to [0, 255] integer.
// @param vec Vector to transform.
// @return A pixel (r, g, b) in the range [0, 255].
// @see Pixel
struct Pixel Vec2Px(Vector vec);

// A box is a 3D box that holds some volumes.
// @author RenTrueWang
typedef struct Box {
    // A box is three pairs of (low, high).
    Pair x, y, z;
} Box;

// Create a box based on x, y, z pairs.
// @param x1 The first x.
// @param x2 The second x.
// @param y1 The first y.
// @param y2 The second y.
// @param z1 The first z.
// @param z2 The second z.
// @return A box that has the boundaries as the parameters.
Box MakeBox(double x1, double x2, double y1, double y2, double z1, double z2);

// Determine whether the box intersects with the ray.
// @param box The box for the ray to hit.
// @param source The source of the ray.
// @param ray The direction of the ray.
// @return True if the ray intersects with the box.
bool ThroughBox(Box box, Vector source, Vector towards);

// The center of the box.
// @param box The box to use.
// @return The 3D dimensional position for the center of the box.
Vector BoxCenter(Box box);

// Create a box that contains both of the boxes.
// @param a The first box.
// @param b The second box.
// @return A box that is big enough to contain both a and b.
Box BoxWraps(Box a, Box b);
