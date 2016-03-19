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
    double wave_triangle::operator()(const double &x) const {
        /*fix out-of-range values*/
        double x2 = std::fabs(std::fmod(x, 1.0));
        if (x2 < 0.5) {
            return 2.0 * x2;
        } else {
            return 2.0 - 2.0 * x2;
        }
    }

    double wave_sawtooth::operator()(const double &x) const {
        return std::fabs(std::fmod(x * 2, 1.0));
    }

    double wave_sine::operator()(const double &x) const {
        return 0.5 + std::sin(2.0 * PI * x) / 2.0;
    }

    double wave_square::operator()(const double &x) const {
        if (std::fabs(std::fmod(x, 1.0)) < 0.5L) {
            return 0;
        } else {
            return 1.0;
        }
    }

    wave_fourier_square::wave_fourier_square(const std::string &spec) {
        constexpr size_t spec_begin_length = std::strlen("fourier_square:");
        n = std::stoull(spec.substr(spec_begin_length));
    }

    double wave_fourier_square::operator()(const double &x) const {
        double result = 0.0;
        for (size_t i = 1; i < n; i++) {
            result +=
                    sin((2.0 * i - 1.0) * 2.0 * PI * x) / (2.0 * i - 1.0);
        }
        /*divide by max value on range*/
        return (0.5 + result * (2.0 / PI))/1.13661977236758;
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


    distance_wave::distance_wave(wave *_w, const size_t table_size,
                                 const double wave_size) : w(_w), offset(0),
                                                           wave_size(
                                                                   wave_size) {

        lookup_table.reserve(table_size);

        if (w == nullptr) {
            w = new wave_sawtooth();
        }
    }

    size_t distance_wave::_find_min(size_t left, size_t right,
                                    const double &x,
                                    const double &y) const {
        /*simply binary-search the lookup table, using the derivative at the
         * midpoint of the interval*/
        constexpr double threshold = std::sqrt(
                std::numeric_limits<double>::epsilon());
        while (left != right && right - left > 1) {
            /*overflow-safe average*/
            size_t mid = left / 2 + right / 2 + (left & right & 1);
            double mid_diff = lookup_table[mid].diff(x, y);
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

    double distance_wave::operator()(const double &x,
                                     const double &y) const {
        double min_dist = INFINITY;
        size_t i = wid;
        for (; i < lookup_table.size(); i += wid) {
            if (lookup_table[i - wid].diff(x, y) < 0 &&
                lookup_table[i].diff(x, y) > 0) {
                size_t new_min_idx = _find_min(i - wid, i, x, y);
                double new_min_dst = lookup_table[new_min_idx].dist2(x, y);
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
                double new_min_dst = lookup_table[new_min_idx].dist2(x, y);
                if (new_min_dst < min_dist) {
                    min_dist = new_min_dst;
                }
            }
        }
        return (*w)(100 * std::sqrt(min_dist) / wave_size + offset);
    }

    void distance_wave::set_offset(const double x) {
        offset = std::fabs(std::fmod(x, 1.0));
    }

    distance_wave::~distance_wave() { }


    /*fillers*/
    void image_fill_circle_grid(matrix<double> &grid,
                                const double &theta_mul,
                                const double &dist_mul,
                                wave *w1, wave *w2) {
        if (w1 == nullptr) {
            w1 = new wave_sine();
        }
        if (w2 == nullptr) {
            w2 = new wave_sine();
        }
        vctr<double> mid(grid.x() / 2.0, grid.y() / 2.0);
        double diagonal_dist = mid.mag() / 2.0;
        double theta, d;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                d = mid.dist(x, y) * dist_mul / diagonal_dist;
                theta = std::atan2(y - mid.y, x - mid.x) / (2.0 * PI) *
                        theta_mul;
                grid(x, y) = (*w1)(theta) + (*w2)(d);
            }
        }
    }

    void image_fill_concentric_waves(matrix<double> &grid,
                                     const double &mul, wave *wave_func) {
        vctr<double> mid(grid.x() / 2.0, grid.y() / 2.0);
        double diagonal_dist = mid.mag() / 2.0;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                grid(x, y) = (*wave_func)(mid.dist(x, y) * mul / diagonal_dist);
            }
        }
    }

    void image_fill_pointing_out(matrix<double> &grid,
                                 const double &mul, wave *wave_func) {
        vctr<double> mid(grid.x() / 2.0, grid.y() / 2.0);
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                grid(x, y) = (*wave_func)(
                        std::atan2(y - mid.y, x - mid.x) / (2.0 * PI) *
                        mul);
            }
        }
    }

    void image_fill_2d_wave(matrix<double> &grid, wave_2d *w_2d) {
        vctr<double> mid(grid.x() / 2.0, grid.y() / 2.0);
        double mag = std::min(grid.x(), grid.y()) / 2;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                /*current point*/
                vctr<double> current(x, y);
                /*scale current point*/
                current -= mid;
                current /= mag;
                grid(x, y) = (*w_2d)(current.x, current.y);
            }
        }
    }

    void image_fill_apply_wave_to_dist(const matrix<double> &in,
                                       matrix<double> &out, wave *w,
                                       const double offset) {
        for (size_t x = 0; x < in.x(); x++) {
            for (size_t y = 0; y < out.y(); y++) {
                out(x, y) = (*w)(in(x, y) + offset);
            }
        }

    }
}
