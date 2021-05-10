#include "material.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "macro.h"

Vector Mat_scatter(Material mat, Vector input, Vector normal, unsigned* seed) {
    return mat.scatter(mat.object, input, normal, seed);
}

Vector Mat_albedo(Material mat) {
    return mat.albedo(mat.object);
}

// MatteScatter is the implementation of scatter for Matte.
static Vector Matte_scatter(const void* ma,
                            Vector input,
                            Vector normal,
                            unsigned* seed) {
    // Matte reflects perfectly so doesn't care about the input.
    (void)ma;
    (void)input;

    Vector un = Vec_unit(normal);
    Vector rb = Vec_rand_ball(1., seed);
    return Vec_add(rb, un);
}

// MatteAlbedo is the implementation of albedo for Matte.
static Vector Matte_albedo(const void* ma) {
    const Matte* matte = ma;
    return matte->albedo;
}

Material Matte_Mat(const Matte* matte) {
    return (Material){
        .object = matte,
        .scatter = Matte_scatter,
        .albedo = Matte_albedo,
    };
}

// MetalScatter is the implementation of scatter for Metal.
static Vector Metal_scatter(const void* me,
                            Vector input,
                            Vector normal,
                            unsigned* seed) {
    const Metal* metal = me;
    Vector ui = Vec_unit(input);
    Vector un = Vec_unit(normal);
    Vector rb = Vec_rand_ball(metal->blur, seed);
    double cast_len = Vec_dot(ui, un) * 2.;
    Vector casted = Vec_mul_s(un, cast_len);
    // Input ray is blurred for a little bit.
    Vector blur_in = Vec_add(rb, ui);
    return Vec_sub(blur_in, casted);
}

// MetalAlbedo is the implementation of albedo for Metal.
static Vector Metal_albedo(const void* me) {
    const Metal* metal = me;
    return metal->albedo;
}

Material Metal_Mat(const Metal* metal) {
    return (Material){
        .object = metal,
        .scatter = Metal_scatter,
        .albedo = Metal_albedo,
    };
}

// Schlick approximates the probability a ray is reflected
static double schlick(double cosine, double ratio) {
    double r = (1. - ratio) / (1. + ratio);
    double sq = r * r;
    return sq + (1. - sq) * powf(1. - cosine, 5);
}

// GlassScatter is the implementation of scatter for Glass.
static Vector Glass_scatter(const void* gl,
                            Vector input,
                            Vector normal,
                            unsigned* seed) {
    const Glass* glass = gl;
    Vector ui = Vec_unit(input);
    Vector un = Vec_unit(normal);

    double refractive = glass->refractive;
    double cos = Vec_dot(ui, un);
    double ratio = (cos < 0) ? 1. / refractive : refractive;

    // sin**2 + cos**2 == 1
    double sin_sq = 1. - cos * cos;
    // cos**2 after refraction according to Snell's law.
    double cos_sq_ref = 1. - ratio * ratio * sin_sq;

    bool will_refract = (cos <= 0) && cos_sq_ref >= 0;

    double random = genfloat(seed);
    Vector rand_blur = Vec_rand_ball(glass->blur, seed);

    if (will_refract && random >= schlick(fabs(cos), refractive)) {
        Vector scaled = Vec_mul_s(un, cos);
        Vector first = Vec_add(ui, scaled);

        double cos_ref = sqrt(cos_sq_ref);
        Vector scaled_ref = Vec_mul_s(un, cos_ref);
        Vector second = Vec_add(scaled_ref, rand_blur);

        return Vec_add(Vec_mul_s(first, ratio), second);
    } else {
        double product = Vec_dot(ui, un);
        Vector casted = Vec_mul_s(un, product * 2.);
        return Vec_sub(Vec_add(input, rand_blur), casted);
    }
}

// GlassAlbedo is the implementation of albedo for Glass.
static Vector Glass_albedo(const void* gl) {
    const Glass* glass = gl;
    return glass->albedo;
}

Material Glass_Mat(const Glass* glass) {
    return (Material){
        .object = glass,
        .scatter = Glass_scatter,
        .albedo = Glass_albedo,
    };
}
