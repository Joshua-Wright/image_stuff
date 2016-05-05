// (c) Copyright 2015 Josh Wright
#pragma once

#include "types.h"

namespace image_utils {

    matrix<containers::vect<double, 3>> gaussian_blur(const matrix<containers::vect<double, 3>> &in);
}