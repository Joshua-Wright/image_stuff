// (c) Copyright 2015 Josh Wright

#include <cstring>
#include "generators.h"
//#include "cpp_containers/lib/debug.h"

namespace image_utils {


    bool startswith(const std::string &substring, const std::string &parent) {
        auto it1 = substring.begin();
        auto it2 = parent.begin();
        while (it1 != substring.end()) {
            if (it2 == parent.end() || *it1 != *it2) {
                return false;
            }
            ++it1;
            ++it2;
        }
        return true;
    }

    /*wave functions*/
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

    wave_fourier_square::wave_fourier_square(const std::string &spec) {
        constexpr size_t spec_begin_length = std::strlen("fourier_square:");
        n = std::stoull(spec.substr(spec_begin_length));
    }

    long double wave_fourier_square::operator()(const long double &x) const {
        long double result = 0.0L;
        for (size_t i = 1; i < n; i++) {
            result +=
                    sin((2.0L * i - 1.0L) * 2.0L * PI * x) / (2.0L * i - 1.0L);
        }
        return 0.5 + result * (2.0L / PI);
    }

    wave *parse_wave_spec(const std::string &spec) {
        if (startswith("sine", spec)) {
            return new wave_sine();
        } else if (startswith("sawtooth", spec)) {
            return new wave_sawtooth();
        } else if (startswith("triangle", spec)) {
            return new wave_triangle();
        } else if (startswith("fourier_square:", spec)) {
            return new wave_fourier_square(spec);
        } else if (startswith("noop", spec)) {
            return new wave_noop();
        }
        return nullptr;
    }

    rose_dist::rose_dist(wave *_w, const int n, const int d,
                         const size_t table_size, const long double wave_size)
            : wave_size(wave_size),
              w(_w) {

        lookup_table.reserve(table_size);

        /*rho=1 if n*d is odd, rho=2 if n*d is even*/
        /*ref: http://www.lmtsd.org/cms/lib/PA01000427/Centricity/Domain/116/Polar%20Roses.pdf*/
        long double rho = ((n * d) % 2) ? 1.0L : 2.0L;
        max_t = PI * d * rho *
                1.01; /*1.01 to account for rounding errors on teh max value*/

        const long double k = (long double) (n) / (long double) (d);
        for (long double t = 0; t <= max_t; t += max_t / table_size) {
            lookup_table.emplace_back(k, t);
        }

        /*determine interval width*/
        /*PI/(3*max_t) determined by experimentation*/
        if (n < d) {
            wid = (size_t) (table_size * PI / (3 * max_t));
        } else {
            wid = (size_t) (table_size * PI / (6 * rho * max_t));
        }

        if (w == nullptr) {
            w = new wave_sawtooth();
        }
    }

    size_t rose_dist::_find_min(size_t left, size_t right,
                                const long double &x,
                                const long double &y) const {
        /*simply binary-search the lookup table, using the derivative at the
         * midpoint of the interval*/
        constexpr long double threshold = std::sqrt(
                std::numeric_limits<long double>::epsilon());
        while (left != right && right - left > 1) {
            /*overflow-safe average*/
            size_t mid = left / 2 + right / 2 + (left & right & 1);
            long double mid_diff = lookup_table[mid].diff(x, y);
            if (std::fabs(mid_diff) < threshold) {
                return mid;
            } else if (mid_diff > 0) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }

    long double rose_dist::operator()(const long double &x,
                                      const long double &y) const {
        long double min_dist = INFINITY;
        size_t i = wid;
        for (; i < lookup_table.size(); i += wid) {
            if (lookup_table[i - wid].diff(x, y) < 0 &&
                lookup_table[i].diff(x, y) > 0) {
                size_t new_min_idx = _find_min(i - wid, i, x, y);
                long double new_min_dst = lookup_table[new_min_idx].dist2(x, y);
                if (new_min_dst < min_dist) {
                    min_dist = new_min_dst;
                }
            }
        }
        /*check if the last part didn't divide evenly*/
        size_t left = i - wid;
        size_t right = lookup_table.size() - 1;
        if (right - left > 0) {
            if (lookup_table[left].diff(x, y) < 0 &&
                lookup_table[right].diff(x, y) > 0) {
                size_t new_min_idx = _find_min(left, right, x, y);
                long double new_min_dst = lookup_table[new_min_idx].dist2(x, y);
                if (new_min_dst < min_dist) {
                    min_dist = new_min_dst;
                }
            }
        }
        return (*w)(100 * std::sqrt(min_dist) / wave_size + offset);
    }

    void rose_dist::set_offset(const long double x) {
        offset = std::fabs(std::fmod(x, 1.0L));
    }

    rose_dist::~rose_dist() { }


    /*fillers*/
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
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                grid(x, y) = (*wave_func)(
                        std::atan2(y - mid.y, x - mid.x) / (2.0L * PI) *
                        mul);
            }
        }
    }

    void image_fill_2d_wave(matrix<long double> &grid, wave_2d *w_2d) {
        vctr<long double> mid(grid.x() / 2.0L, grid.y() / 2.0L);
        long double mag = std::min(grid.x(), grid.y()) / 2;
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

    void image_fill_apply_range_to_dist(const matrix<long double> &in,
                                        matrix<long double> &out, wave *w,
                                        const long double offset) {
        for (size_t x = 0; x < in.x(); x++) {
            for (size_t y = 0; y < out.y(); y++) {
                out(x, y) = (*w)(in(x, y) + offset);
            }
        }

    }
}
