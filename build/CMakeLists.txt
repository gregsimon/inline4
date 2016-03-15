cmake_minimum_required(VERSION 2.8.9)

# Use this plugin for precompiled headers.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

project (inline4)

include_directories(../v8/)
include_directories(../src)
include_directories(../v8/include)
include_directories(../duktape)

# V8 requires C++11
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set ( SOURCES
  ../src/inline4.cc
  
  ../duktape/duktape.c
)

add_executable(shell ${SOURCES} ../v8/samples/shell.cc)

add_executable(test ${SOURCES} ../src/test.cc)

set ( PROJECT_LINK_LIBS 
  pthread m
)

target_link_libraries(shell ${PROJECT_LINK_LIBS} )
target_link_libraries(test ${PROJECT_LINK_LIBS} )

