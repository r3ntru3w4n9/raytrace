#include "geometric.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "macro.h"
#include "pixel.h"

Pair Pair_ordered(double x, double y) {
    return (x <= y) ? (Pair){x, y} : (Pair){y, x};
}

bool Pair_is_ordered(Pair pair) {
    return pair.x <= pair.y;
}

Pair Pair_wraps(Pair a, Pair b) {
    assert(Pair_is_ordered(a));
    assert(Pair_is_ordered(b));

    double lower = (a.x <= b.x) ? a.x : b.x;
    double higher = (a.y >= b.y) ? a.y : b.y;

    return (Pair){lower, higher};
}

Pair Pair_rand_r(unsigned* seed) {
    return (Pair){
        .x = genfloat(seed),
        .y = genfloat(seed),
    };
}

Pair Pair_rand_disk(double radius, unsigned* seed) {
    forever {
        Pair pair = Pair_rand_r(seed);
        if (pair.x * pair.x + pair.y * pair.y <= 1) {
            return (Pair){pair.x * radius, pair.y * radius};
        }
    }
}

double Vec_dim(Vector vec, int dim) {
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

bool Vec_eq(Vector a, Vector b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

Vector Vec_add(Vector a, Vector b) {
    return (Vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector Vec_sub(Vector a, Vector b) {
    return (Vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector Vec_mul(Vector a, Vector b) {
    return (Vector){a.x * b.x, a.y * b.y, a.z * b.z};
}

Vector Vec_div(Vector a, Vector b) {
    assert(Vec_all(b));
    return (Vector){a.x / b.x, a.y / b.y, a.z / b.z};
}

Vector Vec_add_s(Vector vec, double s) {
    return (Vector){vec.x + s, vec.y + s, vec.z + s};
}

Vector Vec_sub_s(Vector vec, double s) {
    return (Vector){vec.x - s, vec.y - s, vec.z - s};
}

Vector Vec_mul_s(Vector vec, double s) {
    return (Vector){vec.x * s, vec.y * s, vec.z * s};
}

Vector Vec_div_s(Vector vec, double s) {
    assert(s != 0);
    return (Vector){vec.x / s, vec.y / s, vec.z / s};
}

void Vec_iadd(Vector* restrict a, Vector b) {
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

void Vec_isub(Vector* restrict a, Vector b) {
    a->x -= b.x;
    a->y -= b.y;
    a->z -= b.z;
}

void Vec_imul(Vector* restrict a, Vector b) {
    a->x *= b.x;
    a->y *= b.y;
    a->z *= b.z;
}

void Vec_idiv(Vector* restrict a, Vector b) {
    assert(Vec_all(b));

    a->x /= b.x;
    a->y /= b.y;
    a->z /= b.z;
}

void Vec_iadd_s(Vector* restrict vec, double s) {
    vec->x += s;
    vec->y += s;
    vec->z += s;
}

void Vec_isub_s(Vector* restrict vec, double s) {
    vec->x -= s;
    vec->y -= s;
    vec->z -= s;
}

void Vec_imul_s(Vector* restrict vec, double s) {
    vec->x *= s;
    vec->y *= s;
    vec->z *= s;
}

void Vec_idiv_s(Vector* restrict vec, double s) {
    assert(s != 0);

    vec->x /= s;
    vec->y /= s;
    vec->z /= s;
}

Vector Vec_cross(Vector a, Vector b) {
    // cross(a, b) will utilize three determinants
    return (Vector){
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x,
    };
}

double Vec_dot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double Vec_l2(Vector vec) {
    return Vec_dot(vec, vec);
}

double Vec_len(Vector vec) {
    return sqrt(Vec_l2(vec));
}

Vector Vec_unit(Vector vec) {
    double length = Vec_len(vec);
    return Vec_div_s(vec, length);
}

Vector Vec_sqrt(Vector vec) {
    return (Vector){sqrt(vec.x), sqrt(vec.y), sqrt(vec.z)};
}

Vector Vec_abs(Vector vec) {
    return (Vector){fabs(vec.x), fabs(vec.y), fabs(vec.z)};
}

bool Vec_NaN(Vector vec) {
    return isnan(vec.x) && isnan(vec.y) && isnan(vec.z);
}

bool Vec_all(Vector vec) {
    return vec.x != 0 && vec.y != 0 && vec.z != 0;
}

bool Vec_any(Vector vec) {
    return vec.x != 0 || vec.y != 0 || vec.z != 0;
}

Vector Vec_from(double n) {
    return (Vector){n, n, n};
}

Vector Vec_o(void) {
    return (Vector){0, 0, 0};
}

Vector Vec_i(void) {
    return (Vector){1, 0, 0};
}

Vector Vec_j(void) {
    return (Vector){0, 1, 0};
}

Vector Vec_k(void) {
    return (Vector){0, 0, 1};
}

Vector Vec_rand_r(unsigned* seed) {
    return (Vector){
        .x = genfloat(seed),
        .y = genfloat(seed),
        .z = genfloat(seed),
    };
}

Vector Vec_rand_ball(double radius, unsigned* seed) {
    forever {
        Vector vec = Vec_rand_r(seed);
        if (Vec_l2(vec) <= 1) {
            return Vec_mul_s(vec, radius);
        }
    }
}

Pixel Vec_2Px(Vector vec) {
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

Box Box_make(double x1, double x2, double y1, double y2, double z1, double z2) {
    // X, Y, Z are ordered pairs.
    return (Box){
        .x = Pair_ordered(x1, x2),
        .y = Pair_ordered(y1, y2),
        .z = Pair_ordered(z1, z2),
    };
}

bool Box_is_through(Box box, Vector source, Vector towards) {
    Vector min = (Vector){box.x.x, box.y.x, box.z.x};
    Vector max = (Vector){box.x.y, box.y.y, box.z.y};

    // t_min will be the largest of the smaller values.
    double t_min = -INFINITY;
    // t_max will be the smallest of the larger values.
    double t_max = +INFINITY;

    // TODO: explain why this works.
    for (int i = 0; i < 3; ++i) {
        double inv_b = 1. / Vec_dim(towards, i);

        double t_small = (Vec_dim(min, i) - Vec_dim(source, i)) * inv_b;
        double t_large = (Vec_dim(max, i) - Vec_dim(source, i)) * inv_b;

        if (inv_b < 0) {
            swap(double, t_small, t_large);
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

Vector Box_center(Box box) {
    return (Vector){
        .x = (box.x.x + box.x.y) / 2.,
        .y = (box.y.x + box.y.y) / 2.,
        .z = (box.z.x + box.z.y) / 2.,
    };
}

Box Box_wraps(Box a, Box b) {
    return (Box){
        .x = Pair_wraps(a.x, b.x),
        .y = Pair_wraps(a.y, b.y),
        .z = Pair_wraps(a.z, b.z),
    };
}
