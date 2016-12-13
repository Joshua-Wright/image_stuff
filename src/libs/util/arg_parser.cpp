// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <iomanip>
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
                string lhs = arg.substr(0, index);
                string rhs = arg.substr(index + 1, string::npos);
                config[lhs] = rhs;
            }
        }
    }
}


void parse_args(std::unordered_map<std::string, std::stringstream> &config,
                const int argc, const char **argv) {
    /*iterate through every argument,
     * start at 1 to skip the program name as first argument*/
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        /*is this argument a key=value pair?*/
        size_t index = arg.find('=');
        if (index == std::string::npos) {
            /*if not, set it's value to an empty string*/
            config[arg] << "";
        } else {
            /*if so, set the key and value apropriately*/
            std::string lhs = arg.substr(0, index);
            std::string rhs = arg.substr(index + 1, std::string::npos);
            config[lhs] << rhs;
        }
    }
}

void help_printer(const int argc, const char **argv, const std::vector<std::pair<std::string, std::string>> &helps,
                  int column_gap, int parameter_column_width) {
    using std::cout;
    using std::endl;
    using std::setw;

    bool print = false;
    if (argc < 2) {
        print = true;
    } else {
        for (int i = 0; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                print = true;
                break;
            }
        }
    }

    if (print) {
        cout << "usage: " << argv[0] << "<key1>=<value1> <key2>=<value2> ..." << endl;

        cout << setw(parameter_column_width) << "key:";
        cout << setw(column_gap) << "";
        cout << "value:";
        cout << endl;

        // TODO automatically determine width
        // TODO wrap after maximum width
        for (auto p : helps) {
            cout << setw(parameter_column_width) << p.first;
            cout << setw(column_gap) << "";
            cout << p.second;
            cout << endl;
        }
        exit(1);
    }
}

arg_parser::arg_parser(const int argc, const char **argv) {
    containers::parse_args(config, argc, argv);

}

void arg_parser::read_bool(bool &val, const std::string &argname) const {
    val = config.find(argname) != config.end();
}
