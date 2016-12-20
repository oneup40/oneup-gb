set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -march=native -std=c++14")

cmake_minimum_required(VERSION 3.0.0)
project(oneup-gb-lr)

execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")

include_directories(${CMAKE_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "CMAKE_BUILD_TYPE not defined, defaulting to Release")
endif(NOT CMAKE_BUILD_TYPE) 

set(LIBGBLR_SRC libretro.cc CPU.cc Joypad.cc LCD.cc Loader.cc LRConnector.cc
                Machine.cc Mapper.cc Serializer.cc Timer.cc)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O3)
    add_definitions(-DRELEASE -DNDEBUG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-O0 -g)
    add_definitions(-DDEBUG)
else (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE")
endif (${CMAKE_BUILD_TYPE} STREQUAL "Release")

add_library(oneup-gb-lr SHARED ${LIBGBLR_SRC})
