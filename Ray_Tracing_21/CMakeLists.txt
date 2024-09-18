cmake_minimum_required(VERSION 3.10)

# Project name
project(RayTracing)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the Intel MPI compiler
set(CMAKE_CXX_COMPILER mpiicpx)

# List of source files
set(SOURCES
    RayTracing_14.cpp
    camera.cpp
    camera_parallel.cpp
    color.cpp
    color_array.cpp
    interval.cpp
    parallel.cpp
    path.cpp
    sphere.cpp
    vec3.cpp
    write.cpp
)

# Define the executable
add_executable(RayTracing_14 ${SOURCES})
