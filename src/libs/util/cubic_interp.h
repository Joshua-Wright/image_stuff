// (c) Copyright 2016 Josh Wright
#ifndef CONTAINERS_CUBIC_INTERPOLANT_H
#define CONTAINERS_CUBIC_INTERPOLANT_H

#include <vector>

class cubic_interp {
    std::vector<double> xs, ys, ms;
    double ymin, ymax;

    double clamp(const double x) const;

public:

    cubic_interp();

    /**
     * assumes that xs:ys are sorted!
     */
    cubic_interp(double ymin, double ymax, std::vector<double> xs, std::vector<double> ys);

    double operator()(const double x) const;

};


#endif //CONTAINERS_CUBIC_INTERPOLANT_H
