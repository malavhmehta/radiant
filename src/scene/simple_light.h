#ifndef RADIANT_SIMPLE_LIGHT_H
#define RADIANT_SIMPLE_LIGHT_H

#include "../material/material.h"
#include "../material/texture.h"
#include "../hittable/hittable_list.h"
#include "../hittable/sphere.h"
#include "../hittable/aarect.h"

hittable_list simple_light() {
    hittable_list objects;

    auto per_text = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point(0, -1000, 0), 1000,
                                    make_shared<lambertian>(per_text)));
    objects.add(make_shared<sphere>(point(0, 2, 0), 2,
                                    make_shared<lambertian>(per_text)));

    auto diff_light = make_shared<diffuse_light>(color(5, 5, 5));
    objects.add(make_shared<sphere>(point(0, 6.5, 0), 2, diff_light));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, diff_light));

    return objects;
}

#endif //RADIANT_SIMPLE_LIGHT_H
