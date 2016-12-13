// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_ANIMATION_ZOOM_H
#define IMAGE_STUFF_FRACTAL_ANIMATION_ZOOM_H

#include "fractal_animator.h"
#include "fractal_singlethread.h"
#include "fractal_multithread.h"

namespace image_utils {
    class fractal_animation_zoom : public animation {
    public:
        fractal_singlethread prototype;
        vec2 center = vec2{-0.743643887037151, -0.743643887037151};
        double max_zoom = 1e12;
        colormap cmap = read_colormap_from_string("hot");

        fractal_animation_zoom(size_t x, size_t y);

        worker_ref get_worker() override;

        ~fractal_animation_zoom() override;

    private:
        struct animation_worker : worker {
            const fractal_animation_zoom &p;
            fractal_singlethread fractal;
            image_RGB color_image;
            animation_worker(const fractal_animation_zoom &parent);

            void render(double t) override;

            image_RGB &get_color_image() override;
        };

    };

}

#endif //IMAGE_STUFF_FRACTAL_ANIMATION_ZOOM_H
