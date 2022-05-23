#ifndef RADIANT_FINAL_SCENE_H
#define RADIANT_FINAL_SCENE_H

#include "../main.h"
#include "../hittable/hittable_list.h"
#include "../hittable/box.h"
#include "../hittable/sphere.h"
#include "../hittable/moving_sphere.h"
#include "../hittable/constant_medium.h"
#include "../bounding_box/bvh.h"
#include "../material/material.h"

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point(x0, y0, z0), point(x1, y1, z1),
                                        ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point(400, 400, 200);
    auto center2 = center1 + vec(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50,
                                           moving_sphere_material));

    objects.add(make_shared<sphere>(point(260, 150, 45), 50,
                                    make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(point(0, 150, 145), 50,
                                    make_shared<metal>(color(0.8, 0.8, 0.9),
                                                       1.0)));

    auto boundary = make_shared<sphere>(point(360, 150, 145), 70,
                                        make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(
            make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point(0, 0, 0), 5000,
                                   make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(
            make_shared<image_texture>("../in/earth_map.jpg"));
    objects.add(make_shared<sphere>(point(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point(220, 280, 300), 80,
                                    make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
            make_shared<rotate_y>(make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec(-100, 270, 395)));

    return objects;
}


#endif //RADIANT_FINAL_SCENE_H
