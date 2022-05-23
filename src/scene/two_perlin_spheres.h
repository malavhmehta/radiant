#ifndef RADIANT_TWO_PERLIN_SPHERES_H
#define RADIANT_TWO_PERLIN_SPHERES_H

#include "../hittable/hittable_list.h"
#include "../material/material.h"
#include "../material/texture.h"
#include "../hittable/sphere.h"

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto per_text = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point(0, -1000, 0), 1000,
                                    make_shared<lambertian>(per_text)));
    objects.add(make_shared<sphere>(point(0, 2, 0), 2,
                                    make_shared<lambertian>(per_text)));

    return objects;
}

#endif //RADIANT_TWO_PERLIN_SPHERES_H
