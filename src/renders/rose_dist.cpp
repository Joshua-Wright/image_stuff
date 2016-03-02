// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include "../libs/generators.h"
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/colormaps.h"
#include "../libs/io.h"


int main(int argc, char const *argv[]) {
    using namespace image_utils;

    std::string output("/home/j0sh/Dropbox/code/Cpp/image_stuff/build/out.png");
    rose_dist rose_dist1(nullptr, 3, 7, std::pow(2,20), 2*8);
//    size_t z = 1000;
    size_t z = 500;
    matrix<long double> grid(z, z);
    image_fill_2d_wave(grid, &rose_dist1);
    color_write_image(grid, &colormap_basic_hot, output);
    return 0;
}
