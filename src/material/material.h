#ifndef RADIANT_MATERIAL_H
#define RADIANT_MATERIAL_H

#include "../main.h"

#include "../hittable/hittable.h"
#include "texture.h"

class material {
public:
    virtual color emitted(double u, double v, const point &p) const {
        return {0, 0, 0};
    }

    virtual bool
    scatter(const ray &r_in, const hit_record &rec, color &attenuation,
            ray &scattered) const = 0;
};

class lambertian : public material {
public:
    shared_ptr<texture> albedo;

    explicit lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}

    explicit lambertian(shared_ptr<texture> a) : albedo(a) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
                 ray &scattered) const override {
        vec scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};

class metal : public material {
public:
    color albedo;
    double fuzz;

    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
                 ray &scattered) const override {
        vec reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(),
                        r_in.time());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
};

class dielectric : public material {
private:
    static double reflectance(double cosine, double ref_idx) {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

public:
    double ir;

    explicit dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
                 ray &scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec direction;

        if (cannot_refract ||
            reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }
};

class diffuse_light : public material {
public:
    shared_ptr<texture> emit;

    diffuse_light(shared_ptr<texture> a) : emit(std::move(a)) {}

    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
                 ray &scattered) const override {
        return false;
    }

    color emitted(double u, double v, const point &p) const override {
        return emit->value(u, v, p);
    }
};

class isotropic : public material {
public:
    shared_ptr<texture> albedo;

    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}

    explicit isotropic(shared_ptr<texture> a) : albedo(std::move(a)) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
                 ray &scattered) const override {
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};

#endif //RADIANT_MATERIAL_H
