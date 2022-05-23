#ifndef RADIANT_AABB_H
#define RADIANT_AABB_H

#include "../main.h"

class aabb {
public:
    point minimum;
    point maximum;

    aabb() = default;

    aabb(const point &a, const point &b) : minimum(a), maximum(b) {}

    [[nodiscard]] point min() const { return minimum; }

    [[nodiscard]] point max() const { return maximum; }

    [[nodiscard]] inline bool
    hit(const ray &r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            double invD = 1.0f / r.direction()[a];
            double t0 = (min()[a] - r.origin()[a]) * invD;
            double t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }
};

#endif //RADIANT_AABB_H
