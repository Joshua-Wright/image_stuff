//
// Created by j0sh on 11/15/15.
//

#ifndef IMAGE_STUFF_TYPES_H
#define IMAGE_STUFF_TYPES_H
namespace image_utils {
    using std::sqrt;
    using std::sin;
    using std::cos;

    const long double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482L;
    const long double Pi = PI;
    const long double pi = PI;

    struct grayscale {
        unsigned char g;
    };

    struct RGB {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct RGBA : RGB {
        unsigned char a;
    };

    struct grayscaled : grayscale { // d for defined
        bool d;
    };

    struct RGBd : RGB { // d for defined
        bool d;
    };

    struct RGBAd : RGBA { // d for defined
        bool d;
    };

    typedef matrix<grayscale> image_gs;
    typedef matrix<RGB> image_RGB;
    typedef matrix<RGBA> image_RGBA;
    typedef matrix<RGBd> image_RGBd;
    typedef matrix<RGBAd> image_RGBAd;
    typedef matrix<grayscaled> image_gsd;
    typedef RGB colormap[256];

    template<typename T>
    struct functor_class {
        virtual T operator()(const T&) {}
    };

}
#endif //IMAGE_STUFF_TYPES_H
