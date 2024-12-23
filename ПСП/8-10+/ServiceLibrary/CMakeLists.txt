cmake_minimum_required(VERSION 3.29)
project(ServiceLibrary)

set(CMAKE_CXX_STANDARD 20)


add_library(ServiceLibrary SHARED ServiceLibrary.cpp Errors.cpp)
target_link_libraries(ServiceLibrary ws2_32)