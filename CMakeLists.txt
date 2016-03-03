cmake_minimum_required(VERSION 3.3)
project(image_stuff)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")

set(LIB_FILES
        src/external_libs/lodepng.cpp
        src/libs/io.cpp
        src/libs/colormaps.cpp
        src/libs/generators.cpp
        )

add_executable(polar_grid ${LIB_FILES} src/renders/polar_grid.cpp)
add_executable(test_generators ${LIB_FILES} src/tests/test_generators.cpp)
add_executable(plot_waves ${LIB_FILES} src/renders/plot_waves.cpp)
add_executable(rose_dist ${LIB_FILES} src/renders/rose_dist.cpp)
