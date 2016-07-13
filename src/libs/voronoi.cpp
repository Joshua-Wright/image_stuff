//
// Created by j0sh on 5/5/16.
//
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include "debug.h"
#include "voronoi.h"

using containers::matrix;
using std::vector;
using std::list;


namespace image_utils {
    double manhattan_dist(const vect &a, const vect &b) {
        /*2d only*/
        return std::fabs(a[0] - b[0]) + std::fabs(a[1] - b[1]);
    }

    voronoi_animation::voronoi_point voronoi_animation::find_closest(const matrix<list<voronoi_animation::voronoi_point>> &buckets,
                                                                     const size_t factor, const vect_l pt) {
        voronoi_point min;
        min.pos[0] = -1;
        min.pos[1] = -1;

        double min_dist = buckets.size() * buckets.size() * factor * factor;
        min_dist *= min_dist;

        /*search the closest 9 cells*/
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                try {
                    for (voronoi_animation::voronoi_point b : buckets((const size_t) (pt[0] / factor + i),
                                                                      (const size_t) (pt[1] / factor + j))) {
                        double dist2 = pt.dist2(b.pos);
                        if (dist2 < min_dist) {
                            min = b;
                            min_dist = dist2;
                        }
                    }
                } catch (std::out_of_range e) { }
            }
        }
        if (min.pos != vect{-1, -1}) {
            return min;
        }

        /*search the rest of the cells until we find something*/
        for (int d = 2; d < buckets.x(); d++) {

            /*horizontal top section*/
            for (int i = -d; i <= d; i++) {
                for (int j : {-d, d}) {
                    try {
                        for (voronoi_animation::voronoi_point b : buckets((const size_t) (pt[0] / factor + i),
                                                                          (const size_t) (pt[1] / factor + j))) {
                            double dist2 = pt.dist2(b.pos);
                            if (dist2 < min_dist) {
                                min = b;
                                min_dist = dist2;
                            }
                        }
                    } catch (std::out_of_range e) { }
                }
            }

            /*vertical sides*/
            for (int i : {-d + 1, d - 1}) {
                for (int j = (-d + 1); j <= (d - 1); j++) {
                    try {
                        for (voronoi_animation::voronoi_point b : buckets((const size_t) (pt[0] / factor + i),
                                                                          (const size_t) (pt[1] / factor + j))) {
                            double dist2 = pt.dist2(b.pos);
                            if (dist2 < min_dist) {
                                min = b;
                                min_dist = dist2;
                            }
                        }
                    } catch (std::out_of_range e) { }
                }
            }

            if (min.pos != vect{-1, -1}) {
                return min;
            }

        }
        std::cerr << "could not find point for input " << pt[0] << "," << pt[1] << std::endl;
        std::cerr << "min dist " << min_dist << std::endl;
        return min;
    }

    vect_l find_closest(const matrix<vector<vect_l>> &buckets, const size_t factor, const vect_l pos) {
        vect_i min{-1, -1};
        double min_dist = buckets.size() * buckets.size() * factor * factor;
        min_dist *= min_dist;

        /*search the closest 9 cells*/
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                try {
                    for (vect_l b : buckets((const size_t) (pos[0] / factor + i), (const size_t) (pos[1] / factor + j))) {
                        double dist2 = pos.dist2(b);
                        if (dist2 < min_dist) {
                            min = b;
                            min_dist = dist2;
                        }
                    }
                } catch (std::out_of_range e) { }
            }
        }
        if (min != vect_i{-1, -1}) {
            return min;
        }

        /*search the rest of the cells until we find something*/
        for (int d = 2; d < buckets.x(); d++) {

            /*horizontal top section*/
            for (int i = -d; i <= d; i++) {
                for (int j : {-d, d}) {
                    try {
                        for (vect_l b : buckets((const size_t) (pos[0] / factor + i), (const size_t) (pos[1] / factor + j))) {
                            double dist2 = pos.dist2(b);
                            if (dist2 < min_dist) {
                                min = b;
                                min_dist = dist2;
                            }
                        }
                    } catch (std::out_of_range e) { }
                }
            }

            /*vertical sides*/
            for (int i : {-d + 1, d - 1}) {
                for (int j = (-d + 1); j <= (d - 1); j++) {
                    try {
                        for (vect_l b : buckets((const size_t) (pos[0] / factor + i), (const size_t) (pos[1] / factor + j))) {
                            double dist2 = pos.dist2(b);
                            if (dist2 < min_dist) {
                                min = b;
                                min_dist = dist2;
                            }
                        }
                    } catch (std::out_of_range e) { }
                }
            }

            if (min != vect_i{-1, -1}) {
                return min;
            }

        }
        std::cerr << "could not find point for input " << pos[0] << "," << pos[1] << std::endl;
        std::cerr << "min dist " << min_dist << std::endl;
        return min;
    }


    voronoi_animation::voronoi_animation(const size_t x,
                                         const size_t y,
                                         const size_t n_points) :
            points(n_points), image_data(x, y), factor(30), buckets(x / 30 + 1, y / 30 + 1) {
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

            buckets(points[i].pos[0] / factor, points[i].pos[1] / factor).push_back(points[i]);
        }
        fill_pixels();
    }

    void voronoi_animation::fill_pixels() {

//        matrix<vector<voronoi_point>> buckets(image_data.x() / factor + 1, image_data.y() / factor + 1);
//        for (auto bucket : buckets) {
//            bucket.clear();
//        }
//        for (voronoi_point p : points) {
//            buckets(p.pos[0] / factor, p.pos[1] / factor).push_back(p);
//        }

#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < image_data.x(); ++x) {
            for (size_t y = 0; y < image_data.y(); ++y) {
                image_data(x, y) = find_closest(buckets, factor, vect_l{x, y}).color;
            }
        }
    }


    void voronoi_animation::advance(const double dt) {
//#pragma omp parallel for schedule(static)
//        for (size_t i = 0; i < points.size(); i++) {
//            points[i].pos = points[i].pos + points[i].velocity * dt;
//        }
        for (size_t i = 0; i < buckets.x(); i++) {
            for (size_t j = 0; j < buckets.y(); j++) {
                for (auto it = buckets(i, j).begin(); it != buckets(i, j).end(); i++) {
                    /*update the positon*/
                    vect newpos = it->pos + it->velocity * dt;
                    it->pos = newpos;
                    /*move it around the matrix if need be*/
                    if ((i / factor) != ((size_t) newpos[0] / factor) || (j / factor) != ((size_t) newpos[1] / factor)) {
                        buckets(newpos[0] / factor, newpos[1] / factor).push_back(*it);
                        buckets(i, j).erase(it);
                    }
                }
            }
        }
        fill_pixels();
    }


    image_RGB voronoi_filter(const image_RGB &in, const size_t n_points,
                             const distance_type dist) {
        std::mt19937_64 gen((unsigned long) time(nullptr));
        std::uniform_int_distribution<size_t> d_posx(0, in.x() - 1);
        std::uniform_int_distribution<size_t> d_posy(0, in.y() - 1);
        image_RGB out(in.x(), in.y());

        const size_t factor = 30;
        matrix<vector<vect_l>> buckets(in.x() / factor + 1, in.y() / factor + 1);
        for (size_t i = 0; i < n_points; i++) {
            vect_l v{d_posx(gen), d_posy(gen)};
            buckets(v[0] / factor, v[1] / factor).push_back(v);
        }
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < in.x(); x++) {
            for (size_t y = 0; y < in.y(); y++) {
                vect_l p = find_closest(buckets, factor, vect_l{x, y});
                out(x, y) = in(p[0], p[1]);
            }
        }
        return out;
    }

}
