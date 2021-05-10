#pragma once

#include <stdbool.h>

#include "geometric.h"
#include "material.h"

// Forward declaration
struct HitData;

// Hittable is an interface representing everything you can hit.
// @author RenTrueWang
typedef struct Hittable {
    // The interface object.
    const void* object;
    // How the ray hits the object.
    // @param object The interface object.
    // @param source The source of the ray.
    // @param towards The direction the ray is moving towards.
    // @return The record of this hit.
    struct HitData (*hit)(const void* object, Vector source, Vector towards);
    // The bounds of the object.
    // @param object The interface object.
    // @return The region that the object occupies.
    Box (*bounds)(const void* object);
} Hittable;

// Calls hit for Hittable.
// @param ht Hittable object to use.
// @param source The source of the ray.
// @param towards The direction the ray is moving towards.
// @return The record of this hit.
struct HitData HittableHit(Hittable ht, Vector source, Vector towards);

// Calls bounds for Hittable.
// @param ht Hittable object to use.
// @return The bounding box of the Hittable.
struct Box HittableBounds(Hittable ht);

// HitData stores information about the hit.
// @author RenTrueWang
typedef struct HitData {
    // The parameter of the hit.
    double t;
    // The location where the ray hits.
    Vector point;
    // The direction of the surface where the ray hits.
    Vector normal;
    // The material of the surface of the hit.
    struct Material mat;
} HitData;

// Whether the hit data indicates a hit.
// @param hd HitData that is going to check.
// @return If the HitData means a hit has happened.
bool HasHit(const HitData hd);

// Hit indicates a hit somewhere.
// @param t Parameter of the ray at the hit.
// @param point The point the ray intersects with the surface.
// @param normal The direction of the surface at the hit.
// @param mat The material of the surface at the hit.
// @return The record of this hit.
HitData Hit(double t, Vector point, Vector normal, Material mat);

// Miss indicates a miss.
// @return The record of this miss. hasHit(Miss()) is always false.
HitData Miss(void);

// HitList stores a list of hittable linearly.
// @author RenTrueWang
typedef struct HitList {
    // An array of hittable.
    Hittable* list;
    // The length of the array.
    int length;
} HitList;

// Create a new list of hittables.
// @param length The length of the array.
// @return A new HitList.
HitList MakeHitList(int length);

// &hitlist[index]
// @param hl HitList to access.
// @param index The index of the element. Should be in the range [0, length)
// @return The Hittable object at index.
Hittable* HitListAt(HitList hl, int index);

// Convert a HitList to a Hittable.
// @param hl HitList to convert. hl lives on the heap.
// @return Hittable object that stores a HitList.
Hittable HitListAsHittable(const HitList* hl);

// A binary node that is hittable.
// @author RenTrueWang
typedef struct HitNode {
    // Child nodes.
    Hittable left, right;
} HitNode;

// Create a new HitNode. Note that left != right.
// @param left The left child node.
// @param right The right child node.
// @return A new HitNode.
HitNode MakeHitNode(Hittable left, Hittable right);

// Allocate a new HitNode. Note that left != right.
// @param left The left child node.
// @param right The right child node.
// @return A new allocated HitNode.
HitNode* NewHitNode(Hittable left, Hittable right);

// Convert a HitNode to a Hittable.
// @param hn HitNode to convert. hn lives on the heap.
// @return Hittable object that stores a HitNode.
Hittable HitNodeAsHittable(const HitNode* hn);

// HitTree stores a list of hittable in the binary tree format.
// @author RenTrueWang
typedef struct HitTree {
    // The root node of the array. The root is Hittable instead of a HitNode
    // because it allows multiple types as long as it implements Hittable.
    Hittable root;
} HitTree;

// Create a new tree of hittables from a list of hittables.
// @param hl A list of hittables.
// @return A new HitTree.
// @see HitList
HitTree MakeHitTree(HitList hl);

// Convert a HitTree to a Hittable.
// @param ht HitTree to convert. ht lives on the heap.
// @return Hittable object that stores a HitTree.
Hittable HitTreeAsHittable(const HitTree* ht);
