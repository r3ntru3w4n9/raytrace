package com.app.source;
public record Pair(double x, double y) {
    public Pair ordered() {
        return this.x < this.y ? this : new Pair(this.y, this.x);
    }
}
