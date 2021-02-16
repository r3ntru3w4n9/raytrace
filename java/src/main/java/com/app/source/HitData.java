package com.app.source;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public final class HitData {
    private double t;
    private Vector point, normal;
    private Material material;

    public HitData(double t) {
        this(t, null, null, null);
    }

    public static HitData hit(double t, Vector point, Vector normal, Material matter) {
        return new HitData(t, point, normal, matter);
    }

    public static HitData miss() {
        return new HitData(Double.POSITIVE_INFINITY);
    }

    public boolean isHit() {
        return t != Double.POSITIVE_INFINITY;
    }
}
