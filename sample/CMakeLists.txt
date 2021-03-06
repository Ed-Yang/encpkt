cmake_minimum_required(VERSION 3.0.2)

project(sample)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_BUILD_TYPE Debug)

#
# if failed to find openssl, set env variable
# set OPENSSL_ROOT_DIR=<root of openssl>
# ex. 
#   
#   set OPENSSL_ROOT_DIR=C:\Program Files (x86)\OpenSSL
#   or
#   set OPENSSL_ROOT_DIR=\Packages\openssl 
#

find_package(OpenSSL) 

# OPENSSL_LIBRARIES
# OPENSSL_INCLUDE_DIR

# message (STATUS ${OPENSSL_LIBRARIES})

include_directories("./" "../external")
include_directories("../external/md5" "../external/getopt" "../external/tiny-AES128-C")
include_directories(${OPENSSL_INCLUDE_DIR})

if (WIN32)
    set(GCC_COVERAGE_LINK_FLAGS "")
else()
    set(GCC_COVERAGE_LINK_FLAGS "-lm -lcrypto")
endif()

file(GLOB TINY_SRC "./tiny/*.c")
file(GLOB SSL_SRC "./ssl/*.c")


set(W32_LIBS "getopt")

if(WIN32)
    set(W32_LIBS ${W32_LIBS} "wsock32" "Ws2_32" "crypt32")
endif()

# vscode cmake tools
include(CMakeToolsHelpers OPTIONAL)

# testenc

add_executable(tinyenc ./testenc.c ${TINY_SRC})
add_executable(sslenc ./testenc.c ${SSL_SRC})

target_link_libraries(tinyenc md5 aes)
target_link_libraries(sslenc ${OPENSSL_LIBRARIES} ${W32_LIBS})

# testpkt

add_executable(tinypkt ./testpkt.c ${TINY_SRC})
add_executable(sslpkt ./testpkt.c ${SSL_SRC})
add_executable(testsha ./testsha.c)

target_link_libraries(tinypkt ${W32_LIBS} md5 aes )
target_link_libraries(sslpkt ${OPENSSL_LIBRARIES} ${W32_LIBS})
target_link_libraries(testsha ${OPENSSL_LIBRARIES} ${W32_LIBS})



