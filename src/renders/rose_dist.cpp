// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include "../libs/generators.h"
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/colormaps.h"
#include "../libs/io.h"

#define DEBUG 0

int main(int argc, char const *argv[]) {
    using namespace image_utils;

#if !DEBUG
    if (argc < 4/*TODO: arg count*/) {
        /*0*/ std::cout << argv[0];
        /*1*/ std::cout << " <output filename>";
        /*2*/ std::cout << " <image x>";
        /*3*/ std::cout << " <image y>";
        /*4*/ std::cout << " <n>";
        /*5*/ std::cout << " <d>";
        /*6*/ std::cout << " [wave size]";
        /*7*/ std::cout << " [wave type]";
        /*8*/ std::cout << " [lookup table size]"; /*TODO with default*/
        /*9*/ /*TODO: colormap*/
        std::cout << std::endl;
        std::cout << "wave size:         default 16" << std::endl;
        std::cout << "lookup table size: 2^x, default 21" << std::endl;
        return 1;
    }
    std::string output(argv[1]);
    matrix<long double> grid(std::stoull(argv[2]), std::stoull(argv[3]));
    int n = std::stoi(argv[4]);
    int d = std::stoi(argv[5]);

    long double distance_multiplier = 16;
    if (argc >= 6) {
        distance_multiplier = std::stold(argv[6]);
    }
    wave *w = nullptr;
    if (argc >= 7) {
        w = parse_wave_spec(argv[7]);
    } else {
        w = new wave_sawtooth();
    }

    std::cout << "filling lookup table" << std::endl;
    rose_dist *rose_dist1 = new rose_dist(w, n, d, std::pow(2, 21),
                                          distance_multiplier);

#else
    /*constants for debugging*/
    std::string output("/home/j0sh/Dropbox/code/Cpp/image_stuff/build/out.png");
    rose_dist *rose_dist1 = new rose_dist(nullptr, 3, 7, std::pow(2,23), 2*8);
    size_t z = 2*1920;
    matrix<long double> grid(z, z);

#endif

    std::cout << "rendering image" << std::endl;
    image_fill_2d_wave(grid, rose_dist1);

    delete w;
    delete rose_dist1;

    color_write_image(grid, new colormap_grayscale(), output);
    return 0;
}
