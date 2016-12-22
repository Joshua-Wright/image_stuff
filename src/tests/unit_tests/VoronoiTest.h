// (c) Copyright 2016 Josh Wright
#pragma once

#include <gtest/gtest.h>
#include <tuple>
#include <io.h>
#include <util/debug.h>
#include "voronoi/voronoi.h"


using namespace image_utils;
using std::tuple;
using ::testing::TestWithParam;
using ::testing::Values;
using ::testing::ValuesIn;
using ::testing::Combine;
using ::testing::Bool;

typedef tuple<size_t, size_t, size_t> VoronoiTestParam_t;

RGB rand_color() {
    return RGB{
            (unsigned char) (rand() % 255),
            (unsigned char) (rand() % 255),
            (unsigned char) (rand() % 255),
    };
}


class VoronoiTest : public ::testing::TestWithParam<VoronoiTestParam_t> {
protected:
    size_t x, y, n_colors;
    std::vector<vec_ull> points;
    std::vector<RGB> colors;

    void SetUp() override {
        using std::get;
        VoronoiTestParam_t param = GetParam();
        x = get<0>(param);
        y = get<1>(param);
        n_colors = get<2>(param);

        srand(30);
        for (size_t i = 0; i < n_colors; ++i) {
            points.push_back(vec_ull{rand() % x, rand() % y});
            colors.push_back(rand_color());
        }
    }
};

INSTANTIATE_TEST_CASE_P(
        VoronoiTests,
        VoronoiTest,
//        ValuesIn(std::vector<VoronoiTestParam_t>{std::make_tuple(500, 300, 1000)})
        Combine(
                Values(300, 500),
                Values(300, 500),
                Values(10, 100, 1000)
        )
);


TEST_P(VoronoiTest, CorrectValue) {
    voronoi voronoi1(x, y);
    voronoi1.calculate(points);
    image_RGB img_actual(x, y);
    voronoi1.into_image(img_actual, colors);

    image_RGB img_expected(x, y);
    for (size_t i = 0; i < x; i++) {
        for (size_t j = 0; j < y; j++) {
            RGB color = colors[closest_point({i, j}, points)];
            img_expected(i, j) = color;
        }
    }

//    write_image(img_actual, "img_actual.png");
//    write_image(img_expected, "img_expected.png");

    for (size_t i = 0; i < x; i++) {
        for (size_t j = 0; j < y; j++) {
            ASSERT_EQ(img_expected(i, j), img_actual(i, j)) << vec_ull{i, j};
        }
    }
}

TEST_P(VoronoiTest, AddPoint) {
    srand(50);
    vec_ull new_point = vec_ull{vec_ull{rand() % x, rand() % y}};
    RGB new_color = rand_color();


    voronoi voronoi1(x, y);
    voronoi1.calculate(points);
    voronoi1.add_point(new_point);
    points.push_back(new_point);
    colors.push_back(new_color);


    image_RGB img_actual(x, y);
    voronoi1.into_image(img_actual, colors);

    image_RGB img_expected(x, y);
    for (size_t i = 0; i < x; i++) {
        for (size_t j = 0; j < y; j++) {
            RGB color = colors[closest_point({i, j}, points)];
            img_expected(i, j) = color;
        }
    }

//    write_image(img_actual, "img_actual.png");
//    write_image(img_expected, "img_expected.png");

    for (size_t i = 0; i < x; i++) {
        for (size_t j = 0; j < y; j++) {
            ASSERT_EQ(img_expected(i, j), img_actual(i, j)) << vec_ull{i, j};
        }
    }
}