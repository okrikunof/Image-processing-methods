cmake_minimum_required(VERSION 3.20)
project(brightness-calculation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(brightness-calculation
        main.cpp
        vector3d.cpp
        color.cpp
        illumination.cpp
)

target_include_directories(brightness-calculation PRIVATE include)