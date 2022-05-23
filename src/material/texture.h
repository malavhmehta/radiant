#ifndef RADIANT_TEXTURE_H
#define RADIANT_TEXTURE_H

#include "../main.h"
#include "../util/r_stb_image.h"
#include "perlin.h"

#include <utility>

class texture {
public:
    [[nodiscard]] virtual color
    value(double u, double v, const point &p) const = 0;
};

class solid_color : public texture {
private:
    color color_value;

public:
    solid_color() = default;

    explicit solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue) : solid_color(
            color(red, green, blue)) {}

    [[nodiscard]] color value(double u, double v, const vec &p) const override {
        return color_value;
    }
};

class checker_texture : public texture {
public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;

    checker_texture() = default;

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(std::move(_even)), odd(std::move(_odd)) {}

    checker_texture(color c1, color c2)
            : even(make_shared<solid_color>(c1)),
              odd(make_shared<solid_color>(c2)) {}

    [[nodiscard]] color
    value(double u, double v, const point &p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        return sines < 0 ? odd->value(u, v, p) : even->value(u, v, p);

    }
};

class noise_texture : public texture {
public:
    perlin noise;
    double scale{};

    noise_texture() = default;

    explicit noise_texture(double sc) : scale(sc) {}

    [[nodiscard]] color
    value(double u, double v, const point &p) const override {
        return color(1, 1, 1) * 0.5 *
               (1 + sin(scale * p.z() + 10 * noise.turbulence(scale * p)));
    }
};

class image_texture : public texture {
private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
    int v_offs, h_offs;

public:
    const static int bytes_per_pixel = 3;
    constexpr const static double color_scale = 1.0 / 255;

    image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0),
                      v_offs(0), h_offs(0) {}

    explicit image_texture(const char *filename, int _v_offs = 0,
                           int _h_offs = 0) {
        int components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel,
                         components_per_pixel);

        if (!data) {
            std::cerr << "[error]: unable to load texture image file '"
                      << filename << "'\n";
            width = height = 0;
        }

        v_offs = _v_offs % height;
        h_offs = _h_offs % width;

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    [[nodiscard]] color value(double u, double v, const vec &p) const override {
        if (data == nullptr)
            return {0, 1, 1};

        u = clamp(u, 0, 1);
        v = 1 - clamp(v, 0, 1);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        i = (i + h_offs + width) % width;
        j = (j + v_offs + height) % height;

        unsigned char *pixel =
                data + j * bytes_per_scanline + i * bytes_per_pixel;
        return {color_scale * pixel[0], color_scale * pixel[1],
                color_scale * pixel[2]};
    }
};

#endif //RADIANT_TEXTURE_H
