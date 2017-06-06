cmake_minimum_required(VERSION 3.0.2)

project(encpkt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_DEFAULT_STARTUP_PROJECT tinypkt)
SET(CMAKE_BUILD_TYPE Debug)

# vscode cmake tools
include(CMakeToolsHelpers OPTIONAL)

add_subdirectory(sample)
add_subdirectory(external)

