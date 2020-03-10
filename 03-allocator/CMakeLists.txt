cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(allocator)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)

add_executable(allocator main.cpp allocation_block.h allocator.h container.h container_element.h container_iterator.h)

set_target_properties(allocator PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        )

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)

