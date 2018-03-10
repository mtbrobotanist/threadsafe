cmake_minimum_required(VERSION 3.5)
project(threadsafe_cpp)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)
set(THREADSAFE_CPP_SOURCES include/map.hpp)


add_library(threadsafe_cpp ${THREADSAFE_CPP_SOURCES})

SET_TARGET_PROPERTIES(threadsafe_cpp PROPERTIES LINKER_LANGUAGE CXX)
