cmake_minimum_required(VERSION 3.17)

# Project name
project(imageprocessor)

# Source files build into build
add_subdirectory(source build)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
