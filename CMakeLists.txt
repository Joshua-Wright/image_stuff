cmake_minimum_required(VERSION 3.3)
project(image_stuff)

#[[
-funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=1

g++ -O3 -std=c++14 -lpthread -ffast-math libs/image_utils.cpp external_libs/lodepng.cpp libs/matrix.h libs/vctr.cpp main.cpp -o test
]]

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#project(assembler C ASM) # this enables building the assembly part of the code
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lfltk -lfltk_images")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_timer -lboost_system")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgsl -lcblas") #[[ needed to link to gsl]]
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include -L/usr/local/lib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lginac -lcln") # for ginac
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglut -lGLU -lGL") #[[ for plot]]


set(IMAGE_UTILS libs/image_utils.cpp external_libs/lodepng.cpp libs/matrix.h libs/vctr.cpp)
add_executable(main main.cpp ${IMAGE_UTILS})

add_executable(asdf asdf.cpp external_libs/lodepng.cpp)

add_executable(plot plot.cpp external_libs/matplotpp.cc external_libs/gl2ps.c ${IMAGE_UTILS})

add_executable(test_minimization tmp_tests/test_minimization.cpp)

add_executable(test_min_val tmp_tests/test_min_val.cpp ${IMAGE_UTILS})

add_executable(bench_double_v_numeric tmp_tests/bench_double_v_numeric.cpp ${IMAGE_UTILS})

add_executable(bench_sin_v_lookup tmp_tests/bench_sin_v_lookup_v_polynomial.cpp)