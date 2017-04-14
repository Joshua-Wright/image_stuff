// (c) Copyright 2015 Josh Wright

#include "io.h"
#include "filters.h"

int main(int argc, char const *argv[]) {

    using namespace image_utils;

    const std::string in = "/home/j0sh/Dropbox/code/Cpp/image_stuff/build/input.png";
    matrix<RGB> in_data = read_image(in);
    matrix<vec3> in_double(in_data.x(), in_data.y());
    std::transform(in_data.begin(), in_data.end(), in_double.begin(),
                   [](const RGB &in) {
                       vec3 out;
                       out[0] = in.r;
                       out[1] = in.g;
                       out[2] = in.b;
                       return out;
                   });

    matrix<vec3> out_double = gaussian_blur(in_double);
    matrix<RGB> out_rgb(out_double.x(), out_double.y());
    std::transform(out_double.begin(), out_double.end(), out_rgb.begin(),
                   [](const vec3 &in) {
                                      RGB out;
                                      out.r = in[0];
                                      out.g = in[1];
                                      out.g = in[2];
                                      return out;
                   });

    write_image(out_rgb, in + "blured.png");
    return 0;
}
