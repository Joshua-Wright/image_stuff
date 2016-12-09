// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_ANIMATION_ZOOM_H
#define IMAGE_STUFF_FRACTAL_ANIMATION_ZOOM_H

#include "fractal_animator.h"
#include "fractal_singlethread.h"
#include "fractal_multithread.h"

namespace image_utils {
    struct fractal_animation_zoom : animation {
        size_t x, y;
        colormap cmap;
        vec2 center;
        size_t iter;
        double max_zoom;

        struct animation_worker : worker {
            const fractal_animation_zoom &p;
            fractal_multithread fractal;
            animation_worker(const fractal_animation_zoom &parent);

            void render() override;
        };

        worker_ref get_worker() override;

        ~fractal_animation_zoom() override;

    };

}

#endif //IMAGE_STUFF_FRACTAL_ANIMATION_ZOOM_H
