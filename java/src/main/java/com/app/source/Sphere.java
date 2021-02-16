package com.app.source;

public record Sphere(Vector center, double radius, Material material) implements Hittable {
    public Vector normal(Vector point) {
        return point.sub(center);
    }

    @Override
    public HitData hit(Vector source, Vector towards) {
        Vector oc = normal(source);
        double a = towards.l2();
        double b = oc.dot(towards);
        double c = oc.l2() - radius * radius;

        double base = Math.sqrt(b * b - a * c);
        double neg = (-b - base) / a;
        double pos = (-b + base) / a;

        if (neg > 0) {
            Vector point = source.add(towards.mul(neg));
            assert bounds().through(source, towards);
            return HitData.hit(neg, point, normal(point), material);
        } else if (pos > 0) {
            Vector point = source.add(towards.mul(pos));
            assert bounds().through(source, towards);
            return HitData.hit(pos, point, normal(point), material);
        } else {
            return HitData.miss();
        }
    }

    @Override
    public Box bounds() {
        Vector min = center.sub(radius);
        Vector max = center.add(radius);

        return new Box(new Pair(min.x(), max.x()), new Pair(min.y(), max.y()), new Pair(min.z(), max.z()));
    }
}
