// (c) Copyright 2015 Josh Wright
#include "filters.h"
#include "io.h"
#include "util/arg_parser.h"
#include "util/struct_tuple.h"

using std::string;
using namespace image_utils;
using namespace util;


int main(int argc, char const **argv) {

    arg_parser args(argc, argv);

    auto in = args.read<string>("in");
    auto out = args.read<string>("out", in + ".out.png");

    auto image = read_image(in);
    write_image(square_to_widescreen(image), out);
    return 0;
}
