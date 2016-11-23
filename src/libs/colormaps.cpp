// (c) Copyright 2016 Josh Wright
#include <iterator>
#include <map>
#include "colormaps.h"
#include "util/matrix.h"

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

    colormap_basic_hot colormap_basic_hot::map = colormap_basic_hot();

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
        return (unsigned char) std::max(256 * (1 - pow(3 * (x - 1.5 / 2.0), 2)), 0.0);
    }

    RGB colormap_offset_waves::get_rgb(const double x) const {
        RGB pix;
        pix.r = (unsigned char) (255 * (*w)(x));
        pix.g = (unsigned char) (255 * (*w)(x + 1.0 / 3.0));
        pix.b = (unsigned char) (255 * (*w)(x + 2.0 / 3.0));
        return pix;
    }

    RGB colormap_gradient::get_rgb(const double x) const {
        double t = x * max_t; /*we need to modify this, so copy it*/
        /*assume that 0<=x<1*/
        size_t idx = 0;

        /*find the line where this color is*/
        while (t > l[idx]) {
            t -= l[idx];
            ++idx;
        }

        /*find the color*/
        containers::vect<double, 3> color = (t / l[idx]) * n[idx] + p[idx];
        RGB out;
        out.r = (unsigned char) color[0];
        out.g = (unsigned char) color[1];
        out.b = (unsigned char) color[2];
        return out;
    }

    colormap_gradient::colormap_gradient(const double &r, const double &g, const double &b) {
        containers::vect<double, 3> cur{r, g, b};
        p.push_back(cur);
    }

    colormap_gradient &colormap_gradient::add_color(const double &r, const double &g, const double &b,
                                                    const double &t) {

        containers::vect<double, 3> cur{r, g, b};
        n.push_back(cur - p.back());
        p.push_back(cur);
        l.push_back(t);
        max_t = std::accumulate(l.begin(), l.end(), 0.0);
        return *this;
    }


    colormap_gradient colormap_gradient::blue_yellow_gradient = colormap_gradient
            (0, 7, 100)
            .add_color(32, 107, 203, 0.16)
            .add_color(237, 255, 255, 0.42)
            .add_color(255, 170, 0, 0.625)
            .add_color(0, 2, 0, 0.8575)
            .add_color(0, 7, 100, 1);

    colormap_3d_cosine::colormap_3d_cosine(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d) : a(a), b(b), c(c), d(d) {}

    /* this is just to make writing out get_rgb() easier */
    vec3 cos(vec3 in) {
        return {
                cos(in[0]),
                cos(in[1]),
                cos(in[2])
        };
    }

    RGB colormap_3d_cosine::get_rgb(const double x) const {
        vec3 color = 256.0 * (a + b * cos(c * x * 64 + d));
        return {
                (unsigned char) color[0],
                (unsigned char) color[1],
                (unsigned char) color[2]
        };
    }

    colormap_3d_cosine colormap_3d_cosine::blue_yellow = colormap_3d_cosine(
            {0.5, 0.5, 0.5},
            {0.5, 0.5, 0.5},
            {0.15, 0.15, 0.15},
            {3.0, 3.6, 4.0}
    );

    colormap_simple_gradient::colormap_simple_gradient(vec3 start, vec3 end) : start(start), end(end) {}

    RGB colormap_simple_gradient::get_rgb(const double x) const {
        vec3 color = start + x * (end - start);
        return {
                (unsigned char) color[0],
                (unsigned char) color[1],
                (unsigned char) color[2]
        };
    }

    colormap_simple_gradient colormap_simple_gradient::hsv_ish = colormap_simple_gradient({0, 0, 255}, {255, 0, 0});

    RGB colormap_rainbow::get_rgb(const double x) const {
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

    colormap_rainbow colormap_rainbow::map = colormap_rainbow();


    const std::map<std::string, colormap *> default_colormaps{
            {"hot",     &colormap_basic_hot::map},
            {"rainbow", &colormap_rainbow::map},
    };

    colormap *read_colormap_from_string(const std::string &spec) {
        std::istringstream iss(spec);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{}};
        auto it = default_colormaps.find(tokens[0]);
        if (it != default_colormaps.end()) {
            return it->second;
        }

        // todo other colormaps

        // default
        return &colormap_basic_hot::map;
    }
}