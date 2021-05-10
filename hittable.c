#include "hittable.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "macro.h"

HitData HittableHit(const Hittable ht, Vector source, Vector towards) {
    return ht.hit(ht.object, source, towards);
}

Box HittableBounds(const Hittable ht) {
    return ht.bounds(ht.object);
}

bool HasHit(const HitData hd) {
    return hd.t != INFINITY;
}

HitData Hit(double t, Vector point, Vector normal, Material mat) {
    return (HitData){t, point, normal, mat};
}

HitData Miss(void) {
    // Other values are default initialized.
    return (HitData){.t = INFINITY};
}

HitList MakeHitList(int length) {
    return (HitList){
        .list = calloc(length, sizeof(HitList)),
        .length = length,
    };
}

Hittable* HitListAt(HitList hl, int index) {
    return hl.list + index;
}

// HitListHit is the implementation of hit for HitList.
static HitData HitListHit(const void* hl, Vector source, Vector towards) {
    const HitList* hitlist = hl;

    towards = VecUnit(towards);
    HitData closest = Miss();

    for (int i = 0; i < hitlist->length; ++i) {
        const Hittable* hittable = HitListAt(*hitlist, i);
        HitData hitdata = HittableHit(*hittable, source, towards);
        if (hitdata.t < closest.t) {
            closest = hitdata;
        }
    }
    return closest;
}

// HitListBounds is the implementation of bounds for HitList.
static Box HitListBounds(const void* hl) {
    const HitList* hitlist = hl;
    int length = hitlist->length;

    if (length) {
        Hittable* list = hitlist->list;
        Box bounds = HittableBounds(list[0]);
        for (int i = 1; i < length; ++i) {
            bounds = BoxWraps(bounds, HittableBounds(list[i]));
        }
        return bounds;
    } else {
        assert(0 && "unreachable");
    }
}

Hittable HitListAsHittable(const HitList* hl) {
    return (Hittable){.object = hl, .hit = HitListHit, .bounds = HitListBounds};
}

HitNode MakeHitNode(Hittable left, Hittable right) {
    assert(left.object != right.object);
    return (HitNode){left, right};
}

HitNode* NewHitNode(Hittable left, Hittable right) {
    HitNode* hn = malloc(sizeof(HitNode));
    *hn = MakeHitNode(left, right);
    return hn;
}

// HitNodeHit is the implementation of hit for HitNode.
static HitData HitNodeHit(const void* hn, Vector source, Vector towards) {
    const HitNode* hitnode = hn;

    const Hittable left = hitnode->left;
    const Hittable right = hitnode->right;

    Box lb = HittableBounds(left);
    Box rb = HittableBounds(right);

    // Are the boxes hit by the ray?
    bool lb_hit = ThroughBox(lb, source, towards);
    bool rb_hit = ThroughBox(rb, source, towards);

    HitData left_hit = lb_hit ? HittableHit(left, source, towards) : Miss();
    HitData right_hit = rb_hit ? HittableHit(left, source, towards) : Miss();

    return (left_hit.t <= right_hit.t) ? left_hit : right_hit;
}

// HitNodeBounds is the implementation of bounds for HitNode.
static Box HitNodeBounds(const void* hn) {
    const HitNode* hitnode = hn;

    Box lb = HittableBounds(hitnode->left);
    Box rb = HittableBounds(hitnode->right);

    return BoxWraps(lb, rb);
}

Hittable HitNodeAsHittable(const HitNode* hn) {
    return (Hittable){
        .object = hn,
        .hit = HitNodeHit,
        .bounds = HitNodeBounds,
    };
}

// Return the center of a hittable.
static Vector HittableCenter(Hittable h) {
    Box bounds = HittableBounds(h);
    return BoxCenter(bounds);
}

// Compare Hittable along the X axis.
static int cmp_x(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return HittableCenter(*ha).x - HittableCenter(*hb).x;
}

// Compare Hittable along the Y axis.
static int cmp_y(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return HittableCenter(*ha).y - HittableCenter(*hb).y;
}

// Compare Hittable along the Z axis.
static int cmp_z(const void* a, const void* b) {
    const Hittable* ha = (Hittable*)a;
    const Hittable* hb = (Hittable*)b;
    return HittableCenter(*ha).z - HittableCenter(*hb).z;
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
            return HitNodeAsHittable(NewHitNode(list[0], list[1]));
        }
        default:
            break;
    }

    Vector avg = VecO();
    for (int i = 0; i < length; ++i) {
        VecIAdd(&avg, HittableCenter(list[i]));
    }
    VecIDivS(&avg, length);

    // Variance along different axises.
    double varx, vary, varz;
    varx = vary = varz = 0.;
    for (int i = 0; i < length; ++i) {
        Vector center = HittableCenter(list[i]);
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

    return HitNodeAsHittable(NewHitNode(left, right));
}

HitTree MakeHitTree(HitList hl) {
    Hittable* list = hl.list;
    int length = hl.length;

    assert(length != 0);

    Hittable root = partition(list, length);

    return (HitTree){root};
}

// HitTreeHit is the implementation of hit for HitTree.
static HitData HitTreeHit(const void* ht, Vector source, Vector towards) {
    const HitTree* hittree = ht;
    return HittableHit(hittree->root, source, towards);
}

// HitTreeBounds is the implementation of bounds for HitTree.
static Box HitTreeBounds(const void* ht) {
    const HitTree* hittree = ht;
    return HittableBounds(hittree->root);
}

Hittable HitTreeAsHittable(const HitTree* ht) {
    return (Hittable){
        .object = ht,
        .hit = HitTreeHit,
        .bounds = HitTreeBounds,
    };
}
