// (c) Copyright 2016 Josh Wright
#pragma once

#ifndef IMAGE_UTILS_GENERATORS
#define IMAGE_UTILS_GENERATORS

#include <iostream>
#include <vector>
#include "types.h"

namespace image_utils {

    /////////////////////
    // wave generators //
    /////////////////////

    class wave {

        void *data;
        enum wave_type {
            NOOP, SINE, TRIANGLE, SAWTOOTH, SQUARE, FOURIER_SQUARE
        };
        wave_type type;

    public:
        wave(const std::string &spec);

        double operator()(const double &x) const;
    };


    class wave_2d {
    public:
        virtual double operator()(const double &x,
                                  const double &y) const = 0;

        virtual ~wave_2d() { };
    };

    class distance_wave : public wave_2d {

        const wave w;
        double wave_size;

        size_t _find_min(size_t left,
                         size_t right,
                         const double &x,
                         const double &y) const;

    public:
        struct cached_value {

            /*constants needed in general*/
            double t;

            /*constants needed for distance*/
            double C1_0;
            double C1_x1;
            double C1_y1;

            /*constants needed for derivative of distance*/
            double C2_0;
            double C2_x1;
            double C2_y1;

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

    protected:
        /*these values must be provided by the subclass*/
        double offset;
        double max_t;
        size_t wid;

        std::vector<cached_value> lookup_table;

    public:
        distance_wave(const wave &_w, const size_t table_size,
                      const double wave_size);

        void set_offset(const double x);

        virtual double operator()(const double &x,
                                  const double &y) const;

        virtual ~distance_wave();
    };

    class rose_dist : public distance_wave {
    public:
        rose_dist(const wave &w, const size_t table_size,
                  const double wav_sz,
                  const int n, const int d);
    };

    class dist_lissajous : public distance_wave {
    public:
        dist_lissajous(const wave &w, const size_t table_size,
                       const double wave_size,
                       const double A, const double B,
                       const double a, const double b,
                       const double sigma);
    };


    /////////////
    // fillers //
    /////////////
    void image_fill_concentric_waves(matrix<double> &grid,
                                     const double &mul,
                                     const wave &wave_func);

    void image_fill_pointing_out(matrix<double> &grid,
                                 const double &mul,
                                 const wave &wave_func);

    /*
     * theta_mul: larger => more angular ripples (each ripple is smaller)
     * dist_mult: larger => more radial ripples (each ripple is smaller)
     */
    void image_fill_circle_grid(matrix<double> &grid,
                                const double &theta_mul,
                                const double &dist_mul,
                                const wave &wave_dist,
                                const wave &wave_theta);

    void image_fill_apply_wave_to_dist(const matrix<double> &in,
                                       matrix<double> &out, const wave &w,
                                       const double offset);

    void image_fill_2d_wave(matrix<double> &grid, wave_2d *w_2d);


}


#endif /*IMAGE_UTILS_GENERATORS*/