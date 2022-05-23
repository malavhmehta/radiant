cmake_minimum_required(VERSION 3.22)
project(radiant)

set(CMAKE_CXX_STANDARD 20)

add_executable(radiant main.cpp src/util/camera.h src/util/color.h src/hittable/hittable_list.h src/hittable/hittable.h src/main.h src/material/material.h src/render/ray.h src/hittable/sphere.h src/util/vec.h src/util/random.h src/render/render.h src/scene/scene.h src/render/thread_pool.h src/hittable/moving_sphere.h src/bounding_box/aabb.h src/bounding_box/bvh.h src/material/texture.h src/scene/random_scene.h src/scene/two_spheres.h src/material/perlin.h src/scene/two_perlin_spheres.h include/stb_image.h src/util/r_stb_image.h src/scene/earth.h src/hittable/aarect.h src/scene/simple_light.h src/scene/cornell_box.h src/hittable/box.h src/hittable/constant_medium.h src/scene/cornell_smoke.h src/scene/final_scene.h)
