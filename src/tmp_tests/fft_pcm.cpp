// (c) Copyright 2015 Josh Wright
#include <cmath>
#include <complex>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fftw3.h>
#include <iomanip>
#include <cstring>

#include "io.h"

/* make/test using
 * make fft_pcm random_notes && ./random_notes |./fft_pcm > test.png
 *
 * expects stdin is single-channel 32-bit little-endian PCM format audio
 * convert using:
 * ffmpeg -i <some file>  -f s32le -acodec pcm_s32le -ac 1 -ar 44100 output.pcm
 */

void read_into_buffer(double *buffer, const size_t length);

int main(int argc, char const *argv[]) {

    using namespace image_utils;
    /*TODO: these as command-line arguments*/
    const size_t samples_per_second = 44100;
    const size_t stepsize = samples_per_second / 10;



    fftw_init_threads();

    double *in = fftw_alloc_real(stepsize);
    std::complex<double> *out = (std::complex<double> *) fftw_alloc_complex(
            stepsize);
    std::vector<std::vector<double>> data_buffer;
    fftw_plan_with_nthreads(4);
    fftw_plan plan = fftw_plan_dft_r2c_1d(stepsize, in, (fftw_complex *) out,
                                          0);
//    fftw_plan plan = fftw_plan_r2r_1d(stepsize, in, out_real, FFTW_RODFT10, 0);

    while (!std::cin.eof()) {
        read_into_buffer(in, stepsize);
        memset(out, 0, sizeof(decltype(out[0])));
        fftw_execute(plan);

        data_buffer.emplace_back();
        data_buffer.back().reserve(stepsize);
        for (size_t i = 0; i < stepsize; i++) {
            data_buffer.back().push_back(std::sqrt(std::norm(out[i])));
//            data_buffer.back().push_back(out_real[i]);
        }
    }

    delete[] in;
    delete[] out;
    fftw_free(plan);
    fftw_cleanup_threads();
    fftw_cleanup();


    size_t width = stepsize / 2 + 1;
    matrix<double> doubles(data_buffer.size(), width);
    for (size_t i = 0; i < data_buffer.size(); i++) {
        for (size_t j = 0; j < width; j++) {
            doubles(i, j) = data_buffer[i][j];
        }
    }
    scale_grid(doubles);
    for (size_t i = 0; i < doubles.x(); i++) {
        for (size_t j = 0; j < doubles.y(); j++) {
//            doubles(i, j) = std::log(10*(doubles(i,j) + 1));
            doubles(i, j) = std::sqrt((doubles(i, j)));
        }
    }
    scale_grid(doubles);
    color_write_image(doubles, new colormap_basic_hot(), "test.png");


    return 0;
}

void read_into_buffer(double *buffer, const size_t length) {
    for (size_t i = 0; i < length; i++) {
        int tmp = 0;
        std::cin.read((char *) (&tmp), sizeof(int32_t));
        buffer[i] = tmp;
    }
}