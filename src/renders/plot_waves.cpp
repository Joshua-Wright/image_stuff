// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/generators.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;

    std::map<std::string, wave*> waves_with_labels;
    waves_with_labels["triangle"] = new wave_triangle();
    waves_with_labels["square"] = new wave_square();
    waves_with_labels["sine"] = new wave_sine();
    waves_with_labels["sawtooth"] = new wave_sawtooth();
    waves_with_labels["fourier n=2"] = new wave_fourier_square(2);
    waves_with_labels["fourier n=3"] = new wave_fourier_square(3);
    waves_with_labels["fourier n=4"] = new wave_fourier_square(4);
    waves_with_labels["fourier n=5"] = new wave_fourier_square(5);
    waves_with_labels["fourier n=7"] = new wave_fourier_square(7);

    std::cout << "clear;clc;close all;" << std::endl;
    std::cout << "x = [" << std::endl;
    std::vector<double> xvalues;
    for (double x = 0; x <= 2; x += 2.0L/10000.0L) {
        std::cout << std::setprecision(7) << x << ", " ;
        xvalues.push_back(x);
    }
    std::cout << "];" << std::endl;

    std::cout << "waves = [" << std::endl;

    std::vector<std::string> labels;
    for (auto &g : waves_with_labels ) {
        std::string label = g.first;
        labels.push_back(label);
        wave *w = g.second;
        for (auto x : xvalues) {
            std::cout << std::setprecision(7) << (*w)(x) << ", " ;
        }
        std::cout << ";" << std::endl;
    }
    std::cout << "];" << std::endl;

    std::cout << "plot(" << std::endl;
    for (int i=0; i<labels.size()-1; i++) {
        std::cout << "x, waves(" << i+1 << ",:)," << std::endl;
    }
    std::cout << "x, waves(" << labels.size() << ",:)" << std::endl;
    std::cout << ")" << std::endl;

    return 0;
}
