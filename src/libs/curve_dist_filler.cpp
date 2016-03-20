//
// Created by j0sh on 3/18/16.
//

#include "curve_dist_filler.h"

namespace image_utils {

    void curve_dist_filler(matrix<double> &grid, curve *curve1) {
        for (auto &x : grid) {
            /*TODO: make a filler method for grid*/
            x = 1;
        }
        vect mid(grid.x() / 2.0, grid.y() / 2.0);
        double mid_mag = std::min(grid.x(), grid.y()) / 2;

        double distance_scale = 0;
        double t_step = 0.005 * PI;
        vect prev_left, prev_right;

        /*finish looping when we've gone several iterations without any effect*/
        int effect_count = 0;
        while (effect_count < 10) {

            /*did we have any effect this round?*/
            bool effect = false;

            { /*define the previous points on the curve*/
                vect point = curve1->get_point(0);
                vect norm = curve1->unit_normal_vector(0);
                prev_left = point + norm * distance_scale;
                prev_right = point + (norm * -1.0) * distance_scale;
                prev_left = prev_left * mid_mag + mid;
                prev_right = prev_right * mid_mag + mid;
            }

            for (double t = curve1->min_t() + t_step;
                 t < curve1->max_t(); t += t_step) {

                /*get current point and normal vector*/
                vect point = curve1->get_point(t);
                vect norm = curve1->unit_normal_vector(t);


                /*find the points to the right and left of it*/
                vect left = point + norm * distance_scale;
                vect right = point + (norm * -1.0) * distance_scale;
                /*scale those points*/
                left = left * mid_mag + mid;
                right = right * mid_mag + mid;

                if (left.dist2(prev_left) > 1 || right.dist2(prev_right) > 1) {
                    t_step *= 0.75;
                }
                /*set those points in the grid*/
                if (left.x >= 0 && left.y >= 0 &&
                    left.x < grid.x() && left.y < grid.y()) {
                    if (grid(left.x, left.y) > distance_scale) {
                        grid(left.x, left.y) = distance_scale * 10;
                        effect = true;
                    }
                }
                if (right.x >= 0 && right.y >= 0 &&
                    right.x < grid.x() && right.y < grid.y()) {
                    if (grid(right.x, right.y) > distance_scale) {
                        grid(right.x, right.y) = distance_scale * 10;
                        effect = true;
                    }
                }
                prev_left = left;
                prev_right = right;
            }

            /*advance the distance for the next round*/
            distance_scale += 0.0001;

            /*check if we had any effect*/
            if (effect) {
                /*we had an effect, so reset the counter*/
                effect_count = 0;
            } else {
                /*had no effect, keep track of that and maybe the loop will
                 * exit*/
                ++effect_count;
            }
        }
    }

}