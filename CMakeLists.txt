cmake_minimum_required(VERSION 3.25)

project(DesktopTown)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources src/*.cpp src/*.c include/*.hpp include/*.h)
add_executable(DesktopTown ${sources})
target_include_directories(DesktopTown PRIVATE include)
