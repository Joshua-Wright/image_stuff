#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <functional>
#include "libs/image_utils.h"
//#include "hot_colormap.h"
//#include "hsv_colormap.h"
#include "libs"

/*
g++ -O3 -std=c++14 -lpthread -ffast-math -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=1 libs/image_utils.cpp external_libs/lodepng.cpp libs/matrix.h libs/vctr.cpp main.cpp -o test
*/

int main() {
    using namespace std;
    using namespace image_utils;
    using image_utils::matrix;
    using image_utils::PI;

    srand((unsigned int) time(0));

//    unsigned z = 100;
//    unsigned z = 200;
//    unsigned z = 250;
//    unsigned z = 300;
    unsigned z = 500;
//    unsigned z = 1024;
//    unsigned z = 2560;
//    unsigned z = 4096;
//    unsigned x = 1080;
//    unsigned y = 1920;
//    unsigned x = 576;
//    unsigned y = 1024;
    unsigned x = z;
    unsigned y = z;
    matrix<long double> doubles(z, z);
//    matrix<long double> doubles(x, y);
//    matrix<long double> doubles(y, x);


    long double scale_const = 5;
//    long double zoom = 1;
//    long double zoom = 1.5;
    long double zoom = 1.75;
//    long double zoom = 2;

//    matrix<long double> doubles = image_fill_rose_ripples_non_recursive(x, y, 3.0L / 5.0L, 0, 5 * pi, zoom);

    image_fill_rose_ripples(doubles, 3.0L / 2.0L, 0, 4*pi);
//    image_fill_rose_ripples(doubles, 3.0L / 5.0L, 0, 5 * pi, zoom);
//    image_fill_rose_ripples_parallel(std::ref(doubles), 3.0L / 5.0L, 0, 5 * pi, zoom);
//    image_fill_rose_ripples(doubles, 6.0L / 7.0L, 0, 16 * pi);

    scale_doubles(doubles);
//    scale_middle_square(doubles, zoom);
//    doubles *= zoom * zoom;
//    doubles *= 2 * zoom;
//    doubles *= zoom * sqrt(zoom);
    for (auto &d : doubles) {
//        d = std::cos(d * 2 * pi * scale_const);
        d = std::sqrt(d);
        d = std::sin(d * 2 * pi * scale_const);
    }


    image_sanity_check(doubles);
    color_write_image(doubles, &colormap_basic_hot, "/home/j0sh/Dropbox/code/Cpp/image_stuff/out.png");

    return 0;
}