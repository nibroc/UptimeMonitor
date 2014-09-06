cmake_minimum_required(VERSION 2.8)

PROJECT(uptimed)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(procparse STATIC hostname.c loadavg.c meminfo.c procparse.c uptime.c)
