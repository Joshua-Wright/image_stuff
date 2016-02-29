// (c) Copyright 2015 Josh Wright

#include "generators.h"

namespace image_utils {

    long double fourier_square_wave(long double x, size_t n) {
        /*
         * period of wave is fixed
         * n: number of iterations of wave
         */
        long double result = 0.0L;
        for (size_t i = 1; i < n; i++) {
            result +=
                    sin((2.0L * i - 1.0L) * 2.0L * PI * x) / (2.0L * i - 1.0L);
        }
        return result * (4.0L / PI);
    }

    long double square_wave(void *, long double x) {
        x = std::fabs(std::fmod(x, 1.0L));
        if (x < 0.5L) {
            return 0L;
        } else {
            return 1.0L;
        }
    }

    long double sine_wave(void *, long double x) {
        return 1.0L + std::sin(2.0L * PI * x) / 2.0L;
    }

    long double triangle_wave(void *, long double x) {
        /*fix out-of-range values*/
        x = std::fabs(std::fmod(x, 1.0L));
        if (x < 0.5) {
            return 2.0L * x;
        } else {
            return 2.0L - 2.0L * x;
        }
    }

    void image_fill_circle_grid(matrix<long double> &grid,
                                long double theta_mul, long double dist_mul,
                                wave_function func,
                                void *wave_param) {
        /*
         * theta_mul: larger => more angular ripples (each ripple is smaller)
         * dist_mult: larger => more radial ripples (each ripple is smaller)
         */
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double diagonal_dist = mid.mag() / 2.0L;
        long double theta, d;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                d = mid.dist(x, y) * dist_mul / diagonal_dist;
                theta = std::atan2(y - mid.y, x - mid.x) / (2.0L * PI) *
                        theta_mul;
                grid(x, y) = func(wave_param, theta) + func(wave_param, d);
            }
        }
    }
}
