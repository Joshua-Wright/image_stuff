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

    long double wave_sawtooth::operator()(const long double &x) const {
        return std::fabs(std::fmod(x * 2, 1.0L));
    }

    long double wave_sine::operator()(const long double &x) const {
        return 0.5L + std::sin(2.0L * PI * x) / 2.0L;
    }

    long double wave_square::operator()(const long double &x) const {
        if (std::fabs(std::fmod(x, 1.0L)) < 0.5L) {
            return 0L;
        } else {
            return 1.0L;
        }
    }

    long double wave_fourier_square::operator()(const long double &x) const {
        long double result = 0.0L;
        for (size_t i = 1; i < n; i++) {
            result +=
                    sin((2.0L * i - 1.0L) * 2.0L * PI * x) / (2.0L * i - 1.0L);
        }
        return 0.5 + result * (2.0L / PI);
    }

    void image_fill_circle_grid(matrix<long double> &grid,
                                const long double &theta_mul,
                                const long double &dist_mul,
                                wave *w1, wave *w2) {
        if (w1 == nullptr) {
            w1 = new wave_sine();
        }
        if (w2 == nullptr) {
            w2 = new wave_sine();
        }
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double diagonal_dist = mid.mag() / 2.0L;
        long double theta, d;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                d = mid.dist(x, y) * dist_mul / diagonal_dist;
                theta = std::atan2(y - mid.y, x - mid.x) / (2.0L * PI) *
                        theta_mul;
                grid(x, y) = (*w1)(theta) + (*w2)(d);
            }
        }
    }

    void image_fill_concentric_waves(matrix<long double> &grid,
                                     const long double &mul, wave *wave_func) {
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double diagonal_dist = mid.mag() / 2.0L;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                grid(x, y) = (*wave_func)(mid.dist(x, y) * mul / diagonal_dist);
            }
        }
    }

    void image_fill_pointing_out(matrix<long double> &grid,
                                 const long double &mul, wave *wave_func) {
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double diagonal_dist = mid.mag() / 2.0L;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                grid(x, y) = (*wave_func)(
                        std::atan2(y - mid.y, x - mid.x) / (2.0L * PI) *
                        mul);
            }
        }
    }

    long double rose_dist::operator()(const long double &x,
                                      const long double &y) const {
        /*TODO: realistic value for the min*/
        long double min = 99999;
        /*TODO: test wid to find the right value in terms of t*/
        size_t wid = (size_t) (lookup_table.size() /
                               (3 * max_t / PI));
        /*TODO: allow this loop to be more easily vectorized*/
        for (size_t i = wid; /*interval width*/
             i < lookup_table.size(); i += wid) {
            if (lookup_table[i - wid].diff(x, y) < 0 &&
                lookup_table[i].diff(x, y) > 0) {
                long double new_min = _find_min(i - wid, i, x, y);
                if (new_min < min) {
                    min = new_min;
                }
            }
        }
        return (*w)(min * 5);
    }

    rose_dist::rose_dist(const long double n, const long double d,
                         const size_t table_size,
                         const long double _max_t) : max_t(_max_t) {

        lookup_table.reserve(table_size);

        /*TODO: auto-determine max t*/
        /*rho=1 if n*d is odd, rho=2 if n*d is even*/
        /*ref: http://www.lmtsd.org/cms/lib/PA01000427/Centricity/Domain/116/Polar%20Roses.pdf*/
        int rho = (int(n*d) % 2) ? 2 : 1;
        max_t = PI * d * rho;
//        max_t *= 0.5;

        /*TODO: pre-allocate this vector*/
        for (long double t = 0; t < max_t; t += max_t / table_size) {
            lookup_table.emplace_back(n / d, t);
        }
        /*TODO: wave function parameter*/
        w = new wave_fourier_square(3);
    }

    long double rose_dist::_find_min(size_t left, size_t right,
                                     const long double &x,
                                     const long double &y) const {
        /*simply binary-search the lookup table, using the derivative at the
         * midpoint of the interval*/
        while (left != right) {
            size_t mid = left / 2 + right / 2 + (left & right & 1);
            if (lookup_table[mid].diff(x, y) > 0) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return lookup_table[left].dist(x, y);
    }

    void image_fill_2d_wave(matrix<long double> &grid, wave_2d *w_2d) {
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double mag = grid.x() / 2;
//        mid *= 2;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                /*current point*/
                vctr<long double> current(x, y);
                /*scale current point*/
                current -= mid;
                current /= mag;
                grid(x, y) = (*w_2d)(current.x, current.y);
            }
        }
    }
}
