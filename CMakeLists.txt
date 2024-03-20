cmake_minimum_required(VERSION 3.28.0)
project(SonLE VERSION 0.1.0 LANGUAGES C CXX)
include("vcpkg.cmake")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_executable(SonLE ${SOURCES})

find_package(muparser CONFIG REQUIRED)
target_link_libraries(SonLE PRIVATE muparser::muparser)
