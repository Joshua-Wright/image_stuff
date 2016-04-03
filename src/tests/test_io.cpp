// (c) Copyright 2015 Josh Wright

#include "../libs/io.h"
#include "../libs/cpp_containers/lib/debug.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    std::string input = "test_image.png";
    std::string output = input + ".2.png";
    matrix<RGB> image1 = read_image(input);
    write_image(image1, output);
    matrix<RGB> image2 = read_image(output);
    test(std::equal(image1.begin(), image1.end(), image2.begin(), image2.end(),
                    memcmp_equal<RGB, RGB>), "read/write image");


    return 0;
}
