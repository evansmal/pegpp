cmake_policy(SET CMP0048 NEW)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.2.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(unit unit.cpp)
target_link_libraries(unit PRIVATE Catch2::Catch2WithMain)
target_compile_options(unit PUBLIC -O0 -Wall -std=c++20)
