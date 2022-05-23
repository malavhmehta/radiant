#include "src/hittable/hittable_list.h"
#include "src/render/render.h"
#include "src/scene/scene.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // image
    const std::string image_name = "image.ppm";

    const double aspect_ratio = 1;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 2000;
    const int max_depth = 50;

    std::ofstream image;
    image.open("../samples/" + image_name);

    // scene
    hittable_list world = final_scene();

    // camera
    const point look_from = point(478, 278, -600);
    const point look_at = point(278, 278, 0);
    const double vfov = 40;
    const double aperture = 0;
    const color background = color(0, 0, 0);
    const vec vup(0, 1, 0);
    const double dist_to_focus = 10;
    camera cam(look_from, look_at, vup, vfov, aspect_ratio, aperture,
               dist_to_focus, 0, 1);

    // render
    const unsigned int num_threads = std::thread::hardware_concurrency();
    const int job_length = 2;

    render(image_width, image_height, samples_per_pixel, max_depth, world,
           background, cam, num_threads, job_length,
           image);
    image.close();

    // execution time
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            stop - start);

    double mins = duration.count() / 60000.0;
    int m = static_cast<int>(mins);
    double s = (mins - m) * 60;

    std::cout << "[info] finished in " << m << "m " << s << "s\n";
    return 0;
}
