#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "libs/image_utils.h"
//#include "hot_colormap.h"
//#include "hsv_colormap.h"

/*
g++ -lginac -lcln -ffast-math -std=c++14 -O3 main.cpp lodepng.cpp -o make_fire_grid
 * */

int main() {
    using namespace std;
    using namespace image_utils;
    using image_utils::matrix;
    using image_utils::PI;

    srand((unsigned int) time(0));

//    unsigned z = 100;
//    unsigned z = 500;
//    unsigned z = 1000;
    unsigned z = 2560;
//    unsigned z = 4096;

    matrix<long double> doubles(z, z);

    long double diagonal_dist(point_dist(point(0, 0), point(z, z)));
    point middle(z / 2, z / 2);
    long double g = z / 1.5;
    cout << g << endl;
    long double sx, sy;
    long double dist_x, dist_y;

    int add_const = z / 8;
    long double theta = 0.0;
    long double dist_multiplier = 10;
    long double theta_multiplier = 10;
//    for (int x = 0; x < doubles.x(); x++) {
//        for (int y = 0; y < doubles.y(); y++) {
////
////            /*fire grid*/
//////            dist_x = min_dist(points_1, point(x, y), 4) * dist_multiplier;
//////            dist_y = min_dist(points_2, point(x, y), 4) * dist_multiplier;
//////            sx = dist_x / g;
//////            sy = dist_y / g;
//////            doubles(x, y) = sin(sx) + sin(sy);
//////            doubles(x, y) = sin(sx);
//////            doubles(x, y) = fourier_series(sx, 8) + fourier_square_wave(sy, 8);
//////            doubles(x, y) = fourier_square_wave(sx, 8);
////
////            /*expanding fourier grid*/
//////            sx = (1.0 * (x + add_const) / g) * (1.0 * (x + add_const) / (z / 10));
//////            sy = (1.0 * (y + add_const) / g) * (1.0 * (y + add_const) / (z / 10));
//////            doubles(x, y) = fourier_series(sx, 3) * fourier_square_wave(sy, 3);
////
//            /* single distance  fire grid */
////            dist_x = min_dist(points_1, point(x, y), 4) * dist_multiplier / g;
////            theta = min_theta(points_1, point(x, y), 4) * theta_multiplier;
//            std::tie(dist_x, theta) = min_dist_theta(points_1, point(x,y), 4);
//            dist_x *= dist_multiplier / g;
//            theta *= theta_multiplier;
////            doubles(x, y) = sin(dist_x) + sin(theta);
////            doubles(x, y) = sin(theta);
//            doubles(x, y) = sin(dist_x);
//        }
//    }
//    image_fill_circle_grid(doubles, 40, 201);
    image_fill_rose_ripples(doubles, 5.0L/4.0L, 0, 8*pi);

    for (auto &d : doubles) {
        d = std::sin((z/10)*d);
    }

    auto min_max_tuple = std::minmax_element(doubles.begin(), doubles.end());
    long double min = *min_max_tuple.first;
    long double max = *min_max_tuple.second;
    if (min == INFINITY || min == -INFINITY || max == INFINITY || max == -INFINITY || std::isnan(min) ||
        std::isnan(max)) {
        cout << "infinity detected" << endl;
    }
    cout << min << " " << max << endl;
    if (min == 0 && max == 0) {
        cout << "everything is 0" << endl;
    } else if (min == max) {
        cout << "everything is equal" << endl;
    }

    scale_doubles(doubles);
//    doubles *= -1;
//    doubles -= 1;

    image_RGB color_image(z, z);
    grayscale_to_rgb(doubles, color_image, &colormap_basic_hot);
    cout << "saving image" << endl;
    write_image(color_image, "/home/j0sh/Dropbox/code/Cpp/image_stuff/out.png");
    return 0;
}