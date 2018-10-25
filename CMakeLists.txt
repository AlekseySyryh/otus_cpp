cmake_minimum_required(VERSION 3.12)
project(allocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(allocator main.cpp allocation_block.h allocator.h container.h container_element.h container_iterator.h)