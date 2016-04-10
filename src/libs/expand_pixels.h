// (c) Copyright 2015 Josh Wright
#pragma once

#include "types.h"

namespace image_utils {
    matrix<RGB> expand_pixels(const matrix<RGB> &in, const size_t px_size = 29);
}