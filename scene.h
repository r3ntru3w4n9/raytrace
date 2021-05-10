#pragma once

#include "geometric.h"
#include "hittable.h"

// Image's properties for the scenes.
// @author RenTrueWang
typedef struct ImgProp {
    // Number of samples per pixel.
    int samples;
    // Number of bounces to trace.
    int depth;
    // The width of the image.
    int width;
    // The height of the image.
    int height;
} ImgProp;

// Camera stores the source, corner, horizontal and vertical directions.
// @author RenTrueWang
typedef struct Camera {
    // The location of the film.
    Vector source;
    // The viewport's bottom-left corner.
    Vector corner;
    // Camera's right direction.
    Vector horiz;
    // Camera's up direction.
    Vector vertic;
    // The size of the aperture.
    double aperture;
} Camera;

// A scene is a shot taken by a camera.
// @author RenTrueWang
typedef struct Scene {
    // The configuration for the scene
    struct ImgProp cfg;
    // The camera for the scene.
    struct Camera cam;
    // Something in the scene to hit.
    Hittable hittable;
} Scene;

// Convert a Scene to a Hittable.
// @param scene Scene to convert.
// Either scene lives on the heap or has static lifetime.
// @return The Hittable object that stores a Scene
Hittable Scn_Hittable(const Scene* scene);

// Tracks the color of a path.
// @param scene The scene to track.
// @param source The source of the ray.
// @param towards The direction of the ray.
// @param depth The remaining depth (bounces) to track.
// @return The resulting color from the reflections
Vector Scn_trace(Scene scene, Vector source, Vector towards, unsigned* seed);

// Determine the pixel color given the scene and the pixel location.
// @param scene The scene to use.
// @param x The X position of the pixel. x is smaller than the width.
// @param y The Y position of the pixel. y is smaller than the height.
// @return The pixel calculated.
Pixel Scn_color(Scene scene, int x, int y, unsigned* seed);
