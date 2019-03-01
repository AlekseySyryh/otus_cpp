cmake_minimum_required(VERSION 3.12)
project(otus_kkmeans)

set(CMAKE_CXX_STANDARD 14)

find_package(dlib REQUIRED)

add_executable(kkmeans_gen generator.cpp)

target_link_libraries(kkmeans_gen dlib::dlib)