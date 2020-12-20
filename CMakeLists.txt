cmake_minimum_required(VERSION 3.15)
project(MethGraph)

set(CMAKE_CXX_STANDARD 14)

add_executable(MethGraph main.cpp typelist.h Graph.h functor.h Visitor_pattern.h)