// (c) Copyright 2016 Josh Wright

#include "VoronoiTest.h"
#include "fractal/fractal_multithread.h"
#include "fractal/fractal_singlethread.h"
#include "test_cubic_interp.h"
#include "test_hsv.h"
#include <gtest/gtest.h>

// horrible macro magic to parametrize types on already value-parametrized tests
#define FRACTAL_T fractal_singlethread

#include "FractalTest.h"

#undef FRACTAL_TEST
#undef FRACTAL_T
#define FRACTAL_T fractal_multithread

#include "FractalTest.h"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
