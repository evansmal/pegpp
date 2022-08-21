project(ccombinators)
cmake_minimum_required(VERSION 3.19)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ccombinators src/main.cpp)
target_compile_options(ccombinators PUBLIC -O3 -Wall -std=c++20 )
