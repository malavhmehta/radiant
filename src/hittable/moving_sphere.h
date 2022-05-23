#ifndef RADIANT_MOVING_SPHERE_H
#define RADIANT_MOVING_SPHERE_H

#include <utility>

#include "../main.h"
#include "hittable.h"

class moving_sphere : public hittable {
public:
    point center0, center1;
    double radius{}, time0{}, time1{};
    shared_ptr<material> mat_ptr;

    moving_sphere() = default;

    moving_sphere(
            point cen0,
            point cen1,
            double _time0,
            double _time1,
            double r,
            shared_ptr<material> m
    ) : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r),
        mat_ptr(std::move(m)) {}

    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const override;

    bool
    bounding_box(double _time0, double _time1, aabb &output_box) const override;

    [[nodiscard]] point center(double time) const;
};

point moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray &r, double t_min, double t_max,
                        hit_record &rec) const {
    vec oc = r.origin() - center(r.time());
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;

    double d = half_b * half_b - a * c;
    if (d < 0) return false;
    double sqrt_d = sqrt(d);

    double root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool moving_sphere::bounding_box(double _time0, double _time1,
                                 aabb &output_box) const {
    aabb box0(center(_time0) - vec(radius, radius, radius),
              center(_time0) + vec(radius, radius, radius));
    aabb box1(center(_time1) - vec(radius, radius, radius),
              center(_time1) + vec(radius, radius, radius));
    output_box = surrounding_box(box0, box1);
    return true;
}

#endif //RADIANT_MOVING_SPHERE_H
