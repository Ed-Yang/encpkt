cmake_minimum_required(VERSION 3.0.2)

project(tiny)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GCC_COVERAGE_COMPILE_FLAGS "ECB=1")

# vscode cmake tools
include(CMakeToolsHelpers OPTIONAL)

include_directories("./" "../external" "../external/md5" "../external/tiny-AES128-C")

if (WIN32)
set(GCC_COVERAGE_LINK_FLAGS "")
ELSE()
set(GCC_COVERAGE_LINK_FLAGS "-lm")
endif()

file(GLOB TINY_SRC "./tiny/*.c")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

add_executable(testpkt ./testpkt.c ${TINY_SRC})
target_link_libraries(testpkt md5 aes)

