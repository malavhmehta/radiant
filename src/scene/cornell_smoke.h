#ifndef RADIANT_CORNELL_SMOKE_H
#define RADIANT_CORNELL_SMOKE_H

#include "../util/camera.h"
#include "../hittable/hittable_list.h"
#include "../hittable/constant_medium.h"
#include "../material/material.h"
#include "../hittable/aarect.h"
#include "../hittable/box.h"
#include "../hittable/sphere.h"
#include "../material/texture.h"
#include "../util/vec.h"

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point(0, 0, 0),
                                                 point(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(point(0, 0, 0),
                                                 point(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}

#endif //RADIANT_CORNELL_SMOKE_H
