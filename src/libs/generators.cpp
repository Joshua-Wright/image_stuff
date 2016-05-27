// (c) Copyright 2016 Josh Wright

#include <cstring>
#include "generators.h"

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


    distance_wave::distance_wave(const wave &_w, const size_t table_size,
                                 const double wave_size) : w(_w),
                                                           wave_size(wave_size),
                                                           offset(0) {

        lookup_table.reserve(table_size);
    }

    size_t distance_wave::_find_min(size_t left, size_t right,
                                    const double &x,
                                    const double &y) const {
        /*simply binary-search the lookup table, using the derivative at the
         * midpoint of the interval*/
        while (left != right && right - left > 1) {
            /*overflow-safe average*/
            size_t mid = left / 2 + right / 2 + (left & right & 1);
            double mid_diff = lookup_table[mid].diff(x, y);
            if (mid_diff > 0) {
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
        return w(100 * std::sqrt(min_dist) / wave_size + offset);
    }

    void distance_wave::set_offset(const double x) {
        offset = std::fabs(std::fmod(x, 1.0));
    }

    distance_wave::~distance_wave() { }


    /*fillers*/
    void image_fill_circle_grid(matrix<double> &grid,
                                const double &theta_mul,
                                const double &dist_mul,
                                const wave &wave_dist, const wave &wave_theta) {
        const vect mid{grid.x() / 2.0, grid.y() / 2.0};
        const double diagonal_dist = mid.norm() / 2.0;
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                vect pt{(double) x, (double) y};
                double d = mid.dist(pt) * dist_mul / diagonal_dist;
                double theta = std::atan2(y - mid[1], x - mid[0]) / (2.0 * PI) *
                        theta_mul;
                grid(x, y) = wave_dist(theta) + wave_theta(d);
            }
        }
    }

    void image_fill_concentric_waves(matrix<double> &grid,
                                     const double &mul, const wave &wave_func) {
        vect mid{grid.x() / 2.0, grid.y() / 2.0};
        double diagonal_dist = mid.norm() / 2.0;
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                vect pt{(double) x, (double) y};
                grid(x, y) = wave_func(mid.dist(pt) * mul / diagonal_dist);
            }
        }
    }

    void image_fill_pointing_out(matrix<double> &grid,
                                 const double &mul, const wave &wave_func) {
        vect mid{grid.x() / 2.0, grid.y() / 2.0};
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                grid(x, y) = wave_func(
                        std::atan2(y - mid[1], x - mid[0]) / (2.0 * PI) *
                        mul);
            }
        }
    }

    void image_fill_2d_wave(matrix<double> &grid, wave_2d *w_2d) {
        const vect mid{grid.x() / 2.0, grid.y() / 2.0};
        const double mag = std::min(grid.x(), grid.y()) / 2;
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < grid.x(); x++) {
            for (size_t y = 0; y < grid.y(); y++) {
                /*current point*/
                vect pt{(double) x, (double) y};
                /*scale current point*/
                pt -= mid;
                pt /= mag;
                grid(x, y) = (*w_2d)(pt[0], pt[1]);
            }
        }
    }

    void image_fill_apply_wave_to_dist(const matrix<double> &in,
                                       matrix<double> &out, const wave &w,
                                       const double offset) {
        for (size_t x = 0; x < in.x(); x++) {
            for (size_t y = 0; y < out.y(); y++) {
                out(x, y) = w(in(x, y) + offset);
            }
        }

    }

    dist_lissajous::dist_lissajous(const wave &w, const size_t table_size,
                                   const double wave_size,
                                   const double A, const double B,
                                   const double a, const double b,
                                   const double sigma) : distance_wave(w,
                                                                       table_size,
                                                                       wave_size) {
        max_t = (2.1 * PI);
        wid = (size_t) (table_size * PI / (12 * max_t));
//        wid = (size_t) (table_size * PI / (80 * max_t));

        lookup_table.reserve(table_size);
        lookup_table.assign(table_size, cached_value(0, 0, 0, 0, 0, 0, 0));

        size_t max = lookup_table.size();
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < max; i++) {
            const double t = i * max_t / table_size;
            lookup_table[i].t = t * 1.0;
            lookup_table[i].C1_0 = A * A * pow(sin(a * t + sigma), 2) +
                                   B * B * pow(sin(b * t), 2);
            lookup_table[i].C1_x1 = -2 * A * sin(a * t + sigma);
            lookup_table[i].C1_y1 = -2 * B * sin(b * t);
            lookup_table[i].C2_0 = 2 * A * A * a * cos(a * t + sigma) *
                                   sin(a * t + sigma) +
                                   2 * B * B * b * cos(b * t) * sin(b * t);
            lookup_table[i].C2_x1 = -2 * A * a * cos(a * t + sigma);
            lookup_table[i].C2_y1 = -2 * B * b * cos(b * t);
        }
    }

    rose_dist::rose_dist(const wave &w, const size_t table_size,
                         const double wav_sz,
                         const int n, const int d) : distance_wave(w,
                                                                   table_size,
                                                                   wav_sz) {

        /*rho=1 if n*d is odd, rho=2 if n*d is even*/
        /*ref: http://www.lmtsd.org/cms/lib/PA01000427/Centricity/Domain/116/Polar%20Roses.pdf*/
        double rho = ((n * d) % 2) ? 1.0 : 2.0;
        /*1.01 to account for rounding errors on teh max value*/
        max_t = PI * d * rho * 1.01;
        const double k = (double) (n) / (double) (d);

        lookup_table.reserve(table_size);
        lookup_table.assign(table_size, cached_value(0, 0, 0, 0, 0, 0, 0));
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < lookup_table.size(); i++) {
            const double t = i * max_t / table_size;
            lookup_table[i].t = t * 1.0;
            lookup_table[i].C1_0 = pow(cos(k * t), 2) * pow(cos(t), 2) +
                                   pow(cos(k * t), 2) * pow(sin(t), 2);
            lookup_table[i].C1_x1 = -2 * cos(k * t) * cos(t);
            lookup_table[i].C1_y1 = -2 * cos(k * t) * sin(t);
            lookup_table[i].C2_0 =
                    -2 * k * cos(k * t) * pow(cos(t), 2) * sin(k * t) -
                    2 * k * cos(k * t) * sin(k * t) * pow(sin(t), 2);
            lookup_table[i].C2_x1 =
                    2 * k * cos(t) * sin(k * t) + 2 * cos(k * t) * sin(t);
            lookup_table[i].C2_y1 =
                    2 * k * sin(k * t) * sin(t) - 2 * cos(k * t) * cos(t);
        }

        /*determine interval width*/
        /*PI/(3*max_t) determined by experimentation*/
        if (n < d) {
            wid = (size_t) (table_size * PI / (3 * max_t));
        } else {
            wid = (size_t) (table_size * PI / (6 * rho * max_t));
        }

    }
}

image_utils::wave::wave(const std::string &spec) : data(nullptr),
                                                   type(NOOP) {
    if (startswith("sine", spec)) {
        type = SINE;
    } else if (startswith("sawtooth", spec)) {
        type = SAWTOOTH;
    } else if (startswith("triangle", spec)) {
        type = TRIANGLE;
    }else if (startswith("square", spec)){
        type = SQUARE;
    } else if (startswith("fourier_square:", spec)) {
        type = FOURIER_SQUARE;
        const size_t spec_begin_length = std::strlen("fourier_square:");
        /*yes, store integer in the pointer*/
        data = (void *) std::stoull(spec.substr(spec_begin_length));
    } else if (startswith("noop", spec)) {
        type = NOOP;
    }
}

double image_utils::wave::operator()(const double &x) const {
    switch (type) {
        case NOOP:
            return x;
        case SINE:
            return 0.5 + std::sin(2.0 * PI * x) / 2.0;
        case TRIANGLE: {
            /*fix out-of-range values*/
            double x2 = std::fabs(std::fmod(x, 1.0));
            if (x2 < 0.5) {
                return 2.0 * x2;
            } else {
                return 2.0 - 2.0 * x2;
            }
        }
        case SAWTOOTH:
            return std::fabs(std::fmod(x * 2, 1.0));
        case SQUARE:
            if (std::fabs(std::fmod(x, 1.0)) < 0.5L) {
                return 0;
            } else {
                return 1.0;
            }
        case FOURIER_SQUARE: {
            double result = 0.0;
            /*get the size_t back from our pointer*/
            for (size_t i = 1; i < ((size_t) data); i++) {
                result += sin((2.0 * i - 1.0) * 2.0 * PI * x) / (2.0 * i - 1.0);
            }
            /*divide by max value on range*/
            return (0.5 + result * (2.0 / PI)) / 1.13661977236758;
        }
        default:
            return x;
    }
}

