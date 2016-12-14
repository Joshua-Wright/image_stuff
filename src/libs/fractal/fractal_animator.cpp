// (c) Copyright 2016 Josh Wright
#include "fractal_animator.h"

namespace image_utils {

    fractal_animator::fractal_animator(animation_ref animation) : animation(animation) {
        threads.reserve((unsigned long) omp_get_max_threads());
        for (int i = 0; i < omp_get_max_threads(); i++) {
            threads.push_back(animation->get_worker());
        }
    }

    void fractal_animator::run() {
//        TODO log metadata about each frame
#pragma omp parallel for schedule(static,1)
        for (size_t i = progress; i < n_frames; i++) {
            const double t = i * 1.0 / n_frames;
            worker_ref &worker = threads[omp_get_thread_num()];
            worker->render(t);

            std::stringstream output;
            output << output_folder << out_filename_prefix << std::setfill('0') << std::setw(5) << i << ".png";

            write_image(worker->get_color_image(), output.str());

            std::cout << "rendered: \t" << progress << "\t/" << n_frames << std::endl;
            ++progress;
        }

        std::cout << "Done! Render using:" << std::endl;
        std::cout << "ffmpeg -framerate 60 -i "
                  << output_folder << "out_frame_%05d.png "
                  << output_folder << "output.mp4" << std::endl;
    }

    worker::worker() {}

    animation::~animation() {}
};