cmake_minimum_required(VERSION 3.3)
project(image_stuff)

# for using clang
#set(CMAKE_C_COMPILER   "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# includes:
include_directories(
        "src/libs/"
        "src/libs/util"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")


# find src/libs/ -type f | copy
set(LIB_FILES
        src/libs/voronoi.h
        src/libs/types.h
        src/libs/generators.h
        src/libs/generators.cpp
        src/libs/fractal.h
        src/libs/expand_pixels.cpp
        src/libs/fractal.cpp
        src/libs/expand_pixels.h
        src/libs/colormaps.cpp
        src/libs/filters.cpp
        src/libs/io.cpp
        src/libs/io.h
        src/libs/voronoi.cpp
        src/libs/util/lodepng.cpp
        src/libs/util/lodepng.h
        src/libs/util/vect.h
        src/libs/util/debug.h
        src/libs/util/matrix.h
        src/libs/util/arg_parser.h
        src/libs/util/arg_parser.cpp
        src/libs/colormaps.h
        src/libs/filters.h
)

add_executable(test_generators ${LIB_FILES} src/tests/test_generators.cpp)

add_executable(polar_grid      ${LIB_FILES} src/renders/polar_grid.cpp)
add_executable(plot_waves      ${LIB_FILES} src/renders/plot_waves.cpp)
add_executable(lissajous_dist  ${LIB_FILES} src/renders/lissajous_dist.cpp)
add_executable(rose_dist       ${LIB_FILES} src/renders/rose_dist.cpp)
add_executable(rose_dist_range ${LIB_FILES} src/renders/rose_dist_range.cpp)

add_executable(benchmark_types   src/tests/benchmark_types.cpp)
add_executable(benchmark_virtual ${LIB_FILES} src/tests/benchmark_virtual.cpp)

add_executable(find_fourier_max src/tests/find_fourier_max.cpp)
add_executable(random_notes     src/tmp_tests/random_notes_pcm.cpp)

add_executable(fft_pcm           ${LIB_FILES} src/tmp_tests/fft_pcm.cpp)
add_executable(fft_upscale_image ${LIB_FILES} src/renders/fft_upscale_image.cpp)
target_link_libraries(fft_pcm           fftw3_omp fftw3 m)
target_link_libraries(fft_upscale_image fftw3_omp fftw3 m)
add_executable(expand_pixels     ${LIB_FILES} src/renders/expand_pixels.cpp)
add_executable(gaussian_blur     ${LIB_FILES} src/renders/gaussian_blur.cpp)

add_executable(voronoi_animation ${LIB_FILES} src/renders/voronoi_animation.cpp)
add_executable(voronoi_transform ${LIB_FILES} src/renders/voronoi_transform.cpp)

add_executable(mandelbrot        ${LIB_FILES} src/renders/mandelbrot.cpp)
add_executable(fractal_animation ${LIB_FILES} src/renders/fractal_animation.cpp)
add_executable(zoom              ${LIB_FILES} src/renders/zoom.cpp)
