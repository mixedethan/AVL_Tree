cmake_minimum_required(VERSION 3.24)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

include_directories(test-unit)

add_executable(Project1
        src/main.cpp
        test-unit/catch.hpp
        test-unit/test.cpp)
