#pragma once

#include "geometric.h"

// Material is an interface that stores a certain kind of material.
// @author RenTrueWang
typedef struct Material {
    // Interface object pointer.
    const void* object;
    // How the output ray is scattered from the surface.
    // @param object The interface object.
    // @param input The direction of the input ray.
    // @param normal The direction that the surface is facing.
    // @return The scattered direction of the ray.
    Vector (*scatter)(const void* object,
                      Vector input,
                      Vector normal,
                      unsigned* seed);
    // The color that the surface reflects.
    // @param object The interface object.
    // @return The color of the surface.
    Vector (*albedo)(const void* object);
} Material;

// Calls scatter for Material.
// @param mat The material in use.
// @param input The direction of the incoming ray.
// @param normal The direction of the surface where the ray hits.
// @return The direction of the reflected ray.
Vector Mat_scatter(Material mat, Vector input, Vector normal, unsigned* seed);

// Calls albedo for Material.
// @param mat The material in use.
// @return The albedo. Albedo_i is in the range [0, 1]
Vector Mat_albedo(Material mat);

// Matte is a Lambertian material.
typedef struct Matte {
    // The albedo of a matte material.
    Vector albedo;
} Matte;

// Convert Matte to Material.
// @param matte Matte to convert.
// @return The material object that holds matte.
Material Matte_Mat(const Matte* matte);

// Metal is like a mirror.
typedef struct Metal {
    // The albedo of a vector material.
    Vector albedo;
    // The blurring radius of the material.
    double blur;
} Metal;

// Convert Metal to Material.
// @param metal Metal to convert.
// @return The material object that holds metal.
Material Metal_Mat(const Metal* metal);

// Glass not only reflects but also refracts.
typedef struct Glass {
    // The albedo of a glass material.
    Vector albedo;
    // The blurring radius of the material.
    double blur;
    // The refractive index for the material.
    double refractive;
} Glass;

// Convert Glass to Material.
// @param glass Glass to convert.
// @return The material object that holds glass.
Material Glass_Mat(const Glass* glass);
