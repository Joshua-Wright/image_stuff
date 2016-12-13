// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
#define IMAGE_STUFF_FRACTAL_SINGLETHREAD_H

#include <map>
#include "types.h"
#include "fractal_common.h"

namespace image_utils {

    class fractal_singlethread : public fractal_base {
        // class-scope variable so it's only allocated once
        rectangle_stack stack = rectangle_stack(256);
    public:

        fractal_singlethread();
        fractal_singlethread(const size_t w, const size_t h);
        fractal_singlethread(const fractal_singlethread &rhs);
        void run();
    };
}


#endif //IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
