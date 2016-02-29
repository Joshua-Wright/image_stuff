//
// Created by j0sh on 11/10/15.
//

#include <functional>
#include <atomic>
#include <list>
#include <thread>
#include <functional>
#include "image_filler.h"


void worker_thread(std::function<double(int x, int y)> &func, matrix<double> &image, std::atomic_ullong &current_idx,
                   const ullong max_idx) {
    ullong current_block;
    int width = image.x();
    unsigned x, y;
    while ((current_block = current_idx.fetch_add(1)) < max_idx) {
        x = (unsigned int) (current_block / width);
        y = (unsigned int) (current_block % width);
        image(x, y) = func(x, y);
    }
}


image_filler::image_filler(const int n_threads) {
}

void image_filler::run(std::function<void(int x, int y)> &func, matrix<double> &image, const ullong max_idx) {
    std::list<std::thread> threads;
    std::atomic_ullong current_idx = 0;
    for (int i = 0; i < n_threads; i++) {
        threads.emplace_back(&func, std::ref(image), std::ref(current_idx), max_idx);
    }
    for (auto &t : threads) {
        t.join();
    }
}
