cmake_minimum_required(VERSION 3.9)
if ($ENV{TRAVIS_BUILD_NUMBER})
    project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(async VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_package(Threads REQUIRED)

add_library(async SHARED async.cpp async.h)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)
target_link_libraries(async ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(async_cli async PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        LINK_SEARCH_START_STATIC 1
        LINK_SEARCH_END_STATIC 1
        )
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

install(TARGETS async LIBRARY DESTINATION lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)