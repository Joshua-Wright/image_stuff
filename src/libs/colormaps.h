// (c) Copyright 2016 Josh Wright
#pragma once

#include <functional>
#include "types.h"
#include "cubic_interp.h"
#include "generators.h"

namespace image_utils {

    typedef std::function<RGB(double)> colormap_func;

    class colormap;

    colormap read_colormap_from_string(const std::string &spec, const size_t steps = 2048);

    void grayscale_to_rgb(const matrix<double> &in_double, image_RGB &out_rgb, const colormap_func &fun);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    class colormap {
    public:
        std::vector<RGB> color_data;
        bool black_zero = true;

        colormap();

        colormap(const std::vector<RGB> &color_data);

        RGB operator()(double x) const;

        colormap(colormap_func func, const size_t steps) : color_data(steps) {
            for (size_t i = 0; i < steps; i++) {
                color_data[i] = func(1.0 * i / steps);
            }
        }

    };

    class colormap_offset_waves {
        wave w;
    public:

        colormap_offset_waves(wave w) : w(w) {}

        RGB operator()(const double x) const;
    };


    /**
     * formula: vec3 color = 255*( a + b*cos( c*inputValue + d) )
     */
    class colormap_3d_cosine {
        vec3 a, b, c, d;
    public:
        colormap_3d_cosine(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d);

        RGB operator()(const double x) const;
    };


    class colormap_interpolant {

        cubic_interp r, g, b;

    public:

        colormap_interpolant(std::vector<std::pair<double, vec3>> xps);

        RGB operator()(const double x) const;

    };

}