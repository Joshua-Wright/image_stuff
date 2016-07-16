// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <iomanip>
#include "voronoi.h"
#include "arg_parser.h"
#include "io.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;
    /*default values*/
    unordered_map<string, string> config;
    config["n"] = "30";
    config["dist"] = "e";
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
        std::cout << std::setw(pw) <<                  "n" << std::setw(dw) <<     "number of input pixels" << std::endl;
        std::cout << std::setw(pw) <<               "dist" << std::setw(dw) <<              "distance type" << std::endl;
        std::cout << std::setw(pw+dw) << "distance can be m (manhattan) or e (euclidian)" << std::endl;
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
    size_t n = std::stoull(config["n"]);
    distance_type distance_type1 = EUCLIDEAN;
    if (config["dist"] == "m") {
        distance_type1 = MANHATTAN;
    }

    image_RGB image1 = read_image(input);
//    image_RGB image2 = voronoi_filter(image1, n, distance_type1);
    image_RGB image2 = fast_voronoi_filter(image1, n);
    write_image(image2, output);

    return 0;
}
