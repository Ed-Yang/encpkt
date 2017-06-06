cmake_minimum_required(VERSION 3.0.2)

project(external)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEFAULT_STARTUP_PROJECT tiny-aes)
SET(CMAKE_BUILD_TYPE Debug)

add_subdirectory(md5)
add_subdirectory(tiny-AES128-C)
add_subdirectory(getopt)

