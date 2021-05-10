#pragma once

#include "geometric.h"
#include "hittable.h"
#include "material.h"

// Sphere is a 3D ball.
// @author RenTrueWang
typedef struct Sphere {
    // Center of the sphere.
    Vector center;
    // The radius of the sphere.
    double radius;
    // The material that the sphere is made of.
    Material mat;
} Sphere;

// Creates a new sphere.
// @param center The center of the sphere.
// @param radius The radius of the sphere. radius >= 0
// @param mat The material of the sphere.
// @return A new sphere created from components.
Sphere Sph_make(Vector center, double radius, Material mat);

// Normal vector at a certain point.
// @param sphere The sphere whose surface normal vector we're interested in.
// @param point The point for which we calculate the normal vector.
// @return The normal vector at the point, with respect to sphere.
Vector Sph_normal(Sphere sphere, Vector point);

// Converts Sphere to Hittable.
// @param sphere The sphere to convert.
// @return The Hittable object that holds a sphere.
Hittable Sph_Hittable(const Sphere* sphere);
