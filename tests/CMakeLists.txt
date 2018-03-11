cmake_minimum_required(VERSION 3.5)
project(threadsafe_tests)

include_directories(${THREADSAFE_INCLUDE_DIRS})

add_executable(threadsafe_test_main test_map.cpp)

target_link_libraries(threadsafe_test_main threadsafe)