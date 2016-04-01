// (c) Copyright 2015 Josh Wright

#include <cmath>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>


int main(int argc, char const *argv[]) {
    std::srand(std::time(nullptr));
    using std::pow;
    using std::cos;
    using std::sin;
    using std::size_t;

    const size_t samples_per_second = 44100;
    std::vector<double> major_notes{
            440 * pow(2.0, 0.0 / 12),
            440 * pow(2, 2.0 / 12),
            440 * pow(2, 4.0 / 12),
            440 * pow(2, 5.0 / 12),
            440 * pow(2, 7.0 / 12),
            440 * pow(2, 9.0 / 12),
            440 * pow(2, 11.0 / 12),
            440 * pow(2, 12.0 / 12)
    };
//    std::cerr << major_notes[0] << std::endl;
//    std::cerr << major_notes.size() << std::endl;

//    double current_freq = major_notes[rand() % major_notes.size()];
    double current_freq = 20; // 20kHz
    double prev_freq = current_freq;


//    std::cerr << current_freq << std::endl;

    double master_t = 0;

    std::vector<double> output;
    /*output 20 notes*/
//    for (int i = 0; i < 20; i++) {
    for (int i = 0; i < 6; i++) {

        current_freq *= 10;


        /*transition*/
        for (double t = 0; t < 0.25; t += 1.0 / samples_per_second) {
            output.push_back(
                    4 * t * sin(current_freq * master_t) +
                    (1 - 4 * t) * sin(prev_freq * master_t)
            );
            master_t += 1.0 / samples_per_second * 2 * 3.14159;
        }

        /*main chunk*/
        for (double t = 0; t < 1; t += 1.0 / samples_per_second) {
            output.push_back(sin(current_freq * master_t));
            master_t += 1.0 / samples_per_second * 2 * 3.14159;
        }
        prev_freq = current_freq;
//        current_freq = major_notes[rand() % major_notes.size()];
    }

    for (auto &sample : output) {
        const int32_t a = std::pow(2, 29) * sample;
        std::cout.write((const char *) &a, sizeof(int32_t) / sizeof(char));
    }

    return 0;
}
