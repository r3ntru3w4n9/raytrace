package com.app.source;

import java.util.ArrayList;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public final class List implements Hittable {
    private ArrayList<Hittable> objects;

    public List() {
        this.objects = new ArrayList<Hittable>();
    }

    public List(ArrayList<Hittable> objects) {
        this.objects = objects;
    }

    public void add(Hittable obj) {
        objects.add(obj);
    }

    @Override
    public HitData hit(Vector source, Vector towards) {
        var minHit = HitData.miss();
        for (Hittable h : objects) {
            var data = h.hit(source, towards);
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

                var bnd = iter.next().bounds();

                while (iter.hasNext()) {
                    bnd = Box.wraps(bnd, iter.next().bounds());
                }
                return bnd;
        }
    }
}
