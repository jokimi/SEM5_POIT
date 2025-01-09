cmake_minimum_required (VERSION 3.8)
project ("Linux" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)

add_library(mapping-d SHARED mapping-d.c)
set_target_properties(mapping-d PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(lab-08d lab-08d.c)
target_link_libraries(lab-08d mapping-d dl)