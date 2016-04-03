// (c) Copyright 2016 Josh Wright

#include <iostream>
//#include <quadmath.h>
#include <sys/times.h>
#include <time.h>
#include <iomanip>

timespec diff(timespec start, timespec end) {
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}


template<typename T>
T __test_type_speed(const char *type_label) {
    /*allocate starting numbers*/
    size_t dataset_size;
//    dataset_size = 1024 * 1024 * 50;
    dataset_size = 1024 * 1024;
    T *dataset = new T[dataset_size];
    std::cout << "Testing " << type_label << ":" << std::endl;
    /*fill with values*/
    for (size_t i = 0; i < dataset_size; i++) {
        dataset[i] = i % 800;
    }
    /*get start time*/
    timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    /*test addition*/
    T startval = 1.234567890L;
    for (size_t i = 0; i < dataset_size; i++) {
        startval += dataset[i];
    }
    std::cout << (double) startval;
    startval = 1L;
    /*test multiplication*/
    for (size_t i = 0; i < dataset_size; i++) {
        startval *= dataset[i] + 1;
    }
    std::cout << (double) startval;
    startval = startval * startval * 2;
    for (size_t i = 0; i < dataset_size; i++) {
        startval -= dataset[i];
    }
    std::cout << (double) startval;

    startval = 1;
    /*test division*/
    for (size_t i = 0; i < dataset_size; i++) {
        startval /= dataset[i] + 1;
    }
    std::cout << (double) startval << std::endl;
    timespec end_time;
    clock_gettime(CLOCK_REALTIME, &end_time);
    timespec time_diff = diff(start_time, end_time);
    std::cout << "Time taken: " <<
    std::setw(10) << time_diff.tv_sec << "s " <<
    std::setw(10) << time_diff.tv_nsec << "ns";
    std::cout << std::endl << std::endl;

    delete[] dataset;

    return startval;


}

#define test_type_speed(type) __test_type_speed<type>(#type)


int main(int argc, char const *argv[]) {
    test_type_speed(std::size_t);
    test_type_speed(float);
    test_type_speed(double);
    test_type_speed(long double);
//    test_type_speed(__float128);
    return 0;
}
