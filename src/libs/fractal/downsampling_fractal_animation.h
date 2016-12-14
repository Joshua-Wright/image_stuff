// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_DOWNSAMPLING_FRACTAL_ANIMATION_H
#define IMAGE_STUFF_DOWNSAMPLING_FRACTAL_ANIMATION_H

#include "fractal_animator.h"

namespace image_utils {

    class downsampling_fractal_animation : public animation {
        animation_ref parent;
        size_t x, y;

    public:
        downsampling_fractal_animation(const animation_ref &parent, size_t x, size_t y);
    private:
        worker_ref get_worker() override;

    private:
        class downsampling_worker : public worker {
            const downsampling_fractal_animation &p;
            worker_ref fullres_worker;
            matrix<vec3> sampled_sum;
            image_RGB sampled;
        public:
            downsampling_worker(const downsampling_fractal_animation &p);
        public:
            void render(double t) override;
            image_RGB &get_color_image() override;
        };

    };

}

#endif //IMAGE_STUFF_DOWNSAMPLING_FRACTAL_ANIMATION_H
