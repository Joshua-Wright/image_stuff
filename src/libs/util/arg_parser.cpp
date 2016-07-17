// (c) Copyright 2016 Josh Wright
#include "arg_parser.h"

namespace containers {

    using std::string;
    using std::unordered_map;

    void parse_args(unordered_map<string, string> &config,
                    const int argc, const char **argv) {
        /*iterate through every argument,
         * start at 1 to skip the program name as first argument*/
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            /*is this argument a key=value pair?*/
            size_t index = arg.find('=');
            if (index == string::npos) {
                /*if not, set it's value to an empty string*/
                config[arg] = "";
            } else {
                /*if so, set the key and value apropriately*/
                string lhs = arg.substr(0,index);
                string rhs = arg.substr(index+1, string::npos);
                config[lhs] = rhs;
            }
        }
    }
}