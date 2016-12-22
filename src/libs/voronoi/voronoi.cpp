// (c) Copyright 2016 Josh Wright
#include <util/debug.h>
#include "voronoi.h"

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

    size_t closest_point(const vec_ull pos, const std::vector<voronoi::point> &points) {
        double min_dist = points[0].position.dist2(pos);
        size_t min_dist_idx = 0;
        for (size_t i = 1; i < points.size(); i++) {
            if (points[i].position.dist2(pos) < min_dist) {
                min_dist = points[i].position.dist2(pos);
                min_dist_idx = i;
            }
        }
        return min_dist_idx;
    }


    void process_rectangle(const rectangle &r, const std::vector<voronoi::point> &points, matrix<voronoi::cell> &grid_indexes) {
        // does not set cell.dist2!!!
        containers::vect<size_t, 4> corner_indexes;
        for (size_t i = 0; i < corner_indexes.size(); i++) {
            // pre-calculate to avoid lazy evaluation skipping
            if (grid_indexes(r.corners[i][0], r.corners[i][1]).point_index == NOT_DEFINED) {
                corner_indexes[i] = closest_point(r.corners[i], points);
                grid_indexes(r.corners[i][0], r.corners[i][1]).point_index = corner_indexes[i];
            } else {
                corner_indexes[i] = grid_indexes(r.corners[i][0], r.corners[i][1]).point_index;
            }
        }
        bool corners_equal = corner_indexes[0] == corner_indexes[1] &&
                             corner_indexes[0] == corner_indexes[2] &&
                             corner_indexes[0] == corner_indexes[3];
        if (corners_equal) {
            size_t index_fill = grid_indexes(r.xmin, r.ymin).point_index;
            for (size_t i = r.xmin; i <= r.xmax; i++) {
                for (size_t j = r.ymin; j <= r.ymax; j++) {
                    grid_indexes(i, j).point_index = index_fill;
                }
            }
        } else if (r.xmax - r.xmin <= 1 && r.ymax - r.ymin <= 1) {
            // rectangle has no content (base case)
        } else if ((r.xmax - r.xmin) > (r.ymax - r.ymin)) {
            // x side is longer, split along x
            process_rectangle(rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, r.ymax), points, grid_indexes);
            process_rectangle(rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, r.ymax), points, grid_indexes);
        } else {
            // same for y-side
            process_rectangle(rectangle(r.xmin, r.xmax, r.ymin, (r.ymin + r.ymax) / 2), points, grid_indexes);
            process_rectangle(rectangle(r.xmin, r.xmax, (r.ymin + r.ymax) / 2, r.ymax), points, grid_indexes);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    voronoi::voronoi(const size_t x, const size_t y) : grid(x, y, {-1.0, NOT_DEFINED}) {}

    void voronoi::calculate(const std::vector<voronoi::point> &pts) {
        rectangle starting_rect(0, grid.x() - 1, 0, grid.y() - 1);

        // calculate closest points efficiently
        this->points = pts;
        process_rectangle(starting_rect, this->points, this->grid);

        // don't bother calculating distances here because we check for them to be valid in add_point()
    }

    void voronoi::calculate_distances() {
        for (size_t i = 0; i < grid.x(); i++) {
            for (size_t j = 0; j < grid.y(); j++) {
                if (grid(i, j).dist2 == -1.0) {
                    grid(i, j).dist2 = points[grid(i, j).point_index].position.dist2(vec_ull{i, j});
                }
            }
        }
    }

    void voronoi::add_point(const voronoi::point &p) {
        flood_out_new_point(p.position, p);
        points.push_back(p);
    }

    void voronoi::flood_out_new_point(const vec_ull &pos, const voronoi::point &p) {
        // assumes new point will be placed at the end of the point array

        // bail if out of bounds
        if (pos[0] >= grid.x() || pos[1] >= grid.y()) {
            // position is unsigned and will therefore wrap-around instead of < 0
            return;
        }

        // make sure cell has real distance
        if (grid(pos).dist2 == -1.0) {
            grid(pos).dist2 = pos.dist2(points[grid(pos).point_index].position);
        }

        // if new point is closer, set new distance and recurse
        double newdist = pos.dist2(p.position);
        if (grid(pos).dist2 > newdist) {

            grid(pos).dist2 = newdist;
            grid(pos).point_index = points.size();

            // recurse on all adjacent cells
            // diagonals are important here to make sure that sharp angles are properly covered
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    flood_out_new_point(vec_ull{pos[0] + i, pos[1] + j}, p);
                }
            }
        }
    }

    void voronoi::into_image(image_RGB &img) {
        assert_same_size(img, grid);
        for (size_t i = 0; i < grid.size(); ++i) {
            img(i) = points[grid(i).point_index].color;
        }
    }


};