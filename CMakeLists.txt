cmake_minimum_required(VERSION 3.25)

project(DesktopTown)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources src/*.cpp src/*.c include/*.hpp include/*.h)
add_executable(DesktopTown ${sources})
target_include_directories(DesktopTown PRIVATE include)

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(deps/glfw)

target_link_libraries(DesktopTown PRIVATE glfw)

set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_STATIC ON)
set(USE_GLU OFF)
set(GLEW_OSMESA OFF)
set(PKG_CONFIG_REPRESENTATIVE_TARGET OFF)
set(ONLY_LIBS ON)
add_subdirectory(deps/glew)

target_link_libraries(DesktopTown PRIVATE libglew_static)
