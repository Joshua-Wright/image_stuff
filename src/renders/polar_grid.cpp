// (c) Copyright 2015 Josh Wright
#include <vector>
#include <string>
#include <iostream>
#include "../libs/colormaps.h"
#include "../libs/generators.h"
#include "../libs/io.h"

/*easy pre-defined parameters for consistent debugging*/
//#define DEBUG 1

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using image_utils::matrix;

#ifndef DEBUG
    if (argc < 4) {
        /*if not run with enough arguments, pring usage*/
        std::cout << argv[0];
        std::cout << " <output filename>";
        std::cout << " <image size>";
        std::cout << " <distance multiplier>";
        std::cout << " <theta multiplier>";
        std::cout << std::endl;
        return 1;
    }
    /*setup command-line parameters*/
    std::string output(argv[1]);
    const size_t z = std::strtoull(argv[2], NULL, 10);
    const long double mul_dist = std::strtold(argv[3], NULL);
    const long double mul_theta = std::strtold(argv[4], NULL);
#else
    std::string output("/home/j0sh/Dropbox/code/Cpp/image_stuff/build/out.png");
    const size_t z = 1000;
    const long double mul_dist = 6;
    const long double mul_theta = 20;
#endif

    /*allocate a grid for the math*/
    matrix<long double> grid(z, z);

    /*fill the grid*/
    /*TODO: command-line option for wave type*/
    image_fill_circle_grid(grid, mul_theta, mul_dist);
    /*write the image*/
    color_write_image(grid, &colormap_basic_hot, output);
}
