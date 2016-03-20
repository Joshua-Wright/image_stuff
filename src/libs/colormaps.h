// (c) Copyright 2016 Josh Wright
#pragma once

#include "types.h"
#include "generators.h"

namespace image_utils {

    //////////////////
    // colormap ADT //
    //////////////////
    /*colormaps expect 0 <= x <= 1 */
    class colormap {
    public:
        virtual RGB get_rgb(const double x) const = 0;
    };

    class colormap_basic_hot : public colormap {
    public:
        virtual RGB get_rgb(const double d) const;
    };

    class colormap_grayscale : public colormap {

    public:
        virtual RGB get_rgb(const double x) const;
    };

    class colormap_threecolor : public colormap {
        unsigned char map_func(double x) const;

    public:
        virtual RGB get_rgb(const double x) const;
    };

    class colormap_offset_waves : public colormap {
        wave *w;
    public:

        colormap_offset_waves(wave *w) : w(w) { }

        virtual RGB get_rgb(const double x) const;
    };


    void grayscale_to_rgb(const matrix<double> &in_double, image_RGB &out_rgb,
                          colormap *fun);


}