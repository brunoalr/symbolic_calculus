cmake_minimum_required(VERSION 3.22.1)
project(symbolic_calculus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

add_compile_options("$<$<PLATFORM_ID:Linux,Darwin>:-Wall;-Wextra;-Werror;-pedantic;>")

add_executable(symbolic_calculus src/main.cpp)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION symbolic_calculus)
