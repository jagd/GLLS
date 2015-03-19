cmake_minimum_required(VERSION 3.0)
project(General-Linear-Least-Squares)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

set(SOURCE_FILES main.cc)
add_executable(glls ${SOURCE_FILES})