project(ccombinators)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(json src/json.cpp)
target_compile_options(json PUBLIC -O3 -Wall -std=c++20 )

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    message(STATUS "Unit tests have been enabled")
    add_subdirectory(tests)
endif()

