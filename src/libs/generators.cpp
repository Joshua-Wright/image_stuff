// (c) Copyright 2015 Josh Wright

#include "generators.h"

namespace image_utils {


    long double wave_triangle::operator()(const long double &x) const {
        /*fix out-of-range values*/
        long double x2 = std::fabs(std::fmod(x, 1.0L));
        if (x2 < 0.5) {
            return 2.0L * x2;
        } else {
            return 2.0L - 2.0L * x2;
        }
    }

    long double wave_fourier_square::operator()(const long double &x) const {
        long double result = 0.0L;
        for (size_t i = 1; i < n; i++) {
            result +=
                    sin((2.0L * i - 1.0L) * 2.0L * PI * x) / (2.0L * i - 1.0L);
        }
        return result * (4.0L / PI);
    }

    long double wave_sine::operator()(const long double &x) const {
        return 1.0L + std::sin(2.0L * PI * x) / 2.0L;
    }

    long double wave_square::operator()(const long double &x) const {
        if (std::fabs(std::fmod(x, 1.0L)) < 0.5L) {
            return 0L;
        } else {
            return 1.0L;
        }
    }


    void image_fill_circle_grid(matrix<long double> &grid,
                                long double theta_mul, long double dist_mul,
                                wave *wave_func){
        if (wave_func == nullptr) {
            wave_func = new wave_sine();
        }
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double diagonal_dist = mid.mag() / 2.0L;
        long double theta, d;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                d = mid.dist(x, y) * dist_mul / diagonal_dist;
                theta = std::atan2(y - mid.y, x - mid.x) / (2.0L * PI) *
                        theta_mul;
                grid(x, y) = (*wave_func)(theta) + (*wave_func)(d);
            }
        }
    }
}
