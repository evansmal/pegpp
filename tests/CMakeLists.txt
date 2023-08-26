Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.2.1
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest) 
include(Catch)

add_executable(unit combinator.cpp)
target_link_libraries(unit PRIVATE Catch2::Catch2WithMain PRIVATE pegpp)
target_compile_options(unit PUBLIC -O0 -Wall -std=c++20)
catch_discover_tests(unit)
