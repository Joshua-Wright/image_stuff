//
// Created by j0sh on 3/18/16.
//

#ifndef IMAGE_STUFF_CURVE_DIST_FILLER_H
#define IMAGE_STUFF_CURVE_DIST_FILLER_H

#include <cmath>
#include "types.h"

namespace image_utils {

    class curve {
    public:
        virtual vect get_point(const double t) const = 0;

        virtual vect unit_normal_vector(const double t) const = 0;
        virtual double min_t() const = 0;
        virtual double max_t() const = 0;
    };

    class curve_circle : public curve {
    public:
        virtual vect get_point(const double t) const {
            return vctr<double>(0.5 * cos(t), 0.5 * sin(t));
        }

        virtual vect unit_normal_vector(const double t) const {
            return vctr<double>(cos(t), sin(t));
        }

        virtual double min_t() const {
            return 0;
        }

        virtual double max_t() const {
            return 2*PI;
        }
    };
    class curve_rose : public curve {
        double k;
    public:
        curve_rose(const double k) : k(k) { }

        virtual vect get_point(const double t) const override {
            return vect(cos(k*t)*cos(t), cos(k*t)*sin(t));
        }

        virtual vect unit_normal_vector(const double t) const override {
//            return vctr<double>((-2*(pow(fabs(cos(k*t)*sin(t) + k*cos(t)*sin(k*t)),2) +
//                                     pow(fabs(cos(t)*cos(k*t) - k*sin(t)*sin(k*t)),2))*
//                                 ((1 + pow(k,2))*cos(t)*cos(k*t) - 2*k*sin(t)*sin(k*t)) -
//                                 (-(cos(k*t)*sin(t)) - k*cos(t)*sin(k*t))*
//                                 (2*fabs(cos(k*t)*sin(t) + k*cos(t)*sin(k*t))*
//                                  ((1 + pow(k,2))*cos(t)*cos(k*t) - 2*k*sin(t)*sin(k*t))*
//                                  Derivative(1)(fabs)(cos(k*t)*sin(t) + k*cos(t)*sin(k*t)) -
//                                  2*fabs(cos(t)*cos(k*t) - k*sin(t)*sin(k*t))*
//                                  ((1 + pow(k,2))*cos(k*t)*sin(t) + 2*k*cos(t)*sin(k*t))*
//                                  Derivative(1)(fabs)(cos(t)*cos(k*t) - k*sin(t)*sin(k*t))))/
//                                (2.*pow(pow(fabs(cos(k*t)*sin(t) + k*cos(t)*sin(k*t)),2) +
//                                        pow(fabs(cos(t)*cos(k*t) - k*sin(t)*sin(k*t)),2),1.5))
//                    ,
//                                (-2*(pow(fabs(cos(k*t)*sin(t) + k*cos(t)*sin(k*t)),2) +
//                                     pow(fabs(cos(t)*cos(k*t) - k*sin(t)*sin(k*t)),2))*
//                                 ((1 + pow(k,2))*cos(k*t)*sin(t) + 2*k*cos(t)*sin(k*t)) -
//                                 (cos(t)*cos(k*t) - k*sin(t)*sin(k*t))*
//                                 (2*fabs(cos(k*t)*sin(t) + k*cos(t)*sin(k*t))*
//                                  ((1 + pow(k,2))*cos(t)*cos(k*t) - 2*k*sin(t)*sin(k*t))*
//                                  Derivative(1)(fabs)(cos(k*t)*sin(t) + k*cos(t)*sin(k*t)) -
//                                  2*fabs(cos(t)*cos(k*t) - k*sin(t)*sin(k*t))*
//                                  ((1 + pow(k,2))*cos(k*t)*sin(t) + 2*k*cos(t)*sin(k*t))*
//                                  Derivative(1)(fabs)(cos(t)*cos(k*t) - k*sin(t)*sin(k*t))))/
//                                (2.*pow(pow(fabs(cos(k*t)*sin(t) + k*cos(t)*sin(k*t)),2) +
//                                        pow(fabs(cos(t)*cos(k*t) - k*sin(t)*sin(k*t)),2),1.5))));
        }

        virtual double min_t() const override {
            return 0;
        }

        virtual double max_t() const override {
            return 4*PI;
        }


    };


    void curve_dist_filler(matrix<double> &grid, curve *curve1);

}
#endif //IMAGE_STUFF_CURVE_DIST_FILLER_H
