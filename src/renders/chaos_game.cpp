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
#include <filters.h>


using std::vector;
using namespace image_utils;

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

const vec2 corners[] = {
        vec2{0, 0},
        vec2{0, 1},
        vec2{1, 1},
        vec2{1, 0},
};
const size_t n_corners = sizeof(corners) / sizeof(corners[0]);

vec2 chaos_square(size_t iter) {
    size_t last_vertex_idx = xorshf96() % n_corners;
    vec2 point{xorshf96_d(), xorshf96_d()};
    for (size_t j = 0; j < iter; ++j) {

        // pick a new vertex
        size_t new_vertex_idx = xorshf96() % n_corners;
        while (new_vertex_idx == last_vertex_idx) {
            new_vertex_idx = xorshf96() % n_corners;
        }
        last_vertex_idx = new_vertex_idx;

        // move halfway toward new vertex
        point = point + (corners[new_vertex_idx] - point) / 2;
    }
    return point;
}

vec2 chaos_diagonal(size_t iter) {
    size_t last_vertex_idx = xorshf96() % n_corners;
    vec2 point{xorshf96_d(), xorshf96_d()};
    for (size_t j = 0; j < iter; ++j) {

        // pick a new vertex
        size_t new_vertex_idx = xorshf96() % n_corners;
        while (((last_vertex_idx + 1) % n_corners) == new_vertex_idx) {
            new_vertex_idx = xorshf96() % n_corners;
        }
        last_vertex_idx = new_vertex_idx;

        // move halfway toward new corner
        point = point + (corners[new_vertex_idx] - point) / 2;
    }
    return point;
}

vec2 chaos_T(size_t iter) {
    size_t last_vertex_idx = xorshf96() % n_corners;
    vec2 point{xorshf96_d(), xorshf96_d()};
    for (size_t j = 0; j < iter; ++j) {

        // pick a new vertex
        size_t new_vertex_idx = xorshf96() % n_corners;
        while (((last_vertex_idx + 2) % n_corners) == new_vertex_idx) {
            new_vertex_idx = xorshf96() % n_corners;
        }
        last_vertex_idx = new_vertex_idx;

        // move halfway toward new corner
        point = point + (corners[new_vertex_idx] - point) / 2;
    }
    return point;
}

vec2 chaos_flower(size_t iter) {
    // FIXME
    size_t last_vertex_idx = xorshf96() % n_corners;
    size_t last_vertex_idx2 = xorshf96() % n_corners;
    vec2 point{xorshf96_d(), xorshf96_d()};
    for (size_t j = 0; j < iter; ++j) {

        // pick a new vertex
        size_t new_vertex_idx = xorshf96() % n_corners;
//        while (((last_vertex_idx2 + 1) % n_corners) == new_vertex_idx ||
//               ((last_vertex_idx2 + 3) % n_corners) == new_vertex_idx ||
//               ((last_vertex_idx + 1) % n_corners) == new_vertex_idx ||
//               ((last_vertex_idx + 3) % n_corners) == new_vertex_idx) {
        while (((last_vertex_idx + 1) % n_corners) == new_vertex_idx ||
               ((last_vertex_idx2 + 3) % n_corners) == new_vertex_idx ) {
            new_vertex_idx = xorshf96() % n_corners;
        }
        last_vertex_idx2 = last_vertex_idx;
        last_vertex_idx = new_vertex_idx;

        // move halfway toward new corner
        point = point + (corners[new_vertex_idx] - point) / 2;
    }
    return point;
}

int main(int argc, char const *argv[]) {
    arg_parser args(argc, argv);

    const auto x = args.read<size_t>("x", 500);
    const auto y = args.read<size_t>("y", 500);
    const auto N = x * y;

    vector<vec2> points(N);
    vector<vec2> last_vertexes(N);
    image_RGB grid(x, y, RGB{255, 255, 255});

#pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        points[i] = vec2{grid.z_to_x(i) * 1.0 / x, grid.z_to_y(i) * 1.0 / y};
    }

    // iterate points
    auto iter = args.read<size_t>("iter", 500);
#pragma omp parallel for private(xor_x, xor_y, xor_z)
    for (size_t i = 0; i < N; ++i) {
        points[i] = chaos_square(iter);
//        points[i] = chaos_T(iter);
//        points[i] = chaos_flower(iter);
//        points[i] = chaos_diagonal(iter);
    }


    // rasterize points
#pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        grid(static_cast<const size_t>(points[i][0] * grid.x()),
             static_cast<const size_t>(points[i][1] * grid.y())
        ) = RGB{0, 0, 0};
    }


    std::string outfile = args.read<std::string>("output", "output.png");
    write_image(grid, outfile);
//    write_image(square_to_widescreen(grid), outfile);
}
