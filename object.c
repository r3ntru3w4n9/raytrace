#include "object.h"

#include <assert.h>
#include <math.h>

Sphere MakeSphere(Vector center, double radius, Material mat) {
    assert(radius >= 0);
    return (Sphere){center, radius, mat};
}

Vector SphereNormal(Sphere sphere, Vector point) {
    return VecSub(point, sphere.center);
}

// SphereHit is the implementation of hit for Sphere.
static HitData SphereHit(const void* sp, Vector source, Vector towards) {
    const Sphere* sphere = sp;
    double radius = sphere->radius;

    // TODO: explain what this part is doing.

    Vector oc = SphereNormal(*sphere, source);
    double a = VecL2(towards);
    double b = VecDot(oc, towards);
    double c = VecL2(oc) - radius * radius;

    double base = sqrt(b * b - a * c);
    double root1 = (-b - base) / a;
    double root2 = (-b + base) / a;

    if (root1 <= 0 && root2 <= 0) {
        return Miss();
    }

    double root = (root1 > 0) ? root1 : root2;

    assert(root > 0);

    Vector point = VecAdd(source, VecMulS(towards, root));
    return Hit(root, point, SphereNormal(*sphere, point), sphere->mat);
}

// SphereBounds is the implementation of bounds for Sphere.
static Box SphereBounds(const void* sp) {
    const Sphere* sphere = sp;
    Vector c = sphere->center;
    double r = sphere->radius;
    return (Box){
        .x = (Pair){c.x - r, c.x + r},
        .y = (Pair){c.y - r, c.y + r},
        .z = (Pair){c.z - r, c.z + r},
    };
}

Hittable SphereAsHittable(const Sphere* sphere) {
    return (Hittable){
        .object = sphere,
        .hit = SphereHit,
        .bounds = SphereBounds,
    };
}
