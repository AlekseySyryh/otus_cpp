cmake_minimum_required(VERSION 3.12)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(ip_filter)
endif ($ENV{TRAVIS_BUILD_NUMBER})


set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(ip_filter ip_filter.cpp split.h ipaddress.h process.h)

add_executable(test_main tests.cpp split.h ipaddress.h process.h)

set_target_properties(ip_filter PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror"
        )

set_target_properties(test_main PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_link_libraries(test_main
        ${Boost_LIBRARIES}
        )

enable_testing()
