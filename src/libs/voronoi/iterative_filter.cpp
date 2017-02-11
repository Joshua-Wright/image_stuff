// (c) Copyright 2016 Josh Wright
#include <io.h>
#include <omp.h>
#include "iterative_filter.h"
#include "image_difference.h"

namespace image_utils {

    voronoi_state::voronoi_state(const image_RGB &target,
                                 const size_t x, const size_t y,
                                 const std::vector<vec_ull> &pts) :
            x(x), y(y), internal(x, y), target(target) {
        internal.calculate(pts);
    }

    double voronoi_state::fitness() const {
        return avg_sq_dist_same_size(internal, target);
    }

    voronoi_state voronoi_state::child(double normalized_deviation) {
        const double std_dev_x = x * normalized_deviation;
        const double std_dev_y = y * normalized_deviation;
        std::normal_distribution<double> dist_x(0.0, std_dev_x);
        std::normal_distribution<double> dist_y(0.0, std_dev_y);

        std::vector<vec_ull> new_points(internal.points());
        for (vec_ull &point : new_points) {
            point[0] = clamp<size_t, double>(point[0] + dist_x(gen), 0, x - 1);
            point[1] = clamp<size_t, double>(point[1] + dist_y(gen), 0, y - 1);
        }

        return voronoi_state(target, x, y, new_points);
    }

    void brute_force_iterative_filter(const image_RGB &target, const std::string &outf, const size_t n, bool dist_named) {
        double min_difference = INF;
        std::vector<vec_ull> points(n);

        std::mt19937_64 gen;
        std::uniform_int_distribution<size_t> dist_x(0, target.x()), dist_y(0, target.y());

#pragma omp parallel for
        for (int i = 0; i < omp_get_max_threads(); ++i) {
            image_RGB out_buffer(target.x(), target.y());
            voronoi voronoi1(target.x(), target.y());
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
            while (true) {
                for (vec_ull &v : points) {
                    v[0] = dist_x(gen);
                    v[1] = dist_y(gen);
                }

                voronoi1.calculate(points);

                double newdiff = avg_sq_dist_same_size(voronoi1, target);
                if (newdiff < min_difference) {
                    voronoi1.into_image_averaging(out_buffer, target);
                }

#pragma omp critical
                {
                    if (newdiff < min_difference) {
                        min_difference = newdiff;
                        if (dist_named) {
                            write_image(out_buffer, outf + std::to_string(newdiff) + ".png");
                        } else {
                            write_image(out_buffer, outf);
                        }
                        std::cout << std::endl << "new min: " << newdiff << std::endl;
                    } else {
                        // make a kind of progress bar
                        std::cout << ".";
                        std::cout.flush();
                    }
                }
            }
#pragma clang diagnostic pop
        }

    }
}
