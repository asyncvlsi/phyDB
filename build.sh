#!/bin/bash

# Check if cmake command exists and is executable
if ! command -v cmake &> /dev/null || ! [[ -x "$(command -v cmake)" ]]; then
    echo "CMake is either not installed or not executable. Please install CMake properly."
    exit 1
fi

# Create build directory and move into it
mkdir build
cd build || exit

# Run CMake
cmake ..
if [[ $? -ne 0 ]]; then
    echo "CMake configuration failed. Please check your CMake setup and try again."
    exit 1
fi

# Run make
make -j
if [[ $? -ne 0 ]]; then
    echo "Make failed. Please check your build configuration and try again."
    exit 1
fi

# Run make install
make install
if [[ $? -ne 0 ]]; then
    echo "Make install failed. Please check your installation configuration and try again."
    exit 1
fi

echo "phyDB has been built and installed successfully."
