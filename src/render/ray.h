#ifndef RADIANT_RAY_H

#include "../util/vec.h"

class ray {
public:
    point orig;
    vec dir;
    double tm{};

    ray() = default;

    ray(const point &origin, const vec &direction, double time = 0.0) : orig(
            origin), dir(direction), tm(time) {}

    [[nodiscard]] point origin() const { return orig; }

    [[nodiscard]] vec direction() const { return dir; }

    [[nodiscard]] double time() const { return tm; }

    [[nodiscard]] point at(double t) const {
        return orig + t * dir;
    }
};

#endif //RADIANT_RAY_H
