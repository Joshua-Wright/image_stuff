// (c) Copyright 2015 Josh Wright
#include <cmath>
#include <complex>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstring>
#include <unordered_map>
#include <fftw3.h>

#include "arg_parser.h"
#include "io.h"


int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;
    unordered_map<string, string> config;
    /*default values*/
    config["output"] = "output.png";
    config["input"] = "input.png";
    config["factor"] = "2";
    containers::parse_args(config, argc, argv);

    /*TODO: help message*/

    size_t factor = std::stoull(config["factor"]);
    matrix<RGB> input = read_image(config["input"]);
    matrix<double> input_r(input.x(), input.y(), fftw_malloc, free);
    matrix<double> input_g(input.x(), input.y(), fftw_malloc, free);
    matrix<double> input_b(input.x(), input.y(), fftw_malloc, free);
    matrix<std::complex<double>> input_r_ft(input.x(), input.y(), fftw_malloc,
                                            free);
    matrix<std::complex<double>> input_g_ft(input.x(), input.y(), fftw_malloc,
                                            free);
    matrix<std::complex<double>> input_b_ft(input.x(), input.y(), fftw_malloc,
                                            free);

    std::vector<fftw_plan> plans;
    plans.push_back(fftw_plan_dft_r2c_2d(input.x(), input.y(), input_r.data(),
                                         (fftw_complex *) input_r_ft.data(), 0));

    matrix<double> output(factor * input.x(), factor * input.y());
    matrix<double> output_r(input.x(), input.y(), fftw_malloc, free);
    matrix<double> output_g(input.x(), input.y(), fftw_malloc, free);
    matrix<double> output_b(input.x(), input.y(), fftw_malloc, free);

    for (size_t i = 0; i < input.size(); ++i) {
        input_r(i) = input(i).r;
        input_g(i) = input(i).g;
        input_b(i) = input(i).b;
    }


    return 0;
}
