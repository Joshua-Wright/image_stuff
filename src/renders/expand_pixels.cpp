// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <iomanip>
#include <arg_parser.h>
#include "io.h"
#include "expand_pixels.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;

    /*default values*/
    unordered_map<string, string> config;
    config["scale"] = "30";
    containers::parse_args(config, argc, argv);


    if (argc == 1 ||
        config.find("--help") != config.end() ||
        config.find("-h") != config.end() ||
        config.find("in") == config.end()) {
        /*help text*/
        std::cout << "Usage: " << argv[0] << " [parameter_name=definition ...]"
        << std::endl;
        std::cout << std::endl;
        int pw = 20; /*parameter width*/
        int dw = 80 - pw - 10; /*description width*/
        // @formatter:off
        std::cout << std::setw(pw) <<         "parameter:" << std::setw(dw) <<               "description:" << std::endl;
        std::cout << std::setw(pw) <<                 "in" << std::setw(dw) <<             "input filename" << std::endl;
        std::cout << std::setw(pw) <<                "out" << std::setw(dw) <<            "output filename" << std::endl;
        std::cout << std::setw(pw) <<              "scale" << std::setw(dw) <<               "scale factor" << std::endl;
        // @formatter:on
        return 0;
    }
    std::string input(config["in"]);
    std::string output;
    if (config.find("out") == config.end()) {
        output = input + ".2.png";
    } else {
        output = config["out"];
    }
    matrix<RGB> image1 = read_image(input);
    matrix<RGB> image2 = expand_pixels(image1);
    write_image(image2, output);
}
