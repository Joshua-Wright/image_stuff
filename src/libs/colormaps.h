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
        static colormap_basic_hot map;
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

        colormap_offset_waves(wave *w) : w(w) {}

        virtual RGB get_rgb(const double x) const;
    };

    class colormap_gradient : public colormap {

        /*p is positions in RGB color cube, n is vectors toward next p*/
        std::vector<containers::vect<double, 3>> p;
        std::vector<containers::vect<double, 3>> n;
        /*l is desired lengths of those edges*/
        std::vector<double> l;
        /*total length of gradient path*/
        double max_t;
    public:
        colormap_gradient(const double &r, const double &g, const double &b);

        colormap_gradient &add_color(const double &r, const double &g, const double &b,
                                     const double &t);

        virtual RGB get_rgb(const double x) const;

        static colormap_gradient blue_yellow_gradient;
    };

    /**
     * formula: vec3 color = 255*( a + b*cos( c*inputValue + d) )
     */
    class colormap_3d_cosine : public colormap {
        vec3 a, b, c, d;
    public:
        colormap_3d_cosine(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d);

        virtual RGB get_rgb(const double x) const override;

        static colormap_3d_cosine blue_yellow;
    };


    void grayscale_to_rgb(const matrix<double> &in_double, image_RGB &out_rgb, colormap *fun);

    class colormap_simple_gradient : public colormap {
        vec3 start;
        vec3 end;
    public:
        colormap_simple_gradient(vec3 start, vec3 end);

        static colormap_simple_gradient hsv_ish;

        virtual RGB get_rgb(const double x) const;
    };

    class colormap_rainbow : public colormap {
    public:
        virtual RGB get_rgb(const double x) const override;
        static colormap_rainbow map;
    };

    colormap *read_colormap_from_string(const std::string &spec);
}