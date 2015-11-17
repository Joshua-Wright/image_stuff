#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <limits>
#include "../external_libs/lodepng.h"
#include <ostream>
#include <iostream>
#include <functional>
#include <stdexcept>
#include "../external_libs/csv.h"
#include "image_utils.h"
#include "matrix.h"

namespace image_utils {

    using namespace std::placeholders;  // for _1, _2, _3...

    void write_image(image_RGB &image_data, std::string output_filename) {
//    std::vector<unsigned char> image_data_vector(image_data.size() * 2, 0); // TODO: make this better, since we need transparency bits also
        std::vector<unsigned char> image_data_vector;
//    for (auto &pixel : image_data) {
        for (auto pixel : image_data) {
            image_data_vector.push_back(pixel.r);
            image_data_vector.push_back(pixel.g);
            image_data_vector.push_back(pixel.b);
            image_data_vector.push_back(0xff); // opaque
        }
        lodepng::encode(output_filename, image_data_vector, image_data.x(), image_data.y());
    };

    long double min_dist(const std::vector<point> &points, const point &target, const int count) {
        long double *distances = new long double[points.size()];
        for (int i = 0; i < points.size(); i++) {
            distances[i] = point_dist(points[i], target);
        }
        std::sort(distances, distances + points.size());
        long double dist_sum = 0;
        int current_count = 1;
        for (int i = 0; i < points.size(); i++) {
            if (current_count > count) {
                break;
            }
            dist_sum += (count - current_count + 1) * distances[i];
            current_count++;
        }
        return dist_sum;
    }

    long double point_dist(const point &p1, const int x, const int y) {
        return sqrt(1.0 * (p1.x - x) * (p1.x - x) + 1.0 * (p1.y - y) * (p1.y - y));
    }

    long double point_dist(const point &p1, const point &p2) {
        return sqrt(1.0 * (p1.x - p2.x) * (p1.x - p2.x) + 1.0 * (p1.y - p2.y) * (p1.y - p2.y));
    }

    long double fourier_square_wave(long double x, int n) {
        long double result = 0.0;
        for (unsigned i = 1; i < n; i++) {
            result += sin((2 * i - 1) * 2 * 3.14159 * x) / (2 * i - 1);
        }
        return result * (4 / PI);
    }

    long double square_wave(long double x, long double T) {
        if (int(x / (T / 2)) % 2 == 0) {
            return 1.0;
        }
        return -1.0;

    }

    long double *read_csv_colormap(std::string filename) {
        const int colormap_size = 256;
        const int colormap_channels = 3;
        long double *color_data = new long double[colormap_channels * colormap_size];
        io::CSVReader<3, io::trim_chars<>, io::double_quote_escape<',', '"'>> csv(filename);
        for (int i = 0; i < colormap_size; i += 3) {
            csv.read_row(color_data[i], color_data[i + 1], color_data[i + 2]);
        }
        return color_data;
    }

    void image_fill_circle_grid(matrix<long double> &grid, long double theta_multiplier, long double dist_multiplier) {
        point middle(grid.x() / 2, grid.y() / 2);
        long double diagonal_dist = point_dist(point(0, 0), grid.x(), grid.y());
        long double theta, dx;
        for (int x = 0; x < grid.x(); x++) {
            for (int y = 0; y < grid.y(); y++) {
                dx = point_dist(middle, x, y) * dist_multiplier / diagonal_dist;
                theta = atan2(y - middle.y, x - middle.x) * theta_multiplier;
                grid(x, y) = sin(theta) + sin(dx);
            }
        }
    }

    RGB colormap_basic_hot(long double x) {
        /* expects 0 <= x <= 1 */
        x *= 255;
        RGB pix;
        /* red */
        if (x > 94) {
            pix.r = 0xff;
        } else {
            pix.r = (unsigned char) (51.0 * x / 19.0);
        }

        /* green */
        if (x > 190) {
            pix.g = 0xff;
        } else if (x > 95) {
            pix.g = (unsigned char) (85.0 * x / 32.0 - 8075.0 / 32.0);
        } else {
            pix.g = 0;
        }

        /* blue */
        if (x > 191) {
            pix.b = (unsigned char) (255.0 * x / 64.0 - 48705.0 / 64.0);
        } else {
            pix.b = 0;
        }
        return pix;
    }

    RGB colormap_grayscale(long double x) {
        RGB pix;
        pix.r = (unsigned char) (256 * x);
        pix.g = pix.r;
        pix.b = pix.r;
        return pix;
    }

    void scale_doubles(matrix<long double> doubles) {
        auto g = std::minmax_element(doubles.begin(), doubles.end());
        long double min = *g.first;
        long double max = *g.second;
        doubles -= min;
        doubles /= (max - min);
    }

    void grayscale_to_rgb(matrix<long double> &in_double, image_RGB &out_rgb, colormap_f fun) {

        if (in_double.x() != out_rgb.x() || in_double.y() != out_rgb.y()) {
            throw std::runtime_error("Image dimensions must be the same!");
        }

        long double *d = in_double.begin();
        RGB *px = out_rgb.begin();
        while (d < in_double.end()) {
            *px = fun(*d);
            ++d;
            ++px;
        }
    }

    void image_fill_test_fill_circles(matrix<long double> &in_double) {
        for (int i = 0; i < in_double.x(); i++) {
            int x0 = 0;
            int y0 = 0;
//            int x = in_double.x() - 1;
            int x = i;
            int y = 0;
            int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

            while (y <= x) {
                in_double(x + x0, y + y0) = 1;
                in_double(y + x0, x + y0) = 1;
                y++;
                if (decisionOver2 <= 0) {
                    decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
                }
                else {
                    x--;
                    decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
                }
            }
        }
    }

    long double dist2_along_line(const matrix<bool> &defined, const vctr &begin, const vctr &end) {
        vctr r = begin;
        vctr dr = (end - begin);
//        dr /= dr.mag() * 2;
        dr /= dr.mag();
        while (r.x > 0 && r.x < defined.x() && r.y > 0 && r.y < defined.y()) {
            if (defined((unsigned int) r.x, (unsigned int) r.y)) {
                return (begin - r).mag();
            }
            r += dr;
        }
        return (begin - end).mag2() * 2;
//        return INFINITY;
    }

    void image_fill_dist(matrix<long double> &out_double, const matrix<bool> &defined) {
        assert_same_size(out_double, defined);
        for (int i = 0; i < out_double.x(); i++) {
            for (int j = 0; j < out_double.y(); j++) {
//                out_double(i,j) = dist2_along_line(defined, i, j, )
            }
        }
    }

    void image_fill_test(matrix<long double> &out_double, const matrix<bool> &defined) {
        assert_same_size(out_double, defined);
        long double diagonal = out_double.x() * out_double.x() + out_double.y() * out_double.y();
        /* TODO: pre-allocate this vector */
        std::vector<vctr> edges;
        for (int i = 0; i < out_double.x(); i++) {
            edges.push_back(vctr(0, i));
            edges.push_back(vctr(i, 0));
            edges.push_back(vctr(out_double.x(), i));
            edges.push_back(vctr(i, out_double.y()));
        }
        for (int i = 0; i < out_double.x(); i++) {
            for (int j = 0; j < out_double.y(); j++) {
                long double min_dist = diagonal;
                long double dist;
                for (vctr &edge : edges) {
                    dist = dist2_along_line(defined, vctr(i, j), edge);
                    if (dist < min_dist) {
                        min_dist = dist;
                    }
                }
                out_double(i, j) = min_dist;
            }
        }
    }


    struct rose_dist : functor_class<long double> {
        long double n;
        long double x;
        long double y;

        rose_dist(long double _n, long double _x, long double _y) : n(_n), x(_x), y(_y) { };

        rose_dist(long double _n) : n(_n), x(0.0L), y(0.0L) { };

        virtual long double operator()(const long double &t) {
//            return (cos(n * t) * cos(t) - x) * (cos(n * t) * cos(t) - x) +
//                   (cos(n * t) * sin(t) - y) * (cos(n * t) * sin(t) - y);
            return pow(cos(n * t) * cos(t) - x, 2) +
                   pow(cos(n * t) * sin(t) - y, 2);
        }

        virtual long double diff(const long double &t) {
//            return -2 * (n * cos(t) * sin(n * t) + cos(n * t) * sin(t)) * (cos(n * t) * cos(t) - x) -
//                   2 * (n * sin(n * t) * sin(t) - cos(n * t) * cos(t)) * (cos(n * t) * sin(t) - y);
//            return - 2 * n * cos(n * t) * cos(t) * cos(t) * sin(n * t)
//                   - 2 * n * cos(n * t) * sin(n * t) * sin(t) * sin(t)
//                   + 2 * n * x * cos(t) * sin(n * t)
//                   + 2 * n * y * sin(n * t) * sin(t)
//                   - 2 * y * cos(n * t) * cos(t)
//                   + 2 * x * cos(n * t) * sin(t);
            long double cos_t = cos(t);
            long double sin_t = sin(t);
            long double cos_nt = cos(n * t);
            long double sin_nt = sin(n * t);
            return -2 * n * cos_nt * cos_t * cos_t * sin_nt
                   - 2 * n * cos_nt * sin_nt * sin_t * sin_t
                   + 2 * n * x * cos_t * sin_nt
                   + 2 * n * y * sin_nt * sin_t
                   - 2 * y * cos_nt * cos_t
                   + 2 * x * cos_nt * sin_t;
        }
    };

    long double function_min(functor_class<long double> *func, const long double a, const long double b,
                             const int steps = 100, const int rounds = 999) {
        if ((b - a) < sqrt(std::numeric_limits<long double>::epsilon()) || rounds <= 0) {
            return (*func)((a + b) / 2);
        }
        long double step = (b - a) / steps;
        long double l_t = a;
        long double r_t = a + step;
        long double l_diff = func->diff(l_t);
        long double r_diff = func->diff(r_t);
//        long double current_min = (*func)((a + b) / 2);
//        long double current_min = std::numeric_limits<long double>::max();
        long double current_min = (*func)(b);
        long double current_val;
        while (l_t < b) {
            if (l_diff < 0 && r_diff > 0) {
                current_val = function_min(func, l_t, r_t, steps, rounds - 1);
                if (current_val < current_min) {
                    current_min = current_val;
                }
            }
            l_t = r_t;
            l_diff = r_diff;
            r_t += step;
            r_diff = func->diff(r_t);
        }
        return current_min;
    }

    void image_fill_rose_ripples(matrix<long double> &out_double, long double n, long double a, long double b) {
        int dx = out_double.x() / 2;
        int dy = out_double.y() / 2;
        rose_dist func(n);
        for (int x = 0; x < out_double.x(); x++) {
            func.x = 2.0L * (1.0L * x - dx) / out_double.x();
            for (int y = 0; y < out_double.y(); y++) {
                func.y = 2.0L * (1.0L * y - dy) / out_double.y();
                out_double(x, y) = sqrt(function_min(&func, a, b, (const int) (b - a), 9));
            }
            if (x % (out_double.x() / 100) == 0) {
                std::cout << x << std::endl;
            }
        }
    }


    void image_sanity_check(const matrix<long double> &doubles) {
        auto min_max_tuple = std::minmax_element(doubles.begin(), doubles.end());
        long double min = *min_max_tuple.first;
        long double max = *min_max_tuple.second;
        if (min == INFINITY || min == -INFINITY || max == INFINITY || max == -INFINITY || std::isnan(min) ||
            std::isnan(max)) {
            std::cout << "infinity detected" << std::endl;
        }
        std::cout << min << " " << max << std::endl;
        if (min == 0 && max == 0) {
            std::cout << "everything is 0" << std::endl;
        } else if (min == max) {
            std::cout << "everything is equal" << std::endl;
        }
    }

    void color_write_image(matrix<long double> &doubles, colormap_f colormap, std::string output_filename) {
        /* modifies argument! */
        scale_doubles(doubles);
        image_RGB color_image(doubles.x(), doubles.y());
        grayscale_to_rgb(doubles, color_image, colormap);
        std::cout << "saving image" << std::endl;
        write_image(color_image, output_filename);

    }

}