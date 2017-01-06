set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

cmake_minimum_required(VERSION 3.0.0)
project(oneup-gb-lr)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

#include(FindFFmpeg REQUIRED)

if (MSVC)
	set (CMAKE_CXX_FLAGS "/W3 /WX /EHsc")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -march=native")
else (MSVC)
    message(FATAL_ERROR "Don't know warning flags for compiler ${CMAKE_CXX_COMPILER_ID}")
endif (MSVC)

execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")

include_directories(${CMAKE_SOURCE_DIR})
#include_directories(${CMAKE_SOURCE_DIR} ${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS})
#add_definitions(${AVCODEC_DEFINITIONS} ${AVFORMAT_DEFINITIONS})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "CMAKE_BUILD_TYPE not defined, defaulting to Release")
endif(NOT CMAKE_BUILD_TYPE) 

set(LIBGBLR_SRC libretro.cc Audio.cc CPU.cc Joypad.cc LCD.cc Loader.cc
                LRConnector.cc Machine.cc Mapper.cc Serializer.cc Timer.cc)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DRELEASE -DNDEBUG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
else (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE")
endif (${CMAKE_BUILD_TYPE} STREQUAL "Release")

add_library(oneup-gb-lr SHARED ${LIBGBLR_SRC})

#target_link_libraries(oneup-gb-lr ${AVCODEC_LIBRARIES} ${AVFORMAT_LIBRARIES})