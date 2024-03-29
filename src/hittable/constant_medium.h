#ifndef RADIANT_CONSTANT_MEDIUM_H
#define RADIANT_CONSTANT_MEDIUM_H

#include <utility>

#include "../main.h"

#include "hittable.h"
#include "../material/material.h"
#include "../material/texture.h"

class constant_medium : public hittable {
public:
    shared_ptr<hittable> boundary;
    shared_ptr<material> phase_function;
    double neg_inv_density;

    constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
            : boundary(std::move(b)),
              neg_inv_density(-1 / d),
              phase_function(make_shared<isotropic>(std::move(a))) {}

    constant_medium(shared_ptr<hittable> b, double d, color c)
            : boundary(std::move(b)),
              neg_inv_density(-1 / d),
              phase_function(make_shared<isotropic>(c)) {}

    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const override;

    bool
    bounding_box(double time0, double time1, aabb &output_box) const override {
        return boundary->bounding_box(time0, time1, output_box);
    }
};

bool constant_medium::hit(const ray &r, double t_min, double t_max,
                          hit_record &rec) const {
    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
        return false;

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0) rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    rec.normal = vec(1, 0, 0);
    rec.front_face = true;
    rec.mat_ptr = phase_function;

    return true;
}

#endif //RADIANT_CONSTANT_MEDIUM_H
