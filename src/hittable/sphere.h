#ifndef RADIANT_SPHERE_H
#define RADIANT_SPHERE_H

#include <utility>

#include "hittable.h"
#include "../util/vec.h"

class sphere : public hittable {
public:
    point center;
    double radius{};
    shared_ptr<material> mat_ptr;

    sphere() = default;

    sphere(point c, double r, shared_ptr<material> m) : center(c), radius(r),
                                                        mat_ptr(std::move(m)) {}

    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const override;

    bool
    bounding_box(double time0, double time1, aabb &output_box) const override;

private:
    static void get_sphere_uv(const point &p, double &u, double &v) {
        double theta = acos(-p.y());
        double phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

bool
sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec oc = r.origin() - center;
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
    vec outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(center - vec(radius, radius, radius),
                      center + vec(radius, radius, radius));
    return true;
}

#endif //RADIANT_SPHERE_H
