#ifndef RADIANT_EARTH_H
#define RADIANT_EARTH_H

#include "../hittable/hittable_list.h"
#include "../material/material.h"
#include "../hittable/sphere.h"
#include "../material/texture.h"

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("../in/earth_map.jpg", -100,
                                                    750);
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

#endif //RADIANT_EARTH_H
