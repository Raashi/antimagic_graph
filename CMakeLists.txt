cmake_minimum_required(VERSION 3.13)
project(antimagic)

set(CMAKE_CXX_STANDARD 14)

file(GLOB CPPS "*.cpp")
add_executable(antimagic ${CPPS} graph.cpp graph.h)