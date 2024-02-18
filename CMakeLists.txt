cmake_minimum_required(VERSION 3.8)
project(bigfloat)

set(CMAKE_CXX_STANDARD 17)

add_library(bignumbers include/bigfloat.h src/bigfloat.cpp include/bigint.h src/bigint.cpp src/bigarithm.h src/bigarithm.cpp)
target_include_directories(bignumbers PUBLIC include src)

add_executable(pi pi.cpp)
target_link_libraries(pi bignumbers)

find_package(Catch2 REQUIRED)
add_executable(testint testint.cpp)
target_link_libraries(testint PRIVATE bignumbers Catch2::Catch2WithMain)

add_executable(testfloat testfloat.cpp)
target_link_libraries(testfloat PRIVATE bignumbers Catch2::Catch2WithMain)