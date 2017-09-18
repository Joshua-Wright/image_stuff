// (c) Copyright 2015 Josh Wright
#include "filters.h"
#include "io.h"
#include "util/arg_parser.h"
#include "util/struct_tuple.h"

using std::string;
using namespace image_utils;
using namespace util;


//std::vector<RGB> colors{
//        RGB{0x49, 0x48, 0x3},
//        RGB{0x66, 0xd9, 0xe},
//        RGB{0x6b, 0xc7, 0x2},
//        RGB{0xf9, 0x26, 0x7},
//        RGB{0x77, 0x54, 0x3},
//        RGB{0x94, 0x59, 0xf},
//        RGB{0xfd, 0x97, 0x1},
//        RGB{0xe6, 0xdb, 0x7},
//        RGB{0xa6, 0xe2, 0x2},
//        RGB{0x35, 0xb0, 0x9},
//        RGB{0x66, 0xef, 0xd},
//        RGB{0x66, 0xd9, 0xe},
//        RGB{0xae, 0x81, 0xf},
//        RGB{0x75, 0x71, 0x5},
//        RGB{0xcf, 0xcf, 0xc},
//        RGB{0xff, 0xff, 0xf},
//        RGB{0x49, 0x48, 0x3},
//        RGB{0x66, 0xd9, 0xe},
//        RGB{0x6b, 0xc7, 0x2},
//        RGB{0xf9, 0x26, 0x7},
//        RGB{0x77, 0x53, 0x3},
//        RGB{0x94, 0x59, 0xf},
//        RGB{0xfd, 0x97, 0x1},
//        RGB{0xe6, 0xdb, 0x7},
//        RGB{0xa6, 0xe2, 0x2},
//        RGB{0x35, 0xb0, 0x9},
//        RGB{0x66, 0xef, 0xd},
//        RGB{0x66, 0xd9, 0xe},
//        RGB{0xae, 0x81, 0xf},
//        RGB{0x75, 0x71, 0x5},
//        RGB{0xcf, 0xcf, 0xc},
//        RGB{0xf8, 0xf8, 0xf},
//        RGB{0x49, 0x48, 0x3},
//        RGB{0xcf, 0xcf, 0xc},
//        RGB{0x27, 0x28, 0x2},
//        RGB{0xe6, 0xdb, 0x7},
//        RGB{0xfd, 0x97, 0x1},
//        RGB{0xa6, 0xe2, 0x2},
//        RGB{0x66, 0xd9, 0xe},
//        RGB{0x75, 0x71, 0x5},
//        RGB{0xf9, 0x26, 0x7},
//};

std::vector<RGB> colors{
//        {249, 38, 114},
//        {102, 217, 239},
//        {166, 226, 46},
////        {253, 151, 31},
//        {39, 40, 34},
//        {0xF8, 0xF8, 0xF2},
//        {0xAE, 0x81, 0xFF},

        {0x2a, 0x0b, 0x3e},
        {0x0f, 0x2c, 0x5d},
        {0x38, 0x39, 0x5a},
        {0x67, 0x7b, 0xaa},
//        {0x7c, 0x7e, 0xb3},
        {0x43, 0xc3, 0xb4},
        {0x6f, 0x15, 0x31},
//        {0x77, 0x3f, 0x4e},
};

int main(int argc, char const **argv) {
    srand(time(NULL));

    arg_parser args(argc, argv);

    auto in = args.read<string>("in");
    auto out = args.read<string>("out", in + ".out.png");

    auto image = read_image(in);

//    std::vector<RGB> colors;
////    RGB init{0xff, 0, 0};
////    int
////            r = 0,
////            g = 0x5e,
////            b = 0xff;
//    int r=0x66, g=0xD9, b=0xEF;
//    int delta = 20;
//    int step = 4;
//    // loop over possibilities, (avoiding overflow)
//    for (int i = -delta; i <= delta; i += step) {
//        if (r + i > 255 || r + i < 0) { continue; }
//
//        for (int j = -delta; j <= delta; j += step) {
//            if (g + j > 255 || g + j < 0) { continue; }
//
//            for (int k = -delta; k <= delta; k += step) {
//                if (b + k > 255 || b + k < 0) { continue; }
//
//                colors.push_back(RGB{r + i, b + j, g + k});
//
//            }
//        }
//    }
//    std::cout << colors.size() << std::endl;
//    for (unsigned char i = 200; i < 250; i++) {
//        colors.push_back(RGB{i, i, i});
//    }

    color_connected_components(image, RGB{255, 255, 255}, colors);

    write_image(image, out);
    return 0;
}
