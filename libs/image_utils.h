#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>
#include <set>
#include <cmath>
#include <stdexcept>
#include <singular/structs.h>
#include "../external_libs/lodepng.h"
#include "matrix.h"
#include "vctr.h"
#include "types.h"

namespace image_utils {

//    template<typename gs>
//    void scale_double_to_grayscale(const matrix<long double> &in_double, matrix<gs> &out_grayscale) {
//        if (in_double.x() != out_grayscale.x() || in_double.y() != out_grayscale.y()) {
//            throw std::runtime_error("Image dimensions must be the same!");
//        }
//        auto g = std::minmax_element(in_double.begin(), in_double.end());
//        long double min = *g.first;
//        long double max = *g.second;
//        // TODO: convert this to use matrix::begin() and matrix::end() iterators
//        for (unsigned x = 0; x < in_double.x(); x++) {
//            for (unsigned y = 0; y < in_double.y(); y++) {
//                out_grayscale(x, y).g =
//                        std::numeric_limits<unsigned char>::max() * ((in_double(x, y) - min) / (max - min));
//            }
//        }
//    }

    void write_image(image_RGB &image_data, std::string output_filename);

    struct point {
        int x, y;

        point(int x, int y) : x(x), y(y) { };
    };

    void interp_gray_double_to_color(const matrix<long double> &in_double, image_RGB &out_color, RGB *colormap);


    long double point_dist(const point &p1, const point &p2);

    long double point_dist(const point &p1, const int x, const int y);

    long double min_dist(const std::vector<point> &points, const point &target, const int count = 1);

    std::tuple<long double, long double> min_dist_theta(const std::vector<point> &points, const point &target,
                                                        const int count);


    long double fourier_square_wave(long double x, int n);

    long double square_wave(long double x, long double T);


    void image_fill_circle_grid(matrix<long double> &grid, long double theta_multiplier = 20,
                                long double dist_multiplier = 100);

    void image_fill_test_fill_circles(matrix<long double> &in_double);

    void image_fill_dist(matrix<long double> &out_double, const matrix<bool> &defined);


    typedef RGB (*colormap_f)(long double);

    RGB colormap_basic_hot(long double x);

    RGB colormap_grayscale(long double x);

    void scale_doubles(matrix<long double> doubles);

    void grayscale_to_rgb(matrix<long double> &in_double, image_RGB &out_rgb, colormap_f fun);

    void image_fill_test(matrix<long double> &out_double, const matrix<bool> &defined);


    void image_fill_rose_ripples(matrix<long double> &out_double, long double n, long double a, long double b);



//    long double rose_dist(long double n, long double t, long double x, long double y);

//    long double function_min(long double (*func)(long double), const long double a, const long double b,
//                             const int steps = 100, const int rounds = 999);

}
#endif //IMAGE_STUFF_MATRIX_H
