// (c) Copyright 2015 Josh Wright

#include <iostream>
#include <quadmath.h>
#include <sys/times.h>

template<typename T>
T __test_type_speed(const char *type_label) {
    /*allocate starting numbers*/
    size_t dataset_size = 1024 * 1024 * 50;
    T* dataset = new T[dataset_size];
    std::cout << "Testing " << type_label << ":" << std::endl;
    /*fill with values*/
    for (size_t i=0; i<dataset_size; i++) {
        dataset[i] = i % 800;
    }
    /*get start time*/
    struct tms start_time;
    times(&start_time);

    /*test addition*/
    T startval = 1.234567890L;
    for (size_t i=0; i<dataset_size; i++) {
        startval += dataset[i];
    }
    std::cout << (double) startval << std::endl;
    startval = 1L;
    /*test multiplication*/
    for (size_t i=0; i<dataset_size; i++) {
        startval *= dataset[i] + 1;
    }
    std::cout << (double) startval << std::endl;
    startval = startval * startval * 2;
    for (size_t i=0; i<dataset_size; i++) {
        startval -= dataset[i];
    }
    std::cout << (double) startval << std::endl;

    startval = 1;
    /*test division*/
    for (size_t i=0; i<dataset_size; i++) {
        startval /= dataset[i] + 1;
    }
    std::cout << (double) startval << std::endl;
    struct tms end_time;
    times(&end_time);
    std::cout << "Time taken: " << end_time.tms_utime - start_time.tms_utime;
    std::cout << std::endl << std::endl;


    /*TODO: calculate and output time used*/

    delete dataset;

    return startval;


}
#define test_type_speed(type) __test_type_speed<type>(#type)


int main(int argc, char const *argv[]) {
//    test_type_speed(size_t);
    test_type_speed(float);
    test_type_speed(double);
    test_type_speed(double);
    test_type_speed(__float128);
    return 0;
}
