#include <iostream>
#include <ginac/ginac.h>
//#include <boost/timer/timer.hpp>
#include <boost/timer.hpp>
#include <boost/math/cstdfloat/cstdfloat_types.hpp>

using namespace std;
using namespace GiNaC;

int cube_minus_2(int b) {
    return b * b * b - 2;
}

float cube_minus_2(float b) {
    return b * b * b - 2.0f;
}


double cube_minus_2(double b) {
    return b * b * b - 2.0;
}


long double cube_minus_2(long double b) {
    return b * b * b - 2.0L;
}

template<typename T>
T cube(T b) {
    return b * b * b;
}


int main() {
    int NUM_ROUNDS = 99999;
    {
        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube((int)i);
        }
        cout << "int: " << t.elapsed() << endl;
    }
    {
        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube((float)i);
        }
        cout << "double: " << t.elapsed() << endl;
    }
    {
        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube((double)i);
        }
        cout << "double: " << t.elapsed() << endl;
    }
    {
//        using namespace boost::math::tools;

        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube((boost::float_fast64_t)i);
        }
        cout << "fast double: " << t.elapsed() << endl;
    }
    {
        boost::timer t;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube((long double)i);
        }
        cout << "long double: " << t.elapsed() << endl;
    }
    {
        symbol t = symbol("t");
        ex cube_minus_2 = t * t * t - 2;
        boost::timer timer;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube_minus_2.subs(t == i);
        }
        cout << "ginac: " << timer.elapsed() << endl;
    }
    {
        symbol t = symbol("t");
        ex cube_minus_2 = t * t * t - 2;
        boost::timer timer;
        for (int i = 0; i < NUM_ROUNDS; i++) {
            cube_minus_2.subs(t == i).evalf();
        }
        cout << "ginac float: " << timer.elapsed() << endl;
    }
}