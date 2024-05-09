cmake_minimum_required(VERSION 3.28)
project(openmp)

set(CMAKE_CXX_STANDARD 17)

add_executable(openmp main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")