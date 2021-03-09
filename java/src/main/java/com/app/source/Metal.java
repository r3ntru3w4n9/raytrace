package com.app.source;

public record Metal(Vector albedo, double blur) implements Material {
    @Override
    public Vector scatter(Vector input, Vector normal) {
        input  = input.unit();
        normal = normal.unit();
        var random = Vector.randomBall(blur);
        var casted = normal.mul(input.dot(normal) * 2.);

        return random.add(input).sub(casted);
    }
}
