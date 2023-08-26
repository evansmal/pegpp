cmake_minimum_required(VERSION 3.22)
project(pegpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pegpp src/ast.cpp src/combinator.cpp src/generator.cpp src/parser.cpp)
target_include_directories(pegpp PUBLIC src)
target_compile_options(pegpp PUBLIC -O2 -Wall -std=c++20)

add_executable(parser src/main.cpp)
target_link_libraries(parser PUBLIC pegpp)
target_compile_options(parser PUBLIC -O2 -Wall -std=c++20)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    message(STATUS "Unit tests have been enabled")
    add_subdirectory(tests)
endif()

