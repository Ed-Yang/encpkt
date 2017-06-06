cmake_minimum_required(VERSION 3.0.2)
project(md5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEFAULT_STARTUP_PROJECT md5main)
SET(CMAKE_BUILD_TYPE Debug)

if (WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
else()
    set(CMAKE_EXE_LINKER_FLAGS "-lm")
endif()

file(GLOB SOURCES "md5.c")

# vscode cmake tools
include(CMakeToolsHelpers OPTIONAL)

# lib
add_library(md5 STATIC ${SOURCES})

# exe
add_executable(md5main md5main.c)
target_link_libraries(md5main md5)

