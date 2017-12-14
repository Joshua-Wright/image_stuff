// (c) Copyright 2017 Josh Wright
#pragma once

#include "HSV.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace image_utils;

TEST(hsv2rgb, conversion) {
  ASSERT_EQ((RGB{255, 0, 0}), vec3_to_RGB(HSV2RGB(HSV{0, 1, 1})));
  ASSERT_EQ((RGB{0, 255, 0}), vec3_to_RGB(HSV2RGB(HSV{120, 1, 1})));
  ASSERT_EQ((RGB{0, 0, 255}), vec3_to_RGB(HSV2RGB(HSV{240, 1, 1})));
  ASSERT_EQ((RGB{124,43,201}), vec3_to_RGB(HSV2RGB(HSV{271,0.786,0.786})));
  ASSERT_EQ((RGB{114,59,191}), vec3_to_RGB(HSV2RGB(HSV{265,0.691,0.749})));
};

TEST(rgb2hsv, conversion) {
  ASSERT_EQ((HSV{0, 1, 1}),         RGB2HSV(vec3{255, 0, 0}));
  ASSERT_EQ((HSV{120, 1, 1}),       RGB2HSV(vec3{0, 255, 0}));
  ASSERT_EQ((HSV{240, 1, 1}),       RGB2HSV(vec3{0, 0, 255}));
};