cmake_minimum_required(VERSION 3.7)
project(server)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(SOURCE_FILES server.c functions.c functions.h)
add_executable(server ${SOURCE_FILES})