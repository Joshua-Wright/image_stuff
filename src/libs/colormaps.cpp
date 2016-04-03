// (c) Copyright 2016 Josh Wright
#include "colormaps.h"
#include "csv.h"

namespace image_utils {

    RGB colormap_basic_hot::get_rgb(const double x) const {
        /* expects 0 <= x <= 1 */
        double d = x * 255;
        RGB pix;
        /* red */
        if (d > 94) {
            pix.r = 0xff;
        } else {
            pix.r = (unsigned char) (51.0 * d / 19.0);
        }

        /* green */
        if (d > 190) {
            pix.g = 0xff;
        } else if (d > 95) {
            pix.g = (unsigned char) (85.0 * d / 32.0 - 8075.0 / 32.0);
        } else {
            pix.g = 0;
        }

        /* blue */
        if (d > 191) {
            pix.b = (unsigned char) (255.0 * d / 64.0 - 48705.0 / 64.0);
        } else {
            pix.b = 0;
        }
        return pix;
    }

    RGB colormap_grayscale::get_rgb(const double x) const {
        RGB pix;
        pix.r = (unsigned char) (256 * x);
        pix.g = pix.r;
        pix.b = pix.r;
        return pix;
    }


    void grayscale_to_rgb(const matrix<double> &in_double,
                          image_RGB &out_rgb, colormap *fun) {

        if (in_double.x() != out_rgb.x() || in_double.y() != out_rgb.y()) {
            throw std::runtime_error("Image dimensions must be the same!");
        }

        const double *d = in_double.cbegin();
        RGB *px = out_rgb.begin();
        while (d < in_double.end()) {
            *px = fun->get_rgb(*d);
            ++d;
            ++px;
        }
    }

    RGB colormap_threecolor::get_rgb(const double x) const {
        RGB pix;
        pix.r = map_func(x);
        pix.g = map_func(x + 1.0 / 3.0);
        pix.b = map_func(x + 2.0 / 3.0);
        return pix;
    }

    unsigned char colormap_threecolor::map_func(double x) const {
        x = std::fabs(std::fmod(x, 1.0));
        return std::max(256 * (1 - pow(3 * (x - 1.5 / 2.0), 2)), 0.0);
    }

    RGB colormap_offset_waves::get_rgb(const double x) const {
        RGB pix;
        pix.r = 255 * (*w)(x);
        pix.g = 255 * (*w)(x + 1.0 / 3.0);
        pix.b = 255 * (*w)(x + 2.0 / 3.0);
        return pix;
    }
}