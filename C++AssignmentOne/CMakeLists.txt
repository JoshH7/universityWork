# cmake_minimum_required(VERSION <specify CMake version here>)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled Driver.cpp Driver.h Point.h Shape.h Circle.h Rectangle.h Square.h Movable.h)