#ifndef RADIANT_RANDOM_H
#define RADIANT_RANDOM_H

#include <random>

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::random_device dev;
    static std::mt19937 generator(dev());
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

#endif //RADIANT_RANDOM_H
