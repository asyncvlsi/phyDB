#!/bin/bash

mkdir build
cd build || exit
cmake ..
make -j
make install

