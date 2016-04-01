cmake_minimum_required(VERSION 3.3)
project(image_stuff)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lfftw3_omp -lfftw3 -lm")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")

set(LIB_FILES
        src/external_libs/lodepng.cpp
        src/libs/io.cpp
        src/libs/colormaps.cpp
        src/libs/generators.cpp
        src/libs/cpp_containers/lib/arg_parser.cpp
        )

add_executable(polar_grid ${LIB_FILES} src/renders/polar_grid.cpp)
add_executable(test_generators ${LIB_FILES} src/tests/test_generators.cpp)
add_executable(plot_waves ${LIB_FILES} src/renders/plot_waves.cpp)
add_executable(lissajous_dist ${LIB_FILES} src/renders/lissajous_dist.cpp)
add_executable(rose_dist ${LIB_FILES} src/renders/rose_dist.cpp)
add_executable(rose_dist_range ${LIB_FILES} src/renders/rose_dist_range.cpp)
add_executable(benchmark_types src/tests/benchmark_types.cpp)
add_executable(find_fourier_max src/tests/find_fourier_max.cpp)
add_executable(curve_dist_filler ${LIB_FILES} src/libs/curve_dist_filler.cpp src/renders/render_curve_dist.cpp)

add_executable(random_notes src/tmp_tests/random_notes_pcm.cpp)

add_executable(fft_pcm ${LIB_FILES} src/tmp_tests/fft_pcm.cpp)