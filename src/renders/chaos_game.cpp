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

//    const auto N = args.read<size_t>("N", 500);
    const auto x = args.read<size_t>("x", 500);
    const auto y = args.read<size_t>("y", 500);
    const auto N = x * y;

    vector<vec2> points(N);
    vector<vec2> last_vertexes(N);
    image_RGB grid(x, y, RGB{255, 255, 255});

    // generate points
//#pragma omp parallel for private(xor_x, xor_y, xor_z)
//    for (size_t i = 0; i < N; ++i) {
//        last_vertexes[i] = vertexes[xorshf96() % sizeof(vertexes) / sizeof(vertexes[0])];
//        points[i][0] = xorshf96_d();
//        points[i][1] = xorshf96_d();
//    }

#pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        points[i] = vec2{grid.z_to_x(i)*1.0/x, grid.z_to_y(i)*1.0/y};
    }

    // iterate points
    auto iter = args.read<size_t>("iter", 500);
#pragma omp parallel for private(xor_x, xor_y, xor_z)
    for (size_t i = 0; i < N; ++i) {
        vec2 last_vertex = vertexes[xorshf96() % sizeof(vertexes) / sizeof(vertexes[0])];
        vec2 point = points[i];
        for (size_t j = 0; j < iter; ++j) {

            // pick a new vertex
            vec2 new_vertex = vertexes[xorshf96() % sizeof(vertexes) / sizeof(vertexes[0])];
            while (new_vertex == last_vertex) {
//            while (new_vertex == last_vertexes[(i + 1) % 4]) {
                new_vertex = vertexes[xorshf96() % sizeof(vertexes) / sizeof(vertexes[0])];
            }
//            vec2 new_vertex{
//                    last_vertex[0] == 1 ? 0 : 1,
//                    last_vertex[1] == 1 ? 0 : 1,
//            };
            last_vertex = new_vertex;

            // move halfway toward new vertex
//            points[i] = points[i] + (new_vertex - points[i]) / 2;
            point = point + (new_vertex - point) / 2;
        }
        points[i] = point;
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
}
