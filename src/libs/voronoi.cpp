//
// Created by j0sh on 5/5/16.
//
#include <algorithm>
#include <iostream>
#include <random>
#include "debug.h"
#include "voronoi.h"

namespace image_utils {
    double manhattan_dist(const vect &a, const vect &b) {
        /*2d only*/
        return std::fabs(a[0] - b[0]) + std::fabs(a[1] - b[1]);
    }


    voronoi_animation::voronoi_animation(const size_t x,
                                         const size_t y,
                                         const size_t n_points) :
            points(n_points), image_data(x, y) {
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

    void voronoi_animation::fill_pixels() {
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < image_data.x(); ++x) {
            for (size_t y = 0; y < image_data.y(); ++y) {
                vect cur{x, y};
                voronoi_point min_point = points.front();
                /*TODO: distance type option*/
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

    void voronoi_animation::advance(const double dt) {
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < points.size(); i++) {
            points[i].pos = points[i].pos + points[i].velocity * dt;
        }
        fill_pixels();
    }

    image_RGB voronoi_filter(const image_RGB &in, const size_t n_points,
                             const distance_type dist) {
        std::mt19937_64 gen((unsigned long) time(nullptr));
        std::uniform_int_distribution<size_t> d_posx(0, in.x() - 1);
        std::uniform_int_distribution<size_t> d_posy(0, in.y() - 1);
        image_RGB out(in.x(), in.y());
        typedef containers::vect<size_t, 2> vect;
        std::vector<vect> points(n_points);
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < n_points; i++) {
            points[i][0] = d_posx(gen);
            points[i][1] = d_posy(gen);
        }
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < in.x(); x++) {
            for (size_t y = 0; y < in.y(); y++) {
                vect cur{x, y};
                vect min_point = points.front();
                double min_dist = in.size(); /*default to large value*/

                for (size_t i = 0; i < points.size(); i++) {
                    double new_dist;
                    switch (dist) {
                        default:
                        case EUCLIDIAN:
                            new_dist = cur.dist2(points[i]);
                            break;
                        case MANHATTAN:
                            new_dist = manhattan_dist(cur, points[i]);
                            break;
                    }
                    if (new_dist < min_dist) {
                        min_dist = new_dist;
                        min_point = points[i];
                    }
                }
                out(x, y) = in(min_point[0], min_point[1]);
            }
        }
        return out;
    }

}