cmake_minimum_required(VERSION 3.6)
project(image_stuff)

# for using clang
#set(CMAKE_C_COMPILER   "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

add_subdirectory(cpp_containers_utilities)

include_directories(
        "src/libs/"
        "src/libs/util"
        cpp_containers_utilities
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -funsafe-math-optimizations")

file(GLOB_RECURSE IMAGE_LIB_SOURCES src/libs/*.cpp)
file(GLOB_RECURSE IMAGE_LIB_HEADERS src/libs/*.h)
add_library(image STATIC ${IMAGE_LIB_SOURCES})
target_link_libraries(image arg_parser cubic_interp)


add_executable(colormap_demo            src/renders/colormap_demo.cpp)
add_executable(expand_pixels            src/renders/expand_pixels.cpp)
add_executable(fractal_animation        src/renders/fractal_animation.cpp)
add_executable(gaussian_blur            src/renders/gaussian_blur.cpp)
add_executable(lissajous_dist           src/renders/lissajous_dist.cpp)
add_executable(mandelbrot               src/renders/mandelbrot.cpp)
add_executable(mandelbrot_tiled         src/renders/mandelbrot_tiled.cpp)
add_executable(plot_waves               src/renders/plot_waves.cpp)
add_executable(polar_grid               src/renders/polar_grid.cpp)
add_executable(rose_dist                src/renders/rose_dist.cpp)
add_executable(rose_dist_range          src/renders/rose_dist_range.cpp)
add_executable(voronoi_iterative        src/renders/voronoi_iterative.cpp)
add_executable(voronoi_transform        src/renders/voronoi_transform.cpp)
add_executable(zoom                     src/renders/zoom.cpp)

target_link_libraries(expand_pixels       image)
target_link_libraries(colormap_demo       image)
target_link_libraries(fractal_animation   image)
target_link_libraries(gaussian_blur       image)
target_link_libraries(lissajous_dist      image)
target_link_libraries(mandelbrot          image)
target_link_libraries(mandelbrot_tiled    image)
target_link_libraries(plot_waves          image)
target_link_libraries(polar_grid          image)
target_link_libraries(rose_dist           image)
target_link_libraries(rose_dist_range     image)
target_link_libraries(voronoi_iterative   image)
target_link_libraries(voronoi_transform   image)
target_link_libraries(zoom                image)


add_executable(benchmark_types   src/tests/benchmark_types.cpp)
add_executable(find_fourier_max  src/tests/find_fourier_max.cpp)


enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(gtest src/tests/unit_tests/all_tests.cpp)
# unfortunately, this macro is not able to see into the macro that is used to
# have parameterized tests in
GTEST_ADD_TESTS(gtest ""
	src/tests/unit_tests/all_tests.cpp
	src/tests/unit_tests/FractalTest.h
	src/tests/unit_tests/test_cubic_interp.h
	src/tests/unit_tests/VoronoiTest.h
	)
target_link_libraries(gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(gtest image)

