cmake_minimum_required(VERSION 3.5)
project(map_test)

include_directories(${THREADSAFE_INCLUDE_DIRS})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(map_test_single_threaded map_test_single_threaded.cpp)
target_link_libraries(map_test_single_threaded
        threadsafe
        gtest
        gtest_main)