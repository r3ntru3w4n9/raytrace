package com.app.source;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public final class Pair {
    double x, y;

    public Pair ordered() {
        return this.x < this.y ? this : new Pair(this.y, this.x);
    }
}
