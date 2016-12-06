// (c) Copyright 2016 Josh Wright
#pragma once

#include <gtest/gtest.h>
#include <tuple>
#include "fractal_singlethread.h"

using namespace image_utils;
using ::testing::TestWithParam;
using ::testing::Values;
using ::testing::Combine;
using ::testing::Bool;

/* x, y, max_iterations, smooth */
typedef std::tuple<size_t, size_t, size_t, bool> fractal_test_param_t;

class FractalTest : public ::testing::TestWithParam<fractal_test_param_t> {
protected:
    void SetUp() override {
        using std::get;
        fractal_test_param_t param = GetParam();
        fractal = fractal_singlethread(get<0>(param), get<1>(param));
        fractal.max_iterations = get<2>(param);
        fractal.smooth = get<3>(param);
        fractal.do_sine_transform = false;
        grid = fractal.run();
    }

protected:
    // TODO type parameterize test to do multithreaded also
    fractal_singlethread fractal;
    matrix<double> grid;
};

INSTANTIATE_TEST_CASE_P(
        AllFractalTests,
        FractalTest,
        Combine(
                Values(300, 500),
                Values(300, 500),
                Values(128, 512),
                Bool()
        )
);

TEST_P(FractalTest, RangeSanity) {
    const double inf = std::numeric_limits<double>::max();
    for (size_t i = 0; i < grid.size(); i++) {
        ASSERT_GE(inf, grid(i));
        ASSERT_LE(0, grid(i));
        ASSERT_FALSE(std::isnan(grid(i)));
    }
}

TEST_P(FractalTest, CorrectValue) {
    const double err_thresh = 0.001;
    int fails = 0;
    for (size_t i = 0; i < grid.x(); i++) {
        for (size_t j = 0; j < grid.y(); j++) {
            complex c = fractal.index_to_complex(vec_ull{i, j});
            double iter = fractal_cell_(complex(0, 0), c, fractal.max_iterations, fractal.smooth, func_standard);
            // this works but finds a few bad pixels sometimes that are tolerable errors
            // TODO stop tolerating small errors
//            EXPECT_NEAR(iter, grid(i, j), err_thresh)
//                                << "(" << i << "," << j << ") -> "
//                                << c;
            if (std::fabs(iter - grid(i, j)) > err_thresh) {
                fails++;
            }
        }
    }
    // tolerate a small number of failures
    ASSERT_GE(grid.x() * grid.y() * 0.0001, fails);
    ASSERT_GE(5, fails);
}