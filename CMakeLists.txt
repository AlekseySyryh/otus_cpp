cmake_minimum_required(VERSION 3.2)
project(yamr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(yamr main.cpp)

set_target_properties(yamr PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        LINK_SEARCH_START_STATIC 1
        LINK_SEARCH_END_STATIC 1
        )
