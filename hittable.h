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
struct HitData Hittable_hit(Hittable ht, Vector source, Vector towards);

// Calls bounds for Hittable.
// @param ht Hittable object to use.
// @return The bounding box of the Hittable.
struct Box Hittable_bounds(Hittable ht);

// Creates a null value for hittable.
// @return A Hittable object that's zero initialized, suitable for null value.
Hittable Hittable_null(void);

// Determines if the Hittable is a null value.
// @param ht Hittable object to check.
// @return true if ht is a null value. Else false.
bool Hittable_is_null(Hittable ht);

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
bool HitData_has_hit(const HitData hd);

// Hit indicates a hit somewhere.
// @param t Parameter of the ray at the hit.
// @param point The point the ray intersects with the surface.
// @param normal The direction of the surface at the hit.
// @param mat The material of the surface at the hit.
// @return The record of this hit.
HitData HitData_hit(double t, Vector point, Vector normal, Material mat);

// Miss indicates a miss.
// @return The record of this miss. hasHit(Miss()) is always false.
HitData HitData_miss(void);

// HitList stores a list of hittable linearly.
// @author RenTrueWang
typedef struct HitList {
    // An array of hittable.
    Hittable* list;
    // The length of the array.
    int length;
} HitList;

// Creates a new list of hittables.
// @param length The length of the array.
// @return A new HitList.
HitList HitList_make(int length);

// &hitlist[index]
// @param hl HitList to access.
// @param index The index of the element. Should be in the range [0, length)
// @return The Hittable object at index.
Hittable* HitList_getitem(HitList hl, int index);

// Free the resources controlled by HitList.
// @param hl HitList to free.
// @see free
void HitList_free(HitList* hl);

// Converts a HitList to a Hittable.
// @param hl HitList to convert. hl lives on the heap.
// @return Hittable object that stores a HitList.
Hittable HitList_Hittable(const HitList* hl);

// A binary node that is hittable.
// @author RenTrueWang
typedef struct _HitNode {
    // The object that the node carries. If the node is internal, hittable
    // contains a null value. If the node is leaf, hittable is non-null.
    Hittable hittable;
    // A cache of bounds. This is used to determine whether the ray will hit.
    // The bounds is cached in order to save computation, or else all possible
    // paths will have to be traversed before finding out if it really hits.
    Box bounds;
    // Child nodes index. (left * right) is always positive (either both
    // positive or both negative). If both of them are negative, the node is a
    // leaf. If both of them are positive, the node is internal.
    int left, right;
} _HitNode;

// Creates a leaf _HitNode.
// @param ht Hittable object that will be stored in the node.
// @return A new leaf node.
_HitNode _HitNode_leaf(Hittable ht);

// Creates an internal _HitNode.
// @param bounds Boundary that wraps over its entire sub-tree.
// @param left Index of the left child.
// @param right Index of the right child.
_HitNode _HitNode_inter(Box bounds, int left, int right);

// HitTree stores a list of hittable in the binary tree format.
// @author RenTrueWang
typedef struct HitTree {
    // The list of nodes. The length should be 2*n-1 with n being the number of
    // actual Hittable objects. 2*n-1 because n objects will need n-1 merges to
    // end up all in a set in a union-find algorithm, which is itself a tree.
    // The index of the root is always going to be the last one nodelist[-1],
    // because of how it is generated, where the nodes are merged and pushed.
    _HitNode* nodelist;
    // The length of the array.
    int length;
} HitTree;

// Creates a new tree of hittables from a list of hittables.
// @param hl A list of hittables. The content of the list is fully copied.
// @return A new HitTree.
// @see HitList
HitTree HitTree_make(HitList hl);

// Free the resources controlled by HitTree.
// @param ht HitTree to free.
// @see free
void HitTree_free(HitTree* ht);

// Converts a HitTree to a Hittable.
// @param ht HitTree to convert. ht lives on the heap.
// @return Hittable object that stores a HitTree.
Hittable HitTree_Hittable(const HitTree* ht);
