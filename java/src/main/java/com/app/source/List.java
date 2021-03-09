package com.app.source;

import java.util.ArrayList;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
@AllArgsConstructor
public final class List implements Hittable {
    private ArrayList<Hittable> objects;

    public List() {
        this(new ArrayList<Hittable>());
    }

    public void add(Hittable obj) {
        objects.add(obj);
    }

    @Override
    public HitData hit(Vector source, Vector towards) {
        HitData minHit = HitData.miss();

        for (Hittable h : objects) {
            HitData data = h.hit(source, towards);
            minHit = (data.t() < minHit.t()) ? data : minHit;
        }

        return minHit;
    }

    @Override
    public Box bounds() {
        switch (objects.size()) {
            case 0:
                throw new RuntimeException();

            default:
                var iter = objects.iterator();

                Box bnd = iter.next().bounds();

                while (iter.hasNext()) {
                    bnd = Box.wraps(bnd, iter.next().bounds());
                }

                return bnd;
        }
    }
}
