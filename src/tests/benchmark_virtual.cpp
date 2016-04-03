// (c) Copyright 2015 Josh Wright

#include <iostream>
#include <time.h>
#include <iomanip>
#include <cmath>
#include "../libs/generators.h"

//#define EXTERNAL 1
//#define EXTERNAL 0

timespec diff(timespec start, timespec end) {
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}


namespace timing_tests {
    const double PI = image_utils::PI;

    class wave {
    public:
        virtual double operator()(const double &x) const = 0;

        virtual ~wave() { };
    };

    class wave_sine : public wave {
    public:
        virtual double operator()(const double &x) const;
    };

    class wave_triangle : public wave {
    public:
        virtual double operator()(const double &x) const;
    };

    class wave_fourier_square : public wave {
        size_t n;
    public:
        wave_fourier_square(size_t _n) : n(_n) { }

        wave_fourier_square(const std::string &spec);

        virtual double operator()(const double &x) const;
    };
}

double sinewave(const double &x) {
    return 0.5 + std::sin(2.0 * image_utils::PI * x) / 2.0;
}

int main(int argc, char const *argv[]) {

    int testsize = 1024 * 1024;

    {
        std::cout << "internal: " << std::endl;
        using namespace timing_tests;
        wave *w1 = new wave_sine();
        wave_sine w2;
        /*make sure other stuff gets instanciated*/
        wave *w3 = new wave_triangle();
        srand((unsigned int) (800 * (*w3)(4)));
        {
            double out = 0;
            timespec start_time;
            clock_gettime(CLOCK_REALTIME, &start_time);
            for (volatile int i = 0; i < testsize; i++) {
                out += (*w1)(2 * i / PI / testsize);
            }
            timespec end_time;
            clock_gettime(CLOCK_REALTIME, &end_time);
            timespec time_diff = diff(start_time, end_time);
            std::cout << std::setw(30) << "virtual pointer: " <<
            std::setw(10) << time_diff.tv_sec << "s " <<
            std::setw(10) << time_diff.tv_nsec << "ns" << std::endl;
        }
        {
            double out = 0;
            timespec start_time;
            clock_gettime(CLOCK_REALTIME, &start_time);
            for (volatile int i = 0; i < testsize; i++) {
                out += w2(2 * i / PI / testsize);
            }
            timespec end_time;
            clock_gettime(CLOCK_REALTIME, &end_time);
            timespec time_diff = diff(start_time, end_time);
            std::cout << std::setw(30) << "virtual method: " <<
            std::setw(10) << time_diff.tv_sec << "s " <<
            std::setw(10) << time_diff.tv_nsec << "ns" << std::endl;
        }
        {
            double out = 0;
            timespec start_time;
            clock_gettime(CLOCK_REALTIME, &start_time);
            for (volatile int i = 0; i < testsize; i++) {
                out += sinewave(2 * i / PI / testsize);
            }
            timespec end_time;
            clock_gettime(CLOCK_REALTIME, &end_time);
            timespec time_diff = diff(start_time, end_time);
            std::cout << std::setw(30) << "regular function: " <<
            std::setw(10) << time_diff.tv_sec << "s " <<
            std::setw(10) << time_diff.tv_nsec << "ns" << std::endl;
        }
    }
    {
        std::cout << "external: " << std::endl;
        using namespace image_utils;
        wave *w1 = new wave_sine();
        wave_sine w2;
        /*make sure other stuff gets instanciated*/
        wave *w3 = new wave_triangle();
        srand((unsigned int) (800 * (*w3)(4)));
        {
            double out = 0;
            timespec start_time;
            clock_gettime(CLOCK_REALTIME, &start_time);
            for (volatile int i = 0; i < testsize; i++) {
                out += (*w1)(2 * i / PI / testsize);
            }
            timespec end_time;
            clock_gettime(CLOCK_REALTIME, &end_time);
            timespec time_diff = diff(start_time, end_time);
            std::cout << std::setw(30) << "virtual pointer: " <<
            std::setw(10) << time_diff.tv_sec << "s " <<
            std::setw(10) << time_diff.tv_nsec << "ns" << std::endl;
        }
        {
            double out = 0;
            timespec start_time;
            clock_gettime(CLOCK_REALTIME, &start_time);
            for (volatile int i = 0; i < testsize; i++) {
                out += w2(2 * i / PI / testsize);
            }
            timespec end_time;
            clock_gettime(CLOCK_REALTIME, &end_time);
            timespec time_diff = diff(start_time, end_time);
            std::cout << std::setw(30) << "virtual method: " <<
            std::setw(10) << time_diff.tv_sec << "s " <<
            std::setw(10) << time_diff.tv_nsec << "ns" << std::endl;
        }
        {
            double out = 0;
            timespec start_time;
            clock_gettime(CLOCK_REALTIME, &start_time);
            for (volatile int i = 0; i < testsize; i++) {
                out += sinewave(2 * i / PI / testsize);
            }
            timespec end_time;
            clock_gettime(CLOCK_REALTIME, &end_time);
            timespec time_diff = diff(start_time, end_time);
            std::cout << std::setw(30) << "regular function: " <<
            std::setw(10) << time_diff.tv_sec << "s " <<
            std::setw(10) << time_diff.tv_nsec << "ns" << std::endl;
        }
    }
}

namespace timing_tests {
    double wave_sine::operator()(const double &x) const {
        return 0.5 + std::sin(2.0 * PI * x) / 2.0;
    }

    double wave_triangle::operator()(const double &x) const {
        /*fix out-of-range values*/
        double x2 = std::fabs(std::fmod(x, 1.0));
        if (x2 < 0.5) {
            return 2.0 * x2;
        } else {
            return 2.0 - 2.0 * x2;
        }
    }

    wave_fourier_square::wave_fourier_square(const std::string &spec) {
        const size_t spec_begin_length = std::strlen("fourier_square:");
        n = std::stoull(spec.substr(spec_begin_length));
    }

    double wave_fourier_square::operator()(const double &x) const {
        double result = 0.0;
        for (size_t i = 1; i < n; i++) {
            result +=
                    sin((2.0 * i - 1.0) * 2.0 * PI * x) / (2.0 * i - 1.0);
        }
        /*divide by max value on range*/
        return (0.5 + result * (2.0 / PI)) / 1.13661977236758;
    }
}