// (c) Copyright 2016 Josh Wright

#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <unordered_map>
#include "../libs/generators.h"
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/colormaps.h"
#include "../libs/io.h"
#include "../libs/cpp_containers/lib/arg_parser.h"


int main(int argc, char const *argv[]) {
    using namespace image_utils;

    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;
    unordered_map<string, string> config;

    /*default values*/
    config["output"] = "output.png";
    config["x"] = "500";
    config["y"] = "500";
    config["A"] = "1";
    config["B"] = "1";
    config["a"] = "6";
    config["b"] = "7";
    config["sigma"] = "0";
    config["wave_size"] = "16";
    config["wave_type"] = "sawtooth";
    config["lookup_table_size"] = "20";
    containers::parse_args(config, argc, argv);


    if (argc == 1 ||
        config.find("--help") != config.end() ||
        config.find("-h") != config.end()) {
        /*help text*/
        std::cout << "Usage: " << argv[0] << " [parameter_name=definition ...]"
        << std::endl;
        std::cout << std::endl;
        int pw = 20; /*parameter width*/
        int dw = 80 - pw - 10; /*description width*/
        // @formatter:off
        std::cout << std::setw(pw) <<         "parameter:" << std::setw(dw) <<               "description:" << std::endl;
        std::cout << std::setw(pw) <<             "output" << std::setw(dw) <<            "output filename" << std::endl;
        std::cout << std::setw(pw) <<                  "x" << std::setw(dw) <<                "image width" << std::endl;
        std::cout << std::setw(pw) <<                  "y" << std::setw(dw) <<               "image height" << std::endl;
        std::cout << std::setw(pw) <<                  "a" << std::setw(dw) <<  "lissajous curve parameter" << std::endl;
        std::cout << std::setw(pw) <<                  "b" << std::setw(dw) <<  "lissajous curve parameter" << std::endl;
        std::cout << std::setw(pw) <<                  "A" << std::setw(dw) <<  "lissajous curve parameter" << std::endl;
        std::cout << std::setw(pw) <<                  "B" << std::setw(dw) <<  "lissajous curve parameter" << std::endl;
        std::cout << std::setw(pw) <<              "sigma" << std::setw(dw) <<  "lissajous curve parameter" << std::endl;
        std::cout << std::setw(pw) <<          "wave_size" << std::setw(dw) <<     "relative size of waves" << std::endl;
        std::cout << std::setw(pw) <<          "wave_type" << std::setw(dw) <<              "type of waves" << std::endl;
        std::cout << std::setw(pw) <<  "lookup_table_size" << std::setw(dw) <<  "size of lookup table size" << std::endl;
        std::cout << std::setw(pw + dw) << "(given as 2^x)" << std::endl;
        // @formatter:on
        return 0;
    }

    std::string output(config["output"]);
    matrix<double> grid(std::stoull(config["x"]), std::stoull(config["y"]));
    const double A = std::stod(config["A"]);
    const double B = std::stod(config["B"]);
    const double a = std::stod(config["a"]);
    const double b = std::stod(config["b"]);
    const double sigma = std::stod(config["sigma"]);

    double distance_multiplier = std::stod(config["wave_size"]);
    wave *w = parse_wave_spec(config["wave_type"]);

    size_t table_size2 = std::stoull(config["lookup_table_size"]);

    std::cout << "filling lookup table" << std::endl;
    distance_wave *dist_lissajous1 = new dist_lissajous(w, std::pow(2, table_size2),
                                                   distance_multiplier, A, B, a,
                                                   b, sigma);

    std::cout << "rendering image" << std::endl;
    image_fill_2d_wave(grid, dist_lissajous1);

    delete w;
    delete dist_lissajous1;

    color_write_image(grid, new colormap_basic_hot(), output);
    return 0;
}
