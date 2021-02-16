package com.app.source;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NonNull;

@Data
@AllArgsConstructor
public final class Metal implements Material {
    @NonNull
    private Vector albedo;
    private double blur;

    @Override
    public Vector scatter(Vector input, Vector normal) {
        input = input.unit();
        normal = normal.unit();
        var random = Vector.randomBall(blur);
        var casted = normal.mul(input.dot(normal) * 2.);
        return random.add(input).sub(casted);
    }
}
