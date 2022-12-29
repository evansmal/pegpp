find_package(Catch2 3 REQUIRED)
add_executable(unit unit.cpp)
target_link_libraries(unit PRIVATE Catch2::Catch2WithMain)
target_compile_options(unit PUBLIC -O0 -Wall -std=c++20)
