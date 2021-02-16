package com.app.source;

public record HitData(double t, Vector point, Vector normal, Material material) {
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
