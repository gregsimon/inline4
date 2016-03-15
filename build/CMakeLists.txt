cmake_minimum_required(VERSION 2.8.9)

# Use this plugin for precompiled headers.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

project (inline4)

include_directories(../src)
include_directories(../v8/include)
include_directories(../duktape)

# V8 requires C++11
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set ( SOURCES
  ../src/inline4.cc
  ../src/test.cc
  ../duktape/duktape.c
)

add_executable(inline4 ${SOURCES})

set ( PROJECT_LINK_LIBS 
  pthread m
)

target_link_libraries(inline4 ${PROJECT_LINK_LIBS} )

