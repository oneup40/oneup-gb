set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -march=native -std=c++14")

cmake_minimum_required(VERSION 3.0.0)
project(oneup-gb-lr)

include_directories(${CMAKE_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "CMAKE_BUILD_TYPE not defined, defaulting to Release")
endif(NOT CMAKE_BUILD_TYPE) 

set(LIBGBLR_SRC libretro.cc CPU.cc Machine.cc)

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
