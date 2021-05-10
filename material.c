#include "material.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "macro.h"

Vector MatScatter(Material mat, Vector input, Vector normal, unsigned* seed) {
    return mat.scatter(mat.object, input, normal, seed);
}

Vector MatAlbedo(Material mat) {
    return mat.albedo(mat.object);
}

// MatteScatter is the implementation of scatter for Matte.
static Vector MatteScatter(const void* ma,
                           Vector input,
                           Vector normal,
                           unsigned* seed) {
    // Matte reflects perfectly so doesn't care about the input.
    (void)ma;
    (void)input;

    Vector un = VecUnit(normal);
    Vector rb = VecRandBall(1., seed);
    return VecAdd(rb, un);
}

// MatteAlbedo is the implementation of albedo for Matte.
static Vector MatteAlbedo(const void* ma) {
    const Matte* matte = ma;
    return matte->albedo;
}

Material MatteAsMaterial(const Matte* matte) {
    return (Material){
        .object = matte,
        .scatter = MatteScatter,
        .albedo = MatteAlbedo,
    };
}

// MetalScatter is the implementation of scatter for Metal.
static Vector MetalScatter(const void* me,
                           Vector input,
                           Vector normal,
                           unsigned* seed) {
    const Metal* metal = me;
    Vector ui = VecUnit(input);
    Vector un = VecUnit(normal);
    Vector rb = VecRandBall(metal->blur, seed);
    double cast_len = VecDot(ui, un) * 2.;
    Vector casted = VecMulS(un, cast_len);
    // Input ray is blurred for a little bit.
    Vector blur_in = VecAdd(rb, ui);
    return VecSub(blur_in, casted);
}

// MetalAlbedo is the implementation of albedo for Metal.
static Vector MetalAlbedo(const void* me) {
    const Metal* metal = me;
    return metal->albedo;
}

Material MetalAsMaterial(const Metal* metal) {
    return (Material){
        .object = metal,
        .scatter = MetalScatter,
        .albedo = MetalAlbedo,
    };
}

// Schlick approximates the probability a ray is reflected
static double schlick(double cosine, double ratio) {
    double r = (1. - ratio) / (1. + ratio);
    double sq = r * r;
    return sq + (1. - sq) * powf(1. - cosine, 5);
}

// GlassScatter is the implementation of scatter for Glass.
static Vector GlassScatter(const void* gl,
                           Vector input,
                           Vector normal,
                           unsigned* seed) {
    const Glass* glass = gl;
    Vector ui = VecUnit(input);
    Vector un = VecUnit(normal);

    double refractive = glass->refractive;
    double cos = VecDot(ui, un);
    double ratio = (cos < 0) ? 1. / refractive : refractive;

    // sin**2 + cos**2 == 1
    double sin_sq = 1. - cos * cos;
    // cos**2 after refraction according to Snell's law.
    double cos_sq_ref = 1. - ratio * ratio * sin_sq;

    bool will_refract = (cos <= 0) && cos_sq_ref >= 0;

    double random = GENF(seed);
    Vector rand_blur = VecRandBall(glass->blur, seed);

    if (will_refract && random >= schlick(fabs(cos), refractive)) {
        Vector scaled = VecMulS(un, cos);
        Vector first = VecAdd(ui, scaled);

        double cos_ref = sqrt(cos_sq_ref);
        Vector scaled_ref = VecMulS(un, cos_ref);
        Vector second = VecAdd(scaled_ref, rand_blur);

        return VecAdd(VecMulS(first, ratio), second);
    } else {
        double product = VecDot(ui, un);
        Vector casted = VecMulS(un, product * 2.);
        return VecSub(VecAdd(input, rand_blur), casted);
    }
}

// GlassAlbedo is the implementation of albedo for Glass.
static Vector GlassAlbedo(const void* gl) {
    const Glass* glass = gl;
    return glass->albedo;
}

Material GlassAsMaterial(const Glass* glass) {
    return (Material){
        .object = glass,
        .scatter = GlassScatter,
        .albedo = GlassAlbedo,
    };
}
