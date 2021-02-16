package com.app.source;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NonNull;

@Data
@AllArgsConstructor
public final class Matte implements Material {
    @NonNull
    private Vector albedo;

    @Override
    public Vector scatter(Vector input, Vector normal) {
        normal = normal.unit();
        return Vector.randomBall(1.).add(normal);
    }
}
