// (c) Copyright 2016 Josh Wright

#include <iostream>
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/generators.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    {
        wave *w = new wave_square();
        double test_values[][2] = {
                {0,0},
                {0.4, 0},
                {0.5,1},
                {0.7,1},
                {0.999,1},
                {1,0}
        };
        for (double *d : test_values) {
            test((*w)(d[0]) == d[1], "square wave");
        }
    }
    {
        wave *w = new wave_triangle();
        double test_values[][2] = {
                {0,0},
                {0.375,2*0.375},
                {0.25,0.5},
                {0.5,1},
                {0.75,0.5},
                {1,0}
        };
        for (double *d : test_values) {
            test((*w)(d[0]) == d[1], "triangle wave");
            if ((*w)(d[0]) != d[1]) {
                debug_log((*w)(d[0]));
                debug_log(d[1]);
            }
        }
    }
    {
        wave *w = new wave_sawtooth();
        double test_values[][2] = {
                {0,0},
                {0.1,0.1},
                {0.9,0.9},
                {4.1,0.1}
        };
        for (double *d : test_values) {
            test(std::fabs((*w)(d[0]) - d[1]) < 0.0001, "sawtooth wave");
            if (std::fabs((*w)(d[0]) - d[1]) > 0.0001) {
                debug_log((*w)(d[0]));
                debug_log(d[1]);
            }
        }
    }
}
