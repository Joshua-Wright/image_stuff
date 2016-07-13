//
// Created by j0sh on 5/27/16.
//
#include <complex>
#include <cmath>
#include "mandelbrot.h"

namespace image_utils {

    void mandelbrot(size_t iterations, matrix<double> &grid,
                    std::array<double, 4> bounds) {
        const size_t x = grid.x();
        const size_t y = grid.y();
        using std::complex;
        using std::pow;
        using std::abs;
        struct mand_cell {
            complex<double> z, c;
            /*iteration that this diverted*/
            size_t i;
            bool done;
        };

        /*initialize the complex grid*/
        matrix<mand_cell> comp_grid(x, y);
        for (size_t i = 0; i < x; ++i) {
            double cr = (double(i) / x) * (bounds[1] - bounds[0]) + bounds[0];
            for (size_t j = 0; j < y; ++j) {
                double ci =
                        (double(j) / y) * (bounds[3] - bounds[2]) + bounds[2];
                comp_grid(i, j).z.real(0);
                comp_grid(i, j).z.imag(0);
                comp_grid(i, j).c.real(cr);
                comp_grid(i, j).c.imag(ci);
                comp_grid(i, j).done = false;
            }
        }

        /*iterate the grid*/
//#pragma omp parallel for schedule(static) collapse(2)
        for (size_t i = 0; i < x; ++i) {
            for (size_t j = 0; j < y; ++j) {

                for (size_t iter = 0; iter < iterations; ++iter) {
                    comp_grid(i, j).z =
                            pow(comp_grid(i, j).z, 2) + comp_grid(i, j).c;
                    if (abs(comp_grid(i, j).z) >= 2) { break; }
                }

            }
        }


        /*normalize the grid (get the magnitude of all the imaginary stuff) */
        for (size_t i = 0; i < x; ++i) {
            for (size_t j = 0; j < y; ++j) {
//                grid(i, j) = abs(comp_grid(i, j).z);
                grid(i, j) = comp_grid(i, j).i;
            }
        }
    }
}
