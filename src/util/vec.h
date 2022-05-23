#ifndef RADIANT_VEC_H
#define RADIANT_VEC_H

#include "random.h"

#include <cmath>
#include <iostream>

using std::sqrt;

class vec {
public:
    double e[3];

    vec() : e{0, 0, 0} {}

    vec(double e0, double e1, double e2) : e{e0, e1, e2} {}

    [[nodiscard]] double x() const { return e[0]; }

    [[nodiscard]] double y() const { return e[1]; }

    [[nodiscard]] double z() const { return e[2]; }

    vec operator-() const { return {-e[0], -e[1], -e[2]}; }

    double operator[](int i) const { return e[i]; }

    double &operator[](int i) { return e[i]; }

    vec &operator+=(const vec &v) {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    vec &operator-=(const vec &v) { return *this += -v; }

    vec &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec &operator/=(const double t) {
        return *this *= 1 / t;
    }

    [[nodiscard]] double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    [[nodiscard]] double length() const {
        return sqrt(length_squared());
    }

    [[nodiscard]] bool near_zero() const {
        const double s = 1e-7;
        return abs(e[0]) < s && abs(e[1]) < s && abs(e[2]) < s;
    }

    inline static vec random() {
        return {random_double(), random_double(), random_double()};
    }

    inline static vec random(double min, double max) {
        return {random_double(min, max), random_double(min, max),
                random_double(min, max)};
    }
};

using point = vec;
using color = vec;

inline std::ostream &operator<<(std::ostream &out, const vec &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec operator+(const vec &u, const vec &v) {
    return {u[0] + v[0], u[1] + v[1], u[2] + v[2]};
}

inline vec operator-(const vec &u, const vec &v) {
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

inline vec operator*(const vec &u, const vec &v) {
    return {u[0] * v[0], u[1] * v[1], u[2] * v[2]};
}

inline vec operator*(const double t, const vec &v) {
    return {v[0] * t, v[1] * t, v[2] * t};
}

inline vec operator*(const vec &v, const double t) {
    return {v[0] * t, v[1] * t, v[2] * t};
}

inline vec operator/(const vec &v, double t) {
    return v * (1 / t);
}

inline double dot(const vec &u, const vec &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec cross(const vec &u, const vec &v) {
    return {u[1] * v[2] - u[2] * v[1],
            u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0]};
}

inline vec unit_vector(const vec &v) {
    return v / v.length();
}

inline vec random_in_unit_sphere() {
    vec p;
    while (p = vec::random(-1, 1), p.length_squared() >= 1);
    return p;
}

vec random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

vec random_in_hemisphere(const vec &normal) {
    vec in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0)
        return in_unit_sphere;
    return -in_unit_sphere;
}

vec reflect(const vec &v, const vec &n) {
    return v - 2 * dot(v, n) * n;
}

vec refract(const vec &uv, const vec &n, double ni_over_nt) {
    double cos_theta = fmin(dot(-uv, n), 1);
    vec r_out_perp = ni_over_nt * (uv + cos_theta * n);
    vec r_out_parallel = -sqrt(abs(1 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

vec random_in_unit_disk() {
    vec p;
    while (p = vec(random_double(-1, 1), random_double(-1, 1), 0),
            p.length_squared() >= 1);
    return p;
}

#endif //RADIANT_VEC_H
