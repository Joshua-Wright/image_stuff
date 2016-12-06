// (c) Copyright 2016 Josh Wright
#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include "cubic_interp.h"

TEST(cubic, datapoints) {
    // TODO determine why final unused data point is needed
    cubic_interp interp(
            -999, 999,
            {0, 1, 2, 3, 4},
            {0, 100, 200, 300, 400}
    );

    // exactly equal because it should return the real data point
    ASSERT_EQ(0, interp(0));
    ASSERT_EQ(100, interp(1));
    ASSERT_EQ(200, interp(2));
    ASSERT_EQ(300, interp(3));
}

TEST(cubic, linear_interp) {
    cubic_interp interp(
            -999, 999,
            {0, 1, 2, 3},
            {0, 100, 200, 300}
    );

    for (double x = 0.1; x < 2.9; x += 0.05) {
        ASSERT_FLOAT_EQ(100 * x, interp(x)) << "linear interp " << x << " to " << 100 * x;
    }
}

TEST(cubic, clamp) {
    srand(10);
    int min = -33;
    int max = 41;
    std::vector<double> xs, ys;
    for (int i = 0; i < 20; ++i) {
        xs.push_back(i);
        ys.push_back(min + rand() % (max - min));
    }
    cubic_interp interp(min, max, xs, ys);

    for (double x = 0; x < 20; x += 0.01) {
        ASSERT_GE(max, interp(x)) << "clamp " << x;
        ASSERT_LE(min, interp(x)) << "clamp " << x;
    }
}

TEST(cubic, cubic) {
    srand(10);
    const int max_coef = 200;
    const double a = rand() % max_coef;
    const double b = rand() % max_coef;
    const double c = rand() % max_coef;
    const double d = rand() % max_coef;
    const double xmax = 100;
    auto poly = [&](double x) { return a * x * x * x + b * x * x + c * x + d; };

    std::vector<double> xs, ys;
    for (double i = -xmax; i < xmax; i += 0.01) {
        xs.push_back(i);
        ys.push_back(poly(i));
    }
    const double max = std::numeric_limits<double>::max();
    const double min = -std::numeric_limits<double>::max();

    cubic_interp interp(min, max, xs, ys);

    for (double x = -0.9 * xmax; x < 0.9 * xmax; x += 0.1) {
        ASSERT_FLOAT_EQ(poly(x), interp(x))
                                    << "x = " << x << " -> "
                                    << a << "*x^3 + " << b << "*x^2 + " << c << "*x + " << d <<
                                    " = " << interp(x);
    }
}
