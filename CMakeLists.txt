cmake_minimum_required(VERSION 3.28)
project(symbolic_calculus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

add_compile_options("$<$<PLATFORM_ID:Linux,Darwin>:-Wall;-Wextra;-pedantic;-fvisibility=hidden>")

add_executable(symbolic_calculus main.cpp)
