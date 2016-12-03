// (c) Copyright 2016 Josh Wright
#include <iterator>
#include <map>
#include "colormaps.h"
#include "util/matrix.h"

namespace image_utils {

    RGB colormap_basic_hot(const double x) {
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

    RGB colormap_grayscale(const double x) {
        RGB pix;
        pix.r = (unsigned char) (256 * x);
        pix.g = pix.r;
        pix.b = pix.r;
        return pix;
    }


    void grayscale_to_rgb(const matrix<double> &in_double,
                          image_RGB &out_rgb, const colormap &fun) {

        if (in_double.x() != out_rgb.x() || in_double.y() != out_rgb.y()) {
            throw std::runtime_error("Image dimensions must be the same!");
        }

        const double *d = in_double.cbegin();
        RGB *px = out_rgb.begin();
        while (d < in_double.end()) {
            *px = fun.get_rgb(*d);
            ++d;
            ++px;
        }
    }

    unsigned char map_func(double x) {
        x = std::fabs(std::fmod(x, 1.0));
        return (unsigned char) std::max(256 * (1 - pow(3 * (x - 1.5 / 2.0), 2)), 0.0);
    }

    RGB colormap_threecolor(const double x) {
        RGB pix;
        pix.r = map_func(x);
        pix.g = map_func(x + 1.0 / 3.0);
        pix.b = map_func(x + 2.0 / 3.0);
        return pix;
    }

    RGB colormap_offset_waves::operator()(const double x) const {
        RGB pix;
        pix.r = (unsigned char) (255 * w(x));
        pix.g = (unsigned char) (255 * w(x + 1.0 / 3.0));
        pix.b = (unsigned char) (255 * w(x + 2.0 / 3.0));
        return pix;
    }

    colormap_3d_cosine::colormap_3d_cosine(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d) : a(a), b(b), c(c), d(d) {}

    /* this is just to make writing out get_rgb() easier */
    vec3 cos(vec3 in) {
        return {
                cos(in[0]),
                cos(in[1]),
                cos(in[2])
        };
    }

    RGB colormap_3d_cosine::operator()(const double x) const {
        vec3 color = 256.0 * (a + b * cos(c * x * 64 + d));
        return {
                (unsigned char) color[0],
                (unsigned char) color[1],
                (unsigned char) color[2]
        };
    }

    RGB colormap_rainbow(const double x) {
        // https://en.wikibooks.org/wiki/Color_Theory/Color_gradient#How_to_use_color_gradients_in_computer_programs
        // here are some my modification but the main code is the same
        // as in Witold J.Janik code

        if (x == 0) {
            return {0, 0, 0};
        }

        unsigned char R = 0, G = 0, B = 0;// byte
        int nmax = 6;// number of color bars
        double m = nmax * x;
        int n = int(m); // integer of m
        double f = m - n;  // fraction of m
        unsigned char t = int(f * 255);

        switch (n) {
            case 0: {
                R = 255;
                G = t;
                B = 0;
                break;
            };
            case 1: {
                R = 255 - t;
                G = 255;
                B = 0;
                break;
            };
            case 2: {
                R = 0;
                G = 255;
                B = t;
                break;
            };
            case 3: {
                R = 0;
                G = 255 - t;
                B = 255;
                break;
            };
            case 4: {
                R = t;
                G = 0;
                B = 255;
                break;
            };
            case 5: {
                R = 255;
                G = 0;
                B = 255 - t;
                break;
            };

        };

        return {R, G, B};
    }


    const std::map<std::string, std::function<RGB(const double)>> default_colormaps{
            {"hot",               &colormap_basic_hot},
            {"rainbow",           &colormap_rainbow},
            {"gray",              &colormap_grayscale},
            {"threecolor",        &colormap_threecolor},
            {"sine",              colormap_offset_waves(wave(wave::SINE))},
            {"cubic_blue_yellow", colormap_interpolant(
                    {
                            {0.0,    {0,   7,   100}},
                            {0.16,   {32,  107, 203}},
                            {0.42,   {237, 255, 255}},
                            {0.6425, {255, 170, 0}},
                            {0.8575, {0,   2,   0}},
                            {1.0,    {0,   7,   100}},
                    })
            },
            {"3d-cosine",         colormap_3d_cosine(
                    {0.5, 0.5, 0.5},
                    {0.5, 0.5, 0.5},
                    {0.15, 0.15, 0.15},
                    {3.0, 3.6, 4.0}
            )},
    };

    colormap read_colormap_from_string(const std::string &spec, const size_t steps) {
        std::istringstream iss(spec);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{}};
        auto it = default_colormaps.find(tokens[0]);
        if (it != default_colormaps.end()) {
            return colormap(it->second, steps);
        }

        // default
        return colormap(&colormap_basic_hot, steps);
    }

    colormap_interpolant::colormap_interpolant(std::vector<std::pair<double, vec3>> xps) {
        std::sort(xps.begin(), xps.end(), [](auto a, auto b) { return a.first < b.first; });
        std::vector<double> xs, yr, yg, yb;
        for (auto p : xps) {
            xs.push_back(p.first);
            yr.push_back(p.second[0]);
            yg.push_back(p.second[1]);
            yb.push_back(p.second[2]);
        }

        r = cubic_interp(0, 255, xs, yr);
        g = cubic_interp(0, 255, xs, yg);
        b = cubic_interp(0, 255, xs, yb);
    }

    RGB colormap_interpolant::operator()(const double x) const {
        /*these are properly clamped in the interpolant class itself*/
        return {(unsigned char) r(x), (unsigned char) g(x), (unsigned char) b(x)};
    }

    RGB colormap::get_rgb(double x) const {
        if (x < 0) {
            x = -x;
        }
        if (x >= 1) {
            x = floor(x);
        }
        return color_data[x * color_data.size()];
    }

    colormap::colormap(const std::vector<RGB> &color_data) : color_data(color_data) {}

    colormap::colormap() {}
}