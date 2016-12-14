// (c) Copyright 2016 Josh Wright
#include "downsampling_fractal_animation.h"

namespace image_utils {

    worker_ref downsampling_fractal_animation::get_worker() {
        downsampling_worker *w = new downsampling_worker(*this);
        return worker_ref(w);
    }

    downsampling_fractal_animation::downsampling_fractal_animation(const animation_ref &parent, size_t x, size_t y)
            : parent(parent), x(x), y(y) {}

    downsampling_fractal_animation::downsampling_worker::downsampling_worker(const downsampling_fractal_animation &p)
            :
            p(p),
            sampled_sum(p.x, p.y, vec3{0, 0, 0}),
            sampled(p.x, p.y, RGB{0, 0, 0}),
            fullres_worker(p.parent->get_worker()) {}

    image_RGB &downsampling_fractal_animation::downsampling_worker::get_color_image() { return sampled; }

    void downsampling_fractal_animation::downsampling_worker::render(double t) {
        fullres_worker->render(t);
        image_RGB &fullres = fullres_worker->get_color_image();

        size_t factor = fullres.x() / sampled_sum.x();
        size_t factor2 = factor * factor;

        std::fill(sampled_sum.begin(), sampled_sum.end(), vec3{0, 0, 0});

        for (size_t i = 0; i < fullres.x(); i++) {
            for (size_t j = 0; j < fullres.y(); j++) {
                RGB color = fullres(i, j);
                sampled_sum(i / factor, j / factor) += vec3{
                        double(color.r),
                        double(color.g),
                        double(color.b),
                };
            }
        }
        for (size_t z = 0; z < sampled_sum.size(); z++) {
            sampled(z) = RGB{
                    (unsigned char) (sampled_sum(z)[0] / factor2),
                    (unsigned char) (sampled_sum(z)[1] / factor2),
                    (unsigned char) (sampled_sum(z)[2] / factor2),
            };
        }
    }


}