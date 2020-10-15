cmake_minimum_required(VERSION 3.15)
project(Project_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_3
        node0.c
        node1.c
        node2.c
        node3.c
        project3.c
        project3.h)
