cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

add_executable(bulk_server main.cpp async.cpp)

target_link_libraries(bulk_server ${CMAKE_THREAD_LIBS_INIT} ${Boost_SYSTEM_LIBRARY})

set_target_properties(bulk_server PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        LINK_SEARCH_START_STATIC 1
        LINK_SEARCH_END_STATIC 1
        )

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)