cmake_minimum_required(VERSION 3.30)
project(image_rendering)

set(CMAKE_CXX_STANDARD 20)

find_package(embree REQUIRED)

add_executable(image_rendering main.cpp)

target_link_libraries(image_rendering PRIVATE embree)