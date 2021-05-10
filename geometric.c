#include "geometric.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "macro.h"
#include "pixel.h"

Pair PairOrdered(double x, double y) {
    return (x <= y) ? (Pair){x, y} : (Pair){y, x};
}

bool IsOrdered(Pair pair) {
    return pair.x <= pair.y;
}

Pair PairWraps(Pair a, Pair b) {
    assert(IsOrdered(a));
    assert(IsOrdered(b));

    double lower = (a.x <= b.x) ? a.x : b.x;
    double higher = (a.y >= b.y) ? a.y : b.y;

    return (Pair){lower, higher};
}

Pair PairRand(unsigned* seed) {
    return (Pair){
        .x = GENF(seed),
        .y = GENF(seed),
    };
}

Pair PairRandDisk(double radius, unsigned* seed) {
    forever {
        Pair pair = PairRand(seed);
        if (pair.x * pair.x + pair.y * pair.y <= 1) {
            return (Pair){pair.x * radius, pair.y * radius};
        }
    }
}

double VecDim(Vector vec, int dim) {
    switch (dim) {
        case 0:
            return vec.x;
        case 1:
            return vec.y;
        case 2:
            return vec.z;
        default:
            assert(0 && "dim should be one of {0, 1, 2}");
    }
}

bool VecEq(Vector a, Vector b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

Vector VecAdd(Vector a, Vector b) {
    return (Vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector VecSub(Vector a, Vector b) {
    return (Vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector VecMul(Vector a, Vector b) {
    return (Vector){a.x * b.x, a.y * b.y, a.z * b.z};
}

Vector VecDiv(Vector a, Vector b) {
    assert(VecAll(b));
    return (Vector){a.x / b.x, a.y / b.y, a.z / b.z};
}

Vector VecAddS(Vector vec, double s) {
    return (Vector){vec.x + s, vec.y + s, vec.z + s};
}

Vector VecSubS(Vector vec, double s) {
    return (Vector){vec.x - s, vec.y - s, vec.z - s};
}

Vector VecMulS(Vector vec, double s) {
    return (Vector){vec.x * s, vec.y * s, vec.z * s};
}

Vector VecDivS(Vector vec, double s) {
    assert(s != 0);
    return (Vector){vec.x / s, vec.y / s, vec.z / s};
}

void VecIAdd(Vector* restrict a, Vector b) {
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

void VecISub(Vector* restrict a, Vector b) {
    a->x -= b.x;
    a->y -= b.y;
    a->z -= b.z;
}

void VecIMul(Vector* restrict a, Vector b) {
    a->x *= b.x;
    a->y *= b.y;
    a->z *= b.z;
}

void VecIDiv(Vector* restrict a, Vector b) {
    assert(VecAll(b));

    a->x /= b.x;
    a->y /= b.y;
    a->z /= b.z;
}

void VecIAddS(Vector* restrict vec, double s) {
    vec->x += s;
    vec->y += s;
    vec->z += s;
}

void VecISubS(Vector* restrict vec, double s) {
    vec->x -= s;
    vec->y -= s;
    vec->z -= s;
}

void VecIMulS(Vector* restrict vec, double s) {
    vec->x *= s;
    vec->y *= s;
    vec->z *= s;
}

void VecIDivS(Vector* restrict vec, double s) {
    assert(s != 0);

    vec->x /= s;
    vec->y /= s;
    vec->z /= s;
}

Vector VecCross(Vector a, Vector b) {
    // cross(a, b) will utilize three determinants
    return (Vector){
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x,
    };
}

double VecDot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double VecL2(Vector vec) {
    return VecDot(vec, vec);
}

double VecLen(Vector vec) {
    return sqrt(VecL2(vec));
}

Vector VecUnit(Vector vec) {
    double length = VecLen(vec);
    return VecDivS(vec, length);
}

Vector VecSqrt(Vector vec) {
    return (Vector){sqrt(vec.x), sqrt(vec.y), sqrt(vec.z)};
}

Vector VecAbs(Vector vec) {
    return (Vector){fabs(vec.x), fabs(vec.y), fabs(vec.z)};
}

bool VecNaN(Vector vec) {
    return isnan(vec.x) && isnan(vec.y) && isnan(vec.z);
}

bool VecAll(Vector vec) {
    return vec.x != 0 && vec.y != 0 && vec.z != 0;
}

bool VecAny(Vector vec) {
    return vec.x != 0 || vec.y != 0 || vec.z != 0;
}

Vector VecUniform(double n) {
    return (Vector){n, n, n};
}

Vector VecO(void) {
    return (Vector){0, 0, 0};
}

Vector VecI(void) {
    return (Vector){1, 0, 0};
}

Vector VecJ(void) {
    return (Vector){0, 1, 0};
}

Vector VecK(void) {
    return (Vector){0, 0, 1};
}

Vector VecRand(unsigned* seed) {
    return (Vector){
        .x = GENF(seed),
        .y = GENF(seed),
        .z = GENF(seed),
    };
}

Vector VecRandBall(double radius, unsigned* seed) {
    forever {
        Vector vec = VecRand(seed);
        if (VecL2(vec) <= 1) {
            return VecMulS(vec, radius);
        }
    }
}

Pixel Vec2Px(Vector vec) {
    assert(vec.x >= 0);
    assert(vec.x < 1);
    assert(vec.y >= 0);
    assert(vec.y < 1);
    assert(vec.z >= 0);
    assert(vec.z < 1);

    return (Pixel){
        .r = (char)(vec.x / 255),
        .g = (char)(vec.y / 255),
        .b = (char)(vec.z / 255),
    };
}

Box MakeBox(double x1, double x2, double y1, double y2, double z1, double z2) {
    // X, Y, Z are ordered pairs.
    return (Box){
        .x = PairOrdered(x1, x2),
        .y = PairOrdered(y1, y2),
        .z = PairOrdered(z1, z2),
    };
}

bool ThroughBox(Box box, Vector source, Vector towards) {
    Vector min = (Vector){box.x.x, box.y.x, box.z.x};
    Vector max = (Vector){box.x.y, box.y.y, box.z.y};

    // t_min will be the largest of the smaller values.
    double t_min = -INFINITY;
    // t_max will be the smallest of the larger values.
    double t_max = +INFINITY;

    // TODO: explain why this works.
    for (int i = 0; i < 3; ++i) {
        double inv_b = 1. / VecDim(towards, i);

        double t_small = (VecDim(min, i) - VecDim(source, i)) * inv_b;
        double t_large = (VecDim(max, i) - VecDim(source, i)) * inv_b;

        if (inv_b < 0) {
            SWAP(double, t_small, t_large);
        }

        assert(t_small < t_large);

        if (t_min < t_small) {
            t_min = t_small;
        }
        if (t_max > t_large) {
            t_max = t_large;
        }
    }

    // If t_min < t_max, that means the ray didn't pass through the block.
    return t_min < t_max;
}

Vector BoxCenter(Box box) {
    return (Vector){
        .x = (box.x.x + box.x.y) / 2.,
        .y = (box.y.x + box.y.y) / 2.,
        .z = (box.z.x + box.z.y) / 2.,
    };
}

Box BoxWraps(Box a, Box b) {
    return (Box){
        .x = PairWraps(a.x, b.x),
        .y = PairWraps(a.y, b.y),
        .z = PairWraps(a.z, b.z),
    };
}
