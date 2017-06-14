cmake_minimum_required(VERSION 3.7)
project(client)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(SOURCE_FILES client.c functions.c functions.h)
add_executable(client ${SOURCE_FILES})