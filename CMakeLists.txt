cmake_minimum_required(VERSION 3.5.1)

project(CacheSim)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

include_directories(./include/)

set(SRC ./src/csim.c
        ./src/directmapping.c
        ./src/fullassociative.c
        ./src/setassociative.c
        ./src/utils.c
        ./src/testsim.c)

add_executable(csim ${SRC})
