cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
    project(bayan VERSION 0.0.999)
endif ($ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS   ON)
find_package(OpenSSL COMPONENTS Crypto)
find_package(Boost COMPONENTS filesystem program_options regex REQUIRED)

add_executable(bayan main.cpp)

IF (${OPENSSL_FOUND})
    target_link_libraries(bayan OpenSSL::Crypto)
    add_definitions(-DWITH_OPENSSL)
ELSE (${OPENSSL_FOUND})
    message(WARNING "OpenSSL::Crypto not found - only Boost hashes will be available")
ENDIF (${OPENSSL_FOUND})

set_target_properties(bayan PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Werror;"
        )

target_link_libraries(bayan ${Boost_LIBRARIES})

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

IF (${OPENSSL_FOUND})
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl-dev")
ENDIF (${OPENSSL_FOUND})

set(CPACK_PACKAGE_CONTACT asyryh@gmail.com)

include(CPack)