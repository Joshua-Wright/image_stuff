// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <unordered_map>
#include "generators.h"
#include "debug.h"
#include "colormaps.h"
#include "io.h"
#include "arg_parser.h"


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
    config["n"] = "3";
    config["d"] = "7";
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
        std::cout << std::setw(pw) <<                  "n" << std::setw(dw) <<             "rose parameter" << std::endl;
        std::cout << std::setw(pw) <<                  "d" << std::setw(dw) <<             "rose parameter" << std::endl;
        std::cout << std::setw(pw) <<          "wave_size" << std::setw(dw) <<     "relative size of waves" << std::endl;
        std::cout << std::setw(pw) <<          "wave_type" << std::setw(dw) <<              "type of waves" << std::endl;
        std::cout << std::setw(pw) <<  "lookup_table_size" << std::setw(dw) <<  "size of lookup table size" << std::endl;
        std::cout << std::setw(pw + dw) << "(given as 2^x)" << std::endl;
        // @formatter:on
        return 0;
    }

    string output(config["output"]);
    /*yes, x and y are reversed. They're also reversed somewhere else in the
     * code, so it works this way...*/
    matrix<double> grid(std::stoull(config["x"]), std::stoull(config["y"]));
    int n = std::stoi(config["n"]);
    int d = std::stoi(config["d"]);
    double distance_multiplier = std::stod(config["wave_size"]);
    wave *w = parse_wave_spec(config["wave_type"]);
    size_t table_size2 = std::stoull(config["lookup_table_size"]);

    std::cout << "filling lookup table" << std::endl;
    distance_wave *rose_dist1 = new rose_dist(w, std::pow(2, table_size2),
                                              distance_multiplier, n, d);

    std::cout << "rendering image" << std::endl;
    image_fill_2d_wave(grid, rose_dist1);

    delete w;
    delete rose_dist1;

    /*TODO: parameterize the colormap*/
//    colormap *map = new colormap_offset_waves(new wave_triangle());
//    colormap *map = new colormap_threecolor();
    colormap *map = new colormap_basic_hot();
//    colormap *map = new colormap_grayscale();
    color_write_image(grid, map, output);
    return 0;
}
