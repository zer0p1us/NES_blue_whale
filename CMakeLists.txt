cmake_minimum_required(VERSION 3.0.0)
project(Blue_Whale VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


IF(CMAKE_COMPILER_IS_GNUCC) # add compiler optimisation when using GCC
    set($CMAKE_CXX_FLAGS_RELEASE "$CMAKE_CXX_FLAGS_RELEASE -O3")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# adding header dir for libraries
message("-- Adding include directory")
include_directories(${CMAKE_SOURCE_DIR}/include/)

# looking for source files and header
message("-- Looking for source and header files")
file(GLOB src
    "src/*.hpp"
    "src/*.cpp"
    "src/core/*.hpp"
    "src/core/*.cpp")

message("-- Adding executable")
add_executable(${PROJECT_NAME} ${src})

message("-- Adding release DLLs of SDL2")
    # use relase DLLs
    file(GLOB sdl2_dll
        "lib/SDL2/*.dll")

    # use release binaries
    message("-- Adding release binaries of SDL2")
    file(GLOB sdl2_lib
        "lib/SDL2/*.a")

    message("-- Adding release binaries of SDL2 dll static library")
    file(GLOB sdl2_dll_static
        "lib/SDL2/*.dll.a")


# linking to libraries
target_link_libraries(${PROJECT_NAME} ${sdl2_dll_static} ${sdl2_lib})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${sdl2_dll}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
