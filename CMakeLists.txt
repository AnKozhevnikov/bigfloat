cmake_minimum_required(VERSION 3.8)
project(bigfloat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB BIGNUMBERS_SRC ${CMAKE_SOURCE_DIR}/src/*.* ${CMAKE_SOURCE_DIR}/include/*.*)
add_library(bignumbers ${BIGNUMBERS_SRC})
target_include_directories(bignumbers PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(pi pi.cpp)
target_link_libraries(pi bignumbers)

find_package(Catch2)
if (Catch2_FOUND)
    add_executable(testint testint.cpp)
    target_link_libraries(testint PRIVATE bignumbers Catch2::Catch2WithMain)

    add_executable(testfloat testfloat.cpp)
    target_link_libraries(testfloat PRIVATE bignumbers Catch2::Catch2WithMain)
endif()