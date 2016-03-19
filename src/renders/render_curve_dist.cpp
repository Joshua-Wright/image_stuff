// (c) Copyright 2015 Josh Wright

#include "../libs/curve_dist_filler.h"
#include "../libs/io.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;

    std::string output("/home/j0sh/Dropbox/code/Cpp/image_stuff/build/out.png");
    size_t z = 800;
    matrix<double> grid(z,z);

    curve *c = new curve_circle();
    curve_dist_filler(grid, c);


    colormap *map = new colormap_basic_hot();
    wave *w = new wave_sine();
    matrix<double> out(z,z);
    image_fill_apply_wave_to_dist(grid, out, w, 0);
    color_write_image(out, map, output);
    return 0;
}
