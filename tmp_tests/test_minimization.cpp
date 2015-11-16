#include <iostream>
#include <cmath>
#include <ginac/ginac.h>

//double d1(double t, double a, double b) {
//    return (cos(2*t)*cos(t) - a)*(cos(2*t)*cos(t) - a) + (cos(2*t)*sin(t) - b)*(cos(2*t)*sin(t) - b);
//}
//
//double test_dist(double t, void* params) {
//    return d1(t,1,1);
//}

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace GiNaC;

    auto t = symbol("t");
    ex test1 = (cos(2*t)*cos(t) - 1)*(cos(2*t)*cos(t) - 1) + (cos(2*t)*sin(t) - 1)*(cos(2*t)*sin(t) - 1);

    cout << test1 << endl;
    cout << test1.subs(t==3).evalf() << endl;
    cout << test1.diff(t).subs(t==2).evalf() << endl;

    cout << fsolve(test1 == 0.4, t, 0, 6) << endl;

    return 0;
}
