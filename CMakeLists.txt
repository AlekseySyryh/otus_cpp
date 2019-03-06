cmake_minimum_required(VERSION 3.12)
project(otus_realty)

set(CMAKE_CXX_STANDARD 14)

include(conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(rclst rclst.cpp)

target_link_libraries(rclst CONAN_PKG::dlib)