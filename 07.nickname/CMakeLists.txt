cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(nickname VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(nickname VERSION 0.0.1)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(nickname main.cpp)

add_executable(test_main tests.cpp)

set_target_properties(nickname test_main PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        )

set_target_properties(test_main PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_link_libraries(test_main
        ${Boost_LIBRARIES}
        )

install(TARGETS nickname RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)

enable_testing()

add_test(tests_add test_main)
add_test(tests_utf8 test_main)
add_test(tests_enumerate test_main)