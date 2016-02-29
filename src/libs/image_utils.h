#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>
#include <set>
#include <cmath>
#include <stdexcept>
//#include <singular/structs.h>
#include "../external_libs/lodepng.h"
//#include "cpp_containers/lib/matrix.h"
#include "colormaps.h"
#include "types.h"

namespace image_utils {

    void image_fill_dist(matrix<long double> &out_double, const matrix<bool> &defined);


    void image_fill_rose_ripples(matrix<long double> &out_double, long double n, long double a, long double b,
                                 long double zoom = 1.1);

    void image_fill_rose_ripples_parallel(matrix<long double> &out_double, long double n, long double a, long double b,
                                          long double zoom);


}
#endif //IMAGE_STUFF_MATRIX_H
