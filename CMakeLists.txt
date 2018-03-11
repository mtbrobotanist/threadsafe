cmake_minimum_required(VERSION 3.5)
project(threadsafe)

set(CMAKE_CXX_STANDARD 14)

set(THREADSAFE_INCLUDE_DIRS include)
include_directories(${THREADSAFE_INCLUDE_DIRS})


set(THREADSAFE_SOURCES
        include/threadsafe/map.hpp
        include/threadsafe/math.hpp)


add_library(threadsafe ${THREADSAFE_SOURCES})
add_executable(threadsafe_test_main tests/main.cpp)


target_link_libraries(threadsafe_test_main threadsafe)


set_target_properties(threadsafe PROPERTIES LINKER_LANGUAGE CXX)