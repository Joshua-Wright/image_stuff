// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <iomanip>
#include "voronoi/voronoi.h"
#include "util/arg_parser.h"
#include "io.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    // TODO help text
    arg_parser args(argc, argv);

    auto input = args.read<std::string>("in");
    auto output = args.read<std::string>("out", input + ".out.png");
    auto n = args.read<size_t>("n", 30);

    image_RGB img_in = read_image(input);
    image_RGB img_out(img_in.x(), img_in.y());

    // random points
    std::vector<vec_ull> points(n);
    for (size_t i = 0; i < n; ++i) {
        points[i] = vec_ull{
                rand() % img_in.x(),
                rand() % img_in.y(),
        };
    }

    voronoi voronoi1(img_in.x(), img_in.y());
    voronoi1.calculate(points);
    voronoi1.into_image_averaging(img_out, img_in);

    write_image(img_out, output);

    return 0;
}
