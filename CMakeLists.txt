cmake_minimum_required(VERSION 3.12)
if ($ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})


set(CMAKE_CXX_STANDARD 14)

include(conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


add_executable(rclst rclst.cpp)
add_executable(rclss rclss.cpp)

target_link_libraries(rclst CONAN_PKG::dlib)
target_link_libraries(rclss CONAN_PKG::dlib)

set_target_properties(rclst PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;")

install(TARGETS rclst rclss RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)
