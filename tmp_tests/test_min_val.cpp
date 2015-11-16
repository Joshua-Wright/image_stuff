#include <iostream>
#include <cmath>
#include <assert.h>
#include <boost/math/tools/minima.hpp>
#include <boost/timer.hpp>
#include "../image_utils.h"

double test_func(double x) {
    /* a parabola */
    return 4 + (x - 5) * (x - 5);
}

class test_functor {
    double operator()(double x) {
        /* a parabola */
        return 4 + (x - 5) * (x - 5);
    }
};

double d1(double t, double a, double b) {
    using namespace std;
    return (cos(2 * t) * cos(t) - a) * (cos(2 * t) * cos(t) - a) +
           (cos(2 * t) * sin(t) - b) * (cos(2 * t) * sin(t) - b);
}


long double d1(long double t, long double a, long double b) {
    using namespace std;
    return (cos(2 * t) * cos(t) - a) * (cos(2 * t) * cos(t) - a) +
           (cos(2 * t) * sin(t) - b) * (cos(2 * t) * sin(t) - b);
}


double test_dist(double t) { return d1(t, 1, 1); }

long double test_dist_long(long double t) { return d1(t, 1, 1); }


int main(int argc, char *argv[]) {
    using namespace std;
    using namespace image_utils;
//    cout << function_min(&test_func, 0, 20) << endl;
//    assert(function_min(&test_func, 0, 20, 100, 10) == 4);

//    std::pair<double, double> min = boost::math::tools::brent_find_minima(&test_func, 0.0, 20.0, 999);
//    cout << min.first << endl;
//    cout << min.second << endl;

//    int NUM_ROUNDS = 999;
//    {
//        boost::timer t;
//        for (int i = 0; i < NUM_ROUNDS; i++) {
//            function_min(&test_func, 0, 20);
//        }
//        cout << "custom: " << t.elapsed() << endl;
//    }
//    {
//        boost::timer t;
//        for (int i = 0; i < NUM_ROUNDS; i++) {
//            boost::math::tools::brent_find_minima(&test_func, 0.0, 20.0, 999);
//        }
//        cout << "boost: " << t.elapsed() << endl;
//    }

//    min = boost::math::tools::brent_find_minima(&test_dist, 0.0, 1.0*PI, 999);
//    cout << min.first << endl;
//    cout << min.second << endl;
//    cout << endl;
//    cout << function_min(&test_dist, 0, 2*PI, 1000, 5) << endl;



    int NUM_ROUNDS = 100;
    {
        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            function_min(&test_dist, 0, 20);
        }
        cout << "double: " << t.elapsed() << endl;
    }
    {
        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            function_min(&test_dist_long, 0, 20);
        }
        cout << "long double: " << t.elapsed() << endl;
    }
}