// (c) Copyright 2016 Josh Wright
#pragma once

#include <unordered_map>
#include <string>

namespace containers {

    /* Usage:
     * 1. make a std::unordered_map<std::string, std::string> of your desired defaults
     * 2. use parse_args() on that map and your command-line parameters
     * 3. parse the string results however you see fit
     *
     * warning: this function modifies config argument!
     */
    void parse_args(std::unordered_map<std::string, std::string> &config,
                    const int argc, const char **argv);

}