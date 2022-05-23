#ifndef RADIANT_RANDOM_SCENE_H
#define RADIANT_RANDOM_SCENE_H

#include "../util/camera.h"
#include "../hittable/hittable_list.h"
#include "../material/material.h"
#include "../hittable/moving_sphere.h"
#include "../hittable/sphere.h"
#include "../material/texture.h"
#include "../util/vec.h"

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1),
                                                color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point(0, -1000, 0), 1000,
                                  make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            double choose_mat = random_double();
            point center(a + 0.9 * random_double(), 0.2,
                         b + 0.9 * random_double());

            if ((center - point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    vec center2 = center + vec(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0, 1,
                                                         0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    color albedo = color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(
                            make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(
                            make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    shared_ptr<dielectric> material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point(0, 1, 0), 1, material1));

    shared_ptr<lambertian> material2 = make_shared<lambertian>(
            color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1, material2));

    shared_ptr<metal> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0);
    world.add(make_shared<sphere>(point(4, 1, 0), 1, material3));

    return world;
}

#endif //RADIANT_RANDOM_SCENE_H
