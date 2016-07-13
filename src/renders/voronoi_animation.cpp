// (c) Copyright 2015 Josh Wright

#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include "arg_parser.h"
#include "io.h"
#include "voronoi.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;

    srand((unsigned int) time(NULL));

    /*default values*/
    unordered_map<string, string> config;
    config["folder"] = "voronoi_frames";
    config["x"] = "1000";
    config["y"] = "1000";
    config["n"] = "3";
    config["pts"] = "50";
    config["n_frames"] = "600";
    containers::parse_args(config, argc, argv);

    size_t sz = 1000;

    std::string output_folder = config["folder"];
    if (output_folder.back() != '/') {
        output_folder.push_back('/');
    }
    const size_t n_frames = std::stoull(config["n_frames"]);
    voronoi_animation voronoi1(std::stoull(config["x"]), std::stoull(config["y"]),std::stoull(config["pts"]));

    for (size_t i = 0; i < n_frames; i++) {

        std::stringstream output;

        output << output_folder <<
        "out_frame_" << std::setfill('0') <<
        std::setw(5) << i << ".png";

        std::string out_filename = output.str();

        voronoi1.advance(1);
        write_image(voronoi1.image_data, out_filename);

        std::cout << "rendered: " << out_filename << std::endl;
    }


    std::cout << "Done! Render using:" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i "
    << output_folder << "out_frame_%05d.png "
    << output_folder << "output.mp4" << std::endl;


    return 0;
}
