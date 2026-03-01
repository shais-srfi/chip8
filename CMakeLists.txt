cmake_minimum_required(VERSION 3.10)

project(chip8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(roms)

find_package(SDL2 REQUIRED)

add_executable(chip8 src/main.cpp)
target_link_libraries(chip8 SDL2::SDL2)