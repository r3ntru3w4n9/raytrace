#include "scene.h"

#include <assert.h>
#include <stdlib.h>

#include "macro.h"
#include "material.h"

// SceneHit is the implementation of hit for Scene.
static HitData Scn_hit(const void* sc, Vector source, Vector towards) {
    const Scene* scene = sc;
    return Hittable_hit(scene->hittable, source, towards);
}

// SceneBounds is the implementation of bounds for Scene.
static Box Scn_bounds(const void* sc) {
    const Scene* scene = sc;
    return Hittable_bounds(scene->hittable);
}

Hittable Scn_Hittable(const Scene* scene) {
    return (Hittable){.object = scene, .hit = Scn_hit, .bounds = Scn_bounds};
}

Vector Scn_trace(Scene scene, Vector source, Vector towards, unsigned* seed) {
    Vector color = Vec_from(1.);
    Hittable sh = Scn_Hittable(&scene);

    for (int d = 0; d < scene.cfg.depth; ++d) {
        HitData hd = Hittable_hit(sh, source, towards);

        if (HitData_has_hit(hd)) {
            Material mat = hd.mat;
            Vector reflected = Mat_scatter(mat, towards, hd.normal, seed);
            Vec_imul(&color, Mat_albedo(mat));
            source = hd.point;
            towards = reflected;
        } else {
            double t = .5 * Vec_unit(towards).y + 1.;
            Vector bg = Vec_add(Vec_from(1. - t), (Vector){.5 * t, .7 * t, t});
            return Vec_mul(color, bg);
        }
    }
    return Vec_o();
}

Pixel Scn_color(Scene scene, int x, int y, unsigned* seed) {
    assert(x >= 0);
    assert(x < scene.cfg.width);
    assert(y >= 0);
    assert(y < scene.cfg.height);

    Pair aij = Pair_rand_disk(scene.cam.aperture, seed);
    double ai = aij.x;
    double aj = aij.y;

    Vector h = Vec_mul_s(Vec_unit(scene.cam.horiz), ai);
    Vector v = Vec_mul_s(Vec_unit(scene.cam.vertic), aj);
    Vector start = Vec_add(scene.cam.source, Vec_add(h, v));

    Vector color = Vec_o();
    for (int s = 0; s < scene.cfg.samples; ++s) {
        double i = (double)(x + genfloat(seed)) / scene.cfg.width;
        double j = (double)(y + genfloat(seed)) / scene.cfg.height;

        // Difference between the endpoint of the vector and the corner
        Vector h = Vec_mul_s(scene.cam.horiz, i);
        Vector v = Vec_mul_s(scene.cam.vertic, j);
        Vector fc = Vec_add(h, v);

        Vector end = Vec_add(scene.cam.corner, fc);
        Vector towards = Vec_sub(end, start);

        // Color on this sample.
        Vector sc = Scn_trace(scene, start, towards, seed);

        Vec_iadd(&color, sc);
    }
    Vec_idiv_s(&color, scene.cfg.samples);
    return Vec_2Px(color);
}
