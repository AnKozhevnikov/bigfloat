cmake_minimum_required(VERSION 3.8)
project(bigfloat)

set(CMAKE_CXX_STANDARD 17)

add_library(bignumbers bigarithm.cpp bigarithm.h bigint.h bigint.cpp bigfloat.h bigfloat.cpp)

add_executable(pi pi.cpp)
target_link_libraries(pi bignumbers)

find_package(Catch2 REQUIRED)
add_executable(testint testint.cpp)
target_link_libraries(testint PUBLIC bignumbers PRIVATE Catch2::Catch2WithMain)