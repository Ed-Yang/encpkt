cmake_minimum_required(VERSION 3.0.2)

project(encpkt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_DEFAULT_STARTUP_PROJECT tinytest)

SET(GCC_COVERAGE_COMPILE_FLAGS "ECB=1")

# vscode cmake tools
include(CMakeToolsHelpers OPTIONAL)

IF (WIN32)
    add_definitions(/WX)
    set(GCC_COVERAGE_LINK_FLAGS "")
ELSE()
    add_definitions(-Werror)
    set(GCC_COVERAGE_LINK_FLAGS "-lm")
ENDIF()

file(GLOB SOURCES "./*.c")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

add_subdirectory(md5)
add_subdirectory(tiny-AES128-C)
add_subdirectory(getopt)

