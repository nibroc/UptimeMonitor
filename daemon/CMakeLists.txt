cmake_minimum_required(VERSION 2.8)

PROJECT(uptimed)

# Put the binary in the top directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# Put intermediary files into the build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Build libraries into the build directory
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Set compiler options
set(CMAKE_C_FLAGS "-O3 -pedantic -std=gnu11 -Wall")
set(CMAKE_CXX_FLAGS "-O3 -pedantic -std=c++11 -Wall")

add_executable(uptimed uptimed.cpp notifier.cpp)

# Build procparse subproject
add_subdirectory(procparse)

# Dependencies
TARGET_LINK_LIBRARIES(uptimed procparse)
TARGET_LINK_LIBRARIES(uptimed curl)
