#include "object.h"

#include <assert.h>
#include <math.h>

Sphere Sph_make(Vector center, double radius, Material mat) {
    assert(radius >= 0);
    return (Sphere){center, radius, mat};
}

Vector Sph_normal(Sphere sphere, Vector point) {
    return Vec_sub(point, sphere.center);
}

// SphereHit is the implementation of hit for Sphere.
// @see Hittable
static HitData Sph_hit(const void* sp, Vector source, Vector towards) {
    const Sphere* sphere = sp;
    double radius = sphere->radius;

    // TODO: explain what this part is doing.

    Vector oc = Sph_normal(*sphere, source);
    double a = Vec_l2(towards);
    double b = Vec_dot(oc, towards);
    double c = Vec_l2(oc) - radius * radius;

    double base = sqrt(b * b - a * c);
    double root1 = (-b - base) / a;
    double root2 = (-b + base) / a;

    if (root1 <= 0 && root2 <= 0) {
        return HitData_miss();
    }

    double root = (root1 > 0) ? root1 : root2;

    assert(root > 0);

    Vector point = Vec_add(source, Vec_mul_s(towards, root));
    return HitData_hit(root, point, Sph_normal(*sphere, point), sphere->mat);
}

// SphereBounds is the implementation of bounds for Sphere.
// @see Hittable
static Box Sph_bounds(const void* sp) {
    const Sphere* sphere = sp;
    Vector c = sphere->center;
    double r = sphere->radius;
    return (Box){
        .x = (Pair){c.x - r, c.x + r},
        .y = (Pair){c.y - r, c.y + r},
        .z = (Pair){c.z - r, c.z + r},
    };
}

Hittable Sph_Hittable(const Sphere* sphere) {
    return (Hittable){
        .object = sphere,
        .hit = Sph_hit,
        .bounds = Sph_bounds,
    };
}
