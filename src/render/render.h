#ifndef RADIANT_RENDER_H
#define RADIANT_RENDER_H

#include "../util/camera.h"
#include "../util/color.h"
#include "../hittable/hittable.h"
#include "../material/material.h"
#include "thread_pool.h"
#include "../util/vec.h"

#include <chrono>
#include <thread>

color ray_color(const ray &r, const color &background, const hittable &world,
                int depth) {
    hit_record rec;
    if (depth <= 0) return {0, 0, 0};

    if (!world.hit(r, 0.001, infinity, rec))
        return background;


    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted +
           attenuation * ray_color(scattered, background, world, depth - 1);
}

class render_job {
public:
    static int image_width;
    static int image_height;
    static int samples_per_pixel;
    static double scale;
    static int max_depth;
    static hittable_list world;
    static color background;
    static camera cam;
    static std::vector<std::vector<icolor>> pixels;

    int start_line;
    int end_line;

    render_job() : start_line(0), end_line(0) {}

    render_job(int sl, int el) : start_line(sl), end_line(el) {}

    void run() {
        for (int j = start_line; j <= end_line; ++j) {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    double u = (i + random_double()) / (image_width - 1);
                    double v = (j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, background, world, max_depth);
                }
                pixels[j][i] = average(pixel_color, scale);
            }
        }
    }
};

int render_job::image_width = 0;
int render_job::image_height = 0;
int render_job::samples_per_pixel = 0;
double render_job::scale = 0;
int render_job::max_depth = 0;
hittable_list render_job::world = hittable_list();
color render_job::background = color(0, 0, 0);
camera render_job::cam = camera(point(0, 0, 0),
                                point(0, 0, 0),
                                vec(0, 0, 0),
                                0,
                                0,
                                0,
                                0);
std::vector<std::vector<icolor>> render_job::pixels = {};

void render(const int &image_width,
            const int &image_height,
            const int &samples_per_pixel,
            const int &max_depth,
            const hittable_list &world,
            const color &background,
            const camera &cam,
            const unsigned int &num_threads,
            const int &job_length,
            std::ostream &image) {
    render_job::image_width = image_width;
    render_job::image_height = image_height;
    render_job::samples_per_pixel = samples_per_pixel;
    render_job::scale = 1.0 / samples_per_pixel;
    render_job::max_depth = max_depth;
    render_job::world = world;
    render_job::background = background;
    render_job::cam = cam;
    render_job::pixels.resize(image_height, std::vector<icolor>(image_width));

    image << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    thread_pool<render_job> pool;
    pool.start(num_threads);
    std::cout << "[info] started pool with " << num_threads << " threads\n";

    const int jobs_left = image_height % job_length;
    int jobs_created = 0;

    for (int i = 0; i + job_length - 1 < image_height; i += job_length) {
        pool.queue_job(render_job(i, i + job_length - 1));
        std::cout << "\r[info] " << ++jobs_created << " render jobs created "
                  << std::flush;
    }

    if (jobs_left != 0) {
        pool.queue_job(
                render_job(image_height - jobs_left - 1, image_height - 1));
        std::cout << "\r[info] " << ++jobs_created << " render jobs created "
                  << std::flush;
    }

    while (!pool.is_done()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pool.stop();
    std::cout << "\n[info] stopped thread pool\n";

    std::cout << "[info] writing image\n";
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            image << render_job::pixels[j][i].r << ' '
                  << render_job::pixels[j][i].g << ' '
                  << render_job::pixels[j][i].b << '\n';
        }
    }
}

#endif //RADIANT_RENDER_H
