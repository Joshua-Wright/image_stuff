// (c) Copyright 2015 Josh Wright
#pragma once

#include <string>
#include <unordered_map>
#include "types.h"

namespace image_utils {

matrix<vec3> convolve(const matrix<vec3> &in, const matrix<double> &kernel);

extern matrix<double> kernel_gaussian;

extern matrix<double> kernel_unsharp;
extern matrix<double> kernel_edge_detect;

extern std::unordered_map<std::string, matrix<double>> kernels;

void color_connected_components(image_RGB &image, const RGB &to_replace, const std::vector<RGB> &replacements);

}
