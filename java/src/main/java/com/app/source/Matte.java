package com.app.source;

public record Matte(Vector albedo) implements Material {
    @Override
    public Vector scatter(Vector input, Vector normal) {
        normal = normal.unit();
        return Vector.randomBall(1.).add(normal);
    }
}
