// (c) Copyright 2016 Josh Wright
#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <functional>

void parse_args(std::unordered_map<std::string, std::stringstream> &config,
                const int argc, const char **argv);

class arg_parser {
    std::unordered_map<std::string, std::stringstream> config;

public:
    arg_parser(const int argc, const char **argv);

    template<typename T>
    T read(const std::string &argname, T def = T()) const {
        auto it = config.find(argname);
        if (it != config.end()) {
            std::stringstream ss2(it->second.str());
            ss2 >> def;
        }
        return def;
    }

    /**
     *
     * @tparam T type of arg being read
     * @tparam U optional, type of literal supplied (for convenience)
     * @param val target to read into
     * @param argname name of argument on command line
     * @param def default value (optional)
     */
    template<typename T, typename U = T>
    void read_into(T &val, const std::string &argname, U def = U()) const {
        auto it = config.find(argname);
        if (it != config.end()) {
            std::stringstream ss2(it->second.str());
            ss2 >> val;
        } else {
            val = def;
        }
    }

    void read_bool(bool &val, const std::string &argname) const;

};

namespace containers {
    /* deprecated */
    void parse_args(std::unordered_map<std::string, std::string> &config,
                    const int argc, const char **argv);

}
