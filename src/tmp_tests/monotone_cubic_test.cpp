// (c) Copyright 2016 Josh Wright
#include "colormaps.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    colormap *colormap1 = new colormap_interpolant(
            {
                    {0.0,    {0,   7,   100}},
                    {0.16,   {32,  107, 203}},
                    {0.42,   {237, 255, 255}},
                    {0.6425, {255, 170, 0}},
                    {0.8575, {0,   2,   0}},
                    {1.0,    {0,   7,   100}},
            }
    );

    RGB val = colormap1->get_rgb(0.15);
    std::cout << (int) val.r;
    std::cout << " ";
    std::cout << (int) val.g;
    std::cout << " ";
    std::cout << (int) val.b;
    std::cout << std::endl;

    for (double x = 0; x < 256; x += 64) {

        RGB val = colormap1->get_rgb(x / 256);

        std::cout << x;
        std::cout << "\t";
        std::cout << (int) val.r;
        std::cout << "\t";
        std::cout << (int) val.g;
        std::cout << "\t";
        std::cout << (int) val.b;
        std::cout << std::endl;
    }
    return 0;
}
