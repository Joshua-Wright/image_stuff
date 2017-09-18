// (c) Copyright 2017 Josh Wright
#include "filters.h"
#include <numeric>

namespace image_utils {

    matrix<vec3> convolve(const matrix<vec3> &in, const matrix<double> &kernel) {
        matrix<vec3> out(in.x(), in.y());
        size_t kx = kernel.x(), ky = kernel.y();

// double kernel_sum = std::accumulate(kernel.begin(), kernel.end(), 0.0);

#pragma omp parallel for schedule(static) collapse(2)
        for (size_t i = 0; i < in.x(); ++i) {
            for (size_t j = 0; j < in.y(); ++j) {
                vec3 sum = {0, 0, 0};
                // only sum the parts from the kernel that we use
                double kernel_sum = 0;

                for (size_t ki = 0; ki < kernel.x(); ++ki) {
                    for (size_t kj = 0; kj < kernel.y(); ++kj) {
                        vec_ull pos{i + ki - kx / 2, j + kj - ky / 2};
                        // unsigned-ness takes care of the <0 case for us
                        if (pos[0] < in.x() && pos[1] < in.y()) {
                            sum += in(pos) * kernel(ki, kj);
                            kernel_sum += kernel(ki, kj);
                        }
                    }
                }
                if (kernel_sum == 0) {
                    kernel_sum = 1;
                }

                out(i, j) = sum / kernel_sum;
            }
        }
        return out;
    }

    matrix<double> kernel_gaussian({
                                           // clang-format off
                                           {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
                                           {0.00002292, 0.00078634, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                                           {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                                           {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
                                           {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                                           {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                                           {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
                                           // clang-format on
                                   });

    matrix<double> kernel_unsharp({
                                          // clang-format off
                                          {1, 4,  6,    4,  1},
                                          {4, 16, 24,   16, 4},
                                          {6, 24, -476, 24, 6},
                                          {4, 16, 24,   16, 4},
                                          {1, 4,  6,    4,  1},
                                          // clang-format on
                                  });

    matrix<double> kernel_edge_detect({
                                              // clang-format off
                                              {-1, -1, -1},
                                              {-1, 8,  -1},
                                              {-1, -1, -1},
                                              // clang-format on
                                      });

    matrix<double> kernel_edge_detect_x2({
                                                 // clang-format off
                                                 {-1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1},
                                                 {-1, -1, 8,  8,  -1, -1},
                                                 {-1, -1, 8,  8,  -1, -1},
                                                 {-1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1},
                                                 // clang-format on
                                         });

    matrix<double> kernel_edge_detect_x4({
                                                 // clang-format off
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, 8,  8,  8,  8,  -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, 8,  8,  8,  8,  -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, 8,  8,  8,  8,  -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, 8,  8,  8,  8,  -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                                 // clang-format on
                                         });

    std::unordered_map<std::string, matrix<double>> kernels{
            {"gaussian",       kernel_gaussian},
            {"unsharp",        kernel_unsharp},
            {"edge_detect",    kernel_edge_detect},
            {"edge_detect_x2", kernel_edge_detect_x2},
            {"edge_detect_x4", kernel_edge_detect_x4},
    };

    void bfs_set_color(image_RGB &image, const vec_ull &start, const RGB &to_replace, const RGB &replacement) {
        std::vector<vec_ull> to_visit(1, start);
        image(start) = replacement;

        while (!to_visit.empty()) {

            std::vector<vec_ull> to_visit_next;

            for (auto &pos : to_visit) {

                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        vec_ll newpos = pos + vec_ull{i, j};

                        if (newpos[0] < image.x() &&
                            newpos[1] < image.y() &&
                            image(newpos) == to_replace) {

                            to_visit_next.push_back(newpos);
                            image(newpos) = replacement;
                        }
                    }
                }
            }

            to_visit = std::move(to_visit_next);
        }
    }

    void color_connected_components(image_RGB &image, const RGB &to_replace, const std::vector<RGB> &replacements) {
        size_t next_replacement = 0;

        for (size_t i = 0; i < image.x(); i++) {
            for (size_t j = 0; j < image.y(); j++) {
                vec_ull pos{i, j};

                if (image(pos) == to_replace) {
//                    bfs_set_color(image, pos, to_replace, replacements[next_replacement]);
                    bfs_set_color(image, pos, to_replace, replacements[rand() % replacements.size()]);

                    ++next_replacement;
                    if (next_replacement == replacements.size()) {
                        next_replacement = 0;
                    }
                }
            }
        }
    }

    image_RGB square_to_widescreen(const image_RGB &image) {
        size_t new_width = image.x() * 16 / 9;
        image_RGB wide_image(new_width, image.y());

        size_t xb = new_width / 2 - image.x() / 2;
        size_t xc = xb + image.x();

        for (size_t i = 0; i < wide_image.x(); ++i) {
            for (size_t j = 0; j < wide_image.y(); ++j) {

                size_t i_;
                if (i < xb) {
                    i_ = image.x() - (xb - i );
                } else if (i < xb + image.x()) {
                    i_ = i - xb;
                } else {
                    i_ = i - xc;
                }

                wide_image(i, j) = image(i_, j);
            }
        }
        return wide_image;
    }
}
