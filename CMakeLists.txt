cmake_minimum_required(VERSION 3.12)
if ($ENV{TRAVIS_BUILD_NUMBER})
    project(kkmeans VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(kkmeans VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)

find_package(dlib REQUIRED)

add_executable(kkmeans_gen generator.cpp)
add_executable(kkmeans kkmeans.cpp)

target_link_libraries(kkmeans dlib::dlib)
target_link_libraries(kkmeans_gen dlib::dlib)

set_target_properties(kkmeans kkmeans_gen PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;")

install(TARGETS kkmeans kkmeans_gen RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)
