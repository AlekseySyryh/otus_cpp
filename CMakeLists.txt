cmake_minimum_required(VERSION 3.12)
project(bayan)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenSSL COMPONENTS Crypto)
find_package(Boost COMPONENTS filesystem program_options regex REQUIRED)

add_executable(bayan main.cpp)

IF (${OPENSSL_FOUND})
    target_link_libraries(bayan OpenSSL::Crypto)
    add_compile_definitions(WITH_OPENSSL)
ELSE (${OPENSSL_FOUND})
    message(WARNING "OpenSSL::Crypto not found - only Boost hashes will be available")
ENDIF (${OPENSSL_FOUND})

target_link_libraries(bayan ${Boost_LIBRARIES})