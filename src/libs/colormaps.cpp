// (c) Copyright 2015 Josh Wright
#include "colormaps.h"
#include "../external_libs/csv.h"

namespace image_utils {

    RGB colormap_basic_hot(long double x) {
        /* expects 0 <= x <= 1 */
        x *= 255;
        RGB pix;
        /* red */
        if (x > 94) {
            pix.r = 0xff;
        } else {
            pix.r = (unsigned char) (51.0 * x / 19.0);
        }

        /* green */
        if (x > 190) {
            pix.g = 0xff;
        } else if (x > 95) {
            pix.g = (unsigned char) (85.0 * x / 32.0 - 8075.0 / 32.0);
        } else {
            pix.g = 0;
        }

        /* blue */
        if (x > 191) {
            pix.b = (unsigned char) (255.0 * x / 64.0 - 48705.0 / 64.0);
        } else {
            pix.b = 0;
        }
        return pix;
    }

    RGB colormap_grayscale(long double x) {
        RGB pix;
        pix.r = (unsigned char) (256 * x);
        pix.g = pix.r;
        pix.b = pix.r;
        return pix;
    }

    long double *read_csv_colormap(std::string filename) {
        const int colormap_size = 256;
        const int colormap_channels = 3;
        long double *color_data = new long double[colormap_channels * colormap_size];
        io::CSVReader<3, io::trim_chars<>, io::double_quote_escape<',', '"'>> csv(filename);
        for (int i = 0; i < colormap_size; i += 3) {
            csv.read_row(color_data[i], color_data[i + 1], color_data[i + 2]);
        }
        return color_data;
    }

    void grayscale_to_rgb(matrix<long double> &in_double, image_RGB &out_rgb, colormap_f fun)  {

        if (in_double.x() != out_rgb.x() || in_double.y() != out_rgb.y()) {
            throw std::runtime_error("Image dimensions must be the same!");
        }

        long double *d = in_double.begin();
        RGB *px = out_rgb.begin();
        while (d < in_double.end()) {
            *px = fun(*d);
            ++d;
            ++px;
        }
    }

}