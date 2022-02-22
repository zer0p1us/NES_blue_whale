cmake_minimum_required(VERSION 3.0.0)
project(Blue_Whale VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


IF(CMAKE_COMPILER_IS_GNUCC) # add compiler optimisation when using GCC
    set($CMAKE_CXX_FLAGS_RELEASE "$CMAKE_CXX_FLAGS_RELEASE -O3")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


# looking for source files and header
message("-- Looking for source and header files")
file(GLOB src
    "src/*.h"
    "src/*.cpp")

message("-- Adding executable")
add_executable(${PROJECT_NAME} ${src})


# check if build is release
IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
    message("-- Adding release DLLs of sfml")
    # use relase DLLs
    file(GLOB sfml_dll
        "lib/SFML/release/*.dll")

    # use release binaries
    message("-- Adding release binaries of sfml")
    file(GLOB sfml_lib
        "lib/SFML/release/*.a")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Release")

# check if build is debug
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message("-- Adding debug DLLs of sfml")
    # use debug DLLs
    file(GLOB sfml_dll
    "lib/SFML/debug/*.dll")

    # use debug binaries
    message("-- Adding debug binaries of sfml")
    file(GLOB sfml_lib
        "lib/SFML/debug/*.a")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")


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
