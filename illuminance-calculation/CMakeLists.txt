cmake_minimum_required(VERSION 3.20)
project(illuminance-calculation)

set(CMAKE_CXX_STANDARD 20)

add_executable(illuminance-calculation
        main.cpp
        vector3d.cpp
        illumination.cpp
)

target_include_directories(illuminance-calculation PRIVATE include)