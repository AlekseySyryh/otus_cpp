cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(yamr VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(yamr VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(yamr main.cpp)

set_target_properties(yamr PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;")

target_link_libraries(yamr ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS yamr RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)