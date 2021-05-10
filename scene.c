#include "scene.h"

#include <assert.h>
#include <stdlib.h>

#include "macro.h"
#include "material.h"

// SceneHit is the implementation of hit for Scene.
static HitData SceneHit(const void* sc, Vector source, Vector towards) {
    const Scene* scene = sc;
    return HittableHit(scene->hittable, source, towards);
}

// SceneBounds is the implementation of bounds for Scene.
static Box SceneBounds(const void* sc) {
    const Scene* scene = sc;
    return HittableBounds(scene->hittable);
}

Hittable SceneAsHittable(const Scene* scene) {
    return (Hittable){.object = scene, .hit = SceneHit, .bounds = SceneBounds};
}

Vector SceneColorTrace(Scene scene,
                       Vector source,
                       Vector towards,
                       unsigned* seed) {
    Vector color = VecUniform(1.);
    Hittable sh = SceneAsHittable(&scene);

    for (int d = 0; d < scene.cfg.depth; ++d) {
        HitData hd = HittableHit(sh, source, towards);

        if (HasHit(hd)) {
            Material mat = hd.mat;
            Vector reflected = MatScatter(mat, towards, hd.normal, seed);
            VecIMul(&color, MatAlbedo(mat));
            source = hd.point;
            towards = reflected;
        } else {
            double t = .5 * VecUnit(towards).y + 1.;
            Vector bg = VecAdd(VecUniform(1. - t), (Vector){.5 * t, .7 * t, t});
            return VecMul(color, bg);
        }
    }
    return VecO();
}

Pixel SceneColor(Scene scene, int x, int y, unsigned* seed) {
    assert(x >= 0);
    assert(x < scene.cfg.width);
    assert(y >= 0);
    assert(y < scene.cfg.height);

    Pair aij = PairRandDisk(scene.cam.aperture, seed);
    double ai = aij.x;
    double aj = aij.y;

    Vector h = VecMulS(VecUnit(scene.cam.horiz), ai);
    Vector v = VecMulS(VecUnit(scene.cam.vertic), aj);
    Vector start = VecAdd(scene.cam.source, VecAdd(h, v));

    Vector color = VecO();
    for (int s = 0; s < scene.cfg.samples; ++s) {
        double i = (double)(x + GENF(seed)) / scene.cfg.width;
        double j = (double)(y + GENF(seed)) / scene.cfg.height;

        // Difference between the endpoint of the vector and the corner
        Vector h = VecMulS(scene.cam.horiz, i);
        Vector v = VecMulS(scene.cam.vertic, j);
        Vector fc = VecAdd(h, v);

        Vector end = VecAdd(scene.cam.corner, fc);
        Vector towards = VecSub(end, start);

        // Color on this sample.
        Vector sc = SceneColorTrace(scene, start, towards, seed);

        VecIAdd(&color, sc);
    }
    VecIDivS(&color, scene.cfg.samples);
    return Vec2Px(color);
}
