// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_ANIMATION_H
#define IMAGE_STUFF_FRACTAL_ANIMATION_H

#include "io.h"
#include "types.h"
#include "util/debug.h"
#include "util/matrix.h"
#include <iomanip>
#include <memory>
#include <omp.h>

namespace image_utils {

using std::shared_ptr;
using std::make_shared;

struct worker;
struct animation;
typedef shared_ptr<worker> worker_ref;
typedef shared_ptr<animation> animation_ref;

struct worker {
    worker();

    virtual void render(double t) = 0;

    virtual image_RGB &get_color_image() = 0;
};

struct animation {
    virtual worker_ref get_worker() = 0;
    virtual ~animation();
};

class fractal_animator {
protected:
    std::vector<worker_ref> threads;
    animation_ref animation;

public:
    std::string out_filename_prefix = "out_frame_";
    std::string output_folder = "fractal_frames";
    size_t n_frames = 200;
    size_t progress = 0;

    fractal_animator(animation_ref animation);

    void run();
};
};
#endif //IMAGE_STUFF_FRACTAL_ANIMATION_H
