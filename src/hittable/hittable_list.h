#ifndef RADIANT_HITTABLE_LIST_H
#define RADIANT_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <utility>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() = default;

    explicit hittable_list(const shared_ptr<hittable> &object) { add(object); }

    void clear() { objects.clear(); }

    void add(const shared_ptr<hittable> &object) { objects.push_back(object); }

    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const override;

    bool
    bounding_box(double time0, double time1, aabb &output_box) const override;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max,
                        hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const shared_ptr<hittable> &object: objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

aabb surrounding_box(aabb box0, aabb box1) {
    point small(std::min(box0.min().x(), box1.min().x()),
                std::min(box0.min().y(), box1.min().y()),
                std::min(box0.min().z(), box1.min().z()));
    point big(std::max(box0.max().x(), box1.max().x()),
              std::max(box0.max().y(), box1.max().y()),
              std::max(box0.max().z(), box1.max().z()));

    return {small, big};
}

bool hittable_list::bounding_box(double time0, double time1,
                                 aabb &output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const shared_ptr<hittable> &object: objects) {
        if (!object->bounding_box(time0, time1, temp_box))
            return false;
        output_box = first_box ? temp_box : surrounding_box(output_box,
                                                            temp_box);
        first_box = false;
    }
    return true;
}

#endif //RADIANT_HITTABLE_LIST_H
