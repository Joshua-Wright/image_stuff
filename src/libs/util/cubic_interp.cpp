// (c) Copyright 2016 Josh Wright
#include <algorithm>
#include "cubic_interp.h"

using std::size_t;
using std::pow;


double h00(double t) { return 2 * pow(t, 3) - 3 * pow(t, 2) + 1; }

double h10(double t) { return pow(t, 3) - 2 * pow(t, 2) + t; }

double h01(double t) { return -2 * pow(t, 3) + 3 * pow(t, 2); }

double h11(double t) { return pow(t, 3) - pow(t, 2); }

double hermite(double x, double x0, double x1, double p0, double p1, double m0, double m1) {
    const double t = (x - x0) / (x1 - x0);
    return h00(t) * p0 +
           h10(t) * (x1 - x0) * m0 +
           h01(t) * p1 +
           h11(t) * (x1 - x0) * m1;
}

cubic_interp::cubic_interp(double ymin, double ymax, std::vector<double> xs, std::vector<double> ys)
        : ymin(ymin), ymax(ymax), xs(xs), ys(ys) {
    // get slopes
    {/*first element*/
        double dx0 = xs.back() - xs.front();
        double dy0 = ys.back() - ys.front();
        double m0 = dy0 / dx0;
        double dx1 = xs[0] - xs[1];
        double dy1 = ys[0] - ys[1];
        double m1 = dy1 / dx1;
        ms.push_back((m0 + m1) / 2);
    }
    for (size_t i = 1; i < xs.size() - 1; i++) {
        double dx0 = xs[i] - xs[i - 1];
        double dy0 = ys[i] - ys[i - 1];
        double m0 = dy0 / dx0;
        double dx1 = xs[i + 1] - xs[i];
        double dy1 = ys[i + 1] - ys[i];
        double m1 = dy1 / dx1;
        ms.push_back((m0 + m1) / 2);
    }
    {/*last element*/
        double dx0 = xs[xs.size() - 2] - xs[xs.size() - 1];
        double dy0 = ys[ys.size() - 2] - ys[ys.size() - 1];
        double m0 = dy0 / dx0;
        double dx1 = xs.front() - xs.back();
        double dy1 = ys.front() - ys.back();
        double m1 = dy1 / dx1;
        ms.push_back((m0 + m1) / 2);
    }

    // pre-clamp any obvious problems
    for (size_t i = 0; i < xs.size(); i++) {
        if (ys[i] <= ymin || ys[i] >= ymax) {
            ms[i] = 0;
        }
    }

}

double cubic_interp::operator()(const double x) const {

    size_t i = 0;
    while (xs[i] <= x) {
        i++;
    }

    return clamp(hermite(x,
                         xs[i - 1], xs[i],
                         ys[i - 1], ys[i],
                         ms[i - 1], ms[i]
    ));
}

double cubic_interp::clamp(const double x) const {
    if (x < ymin) { return ymin; }
    if (x > ymax) { return ymax; }
    return x;
}

cubic_interp::cubic_interp() {
}
