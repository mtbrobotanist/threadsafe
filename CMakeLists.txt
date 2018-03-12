cmake_minimum_required(VERSION 3.5)
project(threadsafe)

option(RUN_TESTS "Run tests" on)

set(CMAKE_CXX_STANDARD 11)

set(THREADSAFE_INCLUDE_DIRS include)
include_directories(${THREADSAFE_INCLUDE_DIRS})


set(THREADSAFE_SOURCES
        include/threadsafe/map.hpp
        include/threadsafe/math.hpp)


add_library(threadsafe ${THREADSAFE_SOURCES})

set_target_properties(threadsafe PROPERTIES LINKER_LANGUAGE CXX)


if(RUN_TESTS)
    add_subdirectory(vendor/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_subdirectory(tests)
endif()