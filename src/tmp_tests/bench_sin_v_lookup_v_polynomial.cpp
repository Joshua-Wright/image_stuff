#include <iostream>
#include <cmath>
#include <boost/timer/timer.hpp>

const long double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482L;
const long double Pi = PI;
const long double pi = PI;


/* long story short, std::sin() is faster than lookup */

struct optimized_sin {
    unsigned long long size;
    long double *values;

    optimized_sin(const unsigned long long n) : size(n), values(new long double[n]) {
        for (unsigned long long i = 0u; i <= n; i++) {
            values[i] = std::sin(idx_to_theta(i));
        }
    }

    long double idx_to_theta(const unsigned long long i) {
        return 2 * PI * i / size;
    }

    unsigned long long theta_to_idx(const long double &theta) {
        return (unsigned long long int) (theta / (2 * PI) * size);
    }

    long double &operator()(const long double theta) {
        /* actual optimized sin function*/
        return values[theta_to_idx(theta)];
    }

};


long double sin_5th_taylor(const long double theta) {
    return -1.0L / 3715891200.0L * std::pow(pi - 2 * theta, 10)
           + 1.0L / 10321920.0L * std::pow(pi - 2 * theta, 8)
           - 1.0L / 46080.0L * std::pow(pi - 2 * theta, 6)
           + 1.0L / 384.0L * std::pow(pi - 2 * theta, 4)
           - 1.0L / 8.0L * std::pow(pi - 2 * theta, 2)
           + 1;
}

long double sin_poly(long double theta) {
    /* arithmetic modulus */
    theta = theta - 2 * pi * std::floor(theta / (2 * pi));
    return pi
           - 1.0L / 139916800L * std::pow(pi - theta, 11)
           + 1.0L / 362880.0L * std::pow(pi - theta, 9)
           - 1.0L / 5040.0L * std::pow(pi - theta, 7)
           + 1.0L / 120.0L * std::pow(pi - theta, 5)
           - 1.0L / 6.0L * std::pow(pi - theta, 3)
           - theta;
}

int main() {
    using namespace std;
    optimized_sin sin_opt((unsigned long long int) 1e7);
    cout << "pre-calculated sines" << endl;
    int NUM_ROUNDS = (int) 1e7;
    {
        boost::timer::auto_cpu_timer t;
        for (volatile int i = 0; i < NUM_ROUNDS; i++) {
            sin_opt(i * 2 * PI / NUM_ROUNDS);
        }
        cout << "lookup: " << endl;
    }
    {
        boost::timer::auto_cpu_timer t;
        for (volatile int i = 0; i < NUM_ROUNDS; i++) {
            std::sin(i * 2 * PI / NUM_ROUNDS);
        }
        cout << "Standard Library: " << endl;
    }
    {
        boost::timer::auto_cpu_timer t;
        for (volatile int i = 0; i < NUM_ROUNDS; i++) {
            sin_5th_taylor(i * 2 * PI / NUM_ROUNDS);
        }
        cout << "5th order taylor approximation: " << endl;
    }
    {
        boost::timer::auto_cpu_timer t;
        for (volatile int i = 0; i < NUM_ROUNDS; i++) {
            sin_poly(i * 2 * PI / NUM_ROUNDS);
        }
        cout << "high order taylor approximation: " << endl;
    }
    {
        double err = 0;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            err += fabs(std::sin(i * 2 * PI / NUM_ROUNDS) - sin_poly(i * 2 * PI / NUM_ROUNDS));
        }
        err /= NUM_ROUNDS;
        cout << "average error of 9th order taylor series: " << err << endl;
    }
}