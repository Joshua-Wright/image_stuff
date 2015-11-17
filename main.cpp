#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "libs/image_utils.h"
//#include "hot_colormap.h"
//#include "hsv_colormap.h"

/*
g++ -lginac -lcln -ffast-math -std=c++14 -O3 main.cpp lodepng.cpp -o make_fire_grid
 * */

int main() {
    using namespace std;
    using namespace image_utils;
    using image_utils::matrix;
    using image_utils::PI;

    srand((unsigned int) time(0));

//    unsigned z = 100;
    unsigned z = 500;
//    unsigned z = 1000;
//    unsigned z = 2560;
//    unsigned z = 4096;
    matrix<long double> doubles(z, z);
    long double scale_factor = 0.5*z;
    long double diag(point_dist(point(0, 0), point(z, z)));



//    image_fill_rose_ripples(doubles, 3.0L / 2.0L, 0, 4 * pi);
    image_fill_rose_ripples(doubles, 6.0L / 7.0L, 0, 16 * pi);

    for (auto &d : doubles) {
//        d = std::sin(z * d / scale_factor);
        d = std::sin(diag * d / 8);
    }


    image_sanity_check(doubles);
    color_write_image(doubles, &colormap_basic_hot, "/home/j0sh/Dropbox/code/Cpp/image_stuff/out.png");

    return 0;
}