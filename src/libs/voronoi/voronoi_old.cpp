//
// Created by j0sh on 5/5/16.
//
#include <algorithm>
#include <iostream>
#include <random>
#include "util/debug.h"
#include "voronoi_old.h"

#ifndef DO_GRID
#define DO_GRID 0
#endif

namespace image_utils {
    const size_t NOT_DEFINED = (const size_t) -1;


    struct rectangle {
        // bounds are inclusive
        size_t xmin, xmax, ymin, ymax;
        vec_ull corners[4];

        rectangle() {}

        rectangle(const size_t x_min, const size_t x_max,
                  const size_t y_min, const size_t y_max) : xmin(x_min), xmax(x_max),
                                                            ymin(y_min), ymax(y_max) {
            corners[0] = {x_min, y_min};
            corners[1] = {x_max, y_min};
            corners[2] = {x_min, y_max};
            corners[3] = {x_max, y_max};

        };
    };

    size_t closest_point(const vec_ull pos, const std::vector<vec_ull> &points) {
        double min_dist = points[0].dist2(pos);
        size_t min_dist_idx = 0;
        for (size_t i = 1; i < points.size(); i++) {
            if (points[i].dist2(pos) < min_dist) {
                min_dist = points[i].dist2(pos);
                min_dist_idx = i;
            }
        }
        return min_dist_idx;
    }

#if DO_GRID
    void process_rectangle(const rectangle &r, const std::vector<vec_ull> &points, matrix<size_t> &grid_indexes, matrix<size_t> &mask) {
#else

    void process_rectangle(const rectangle &r, const std::vector<vec_ull> &points, matrix<size_t> &grid_indexes) {
#endif
        containers::vect<size_t, 4> corner_indexes;
        for (size_t i = 0; i < corner_indexes.size(); i++) {
            // pre-calculate to avoid lazy evaluation skipping
            if (grid_indexes(r.corners[i][0], r.corners[i][1]) == NOT_DEFINED) {
                corner_indexes[i] = closest_point(r.corners[i], points);
                grid_indexes(r.corners[i][0], r.corners[i][1]) = corner_indexes[i];
            } else {
                corner_indexes[i] = grid_indexes(r.corners[i][0], r.corners[i][1]);
            }
        }
        bool corners_equal = corner_indexes[0] == corner_indexes[1] &&
                             corner_indexes[0] == corner_indexes[2] &&
                             corner_indexes[0] == corner_indexes[3];
        // todo improve magic number?
        if (corners_equal /*&& shortest_edge < longest_bound / 10*/) {
            size_t index_fill = grid_indexes(r.xmin, r.ymin);
            for (size_t i = r.xmin; i <= r.xmax; i++) {
                for (size_t j = r.ymin; j <= r.ymax; j++) {
                    grid_indexes(i, j) = index_fill;
                }
            }
        } else if (r.xmax - r.xmin <= 1 && r.ymax - r.ymin <= 1) {
            // rectangle has no content
        } else if ((r.xmax - r.xmin) > (r.ymax - r.ymin)) {
            // x side is longer, split along x
#if DO_GRID
            process_rectangle(rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, r.ymax), points, grid_indexes, mask);
            process_rectangle(rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, r.ymax), points, grid_indexes, mask);
#else
            process_rectangle(rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, r.ymax), points, grid_indexes);
            process_rectangle(rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, r.ymax), points, grid_indexes);
#endif
        } else /*if ((r.xmax - r.xmin) > (r.ymax - r.ymin))*/ {
            // same for y-side
#if DO_GRID
            process_rectangle(rectangle(r.xmin, r.xmax, r.ymin, (r.ymin + r.ymax) / 2), points, grid_indexes, mask);
            process_rectangle(rectangle(r.xmin, r.xmax, (r.ymin + r.ymax) / 2, r.ymax), points, grid_indexes, mask);
#else
            process_rectangle(rectangle(r.xmin, r.xmax, r.ymin, (r.ymin + r.ymax) / 2), points, grid_indexes);
            process_rectangle(rectangle(r.xmin, r.xmax, (r.ymin + r.ymax) / 2, r.ymax), points, grid_indexes);
#endif
        }
#if DO_GRID
        for (size_t i = r.xmin; i <= r.xmax; i++) {
            mask(i,r.ymin) = 0;
        }
        for (size_t j = r.ymin; j <= r.ymax;j ++) {
            mask(r.xmin, j) = 0;
        }
#endif
    }


    image_RGB fast_voronoi_filter(const image_RGB &in, const size_t n_points) {

        // generate points
#ifdef SEED
        std::mt19937_64 gen(SEED);
#else
        std::mt19937_64 gen((unsigned long) time(nullptr));
#endif
        std::uniform_int_distribution<size_t> d_posx(0, in.x() - 1);
        std::uniform_int_distribution<size_t> d_posy(0, in.y() - 1);
        std::vector<vec_ull> points(n_points);
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < n_points; i++) {
            points[i][0] = d_posx(gen);
            points[i][1] = d_posy(gen);
        }

        // run recursive algorithm
        rectangle starting_rect(0, in.x() - 1, 0, in.y() - 1);
        matrix<size_t> indexes(in.x(), in.y(), NOT_DEFINED);
#if DO_GRID
        matrix<size_t> mask(indexes);
        process_rectangle(starting_rect, points, indexes, mask);
#else
        process_rectangle(starting_rect, points, indexes);
#endif

        image_RGB out(in.x(), in.y());
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < out.x(); ++x) {
            for (size_t y = 0; y < out.y(); ++y) {
                out(x, y) = in(
                        points[indexes(x, y)][0],
                        points[indexes(x, y)][1]
                );
#if DO_GRID
                if (mask(x,y) != NOT_DEFINED) {
                    out(x, y) = in(
                            points[mask(x, y)][0],
                            points[mask(x, y)][1]
                    );
                }
#endif
            }
        }
        return out;
    }


    double manhattan_dist(const vec2 &a, const vec2 &b) {
        /*2d only*/
        return std::fabs(a[0] - b[0]) + std::fabs(a[1] - b[1]);
    }


    voronoi_animation::voronoi_animation(const size_t x,
                                         const size_t y,
                                         const size_t n_points) :
            points(n_points), size_x(x), size_y(y) {
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
    }


    image_RGB voronoi_animation::get_advanced_to_time(const double time) const {
        image_RGB out(size_x, size_y);
        std::vector<vec_ull> int_points(points.size());
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < points.size(); i++) {
            int_points[i] = points[i].pos + points[i].velocity * time;
        }

        rectangle starting_rect(0, size_x - 1, 0, size_y - 1);
        matrix<size_t> indexes(size_x, size_y, NOT_DEFINED);
#if DO_GRID
        matrix<size_t> mask(indexes);
        process_rectangle(starting_rect, int_points, indexes, mask);
#else
        process_rectangle(starting_rect, int_points, indexes);
#endif


#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < out.x(); ++x) {
            for (size_t y = 0; y < out.y(); ++y) {
                out(x, y) = points[indexes(x, y)].color;
#if DO_GRID
                if (mask(x,y) != NOT_DEFINED) {
                    out(x, y) = points[mask(x,y)].color;
                }
#endif
            }
        }

        return out;
    }

    image_RGB voronoi_filter(const image_RGB &in, const size_t n_points,
                             const distance_type dist) {
        std::mt19937_64 gen((unsigned long) time(nullptr));
        std::uniform_int_distribution<size_t> d_posx(0, in.x() - 1);
        std::uniform_int_distribution<size_t> d_posy(0, in.y() - 1);
        image_RGB out(in.x(), in.y());
        std::vector<vec2> points(n_points);
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < n_points; i++) {
            points[i][0] = d_posx(gen);
            points[i][1] = d_posy(gen);
        }
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < in.x(); x++) {
            for (size_t y = 0; y < in.y(); y++) {
                vec2 cur{double(x), double(y)};
                vec2 min_point = points.front();
                double min_dist = in.size(); /*default to large value*/

                for (size_t i = 0; i < points.size(); i++) {
                    double new_dist;
                    switch (dist) {
                        default:
                        case EUCLIDEAN:
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
