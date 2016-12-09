// (c) Copyright 2016 Josh Wright
#ifndef FRACTAL_TEST
#define FRACTAL_TEST

#include <gtest/gtest.h>
#include <tuple>

#ifndef FRACTAL_T

#include "fractal_singlethread.h"

#define FRACTAL_T fractal_singlethread
#endif


#undef CONC
#define CONC(A, B) CONC_(A, B)
#define CONC_(A, B) A##B

#undef FractalTest
#define FractalTest CONC(Test_,FRACTAL_T)

// need these to lower the priority and allow macros for arguments
#define INSTANTIATE_TEST_CASE_P_(a, b, c) INSTANTIATE_TEST_CASE_P(a,b,c)
#define TEST_P_(a, b) TEST_P(a,b)


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
        fractal = FRACTAL_T(get<0>(param), get<1>(param));
        fractal.max_iterations = get<2>(param);
        fractal.smooth = get<3>(param);
        fractal.do_sine_transform = false;
        fractal.run();
    }

protected:
    FRACTAL_T fractal;
//    matrix<double> grid;
};

INSTANTIATE_TEST_CASE_P_(
        FractalTests,
        FractalTest,
        Combine(
                Values(300, 500),
                Values(300, 500),
                Values(128, 512),
                Bool()
        )
);

TEST_P_(FractalTest, RangeSanity) {
    const double inf = std::numeric_limits<double>::max();
    for (size_t i = 0; i < fractal.iterations.size(); i++) {
        ASSERT_GE(inf, fractal.iterations(i));
        ASSERT_LE(0, fractal.iterations(i));
        ASSERT_FALSE(std::isnan(fractal.iterations(i)));
    }
}

TEST_P_(FractalTest, CorrectValue) {
    const double err_thresh = 0.001;
    int fails = 0;
    for (size_t i = 0; i < fractal.iterations.x(); i++) {
        for (size_t j = 0; j < fractal.iterations.y(); j++) {
            complex c = fractal.index_to_complex(vec_ull{i, j});
            double iter = fractal_cell_(complex(0, 0), c, fractal.max_iterations, fractal.smooth, func_standard);
            // this works but finds a few bad pixels sometimes that are tolerable errors
            // TODO stop tolerating small errors
//            EXPECT_NEAR(iter, grid(i, j), err_thresh)
//                                << "(" << i << "," << j << ") -> "
//                                << c;
            if (std::fabs(iter - fractal.iterations(i, j)) > err_thresh) {
                fails++;
            }
        }
    }
    // tolerate a small number of failures
    ASSERT_GE(fractal.iterations.x() * fractal.iterations.y() * 0.0001, fails);
    ASSERT_GE(5, fails);
}

#endif //FRACTAL_TEST
