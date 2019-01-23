cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulkmt VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

find_package(Threads REQUIRED)
add_executable(bulkmt main.cpp)
target_link_libraries(bulkmt ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(bulkmt PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        LINK_SEARCH_START_STATIC 1
        LINK_SEARCH_END_STATIC 1
        )
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)
