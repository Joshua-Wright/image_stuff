#!/usr/bin/env bash


cd src

git clone git@github.com:lvandeve/lodepng.git
cd lodepng; git fetch; git pull; cd ..

git clone git@github.com:Joshua-Wright/cpp_containers_utilities.git
cd cpp_containers_utilities; git fetch; git pull; cd ..