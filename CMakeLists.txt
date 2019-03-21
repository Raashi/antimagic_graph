cmake_minimum_required(VERSION 3.13)
project(antimagic)

set(CMAKE_CXX_STANDARD 14)

file(GLOB CPPS "*.cpp")
add_executable(antimagic ${CPPS})
add_custom_command(TARGET antimagic
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:antimagic> ..)