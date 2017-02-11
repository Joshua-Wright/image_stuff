// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <iomanip>
#include "util/arg_parser.h"
#include "io.h"
#include "voronoi/iterative_filter.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    // TODO help text
    arg_parser args(argc, argv);

    auto input = args.read<std::string>("in");
    auto output = args.read<std::string>("out", input + ".out.png");
    auto n = args.read<size_t>("n", 3000);

    image_RGB img_in = read_image(input);

    bool dist_named;
    args.read_bool(dist_named, "dist_named");

    brute_force_iterative_filter(img_in, output, n, dist_named);

}
