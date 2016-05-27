// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include "debug.h"
#include "generators.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;

    std::map<std::string, wave> waves_with_labels;
    waves_with_labels.insert(std::make_pair("triangle", wave("triangle")));
    waves_with_labels.insert(std::make_pair("square", wave("square")));
    waves_with_labels.insert(std::make_pair("sine", wave("sine")));
    waves_with_labels.insert(std::make_pair("sawtooth", wave("sawtooth")));
    waves_with_labels.insert(
            std::make_pair("fourier n=2", wave("fourier_square:2")));
    waves_with_labels.insert(
            std::make_pair("fourier n=3", wave("fourier_square:3")));
    waves_with_labels.insert(
            std::make_pair("fourier n=4", wave("fourier_square:4")));
    waves_with_labels.insert(
            std::make_pair("fourier n=5", wave("fourier_square:5")));
    waves_with_labels.insert(
            std::make_pair("fourier n=7", wave("fourier_square:7")));

    std::cout << "clear;clc;close all;" << std::endl;
    std::cout << "x = [" << std::endl;
    std::vector<double> xvalues;
    for (double x = 0; x <= 2; x += 2.0L / 10000.0L) {
        std::cout << std::setprecision(7) << x << ", ";
        xvalues.push_back(x);
    }
    std::cout << "];" << std::endl;

    std::cout << "waves = [" << std::endl;

    std::vector<std::string> labels;
    for (auto &g : waves_with_labels) {
        std::string label = g.first;
        labels.push_back(label);
        wave &w = g.second;
        for (auto x : xvalues) {
            std::cout << std::setprecision(7) << (w)(x) << ", ";
        }
        std::cout << ";" << std::endl;
    }
    std::cout << "];" << std::endl;

    std::cout << "plot(" << std::endl;
    for (int i = 0; i < labels.size() - 1; i++) {
        std::cout << "x, waves(" << i + 1 << ",:)," << std::endl;
    }
    std::cout << "x, waves(" << labels.size() << ",:)" << std::endl;
    std::cout << ")" << std::endl;

    return 0;
}
