cmake_minimum_required(VERSION 3.10)
project(mini_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Minimal CMakeLists for building the example vectors program in this repo.
# This file was simplified so the project builds without external deps.

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(mini_engine
    src/vectors.cpp
)

# No external libraries required for this small example. If you later add
# GLFW/GLM/GLAD sources, restore the original find_package() and targets.
