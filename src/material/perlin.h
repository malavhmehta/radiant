#ifndef RADIANT_PERLIN_H
#define RADIANT_PERLIN_H

#include "../main.h"

class perlin {
private:
    static const int point_count = 256;
    vec *ran_vec;
    int *perm_x, *perm_y, *perm_z;

    static void permute(int *p, int n) {
        for (int i = n - 1; i > 0; --i) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static int *perlin_generator_perm() {
        int *p = new int[point_count];

        for (int i = 0; i < point_count; ++i)
            p[i] = i;

        permute(p, point_count);

        return p;
    }

    static double
    tri_linear_interp(double c[2][2][2], double u, double v, double w) {
        double accum = 0.0;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    accum += (i * u + (1 - i) * (1 - u)) *
                             (j * v + (1 - j) * (1 - v)) *
                             (k * w + (1 - k) * (1 - w)) *
                             c[i][j][k];
        return accum;
    }

    static double perlin_interp(vec c[2][2][2], double u, double v, double w) {
        double uu = u * u * (3 - 2 * u);
        double vv = v * v * (3 - 2 * v);
        double ww = w * w * (3 - 2 * w);
        double accum = 0;

        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k) {
                    vec weight(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) *
                             (j * vv + (1 - j) * (1 - vv)) *
                             (k * ww + (1 - k) * (1 - ww)) *
                             dot(c[i][j][k], weight);
                }

        return accum;
    }

public:
    perlin() {
        ran_vec = new vec[point_count];
        for (int i = 0; i < point_count; ++i) {
            ran_vec[i] = unit_vector(vec::random(-1, 1));
        }
        perm_x = perlin_generator_perm();
        perm_y = perlin_generator_perm();
        perm_z = perlin_generator_perm();
    }

    ~perlin() {
        delete[] ran_vec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    [[nodiscard]] double noise(const point &p) const {
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());
        int i = static_cast<int>(floor(p.x()));
        int j = static_cast<int>(floor(p.y()));
        int k = static_cast<int>(floor(p.z()));
        vec c[2][2][2];

        for (int di = 0; di < 2; ++di)
            for (int dj = 0; dj < 2; ++dj)
                for (int dk = 0; dk < 2; ++dk)
                    c[di][dj][dk] = ran_vec[perm_x[(i + di) & 255] ^
                                            perm_y[(j + dj) & 255] ^
                                            perm_z[(k + dk) & 255]];

        return perlin_interp(c, u, v, w);
    }

    [[nodiscard]] double turbulence(const point &p, int depth = 7) const {
        double accum = 0;
        point temp_p = p;
        double weight = 1;

        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return abs(accum);
    }
};

#endif //RADIANT_PERLIN_H
