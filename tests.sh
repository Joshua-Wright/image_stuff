#!/usr/bin/env bash

if make mandelbrot; then
    ./mandelbrot x=1024 y=1024 output=test_mandelbrot_1.png
    ./mandelbrot x=1024 y=1024 output=test_mandelbrot_2.png xa=-1 xb=0 ya=0 yb=1

    ./mandelbrot x=1024 y=1024 output=test_julia_1.png julia
    ./mandelbrot x=1024 y=1024 output=test_julia_2.png julia iter=1024
    ./mandelbrot x=1024 y=1024 output=test_julia_3.png julia cr=0.34 ci=0.04

    rm mandelbrot
fi

if make rose_dist; then
    ./rose_dist x=1024 y=1024 output=test_rose_dist_1.png
    ./rose_dist x=1024 y=1024 output=test_rose_dist_2.png n=5 d=7
    ./rose_dist x=1024 y=1024 output=test_rose_dist_3.png n=5 d=7 wave_size=8

    rm rose_dist
fi
