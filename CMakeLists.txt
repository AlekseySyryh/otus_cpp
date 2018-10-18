cmake_minimum_required(VERSION 3.2)

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
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;-std=c++1y"
        )

set_target_properties(test_main PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_link_libraries(test_main
        ${Boost_LIBRARIES}
        )

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)

enable_testing()

add_test(case1 test_main)
add_test(case2 test_main)
add_test(case3 test_main)
add_test(case4 test_main)
add_test(case5 test_main)
add_test(case6 test_main)
add_test(WrongLength test_main)
add_test(WrongNumber test_main)
add_test(NonNumber test_main)
add_test(TupleChecker test_main)
add_test(IsMatchFirst test_main)
add_test(IsMatchTwo test_main)
add_test(IsMatchAny test_main)
add_test(AddressEqual test_main)
add_test(Sort test_main)
add_test(ShowAll test_main)
add_test(Filter1Match test_main)
add_test(Filter1NoMatch test_main)
add_test(Filter2Match test_main)
add_test(Filter2NoMatch test_main)
add_test(FilterAnyMatch test_main)
add_test(FilterAnyNoMatch test_main)