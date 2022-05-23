#ifndef RADIANT_COLOR_H
#define RADIANT_COLOR_H

#include "../main.h"

#include <iostream>
#include <sstream>

const double max_color = 255.99999;

struct icolor {
    int r, g, b;
};

inline int channel(const double &c, const double &scale) {
    return static_cast<int>(max_color * clamp(sqrt(scale * c), 0, 1));
}

inline std::string color_str(color pixel_color, int samples_per_pixel) {
    double scale = 1.0 / samples_per_pixel;
    std::string r = std::to_string(channel(pixel_color.x(), scale));
    std::string g = std::to_string(channel(pixel_color.y(), scale));
    std::string b = std::to_string(channel(pixel_color.z(), scale));
    return r + ' ' + g + ' ' + b + '\n';
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    out << color_str(pixel_color, samples_per_pixel);
}

void
write_color(std::stringstream &out, color pixel_color, int samples_per_pixel) {
    out << color_str(pixel_color, samples_per_pixel);
}

inline icolor average(const color &pixel_color, const double &scale) {
    return {channel(pixel_color.x(), scale), channel(pixel_color.y(), scale),
            channel(pixel_color.z(), scale)};
}

#endif //RADIANT_COLOR_H
