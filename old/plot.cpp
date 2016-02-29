//
// Created by j0sh on 11/6/15.
//
using namespace std;

#include "lib/matplotpp.h"
#include "image_utils.h"



class MP : public MatPlot {
    void DISPLAY() {
        double x_max = 2;
        double x_min = 0;
        int x_steps = 1000;
        vector<double> x(x_steps);
        vector<double> y1(x_steps);
        vector<double> y2(x_steps);
        vector<double> y3(x_steps);
        double x_step = (x_max - x_min) / x_steps;
        for (int i = 0; i < 1000; ++i) {
            x[i] = i * x_step + x_min;
//            y[i] = fourier_square_wave(x[i], 20);
            y1[i] = square_wave(x[i], 1);
            y2[i] = sin(x[i] * 2*PI);
            y3[i] = ( y1[i] + y2[i] ) / 2;
//            y2[i] = fourier_square_wave(x[i], 20);
        }
//        plot(x, y1);
//        plot(x, y2);
        plot(x, y3);
    }
} mp;

void display() { mp.display(); }

void reshape(int w, int h) { mp.reshape(w, h); }

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
//    glutCreateWindow(100,100,400,300);
    glutCreateWindow(100, 100, 1000, 800);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}