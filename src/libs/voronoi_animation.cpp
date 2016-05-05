//
// Created by j0sh on 5/5/16.
//
#include <algorithm>
#include "voronoi_animation.h"

namespace image_utils {
    double manhattan_dist(const vect &a, const vect &b) {
        /*2d only*/
        return std::fabs(a[0] - b[0]) + std::fabs(a[1] - b[1]);
    }
}


image_utils::voronoi_animation::voronoi_animation(const size_t x,
                              const size_t y,
                              const size_t n_points) : points(n_points),
                                                       image_data(x, y) {
    std::mt19937_64 gen((unsigned long) time(nullptr));
    std::uniform_int_distribution<> d_color(0, 255);
    std::uniform_int_distribution<> d_posx(0, (int) x);
    std::uniform_int_distribution<> d_posy(0, (int) y);
    std::uniform_real_distribution<> d_v(-1, 1);

    for (size_t i = 0; i < n_points; i++) {
        /*TODO: better random stuff*/

        points[i].color.r = (unsigned char) d_color(gen);
        points[i].color.g = (unsigned char) d_color(gen);
        points[i].color.b = (unsigned char) d_color(gen);

        points[i].pos[0] = d_posx(gen);
        points[i].pos[1] = d_posy(gen);

        points[i].velocity[0] = d_v(gen);
        points[i].velocity[1] = d_v(gen);
    }
    fill_pixels();
}

void image_utils::voronoi_animation::fill_pixels() {
#pragma omp parallel for schedule(static) collapse(2)
    for (size_t x = 0; x < image_data.x(); ++x) {
        for (size_t y = 0; y < image_data.y(); ++y) {
            vect cur{x, y};
            voronoi_point min_point = points.front();
//            double min_dist = cur.dist2(min_point.pos);
            double min_dist = manhattan_dist(cur, min_point.pos);
            for (auto &pt : points) {
//                double new_dist = cur.dist2(pt.pos);
                double new_dist = manhattan_dist(cur, pt.pos);
                if (new_dist < min_dist) {
                    min_dist = new_dist;
                    min_point = pt;
                }
            }
            image_data(x, y) = min_point.color;
        }
    }
}

void image_utils::voronoi_animation::advance(const double dt) {
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < points.size(); i++) {
        points[i].pos = points[i].pos + points[i].velocity * dt;
    }
    fill_pixels();
}





