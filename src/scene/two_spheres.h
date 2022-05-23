#ifndef RADIANT_TWO_SPHERES_H
#define RADIANT_TWO_SPHERES_H

#include "../hittable/hittable_list.h"
#include "../hittable/sphere.h"
#include "../material/texture.h"
#include "../material/material.h"

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1),
                                                color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point(0, -10, 0), 10,
                                    make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point(0, 10, 0), 10,
                                    make_shared<lambertian>(checker)));

    return objects;
}

#endif //RADIANT_TWO_SPHERES_H
