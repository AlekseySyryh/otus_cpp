cmake_minimum_required(VERSION 3.12)
project(print_ip)

set(CMAKE_CXX_STANDARD 14)

add_executable(print_ip main.cpp int_traits.h string_traits.h container_traits.h tuple_traits.h all_same_traits.h)