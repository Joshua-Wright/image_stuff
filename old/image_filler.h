//
// Created by j0sh on 11/10/15.
//

#ifndef IMAGE_STUFF_IMAGE_FILLER_H
#define IMAGE_STUFF_IMAGE_FILLER_H

#include <functional>
#include "image_utils.h"

typedef unsigned long long ullong;


class image_filler {
    int n_threads;
public:
    image_filler(const int n_threads);

    void run(std::function<void(int x, int y)> &func, matrix<double> &image, const ullong max_idx);
};


#endif //IMAGE_STUFF_IMAGE_FILLER_H
