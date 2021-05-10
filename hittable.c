#include "hittable.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "macro.h"

HitData Hittable_hit(const Hittable ht, Vector source, Vector towards) {
    return ht.hit(ht.object, source, towards);
}

Box Hittable_bounds(const Hittable ht) {
    return ht.bounds(ht.object);
}

Hittable Hittable_null(void) {
    return (Hittable){
        .object = NULL,
        .hit = NULL,
        .bounds = NULL,
    };
}

bool Hittable_is_null(Hittable ht) {
    // If object is null, don't bother with other attributes.
    return !ht.object;
}

bool HitData_has_hit(const HitData hd) {
    return hd.t != INFINITY;
}

HitData HitData_hit(double t, Vector point, Vector normal, Material mat) {
    return (HitData){t, point, normal, mat};
}

HitData HitData_miss(void) {
    // Other values are default initialized.
    return (HitData){.t = INFINITY};
}

HitList HitList_make(int length) {
    return (HitList){
        .list = calloc(length, sizeof(HitList)),
        .length = length,
    };
}

Hittable* HitList_getitem(HitList hl, int index) {
    return hl.list + index;
}

void HitList_free(HitList* hl) {
    free(hl->list);
    hl->list = NULL;
}

// HitListHit is the implementation of hit for HitList.
// @see Hittable
static HitData HitList_hit(const void* hl, Vector source, Vector towards) {
    const HitList* hitlist = hl;

    towards = Vec_unit(towards);

    HitData closest = HitData_miss();
    for (int i = 0; i < hitlist->length; ++i) {
        const Hittable* hittable = HitList_getitem(*hitlist, i);
        HitData hitdata = Hittable_hit(*hittable, source, towards);
        if (hitdata.t < closest.t) {
            closest = hitdata;
        }
    }
    return closest;
}

// HitListBounds is the implementation of bounds for HitList.
// @see Hittable
static Box HitList_bounds(const void* hl) {
    const HitList* hitlist = hl;
    int length = hitlist->length;

    if (length) {
        Hittable* list = hitlist->list;
        Box bounds = Hittable_bounds(list[0]);
        for (int i = 1; i < length; ++i) {
            bounds = Box_wraps(bounds, Hittable_bounds(list[i]));
        }
        return bounds;
    } else {
        assert(0 && "unreachable");
    }
}

Hittable HitList_Hittable(const HitList* hl) {
    return (Hittable){
        .object = hl,
        .hit = HitList_hit,
        .bounds = HitList_bounds,
    };
}

_HitNode _HitNode_leaf(Hittable ht) {
    return (_HitNode){
        .hittable = ht,
        .bounds = Hittable_bounds(ht),
        .left = -1,
        .right = -1,
    };
}

_HitNode _HitNode_inter(Box bounds, int left, int right) {
    return (_HitNode){
        .hittable = Hittable_null(),
        .bounds = bounds,
        .left = left,
        .right = right,
    };
}

// Return the center of a hittable.
// @param h Hittable to use.
// @return The center of the Hittable h.
static Vector Hittable_center(Hittable h) {
    Box bounds = Hittable_bounds(h);
    return Box_center(bounds);
}

// Compare Hittable along the X axis.
// @param a The first Hittable to compare.
// @param b The second Hittable to compare.
// @return a.center.x - b.center.x
static int cmp_x(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return Hittable_center(*ha).x - Hittable_center(*hb).x;
}

// Compare Hittable along the Y axis.
// @param a The first Hittable to compare.
// @param b The second Hittable to compare.
// @return a.center.y - b.center.y
static int cmp_y(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return Hittable_center(*ha).y - Hittable_center(*hb).y;
}

// Compare Hittable along the Z axis.
// @param a The first Hittable to compare.
// @param b The second Hittable to compare.
// @return a.center.z - b.center.z
static int cmp_z(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return Hittable_center(*ha).z - Hittable_center(*hb).z;
}

// Variance along a dimension. The definition of variance is the sum of absolute
// difference between individual entries and the mean. This does just that.
// @param hl HitList to calculate variance on.
// @param len Length of the HitList
// @param dim Dimension to search. Only 0, 1, 2 are allowed. They correspond to
// x, y, z dimensions. If any other value is passed-in, the function breaks.
// @return The variance along the given axis.
static double var_dim(Hittable* hl, int len, int dim) {
    switch (dim) {
        case 0:
        case 1:
        case 2:
            break;
        default:
            // Only {0, 1, 2} are legal values for dim.
            assert(0 && "unreachable");
    }

    double avg = 0;
    for (int i = 0; i < len; ++i) {
        Vector center = Hittable_center(hl[i]);
        avg += Vec_dim(center, dim);
    }
    avg /= len;

    double var = 0;
    for (int i = 0; i < len; ++i) {
        Vector center = Hittable_center(hl[i]);
        var += fabs(Vec_dim(center, dim) - avg);
    }
    return var;
}

// Sort the Hittable array according to the dimension with maximum variance.
// @param hl The Hittable array.
// @param len Length of the Hittable array.
static void sort_max_var(Hittable* hl, int len) {
    double varx = var_dim(hl, len, 0);
    double vary = var_dim(hl, len, 1);
    double varz = var_dim(hl, len, 2);

    // Only one of the following three if's will be called. It does not matter
    // which, what's important is to separate the list into far apart groups.

    if (varx >= vary && varx >= varz) {
        qsort(hl, len, sizeof(Hittable), cmp_x);
    }

    if (vary >= varx && vary >= varz) {
        qsort(hl, len, sizeof(Hittable), cmp_y);
    }

    if (varz >= varx && varz >= vary) {
        qsort(hl, len, sizeof(Hittable), cmp_z);
    }
}

// Partition the list according to how far apart each objects are along each
// axis, and convert the list into a tree. List elements will end up in tree's
// leaves, and internal nodes will be freshly allocated. This is not
// thread-safe. Every call to this function stores a new element in the
// nodelist. Every call to this function visits an increasingly narrow region in
// the original Hittable list hl, eventually down to length 1.
// @param hl HitList's pointer. The original elements.
// @param hl_len Length of HitList.
// @param nl NodeList's pointer. The new elements.
// @param nl_idx Current index of NodeList to modify.
// @return Index of node stored by the function.
static int partition(Hittable* hl, int hl_len, _HitNode* nl, int* nl_idx) {
    if (hl_len == 0) {
        // Because case 1 and case 2 are all handled, case 0 will not happen.
        assert(0 && "unreachable");
    } else if (hl_len == 1) {
        // Every call eventually falls into this case since case 2 directly
        // calls this 2 times. So every element is going to be visited.
        int mod = (*nl_idx)++;

        // Makes copy to hl[0]
        Hittable object = hl[0];

        nl[mod] = _HitNode_leaf(object);
        return mod;
    }

    // This basically performs clustering. (Sort and group by location.)

    sort_max_var(hl, hl_len);

    int half = hl_len / 2;
    int left = partition(hl, half, nl, nl_idx);
    int right = partition(hl + half, hl_len - half, nl, nl_idx);

    // Now nl[left] is the left node, and nl[right] is the right node.
    // mod is the index to modify.
    int mod = (*nl_idx)++;
    assert(left < mod);
    assert(right < mod);

    // Boundary wraps its entire sub-trees.
    Box bounds = Box_wraps(nl[left].bounds, nl[right].bounds);
    nl[mod] = _HitNode_inter(bounds, left, right);
    return mod;
}

HitTree HitTree_make(HitList hl) {
    Hittable* list = hl.list;
    int length = 2 * hl.length - 1;
    assert(length > 0);

    _HitNode* nodelist = calloc(length, sizeof(_HitNode));

    int cidx = 0;
    partition(list, hl.length, nodelist, &cidx);
    assert(cidx == length);

    return (HitTree){.nodelist = nodelist, .length = length};
}

void HitTree_free(HitTree* ht) {
    free(ht->nodelist);
    ht->nodelist = NULL;
}

// Performs hit on a nodelist representing a tree.
// @param nodelist The nodelist that is actually a tree.
// @param index The root index of the current sub-tree.
// @param source The source of the ray.
// @param towards The direction the ray is moving towards.
// @return The record of this hit.
static HitData nl_hit(const _HitNode* nodelist,
                      int index,
                      Vector source,
                      Vector towards) {
    _HitNode root = nodelist[index];
    // The ray passes through the object only if it passes through the box.
    if (Box_is_through(root.bounds, source, towards)) {
        Hittable ht = root.hittable;
        if (Hittable_is_null(ht)) {
            // The node is internal.
            HitData lh = nl_hit(nodelist, root.left, source, towards);
            HitData rh = nl_hit(nodelist, root.right, source, towards);

            // Return the closer one.
            // If either or both lh.t and rh.t are infinity,
            // this function still works.
            return (lh.t <= rh.t) ? lh : rh;
        } else {
            // The node is a leaf.
            return Hittable_hit(ht, source, towards);
        }
    } else {
        return HitData_miss();
    }
}

// HitTreeHit is the implementation of hit for HitTree.
// @see Hittable
static HitData HitTree_hit(const void* ht, Vector source, Vector towards) {
    const HitTree* hittree = ht;
    int root_idx = hittree->length - 1;
    return nl_hit(hittree->nodelist, root_idx, source, towards);
}

// HitTreeBounds is the implementation of bounds for HitTree.
// @see Hittable
static Box HitTree_bounds(const void* ht) {
    const HitTree* hittree = ht;
    int root_idx = hittree->length - 1;
    return hittree->nodelist[root_idx].bounds;
}

Hittable HitTree_Hittable(const HitTree* ht) {
    return (Hittable){
        .object = ht,
        .hit = HitTree_hit,
        .bounds = HitTree_bounds,
    };
}
