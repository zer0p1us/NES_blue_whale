cmake_minimum_required(VERSION 3.0.0)
project(Blue_Whale VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "$CMAKE_CXX_FLAGS_DEBUG -O2")


# looking for source files and header
message("-- Looking for ")
file(GLOB src
    "src/*.h"
    "src/*.cpp")

message("-- Adding executable")
add_executable(${PROJECT_NAME} ${src})

# looking for sfml dll files to move to executable later
message("-- Looking for SFML DLLs")
file(GLOB sfml_dll
    "lib/SFML/*.dll")

message("-- Looking for SFML binaries")
file(GLOB sfml_lib
    "lib/SFML/*.a")

# adding header dir for libraries
message("-- Adding include directory")
include_directories(${CMAKE_SOURCE_DIR}/include/)

# linking to sfml libraries
target_link_libraries(${PROJECT_NAME} ${sfml_lib})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${sfml_dll}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
