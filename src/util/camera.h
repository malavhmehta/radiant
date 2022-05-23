#ifndef RADIANT_CAMERA_H
#define RADIANT_CAMERA_H

#include "../main.h"

class camera {
private:
    point origin, lower_left_corner;
    vec horizontal, vertical, u, v, w;
    double lens_radius, time0, time1;

public:
    camera(
            point lookfrom,
            point lookat,
            vec vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0 = 0,
            double _time1 = 0
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner =
                origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    [[nodiscard]] ray get_ray(double s, double t) const {
        vec rd = lens_radius * random_in_unit_disk();
        vec offset = u * rd.x() + v * rd.y();
        return {origin + offset,
                lower_left_corner + s * horizontal + t * vertical - origin -
                offset,
                random_double(time0, time1)};
    }
};

#endif //RADIANT_CAMERA_H
