// (c) Copyright 2015 Josh Wright

#include <iostream>
#include <cmath>
#include <iomanip>

int main(int argc, char const *argv[]) {
    using std::sin;
    const double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482;

    for (int i=1; i<20; i++) {
        double max;
        for (double x=0; x<1; x+= 0.001) {
            double result = 0.0;
            for (size_t j = 1; j < i; j++) {
                result +=
                        sin((2.0 * j - 1.0) * 2.0 * PI * x) / (2.0 * j - 1.0);
            }
            result = 0.5 + result * (2.0 / PI);
            if (max < result) {
                max = result;
            }
        }
        std::cout << std::setprecision(15);
        std::cout << max << std::endl;
    }
}
