cmake_minimum_required(VERSION 3.28.0)
project(SonLE VERSION 0.1.0 LANGUAGES C CXX)
include("vcpkg.cmake")

add_executable(SonLE src/main.cpp)

find_package(muparser CONFIG REQUIRED)
target_link_libraries(SonLE PRIVATE muparser::muparser)
