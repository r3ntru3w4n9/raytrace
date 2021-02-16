package com.app.source;

import lombok.Data;
import lombok.NonNull;

@Data
public final class Box {
    @NonNull
    Pair x, y, z;

    Box wrap(Box other) {
        return new Box(Box.largerBound(this.x, other.x), Box.largerBound(this.y, other.y),
                Box.largerBound(this.z, other.z));
    }

    public Vector center() {
        return new Vector((this.x.getX() + this.x.getY()) / 2., (this.y.getX() + this.y.getY()) / 2.,
                (this.z.getX() + this.z.getY()) / 2.);
    }

    public Vector min() {
        return new Vector(x.getX(), y.getX(), z.getX());
    }

    public Vector max() {
        return new Vector(x.getY(), y.getY(), z.getY());
    }

    public boolean through(Vector source, Vector towards) {
        Vector minimum = min();
        Vector maximum = max();

        double tMin = Double.NEGATIVE_INFINITY;
        double tMax = Double.POSITIVE_INFINITY;

        for (int i = 0; i < 3; ++i) {
            double invB = 1. / towards.get(i);
            double tSmall = (minimum.get(i) - source.get(i)) * invB;
            double tLarge = (maximum.get(i) - source.get(i)) * invB;

            if (invB < 0) {
                double temp = tSmall;
                tSmall = tLarge;
                tLarge = temp;
            }

            assert tSmall <= tLarge;

            tMin = tSmall > tMin ? tSmall : tMin;
            tMax = tLarge < tMax ? tLarge : tMax;
        }

        return tMin < tMax;
    }

    static Pair largerBound(Pair a, Pair b) {
        return new Pair(a.getX() < b.getX() ? a.getX() : b.getX(), a.getY() > b.getY() ? a.getY() : b.getY());
    }

    public static Box wraps(Box a, Box b) {
        return a.wrap(b);
    }
}
