//
// Created by j0sh on 9/17/17.
//

#include "util/arg_parser.h"
#include <types.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <io.h>


using std::vector;

namespace {
    static unsigned long xor_x = 123456789, xor_y = 362436069, xor_z = 521288629;

    unsigned long xorshf96() {          //period 2^96-1
        unsigned long t;
        xor_x ^= xor_x << 16;
        xor_x ^= xor_x >> 5;
        xor_x ^= xor_x << 1;

        t = xor_x;
        xor_x = xor_y;
        xor_y = xor_z;
        xor_z = t ^ xor_x ^ xor_y;

        return xor_z;
    }

    double xorshf96_d() {
        return xorshf96() * 1.0 / std::numeric_limits<unsigned long>::max();
    }
};


int main(int argc, char const *argv[]) {
    using namespace image_utils;
    arg_parser args(argc, argv);

    const vec2 vertexes[] = {
            vec2{0, 0},
            vec2{0, 1},
            vec2{1, 0},
            vec2{1, 1},
//            vec2{0.5, 1},
//            vec2{0.5, 0},
    };
    const size_t n_vertexes = sizeof(vertexes) / sizeof(vertexes[0]);

    const auto x = args.read<size_t>("x", 500);
    const auto y = args.read<size_t>("y", 500);
    const auto N = x * y;

    image_RGB grid(x, y, RGB{255, 255, 255});

    vector<vec2> points;
    vector<vec2> last_vertexes;
    // start toward all 4 vertexes
    for (auto v : vertexes) {
        auto point = vec2{0.5, 0.5};
        points.push_back(point + (v - point) / 2);
        last_vertexes.push_back(v);
    }

    // iterate points
    auto iter = args.read<size_t>("iter", 3);
    for (size_t __i = 0; __i <= iter; ++__i) {

        vector<vec2> next_points(points.size() * (n_vertexes - 1));
        vector<vec2> next_last_vertexes(last_vertexes.size() * (n_vertexes - 1));

        std::cout << "iter: " << __i << std::endl;

#pragma omp parallel for private(xor_x, xor_y, xor_z)
        for (size_t i = 0; i < points.size(); i++) {

            vec2 point = points[i];
            vec2 last_vertex = last_vertexes[i];
            vector<vec2> tmp_points;
            vector<vec2> tmp_last_vertexes;

            for (int j = 0; j < n_vertexes; ++j) {
                if (last_vertex != vertexes[j]) {
                    tmp_points.push_back(point + (vertexes[j] - point) / 2);
                    tmp_last_vertexes.push_back(vertexes[j]);
                }
            }

            for (size_t j = 0; j < (n_vertexes - 1); ++j) {
                next_points[(n_vertexes-1) * i + j] = tmp_points[j];
            }
            for (size_t j = 0; j < (n_vertexes - 1); ++j) {
                next_last_vertexes[(n_vertexes-1) * i + j] = tmp_last_vertexes[j];
            }
        }
        points = std::move(next_points);
        last_vertexes = std::move(next_last_vertexes);
    }


    // rasterize points
#pragma omp parallel for
    for (size_t i = 0; i < points.size(); i++) {
        vec2 point = points[i];
        if (std::isnan(point[0]) || std::isnan(point[1])) {
            continue;
        }

        auto x = static_cast<const size_t>(point[0] * grid.x());
        auto y = static_cast<const size_t>(point[1] * grid.y());
        if (x >= grid.x()) { --x; }
        if (y >= grid.y()) { --y; }

//        std::cout << point[0] << ", " << point[1] << std::endl;

        grid(x, y) = RGB{0, 0, 0};
    }


    std::string outfile = args.read<std::string>("output", "output.png");
    write_image(grid, outfile);
}
