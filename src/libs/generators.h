// (c) Copyright 2016 Josh Wright
#pragma once

#ifndef IMAGE_UTILS_GENERATORS
#define IMAGE_UTILS_GENERATORS

#include <iostream>
#include "types.h"
//#include "debug.h"

namespace image_utils {

    /////////////////////
    // wave generators //
    /////////////////////
    class wave {
    public:
        virtual double operator()(const double &x) const = 0;

        virtual ~wave() { };
    };

    class wave_triangle : public wave {
    public:
        virtual double operator()(const double &x) const;
    };

    class wave_sine : public wave {
    public:
        virtual double operator()(const double &x) const;
    };

    class wave_square : public wave {
    public:
        virtual double operator()(const double &x) const;
    };

    class wave_sawtooth : public wave {
    public:
        virtual double operator()(const double &x) const;
    };

    class wave_fourier_square : public wave {
        size_t n;
    public:
        wave_fourier_square(size_t _n) : n(_n) { }

        wave_fourier_square(const std::string &spec);

        virtual double operator()(const double &x) const;
    };

    class wave_noop : public wave {
        virtual double operator()(
                const double &x) const { return x; };
    };

    wave *parse_wave_spec(const std::string &spec);

    class wave_2d {
    public:
        virtual double operator()(const double &x,
                                  const double &y) const = 0;

        virtual ~wave_2d() { };
    };

    class distance_wave : public wave_2d {

        wave *w;
        double wave_size;

        size_t _find_min(size_t left, size_t right,
                         const double &x,
                         const double &y) const;

    protected:
        /*these values must be provided by the subclass*/
        double offset;
        double max_t;
        size_t wid;

        struct cached_value {

            /*constants needed in general*/
            const double t;

            /*constants needed for distance*/
            const double C1_0;
            const double C1_x1;
            const double C1_y1;

            /*constants needed for derivative of distance*/
            const double C2_0;
            const double C2_x1;
            const double C2_y1;

            cached_value(const double _t,
                         const double _C1_0,
                         const double _C1_x1,
                         const double _C1_y1,
                         const double _C2_0,
                         const double _C2_x1,
                         const double _C2_y1) : t(_t),
                                                C1_0(_C1_0),
                                                C1_x1(_C1_x1),
                                                C1_y1(_C1_y1),
                                                C2_0(_C2_0),
                                                C2_x1(_C2_x1),
                                                C2_y1(_C2_y1) { }

            double dist2(const double x, const double y) const {
                return C1_0
                       + C1_x1 * x + x * x
                       + C1_y1 * y + y * y;
            }

            double dist(const double x, const double y) const {
                return std::sqrt(dist2(x, y));
            }

            double diff(const double x,
                        const double y) const {
                return C2_0 + C2_x1 * x + C2_y1 * y;
            }
        };

        std::vector<cached_value> lookup_table;

    public:
        distance_wave(wave *_w, const size_t table_size,
                      const double wave_size);

        void set_offset(const double x);

        virtual double operator()(const double &x,
                                  const double &y) const;

        virtual ~distance_wave();

    };

    class rose_dist : public distance_wave {
    public:
        rose_dist(wave *w, const size_t table_size,
                  const double wav_sz,
                  const int n,
                  const int d) : distance_wave(w, table_size, wav_sz) {

            /*rho=1 if n*d is odd, rho=2 if n*d is even*/
            /*ref: http://www.lmtsd.org/cms/lib/PA01000427/Centricity/Domain/116/Polar%20Roses.pdf*/
            double rho = ((n * d) % 2) ? 1.0 : 2.0;
            /*1.01 to account for rounding errors on teh max value*/
            max_t = PI * d * rho * 1.01;

            const double k = (double) (n) / (double) (d);
            for (double t = 0; t <= max_t; t += max_t / table_size) {
                lookup_table.emplace_back(t * 1.0,
                                          pow(cos(k * t), 2) * pow(cos(t), 2) +
                                          pow(cos(k * t), 2) * pow(sin(t), 2),
                                          -2 * cos(k * t) * cos(t),
                                          -2 * cos(k * t) * sin(t),
                                          -2 * k * cos(k * t) * pow(cos(t), 2) *
                                          sin(k * t) -
                                          2 * k * cos(k * t) * sin(k * t) *
                                          pow(sin(t), 2),
                                          2 * k * cos(t) * sin(k * t) +
                                          2 * cos(k * t) * sin(t),
                                          2 * k * sin(k * t) * sin(t) -
                                          2 * cos(k * t) * cos(t));
            }
            /*determine interval width*/
            /*PI/(3*max_t) determined by experimentation*/
            if (n < d) {
                wid = (size_t) (table_size * PI / (3 * max_t));
            } else {
                wid = (size_t) (table_size * PI / (6 * rho * max_t));
            }

        }
    };

    class dist_lissajous : public distance_wave {
    public:
        dist_lissajous(wave *w, const size_t table_size,
                       const double wave_size,
                       const double A, const double B,
                       const double a, const double b,
                       const double sigma) : distance_wave(w, table_size,
                                                           wave_size) {
            max_t = (2.01 * PI);
            /*TODO: move this to initializer*/
            wid = (size_t) (table_size * PI / (12*max_t));
//            wid = 3000;
            for (double t = 0; t <= max_t; t += max_t / table_size) {
                lookup_table.emplace_back(t * 1.0,
                                          A * A * pow(sin(a * t + sigma), 2) +
                                          B * B * pow(sin(b * t), 2),
                                          -2 * A * sin(a * t + sigma),
                                          -2 * B * sin(b * t),
                                          2 * A * A * a * cos(a * t + sigma) *
                                          sin(a * t + sigma)
                                          + 2 * B * B * b * cos(b * t) *
                                            sin(b * t),
                                          -2 * A * a * cos(a * t + sigma),
                                          -2 * B * b * cos(b * t));
            }
        }
    };


    /////////////
    // fillers //
    /////////////
    void image_fill_concentric_waves(matrix<double> &grid,
                                     const double &mul,
                                     wave *wave_func);

    void image_fill_pointing_out(matrix<double> &grid,
                                 const double &mul,
                                 wave *wave_func);

    /*
     * theta_mul: larger => more angular ripples (each ripple is smaller)
     * dist_mult: larger => more radial ripples (each ripple is smaller)
     */
    void image_fill_circle_grid(matrix<double> &grid,
                                const double &theta_mul,
                                const double &dist_mul,
                                wave *wave_dist = nullptr,
                                wave *wave_theta = nullptr);

    void image_fill_apply_wave_to_dist(const matrix<double> &in,
                                       matrix<double> &out, wave *w,
                                       const double offset);

    void image_fill_2d_wave(matrix<double> &grid, wave_2d *w_2d);


}


#endif /*IMAGE_UTILS_GENERATORS*/