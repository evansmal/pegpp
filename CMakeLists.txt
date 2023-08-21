cmake_minimum_required(VERSION 3.22)
project(ccombinators)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(peg src/combinator.cpp)
target_include_directories(peg PUBLIC src)
target_compile_options(peg PUBLIC -O2 -Wall -std=c++20)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    message(STATUS "Unit tests have been enabled")
    add_subdirectory(tests)
endif()

