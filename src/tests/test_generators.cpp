// (c) Copyright 2015 Josh Wright

//#include <vector>
//#include <string>
//#include <functional>
#include <iostream>
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/generators.h"
//#include "../libs/colormaps.h"
//#include "../libs/io.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    {
        wave *w = new wave_square();
        long double test_values[][2] = {
                {0,0},
                {0.4, 0},
                {0.5,1},
                {0.7,1},
                {0.999,1},
                {1,0}
        };
        for (long double *d : test_values) {
            test((*w)(d[0]) == d[1], "square wave");
        }
    }
    {
        wave *w = new wave_triangle();
        long double test_values[][2] = {
                {0,0},
                {0.375,2*0.375},
                {0.25,0.5},
                {0.5,1},
                {0.75,0.5},
                {1,0}
        };
        for (long double *d : test_values) {
            test((*w)(d[0]) == d[1], "triangle wave");
        }
    }
}
