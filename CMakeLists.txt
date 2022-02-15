cmake_minimum_required(VERSION 3.0.0)
project(NES_AES VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "$CMAKE_CXX_FLAGS_DEBUG -O2")

file(GLOB src
    "src/*.h"
    "src/*.cpp")

add_executable(NES_AES ${src})
