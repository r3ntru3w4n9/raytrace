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

// HitListHit is the implementation of hit for HitList.
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
        .object = hl, .hit = HitList_hit, .bounds = HitList_bounds};
}

HitNode HitNode_make(Hittable left, Hittable right) {
    assert(left.object != right.object);
    return (HitNode){left, right};
}

HitNode* HitNode_new(Hittable left, Hittable right) {
    HitNode* hn = malloc(sizeof(HitNode));
    *hn = HitNode_make(left, right);
    return hn;
}

// HitNodeHit is the implementation of hit for HitNode.
static HitData HitNode_hit(const void* hn, Vector source, Vector towards) {
    const HitNode* hitnode = hn;

    const Hittable left = hitnode->left;
    const Hittable right = hitnode->right;

    Box lb = Hittable_bounds(left);
    Box rb = Hittable_bounds(right);

    // Are the boxes hit by the ray?
    bool lb_hit = Box_is_through(lb, source, towards);
    bool rb_hit = Box_is_through(rb, source, towards);

    HitData left_hit =
        lb_hit ? Hittable_hit(left, source, towards) : HitData_miss();
    HitData right_hit =
        rb_hit ? Hittable_hit(left, source, towards) : HitData_miss();

    return (left_hit.t <= right_hit.t) ? left_hit : right_hit;
}

// HitNodeBounds is the implementation of bounds for HitNode.
static Box HitNode_bounds(const void* hn) {
    const HitNode* hitnode = hn;

    Box lb = Hittable_bounds(hitnode->left);
    Box rb = Hittable_bounds(hitnode->right);

    return Box_wraps(lb, rb);
}

Hittable HitNode_Hittable(const HitNode* hn) {
    return (Hittable){
        .object = hn,
        .hit = HitNode_hit,
        .bounds = HitNode_bounds,
    };
}

// Return the center of a hittable.
static Vector Hittable_center(Hittable h) {
    Box bounds = Hittable_bounds(h);
    return Box_center(bounds);
}

// Compare Hittable along the X axis.
static int cmp_x(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return Hittable_center(*ha).x - Hittable_center(*hb).x;
}

// Compare Hittable along the Y axis.
static int cmp_y(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return Hittable_center(*ha).y - Hittable_center(*hb).y;
}

// Compare Hittable along the Z axis.
static int cmp_z(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return Hittable_center(*ha).z - Hittable_center(*hb).z;
}

// Partition the list according to how far apart each objects are along each
// axis, and convert the list into a tree. List elements will end up in tree's
// leaves, and internal nodes will be freshly allocated.
static Hittable partition(Hittable* list, int length) {
    switch (length) {
        case 0:
            // Because case 1 and case 2 are all handled, case 0 will not
            // happen.
            assert(0 && "unreachable");
        case 1:
            return list[0];
        case 2: {
            return HitNode_Hittable(HitNode_new(list[0], list[1]));
        }
        default:
            break;
    }

    Vector avg = Vec_o();
    for (int i = 0; i < length; ++i) {
        Vec_iadd(&avg, Hittable_center(list[i]));
    }
    Vec_idiv_s(&avg, length);

    // Variance along different axises.
    double varx, vary, varz;
    varx = vary = varz = 0.;
    for (int i = 0; i < length; ++i) {
        Vector center = Hittable_center(list[i]);
        varx += fabs(center.x - avg.x);
        vary += fabs(center.y - avg.y);
        varz += fabs(center.z - avg.z);
    }

    if (varx >= vary && varx >= varz) {
        qsort(list, length, sizeof(Hittable), cmp_x);
    }

    if (vary >= varx && vary >= varz) {
        qsort(list, length, sizeof(Hittable), cmp_y);
    }

    if (varz >= varx && varz >= vary) {
        qsort(list, length, sizeof(Hittable), cmp_z);
    }

    int half = length / 2;

    Hittable left = partition(list, half);
    Hittable right = partition(list + half, length - half);

    return HitNode_Hittable(HitNode_new(left, right));
}

HitTree HitTree_make(HitList hl) {
    Hittable* list = hl.list;
    int length = hl.length;

    assert(length != 0);

    Hittable root = partition(list, length);

    return (HitTree){root};
}

// HitTreeHit is the implementation of hit for HitTree.
static HitData HitTree_hit(const void* ht, Vector source, Vector towards) {
    const HitTree* hittree = ht;
    return Hittable_hit(hittree->root, source, towards);
}

// HitTreeBounds is the implementation of bounds for HitTree.
static Box HitTree_bounds(const void* ht) {
    const HitTree* hittree = ht;
    return Hittable_bounds(hittree->root);
}

Hittable HitTree_Hittable(const HitTree* ht) {
    return (Hittable){
        .object = ht,
        .hit = HitTree_hit,
        .bounds = HitTree_bounds,
    };
}
